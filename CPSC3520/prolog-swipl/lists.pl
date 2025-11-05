%task: build a list of integers from i to j
% build empty if i > j (base case)
buildList(I,J,[]) :- I > J.
% build list with head I and tail being the list from I+1 to J
buildList(I,J,[I|T]) :- I =< J, N is I + 1, buildList(N,J,T).
% query by ?- buildList(3,7,L). % L = [3,4,5,6,7]

%task: get the size of a list
%if the list is empty then the size is zero
size([],0).
% if there is an element and a tail (includes the list [n | [] ])
% then get the size of the tail and add 1 to the size of the tail
size([_|T],N) :- size(T,N1), N is N1+1.
% query by ?- size([a,b,c,d,e],N). % N = 5.

% task: sum the elements of a list of integers
% if there is nothing in the list then the sum of the list is 0
suml([],0).
% if there is a head with a tail (includes the list [n | [] ])
% then get the sum of the tail and add the head to the sum of the tail
suml([H|T],S) :- suml(T,S1), S is S1+H.
% query by ?- suml([1,2,3,4,5],S). % S = 15.

%task: append two lists to form a third list
% if there is an empty first list, then return the second list
append([], L2, L3) :- L3 = L2.
% if there is a populated first list (includes the list [_ | [] ])
% then append the tail to L2 and join the head to the append of the tail and L2
append([H|T], L2, L3) :- append(T, L2, T1), L3 = [H | T1].
% query by ?- append([a,b,c],[1,2,3],L3).
% or by ?- append([a],[1,2,3],L3). 
% ?- append([a | []],[1,2,3],L3).
% ?- append([a | [b,c]],[1],L3). 

/* or more simply, from the example in the lecture slides
append([], L, L).
append([H|T], L, [H|R]) :- append(T, L, R).
*/