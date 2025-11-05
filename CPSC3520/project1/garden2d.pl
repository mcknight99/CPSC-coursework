/*
Samuel Pupke
CPSC 3520, Fall 2025

### Notes:

1. In all predicates, the List argument refers to the same list of N elements,
where each element is a flower species name (not a full flower(. . . ) structure).
2. To implement each predicate, you may define appropriate supporting rules
as needed.

Extend your solution to support a 2D flower garden with N × M plantings.
Implement all of the functions listed above in the 2D setting:

- [ ]  plantassign(N, M, List) Construct a length-NxM list and assign a species
to each position.
- [ ]  uniquecheck(List) Ensure no species is repeated.
- [ ]  colorcheck(List) Ensure adjacent plantings do not share the same color.
- [ ]  sizecheck(List) Ensure adjacent plantings differ by at most one size level.
- [ ]  wetcheck(N, M, List) Enforce the watering rule.
- [ ]  writegarden(List) Print the final garden plan in a readable format.
- [ ]  gardenplan(N, M, List) Orchestrate the assignment, check all constraints,
and print the garden.

Each new row of flowers acts as an independent row, rows are not constrained by rules between them.
*/

:- use_module(library(lists)). % for nth1/3


/*
Rule 1 - gardenplan(Row)
Orchestrate the assignment, check all constraints, and print the garden.
The garden consists of 1 row, each with N plantings (N is at least 4).
One flower species occupies each planting. The Row is horizontal layed out
with plantings from 1 at the left to N at the right. You will use a Prolog
list to represent the row.

% force a row length with a query like:
?- gardenplan([A, B, C, D, E, F]). 
% or let Prolog determine the length with a query like:
?- gardenplan(Row).
% (Row will be bound to a list of length N, where N >= 4)
% check the validity of the garden plan with:
?- gardenplan([azalea, begonias, buttercup, bird_of_paradise]).
*/
/*
gardenplan(Row) :-
    % check length of Row
    length(Row, N), N >= 4,
    % assign flowers to each position in the row
    plantassign(N, Row),
    %print('Attempting garden plan: '), writegarden(Row), nl,
    % check all constraints
    uniquecheck(Row),
    colorcheck(Row),
    sizecheck(Row),
    wetcheck(Row),
    write('Final garden plan: '), writegarden(Row), nl.
    */

% N rows, M columns
gardenplan(N, M, Garden) :-
    M >= 4, 
    % assign flowers to each position in the row
    plantassign(N, M, Garden),
    %print('Attempting garden plan: '), writegarden(Row), nl,
    % check all constraints
    uniquecheck(Garden),
    colorcheck(Garden),
    sizecheck(Garden),
    wetcheck(N, M, Garden),
    write('Final garden plan: '), writegarden(Garden), nl.

/*
Helper - writegarden(List) 
Print the final garden plan in a readable format.
Helper to print the garden plan
print in format "name(size, wet/dry, color)"
*/
writegardenrow([]).
writegardenrow([H|T]) :-
    flower(H, S, W, C),
    format('~w (~w, ~w, ~w)', [H, S, W, C]),
    (T \= [] -> write(', '); true), % print comma if not last element (if T is not empty)
    writegarden(T).
    
writegarden([]).
writegarden([H|T]) :-
    writegardenrow(H), nl,
    writegarden(T).

/*
Rule 2 - plantassign(N, list)
Construct a length-N list and assign a species to each position.
Need to add a way to not repeat species in this rule to speed up the search. Don't use uniquecheck for this. Use member to check if the current flower is in the list, if so, skip, fail, and retry
Better yet, maybe do an append instead of a recursive build to do notMember check and be able to insert uniques only
Better better yet, use CSP approach by placing only:
 - available, unused flowers
 - no immediate rule breaks (place neighbors w/ different color, size diff <= 1. if we know list length, we can also only place wet and dry flowers in the correct spots)
*/

plantassignrow(M, Row) :-
    length(Row, M), % ensure Row is length M
    findall(flower(Name, Size, WetDry, Color), flower(Name, Size, WetDry, Color), Flowers), % get all available flowers
    assignUnique(Flowers, Row).

plantassign(0, _, []).
plantassign(N, M, [H|T]) :-
    length(H, M), % ensure each row is length M
    length(T, N1), N1 is N - 1, % ensure there are N rows total
    findall(flower(Name, Size, WetDry, Color), flower(Name, Size, WetDry, Color), Flowers), % get all available flowers
    assignUnique(Flowers, H), % assign flowers to the first row
    plantassign(N1, M, T). % recurse to assign flowers to the remaining rows

 % ensuring no duplicates in the current assignment to make generation a bit quicker.
 % I would like to implement a CSP approach to only place valid flowers in each position based on the current state of the list, but that may be a bit out of my depth for now.
assignUnique(_, []).
assignUnique(Available, [H|T]) :-
    select(F, Available, Remaining), % select a flower from the available list
    % make H the name of the flower F, not the whole flower structure
    flower(H, _, _, _) = F,
    assignUnique(Remaining, T). % recurse with the remaining flowers

/*
Rule 3 - uniquecheck(List)
Ensure no species is repeated.
A given flower species can only be used once per row.
*/
uniquecheckrow([]).
uniquecheckrow([H|T]) :- 
    notMember(H, T),
    uniquecheckrow(T).

uniquecheck([]).
uniquecheck([H|T]) :-
    uniquecheckrow(H), % check each row for uniqueness
    uniquecheck(T). % recurse with the remaining rows



notMember(Element, List) :- \+ member(Element, List).

/*
Rule 4 - colorcheck(List)
Ensure adjacent plantings do not share the same color.
*/
% example query: ?- colorcheck([azalea, begonias, buttercup, bird_of_paradise]).
colorcheckrow([]).
colorcheckrow([_]).
colorcheckrow([L|[R|T]]) :-
    flower(L, _, _, Color1),
    flower(R, _, _, Color2),
    Color1 \= Color2,
    colorcheckrow([R|T]).

colorcheck([]).
colorcheck([H|T]) :-
    colorcheckrow(H), % check each row for color constraints
    colorcheck(T). % recurse with the remaining rows
    

/*
Rule 5 - sizecheck(List)
Ensure adjacent plantings differ by at most one size level.
No two adjacent plantings can have flowers whose size is more than one
size difference. Sizes are small, med, tall so small next to small is fine,
small next to medium is fine, but small next to tall is not.
example query: ?- sizecheck([daisies, roses, petunias, daffodils]).
*/
sizecheckrow([]).
sizecheckrow([_]).
sizecheckrow([L|[R|T]]) :-
    flower(L, Size1, _, _),
    flower(R, Size2, _, _),
    sizeValue(Size1, Value1),
    sizeValue(Size2, Value2),
    Diff is abs(Value1 - Value2),
    Diff =< 1,
    sizecheckrow([R|T]).

sizecheck([]).
sizecheck([H|T]) :-
    sizecheckrow(H), % check each row for size constraints
    sizecheck(T). % recurse with the remaining rows

/*
Rule 6 - wetcheck(List)
Enforce the watering rule.
The two outermost plantings (1 and N) are dry, the two innermost are
wet, the ones in between (if there are any) can take either.

example query: ?- wetcheck([daisies, roses, petunias, daffodils]).
*/
wetcheckrow(_, []).
wetcheckrow(_, [_]).
wetcheckrow(N, List) :-
    getFirst(List, First), flower(First, _, dry, _),
    getLast(List, Last), flower(Last, _, dry, _),
    getMiddleLeft(N, List, MiddleLeft), flower(MiddleLeft, _, wet, _),
    getMiddleRight(N, List, MiddleRight), flower(MiddleRight, _, wet, _).


wetcheck(_,_, []).
wetcheck(_,_, [_]).
wetcheck(N, M, [H | T]) :-
    wetcheckrow(N, H), % check each row for wet/dry constraints
    wetcheck(N, M, T). % recurse with the remaining rows

getFirst([H|_], H).
getLast(List, Last) :- 
    last(List, Last).
getMiddleLeft(N, List, MiddleLeft) :-
    MidPos is N // 2,
    getItemAtPos(List, MidPos, MiddleLeft).

getMiddleRight(N, List, MiddleRight) :-
    MidPos is N // 2 + 1,
    getItemAtPos(List, MidPos, MiddleRight).

getItemAtPos([H|_], 1, H).
getItemAtPos([_|T], Pos, Item) :-
    Pos > 1,
    NewPos is Pos - 1,
    getItemAtPos(T, NewPos, Item).


% helper to convert size to numeric value
sizeValue(short, 1).
sizeValue(med, 2).
sizeValue(tall, 3).


% flower(Name, Size, WetDry, Color).
% Flower definitions
flower(azalea, med, dry, pink).
flower(begonias, tall, wet, white).
flower(buttercup, short, dry, yellow).
flower(bird_of_paradise, tall, wet, white).
flower(carnation, med, wet, white).
flower(chrysanthemum, tall, dry, pink).
flower(crocus, med, dry, orange).
flower(daffodils, med, wet, yellow).
flower(dahlia, med, wet, purple).
flower(daisies, med, wet, yellow).
flower(gardenias, med, wet, red).
flower(geranium, short, dry, red).
flower(gladiolas, tall, wet, red).
flower(iris, tall, dry, purple).
flower(lavender, short, dry, purple).
flower(lilies, short, dry, white).
flower(marigolds, short, wet, yellow).
flower(orchid, short, wet, white).
flower(peonies, short, dry, pink).
flower(periwinkle, med, wet, purple).
flower(petunias, med, wet, pink).
flower(poppy, med, dry, red).
flower(roses, med, dry, red).
flower(snapdragons, tall, dry, red).
flower(sunflower, tall, dry, yellow).
flower(tulip, short, wet, red).
flower(violet, short, dry, purple).
flower(zinnia, short, wet, yellow).