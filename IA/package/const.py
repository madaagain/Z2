import types
from .memory import GameObject, Stone, Food, Player, Memory


INCANT_LEVELS: Memory[int, Memory[GameObject, int]] = Memory[int, Memory[GameObject, int]]()

INCANT_LEVELS[1] = Memory[GameObject, int]()
INCANT_LEVELS[1][Player()] = 1
INCANT_LEVELS[1][Stone("linemate")] = 1
INCANT_LEVELS[1][Stone("deraumere")] = 0
INCANT_LEVELS[1][Stone("sibur")] = 0
INCANT_LEVELS[1][Stone("mendiane")] = 0
INCANT_LEVELS[1][Stone("phiras")] = 0
INCANT_LEVELS[1][Stone("thystame")] = 0
INCANT_LEVELS[1][Food()] = 10

INCANT_LEVELS[2] = Memory[GameObject, int]()
INCANT_LEVELS[2][Player()] = 2
INCANT_LEVELS[2][Stone("linemate")] = 1
INCANT_LEVELS[2][Stone("deraumere")] = 1
INCANT_LEVELS[2][Stone("sibur")] = 1
INCANT_LEVELS[2][Stone("mendiane")] = 0
INCANT_LEVELS[2][Stone("phiras")] = 0
INCANT_LEVELS[2][Stone("thystame")] = 0
INCANT_LEVELS[2][Food()] = 10

INCANT_LEVELS[3] = Memory[GameObject, int]()
INCANT_LEVELS[3][Player()] = 2
INCANT_LEVELS[3][Stone("linemate")] = 2
INCANT_LEVELS[3][Stone("deraumere")] = 0
INCANT_LEVELS[3][Stone("sibur")] = 1
INCANT_LEVELS[3][Stone("mendiane")] = 0
INCANT_LEVELS[3][Stone("phiras")] = 2
INCANT_LEVELS[3][Stone("thystame")] = 0
INCANT_LEVELS[3][Food()] = 10

INCANT_LEVELS[4] = Memory[GameObject, int]()
INCANT_LEVELS[4][Player()] = 4
INCANT_LEVELS[4][Stone("linemate")] = 1
INCANT_LEVELS[4][Stone("deraumere")] = 1
INCANT_LEVELS[4][Stone("sibur")] = 2
INCANT_LEVELS[4][Stone("mendiane")] = 0
INCANT_LEVELS[4][Stone("phiras")] = 1
INCANT_LEVELS[4][Stone("thystame")] = 0
INCANT_LEVELS[4][Food()] = 10

INCANT_LEVELS[5] = Memory[GameObject, int]()
INCANT_LEVELS[5][Player()] = 4
INCANT_LEVELS[5][Stone("linemate")] = 1
INCANT_LEVELS[5][Stone("deraumere")] = 2
INCANT_LEVELS[5][Stone("sibur")] = 1
INCANT_LEVELS[5][Stone("mendiane")] = 3
INCANT_LEVELS[5][Stone("phiras")] = 0
INCANT_LEVELS[5][Stone("thystame")] = 0
INCANT_LEVELS[5][Food()] = 10

INCANT_LEVELS[6] = Memory[GameObject, int]()
INCANT_LEVELS[6][Player()] = 6
INCANT_LEVELS[6][Stone("linemate")] = 1
INCANT_LEVELS[6][Stone("deraumere")] = 2
INCANT_LEVELS[6][Stone("sibur")] = 3
INCANT_LEVELS[6][Stone("mendiane")] = 0
INCANT_LEVELS[6][Stone("phiras")] = 1
INCANT_LEVELS[6][Stone("thystame")] = 0
INCANT_LEVELS[6][Food()] = 10

INCANT_LEVELS[7] = Memory[GameObject, int]()
INCANT_LEVELS[7][Player()] = 6
INCANT_LEVELS[7][Stone("linemate")] = 2
INCANT_LEVELS[7][Stone("deraumere")] = 2
INCANT_LEVELS[7][Stone("sibur")] = 2
INCANT_LEVELS[7][Stone("mendiane")] = 2
INCANT_LEVELS[7][Stone("phiras")] = 2
INCANT_LEVELS[7][Stone("thystame")] = 1
INCANT_LEVELS[7][Food()] = 10


STARTING_LEVEL: int = 1
OPTIONS: dict[str, list[str]] = {}
OPTIONS_ARGS_AMOUNT: dict[str, tuple[bool, tuple[int, int]]] = {
    "-p": (False, (1, 1)),
    "-n": (False, (1, 1)),
    "-h": (False, (1, 1)),
    "--assure-at-least-n-places": (False, (1, 1)),
    "--auto-fork": (False, (0, 1)),
    # "--cheat": (False, (0, 0))
}

# Strings representing TRUE/FALSE
# LANGUAGES PRESENT:
#   - En
#   - Fr
TRUE: list[str] = ["true", "vrai", "t", "v", "y", "o", "yes", "oui", "1"]
FALSE: list[str] = ["false", "faux", "f", "n", "no", "non", "0"]


def make_bool(str: str) -> bool:
    """A function to parse an argument string as a bool correctly.
    Uses a table of words for True and False statements. False has priority.

    Args:
        str (str): The string to be parsed

    Raises:
        ValueError: String is not found in the tables and therefor don't represent a valid boolean

    Returns:
        bool: The boolean it represents
    """
    if str.lower() not in TRUE and str.lower() not in FALSE:
        raise ValueError(f"{str=} is not a string representing a boolean")
    if str.lower() in FALSE:
        return False
    if str.lower() in TRUE:
        return True
    raise ValueError(f"{str=} disappeared from the boolean word tables")


DEBUG: bool = False
EXIT_SUCCESS: int = 0
EXIT_FAILURE: int = 84
HELP: str = ("""USAGE
    ./zappy_ai [-h <machine>] -p <port> -n <name>
OPTIONS
    -h <machine>\tName/Ip of the machine to connect to (default localhost)
    -p <port>\tPort to which to connect on the machine
    -n <name>\tName of the team to join
    --assure-at-least-n-places <n>\tAssure that at least n slots are free on the team
    --auto-fork [True|False]\tAutomatically creates a new AI to run when the fork command is run """
             """(does not apply when it is run to have enough free slots in the team). Defaults to False,
""")
MAX_ARGS: int = 0
MIN_ARGS: int = 0
TYPE_OF_ARGS: types.UnionType = int | str
