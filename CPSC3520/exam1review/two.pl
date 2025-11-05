gets_tenure(Faculty) :-
    publishes(Faculty),
    gets_research(Faculty),
    teaches_well(Faculty).

publishes(Professor) :-
    does_research(Professor),
    documents_research(Professor).

gets_research(Researcher) :-
    writes_proposals(Researcher),
    gets_funded(Researcher).

teaches_well(Educator) :-
    prepares_lectures(Educator),
    lectures_well(Educator),
    gets_good_evaluations(Educator).

prepares_lectures(aa).
prepares_lectures(bb).
prepares_lectures(cc).
prepares_lectures(dd).
prepares_lectures(ee).

writes_proposals(aa).
writes_proposals(bb). 
writes_proposals(cc).
writes_proposals(dd).
writes_proposals(ff).

gets_good_evaluations(aa).
gets_good_evaluations(bb).
gets_good_evaluations(cc).
gets_good_evaluations(ee).
gets_good_evaluations(ff).

does_research(aa).
does_research(bb).
does_research(cc).
does_research(ee).

good(ff).

gets_funded(aa).
gets_funded(bb).
gets_funded(cc).
gets_funded(dd).
gets_funded(ee).

documents_research(aa).
%documents_research(bb).
documents_research(dd).
%documents_research(ee).

lectures_well(aa).
lectures_well(bb).
lectures_well(cc).
lectures_well(dd).
lectures_well(ee).












is_red(car_a).
is_red(car_b).
is_red(car_e).
is_red(car_f).

is_blue(car_c).
is_blue(car_g).

is_gray(car_d).
is_gray(car_h).

is_ford(car_a).
is_ford(car_d).
is_ford(car_f).
is_ford(car_g).

is_chevy(car_b).
is_chevy(car_c).
is_chevy(car_e).
is_chevy(car_h).

is_sedan(car_a).
is_sedan(car_c).
is_sedan(car_h).
is_sedan(car_f).

is_coupe(car_b).
is_coupe(car_d).
is_coupe(car_e).
is_coupe(car_g).

is_stick(car_b).
is_stick(car_g).

is_auto(car_a).
is_auto(car_c).
is_auto(car_d).
is_auto(car_e).
is_auto(car_f).
is_auto(car_h).

is_sporty(X) :- is_stick(X), is_coupe(X).

is_practical(X) :- is_sedan(X), is_auto(X).
is_practical(X) :- is_coupe(X), is_auto(X).