%member(X,Y) %X is a member of list Y.
%member(X, [X|_]).
member(X,[_|T]):-member(X,T). % recursive call down the list