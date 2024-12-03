cr: compile run

compile:
	gcc ppm.c -c -o ppm.o -Wall -Werror -Wpedantic
	gcc shape.c -c -o shape.o -Wall -Werror -Wpedantic
	gcc driver.c ppm.o shape.o -o driver.exe -Wall -Werror -Wpedantic

run:
	./driver input.txt output.ppm

test: compile
	./driver my-test-input.txt my-test-output.ppm

clean:
	del ppm.o shape.o driver.exe *.ppm