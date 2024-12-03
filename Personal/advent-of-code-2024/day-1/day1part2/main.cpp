#include <iostream>
#include <fstream>
#include <vector>

int main() {
    // we need to find out how often each number in list 1 appears in list 2, then calculate a "similarity score" by adding each number in the left list times the appearance count in the right list

    // read the input
    // input formatted as [num1] [num2] \n
    std::ifstream input("input.txt");
    std::vector<int> list1;
    std::vector<int> list2;
    int num1, num2;
    while (input >> num1 >> num2) {
        list1.push_back(num1);
        list2.push_back(num2);
    }

    // calculate the similarity score
    int score = 0;
    for (int i = 0; i < list1.size(); i++) {
        int count = 0;
        for (int j = 0; j < list2.size(); j++) {
            if (list1[i] == list2[j]) {
                count++;
            }
        }
        score += list1[i] * count;
    }

    std::cout<<score<<std::endl;


}