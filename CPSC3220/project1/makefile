# run leakcount.c to output memory leaks
# mem shim is the .so that intercepts malloc/free calls
# test_mem_leak.c is an example program that leaks memory

# i usually like to use -Werror -Wpedantic but i cant figure out dlsym's warning
# ISO C forbids initialization between function pointer and void*

compile:
	gcc -shared -fPIC -o mem_shim.so mem_shim.c -ldl -pthread -Wall
	gcc -o leakcount leakcount.c -ldl -pthread -Wall
	gcc -o sctracer sctracer.c -Wall

compile_tests:
	gcc -o test_mem_leak test_mem_leak.c -Wall
	gcc -o test_tracer test_tracer.c -Wall

run: 
	./leakcount ./test_mem_leak
	./sctracer ./test_tracer

clean:
	rm -f mem_shim.so leakcount test_mem_leak sctracer test_tracer

ccr: 
	make clean
	make compile
	make run

tar:
	tar cvzf project1.tgz README makefile mem_shim.c leakcount.c sctracer.c
	mkdir tar-test
	mv project1.tgz tar-test
	tar -xvzf tar-test/project1.tgz
	tar cvzf project1.tgz README makefile mem_shim.c leakcount.c sctracer.c

cleantartest: 
	rm -rf tar-test