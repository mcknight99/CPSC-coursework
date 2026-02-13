// sanity checker for mapping strace.txt function calls and tracer_output.txt counts

// basic idea:
// for every line in strace.txt, read the line up until the first "("
// record the count of each syscall name in a map
// for every line in tracer_output.txt, read the syscall number and count
// use the syscall number to get the syscall name from the LUT
// compare the count from tracer_output.txt with the count from strace.txt for that syscall name

// create a LUT for syscall number to name
// thank you https://stackoverflow.com/questions/65532775/a-function-that-gives-it-system-call-number-and-return-its-name
// more specifically # Source - https://stackoverflow.com/a/65533893

// modified script to better suit implementation ease
// only downside now is not knowing the number at a glance but just subtract line number by 4 to get the syscallnum
/*
perl -nE '
  BEGIN { say "#include <vector>\n#include <string>\nstd::vector<std::string> syscallnames = {" }
  if (/__NR_(\w+) (\d+)/) { say qq/\t"$1",/ }
  END { say "};" }' /usr/include/x86_64-linux-gnu/asm/unistd_64.h > syscallnames.h
*/

#include "syscallnames.h"
// syscallnames, after the modified script, is now a std::vector<std::string> called syscallnames
#define MAX_SYSCALL_TYPES 1024 // should match the one in sctracer.c

// now we can do a difference checker between strace_output.txt (arg1) and tracer_output.txt (arg2) to make sure the counts match up for each syscall number and that the names match up for the syscall numbers

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <cstring> // for strcmp
#include <iomanip>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <strace_output.txt> <tracer_output.txt>\n";
        return 1;
    }

    const char *strace_file = argv[1];
    const char *tracer_file = argv[2];

    // Read strace output and count syscalls
    std::unordered_map<std::string, unsigned long> strace_counts;
    std::ifstream strace_data(strace_file);
    std::string line;
    while (std::getline(strace_data, line))
    {
        std::cout << "Parsing strace line: " << line << "\n";
        size_t pos = line.find('(');
        if (pos != std::string::npos)
        {
            // extract syscall name up to first '(' for each line
            std::string syscall_name = line.substr(0, pos);
            // increment count for this syscall name
            strace_counts[syscall_name]++;
        }
        std::cout << "found new syscall: " << line.substr(0, pos) << "\n";
    }
    strace_data.close();

    // print all syscall names read in for debugging
    std::cout << "Syscalls found in strace output:\n";
    for (const auto &pair : strace_counts)
    {
        std::cout << pair.first << ": " << pair.second << ", ";
    }
    std::cout << "\n\n";

    // Read tracer output and compare counts
    std::ifstream tracer_data(tracer_file);
    unsigned long syscall_num, tracer_count;
    bool success = true;
    while (tracer_data >> syscall_num >> tracer_count)
    {
        if (syscall_num >= MAX_SYSCALL_TYPES)
        {
            // should never happen, correct? there's less than 400. just in case though as extra sanity check
            std::cerr << "Syscall number " << syscall_num << " exceeds max limit\n";
            success = false;
            continue;
        }

        // get syscall name from LUT, then get count from strace_counts
        std::string syscall_name = syscallnames[syscall_num];
        unsigned long strace_count = strace_counts[syscall_name];

        if (strace_count != tracer_count)
        {
            std::cerr << "Mismatch for syscall " << syscall_name << " (number " << syscall_num << "): "
                      << "strace count = " << strace_count << ", tracer count = " << tracer_count << "\n";
            success = false;
        }
    }
    tracer_data.close();

    if (success)
    {
        std::cout << "All syscall counts match between strace and tracer output.\n";
        // return 0;
    }
    else
    {
        std::cout << "Some syscall counts did not match. See errors above.\n";
        // return 1;
    }

    // print the following for easier debugging to cout: STRACE_NAME | SYSCALLNUM | STRACE_COUNT | TRACER_COUNT
    std::cout << "\nDetailed counts:\n";
    std::cout << std::left << std::setw(20) << "Syscall Name"
              << std::setw(15) << "Syscall Num"
              << std::setw(15) << "Strace Count"
              << std::setw(15) << "Tracer Count" << "\n";
    std::cout << std::string(65, '-') << "\n";
    // make sure to print by strace_name OR syscallnum in case one or the other is missing in one of the outputs (print "NOT FOUND" if missing)

    for (const auto &pair : strace_counts)
    {
        const std::string &syscall_name = pair.first;
        unsigned long strace_count = pair.second;

        // find syscallnum for this syscall_name
        int syscall_num = -1;
        for (size_t i = 0; i < syscallnames.size(); i++)
        {
            if (syscallnames[i] == syscall_name)
            {
                syscall_num = i;
                break;
            }
        }

        // find tracer count for this syscall_name
        unsigned long tracer_count = 0;
        if (syscall_num != -1)
        {
            std::ifstream tracer_data(tracer_file);
            unsigned long num, count;
            bool found = false;
            while (tracer_data >> num >> count)
            {
                if (num == static_cast<unsigned long>(syscall_num))
                {
                    tracer_count = count;
                    found = true;
                    break;
                }
            }
            tracer_data.close();
            if (!found)
            {
                std::cerr << "\tSyscall " << syscall_name << " (number " << syscall_num << ") not found in tracer output\n";
                tracer_count = 0; // or some indicator of not found
            }
        }
        else
        {
            std::cerr << "\tSyscall name " << syscall_name << " not found in LUT\n";
            syscall_num = -1; // or some indicator of not found
        }

        std::cout << std::left << std::setw(20) << syscall_name
                  << std::setw(15) << (syscall_num != -1 ? std::to_string(syscall_num) : "NOT FOUND")
                  << std::setw(15) << strace_count
                  << std::setw(15) << (tracer_count > 0 ? std::to_string(tracer_count) : "NOT FOUND") << "\n";
    }
}