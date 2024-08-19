#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream inputFile("words_alpha.txt");
    std::ofstream outputFile("words_five.txt");

    if (!inputFile)
    {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    if (!outputFile)
    {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    std::string word;
    while (inputFile >> word)
    {
        if (word.length() == 5)
        {
            outputFile << word << std::endl;
        }
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Five-letter words have been written to words_five.txt." << std::endl;

    return 0;
}