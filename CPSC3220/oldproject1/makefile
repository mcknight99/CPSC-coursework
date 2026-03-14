# run leakcount.c to output memory leaks
# mem shim is the .so that intercepts malloc/free calls
# test_mem_leak.c is an example program that leaks memory

# i usually like to use -Werror -Wpedantic but i cant figure out dlsym's warning:
# ISO C forbids initialization between function pointer and void*

compile:
	gcc -shared -fPIC -o mem_shim.so mem_shim.c -ldl -pthread -Wall
	gcc -o leakcount leakcount.c -ldl -pthread -Wall
	gcc -o sctracer sctracer.c -Wall

tests:
	gcc -o test_mem_leak test_mem_leak.c -Wall
	gcc -o test_tracer test_tracer.c -Wall

run: 
	./leakcount ./test_mem_leak 
	./leakcount ./test_mem_leak 5
	./leakcount ./test_mem_leak 10 7
	./sctracer ./test_tracer tracer_output.txt
	./sctracer "./test_tracer 1" tracer_output1.txt
	./sctracer "./test_tracer 2 3" tracer_output23.txt

clean:
	rm -f mem_shim.so leakcount test_mem_leak sctracer test_tracer a.out tracer_output.txt tracer_test.txt tracer_output1.txt tracer_output23.txt map_strace_and_output strace_output.txt

ccr: 
	make clean
	make compile
	make run

tar:
	make cleantartest
	tar cvzf project1.tgz README makefile mem_shim.c leakcount.c sctracer.c
	mkdir tar-test
	mv project1.tgz tar-test
	tar cvzf project1.tgz README makefile mem_shim.c leakcount.c sctracer.c
	tar -xvzf tar-test/project1.tgz

bigtar:
	make cleantartest
	tar cvzf project1.tgz README makefile mem_shim.c leakcount.c sctracer.c test_tracer.c syscallnames.h map_strace_and_output.cpp test_mem_leak.c
	mkdir tar-test
	mv project1.tgz tar-test
	tar cvzf project1.tgz README makefile mem_shim.c leakcount.c sctracer.c test_tracer.c syscallnames.h map_strace_and_output.cpp test_mem_leak.c
	tar -xvzf tar-test/project1.tgz

cleantartest: 
	rm -rf tar-test


diff:
	make compile
	make tests
	strace -o strace_output.txt ./test_tracer 20 30 12 
	./sctracer "./test_tracer 20 30 12" tracer_output.txt
	g++ map_strace_and_output.cpp -o map_strace_and_output
	./map_strace_and_output strace_output.txt tracer_output.txt

newsyscallnames:
	echo abc | perl -nE '  BEGIN { say "#include <vector>\n#include <string>\nstd::vector<std::string> syscallnames = {" }  if (/__NR_(\w+) (\d+)/) { say qq/\t"$1",/ }  END { say "};" }' /usr/include/x86_64-linux-gnu/asm/unistd_64.h > syscallnames.h
#perl -nE '  BEGIN { say "#include <vector>\n#include <string>\nstd::vector<std::string> syscallnames = {" }  if (/__NR_(\w+) (\d+)/) { say qq/\t"$1",/ }  END { say "};" }' /usr/include/x86_64-linux-gnu/asm/unistd_64.h > syscallnames.h