from sys import stderr
from typing import Never, Sequence
import socket
from .const import OPTIONS, make_bool
from . import AI


def run_AI(connection: socket.socket) -> Never:
    """Run an AI and raises an exception when it ends. The socket maight be closed.

    Args:
        connection (socket.socket): The socket the AI acts on

    Raises:
        any exception on unrecoverable error
    """
    IOConnectionFile = connection.makefile("rw", encoding='utf-8')
    assert "WELCOME\n" == IOConnectionFile.readline()
    IOConnectionFile.write(OPTIONS["-n"][0] + "\n")
    IOConnectionFile.flush()
    ai = AI.AI(IOConnectionFile, connection)
    while True:
        try:
            ai.update()
            ai.inventory()
            ai.look()
            if ai.has_food(10):
                if ai.can_incant():
                    if ai.has_food(10):
                        ai.incant()
                    else:
                        ai.get_food()
                else:
                    ai.get_stone()
            else:
                ai.get_food()
        except SyntaxError as err:
            print(repr(err), file=stderr)
        except KeyError as err:
            print(repr(err), file=stderr)
        except AssertionError as err:
            print(repr(err), file=stderr)


def main(const: Sequence[int | str]):
    if const:
        raise ValueError(f"Parsing of {const} failed")
    if "-h" not in OPTIONS.keys():
        OPTIONS["-h"] = ["localhost"]
    if "-p" not in OPTIONS.keys():
        OPTIONS["-p"] = ["4242"]
    if "-n" not in OPTIONS.keys():
        OPTIONS["-n"] = ["Team1"]
    if "--assure-at-least-n-places" not in OPTIONS.keys():
        OPTIONS["--assure-at-least-n-places"] = ["0"]
    int(OPTIONS["--assure-at-least-n-places"][0])
    if "--auto-fork" not in OPTIONS.keys():
        OPTIONS["--auto-fork"] = ["False"]
    if len(OPTIONS["--auto-fork"]) < 1:
        OPTIONS["--auto-fork"] = ["True"]
    for fork in OPTIONS["--auto-fork"]:
        make_bool(fork)
    OPTIONS["--auto-fork"] = [OPTIONS["--auto-fork"][-1]]
    connection = socket.create_connection((OPTIONS["-h"][0], int(OPTIONS["-p"][0])))
    try:
        run_AI(connection)
    except SystemExit:
        pass
    finally:
        try:
            connection.close()
        except BaseException:
            pass
