cr:
	@ echo ####################################################################################################################################################################################################
	make compile
	make run

compile:
	g++ HashiMemImplementation.cpp -o HashiMemImplementation.o -c -Wall -Werror -Wpedantic
	g++ HashiArrayImplementation.cpp -o HashiArrayImplementation.o -c -Wall -Werror -Wpedantic
	g++ main.cpp HashiMemImplementation.o HashiArrayImplementation.o -o main.exe -Wall -Werror -Wpedantic -save-temps -Og

run:
	./main.exe

clean:
	del hashi-solver
	del *.o
	del *.exe
	del *.ii
	del *.s