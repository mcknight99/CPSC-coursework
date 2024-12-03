#include <iostream>
#include <vector>

int main() {
    int x = 4;
    
    int b =  (x<<6)+(x<<3)+(x);
    int y = x*73;
    
    std::cout << "x: " << x << std::endl;
    std::cout << "b: " << b << std::endl;
    std::cout << "y: " << y << std::endl;

    std::vector<int> abc;
    abc.push_back(1);
    abc.push_back(2);
    abc.push_back(3);
    abc.push_back(4);
    abc.push_back(5);

    std::cout<< "abc[0]: " << abc[0] << std::endl;
}