cr: compile run

compile:
	g++ -std=c++17 cpsc3120homework01part01.cpp -c -o cpsc3120homework01part01.o 
	g++ -std=c++17 test01.cpp -o t1main.exe cpsc3120homework01part01.o 
	g++ -std=c++17 cpsc3120homework01part02.cpp -c -o cpsc3120homework01part02.o
	g++ -std=c++17 test02.cpp -o t2main.exe cpsc3120homework01part02.o

run:
	type .\tests\part01test01.input | t1main.exe
	type .\tests\part01test02.input | t1main.exe
	type .\tests\part01test03.input | t1main.exe
	type .\tests\part01test04.input | t1main.exe
	type .\tests\part01test05.input | t1main.exe
	type .\tests\part01test06.input | t1main.exe
	type .\tests\part01test07.input | t1main.exe
	type .\tests\manual.input | t1main.exe
	type .\tests\part02test01.input | t2main.exe
	type .\tests\part02test02.input | t2main.exe
	type .\tests\part02test03.input | t2main.exe
	type .\tests\part02test04.input | t2main.exe
	type .\tests\part02test05.input | t2main.exe
	type .\tests\part02test06.input | t2main.exe
	type .\tests\part02test07.input | t2main.exe