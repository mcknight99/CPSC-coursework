#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

// today's part two we only add to the sum after "do()"s and don't add after "don't()"s
// basically sum between a do and a don't
// when we reach a don't, scan for the next valid do (using a similar method to the day 1 helper)
// control loop: start as a "do" at the beginning of the file
//     search for the next "don't" and sum between the two
// for ease of control, we can use the end of file as a "don't" and sum between the previous "do" and the end of the file
//     search for the next "do"
//     repeat until we reach the end of the file
// substring between a do and a don't and process it like part 1. repeat until we run out of do windows

// part 2 should be less than the answer to part 1
// 2217 too low
// 209565887 too high
// 191183308 correct

// part 2:
// 53587524 too low
// 191183308 too high (answer to part 1)

// helper function constructors
size_t findNextMulInString(const std::string &input, int &X, int &Y);
std::string findNextDoWindowInString(std::string str, bool startAtDo);
size_t findNextDoInString(std::string str);
size_t findNextDontInString(std::string str);
int day1Solution(std::string str);
size_t findNextMulInString(const std::string &input);

int main()
{
    std::ifstream file("test2input.txt");
    std::string line;
    int sum = 0;
    bool start = true; // start is a single time run flag to act as a "do" at the beginning of the file
    while (std::getline(file, line))
    {
        std::cout << "testing line: " << line << std::endl;
        // we need to find a substring between a "do()" and a "don't()" using helper function findNextDoWindowInString
        // we need to sum the valid muls in that substring using helper function day1Solution
        // we need to add that sum to the total sum
        // we need to repeat this until we run out of do windows

        // we need to start the file as a "do" window
        // we need to find the next "don't" and sum between the two

        while (findNextDoInString(line) != std::string::npos)
        {
            std::string doWindow = findNextDoWindowInString(line, start);
            start = false;
            std::cout << "doWindow: " << doWindow << std::endl;
            sum += day1Solution(doWindow);
            line = line.substr(findNextDontInString(line)+7); // +7 to skip past the "don't()" string
            std::cout << "new substr line: " << line << std::endl;
            std::cout << "2sum: " << sum << std::endl;
        }
    }
    std::cout << sum << std::endl;
}

// helper function for day 1 to return the sum of the valid muls in the given string
int day1Solution(std::string str)
{
    int sum = 0;
    std::cout << "testing line: " << str << std::endl;
    // we need to find each instance of "mul(X,Y)" in the line
    // we need to multiply X by Y and add it to the sum
    // we need to delete the string up to the instance of "mul(X,Y)" from the line (substr to index)
    // we need to repeat this until there are no more instances of "mul(X,Y)" in the line
    // we need to do this for each line in the input file
    int X, Y;
    size_t pos;
    while ((pos = findNextMulInString(str, X, Y)) != std::string::npos)
    {
        sum += X * Y;
        str = str.substr(pos);
        std::cout << "substr: " << str << std::endl;
    }
    std::cout<<"subsum: "<<sum<<std::endl<<std::endl;
    return sum;
}

// helper function to find the next instance of "mul(X,Y)" in a given string
// start by doing a sscanf on the input string to find the values of X and Y
// then assemble a string from the two values and do a find on the input string
// then we return the instance of that string in the input string and pass X and Y by reference
// returning the index of the found instance of "mul(X,Y)" in the input string allows us to delete it in the main logic
size_t findNextMulInString(const std::string &input, int &X, int &Y)
{
    // if no mul exists at all, return npos
    if (input.find("mul") == std::string::npos)
    {
        return std::string::npos;
    }

    int pos = 0; // pos is a manual iterator for the input string to see how far in it takes to match the input format
    std::string mulStr;
    do
    {
        int test = sscanf(input.c_str() + pos, "mul(%d,%d)", &X, &Y);
        mulStr = "mul(" + std::to_string(X) + "," + std::to_string(Y) + ")";
        pos++;
        if (test == 2)
        {
            std::cout << "string: " << input << std::endl;
            std::cout << "mulStr: " << mulStr << std::endl;
            std::cout << "pos-1: " << pos - 1 << std::endl;
            size_t found = input.find(mulStr);
            std::cout << "found: " << found << std::endl;
            // found and pos need to be the same to get rid of edge cases to fully match the desired format
            // using this we can see that when the string is valid, pos = found + 1
            if (input.find(mulStr) == pos - 1)
            {
                std::cout << "found expected string: " << mulStr << " at pos: " << pos - 1 << std::endl;
            }
            else
            {
                std::cout << "this is a false match; perhaps incorrect formatting after the \"mul(X,Y\". Continuing to search for a valid mul in this string" << std::endl;
            }
        }
        // if we reach the end of the string without finding a valid mul, return npos
        if (pos == input.length())
        {
            std::cout<<"no valid mul found in string "<<input<<std::endl;
            return std::string::npos;
        }
    } while (input.find(mulStr) != pos - 1);
    std::cout << "found mul(" << X << "," << Y << ")" << std::endl
              << std::endl;
    return pos - 1 + mulStr.length();
}

// helper function for nextMulInString without the X and Y in the parameters
size_t findNextMulInString(const std::string &input)
{
    int X, Y;
    return findNextMulInString(input, X, Y);
}

// helper function to find the next do window in the given string (return the do window string)
std::string findNextDoWindowInString(std::string str, bool startAtDo = false)
{
    size_t doPos = findNextDoInString(str);
    size_t dontPos = findNextDontInString(str);
    if (startAtDo)
    {
        std::cout << "starting do at start of file" << std::endl;
        std::cout << "ending at dont: " << dontPos << std::endl;
        return str.substr(0, dontPos);
    }
    else
    {
        std::cout << "starting at do: " << doPos << std::endl;
        std::cout << "ending at dont: " << dontPos << std::endl;
        return str.substr(doPos, str.length() - doPos);
    }
}

// helper function to find the next valid "don't()" after a "do()" (return the index of the start of the next don't in the string)
size_t findNextDontInString(std::string str)
{
    return str.find("don't()");
}

// helper function to find the next "do()" after a "don't()" (return the index of the start of the next do in the string)
size_t findNextDoInString(std::string str)
{
    return str.find("do()");
}