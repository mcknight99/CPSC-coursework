Upon running the terminal executable:
<load/default: new>
    load/l: log file name is expecting a log file in a subdirectory relative to the executable at /logs/
        If it fails, it will default to new game
    new: a new log file will be made with the current datetime as the name
        the board:
            the first instruction is to enter a board. traditionally, it is played with a 4x4 or 5x5*
                *HOWEVER this program has functionality of any shape or size containing A-Z and $
            islands of A-Z can be entered without case sensitivity. 
            A port can be denotated as $ when entering the board.
            Use a new line for new rows, spaces are not necessary between any entries (example at bottom of README).
            Type #done when finished entering the board
        the players:
            the next instruction is to enter your players. each player will have a different color ship
            enter your players one line at a time in the format of their name, a space, their ship color
            


For all parameters:
<player> can be entered as name or color, both name and color are case sensitive to the original input.
    color has no effect on call, so it can optionally be used as a player's name "shorthand" by the user.
    however, names will be prioritized during a call, so if a color/"shorthand" and name string are identical, the name will be prioritized.
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
    When Player A digs against Player B, dig for both players. Player A wouldn't dig their own, and no matter if it was or wasn't B's loot island, the dig information is still valid. If it was, you no longer can get it. If it wasn't, we now know to not attempt there. 
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

--------------------------------

This program was intended to be used as a "very rich text editor" so it processes notes and displays your information back to you
It was also made to support several shorthands so your longest commands could be as short as 5 characters, excluding spaces

Here are two identical examples of using this program:
1. more readable to a general user looking at it for the first time
2. shorthand that looks like gibberish at first but will come to be another language for you

We are a player who is playing with Alice on the yellow boat and Bob on the red boat
the board looks like the following from our perspective:
UMIHV
RO$LB
QCTF$ 
DAJSP
EG$KN

We have the following loot islands: L,Q,N
This means we want to remove these from everyone else's potential loot islands

As we play a few rounds, we get the following information:
Alice asks if Bob has loot on the ring of radius 2 at H, Bob says yes
    We would type > hint Bob H 2 yes
Bob asks if Alice has loot on the ring of radius 1 at D, Alice says no 
    We would type > hint Alice D 1 no
We ask if Bob has loot on the ring of radius 2 at P, Bob says no
    We would type > hint Bob P 2 no
We ask if Alice has loot on the ring of radius 1 at S, Alice says yes
    We would type > hint Alice S 1 yes
Alice digs against Bob at C and fails
    We would type > dig Alice C
                  > dig Bob C
        because Alice wouldn't dig at their own island
        and Bob doesn't have any loot at that island
        (effectively this would "remove" with two players,
        but with more, we want to maintain knowledge that C
        can still belong to another player)
Anyone digs against Bob at F and succeeds
    We would type > remove F

We get a Map drawn and get to look at the top of the island deck, R is not owned.
    We would type > remove R

We want to see the remaining information on both players
    > print Alice
    > print Bob

We want to exit the program
    > exit

1: example log stored in text file "example.txt"
--------------------------------

> new
> UMIHV
> RO$LB
> QCTF$ 
> DAJSP
> EG$KN
> #done
> Alice yellow
> Bob red
> #done
> remove L
> remove Q
> remove N
> hint Bob H 2 yes
> hint Alice D 1 no
> hint Bob P 2 no
> hint Alice S 1 yes
> dig Alice C
> dig Bob C
> remove F
> remove R
> print alice
> print Bob
> exit

--------------------------------

The following contains the exact information from 1, but would be what I would shorthand everything as,
while still maintaining valid information

2: example log stored in text file "example-shorthand.txt"
--------------------------------

> new
> UMIHV
> RO$LB
> QCTF$
> DAJSP
> EG$KN
> #done
> alice a
> bob b
> r l
> r q
> r n
> h b h 2 y
> h a d 1 n
> h b p 2 n
> h a s 1 y
> d a c
> d b c
> r f
> r r
> p a
> p b
> e