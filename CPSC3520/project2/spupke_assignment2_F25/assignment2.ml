(* First Duplicate Function *)
(*
Find and return the first duplicate in an integer list.

first_duplicate of a list returns -10000 if there are no duplications in the
integer list argument. Otherwise, the first item that has a duplicate in the list is returned. Note: This is not the first element that occurs twice, but rather the first element with a duplicate later in the list. See the second example: the function will return 2, not 5.
*)

 (* helper checks if list contains item *)
let rec contains list item =
 match list with
 (*If the list is empty then list can't contain item *)
 | [] -> false
 (*If the list has a head then check the head or return the recursive
     if the rest of the list may contain item*)
 | h :: t -> h = item || contains t item;;

let rec first_duplicate list =
 match list with
 (*If the list is empty then there is nothing else to check duplicates
     for*)
 | [] -> -10000
 (*If the list is not empty then check if the new head is found.
     if it is, return the head. if it isn't, recurse with the rest of the         list*)
 | h :: t -> if contains t h then h else first_duplicate t;;

first_duplicate [1;2;3;4;5;6;7;4;5;8;9];; 
first_duplicate [1;2;3;4;5;6;7;8;5;2;9];; 
first_duplicate [1;2;3;4;5;6;7;8;9;10];; 


(* Sum of Two Function *)
(*
Look for a pair of elements from two integer lists that sum to a given value.
Two arrays contain numbers able to produce a given sum. For example, if a =
[1;2;3], b = [10;20;30;40], and v = 42, then sumOfTwo(a, b, v) = true
because (2 + 40) = 42 = v.

sumOfTwo(a, b, v) returns false if there does not exist an integer in a which, added to any integer in b, equals v. If there is such a pair, it returns true.
*)

(*helper checks if list b has any value that sums with anum to v*)
let rec sumcheck (anum, b, v) =
 match b with
 (*if b is empty then there can be no value to sum to v*)
 | [] -> false
 (*if b has a head, check if it plus anum equals v or recurse and try again*)
 | h :: t -> (anum + h = v) || sumcheck (anum, t, v);;

let rec sumOfTwo (a, b, v) =
(*two layers of recursion, for each num in a check, use helper to
  determine if there is a value in b to sum to v*)
 match a with
  (*If a is empty then there is nothing else to help sum to v*)
  | [] -> false
  (*If a has a head, use helper sumcheck to look for a value in b*)
  | h :: t -> sumcheck (h, b, v) || sumOfTwo (t, b, v);;

sumOfTwo([1;2;3],[10;20;30;40],42);; 
sumOfTwo([1;2;3],[10;20;30;40],40);;
sumOfTwo([1;2;3],[10;20;30;40],41);;
sumOfTwo([1;2;3],[10;20;30;40],43);;
sumOfTwo([1;2;3],[10;20;30;40],44);;
sumOfTwo([1;2;3],[10;20;30;40],11);;
sumOfTwo([1;2;3],[10;20;30;40],15);;


(* List Intersection Function *)
(*
Compute the intersection of two lists as a set (no duplicates), preserving the order in which elements first appear in the first list.

list_intersection (a, b) returns a list of elements that are in both a and b,
without duplicates, ordered as in a.
*)

(*helper to keep track of seen values. return list of all uniques in order*)
let rec track seen alist blist =
 match alist with
  (*If alist is empty then return an empty list*)
  | [] -> []
  (*If alist has a head, h, then:*)
  | h :: t ->
   (*... check if list b contains h and seen doesn't already have h*)
   if contains blist h && not (contains seen h)
    (*...then return h appended to recursively track with h added to seen*)
    then h :: track (h :: seen) t blist
    (*otherwise do nothing with h and continue to recursively track*)
    else track seen t blist;;

(*whats cool about this language is it almost looks like written english if you write it to look like that*)

let list_intersection (a, b) =
 track [] a b;;

list_intersection ([3;1;2;3;4;1],[5;4;3;3;2]);; 
list_intersection (["x";"y";"z"],["a";"y";"y";"x"]);;


(* Unique Function: keeps first occurrences *)
(*
Remove duplicates from a list while preserving the order of first appearances.

unique lst returns a list containing each distinct element of lst once, in the order of their first occurrence.
*)
let unique list =
 (*I should have looked ahead in the code to see unique and could have
   used it for the previous problem, however I can use the track function
   from before and hack it by sending in list twice *)
 track [] list list;;

unique [3;1;3;2;1;3;4];; 
unique ["a";"b";"a";"a";"c";"b";"d"];;


(* Powerset Function *)
(*
The powerset function takes a set S represented as a list and returns the set of all subsets of S. The order of subsets in the powerset and the order of elements in the subsets do not matter.

Consider the recursive structure of this problem. Suppose you already
have p, such that p = powerset s. How could you use p to compute powerset
(x::s)?
*)

(*helper function to add item to the head of each list *)
let rec add_head item lists =
 match lists with
 (*if lists is an empty list, there is nothing to add item to*)
 | [] -> []
 (*if lists has a head, even an empty list ([[]]), add item to the head
   of each list in lists, recursively*)
 | h :: t -> (item :: h) :: add_head item t;;

(*helper function to join two lists, preserving order: [1;2] [3;4] = [1;2;3;4]*)
let rec join_lists lista listb = 
 match lista with
 (*if lista is empty, return listb. this is for base case of joining ([] b)
   otherwise if it was [] -> [] it would just return list a*)
 | [] -> listb
 | h :: t -> h :: (join_lists t listb);;

let rec powerset list =
 match list with
 (*if list is empty then the powerset list is an empty list*)
 | [] -> [[]]
 (*if list has a head then join powerset of the tail and all lists
   that can be made with the head.*)
 | h :: t -> join_lists (powerset t) (add_head h (powerset t));;
    
powerset [1;2;3];; 
powerset [1;2];; 
powerset [1;2;3;4];;
