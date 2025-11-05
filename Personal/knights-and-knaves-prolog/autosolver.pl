% -----------------------------------------------------------------------------
% solver.pl
% Knights & Knaves solver + parser for the provided textual statement forms.
% -----------------------------------------------------------------------------
:- use_module(library(pcre)).       % for re_matchsub/4 (regex parsing)
:- use_module(library(readutil)).   % for read_line_to_string/2 

% says_fact/2 is populated by load_puzzle/1
:- dynamic says_fact/2.

% Allowed role values
role_val(knight).
role_val(knave).

% --------------------------
% Public entrypoints
% --------------------------
% load_puzzle(+File)
%   Load statements from File, parse them and assert says_fact/2 facts.
load_puzzle(File) :-
    retractall(says_fact(_,_)),      % clear previous puzzle
    open(File, read, Str),
    load_lines(Str, 1),             % line number for diagnostics
    close(Str).

% solve(-Pairs)
%   Generic solver: Pairs is list of Person=Role that satisfy all says.
solve(Pairs) :-
    collect_people(People),
    assign_roles(People, Roles),
    check_all_statements(People, Roles),
    pair_people_roles(People, Roles, Pairs).

% print_solutions
%   Print all solutions to the current puzzle (loaded via load_puzzle/1).
print_solutions :-
    solve(S),
    writeln(S),
    fail ; true.

% save_solutions(+File)
%  Save all solutions to File, one per line.
save_solutions(File) :-
    open(File, write, Stream),
    forall(solve(S),
           (write(Stream, S), nl(Stream))),
    close(Stream).


% --------------------------
% File reading & parsing
% --------------------------
% load_lines(+Stream, +LineNo)
load_lines(Stream, LineNo) :-
    read_line_to_string(Stream, Line),
    ( Line == end_of_file ->
        true
    ; string_codes(Line, Codes),
      ( Codes = [] ->                           % blank line -> skip
          NextLine is LineNo + 1,
          load_lines(Stream, NextLine)
      ; parse_line(Line, SpeakerAtom, StatementTerm) ->
          assertz(says_fact(SpeakerAtom, StatementTerm)),
          NextLine is LineNo + 1,
          load_lines(Stream, NextLine)
      ; % parse failed: report a helpful error with line number
          format(user_error, '*** parse error on line ~w: ~s~n', [LineNo, Line]),
          close(Stream),
          fail
      )
    ).

% parse_line(+RawLine, -SpeakerAtom, -StatementTerm)
% Extract speaker and quoted statement, then convert the natural text to our internal term.
parse_line(Raw, SpeakerAtom, Term) :-
    % Match lines like:  Cora says "Madelena and I are both knaves"
    re_matchsub('^[ \t]*(?P<speaker>[^ \t"]+)[ \t]+says[ \t]+"(?P<stmt>.+)"[ \t]*$',
                Raw, M, []),
    get_dict(speaker, M, SpeakerStr),
    get_dict(stmt,    M, StmtStr),
    atom_string(SpeakerAtom, SpeakerStr),
    parse_stmt_string(SpeakerAtom, StmtStr, Term).

% --------------------------
% Statement string -> statement term
% The patterns below cover every sentence form you listed.
% --------------------------
parse_stmt_string(Speaker, Str, Term) :-
    % --- three-person "all knights" ---
    ( re_matchsub('^[ \t]*(?P<o1>[^ \t,"]+),[ \t]*(?P<o2>[^ \t,"]+),[ \t]*and[ \t]*I[ \t]*are[ \t]*all[ \t]*knights[ \t]*$',
                  Str, M, []) ->
        get_dict(o1, M, O1s), get_dict(o2, M, O2s),
        atom_string(O1, O1s), atom_string(O2, O2s),
        nested_and([ is_knight(O1), is_knight(O2), is_knight(Speaker) ], Term)
    ; % --- three-person "all knaves" ---
      re_matchsub('^[ \t]*(?P<o1>[^ \t,"]+),[ \t]*(?P<o2>[^ \t,"]+),[ \t]*and[ \t]*I[ \t]*are[ \t]*all[ \t]*knaves[ \t]*$',
                  Str, M, []) ->
        get_dict(o1, M, O1s), get_dict(o2, M, O2s),
        atom_string(O1, O1s), atom_string(O2, O2s),
        nested_and([ is_knave(O1), is_knave(O2), is_knave(Speaker) ], Term)
    ; % --- three-person "all the same" ---
      re_matchsub('^[ \t]*(?P<o1>[^ \t,"]+),[ \t]*(?P<o2>[^ \t,"]+),[ \t]*and[ \t]*I[ \t]*are[ \t]*all[ \t]*the[ \t]*same[ \t]*$',
                  Str, M, []) ->
        get_dict(o1, M, O1s), get_dict(o2, M, O2s),
        atom_string(O1, O1s), atom_string(O2, O2s),
        % "all the same" -> O1 = O2 AND O2 = Speaker
        Term = and(same(O1,O2), same(O2,Speaker))
    ; % --- two-person "both knights" ---
      re_matchsub('^[ \t]*(?P<other>[^ \t,"]+)[ \t]+and[ \t]*I[ \t]*are[ \t]*both[ \t]*knights[ \t]*$',
                  Str, M, []) ->
        get_dict(other, M, Os), atom_string(Other, Os),
        Term = and(is_knight(Other), is_knight(Speaker))
    ; % --- two-person "both knaves" ---
      re_matchsub('^[ \t]*(?P<other>[^ \t,"]+)[ \t]+and[ \t]*I[ \t]*are[ \t]*both[ \t]*knaves[ \t]*$',
                  Str, M, []) ->
        get_dict(other, M, Os), atom_string(Other, Os),
        Term = and(is_knave(Other), is_knave(Speaker))
    ; % --- two-person "are the same" ---
      re_matchsub('^[ \t]*(?P<other>[^ \t,"]+)[ \t]+and[ \t]*I[ \t]*are[ \t]*the[ \t]*same[ \t]*$',
                  Str, M, []) ->
        get_dict(other, M, Os), atom_string(Other, Os),
        Term = same(Other, Speaker)
    ; % --- two-person "are different" ---
      re_matchsub('^[ \t]*(?P<other>[^ \t,"]+)[ \t]+and[ \t]*I[ \t]*are[ \t]*different[ \t]*$',
                  Str, M, []) ->
        get_dict(other, M, Os), atom_string(Other, Os),
        Term = different(Other, Speaker)
    ; % --- "Other is a knight" ---
      re_matchsub('^[ \t]*(?P<other>[^ \t,"]+)[ \t]+is[ \t]+a[ \t]*knight[ \t]*$',
                  Str, M, []) ->
        get_dict(other, M, Os), atom_string(Other, Os),
        Term = is_knight(Other)
    ; % --- "Other is a knave" ---
      re_matchsub('^[ \t]*(?P<other>[^ \t,"]+)[ \t]+is[ \t]+a[ \t]*knave[ \t]*$',
                  Str, M, []) ->
        get_dict(other, M, Os), atom_string(Other, Os),
        Term = is_knave(Other)
    ; % --- exclusive OR: "Either X or Y is a knight, but not both" ---
      re_matchsub('^[ \t]*Either[ \t]+(?P<o1>[^ \t,"]+)[ \t]+or[ \t]+(?P<o2>[^ \t,"]+)[ \t]+is[ \t]+a[ \t]*knight,[ \t]*but[ \t]*not[ \t]*both[ \t]*$',
                  Str, M, []) ->
        get_dict(o1, M, O1s), get_dict(o2, M, O2s),
        atom_string(O1, O1s), atom_string(O2, O2s),
        Term = xor(is_knight(O1), is_knight(O2))
    ; % fallback: unknown pattern
      format(user_error, '*** warning: statement did not match any pattern: "~s"~n', [Str]),
      fail
    ).

% nested_and(+ListOfAtomsOrCompound, -NestedAndTerm)
% Build a left-nested and(A,B) structure from a list [A,B,C] -> and(A, and(B,C)).
nested_and([X], X).
nested_and([A,B|T], and(A, Rest)) :-
    nested_and([B|T], Rest).

% --------------------------
% Person collection (speakers + names mentioned inside statements)
% --------------------------
collect_people(PeopleSorted) :-
    findall(S, says_fact(S,_), SpeakersDup),
    sort(SpeakersDup, Speakers),
    findall(P, (says_fact(_,Stmt), persons_in_stmt(Stmt, P)), MentionedDup),
    sort(MentionedDup, Mentioned),
    append(Speakers, Mentioned, All),
    sort(All, PeopleSorted).

% persons_in_stmt/2 extracts all person atoms appearing in a statement term.
persons_in_stmt(is_knight(P), P).
persons_in_stmt(is_knave(P), P).
persons_in_stmt(same(A,B), A).
persons_in_stmt(same(A,B), B).
persons_in_stmt(different(A,B), A).
persons_in_stmt(different(A,B), B).
persons_in_stmt(and(A,B), P)     :- ( persons_in_stmt(A,P) ; persons_in_stmt(B,P) ).
persons_in_stmt(or(A,B), P)      :- ( persons_in_stmt(A,P) ; persons_in_stmt(B,P) ).
persons_in_stmt(not(A), P)       :- persons_in_stmt(A,P).
persons_in_stmt(implies(A,B), P) :- ( persons_in_stmt(A,P) ; persons_in_stmt(B,P) ).
persons_in_stmt(iff(A,B), P)     :- ( persons_in_stmt(A,P) ; persons_in_stmt(B,P) ).
persons_in_stmt(xor(A,B), P)     :- ( persons_in_stmt(A,P) ; persons_in_stmt(B,P) ).

% --------------------------
% Role assignment generation
% --------------------------
assign_roles([], []).
assign_roles([_P|PT], [R|RT]) :-
    role_val(R),
    assign_roles(PT, RT).

% --------------------------
% Check statements vs roles
% --------------------------
check_all_statements(People, Roles) :-
    forall(
      says_fact(Speaker, Statement),
      (
        role_of(Speaker, People, Roles, RoleSpeaker),
        ( RoleSpeaker == knight ->
            eval(Statement, People, Roles)        % knights must speak truth
        ; RoleSpeaker == knave ->
            \+ eval(Statement, People, Roles)     % knaves must lie
        )
      )
    ).

% role_of(+Person, +People, +Roles, -Role)
role_of(Person, People, Roles, Role) :-
    nth0(Index, People, Person),
    nth0(Index, Roles, Role).

% --------------------------
% Evaluation of statement terms under an assignment
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

% exclusive-or (exactly one true)
eval(xor(A,B), People, Roles) :-
    ( eval(A, People, Roles), \+ eval(B, People, Roles) )
    ;
    ( \+ eval(A, People, Roles), eval(B, People, Roles) ).

% --------------------------
% Output helpers
% --------------------------
pair_people_roles([], [], []).
pair_people_roles([P|PT], [R|RT], [P=R|Pairs]) :-
    pair_people_roles(PT, RT, Pairs).

% -----------------------------------------------------------------------------
% End of solver.pl
% -----------------------------------------------------------------------------
