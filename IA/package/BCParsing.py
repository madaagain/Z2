from .memory import GameObject, Player, Stone, Memory


def BroadCast_Parser(dic: Memory[GameObject, int], level: int) -> str:
    message = "Hey I need "
    items: list[str] = []
    players_needed = 0

    for gameobject, quantity in dic.data.items():
        if isinstance(gameobject, Stone) and quantity > 0:
            items.append(f"{quantity} {gameobject.name}")
        elif isinstance(gameobject, Player):
            players_needed = quantity

    if items or players_needed > 0:
        if items:
            message += ", ".join(items)
            if players_needed > 0:
                message += f" and {players_needed} players"
        elif players_needed > 0:
            message += f"{players_needed} players"
        message += f" to incant to level {level + 1}"
    else:
        print(f"No additional resources needed for level {level} incantation")
    return message
