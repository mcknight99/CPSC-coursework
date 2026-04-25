/*
I'm using SWI-Prolog
Here's the flow to run this program:
> swipl
?- [solver].
?- solve([907], [1315,1203,1211,2245,635,2542,2578,1296], 25, 0, LowestSum, LowestCombo).

To do the input:
?- R = [907],
   S = [1315,1203,1211,2245,635,2542,2578,1296],
   M = 25,
   D = 0,
   solve(R, S, M, D, LowestSum, LowestCombo).

R = required
S = supplementary
M = modulus
D = desired modulus result

or directly put the values in the query:
?- solve([907], [1315,1203,1211,2245,635,2542,2578,1296], 25, 0, LowestSum, LowestCombo).
*/


% subset(+List, -Subset)
% Generate all possible subsets of a list
subset([], []).
subset([H|T], [H|Rest]) :-
    subset(T, Rest).
subset([_|T], Rest) :-
    subset(T, Rest).

% sum_list(+List, -Sum) -- built-in in SWI-Prolog, but here for clarity
sum_list([], 0).
sum_list([H|T], S) :-
    sum_list(T, Rest),
    S is H + Rest.

% solve(+Required, +Supplementary, +Modulus, +Desired, -LowestSum, -LowestCombo)
solve(Required, Supplementary, Modulus, Desired, LowestSum, LowestCombo) :-
    sum_list(Required, RequiredSum),
    findall((Total, Combo),
        (   subset(Supplementary, Combo),
            sum_list(Combo, SupplementSum),
            Total is RequiredSum + SupplementSum,
            Total mod Modulus =:= Desired
        ),
        Candidates),
    sort(Candidates, Sorted),  % sort by Total
    Sorted = [(LowestSum, LowestCombo)|_].
