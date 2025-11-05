% -----------------------------------------------------------------------------
% knights_and_knaves.pl
% Generic Knights & Knaves solver.
%
% Usage:
%  - Add facts of the form:
%      says_fact(Person, Statement).
%    where Statement uses the constructors defined below.
%  - Query:
%      ?- solve(Solution).
%    Backtracking returns all solutions (each `Solution` is a list Person=Role).
%
% Statement language (compose these):
%  - Atomic:
%      is_knight(X)      % "X is a knight"
%      is_knave(X)       % "X is a knave"
%      same(X,Y)         % "X and Y have same role"
%      different(X,Y)    % "X and Y have different roles"
%  - Boolean connectives:
%      and(A,B), or(A,B), not(A), implies(A,B), iff(A,B)
% -----------------------------------------------------------------------------

% allowed role values used when generating assignments
role_val(knight).
role_val(knave).

% --------------------------
% MAIN SOLVER
% --------------------------

% solve(-PersonRolePairs)
% PersonRolePairs is a list like [a=knight, b=knave, ...] that satisfies all says.
solve(Pairs) :-
    collect_people(People),          % gather all people mentioned and speakers
    assign_roles(People, Roles),     % nondeterministically generate an assignment
    check_all_statements(People, Roles), % verify every statement matches speakers role
    pair_people_roles(People, Roles, Pairs).

% --------------------------
% Collect people: speakers + names mentioned in statements
% --------------------------
collect_people(PeopleSorted) :-
    findall(S, says_fact(S,_), SpeakersDup),
    sort(SpeakersDup, Speakers),                    % remove duplicates & sort
    findall(P, (says_fact(_,Stmt), persons_in_stmt(Stmt,P)), MentionedDup),
    sort(MentionedDup, Mentioned),
    append(Speakers, Mentioned, All),
    sort(All, PeopleSorted).                        % final canonical list of people

% persons_in_stmt/2 traverses a statement term and yields each person atom found.
% Only the constructors we use are inspected; unknown functors are ignored.
persons_in_stmt(is_knight(P), P).
persons_in_stmt(is_knave(P), P).
persons_in_stmt(same(A,B), A).
persons_in_stmt(same(A,B), B).
persons_in_stmt(different(A,B), A).
persons_in_stmt(different(A,B), B).
persons_in_stmt(and(A,B), P)       :- ( persons_in_stmt(A,P) ; persons_in_stmt(B,P) ).
persons_in_stmt(or(A,B), P)        :- ( persons_in_stmt(A,P) ; persons_in_stmt(B,P) ).
persons_in_stmt(implies(A,B), P)   :- ( persons_in_stmt(A,P) ; persons_in_stmt(B,P) ).
persons_in_stmt(iff(A,B), P)       :- ( persons_in_stmt(A,P) ; persons_in_stmt(B,P) ).
persons_in_stmt(not(A), P)         :- persons_in_stmt(A,P).

% --------------------------
% Generate all combinations of roles for the People list
% assign_roles(+People, -Roles)
% --------------------------
assign_roles([], []).
assign_roles([_P|PT], [R|RT]) :-
    role_val(R),              % R = knight ; R = knave
    assign_roles(PT, RT).

% --------------------------
% Check every says_fact holds under the current assignment
% check_all_statements(+People, +Roles)
% --------------------------
check_all_statements(People, Roles) :-
    forall(
      says_fact(Speaker, Statement),
      (
        role_of(Speaker, People, Roles, RoleSpeaker),
        ( RoleSpeaker == knight ->               % knights must tell true statements
            eval(Statement, People, Roles)
        ; RoleSpeaker == knave ->                % knaves must tell false statements
            \+ eval(Statement, People, Roles)
        )
      )
    ).

% role_of(+Person, +People, +Roles, -Role)
% Map a Person atom to its Role by using the same index in People and Roles.
role_of(Person, People, Roles, Role) :-
    nth0(Index, People, Person),
    nth0(Index, Roles, Role).

% --------------------------
% Evaluate statements under the given assignment
% eval(+Statement, +People, +Roles)
% --------------------------
eval(is_knight(P), People, Roles) :-
    role_of(P, People, Roles, knight).

eval(is_knave(P), People, Roles) :-
    role_of(P, People, Roles, knave).

eval(same(A,B), People, Roles) :-
    role_of(A, People, Roles, RA),
    role_of(B, People, Roles, RB),
    RA == RB.

eval(different(A,B), People, Roles) :-
    role_of(A, People, Roles, RA),
    role_of(B, People, Roles, RB),
    RA \== RB.

eval(and(A,B), People, Roles) :-
    eval(A, People, Roles),
    eval(B, People, Roles).

eval(or(A,B), People, Roles) :-
    ( eval(A, People, Roles) ; eval(B, People, Roles) ).

eval(not(A), People, Roles) :-
    \+ eval(A, People, Roles).

eval(implies(A,B), People, Roles) :-
    ( \+ eval(A, People, Roles) ; eval(B, People, Roles) ).

eval(iff(A,B), People, Roles) :-
    ( eval(A, People, Roles), eval(B, People, Roles) )
    ;
    ( \+ eval(A, People, Roles), \+ eval(B, People, Roles) ).

% --------------------------
% Pair people with roles into Person=Role terms for output
% pair_people_roles(+People, +Roles, -Pairs)
% --------------------------
pair_people_roles([], [], []).
pair_people_roles([P|PT], [R|RT], [P=R|Pairs]) :-
    pair_people_roles(PT, RT, Pairs).

% -----------------------------------------------------------------------------
% EXAMPLES - comment/uncomment to try different puzzles
% -----------------------------------------------------------------------------

% Example 1: mutual accusations (two solutions)
% A says "B is a knave".
% B says "A is a knave".
%says_fact(a, is_knave(b)).
%says_fact(b, is_knave(a)).

% Example 2: classical 2-person (single solution):
% Uncomment these and comment the Example 1 facts to test.
says_fact(a, is_knave(b)).   % A says "B is a knave"
says_fact(b, same(a,b)).     % B says "A and I are the same"

% You can write nested statements, e.g.:
% says_fact(c, implies(is_knight(a), is_knave(b))).
% -----------------------------------------------------------------------------
