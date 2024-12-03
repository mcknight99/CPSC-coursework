cr: compile run

crc: compile run clean

compile:
	gcc ppmUtil.c -c
	gcc ppmDriver.c ppmUtil.o -o ppmDriver

run:
	./ppmDriver "input4.ppm" "output4.ppm"
	./ppmDriver "commentTest.ppm" "outputCommentTest.ppm"
	./ppmDriver "commentTest2.ppm" "outputCommentTest2.ppm"
	./ppmDriver "poohTest.ppm" "outputPoohTest.ppm"
	./ppmDriver "poohWComments.ppm" "outputPoohWComments.ppm"

clean:
	del ppmDriver ppmUtil.o