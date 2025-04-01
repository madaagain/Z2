from io import TextIOWrapper
import select
import socket
from sys import stderr
import time
import traceback
from typing import overload
from .BCParsing import BroadCast_Parser
from .const import INCANT_LEVELS, STARTING_LEVEL, OPTIONS, make_bool
from .memory import GameObject, Memory, Player, Pos, Tile, Item, Food, Stone
from . import const

NANOSEC_PER_SEC: float = float(1e+09)


def debug(*values: object, sep: str | None = " ", end: str | None = "\n") -> None:
    if not const.DEBUG:
        return
    return print(*values, sep=sep, end=end, file=stderr, flush=True)


def make_item(string: str) -> Item:
    """Make a singular item

    Args:
        string (str): the item as a string

    Returns:
        Item: The item made
    """
    if string.lower() == "food":
        return Food()
    return Stone(string.lower())


def make_obj_list(string: str, x: int, y: int) -> Tile:
    """Make a Tile of object from a string

    Args:
        string (str): The string to make the objects out of
        x (int): The position of the Tile
        y (int): The position of the Tile

    Returns:
        Tile: _description_
    """
    t = Tile(x, y)
    for i in string.split():
        if i.lower() == "player":
            t.add_player()
            continue
        if i.lower() == "food":
            t.add_object(Food(x=x, y=y))
            continue
        t.add_object(Stone(i.lower(), x=x, y=y))
    return t


class AI():
    """Class for everything the AI need

    Raises:
        EndException if not custom exception was given for death
        SyntaxError: Command failed
        AssertionError: Some invalid values are found by the AI
        Any error from socket if an error happens
    """
    _world: Memory[Pos, Tile]
    _connection: TextIOWrapper
    _connection_socket: socket.socket
    _inventory: Memory[Item, int]
    _team_empty_places: int
    _map_size: tuple[int, int]
    _level: int
    _missing: Memory[GameObject, int]
    _speed: float
    _last_fork: int

    class EndException(BaseException):
        """Exception indicating the end of the AI
        """
        pass

    _death: EndException | BaseException

    @overload
    def __init__(self, connection: TextIOWrapper, connection_socket: socket.socket) -> None:
        """Create The AI class

        Args:
            connection (TextIOWrapper): The file to write/read to/from that's connected to the socket
            connection_socket (socket.socket): The socket to interact with for select
        """
        ...

    @overload
    def __init__(self, connection: TextIOWrapper, connection_socket: socket.socket,
                 end_exception: BaseException) -> None:
        """Create The AI class

        Args:
            connection (TextIOWrapper): The file to write/read to/from that's connected to the socket
            connection_socket (socket.socket): The socket to interact with for select
            end_exception (BaseException): The exception to raise on death
        """
        ...

    def __init__(self, connection: TextIOWrapper, connection_socket: socket.socket,
                 end_exception: BaseException = EndException()) -> None:
        """Create The AI class

        Args:
            connection (TextIOWrapper): The file to write/read to/from that's connected to the socket
            connection_socket (socket.socket): The socket to interact with for select
            end_exception (BaseException, optional): The exception to raise on death. Defaults to EndException()
        """
        self._team_empty_places = int(connection.readline())
        line = connection.readline().split()
        assert len(line) == 2
        self._map_size = tuple(map(int, line))  # type: ignore
        self._level = STARTING_LEVEL
        self._connection = connection
        self._connection_socket = connection_socket
        self._death = end_exception
        self._world = Memory[Pos, Tile]()
        self._inventory = Memory[Item, int]()
        self._speed = 0
        self._missing = Memory[GameObject, int]()
        self._missing[GameObject()] = 1
        self._last_fork = time.time_ns()
        self._last_broad = 0
        print(f"AI succesfully created at address {id(self)}")

    def _readlines(self, block: bool = False) -> list[str]:
        """Redefine the readlines for the connection because it blocks even after recieving

        Args:
            block (bool, optional): Wether it should block. Defaults to False.

        Returns:
            list[str]: The lines that were read
        """
        r, _, e = select.select([self._connection_socket], [], [self._connection_socket], None if block else 0)
        if e:
            raise ConnectionAbortedError("Connection with the server does not exist anymore")
        if len(r) == 0:
            return []
        buffer: list[str] = []
        while True:
            data: bytes = self._connection_socket.recv(4096)
            buffer.append(data.decode())
            if data.endswith(b"\n"):
                break

        response = ''.join(buffer)
        debug("read", response)
        self._parse_all(response.splitlines())
        return response.splitlines()

    @overload
    def _command(self, command: str) -> list[str]:
        """Runs a command and returns the result

        Args:
            command (str): The command to run

        Returns:
            list[str]: The result gotten from the server as a list of string
        """
        ...

    @overload
    def _command(self, command: str, *args: str) -> list[str]:
        """Runs a command with arguments and returns the result

        Args:
            command (str): The command to run
            args (str): The arguments

        Returns:
            list[str]: The result gotten from the server as a list of string
        """
        ...

    @overload
    def _command(self, command: str, *, block: bool) -> list[str]:
        """Runs a command and returns the result

        Args:
            command (str): The command to run
            block (bool): Whether it should block and wait for an anwser from the server

        Returns:
            list[str]: The result gotten from the server as a list of string
        """
        ...

    @overload
    def _command(self, command: str, *args: str, block: bool) -> list[str]:
        """Runs a command with arguments and returns the result

        Args:
            command (str): The command to run
            args (str): The arguments
            block (bool): Whether it should block and wait for an anwser from the server

        Returns:
            list[str]: The result gotten from the server as a list of string
        """
        ...

    def _command(self, command: str, *args: str, block: bool = True) -> list[str]:
        new_args: list[str] = list(args)
        for i in range(len(new_args)):
            new_args[i] = new_args[i].replace("\n", "\\n")
        command = command.removesuffix("\n")
        debug("Sending command:", command.capitalize() + (" " if len(new_args) else "") + " ".join(new_args) + "\\n\"",
              sep="\"", end=None)
        self._connection.write(command.capitalize() + (" " if len(new_args) else "") + " ".join(new_args) + "\n")
        self._connection.flush()

        lines = self._readlines(block=block)
        for i in range(len(lines)):
            lines[i] = lines[i].removesuffix("\n").removesuffix("\r")
        self._parse_all(lines)
        return lines

    def update(self) -> None:
        """Updates the number of unused places in a team using the Connect_nbr command
        """
        test: bool = False
        need_players: bool = False
        for i in self._missing.data:
            if self._missing.data[i] > 0:
                if i == Player():
                    need_players = True
                else:
                    test = True
        if need_players and not test and make_bool(OPTIONS["--auto-fork"][-1]):
            self.fork()
        if self._level == 8:
            self._connection_socket.close()
            from .win import root
            root.destroy()
            exit()
        self._parse_all()
        list = self._command("Connect_nbr")
        self._parse_all(list)
        for i in list:
            try:
                self._team_empty_places = int(i)
                break
            except ValueError:
                pass
        if Food() not in self._inventory.data:
            return
        if self._inventory.data[Food()] < 5:
            return
        if self._team_empty_places < int(OPTIONS["--assure-at-least-n-places"][0]):
            self.fork(disable_true_fork=True)

    def look(self) -> None:
        """Runs the look command
        """
        self._parse_all()
        lines = self._command("Look")
        view: str | None = None
        while view is None:
            for i in lines:
                if i.startswith("[") and i.endswith("]"):
                    view = i.removeprefix("[").removesuffix("]")
                    break
                if i.startswith("[") or i.endswith("]"):
                    raise SyntaxError(f"Unknown return string: {i}")
            if view is None:
                lines = self._readlines()
        part: list[str] = view.split(",")
        assert len(part) >= (self._level + 1) ** 2
        del self._world
        self._world = Memory[Pos, Tile]()
        for y in range(self._level + 1):
            for x in range(-y, y + 1):
                self._world[Pos(x, y)] = make_obj_list(part[0], x, y)
                part = part[1:]
        debug(self._world)

    def inventory(self) -> None:
        """Runs the inventory command and updates the inventory accordinly
        """
        self._parse_all()
        t = time.time_ns()
        lines = self._command("Inventory")
        nt = time.time_ns()
        inv: str | None = None
        while inv is None:
            for i in lines:
                if i.startswith("[") and i.endswith("]"):
                    inv = i.removeprefix("[").removesuffix("]")
                    break
                if i.startswith("[") or i.endswith("]"):
                    raise SyntaxError(f"Unknown return string: {i}")
            if inv is None:
                lines = self._readlines()
        items = inv.split(",")
        self._inventory.clear()
        for e in items:
            item = make_item(e.split()[0])
            self._inventory[item] = int(e.split()[1])
        self._speed = float(nt - t) / NANOSEC_PER_SEC
        debug(f"{self._inventory=}")
        debug(f"{self._speed=}")

    def incant(self) -> None:
        """Starts an incantation
        """
        self._parse_all()
        assert self.has_food(2)
        assert self.can_incant()
        print("Preparing incantation")
        self.prepare_incant()
        self.update()
        self.look()
        here: Memory[GameObject, int] = Memory[GameObject, int]()
        for i in self._world[Pos(0, 0)].all:
            if i in here.data:
                here.data[i] += 1
            else:
                here.data[i] = 1
        self._missing = INCANT_LEVELS[self._level] - here
        if Food() in self._missing.data:
            self._missing[Food()] = 0
        lvl: int = self._level
        test: bool = True
        for i in self._missing.data.values():
            if i > 0:
                time.sleep(10 * self._speed)
                test = False
                if not self.has_food(2):
                    return
        print("Incantation ready.",
              "This AI will call the incantation command." if test
              else "This Ai will wait for another to call the incantation.")
        if test:
            lines = self._command("Incantation")
            self._parse_all(lines)
        else:
            lines = self._readlines()
            if not lines:
                return
        test: bool = False
        for i in lines:
            if i.capitalize() == "Elevation underway".capitalize():
                test = True
        if not test:
            time.sleep(10 * self._speed)
            lines = self._readlines()
            for i in lines:
                if i.capitalize() == "Elevation underway".capitalize():
                    test = True
        if not test:
            raise SyntaxError("Cannot start elevation")
        print("Elevation underway received")
        string: str = self._connection.readline()
        while not string.capitalize().startswith("Current level: ".capitalize()):
            self._parse_all([string])
            string = self._connection.readline()
        string = string.capitalize().removeprefix("Current level: ".capitalize())
        self._level = int(string)
        print("Incantation", "succeded" if self._level != lvl else "failed")
        print(f"New level: {self._level}")

    def incantation(self) -> None:
        """Starts an incantation
        """
        return self.incant()

    def forward(self) -> None:
        """Make the player go forward
        """
        self._parse_all()
        lines = self._command("Forward")
        self._test_ok(lines)
        new_world: Memory[Pos, Tile] = Memory[Pos, Tile]()
        for i in self._world.data:
            new_pos = Pos(i.x, i.y - 1)
            new_world[new_pos] = self._world[i]
        if not Pos(0, 0) in new_world.data:
            new_world[Pos(0, 0)] = Tile(0, 0)
        self._world = new_world

    def turn_left(self) -> None:
        """Turn left
        """
        self._parse_all()
        lines = self._command("Left")
        self._test_ok(lines)
        new_world: Memory[Pos, Tile] = Memory[Pos, Tile]()
        for i in self._world.data:
            new_pos = Pos(-i.y, i.x)
            new_world[new_pos] = self._world[i]
        if not Pos(0, 0) in new_world.data:
            new_world[Pos(0, 0)] = Tile(0, 0)
        self._world = new_world

    def turn_right(self) -> None:
        """Turn right
        """
        self._parse_all()
        lines = self._command("Right")
        self._test_ok(lines)
        new_world: Memory[Pos, Tile] = Memory[Pos, Tile]()
        for i in self._world.data:
            new_pos = Pos(i.y, -i.x)
            new_world[new_pos] = self._world[i]
        if not Pos(0, 0) in new_world.data:
            new_world[Pos(0, 0)] = Tile(0, 0)
        self._world = new_world

    @overload
    def fork(self) -> None:
        """Fork to add a new slot and a new player
        """
        ...

    @overload
    def fork(self, disable_true_fork: bool) -> None:
        """Fork to add a new slot

        Args:
            disable_true_fork (bool): Whether the program should avoid truly forkind and adding a new player
        """
        ...

    def fork(self, disable_true_fork: bool = False) -> None:
        """Fork to add a new slot

        Args:
            disable_true_fork (bool): Whether the program should avoid truly forkind and adding a new player
        """
        assert self.has_food(3)
        debug(f"time since last fork: {(time.time_ns() - self._last_fork) / NANOSEC_PER_SEC}")
        debug(f"min time between fork: {self._speed * 3000}")
        if (time.time_ns() - self._last_fork) / NANOSEC_PER_SEC < self._speed * 3000:
            return
        self._last_fork = time.time_ns()
        self._parse_all()
        lines = self._command("Fork")
        self._test_ok(lines)
        if disable_true_fork:
            return
        if make_bool(OPTIONS["--auto-fork"][0]):
            import subprocess
            import sys
            debug("Forking...")
            subprocess.Popen(sys.argv.copy())

    def eject(self) -> None:
        """Eject players from the cell (idk why it would be used if not for PvP)
        """
        self._parse_all()
        lines = self._command("Eject")
        self._test_ok(lines)

    @overload
    def _parse_all(self) -> None:
        """Parsing all commands results using select
        """
        ...

    @overload
    def _parse_all(self, lines: list[str]) -> None:
        """Parsing all commands results

        Args:
            lines (list[str]): The lines to parse.
        """
        ...

    def _parse_all(self, lines: list[str] | None = None) -> None:
        """Parsing all commands results using select if lines is not given

        Args:
            lines (list[str] | None, optional): The lines to parse. Defaults to None.
        """
        if not lines:
            r, _, e = select.select([self._connection_socket], [], [self._connection_socket], 0)
            if e:
                raise ConnectionAbortedError("Connection with the server does not exist anymore")
            if len(r) == 0:
                return
            lines = self._readlines()
        for line in lines:
            line = line.removesuffix("\n")
            debug(f"{line=}")
            if line == "ko":
                continue
            if line == "ok":
                continue
            if line == "dead":
                raise self._death
            if line.startswith("message "):
                line = line.removeprefix("message ")
                direction: int = int(line.split(",")[0])
                message = ",".join(line.split(",")[1:]).strip()
                for i in self._missing.data:
                    if i != Player():
                        return
                if direction == 1:
                    self.forward()
                if direction == 2:
                    self.forward()
                    self.turn_right()
                    self.forward()
                if direction == 3:
                    self.turn_right()
                    self.forward()
                if direction == 4:
                    self.turn_right()
                    self.forward()
                    self.turn_right()
                    self.forward()
                if direction == 5:
                    self.turn_right()
                    self.turn_right()
                    self.forward()
                if direction == 6:
                    self.turn_left()
                    self.forward()
                    self.turn_left()
                    self.forward()
                if direction == 7:
                    self.turn_left()
                    self.forward()
                if direction == 8:
                    self.forward()
                    self.turn_left()
                    self.forward()

                # @brief Appel à process_broadcast si le message commence par "Hey I need"
                if message.startswith("Hey I need"):
                    response: str = self.process_broadcast(message)
                    print(f"Response to direction {direction}: {response}")

    def _test_ok(self, lines: list[str]) -> None:
        """Test if at least 1 "ok" was found

        Args:
            lines (list[str]): the lines to parse

        Raises:
            SyntaxError: No "ok" was found
        """
        while True:
            self._parse_all(lines)
            for i in lines:
                if i == "ok":
                    return
                if i == "ko":
                    raise SyntaxError(f"Command {traceback.extract_stack()[-2].name} failed with: {lines=}")
            lines = self._readlines(True)

    def has_food(self, amount: int = 1) -> bool:
        """Test is there is enough food

        Args:
            amount (int, optional): The amount of food to have. Defaults to 1.

        Returns:
            bool: returns if there's enough food in it
        """
        return self._inventory[Food()] >= amount

    def can_incant(self) -> bool:
        """Calculate if an incantation can be done

        Returns:
            bool: Whether and incantation can be done
        """
        self.look()
        self.update()
        self.inventory()
        here: Memory[GameObject, int] = self._inventory.copy()  # type: ignore
        for i in self._world[Pos(0, 0)].all:
            if i in here.data:
                here.data[i] += 1
            else:
                here.data[i] = 1
        self._missing = INCANT_LEVELS[self._level] - here
        debug(f"{self._missing}")
        return not self._missing > 0

    def prepare_incant(self) -> None:
        """Place items on the ground for the incantation
        """
        here: Memory[GameObject, int] = Memory[GameObject, int]()
        for i in self._world[Pos(0, 0)].all:
            if i in here.data:
                here.data[i] += 1
            else:
                here.data[i] = 1
        missing = INCANT_LEVELS[self._level] - here
        for i in missing.data:
            if i in self._inventory.data:
                for _ in range(min(self._inventory[i], missing[i])):
                    self.set(i)

    def get_stone(self) -> None:
        """Broadcast what's missing and get it
        """
        if self._missing > 0:
            if self._last_broad == 0:
                self._command("broadcast", BroadCast_Parser(self._missing, self._level), block=True)
            self._last_broad += 1
            self._last_broad %= 10
        else:
            self._last_broad = 0
        self.update()
        stone_wanted: bool = False
        for obj, i in self._missing.data.items():
            if i <= 0:
                continue
            if isinstance(obj, Stone):
                stone_wanted = True
                break
        if not stone_wanted:
            return
        liste: list[Pos] = []
        minim: int = 0
        for i in self._world.data:
            for j in self._world.data[i].all:
                for k in self._missing.data:
                    if j == k:
                        liste.append(i)
        if len(liste) == 0:
            self.forward()
            raise SyntaxError("Cannot find the wanted stones, moved forwards")
        for i, e in enumerate(liste):
            if float(int(e)) / float(liste.count(e)) < float(int(liste[minim])) / float(liste.count(liste[minim])):
                minim = i
        dir: Pos = liste[minim]
        if dir.y > 0:
            debug(f"Move forward {dir.y}")
            for i in range(dir.y):
                self.forward()
                for i in self._world[Pos(0, 0)].all:
                    if isinstance(i, Stone):
                        self.take(i)
                    if isinstance(i, Food):
                        self.take(i)
        if dir.x > 0:
            self.turn_right()
        if dir.x < 0:
            self.turn_left()
        debug(f"Move to the side {abs(dir.x)}")
        for i in range(abs(dir.x)):
            self.forward()
            for i in self._world[Pos(0, 0)].all:
                if isinstance(i, Stone):
                    self.take(i)
                if isinstance(i, Food):
                    self.take(i)
        if dir.y >= 0:
            return
        if dir.x > 0:
            self.turn_right()
        if dir.x < 0:
            self.turn_left()
        debug(f"move backwards {-dir.y}")
        for i in range(-dir.y):
            self.forward()
            for i in self._world[Pos(0, 0)].all:
                if isinstance(i, Stone):
                    self.take(i)
                if isinstance(i, Food):
                    self.take(i)
        pass

    def get_food(self) -> None:
        """Search for food
        """
        liste: list[Pos] = []
        minim: int = 0
        for i in self._world.data:
            for j in self._world.data[i].all:
                if j == Food():
                    liste.append(i)
                    break
        debug(f"liste of food tiles {liste}")
        if len(liste) == 0:
            self.forward()
            raise SyntaxError("Cannot find food, moved forwards")
        for i, e in enumerate(liste):
            if int(e) < int(liste[minim]):
                minim = i
        dir: Pos = liste[minim]
        try:
            self.take(Food())
        except SyntaxError:
            pass
        self.update()
        if dir.y > 0:
            debug(f"Move forward {dir.y}")
            for i in range(dir.y):
                self.forward()
                try:
                    self.take(Food())
                except SyntaxError:
                    pass
                if self._inventory[Food()] > 3:
                    for i in self._world[Pos(0, 0)].all:
                        if isinstance(i, Stone):
                            self.take(i)
        self.update()
        if dir.x > 0:
            self.turn_right()
        if dir.x < 0:
            self.turn_left()
        debug(f"Move to the side {abs(dir.x)}")
        for i in range(abs(dir.x)):
            self.forward()
            try:
                self.take(Food())
            except SyntaxError:
                pass
            if self._inventory[Food()] > 3:
                for i in self._world[Pos(0, 0)].all:
                    if isinstance(i, Stone):
                        self.take(i)
        self.update()
        if dir.y >= 0:
            return
        if dir.x > 0:
            self.turn_right()
        if dir.x < 0:
            self.turn_left()
        debug(f"move backwards {-dir.y}")
        for i in range(-dir.y):
            self.forward()
            try:
                self.take(Food())
            except SyntaxError:
                pass
            if self._inventory[Food()] > 3:
                for i in self._world[Pos(0, 0)].all:
                    if isinstance(i, Stone):
                        self.take(i)
        self.update()

    def take(self, object: Item) -> None:
        test: bool = False
        for i in self._world[Pos(0, 0)].all:
            if i == object:
                test = True
        if not test:
            raise SyntaxError("Cannot find the object on the tile")
        lines = self._command(f"take {object}")
        self._test_ok(lines)
        if object in self._inventory.data:
            self._inventory[object] += 1
        else:
            self._inventory[object] = 1

    def set(self, object: Item) -> None:
        if object == Food():
            return
        if object not in self._inventory.data:
            raise SyntaxError("Trying to set an pbject not in inventory")
        lines = self._command(f"set {object}")
        self._test_ok(lines)
        self._inventory[object] -= 1

    def parse_broadcast_message(self, message: str) -> dict[GameObject, int]:
        resources: dict[GameObject, int] = {}
        parts = message.split("need ")[1].split(" to incant")[0].split(", ")
        for part in parts:
            quantity, name = part.split(" ", 1)
            if name == "players":
                resources[Player()] = int(quantity)
            else:
                resources[Stone(name)] = int(quantity)
        return resources

    def process_broadcast(self, broadcast_message: str) -> str:

        requested_resources = self.parse_broadcast_message(broadcast_message)

        response = "I have "
        available_resources: list[str] = []

        for resource, quantity in requested_resources.items():
            if quantity <= 0:
                continue
            if isinstance(resource, Stone):
                if resource in self._missing.data and self._missing.data[resource] < 0:

                    available = abs(self._missing.data[resource])
                    if available > 0:
                        available_resources.append(f"{available} {resource.name}")
                elif resource in self._inventory.data:

                    available = self._inventory.data[resource]
                    if available > 0:
                        available_resources.append(f"{available} {resource.name}")
            elif resource == Player():
                if self._team_empty_places > 0:
                    available_resources.append(f"{self._team_empty_places} players")

        if available_resources:
            response += ", ".join(available_resources)
        else:
            response = "I don't have any requested resources"

        return response
