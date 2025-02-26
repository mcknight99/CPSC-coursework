// using this code to solve the following problem:
// i need to find the exact amount of times i can run an algorithm with a certain runtime
// most algorithms can be solved by hand, but with f(n) = nlogn and f(n) = n!, we need to do trial and error
// so here's how this will work:
// we have four different times we are testing against (1 second, 1 hour, 1 month, 1 century)
// n is the problem size, and we will keep incrementing the problem size until we reach the time limit given by the function
// time limits are in microseconds, so each comparison will be to the scale of 1E6
// here are some expected values, so I know if my code is working correctly
// 1 second and n! should be 9
// 1 hour and n! should be 12
// 1 second and nlogn should be between 32768 and 65536 (62745 found, then 72746 with binary search)

#include <iostream>
#include <cmath>
// lets also use the largest unsigned integer data type which is unsigned long long because the values can get very large
#include <limits>
// also using setw for formatting
#include <iomanip>
using namespace std;
#define ull unsigned long long
#define PRINT_NLOGN 1
#define PRINT_STRESS_TEST_NLOGN 1
#define PRINT_FACTORIAL 0

// helper function to calculate the time for f(n) = nlogn
// speed up using a binary search approach
// test at each 2^n until we exceed the time limit
// then do a binary search between the failed and the last successful n
ull find_n_nlogn(ull time_limit,
                 ull start = 1)
{
    unsigned long long n = start;
    while (true)
    {
        unsigned long long time = n * log2(n);
        if (time > time_limit)
        {
            break;
        }
        n *= 2;
    }

    // do a binary search between the last successful n and the failed n
    unsigned long long left = n / 2;
    unsigned long long right = n;
    while (left < right)
    {
        unsigned long long mid = (left + right) / 2;
        unsigned long long time = mid * log2(mid);
        if (time > time_limit)
        {
            right = mid;
        }
        else
        {
            left = mid + 1;
        }
    }
    return left;
}

// helper function to show that the previous n is too low, the current n is just right, and the next n goes over the time limit
void print_nlogn_surrounding_values(ull n,
                                    ull time_limit)
{
    n--;
    cout << setw(11) << "f(n) = nlogn timelimit: " << right << setw(11) << time_limit << endl;
    ull time = (n - 1) * log2(n - 1);
    cout << setw(11) << "n - 1: " << n - 1 << ", time: " << right << setw(11) << time << endl;
    time = n * log2(n);
    cout << setw(11) << "n: " << n << ", time: " << right << setw(11) << time << endl;
    time = (n + 1) * log2(n + 1);
    cout << setw(11) << "n + 1: " << n + 1 << ", time: " << right << setw(11) << time << endl;
}

// helper function to calculate the time for f(n) = n!
// because of how slow n! already is, trial and error is extremely quick, so we don't need to optimize
ull find_n_factorial(ull time_limit)
{
    unsigned long long n = 1;
    while (true)
    {
        ull time = tgamma(n + 1); // tgamma is the gamma function, which is equivalent to (n-1)!
        if (time > time_limit)
        {
            break;
        }
        n++;
    }
    return n;
}

// helper function to show that the previous n is too low, the current n is just right, and the next n goes over the time limit
void print_factorial_surrounding_values(ull n,
                                        ull time_limit)
{
    n--;
    cout << setw(11) << "f(n) = n! timelimit: " << right << setw(11) << time_limit << endl;
    ull time = tgamma(n);
    cout << setw(11) << "n - 1: " << n - 1 << ", time: " << right << setw(11) << time << endl;
    time = tgamma(n + 1);
    cout << setw(11) << "n: " << n << ", time: " << right << setw(11) << time << endl;
    time = tgamma(n + 2);
    cout << setw(11) << "n + 1: " << n + 1 << ", time: " << right << setw(11) << time << endl;
}

int main()
{
    if (PRINT_NLOGN)
    {
        ull n;
        // 1 second
        cout << "1 second" << endl;
        n = find_n_nlogn(1 * 1E6);
        cout << "f(n) = nlogn: " << n - 1 << endl;
        print_nlogn_surrounding_values(n, 1 * 1E6);

        // 1 hour
        cout << "1 hour" << endl;
        // 3600 seconds in 1 hour, 1 hour = 3600 * 1E6 microseconds
        n = find_n_nlogn(3600 * 1E6);
        cout << "f(n) = nlogn: " << n - 1 << endl;
        print_nlogn_surrounding_values(n, 3600 * 1E6);

        // 1 month
        cout << "1 month" << endl;
        // 2.628e+6 seconds in 1 month, 1 month = 2.628e+6 * 1E6 microseconds
        n = find_n_nlogn((2.628E6) * 1E6);
        cout << "f(n) = nlogn: " << n - 1 << endl;
        print_nlogn_surrounding_values(n, (2628E3) * 1E6);

        // 1 century
        cout << "1 century" << endl;
        // 3.154e+9 seconds in 1 century, 1 century = 3.154e+9 * 1E6 microseconds
        n = find_n_nlogn((3.154E9) * 1E6);
        cout << "f(n) = nlogn: " << n - 1 << endl;
        print_nlogn_surrounding_values(n, (3154E6) * 1E6);
    }

    if (PRINT_STRESS_TEST_NLOGN)
    {
        ull n;
        // this wasn't required but I did it for fun anyways for stress testing out of interest
        // keep increasing stress testing on nlogn, starting at 1 year and multiplying by 10 every i
        int i = 0;
        do
        {
            cout << "1";
            for (int j = 0; j < i; j++)
            {
                cout << "0";
            }
            cout << " years" << endl;
            n = find_n_nlogn(31536000 * pow(10, i) * 1E6);
            cout << "f(n) = nlogn: " << n - 1 << endl;
            print_nlogn_surrounding_values(n, 31536000 * pow(10, i) * 1E6);
            i++;
        } while (n < (n * log2(n))); // once nlogn is less than n, we can stop because nlogn overflowed

        std::cout << "nlogn is greater than the maximum storable value, stopping stress test" << std::endl;
    }

    // f(n) = n!

    if (PRINT_FACTORIAL)
    {
        ull n;
        // 1 second
        cout << "1 second" << endl;
        n = find_n_factorial(1 * 1E6);
        cout << "f(n) = n!: " << n - 1 << endl;
        print_factorial_surrounding_values(n, 1 * 1E6);

        // 1 hour
        cout << "1 hour" << endl;
        n = find_n_factorial(3600 * 1E6);
        cout << "f(n) = n!: " << n - 1 << endl;
        print_factorial_surrounding_values(n, 3600 * 1E6);

        // 1 month
        cout << "1 month" << endl;
        n = find_n_factorial(2.628E6 * 1E6);
        cout << "f(n) = n!: " << n - 1 << endl;
        print_factorial_surrounding_values(n, 2628E3 * 1E6);

        // 1 century
        cout << "1 century" << endl;
        n = find_n_factorial(3.154E9 * 1E6);
        cout << "f(n) = n!: " << n - 1 << endl;
        print_factorial_surrounding_values(n, 3154E6 * 1E6);
        return 0;
    }
}
