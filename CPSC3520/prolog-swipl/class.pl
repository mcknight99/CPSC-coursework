edge(a,b).
edge(a,c).
edge(b,d).
edge(d,e).

path(X,Y) :- edge(X,Y).
path(X,Y) :- edge(X,Z),path(Z,Y).

% query paths
%path(a,e).
%path(c,R).
%path(S,c).