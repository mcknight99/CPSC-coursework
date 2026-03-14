# in this project, we will make a static library called "libmythreads.a"
# programs will link to this library when compiled and become part of the compiled program
# from part 4 of project 2 description, compile by:
# gcc -g -Wall -c a.c b.c 
# ar -cvrs libmythreads.a a.o b.o
# he also recommends having the makefile delete the old library and recreate it instead of updating it
# 	(basically just run a clean compile every time, but don't actually rm at the start of compile 
# 	because if it fails then the makefile fails and we dont want the autograder to fail compilation
#  	so have a personal macro for clean compile run (my usual "ccr") but "tests" instead of "run")

compile:
# could end up being a problem child, delete the rm lines if so
	rm -f libmythreads.a 
	gcc -g -Wall -c b.c 
	ar -cvrs libmythreads.a b.o
	rm -f *.o

clean:
	rm -f *.o libmythreads.a
	rm -f cooperative_test preemptive_test lock_test
	rm -f -r tar_output

tests: 
	make
	gcc -g -Wall -o cooperative_test cooperative_test.c libmythreads.a
	./cooperative_test
	gcc -g -Wall -o preemptive_test preemptive_test.c libmythreads.a
#./preemptive_test
	gcc -g -Wall -o lock_test lock_test.c libmythreads.a
	./lock_test

ccr: 
	make clean
	make compile
	make tests

tar:
	tar cvzf project2.tgz README makefile b.c 
	rm -rf tar_output
	mkdir tar_output
	mv project2.tgz tar_output/
	cp mythreads.h tar_output/
	tar -xvzf tar_output/project2.tgz -C tar_output/
	tar cvzf project2.tgz README makefile b.c 

bigtar:
	tar cvzf bigproject2.tgz a.c b.c coop_expected.txt cooperative_test.c lock_test.c locktest_expected.txt makefile mythreads.h preemptive_test.c project2.pdf README 
