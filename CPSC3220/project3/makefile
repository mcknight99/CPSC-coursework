compile:
	gcc -g -Wall -fPIC -shared allocator.c -o libmyalloc.so

optimized:
	gcc -O2 -DNDEBUG -fPIC -shared allocator.c -o libmyalloc.so

debug:
	gcc -Wall -g -o test allocator.c test.c

tests:
	gcc -g -Wall -L. -lmyalloc test.c -o test
	% LD_PRELOAD=./libmyalloc.so ./test

tar:
	tar cvzf project3.tgz README makefile allocator.c