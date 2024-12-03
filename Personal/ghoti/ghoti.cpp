#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

// Function to load the CMU Pronouncing Dictionary
std::unordered_map<std::string, std::vector<std::string>> loadWordDict(const std::string &filename)
{
    std::unordered_map<std::string, std::vector<std::string>> dictionary;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        // if (line[0] == ';') continue; // Skip comments
        std::istringstream iss(line);
        std::string word;
        iss >> word;
        std::string pronunciation;
        std::vector<std::string> pronunciations;
        // if iss is a # then what comes after is a comment and can be ignored
        while (iss >> pronunciation && pronunciation != "#")
        {
            pronunciations.push_back(pronunciation);
        }
        dictionary[word] = pronunciations;
    }
    std::cout << "Dictionary loaded successfully" << std::endl;
    return dictionary;
}

bool sharedSpellingCheck(const std::string &word1, const std::string &word2)
{
    // check if the two words share letter combinations (2 or 3 letter long combinations)
    // if they do, they are likely to have similar spellings for similar pronounciations
    // if they do not, they are likely to have different spellings for similar pronounciations
    // this isn't the end of the world but it makes finding sillier spellings easier

    // 2 letter combos
    for (int i = 0; i < word1.size() - 1; i++)
    {
        std::string combo = word1.substr(i, 2);
        if (word2.find(combo) != std::string::npos)
        {
            return true;
        }
    }
    // 3 letter combos
    for (int i = 0; i < word1.size() - 2; i++)
    {
        std::string combo = word1.substr(i, 3);
        if (word2.find(combo) != std::string::npos)
        {
            return true;
        }
    }
    // 1 letter combos
    // for(int i = 0; i < word1.size(); i++)
    // {
    //     std::string combo = word1.substr(i, 1);
    //     if (word2.find(combo) != std::string::npos)
    //     {
    //         return true;
    //     }
    // }

    return false;
}

std::pair<std::string, std::vector<std::string>> randomWordPicker(const std::unordered_map<std::string, std::vector<std::string>> &dictionary)
{
    // pick a random word from the list
    // this is to avoid always picking the same word for the same pronounciation

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, dictionary.size() - 1);
    auto it = dictionary.begin();
    std::advance(it, dis(gen));
    return *it;
}

// Function to find alternative spellings
std::vector<std::string> findAlternativeSpellings(const std::string &inputWord,
                                                  std::unordered_map<std::string, std::vector<std::string>> &dictionary)
{
    std::vector<std::string> result;
    // find the input word in the dictionary
    auto it = dictionary.find(inputWord);
    
    // check if input word is in dictionary
    if (it == dictionary.end())
    {
        std::cout << "Word not found in dictionary" << std::endl;
        return result;
    }

    // get the pronunciation of the input word
    const auto &inputPronunciation = it->second;
    std::unordered_map<std::string, std::vector<std::string>> pronunciationWords;
    for (const auto &entry : inputPronunciation)
    {
        pronunciationWords[entry] = std::vector<std::string>();
    }

    // basically do pick random word while pronounciationWords isnt filled yet
    // it is slightly less efficient (? maybe? it's not like the words are sorted by pronunciation so iterating is pretty close to random for pronunciation anywyas) but it makes the resulting words more varied than iterating the same order every time
    for (int i = 0; i < inputPronunciation.size(); i++)
    {
        while (pronunciationWords[inputPronunciation[i]].size() < 3)
        {
            std::pair<std::string, std::vector<std::string>> word = randomWordPicker(dictionary);
            if (word.first == inputWord)
                continue;
            if (sharedSpellingCheck(word.first, inputWord))
                continue;
            for (const auto &pronounciation : word.second)
            {
                for (const auto &inputPron : inputPronunciation)
                {
                    if (pronounciation == inputPron)
                    {
                        if (pronunciationWords[inputPron].size() < 3)
                        {
                            pronunciationWords[inputPron].push_back(word.first);
                            continue; // if we found a word with the same pronounciation, we don't want to check it again and add it to several pronounciations
                        }
                    }
                }
            }
        }
    }

    // now we have a map of words for each pronounciation
    // here it is printed
    for (const auto &entry : pronunciationWords)
    {
        std::cout << "Pronounciation: " << entry.first << std::endl;
        for (const auto &word : entry.second)
        {
            std::cout << word << "(Pronunciation: ";
            for (const auto &pronounciation : dictionary[word])
            {
                std::cout << pronounciation << " ";
            }
            std::cout << ")" << std::endl<<std::endl;
            result.push_back(word);
        }
    }

    return result;
}

int main()
{
    // map of words to their pronunciations
    std::string filename = "cmudict.txt";
    std::unordered_map<std::string, std::vector<std::string>> dictionary = loadWordDict(filename);

    std::string inputWord;
    std::cout << "Enter a word: ";
    std::cin >> inputWord;

    // Convert input word to lowercase to match dictionary format
    std::transform(inputWord.begin(), inputWord.end(), inputWord.begin(), ::tolower);

    std::vector<std::string> alternativeSpellings = findAlternativeSpellings(inputWord, dictionary);

    std::cout << "Alternative spellings for " << inputWord;
    std::cout << "(pronounciation: ";
    for (const auto &pronounciation : dictionary[inputWord])
    {
        std::cout << pronounciation << " ";
    }
    std::cout << ") are:" << std::endl;

    for (const auto &word : alternativeSpellings)
    {
        std::cout << word << std::endl;
    }

    return 0;
    
}