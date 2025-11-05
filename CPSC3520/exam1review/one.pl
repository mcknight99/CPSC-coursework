mystery1(a,1).
mystery1(b,2).

mystery2([], 0).
mystery2([A|B], Soln) :-   mystery1(A, C), 
                        mystery2(B, D), 
                        Soln is C + D.