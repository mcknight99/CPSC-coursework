How to use the autosolver:
You must have some version of Prolog installed on your machine. I am using swipl
Open a terminal in this directory (/swipl)

Paste your puzzle into a text file ('puzzle.txt' for example. I also have included sample sizes of 2, 3, 5, 10, and 20)
Autosolver is parsing strings in the format from the site: https://jben3560.github.io/KnightsAndKnaves/

> swipl
?- [autosolver].
?- load_puzzle('puzzle.txt').

For a single solution, 
?- solve(S).
To go to the next solution,
?- ;

For all solutions,
?- print_solutions.

To save all solutions,
?- save_solutions('solutions.txt').