cr:
	make compile
	make run

compile:
	g++ generate.cpp -c -o generate.o
	g++ main.cpp generate.o -o main

run:
	./main

clean:
	del *.o main