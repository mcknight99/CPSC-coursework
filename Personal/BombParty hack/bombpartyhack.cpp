/*
BombParty is an online game that gives you a short string of letters and you have a time limit to come up with a word that contains that string.
You face other players, and upon not coming up with a word in that time limit, you lose a life. The game continues until only one player remains.
You may regain a life if you use every letter in the alphabet over the span of your guesses. Once you regain a life by using every letter once,
    it allows you to get another life again by using every letter in the alphabet again, resetting your letter counter.
The goal of this program is to find the word with the optimal words based on the number of unique letters in the word.
This program must also be able to run within the time limit, so it must be quick.

ex:
the game gives you the string "lu"
the word "lucky" would be a better word than "lulu" because it has more unique letters
or if you were this program, you would use "counterrevolutionarys"

Word lists graciously taken from Clemson student linux servers.

Here is the original website where I found the game, but it is strictly multiplayer:
https://jklm.fun/

Here is a clone website that is also single player friendly:
https://bombparty-clone.fly.dev/
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <list>

const std::string DEFAULT_COLOR = "\033[0m";       // white text
const std::string SUBSTRING_COLOR = "\033[1;33m";  // yellow text
const std::string NEW_CHAR_COLOR = "\033[1;36m";   // teal text
const std::string BACKGROUND_COLOR = "\u001B[40m"; // black highlight background

// Function to read words from a file and store them in a set
bool readWordsFromFile(const std::string &filename, std::unordered_set<std::string> &wordSet)
{
    int total = 0;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return false;
    }
    std::string word;
    while (file >> word)
    {
        bool fail = false;
        // Convert word to lowercase and skip words with non-alphabetic characters
        std::string filteredWord;
        for (char c : word)
        {
            if (std::isalpha(c))
            {
                filteredWord += std::tolower(c);
            }
            else
            {
                fail = true;
            }
        }
        if (!filteredWord.empty() && !fail)
        {
            wordSet.insert(filteredWord);
            ++total;
        }
    }
    file.close();
    std::cout << "Total words read: " << total << std::endl;
    return true;
}

// Function to print a word with color formatting
void printWordWithFormatting(const std::string &word, const std::string &substring, std::list<char> differentLetters)
{
    std::cout << "Word found (+" << differentLetters.size() << "): " << BACKGROUND_COLOR;
    for (int i = 0; i < word.length(); ++i)
    {
        // Check if the letter is part of the substring
        if (word.substr(i, substring.length()) == substring)
        {
            // Print the substring in yellow
            std::cout << SUBSTRING_COLOR << substring;
            i += substring.length() - 1;
        }
        // Check if the letter is one of the different and unused letters
        else if (find(differentLetters.begin(), differentLetters.end(), word[i]) != differentLetters.end())
        {
            // Print new letters in blue
            std::cout << NEW_CHAR_COLOR << word[i];
            differentLetters.remove(word.at(i));
        }
        else
        {
            // Print the letter with normal color
            std::cout << DEFAULT_COLOR << BACKGROUND_COLOR << word[i];
        }
    }
    std::cout << DEFAULT_COLOR << std::endl;
}

// Function to calculate the score of a word based on the number of unique letters
int calculateScore(const std::string &word, std::unordered_map<char, bool> usedLetters)
{
    int score = 0;
    for (char c : word)
    {
        if (!usedLetters[c])
        {
            usedLetters[c] = true;
            ++score;
        }
    }
    return score;
}

// Function to find the word with the best score using BFS
std::string findWord(const std::string &input, const std::unordered_set<std::string> &wordSet, const std::unordered_map<char, bool> &usedLetters)
{
    int currBestScore = -1;
    std::string bestWord;

    for (const std::string &word : wordSet)
    {
        if (word.find(input) != std::string::npos) // also find at least one of the unused letters to avoid calculating a score of 0. pick at random to avoid bias
        {
            int currScore = calculateScore(word, usedLetters);
            if (currScore > currBestScore)
            {
                // std::cout << "test? " << word << " " << currScore << std::endl;
                currBestScore = currScore;
                bestWord = word;
            }
        }
    }

    return bestWord;
}

int main()
{
    std::unordered_set<std::string> wordSet; // Set to store words
    std::string filename = "words.txt";
    if (!readWordsFromFile(filename, wordSet))
    {
        return 1;
    } // Read words from file and store them

    std::unordered_map<char, bool> usedLetters; // Map to track used letters
    char choice;

    do
    {
        std::cout << "Enter a substring: ";
        std::string input;
        std::cin >> input;
        std::string word = findWord(input, wordSet, usedLetters);

        // for showing used letters: tempUsedLetters xor with new usedLetters after findWord(). The first instance in the new word is a newly used letter.
        std::unordered_map<char, bool> tempUsedLetters = usedLetters;
        // Vector to store the differences
        std::list<char> differences;
        for (char c : word)
        {
            usedLetters[c] = true;   // Mark letters used
            if (!tempUsedLetters[c]) // Check if the letter is newly used
            {
                differences.push_back(c);  // Store the difference
                tempUsedLetters[c] = true; // Mark the letter as used in the temporary map
            }
        }

        if (!word.empty())
        {
            // for showing contained user string: word.find(input) != std::string::npos and word length to highlight the substring
            // use different coloring for substring and used letters- one is highlight, other is text color?
            // std::cout << "Word found (+" << calculateScore(word, usedLetters) << "): " << word << std::endl;
            printWordWithFormatting(word, input, differences);
        }
        else
        {
            std::cout << "No word found containing the given substring." << std::endl;
        }

        // Output differences
        std::cout << "Differences: ";
        for (char c : differences)
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;

        std::cout << "Unused letters:";
        // Check if all letters have been used
        bool allLettersUsed = true;
        for (char c = 'a'; c <= 'z'; ++c)
        {
            if (!usedLetters[c])
            {
                allLettersUsed = false;
                std::cout << c << " ";
            }
        }
        std::cout << std::endl;

        if (allLettersUsed)
        {
            std::cout << "All letters used. Do you want to start again? (y/n): ";
            // std::cin >> choice;
            choice = 'y';        // For testing purposes
            usedLetters.clear(); // Clear used letters map
        }
        else
        {
            choice = 'y';
        }
    } while (choice == 'y' || choice == 'Y');

    std::cout << "Exiting program." << std::endl;

    return 0;
}
