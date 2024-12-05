#include <iostream>
#include <fstream>
#include <string>

// i got frustrated with whatever bug was causing a segfault in part 1 so this is me restarting with a fresh slate

// answer: 191183308

// today's part 1 we have to find each instance of "mul(X,Y)" in a jumble from our input.txt
// and multiply X by Y and add it to a total sum

// let's make a helper function that finds the next instance of "mul(X,Y)" in a given string
// let's use sscanf to easily parse the string and get the values of X and Y
// let's pass X and Y by reference so we can return them that way
// let's return the position of the current instance of "mul(X,Y)" in the string plus the length of the mulStr so we can delete it in the main logic
// we have to do this by finding the entire instance of "mul(X,Y)" by assembling a string from the two found values and doing a find on the input string for that entire string

// helper function prototype
size_t findNextMul(const std::string &input, int &X, int &Y);

int main()
{
    // open the input file
    std::ifstream input("input.txt");
    if (!input.is_open())
    {
        std::cerr << "Could not open input file" << std::endl;
        return 1;
    }
    // the input file has several lines, so we need to read them all
    int sum = 0;
    std::string line;
    while (std::getline(input, line))
    {
        std::cout << "testing line: " << line << std::endl;
        // we need to find each instance of "mul(X,Y)" in the line
        // we need to multiply X by Y and add it to the sum
        // we need to delete the string up to the instance of "mul(X,Y)" from the line (substr to index)
        // we need to repeat this until there are no more instances of "mul(X,Y)" in the line
        // we need to do this for each line in the input file
        int X, Y;
        size_t pos;
        while ((pos = findNextMul(line, X, Y)) != std::string::npos)
        {
            sum += X * Y;
            line = line.substr(pos);
            std::cout << "substr: " << line << std::endl;
        }
    }

    // print the sum
    std::cout << sum << std::endl;

    return 0;
}

// helper function to find the next instance of "mul(X,Y)" in a given string
// start by doing a sscanf on the input string to find the values of X and Y
// then assemble a string from the two values and do a find on the input string
// then we return the instance of that string in the input string and pass X and Y by reference
// returning the index of the found instance of "mul(X,Y)" in the input string allows us to delete it in the main logic
size_t findNextMul(const std::string &input, int &X, int &Y)
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
            if(input.find(mulStr) == pos - 1){
                std::cout<<"found expected string: "<<mulStr<< " at pos: "<<pos-1<<std::endl;
            } else {
                std::cout<<"this is a false match; perhaps incorrect formatting after the \"mul(X,Y\". Continuing to search for a valid mul in this string" << std::endl;
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
