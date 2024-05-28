#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

// Function to convert a character to lowercase
char toLower(char c) {
    return std::tolower(static_cast<unsigned char>(c));
}

int main() {
    std::ifstream inputFile("words_five.txt");
    std::ofstream outputFile("five_frequency.txt");

    if (!inputFile) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    if (!outputFile) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    std::map<char, int> letterCount;

    std::string word;
    while (inputFile >> word) {
        for (char c : word) {
            c = toLower(c);
            if (std::isalpha(c)) {
                letterCount[c]++;
            }
        }
    }

    std::vector<std::pair<char, int>> frequencyCount(letterCount.begin(), letterCount.end());

    // Sort the frequency count in descending order
    std::sort(frequencyCount.begin(), frequencyCount.end(),
              [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
                  return a.second > b.second;
              });

    // Calculate the total count
    int totalCount = 0;
    for (const auto& pair : frequencyCount) {
        totalCount += pair.second;
    }

    // Output the letters sorted by frequency at the start of the file
    for (const auto& pair : frequencyCount) {
        outputFile << pair.first;
    }
    outputFile << std::endl;
    
    // Output the frequency count to the file
    for (const auto& pair : frequencyCount) {
        double percentage = static_cast<double>(pair.second) / totalCount * 100;
        outputFile << pair.first << ": " << pair.second << " (" << percentage << "%)" << std::endl;
    }


    return 0;
}