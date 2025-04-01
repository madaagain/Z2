# Zappy Server

The Zappy's server is the core of the project.

The goal of the server is to manage the game and the communication between the clients, which can be either AI players or graphical clients.

## Configuration

When launching the server, you can specify different options.

You can retrieve all of them using the following syntax:

```bash
./zappy_server -help
```

#### Table of contents

- [Connectivity](#connectivity)
- [Map size](#map-size)
- [Teams](#teams)
- [Teams capacity](#teams-capacity)
- [Frequency](#frequency)
- [Immortal mode](#immortal-mode)
- [Debug mode](#debug-mode)
- [Custom seed](#custom-seed)

### Connectivity

You can use the `-p` (`--port`) option to define a specific port for the server.

> :bulb: The default port is 4242.

> :warning: The port must be between 1 and 65535, and you may need root privileges to use a port below 1024.

### Map size

The map size can be edited using the `-x` (``--width``) and `-y` (``--height``) options.

> :bulb: The default map size is 10x10.

> :wrench: The minimum map size is **1x1**, and the maximum map size is **100x100**.

### Teams

You can define the teams that will be available in the game using the `-n` (`--names`) option.

To use it, you have to chain the names of the teams, separated by a space.

:mag: Example: `./zappy_server -n my_team my_second_team my_third_team`

> :bulb: The default teams are `Team1`, `Team2`, `Team3` and `Team4`."

> :warning: The team name `GRAPHIC` is reserved for the graphical client.

> :information_source: A total of 100 teams can be created.

### Teams capacity

You can set the maximum number of players per team using the `-c` (`--clients`) option.

> :bulb: The default number of players per team is 4.

> :rocket: The maximum number of players per team is 100, totalizing a maximum of **10,000** players.

### Frequency

You can set the frequency of the game using the `-f` (`--frequency`) option.

Please take a look at the [Server/AI Communication Protocol](./README.md#server--ai-communication-protocol) for more information about the frequency.

> :bulb: The default frequency is 100.

> :wrench: The maximum frequency is 10,000.

### Immortal mode

You can enable the immortal mode using the `--immortal` option.

In this mode, the players will consume food, which is their only way to die on **Trantor**.

### Debug mode

You can enable the debug mode using the `--debug` option.

This mode allows developers to get more information about the game.

The debug mode show the following information:

- Resources map refills
- Incantation status and verifications

### Custom seed

You can set a custom seed for the random number generator using the `--seed` option.

## Connection

When a client successfully connects to the server, it should receive a welcome message.
The client must then identify itself, either by sending its team name or by sending the graphical identifier: `GRAPHIC`.

> :warning: All instructions sent to the server must end with a newline character (`\n`). All responses from the server will also end with a newline character.

The protocol used for communication is detailed in the [Server/AI Communication Protocol](./README.md#server--ai-communication-protocol) and [Server/Graphic Communication Protocol](./README.md#server--graphical-communication-protocol) sections, depending on the type of client.

## Server / AI Communication Protocol

The communication protocol between AI clients and the server follows some rules:

- The AI player can send actions to the server. Each action has a specific duration, within which the player cannot do another action.
- The server can send messages to the AI player, in form of events: for example when the player received a message, or when he *died*.
- When doing an action, the client will still receive events and can still send actions to the server. However, the actions will execute one by one in the order they were sent, with a limit of 10 incoming actions. If the limit is reached, **the server will ignore the player requests** until the current action is done.

> :information_source: The duration of each action is defined by the frequency of the server. For example, an action with a duration of 7 will last 7 / frequency seconds.

Here is the list of the commands that can be sent by the AI player to the server.

| Command           | Duration | Description                                   | Response Format                                          |
|-------------------|----------|-----------------------------------------------|----------------------------------------------------------|
| Forward           | 7f       | Move forward                                  | ok                                                       |
| Left              | 7f       | Rotate 90° left                               | ok                                                       |
| Right             | 7f       | Rotate 90° right                              | ok                                                       |
| Look              | 7f       | Look around                                   | [see below](#look)                                       |
| Inventory         | 1f       | Get inventory                                 | [see below](#inventory)                                  |
| Broadcast \<text> | 7f       | Broadcast a message                           | ok                                                       |
| Connect_nbr       | 0f       | Get the number of available slots in the team | (number)                                                 |
| Fork              | 42f      | Fork a new player                             | ok                                                       |
| Eject             | 7f       | Eject a player from the tile                  | ok                                                       |
| Take \<item>      | 7f       | Take a resource from the tile                 | ok / ko                                                  |
| Set \<item>       | 7f       | Set a resource on the tile                    | ok / ko                                                  |
| Incantation       | 300f     | Start an incantation                          | Elevation underway<br/>Current level: (number)<br/> / ko |

In addition to the command responses, the server can send the following events to the AI player:

| Event                           | Description                 |
|---------------------------------|-----------------------------|
| message \<orientation>, \<text> | Received a message          |
| eject: \<number>                | Player eject from direction |
| dead                            | Death of the player         |

### Map

The map is a square of size `width` x `height`. The four sides of the map are connected, so that a player can move from one side to another.

> :bulb: A player that moves on the right side of the map will appear on the left side of the map, and so on.

There is seven types of resources on the map:

- Food
- Linemate
- Deraumere
- Sibur
- Mendiane
- Phiras
- Thystame

### Look

The `Look` command returns the content of the tiles around the player. Depending on the level of the player, he will see further.

The format of the response is the following:

```
[player food linemate,,player,food]
```

Each tile is separated by a comma, and each resource within the same tile is separated by a space.

The position of the tiles follow this schema:

![docs/look_schema.png](docs/look_schema.png)

> :bulb: A player that is at level 1 will see tiles 0, 1, 2 and 3 according to its position and orientation. The range increases by one for each level.

> :information_source: The player that executes the command sees himself in the first tile.

### Inventory

The `Inventory` command returns the content of the player inventory.

The format of the response is the following:

```
[food 19, linemate 8, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0]
```

> :bulb: Every resource is shown even if the player doesn't have it in his inventory

## Server / Graphical Communication Protocol

As well as any AI player that follows the AI / Server protocol will work with the server, any graphical client that respects the following protocol will do so.

After etablishing the connection to the server, the graphical client must send `GRAPHIC\n` in order to be identified as a graphical client.

The server will now receive all the events related to the game, and can send the following commands to get information about the game:

| Command | Description                    | Response Format                       |
|---------|--------------------------------|---------------------------------------|
| msz     | Get the map size               | msz X Y                               |
| bct X Y | Get the content of a map tile  | bct X Y q0 q1 q2 q3 q4 q5 q6          |
| mct     | Get the content of all the map | Successive `bct` lines (1 per tile)   |
| tna     | Get team names                 | Successive `tna N` lines (1 per team) |
| ppo n   | Get a player's position        | ppo n X Y O                           |
| plv n   | Get a player's level           | plv n L                               |
| pin n   | Get a player's inventory       | pin n X Y q0 q1 q2 q3 q4 q5 q6        |
| sgt     | Get the server frequency       | sgt T                                 |
| sst T   | Change the server frequency    | sst T                                 |


Additionally to the command responses, the graphical client will receive the following lines:


| Line received                  | Description                   |
|--------------------------------|-------------------------------|
| bct X Y q0 q1 q2 q3 q4 q5 q6   | Tile                          |
| pnw n X Y O L N                | Player connection             |
| ppo n X Y O                    | Player's position             |
| plv n L                        | Player's level                |
| pin n X Y q0 q1 q2 q3 q4 q5 q6 | Player's inventory            |
| pex n                          | Ejection                      |
| pbc n M                        | Broadcast                     |
| pic X Y L n                    | Incantation beginning         |
| pie X Y R                      | Incantation ending            |
| pfk n                          | Egg laying                    |
| pdr n i                        | Resource picked up            |
| pgt n i                        | Resource dropped              |
| pdi n                          | Player death                  |
| enw e n X Y                    | Egg laid by a player          |
| ebo e                          | Player connection from an egg |
| edi e                          | Egg destroyed                 |
| sgt T                          | Time unit modification        |
| seg N                          | End of the game               |
| smg eni e X Y                  | First spawn egg               |
| smg pse P                      | Game paused/resumed           |

When an element is updated, it is sent to the player.

> :information_source: For example, when a player drops an item, the inventory of the player and the tile content are sent to the graphical client after the drop event.

> :warning: When the client sends a command that does not exists, the server will respond `suc`, and `sbp` in case of wrong arguments in a valid command.

Here is the legend for the placeholders used:

| Code          | Description                                |
|---------------|--------------------------------------------|
| X             | X coordinate                               |
| Y             | Y coordinate                               |
| O             | Orientation: 1(N), 2(E), 3(S), 4(W)        |
| q0            | Resource 0 (food) quantity                 |
| q1            | Resource 1 (linemate) quantity             |
| q2            | Resource 2 (deraumere) quantity            |
| q3            | Resource 3 (sibur) quantity                |
| q4            | Resource 4 (mendiane) quantity             |
| q5            | Resource 5 (phiras) quantity               |
| q6            | Resource 6 (thystame) quantity             |
| i             | Resource number                            |
| n             | Player identifier                          |
| N             | Team name                                  |
| e             | Egg identifier                             |
| T             | Time unit                                  |
| L             | Level                                      |
| M             | Message                                    |
| R             | Incantation result (0: failed, 1: success) |
| P             | Pause state (0: resumed, 1: paused)        |

## Server commands

It is possible to execute commands from the server, which will affect the game.

### Source operators

Commands that rely on a specific need an identifier to determine the target of your command.

The player ID is used as an identifier, but you can also use modifiers:

- `@a` targets **all the players**
- `@e` targets **all the entities**, which includes players, eggs and items on the floor
- `@r` targets **a random player**.

You can find the list of the available commands using `/help`.

> :bulb: Sending EOF (a.k.a. `CTRL+D`) will stop the game and close the server.

> :information_source: Mandatory arguments are surrounded with `< >`, while optional are surrounded with `[ ]`.

#### List of the available commands

- [/tp](./README.md#position-tp)
- [/give](./README.md#items-give)
- [/kill](./README.md#entites-kill)
- [/level](./README.md#players-level)
- [/pause](./README.md#game-pausing-pause--resume)
- [/resume](./README.md#game-pausing-pause--resume)
- [/debug](./README.md#debug-mode-1)

### Position: /tp

The `/tp` allows to teleport a source to a specified destination or another player.

The syntax `/tp <src> <dest>` teleports a source to the location of a specified player, and the syntax `/tp <src> <x> <y>` teleports a source to a specific location.

> :bulb: Relative coordinates can be used, using a `~` before the number.

Examples:
- `/tp 0 1` - Teleports the player `0` to the position of player `1`
- `/tp @a 4 3` - Teleports all players to the coordinates `(4, 3)`
- `/tp 0 ~-1 ~2` - Teleports the player `0` 1 tile on South and 2 tiles on East
- `/tp @e 2` - Teleports all entities to the position of player `2`

> :warning: `@a` and `@e` cannot be used as a destination, and relative coordinates cannot be used with these modifiers as the source.

### Items: /give

The `/give` command gives items to a source, directly in its inventory.

Format: `/give <src> <type> [amount]`

> :bulb: You need to specify the item type (`food`, `linemate`...), and the default amount is **1**.

Examples:
- `/give 0 linemate` - Gives 1 linemate to the player `0`
- `/give @a food 4` - Gives 4 food to all players

> :warning: The maximum amount of items you can give in one command is **1000**.

### Entites: /kill

The `/kill` command can be used to kill player or entities.

Format: `/kill <src>`

Examples:
- `/kill 2` - Kills the player `2`
- `/kill @r` - Kills a random player
- `/kill @e` - Kills all entities (players, resources, eggs)

> :wrench: You cannot kill a specific entity, you need to kill them all using `@e`.

### Players: /level

You can get or modify a player's level using `/level`.

Format: `/level <src> [level]`

> :bulb: The optional argument allows you to modify the player level

Examples:
- `/level 0` - Shows the level of the player `0`
- `/level @a` - Shows the level of all players
- `/level 2 1` - Sets the player `2` to the level 1
- `/level @r 7` - Sets a random player to the level 7

### Game pausing: /pause & /resume

You can pause and resume the game using `/pause` and `/resume`.

> :information_source: When the game is paused, players actions are stopped, and the server does not process their requests anymore until the game is resumed.

> :bulb: Graphical clients can still normally interact with the server.

### Debug mode

The debug mode can be enabled and disabled at runtime using `/debug`.
