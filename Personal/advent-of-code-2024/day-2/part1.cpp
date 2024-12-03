// Samuel Pupke
// Date: 2 December 2024
// Advent of Code 2024 Day 2 Part 1

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// helper function prototypes
bool isStrictlyIncreasing(std::vector<int> levels);
bool isStrictlyDecreasing(std::vector<int> levels);

int main()
{
    // the data is formatted as such:
    // each line is a "report", and each value in a report is a "level"
    // we must figure out how many reports are safe
    // reports are safe if both of the following are true
    // 1. The levels are either all strictly increasing or all strictly decreasing.
    // 2. Any two adjacent levels differ by at least one and at most three. (absolute dif between 1 and 3 from one level to the next)
    // example:
    /*
    7 6 4 2 1: Safe because the levels are all decreasing by 1 or 2.
    1 2 7 8 9: Unsafe because 2 7 is an increase of 5.
    9 7 6 2 1: Unsafe because 6 2 is a decrease of 4.
    1 3 2 4 5: Unsafe because 1 3 is increasing but 3 2 is decreasing.
    8 6 4 4 1: Unsafe because 4 4 is neither an increase or a decrease.
    1 3 6 7 9: Safe because the levels are all increasing by 1, 2, or 3.
    So, in this example, 2 reports are safe.
    */

    // open the file
    std::ifstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Could not open file." << std::endl;
        return 1;
    }

    // read the file
    int safeReports = 0;
    std::string line;
    while (std::getline(file, line))
    {
        // lines can be any length, so we can compare while there is still data
        // we will use a vector to store the levels just to make it easier to compare
        std::vector<int> levels;
        int level;
        std::istringstream iss(line);
        while (iss >> level)
        {
            levels.push_back(level);
        }
        // now we can just run each report through the helper functions to check if it is safe
        if (isStrictlyIncreasing(levels) || isStrictlyDecreasing(levels))
        {
            safeReports++;
        }
    }

    // output the result
    std::cout << "Safe reports: " << safeReports << std::endl;
}

// check if the levels are strictly increasing and differences are between 1 and 3.
// return true if this is a safe report
bool isStrictlyIncreasing(std::vector<int> levels)
{
    for (int i = 0; i < levels.size() - 1; i++)
    {
        // if we check that it is strictly increasing, we only need to limit the difference to 3 because we know the levels are increasing (difference cannot be 0 or less)
        // pay attention to the order of the levels in the difference comparison
        if (levels[i] >= levels[i + 1] || levels[i + 1] - levels[i] > 3)
        {
            return false;
        }
    }
    return true;
}

// check if the levels are strictly decreasing and differences are between 1 and 3.
// return true if this is a safe report
bool isStrictlyDecreasing(std::vector<int> levels)
{
    for (int i = 0; i < levels.size() - 1; i++)
    {
        // if we check that it is strictly decreasing, we only need to limit the difference to 3 because we know the levels are decreasing (difference cannot be 0 or less)
        // pay attention to the order of the levels in the difference comparison
        if (levels[i] <= levels[i + 1] || levels[i] - levels[i + 1] > 3)
        {
            return false;
        }
    }
    return true;
}