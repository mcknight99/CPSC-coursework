This is a puzzle game that I've wanted to develop for a whle now, and I've had plenty time to brainstorm.

The puzzle board is a hexagonal grid

The player will move tiles (the puzzle pieces) from a sidebar area to the hexagonal grid. 
These tiles store an "aspect" - these are a complex map of interweaving tiers between other aspects
Tiles will have a valid connection between them if they share a vertical connection in the map 
    (ex. Earth and Water mix to make Life. Life could connect to Earth or Water, and vice versa.
    Life and Chaos mix to make death. Death could connect to Life or Chaos, and vice versa.)
The puzzle starts with a few pre-placed, unmovable tiles that the player must use other aspects to solve the board.
Solving the board means all tiles placed have a relation to each connected tile.
The player should be able to left click and drag to place a new tile or to move an existing tile.
The player should be able to right click an existing tile to delete it. 
When hovering over a tile in the sidebar, I want to show the two aspects used to craft the one being hovered over, as well as its name.

I also want a way to store and import aspect relations in a file 
A file example could be: "FIRE\nWATER\nAIR\nEARTH\nORDER\nCHAOS\nFIRE+AIR=LIGHT\nLIGHT+CHAOS=DARKNESS\nAIR+CHAOS=VOID"
I also want a folder that stores images to be used as the tiles to represent aspects at a glance. 
If an image isn't found by name, it uses the first letter of the aspect on top of the tile.

I want to focus less on the technical side and more on the puzzle development side, 
so I want to easily be able to tweak rules, board size and shape, and aspect relations. 
I will also want to add "voids" in the board later, so I can make unique board shapes where players can't place tiles.