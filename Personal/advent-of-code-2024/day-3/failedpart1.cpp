#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

// today's part 1 we have to find each instance of "mul(X,Y)" in a jumble from our input.txt
// we sum each instance's product as our output
// let's write a function to read in to find the next matching mul by the following:
// "mul(%d,%d)" where %d is an integer
// it does not accept inputs with spaces, so we can use a simple fscanf to find the next input (thank you 2310 lab 9)

// 2217 too low
// 209565887 too high
// 191183308 correct

// helper function constructors
int findNextMulInString(std::string str, int &n1, int &n2);

int main()
{
    std::ifstream file("testinput.txt");
    std::string line;
    int sum = 0;
    while (std::getline(file, line))
    {
        std::cout << "new line: " << line << std::endl;
        int n1=0, n2=0;
        while (findNextMulInString(line, n1, n2) != -1)
        {
            sum += n1 * n2;
            // adding 3 effectively deletes the mul from the string no matter how long the whole mul matching string is
            line = line.substr(findNextMulInString(line, n1, n2) + 3);
            std::cout << "adding " << n1 << "*" << n2 << " to sum" << std::endl;
            std::cout << "substringed line: " << line << std::endl;
        }
    }
    std::cout << sum << std::endl;
}

// return the index of the start of the next mul in the string
// return -1 if no mul is found
// pass by reference the two integers found in the mul for ease of use
// to avoid finding a fake mul, we need to match n1 and n2 to this found mul
// if we don't find a match, we move onto the next found mul in the string until either
// we find a match or we run out of muls
int findNextMulInString(std::string str, int &n1, int &n2)
{
    int n1_, n2_; // safety to avoid changing n1 and n2 if we find a bad match, it shouldn't provide much help anymore 
    int found = sscanf(str.c_str(), "mul(%d,%d)", &n1_, &n2_);
    if (found == 2)
    {
        n1 = n1_;
        n2 = n2_;
        return str.find("mul");
    }
    return -1;
}