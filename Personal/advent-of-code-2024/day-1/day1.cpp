// Samuel Pupke
// Date: Dec 1 2024
// Description: Advent of Code 2024 Day 1

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

int main()
{
    std::ifstream file("input.txt");
    // we have to sort both lists from lowest to highest value
    // so we need two vectors: one for the first list and one for the second list

    // the file is formatted as follows: [number-list1]   [number-list2]
    // so we can read the file line by line and split the line into two numbers
    // then we can add the numbers to the respective vectors

    std::vector<int> list1;
    std::vector<int> list2;

    std::string line;
    while (std::getline(file, line))
    {
        // split the line into two numbers
        int number1;
        int number2;
        std::istringstream iss(line);
        iss >> number1 >> number2;

        // add the numbers to the respective vectors
        list1.push_back(number1);
        list2.push_back(number2);
    }

    // sort the vectors
    std::sort(list1.begin(), list1.end());
    std::sort(list2.begin(), list2.end());

    // now all we do is add to the sum of absolute difference between the two lists
    int sum = 0;
    for (int i = 0; i < list1.size(); i++)
    {
        sum += std::abs(list1[i] - list2[i]);
    }

    // print the result
    std::cout << sum << std::endl;

    return 0;
}