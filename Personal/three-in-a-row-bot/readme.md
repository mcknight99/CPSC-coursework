i want to make a python program to solve a puzzle on my screen by taking control of the mouse and solve the puzzle live as it does the logic to solve it. 
the program should also have start/stop/kill hotkeys so the user doesn't lose control or get the computer softlocked (start/stop F8, kill F9)
the puzzle is "three in a row", and the following is how the program needs to work:

find the boundaries of the square shaped puzzle on the screen using a hotkey prompted by the program for each tile on the negative diagonal (top left to bottom right). i.e. F1 for the start, F2 for each diagonal, F3 for the final corner. This will help calibrate tile and board size using user mouse locations at the time of the function keys, letting the program remember where the board is located and approximately how big each tile is. 

Then, F4 should be used while hovering over a black, then while over a white, then over an empty to get an idea of the colors it's looking at. The program will take the colors from the mouse's position at each of these and use them when scanning the board. once calibration is complete, the program should approximate the locations of the rest of the board and scan those locations for black, white, or empty and store those locations and values. 

I would also like an optional overlay on the screen (toggle hotkey F5) from python of where it thinks each tile is and what they are (a small circle 1/4 the size of a tile centered on the tile, red for empty, green for white, blue for black)

when the program is started, start using logic to solve the puzzle, and clicking on the screen (left click=black,right click=white) at the locations of solved locations as the puzzle gets solved until the puzzle is complete.

there are several logic check functions that need to pass the board in and return a board with all applicable moves the logic function can make. these will run in a loop until there are no more valid moves to make
these functions include: 
- 2 in a row -> if there are two of the same color in line, the opposite color should be placed on both sides
- 1 gap -> if there are two of the same color on either side of an empty tile, the opposite color should be placed
- half satisfied line -> if a row or column of size n has n/2 of a color, all empty spaces in that row or column should be filled with the opposite color
- one off -> if a row or column of size n has (n/2)-1 of a color and less than (n/2)-1 of the other, check all possible combinations for the empty spaces by placing the 1 of the first color in each empty space at a time and filling the empty spaces in the row or column. check if the row or column is valid (no 3 in a rows), and if the combination is possible, add it to an array of the possible answers. once all combinations have been tried, sift through the array. all empty tiles with colors that are the same in all possible combinations should be placed with that color
