#!/bin/python3.11
# Import main valus
from ZappyIAPackage.const import EXIT_FAILURE, EXIT_SUCCESS, HELP
# Import arguments setup
from ZappyIAPackage.const import MAX_ARGS, MIN_ARGS, TYPE_OF_ARGS as TYPE, OPTIONS_ARGS_AMOUNT
import ZappyIAPackage
# Import arguments
import sys


if __name__ == "__main__":
    argv: list[str] = sys.argv[1:]  # Don't care how you call the program
    k: int = 0
    constants: list[str] = []
    opt: str = ""
    is_opt: bool = False
    opt_amount: int = 0

    while k < len(argv):
        e: str = argv[k]
        if e == "--debug":
            ZappyIAPackage.const.DEBUG = True  # type: ignore
            argv.pop(k)
            continue
        if e == "--help" or e == "-?" or e == "/h" or e == "/help" or e == "/?":
            print(HELP)
            exit(EXIT_SUCCESS)
        if e.startswith("--"):
            is_opt = True
            opt = e
            opt_amount = 0
            ZappyIAPackage.const.OPTIONS[e] = []
            k += 1
            continue
        if e.startswith("-"):
            if len(e) == 2:
                is_opt = True
                opt = e[:2]
                opt_amount = 0
                ZappyIAPackage.const.OPTIONS[e[:2]] = []
            else:
                ZappyIAPackage.const.OPTIONS[e[:2]] = [e[2:]]
                is_opt = True
                opt = e[:2]
                opt_amount = 1
            k += 1
            continue
        if is_opt and OPTIONS_ARGS_AMOUNT[opt][1][1] > opt_amount:
            ZappyIAPackage.const.OPTIONS[opt].append(e)
            opt_amount += 1
            k += 1
            continue
        try:
            lenght: int = len(constants)
            for i in TYPE.__args__:
                try:
                    val = i(e)
                    assert str(val) == e
                    constants.append(val)
                    break
                except ValueError:
                    pass
                except AssertionError:
                    pass
                except TypeError:
                    pass
            if len(constants) == lenght:
                TYPE.__args__[0](e)
        except ValueError as err:
            if ZappyIAPackage.const.DEBUG:
                raise err
            print(repr(err), file=sys.stderr)
            exit(EXIT_FAILURE)
        k += 1

    try:
        for e in OPTIONS_ARGS_AMOUNT:
            if e not in ZappyIAPackage.const.OPTIONS:
                if OPTIONS_ARGS_AMOUNT[e][0]:
                    raise KeyError(f"The {e} flag must be given with "
                                   f"{OPTIONS_ARGS_AMOUNT[e][1][0]} to {OPTIONS_ARGS_AMOUNT[e][1][1]} arguments.")
                continue
            if len(ZappyIAPackage.const.OPTIONS[e]) < OPTIONS_ARGS_AMOUNT[e][1][0]:
                raise IndexError(f"Not enough arguments for flag {e}. "
                                 f"You gave {len(ZappyIAPackage.const.OPTIONS[e])} arguments. "
                                 f"The flag takes {OPTIONS_ARGS_AMOUNT[e][1][0]} to {OPTIONS_ARGS_AMOUNT[e][1][1]} arguments.")
        if len(constants) > MAX_ARGS:
            raise IndexError(f"{MAX_ARGS} non-flag arguments at most must be given.")
        if len(constants) < MIN_ARGS:
            raise IndexError(f"{MIN_ARGS} non-flag arguments at least must be given.")
    except IndexError as err:
        if ZappyIAPackage.const.DEBUG:
            raise err
        print(repr(err), file=sys.stderr)
        exit(EXIT_FAILURE)
    except KeyError as err:
        if ZappyIAPackage.const.DEBUG:
            raise err
        print(repr(err), file=sys.stderr)
        exit(EXIT_FAILURE)
    try:
        # Actually we do care what you called the program
        if ZappyIAPackage.const.DEBUG:
            print(f"Options= {ZappyIAPackage.const.OPTIONS}")
            print(f"{constants= }")
        ZappyIAPackage.main(constants)
    except BaseException as err:
        if ZappyIAPackage.const.DEBUG:
            raise err
        print(repr(err), file=sys.stderr)
        exit(EXIT_FAILURE)
    exit(EXIT_SUCCESS)
