// doing this to calculate the runtime of a few lines in seconds

#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>

int main() {
    std::unordered_map<std::string, bool> dictTest;
    int m, n = 0;
    dictTest["hello"] = true;
    dictTest["world"] = true;
    std::string check = "hello";
    auto start = std::chrono::high_resolution_clock::now();
    // code to time
    check[0] = 'w';
    check[1] = 'o';
    check[2] = 'r';
    check[3] = 'l';
    check[4] = 'd';
    if(dictTest[check]) {
        n++;
    }
    m++;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    printf("Time taken: %f\n", duration.count());
    return 0;
}