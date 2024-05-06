#include <iostream>
#include <fstream>

int main() {
    std::ifstream file("selfread.cpp");
    std::string word;
    while(std::getline(file, word)) {
        std::cout<<word<<std::endl;
    }
}