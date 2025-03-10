Upon running the terminal executable:
<load/default: new>
    load/l: log file name is expecting a log file in a subdirectory relative to the executable at /logs/
        If it fails, it will default to a new game with the current datetime as the name
    new:

For all parameters:
<player> can be entered as name or color, both name and color are case sensitive to the original input.
    color has no effect on call, so it can optionally be used as a player's name shorthand by the user.
<island> is not case sensitive, passes as long as it exists on the board.
<command> can be shortened to their first letter when passing, but are case sensitive;
    hint = h
    dig = d
    remove = r
    print = p
    list = l
    exit = e
<yes/no> can be shorthanded to several entries:
    yes: y, true, t, 1
    no: n, false, f, 0

Commands:
hint <player> <island> <radius> <yes/no>
    When player A has to answer a hint from another player, B, enter the hint command with A as <player>
    Pass <island> as the island player B is located, and <radius> as the radius player B asked
    And enter <yes/no> with whether or not player A has a loot island located on the <radius> centered at <island>

dig <player> <island>
    When <player> digs at <island>, log this for them because they most likely wouldn't dig their own island, so we know it isn't their loot island.
    This can also be generally used to remove a potential island from just one player

remove <island>
    When any island information excludes an island from being potential loot for anyone 
    This will remove (effectively dig) <island> from all players' potential islands, so you can use this for:
    A. Your own loot islands; removing your own loot islands because that is known information 
    B. Dug loot islands; removing successful digs as remaining options 
    C. Mapped loot islands; remembering what map information you received 

print <player/default: board>
    By default, print will print the original board input to the program. "board" can also be entered as the player parameter
    If a player gets passed, it will print the most current information on that player, formatted by:
        Red spaces : Guaranteed not an island
        Bold colored text : previously been answered "yes" as a loot island
        Default text : No information

list
    Lists all existing players and their corresponding colors

exit
    Exit the program