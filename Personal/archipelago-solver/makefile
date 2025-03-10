cr: compile run

compile:
	g++ player.cpp -c -o player.o
	g++ board.cpp -c -o board.o
	g++ interface.cpp -c -o interface.o
	g++ main.cpp player.o board.o interface.o -o main

run:
	./main

clean:
	del -f *.o main.exe