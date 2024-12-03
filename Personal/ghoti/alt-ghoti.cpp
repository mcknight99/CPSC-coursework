#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

bool debug = false;

// How do i teach it that some letter combos cause a sound but don't have a spelling?
// Ex: putting in "payable"

// I tried my best to avoid editing the cmudict, but I had to make some changes to the cmudict to make it work with the altDict
// Current change:
// all occurrences of Y UW0-UW2 in cmudict are now YUW0 and cmudict has spellings for YUW0
//      this fixed an issue of two sound pairing of /y//ü/ as one sound spelling
//      because i would rather have not had to fix that code-side

// Added: Teagan T IY0 G AH0 N

// To do: function to find a word to match a spelling for a sound by disassembly

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

std::unordered_map<std::string, std::vector<std::string>> loadAltDict(const std::string &filename)
{
    // alt dict is stored in the format of
    // SOUND = COMMON_SPELLING ALTERNATE_SPELLING ... ALTERNATE_SPELLING
    // for most sounds.
    // but for similar sounds in the cmudict, the spellings will point to the same sound in the alt phoneme dict
    // here is the format of a sound pointing to another (this means it should reference the spelling list of the sound)
    // FROM_SOUND @ TO_SOUND
    // here is an example format:
    // AA0 = aw a or oor ore oar our augh ar ough au
    // AA1 @ AA0
    // AA2 @ AA0
    // some spellings used to have a - in them, which acted as a "wild card" flag for a letter to be inserted
    // example: /ā/ = ai in snail or a-e in snake or cake
    // but the - was removed because it changes the sound in some cases
    // it might be possible to correct for this by finding a pairing with a following sound and adding the wild card letter
    // but this would require a lot of work and would be difficult to implement in the current system
    std::unordered_map<std::string, std::vector<std::string>> altDict;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string sound;
        iss >> sound;
        std::string spelling;
        std::vector<std::string> spellings;
        while (iss >> spelling)
        {
            spellings.push_back(spelling);
        }
        altDict[sound] = spellings;
    }

    // std::cout << "Alternative dictionary loaded successfully" << std::endl;
    // std::cout << "Sound | Identifier | Spellings" << std::endl;
    // for (const auto &entry : altDict)
    // {
    //     std::cout << entry.first << " | " << entry.second[0] << " | ";
    //     for (int i = 1; i < entry.second.size(); i++)
    //     {
    //         std::cout << entry.second[i] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // we also need to sort the spellings by length longest to shortest to avoid removing substrings of longer spellings when we disassemble a word
    // but we need to keep the first element as the identifier
    // we can do this by removing the first element, sorting the rest, then adding the first element back
    // actually let's sort by shortest to longest to avoid removing false positives of longer substrings
    for (auto &entry : altDict)
    {
        std::string identifier = entry.second[0];
        entry.second.erase(entry.second.begin());
        std::sort(entry.second.begin(), entry.second.end(), [](const std::string &a, const std::string &b)
                  { return a.size() < b.size(); });
        entry.second.insert(entry.second.begin(), identifier);
    }

    std::cout << "Alternative dictionary loaded successfully" << std::endl;
    return altDict;
}

// lets get rid of the sounds in the cmudict that are similar enough to be considered the same
// these are defined by the altDict pointers that point to the same sound
// all sounds in the dictionary should be turned from the actual sound to the sound that it points to
// reference altDict's format:
// FROM_SOUND @ TO_SOUND
void updateWordDict(std::unordered_map<std::string, std::vector<std::string>> &wordDict, std::unordered_map<std::string, std::vector<std::string>> &altDict)
{

    for (auto &entry : wordDict)
    {
        for (size_t i = 0; i < entry.second.size(); i++)
        {
            auto &pronunciation = entry.second[i];
            auto it = altDict.find(pronunciation);
            if (it != altDict.end() && it->second[0] == "@")
            {
                // we found a similar sound, replace it with the sound it points to
                entry.second[i] = it->second[1];
            }
        }
    }
    std::cout << "Word dictionary updated successfully" << std::endl;
}

// Function to get a pronunciation from the dictionary
std::vector<std::string> getPronunciation(const std::string &word, const std::unordered_map<std::string, std::vector<std::string>> &wordDict)
{
    auto it = wordDict.find(word);
    if (it == wordDict.end())
    {
        std::cout << "Word not found in dictionary" << std::endl;
        return std::vector<std::string>();
    }
    return it->second;
}

// Function to pick a random word from the dictionary
std::pair<std::string, std::vector<std::string>> randomWordPicker(const std::unordered_map<std::string, std::vector<std::string>> &wordDict)
{
    // pick a random word from the list
    // this is to avoid always picking the same word for the same pronounciation

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, wordDict.size() - 1);
    auto it = wordDict.begin();
    std::advance(it, dis(gen));
    return *it;
}

// Function to disassemble a word into its phonemes
std::vector<std::string> disassembleWordToSpellings(const std::string &inputWord,
                                                    std::unordered_map<std::string, std::vector<std::string>> &wordDict,
                                                    std::unordered_map<std::string, std::vector<std::string>> &altDict)
{
    // std::cout << "Disassembling word: " << inputWord << std::endl;
    std::string inputWordCopy = inputWord;
    std::vector<std::string> spellingDisassembly;
    std::vector<std::string> wordPronunciation = getPronunciation(inputWordCopy, wordDict);
    // we can disassemble a word by iterating through the word's pronunciations and finding the corresponding spellings and removing them from the word and adding them to the disassembly
    for (size_t i = 0; i < wordPronunciation.size(); i++)
    {
        auto &pronunciation = wordPronunciation[i];
        if (debug)
            std::cout << "Disassembling pronunciation: " << pronunciation << std::endl;
        auto &spellings = altDict[pronunciation];
        bool foundSpelling = false;
        for (auto &spelling : spellings)
        {
            if (spelling == "=" || foundSpelling)
            {
                continue;
            }
            // find the spelling in the word and remove it
            // because we're going in order by pronounciation and spelling, we can just find the first instance of the spelling in the word
            // we want to remove them so we dont duplicate occurrences of the same spelling
            // but what we don't want to do is remove a spelling later in the word of an identical sound (ex. finding spelling "i" and "y" in "dynamics" and removing both the "i" and "y" as matching the first AY0)
            // therefore we should iterate through spellings by starting at the beginning of the word and finding the first spelling and removing it.
            // sometimes we might have leftover letters from incomplete spellings from previous incomplete pronunciation spellings, so we should then iterate through the word until we find a spelling
            // once we find a spelling, we add the incomplete spelling to the previous spelling and remove the incomplete spelling from the word, then match the current spelling to the sound and remove it too
            // we can then continue this process until we have no more spellings to match
            auto it = inputWordCopy.find(spelling);
            if (it != std::string::npos && it == 0)
            {
                if (debug)
                    std::cout << "Found spelling: " << spelling << " of pronunciation: " << pronunciation << " in word: " << inputWordCopy;
                // we found a spelling at the start of the word, add it to the disassembly
                spellingDisassembly.push_back(spelling);
                // remove the spelling from the word
                inputWordCopy.erase(it, spelling.size());
                foundSpelling = true;
                if (debug)
                    std::cout << " and new word is " << inputWordCopy << std::endl;
            }
        }
        // after going through all spellings and not finding a spelling at the start of the word, we can iterate through the word to find the next spelling
        // we can then add the incomplete spelling to the previous spelling and remove the incomplete spelling from the word
        // we can then match the current spelling to the sound and remove it too
        // we can then continue this process until we have no more spellings to match
        if (!foundSpelling)
        {
            for (size_t i = 0; i < inputWordCopy.size() && !foundSpelling; i++)
            {
                for (size_t j = 0; j < spellings.size() && !foundSpelling; j++)
                {
                    // if the word up until the current index contains a spelling, we can add the spelling to the disassembly
                    // basically if we recognize a spelling for the current sound in the word, we can add it to the disassembly, while fixing the previous spelling by adding the incomplete spelling to it
                    // we can then remove the incomplete spelling from the word and continue to the next spelling
                    auto it = inputWordCopy.find(spellings[j], i);
                    if (it != std::string::npos)
                    {
                        if (debug)
                        {
                            std::cout << "Found late spelling: " << spellings[j] << " of pronunciation: " << pronunciation << " in word: " << inputWordCopy;
                            std::cout << " and incomplete spelling is: " << inputWordCopy.substr(0, it);
                        }
                        // we found a spelling in the word, add it to the disassembly
                        spellingDisassembly.back() += inputWordCopy.substr(0, it);
                        spellingDisassembly.push_back(spellings[j]);
                        // remove the spelling from the word
                        inputWordCopy.erase(0, it + spellings[j].size());
                        foundSpelling = true;
                        if (debug)
                        {
                            std::cout << " and new word is " << inputWordCopy << std::endl;
                        }
                    }

                } // end for loop j
            } // end for loop i
        }
        if (!foundSpelling)
        {
            if (debug)
            {
                std::cout << "No spelling found for pronunciation: " << pronunciation << " from word: " << inputWordCopy << " and original word was: " << inputWord;
                std::cout << " and current disassembly is: (";
            }
            for (size_t i = 0; i < spellingDisassembly.size(); i++)
            {
                std::cout << spellingDisassembly[i] << "(" << wordPronunciation[i] << ") ";
            }
            std::cout << ")" << std::endl;
        }

        // if we're on the last pronounciation, we should add the remaining letters to the last spelling
        // ex: "a" and "ay" at the end of a word spell the same for a same sound of EY0 but will get left off by not checking for incomplete spellings
        if (!inputWordCopy.empty() && i == wordPronunciation.size() - 1)
        {
            if (debug)
                std::cout << "Adding remaining letters: " << inputWordCopy << " to last spelling: " << spellingDisassembly.back() << std::endl;
            spellingDisassembly.back() += inputWordCopy;
        }
    }

    // if disassembly isn't the length of the input word pronunciation, we should print a warning and fill the remaining intended length with blank strings
    if (spellingDisassembly.size() != wordPronunciation.size())
    {
        if (debug)
            std::cout << "\t\t\t\t\t\t\t\t\t\tWarning: Disassembly length does not match pronunciation length" << std::endl;
        while (spellingDisassembly.size() < wordPronunciation.size())
        {
            spellingDisassembly.push_back("");
        }
    }

    // because it is very hard to avoid all silent sound spellings, we can find disassemblies that don't match pronunciation lengths and ignore them in the alternative spellings search
    // but because we're filling in the rest with empty strings, all we have to do is check if the last element is empty to know if the disassembly is incomplete so we can ignore and replace it

    return spellingDisassembly;
}

// Function to find alternative spellings
std::vector<std::string> findAlternativeSpelling(std::string &inputWord,
                                                 std::unordered_map<std::string, std::vector<std::string>> &wordDict,
                                                 std::unordered_map<std::string, std::vector<std::string>> &altDict)
{
    std::cout << "Finding alternative spellings for: " << inputWord << std::endl;

    std::vector<std::string> result;
    // find the input word in the dictionary
    auto it = wordDict.find(inputWord);

    // check if input word is in dictionary
    if (it == wordDict.end())
    {
        std::cout << "Word not found in dictionary" << std::endl;
        return result;
    }

    // get the pronunciation of the input word
    // we can also store how many times a sound comes up in the pronunciation for later when we want to find a certain amount of words with the same sound, store this in a map called pronunciationCount
    auto &inputPronunciation = it->second;
    std::unordered_map<std::string, std::vector<std::string>> pronunciationWords;
    std::unordered_map<std::string, int> pronunciationCount;
    for (auto &pronunciation : inputPronunciation)
    {
        pronunciationWords[pronunciation] = std::vector<std::string>();
        pronunciationCount[pronunciation]++;
    }

    std::cout << "Pronunciation and occurrences: ";
    for (auto &pronunciation : inputPronunciation)
    {
        std::cout << pronunciation << "(" << pronunciationCount[pronunciation] << ") ";
    }
    std::cout << std::endl;

    // go through each pronunciation and find alternative spellings first, then we come back and find words that have the same spellings and sounds later
    std::vector<std::vector<std::string>> altSpellings;
    for (auto &entry : inputPronunciation)
    {
        std::vector<std::string> spellings = altDict[entry];
        altSpellings.push_back(spellings);
    }

    // lets disassemble our word into its phonemes and find alternative spellings for each phoneme so we don't use the same spelling for the same phoneme
    // we can do this by iterating through our pronunciation and its spellings, and removing the spelling from the list of alternate spellings and from the word itself to avoid duplicates
    // we can then use the remaining spellings to find words that have the same spellings and sounds

    // disassemble the word into its phonemes
    std::vector<std::string> wordDisassembly = disassembleWordToSpellings(inputWord, wordDict, altDict);

    // now we can remove the proper spellings from the alternative spellings
    for (size_t i = 0; i < wordDisassembly.size(); i++)
    {
        for (size_t j = 0; j < altSpellings.size(); j++)
        {
            for (size_t k = 0; k < altSpellings[j].size(); k++)
            {
                if (wordDisassembly[i] == altSpellings[j][k])
                {
                    altSpellings[j].erase(altSpellings[j].begin() + k);
                    break;
                }
            }
        }
    }

    // now we have our disassembled word and our alternative spellings, we can now find words that have the same spellings and sounds
    for (size_t i = 0; i < altSpellings.size(); i++)
    {
        std::cout << "All alternative spellings for: " << inputPronunciation[i] << ", previously spelled as: " << wordDisassembly[i] << std::endl;
        for (const auto &spelling : altSpellings[i])
        {
            std::cout << spelling << " ";
        }
        std::cout << std::endl;
    }

    // now we need to find words that have different spellings but the same sounds
    // this is made easy by the fact that we have the alternative spellings for each phoneme
    // it is also made easier with the disassembly, we can ignore words that use the same spelling for the same phoneme
    // we can iterate through the alternative spellings and find words that have the same spellings and sounds
    // we can then add these words to the pronunciationWords map
    // we want to limit each pronunciation to the required amount of times it was found earlier (stored in pronunciationCount)
    // we can randomize the words by doing a while loop to fill each pronunciation with the required amount of words rather than iterating through the dictionary to make words more random

    // print input pronunciation
    std::cout << "Input Pronunciation: ";
    for (const auto &pronunciation : inputPronunciation)
    {
        std::cout << pronunciation << " ";
    }
    std::cout << std::endl;

    // print disassembled word
    std::cout << "Disassembled Word: ";
    for (const auto &spelling : wordDisassembly)
    {
        std::cout << spelling << " ";
    }
    std::cout << "\n----------------------------------------------------------------" << std::endl;

    std::vector<std::tuple<std::string, std::string, std::string>> resultWords; // tuple: pronunciation, new spelling, word
    // find words that have different spellings but the same sounds
    // we pick words out of the dictionary at random to avoid always picking the same words for the same pronunciation
    // we fill up each entry of pronunciationWords with the required amount of words from the map pronunciationCount
    for (size_t i = 0; i < inputPronunciation.size(); i++)
    {
        bool quit = false;
        std::cout << "Finding words for pronunciation: " << inputPronunciation[i] << " without spelling: " << wordDisassembly[i] << std::endl;
        while (pronunciationWords[inputPronunciation[i]].size() < static_cast<size_t>(pronunciationCount[inputPronunciation[i]]) && !quit)
        {
            quit = false;
            auto randomWord = randomWordPicker(wordDict);
            std::vector<std::string> randomWordDisassembly = disassembleWordToSpellings(randomWord.first, wordDict, altDict);
            std::vector<std::string> randomWordPronunciation = getPronunciation(randomWord.first, wordDict);
            while (randomWordDisassembly.at(randomWordDisassembly.size() - 1) == "")
            {
                randomWord = randomWordPicker(wordDict);
                randomWordDisassembly = disassembleWordToSpellings(randomWord.first, wordDict, altDict);
                randomWordPronunciation = getPronunciation(randomWord.first, wordDict);
            }
            // now that we have a valid random word, we want to check each pronunciation and its disassembly to see if there is a matching pronunciation with a different disassembly from the input word
            // we can do this by iterating through the pronunciations and disassemblies and checking if the disassembly is different from the input word's disassembly
            // if it is, we can add the word to the pronunciationWords map
            for (size_t j = 0; j < randomWordPronunciation.size() && !quit; j++)
            {
                if (randomWordPronunciation[j] == inputPronunciation[i] && randomWordDisassembly[j] != wordDisassembly[i] && pronunciationWords[inputPronunciation[i]].size() < static_cast<size_t>(pronunciationCount[inputPronunciation[i]]))
                {
                    // we found a matching pronunciation with a different disassembly, add the word to the pronunciationWords map
                    pronunciationWords[inputPronunciation[i]].push_back(randomWord.first);
                    // also add the new spelling to the result words list
                    resultWords.push_back(std::make_tuple(inputPronunciation[i], randomWordDisassembly[j], randomWord.first));
                    std::cout << "\tFound word: " << randomWord.first << " with pronunciation: " << inputPronunciation[i] << " and spelling: " << randomWordDisassembly[j] << std::endl;
                    quit = true;
                    break;
                }
            }
            // std::cout << "EL3";
            if (quit)
            {
                break;
            }
        }
        // std::cout << "EL4\n";
    }

    std::cout << "so why are we breaking?" << std::endl;

    std::vector<std::string> usedWords;
    // now we need to assemble the new spellings into the result vector in order of the input pronunciation, then remove the word from the pronunciationWords map to avoid duplicates
    for (size_t i = 0; i < inputPronunciation.size(); i++)
    {
        for (size_t j = 0; j < resultWords.size(); j++)
        {
            // if the pronunciation matches the input pronunciation and the spelling is not already in the result vector, add it to the result vector
            // also make sure that the word hasn't already been used as a result
            if (inputPronunciation[i] == std::get<0>(resultWords[j]) && std::get<1>(resultWords[j]) != "" && std::find(usedWords.begin(), usedWords.end(), std::get<2>(resultWords[j])) == usedWords.end())
            {
                result.push_back(std::get<1>(resultWords[j]));
                usedWords.push_back(std::get<2>(resultWords[j]));
                std::cout << "Adding to spelling: " << std::get<1>(resultWords[j]) << " with pronunciation: " << inputPronunciation[i] << " and original word: " << std::get<2>(resultWords[j]) << std::endl;
                break;
            }
        }
    }

    // print new spellings
    std::cout << "New Spellings: ";
    for (const auto &spelling : result)
    {
        std::cout << spelling << " ";
    }
    std::cout << std::endl;
    return result;
}

int main()
{
    // map of words to their pronunciations
    std::string filename = "cmudict.txt";
    std::unordered_map<std::string, std::vector<std::string>> wordDict = loadWordDict(filename);

    // map of sounds to their spellings
    std::string altFilename = "altdict.txt";
    std::unordered_map<std::string, std::vector<std::string>> altDict = loadAltDict(altFilename);

    // update the word dictionary to replace similar sounds
    updateWordDict(wordDict, altDict);

    std::string inputWord;
    std::cout << "Enter a word: ";
    std::cin >> inputWord;

    // Convert input word to lowercase to match dictionary format
    std::transform(inputWord.begin(), inputWord.end(), inputWord.begin(), ::tolower);

    std::vector<std::string> alternativeSpellings = findAlternativeSpelling(inputWord, wordDict, altDict);

    // debug = true;
    // std::string word = "autenrieth(2)";
    // std::vector<std::string> pronounced = getPronunciation(word, wordDict);
    // std::vector<std::string> disassembled = disassembleWordToSpellings(word, wordDict, altDict);
    // std::cout << "Pronunciation: ";
    // for (size_t i = 0; i < pronounced.size(); i++)
    // {
    //     std::cout << pronounced[i] << " (" << disassembled[i] << ") ";
    // }
    // std::cout << std::endl
    //           << std::endl;

    // std::string randomWord;
    // for (int i = 0; i < 5; i++)
    // {
    //     randomWord = randomWordPicker(wordDict).first;
    //     std::cout << "Random word: " << randomWord << std::endl;
    //     std::vector<std::string> pronounced = getPronunciation(randomWord, wordDict);
    //     std::vector<std::string> disassembled = disassembleWordToSpellings(randomWord, wordDict, altDict);
    //     std::cout << "Pronunciation: ";
    //     for (int i = 0; i < pronounced.size(); i++)
    //     {
    //         std::cout << pronounced[i] << " (" << disassembled[i] << ") ";
    //     }
    //     std::cout << std::endl
    //               << std::endl;
    // }

    return 0;
}