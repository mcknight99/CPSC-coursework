#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream file("cmudict.txt");
    std::ofstream output("nen.txt");
    if (!file)
    {
        std::cerr << "Failed to open cmudict.txt" << std::endl;
        return 1;
    }

    std::string line;
    std::string word;
    std::string pronunciation;

    while (std::getline(file, line))
    {

        size_t space = line.find(' ');
        word = line.substr(0, space);
        pronunciation = line.substr(space + 1);

        if (word.find("inen") != std::string::npos &&
            pronunciation.find("IH1 N AH0 N") == std::string::npos &&
            
            pronunciation.find("IH0 N AH0 N") == std::string::npos
            
            ) //N IH0 N is the "nen" in linen
        {
            
            std::cout << word << " (" << pronunciation << ")" << std::endl;
            output << word << " (" << pronunciation << ")" << std::endl;
        }
    }

    file.close();
    output.close();

    std::cout<<"Done!"<<std::endl;

    return 0;
}