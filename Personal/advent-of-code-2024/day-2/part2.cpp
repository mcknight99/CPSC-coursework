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
bool checkStrictlySafe(std::vector<int> levels);
bool tryUnsafeDampening(std::vector<int> levels);
bool forceUnsafeDampening(std::vector<int> levels);
std::string vectorToString(std::vector<int> levels);

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
    // in part two, we need to count how many unsafe entries there are in a line. if there is more than one, the line is unsafe.

    // here's how im thinking about solving it:
    // 1: check if the levels are strictly increasing or strictly decreasing. if they are, the line is safe. (helper function: checkStrictlySafe)
    // 2: if the levels are not strictly increasing or strictly decreasing, we need to check how many unsafe entries there are. (helper function: tryUnsafeDampening)
    // 2.1: check each entry, and if the entry is unsafe, re-test without that entry. if the line is safe, the line is safe.
    // 2.2: this can be a "recursive" function where there is a boolean in the params to determine if the single entry dampening has been used or not
    // 2.3: if the line is still unsafe after dampening, return false up the stack to the original call. if the line is safe, return true up the stack to the original call.

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
        // using operator logic with checkStrictlySafe first in the or statement to avoid running tryUnsafeDampening if the line is already safe
        // checkStrictlySafe is effectively just part 1
        if (checkStrictlySafe(levels) || forceUnsafeDampening(levels))
        {
            safeReports++;
        }
    }

    // output the result
    std::cout << "Safe reports: " << safeReports << std::endl;
}

// check if the levels are strictly increasing
bool isStrictlyIncreasing(std::vector<int> levels)
{
    for (int i = 0; i < levels.size() - 1; i++)
    {
        // if we check that it is strictly increasing, we only need to limit the difference to 3 because we know the levels are increasing (difference cannot be 0 or less)
        // pay attention to the order of the levels in the difference comparison
        if (levels[i] >= levels[i + 1])
        {
            return false;
        }
    }
    return true;
}

// check if the levels are strictly decreasing
bool isStrictlyDecreasing(std::vector<int> levels)
{
    for (int i = 0; i < levels.size() - 1; i++)
    {
        // if we check that it is strictly decreasing, we only need to limit the difference to 3 because we know the levels are decreasing (difference cannot be 0 or less)
        // pay attention to the order of the levels in the difference comparison
        if (levels[i] <= levels[i + 1])
        {
            return false;
        }
    }
    return true;
}

// check if there are no unsafe differences
bool isStrictlySafeDifferences(std::vector<int> levels)
{
    for (int i = 0; i < levels.size() - 1; i++)
    {
        int dif = abs(levels[i + 1] - levels[i]);
        if (dif > 3 || dif == 0)
        {
            return false;
        }
    }
    return true;
}

// check if the levels are strictly increasing or strictly decreasing and differences are between 1 and 3.
bool checkStrictlySafe(std::vector<int> levels)
{
    if (isStrictlySafeDifferences(levels) && (isStrictlyIncreasing(levels) || isStrictlyDecreasing(levels)))
    {
        return true;
    }
    return false;
}

// check if an entry can be strictly safe if dampened by removing the first unsafe entry and running it through the checkStrictlySafe function
bool tryUnsafeDampening(std::vector<int> levels)
{
    std::cout << "Trying unsafe dampening on " << std::endl;
    for (int i = 0; i < levels.size(); i++)
    {
        std::cout << levels[i] << " ";
    }
    std::cout << std::endl;

    // check each entry to see if it is unsafe
    for (int i = 0; i < levels.size() - 1; i++)
    {
        int dif = abs(levels[i + 1] - levels[i]);
        if (dif > 3 || dif == 0)
        {
            std::cout << "Unsafe entry found (" << levels[i] << " at index " << i << ")" << std::endl;
            // if the entry is unsafe, remove it and check if the line is safe
            std::vector<int> dampenedLevels;
            for (int j = 0; j < levels.size(); j++)
            {
                if (j != i)
                {
                    dampenedLevels.push_back(levels[j]);
                }
            }
            std::cout << "\tDampened levels: ";
            for (int j = 0; j < dampenedLevels.size(); j++)
            {
                std::cout << dampenedLevels[j] << " ";
            }
            std::cout << std::endl;

            // this can be more efficient by just checking indexes around the unsafe entry instead of sending a new vector into the function
            // but this gets the job done and is more readable
            if (checkStrictlySafe(dampenedLevels))
            {
                std::cout << "\tLine is safe after dampening." << std::endl;
                return true;
            }
        }
    }

    // if the line is still unsafe after dampening, return false
    std::cout << "\tLine is still unsafe after dampening." << std::endl;
    return false;
}

// instead of trying to find an unsafe entry, we will just brute force and remove each entry one by one and check if the line is safe
bool forceUnsafeDampening(std::vector<int> levels)
{
    std::string errorstring;
    errorstring += "Forcing unsafe dampening on " + vectorToString(levels) + "\n";

    for (int i = 0; i < levels.size(); i++)
    {
        std::vector<int> dampenedLevels;
        for (int j = 0; j < levels.size(); j++)
        {
            if (j != i)
            {
                dampenedLevels.push_back(levels[j]);
            }
        }

        if (checkStrictlySafe(dampenedLevels))
        {
            errorstring += "\t\t\t\t\tSucessful dampened levels: " + vectorToString(dampenedLevels) + "\n";
            errorstring += "\tLine is safe after dampening";
            errorstring += " by removing " + std::to_string(levels[i]) + " at index " + std::to_string(i) + "\n";
            std::cout << errorstring;
            return true;
        }
    }
    return false;
}

std::string vectorToString(std::vector<int> levels)
{
    std::string str = "";
    for (int i = 0; i < levels.size(); i++)
    {
        str += std::to_string(levels[i]);
        if (i != levels.size() - 1)
        {
            str += " ";
        }
    }
    return str;
}