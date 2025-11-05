/*
A simple program saved in a file family.pl
*/
male(albert). % a fact stating that albert is male
female(alice). % a fact stating that alice is female
male(edward). % a fact stating that edward is male
female(victoria). % a fact stating that victoria is female
parent(albert,edward). % a fact stating that albert is a parent of edward
parent(victoria,edward). % a fact stating that victoria is a parent of edward
father(X,Y) :- % a rule stating that X is father of Y if X is a male parent of Y
	parent(X,Y), male(X).
mother(X,Y) :- parent(X,Y), female(X). % a rule stating that X is a mother of Y if X is a female parent of Y


% pass a predicate as an argument to a rule
test(X) :- X. % a rule that tests if the argument X is true
% pass something like test(male(albert)). It will return true.

% we could not do the following
% test(X,Y) :- X(Y). % this does not work because Prolog does not allow passing a predicate as an argument directly
% but we can use ellipses to generate predicates 
test(X,Y) :- G = .. [X,Y], G. % this works because we use ellipses to generate a predicate G from the functor X and argument Y
% what this does is effectuvely G=..[X,Y] turns into G=X(Y) and then we call G.