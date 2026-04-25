// test to try out a fun number experiment
// given input N, take the sum of all of the digits minus the ones place and take the sum and put the ones place as the power
// repeat this process count times, if count is -1 then repeat until the number is a single digit

// unsigned long long int for big numbers
#define ulli unsigned long long int

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

int main() {
    ulli N;
    std::cout<<"Enter a starting number: ";
    std::cin >> N;
    int count;
    //std::cout<<"Enter a count: ";
    //std::cin >> count;
    count = -1; // infinite for now bc lazy
    int sum = 0;
    std::vector<ulli> results;
    results.push_back(N);
    for (int i = 0; i < count - 1 || count == -1; i++) {
        // order: take the ones place, then sum the rest of the digits
        // print out the disassembly as "(Nn + Nn-1 + ... + N1)^(N0) = (Sum)^(N0) = Result"
        int ones = N % 10;
        ulli rest = N / 10;
        sum = 0;
        while (rest > 0) {
            sum += rest % 10;
            rest /= 10;
        }
        ulli result = std::pow(sum, ones);
        std::cout << "(";
        for (ulli j = (ulli)(N/10); j > 0; j /= 10) {
            std::cout << j % 10;
            if (j >= 10) {
                std::cout << " + ";
            }
        }
        std::cout << ")^" << ones << " = " << sum << "^" << ones << " = " << result << "\n";
        N = result;
        if(N < 10) {
            break;
        }
        // if N is in results then break
        if(std::find(results.begin(), results.end(), N) != results.end()) {
            std::cout << "Cycle detected (" << N << "), stopping.\n";
            break;
        }
        results.push_back(N);
    }

}