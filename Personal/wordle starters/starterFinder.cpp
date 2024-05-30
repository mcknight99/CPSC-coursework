#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <fstream>

bool isValidPair(const std::string &word1, const std::string &word2)
{
    // Check if either word ends with 's'
    if (word1.back() == 's' || word2.back() == 's')
    {
        return false;
    }

    // Check if the two words share any letters
    std::unordered_set<char> letters(word1.begin(), word1.end());
    for (char c : word2)
    {
        if (letters.count(c) > 0)
        {
            return false;
        }
    }

    // Check if either word has duplicate letters
    std::string sortedWord1 = word1;
    std::string sortedWord2 = word2;
    std::sort(sortedWord1.begin(), sortedWord1.end());
    std::sort(sortedWord2.begin(), sortedWord2.end());
    if (std::adjacent_find(sortedWord1.begin(), sortedWord1.end()) != sortedWord1.end() ||
        std::adjacent_find(sortedWord2.begin(), sortedWord2.end()) != sortedWord2.end())
    {
        return false;
    }

    return true;
}

std::vector<std::string> readWordsFromFile()
{
    std::vector<std::string> words;
    std::ifstream inputFile("words_five.txt");
    std::string word;

    while (inputFile >> word)
    {
        words.push_back(word);
    }

    return words;
}

// Written modularly to allow for easy changes to use better scoring methods
int calculateScore(const std::string &word1, const std::string &word2)
{
    int score = 0;
    std::string preferredLetters = "aesoriltnudcymphbgkfwvzjxq";

    // words being passed in do not have duplicate letters or shared letters, so we can just check if the preferred letters are in the words once and add the bias to the score

    for (char c : preferredLetters)
    {
        if (word1.find(c) != std::string::npos)
        {
            // Uses the size of the preferredLetters string to be flexible to set bias to unwanted letters to zero-
            // be wary, this also means they have equal bias when removed (say if we only had the top 10, anything not in the top 10 would have a bias of 0 and q and d would have the same bias which is non-ideal)
            score += preferredLetters.size() - preferredLetters.find(c);
        }
        if (word2.find(c) != std::string::npos)
        {
            score += preferredLetters.size() - preferredLetters.find(c);
        }
    }

    return score;
}

std::vector<std::tuple<std::string, std::string, int>> findTopScoringPairs(const std::vector<std::string> &words)
{
    std::vector<std::tuple<std::string, std::string, int>> topPairs;
    std::vector<int> scores;

    for (size_t i = 0; i < words.size(); ++i)
    {
        for (size_t j = i + 1; j < words.size(); ++j)
        {
            if (!isValidPair(words[i], words[j]))
            {
                continue;
            }
            const std::string &word1 = words[i];
            const std::string &word2 = words[j];

            int score = calculateScore(word1, word2);

            // Update the topPairs and scores vectors
            if (topPairs.size() < 10)
            {
                topPairs.push_back(std::make_tuple(word1, word2, score));
                scores.push_back(score);
                std::cout << "New top pair: " << word1 << " " << word2 << " with score " << score << std::endl;
            }
            else
            {
                // Find the index of the lowest score in the scores vector
                auto minScoreIt = std::min_element(scores.begin(), scores.end());
                size_t minScoreIndex = std::distance(scores.begin(), minScoreIt);

                // Replace the lowest scored if the new score is higher
                if (score > *minScoreIt)
                {
                    topPairs[minScoreIndex] = std::make_tuple(word1, word2, score);
                    scores[minScoreIndex] = score;
                    std::cout << "New top pair: " << word1 << " " << word2 << " with score " << score << std::endl;
                }
            }
        }
    }

    return topPairs;
}

int main()
{
    std::vector<std::string> words = readWordsFromFile();
    std::vector<std::tuple<std::string, std::string, int>> topPairs = findTopScoringPairs(words);

    std::cout << "Top 10 pairs:" << std::endl;
    for (const auto &pair : topPairs)
    {
        std::cout << std::get<0>(pair) << " " << std::get<1>(pair) << " with score " << std::get<2>(pair) << std::endl;
    }

    std::ofstream outputFile("best_starters.txt");
    for (const auto &pair : topPairs)
    {
        outputFile << std::get<0>(pair) << " " << std::get<1>(pair) << " with score " << std::get<2>(pair) << std::endl;
    }
    outputFile.close();

    return 0;
}