// find largest power of 2 that can be made from first N digits on the keyboard (N=1 =1, N=2 = 1&2, N=3 = 1&2&3, N=4 = ... N=10 = 1&2&3&4&5&6&7&8&9&0)
// largest power of 2 that can be made from first N digits: // more can probably be made but these are within the limit of ull
// N=1: 1
// N=2: 2
// N=3: 32
// N=4: 32
// N=5: 33554432
// N=6: 33554432
// N=7: 33554432
// N=8: 2147483648
// N=9: 2251799813685248
// N=10: anything 2^N can be made with all 10 digits

// a cool medium could be a mix: find the largest power of 2 that is strictly made of the first N digits. I wonder if this would bring the numbers down or cause them to have No Solutions to some

// could be cool to find the largest power of 2 that can be made with the least N of digits
// ^ that has now been made into the below 

// new approach: largest power of 2 that can be made from any N unique digits:
// N=1: 8
// N=2: 64
// N=3: 65536
// N=4: 33554432
// N=5: 4294967296
// N=6: 2199023255552
// N=7: 144115188075855872
// N=8: 576460752303423488
// N=9: 9223372036854775808
// N=10: nothing (within ULL) can be made of strictly 10 digits (which is very counterintuitive, 
                                                            //   but makes sense since it *has* to have all 10 digits to be valid and you'd need a massive number to do that)

#include <iostream>
#include <cstdint>
typedef unsigned long long ull;
typedef uint16_t u16;

// only print the first 10 bits
void printBinaryFirst10(u16 num)
{
    for (int i = 9; i >= 0; i--)
    {
        // print 1s in green
        if ((num >> i) & 1)
        {
            std::cout << "\033[42m1\033[0m"; // Green color for 1
        }
        else
        {
            std::cout << "0"; // Default color for 0
        }
    }
}

// bool consistsOfDigits(ull num, int n)
// {
//     for (ull temp = num; temp > 0; temp /= 10)
//     {
//         std::cout << "Checking digit: " << temp % 10 << " for number: " << num << std::endl;
//         int digit = temp % 10;
//         if (digit > n || (digit == 0 && n < 10)) // If digit is greater than n or if digit is 0 and n is less than 10, it's invalid // include 0 at 10
//         {
//             std::cout << "\tDigit " << digit << " is greater than " << n << ". Skipping number: " << num << std::endl;
//             return false; // If any digit is greater than n, return false
//         }
//     }
//     std::cout << "\tAll digits of " << num << " are valid (<= " << n << ")." << std::endl;
//     return true; // All digits are valid
// }

// return true if num consists of digits strictly in the array n. for example, if num = 2199023255552, nums = {0, 1, 2, 3, 5, 9}, and n = 6 then return true, but if nums = {0, 1, 2, 3, 5} then return false since there is a 9 in num that is not in the array. also, if the array was {1, 6, 7} but num had all but one (i.e. 16777216, there is a 2 in the number that is not in the array)
//This function will be used to check if a power of 2 consists of the valid digits in the current combination
bool consistsOfDigitArray(ull num, int nums[], int n)
{
    // map for found nums in num
    bool found[10] = {false}; // index = digit, value = if it has been found. only indexes that are in nums need to be checked and true
    // bool for if any digits are in num that arent in nums
    bool hasInvalidDigit = false;
    // printf("Checking if number %llu consists of digits in the array: ", num);
    // for (int i = 0; i < n; i++)
    // {
    //     printf("%d ", nums[i]);
    // }
    // std::cout << std::endl;
 
    for (ull temp = num; temp > 0; temp /= 10)
    {
        int digit = temp % 10;
        bool isValidDigit = false;

        // printf("Checking digit: %d for number: %llu\t\n", digit, num);
        for (int i = 0; i < n; i++)
        {
            if (digit == nums[i])
            {
                found[nums[i]] = true; // Mark the digit as found
                isValidDigit = true;
                break;
            }
        }
        if (!isValidDigit)
        {
            hasInvalidDigit = true; // If any digit is not in the array, mark it as invalid
            // printf("\tDigit %d is not in the array of valid digits.\n", digit);
            // std::cout << "\tDigit " << digit << " is not in the array of valid digits." << std::endl;
            return false;
        }
    }

    for(int i = 0; i < n; i++)
    {
        if (!found[nums[i]])
        {
            // std::cout << "\tNumber " << num << " is missing a valid digit " << nums[i] << " from the array." << std::endl;
            return false; // If any valid digit from the array is not found in the number, return false
        }
    }
    return true;

}

bool testConsistsOfDigitArray(ull num, int nums[], int n)
{
    std::cout << "Testing if " << num << " consists of digits in the array: ";
    for (int i = 0; i < n; i++)
    {
        std::cout << nums[i] << " ";
    }
    std::cout << std::endl;

    bool result = consistsOfDigitArray(num, nums, n);
    if (result)
    {
        std::cout << num << " consists of digits in the array." << std::endl;
    }
    else
    {
        std::cout << num << " does NOT consist of digits in the array." << std::endl;
    }
    return result;
}

// // function takes in start value (i.e. 0b0000000011 for N=2) and returns a number that marches upwards for N=2 valid bits (next would be 0b0000000101, 0b0000001001, ... 0b1000000001, then 0b0000000110... )
// // this is necessary because counting up from 0d1 and checking if the number consists of valid digits is very inefficient, especially as N increases, since the number of valid combinations grows exponentially with N. For example, with N=4, there are 4! = 24 valid combinations of digits, but if we were to count up from 1 and check each number, we would have to check a much larger range of numbers (up to 4321) to find all valid combinations. By directly generating the next valid combination of bits, we can significantly reduce the number of checks needed and improve efficiency.
// int countUpBits(int n, u16 start)
// {
//     // if highest n bits are all 1s, return all zeroes
//     bool allOnes = true;
//     for (int i = 0; i < n; i++)
//     {
//         if (((start >> (9 - i)) & 1) == 0)
//         {                    // Check if the (9 - i)-th bit is not set (0)
//             allOnes = false; // If any of the highest n bits is not 1, set allOnes to false
//             break;
//         }
//     }
//     if (allOnes)
//     {
//         return 0; // If all first n bits are 1s, return 0 to flag the count is done
//     }

//     int clz = __builtin_clz(start) - 16; // Count leading zeros to find the position of the highest set bit
//     // printf("Leading zeros in %d: %d\t", start, clz);
//     int highestBit = 16 - clz - 1; // Calculate the position of the highest set bit

//     // if highestbit = 10, then we want to forget about the highest bit and march the next bit foward
//     // do this by recursing with a subtracted value of the highest bit
//     // but we dont want the recursion to march into the current bit, so we check if the n matches the amount of bits
//     // if there is 1 bit missing, then the highest bit should stop 1 short of the 10th bit (9 from 0)
//     // if there are 2 bits missing, then the highest bit should stop 2 short of the 10th bit (8 from 0) etc
//     if (highestBit + n >= 10 + 1)
//     {
//         // printf("Highest bit %d is at the limit for n=%d, recursing with start=%d\t", highestBit, n, start);
//         int part = countUpBits(n - 1, start - (1 << highestBit)); // Recurse with the highest bit removed
//         int newclz = __builtin_clz(part) - 16;                    // Count leading zeros of the result from recursion to find the new highest set bit
//         int newHighestBit = 16 - newclz - 1;                      // Calculate the position of the new highest set bit after recursion
//         int ret = part + (1 << (newHighestBit + 1));              // Add the highest bit back to the result of the recursion
//         // printf("\tPost recursing, returning: \t");
//         // printBinaryFirst10(ret);
//         // std::cout << "\n";
//         return ret; // Add the highest bit back to the result of the recursion
//     }

//     // printf("Highest set bit in %d is at position: %d\t", start, highestBit);
//     int ret = start - (1 << highestBit) + (1 << (highestBit + 1)); // Remove the highest set bit and add a new bit

//     // printf("Current: %d (", start);
//     // printBinaryFirst10(start);
//     // std::cout << ") -> Next: " << ret << " (";
//     // printBinaryFirst10(ret);
//     // std::cout << ")";
//     // std::cout << "\n";
//     return ret; // Return the updated combination
// }

// effectively the same as count up bits but in the opposite direction to more easily limit the marching bit. I found marching left made it difficult to send in just bits and get out the right number
// so this new one will be provided with a start of the highest combination (i.e. n=4: start=1111000000, next count is 1110100000 -> 1110010000... until 1110000001, then 1101100000 etc)
// the same recursive approach will be used where if there is a 1 in the least significant position, the entire number gets right shifted and recursed with n-1 until all n bits are on the right
// once all n bits are on the right, return 0 to flag the count is done
int countDownBits(int n, u16 start)
{
    if (start == (1 << n) - 1)
    {
        return 0; // If we have exhausted all combinations, return 0 to flag the count is done
    }

    int lsb = start & -start; // Get the least significant bit that is set to 1
    if (lsb == 1)
    { // If the least significant bit is in the rightmost position, we need to recurse with n-1 and shift right
        int next = start >> 1;
        int part = countDownBits(n - 1, next); // Recurse with the least significant bit removed and n-1")
        int nextlsb = part & -part;            // Get the least significant bit of the result from recursion
        // printf("\trecursive part: %d\t", part); printBinaryFirst10(part); printf("\tnextlsb: %d\t", nextlsb); printBinaryFirst10(nextlsb); std::cout << "\n";
        return (part << 1) + nextlsb; // Shift the result of the recursion
    }

    return start - lsb + (lsb >> 1); // Remove the least significant bit and add a new bit one position to the right
}

void testCountDownBits(int n)
{
    // int n = 10;                                      // base case
    u16 start = (0xFFFF << (10 - n)) & 0b1111111111; // Start with the largest combination of n bits (e.g., for n=3, start=0b0000000111)
    printBinaryFirst10(start);
    printf("\t%d", start);
    std::cout << "\n";

    int end = (1 << n) - 1;
    printBinaryFirst10(end);
    printf("\t%d", end);
    std::cout << "\n";

    int next = countDownBits(n, start);
    printf("Next combination: %d\t", next);
    printBinaryFirst10(next);
    std::cout << "\n";

    int count = 1;
    for (int i = 0; next != 0; i++)
    {
        printf("Current combination: %d\t", next);
        printBinaryFirst10(next);
        next = countDownBits(n, next);
        count++;
        if (next == 0)
        {
            std::cout << "All combinations exhausted.\n";
            break;
        }
        printf("\tNext combination: %d\t", next);
        printBinaryFirst10(next);
        std::cout << "\n";
    }
    printf("Total combinations for n=%d: %d\n", n, count);
}

ull highestPowerOf2ForNDigits(int n)
{
    // using the countdownbits and consistsOfDigitArray functions, we can find the highest power of 2 that can be made with the first n digits by starting with the largest power of 2 and counting down until we find a power of 2 that consists of the valid digits
    ull highestpowerof2 = 0;

    int start = (0xFFFF << (10 - n)) & 0b1111111111;

    while (start != 0)
    {
        ull powerof2 = 1ULL << 63; // Start with the largest power of 2 that fits in an unsigned long long
        int index = 0;
        int validDigitsArray[10]; // Array to store the valid digits
        for (int i = 9; i >= 0; i--)
        {
            if ((start >> i) & 1)
            {
                validDigitsArray[index++] = 9 - i; // Store the valid digits in the array
            } 
        }
        // printf("Valid digits permutation for n=%d: ", n);
        // for (int i = 0; i < index; i++)
        // {
        //     printf("%d ", validDigitsArray[i]);
        // }
        // std::cout << "\n";

        while (powerof2 > 0)
        {
            // printf("Checking power of 2: %llu against array: ", powerof2);
            // for (int i = 0; i < index; i++)
            // {
            //     printf("%d ", validDigitsArray[i]);
            // }
            // std::cout << "\n";
            if (consistsOfDigitArray(powerof2, validDigitsArray, index) && powerof2 > highestpowerof2)
            {
                printf("\t\t\t\t\tPower of 2 %llu consists of valid digits and is greater than current highest %llu\n", powerof2, highestpowerof2);
                highestpowerof2 = powerof2; // Update the highest power of 2 that consists of the valid digits
            }
            powerof2 >>= 1; // Move to the next lower power of 2
        }
        start = countDownBits(n, start); // Get the next combination of valid digits
    }

    return highestpowerof2; // Return the highest power of 2 that consists of the valid digits
}

int main()
{
    for (int n = 1; n <= 10; n++) 
    {
        ull result = highestPowerOf2ForNDigits(n);
        printf("Highest power of 2 that can be made with any %d digits: %llu\n", n, result);
    }

    // int a[] = {0, 1, 2, 3, 5, 9};
    // testConsistsOfDigitArray(2199023255552, a, 6); // Should return true
    // testConsistsOfDigitArray(21990225552, a, 6); // Should return false (missing 3)
    // int b[] = {0, 1, 2, 3, 5};
    // testConsistsOfDigitArray(2199023255552, b, 5); // Should return false (9 is not in the array)

    return 0;
}