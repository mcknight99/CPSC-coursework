cr:
	@ echo ####################################################################################################################################################################################################
	make compile
	make run

compile:
	g++ HashiMemImplementation.cpp -o HashiMemImplementation.o -c -Wall -Werror -Wpedantic
# g++ HashiArrayImplementation.cpp -o HashiArrayImplementation.o -c -Wall -Werror -Wpedantic
# need to include HashiArrayImplementation.o below when implementing
	g++ main.cpp HashiMemImplementation.o  -o main.exe -Wall -Werror -Wpedantic

run:
	./main.exe

clean:
	del *.o
	del *.exe
	del *.ii
	del *.s