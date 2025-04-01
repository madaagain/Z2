from typing import Any, Generic, Protocol, TypeVar, Union, final, overload, runtime_checkable

from .matrice import Matrix


T = TypeVar('T')
U = TypeVar('U')


@runtime_checkable
class SupportsAdditionAndGT(Protocol):
    def __add__(self: U, other: U) -> U:
        ...

    def __gt__(self: U, other: U) -> bool:
        ...


@runtime_checkable
class SupportsSubstraction(Protocol):
    def __sub__(self: U, other: U) -> U:
        ...


class Memory(Generic[T, U]):
    _data: dict[T, U]

    def __init__(self) -> None:
        self._data = {}

    @property
    def data(self) -> dict[T, U]:
        return self._data

    @data.setter
    def data(self, data: dict[T, U]):
        for name in data:
            self._data[name] = data[name]

    @overload
    def clear(self) -> None:
        """Clear all data
        """
        ...

    @overload
    def clear(self, key: T) -> None:
        """Clear the data of the given key

        Args:
            key (T): The key to clear the data from
        """

    def clear(self, key: T | None = None) -> None:
        if key is None:
            self._data = {}
        else:
            self._data.pop(key)

    def __getitem__(self, key: T) -> U:
        return self._data[key]

    def __setitem__(self, key: T, value: U) -> None:
        self._data[key] = value

    def __repr__(self) -> str:
        return repr(self.data)

    def __gt__(self, value: U) -> bool:
        """Compare with a specific value

        Args:
            value (U): The value to compare with, if 0, will ignore the negative values

        Raises:
            TypeError: U type does no support addition

        Returns:
            bool: if self > value
        """
        total: None | U = None
        for val in self.data.values():
            if not isinstance(val, SupportsAdditionAndGT):
                raise TypeError(f"Value {val} does not support addition")
            try:
                if value == 0 and val < 0:  # type: ignore
                    continue
            except ValueError:
                pass
            if total is None:
                total = val
            else:
                total += val
        if total is None:
            return False
        return total > value  # type: ignore

    def copy(self) -> 'Memory[T, U]':
        import copy
        mem = Memory[T, U]()
        mem.data = copy.deepcopy(self.data)
        return mem

    def __sub__(self, value: 'Memory[T, U]') -> 'Memory[T, U]':
        for val in self.data.values():
            if not isinstance(val, SupportsSubstraction):
                raise TypeError(f"Value {val} does not support substraction")
        for val in value.data.values():
            if not isinstance(val, SupportsSubstraction):
                raise TypeError(f"Value {val} does not support substraction")
        mem = self.copy()
        for i in value.data.keys():
            if i in mem.data:
                mem.data[i] -= value.data[i]  # type: ignore
            else:
                mem.data[i] = -value.data[i]  # type: ignore
        return mem


class Pos(Matrix):
    def __init__(self: 'Pos', *args: Any) -> None:
        """Make a position

        Args:
            x (int): The x position
            y (int): The y position
        """
        self._init = True
        super().__init__(1, 2, *args)
        self._init = False

    @property
    def x(self) -> int:
        """The x position

        Returns:
            int: The x position
        """
        return self.data[0]

    @property
    def y(self) -> int:
        """The y position

        Returns:
            int: The y position
        """
        return self.data[1]

    def __setattr__(self, name: str, value: Any) -> None:
        if getattr(self, '_initializing', True):
            return super().__setattr__(name, value)
        raise AttributeError("This class is immuable")

    def __hash__(self) -> int:
        return tuple(self.data).__hash__()

    def __int__(self) -> int:
        val = 0
        if self.x != 0:
            val += 1  # Tourner
        val += abs(self.x)
        if self.y < 0:
            val += 1 + (self.x != 0)  # Faire demi-tour ou tourner après avoir tourner plus haut
        val += abs(self.y)
        return val

    def __str__(self) -> str:
        return f"({self.x},{self.y})"

    def __repr__(self) -> str:
        return f"({self.x};{self.y})"


class GameObject():
    _pos: Pos

    def __init__(self, x: int = 0, y: int = 0) -> None:
        """New Game object

        Args:
            x (int): X position of the object. Defaults to 0
            y (int): Y position of the object. Defaults to 0
        """
        self._pos = Pos(x, y)

    def __hash__(self) -> int:
        return id(GameObject) + self._pos.__hash__()

    @property
    def x(self) -> int:
        """The x position of the GameObject

        Returns:
            int: The x position of the GameObject
        """
        return self._pos.x

    @property
    def y(self) -> int:
        """The y position of the GameObject

        Returns:
            int: The y position of the GameObject
        """
        return self._pos.y

    def __eq__(self, value: object) -> bool:
        ...

    def __str__(self) -> str:
        ...


@final
class Player(GameObject):
    def __init__(self, x: int = 0, y: int = 0) -> None:
        """New player

        Args:
            x (int): Position x of the player. Defaults to 0
            y (int): Position y of the player. Defaults to 0
        """
        super().__init__(x, y)

    def __hash__(self) -> int:
        return id(Player)

    def __eq__(self, value: object) -> bool:
        return isinstance(value, Player)

    def __str__(self) -> str:
        return "player"


@final
class Stone(GameObject):
    _name: str

    def __init__(self, name: str, x: int = 0, y: int = 0) -> None:
        """New stone

        Args:
            name (str): Name of the stone
            x (int): Position x of the stone. Defaults to 0
            y (int): Position y of the stone. Defaults to 0
        """
        super().__init__(x, y)
        self._name = name.lower()

    @property
    def name(self) -> str:
        return self._name

    @name.setter
    def name(self, name: str) -> None:
        self._name = name.lower()

    def __eq__(self, value: object) -> bool:
        return isinstance(value, Stone) and value.name == self.name

    def __hash__(self) -> int:
        return id(Stone) * 2 + self.name.__hash__()

    def __str__(self) -> str:
        return self.name


@final
class Food(GameObject):
    def __init__(self, x: int = 0, y: int = 0) -> None:
        """New food

        Args:
            x (int): Position x of the food. Defaults to 0
            y (int): Position y of the food. Defaults to 0
        """
        super().__init__(x, y)

    def __hash__(self) -> int:
        return id(Food)

    def __eq__(self, value: object) -> bool:
        return isinstance(value, Food)

    def __str__(self) -> str:
        return "food"


# An alias for all kinds of items
Item = Union[Stone, Food]


class Tile(Memory[str, list[GameObject]]):
    _x: int
    _y: int

    def __init__(self, x: int, y: int) -> None:
        """Make a new Tile

        Args:
            x (int): Position x of the tile
            y (int): Position y of the tile
        """
        super().__init__()
        self.data["items"] = []
        self.data["players"] = []
        self._x = x
        self._y = y

    @property
    def all(self) -> list[GameObject]:
        mem: list[GameObject] = self.data["items"]
        mem.extend(self.data["players"])
        return mem

    @property
    def x(self) -> int:
        """The x position of the Tile

        Returns:
            int: The x position of the Tile
        """
        return self._x

    @property
    def y(self) -> int:
        """The y position of the Tile

        Returns:
            int: The y position of the Tile
        """
        return self._y

    def add_player(self, *args: Any, **kwargs: Any) -> None:
        """Add a player to the tile

        Args:
            All arguments to give to the Player class
        """
        self.data["players"].append(Player(self._x, self._y, *args, **kwargs))

    def add_object(self, obj: GameObject) -> None:
        """Add an object to the tile

        Args:
            The object to add
        """
        self.data["items"].append(obj)
