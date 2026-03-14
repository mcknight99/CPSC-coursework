// solver based on https://www.youtube.com/watch?v=1aywN5eFOys
// intended to find any matching solutions between using and ignoring PEMDAS

// the in-order problem: A+13*B/C+D+12*E-F-11+G*H/I-10=66
// or with pemdas: A + ((13*B)/C) + D + (12*E) - F - 11 + ((G*H)/I) - 10 = 66
// place each digit 1-9 in place of A,B,C,D,E,F,G,H,I exactly once each

// the goal: find digits 1-9 that satisfy both equations

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

bool check_inorder(const std::vector<int>& digits) {
    float A = digits[0];
    float B = digits[1];
    float C = digits[2];
    float D = digits[3];
    float E = digits[4];
    float F = digits[5];
    float G = digits[6];
    float H = digits[7];
    float I = digits[8];

    float result = ((((((((((((A + 13) * B) / C) + D) + 12) * E) - F) - 11) + G) * H) / I) - 10);
    return result == 66.0f;
}

bool check_with_pemdas(const std::vector<int>& digits) {
    float A = digits[0];
    float B = digits[1];
    float C = digits[2];
    float D = digits[3];
    float E = digits[4];
    float F = digits[5];
    float G = digits[6];
    float H = digits[7];
    float I = digits[8];

    float result = A + ((13 * B) / C) + D + (12 * E) - F - 11 + ((G * H) / I) - 10;
    return result == 66.0f;
}

int main() {
    std::vector<int> digits = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int solutions_found = 0;

    do {
        std::vector<int> current_digits(digits.begin(), digits.begin() + 9);

        if (check_inorder(current_digits) && check_with_pemdas(current_digits)) {
            std::cout << "Found solution: ";
            for (int digit : current_digits) {
                std::cout << digit << " ";
            }
            std::cout << std::endl;
            solutions_found++;

            std::cout<< "In-order calculation: ";
            float inorder_result = ((((((((((((current_digits[0] + 13) * current_digits[1]) / current_digits[2]) + current_digits[3]) + 12) * current_digits[4]) - current_digits[5]) - 11) + current_digits[6]) * current_digits[7]) / current_digits[8]) - 10);
            std::cout << inorder_result << std::endl;
            std::cout<< "With PEMDAS calculation: ";
            float pemdas_result = current_digits[0] + ((13 * current_digits[1]) / current_digits[2]) + current_digits[3] + (12 * current_digits[4]) - current_digits[5] - 11 + ((current_digits[6] * current_digits[7]) / current_digits[8]) - 10;
            std::cout << pemdas_result << std::endl;
        } else {
            // Uncomment the following lines to see all checked combinations
            // std::cout << "Checked combination: ";
            // for (int digit : current_digits) {
            //     std::cout << digit << " ";
            // }
            // std::cout << " - No match" << std::endl;
        }
    } while (std::next_permutation(digits.begin(), digits.end()));
    std::cout << "Total solutions found: " << solutions_found << std::endl;

    return 0;
}