// Sorry for the long link but this puzzle is beautiful
// https://app.crackingthecryptic.com/fpuzN4IgzglgXgpiBcBOANCALhNAbO8QEEAjMAeywFc0YACAcXwGURUBDSgCxICcEQAxcliwARGAGMA1tRYA7ACbUAshDCQZAc2nU%2BMAO7UAwiy5ywzEF0EwwMNLwZpZc4wrDk5JCeWqWcYaQAOAVgAngB0ADoyUQAq7DBcALYkibYJ%2FhAyYlwwLDbUAGZcKdSEgoTUaCSVEAFh5upcEHIIANqtwAC%2ByF093b0D%2FUN9ALrIHcODfdNTXWMTM5NLi%2FOzy2uj4xvbS6vr%2BytbBzubCyc7e4tXF2Po8UkpaTzwHSBYmdZtrSAASgCMBj%2B5h%2BACZAcCAMzg1A%2FAAsBhBwIArAjgQA2VEwgDsBghwIAHLjgYgiSMRv03h8zC9vv8DLDgWCGTCoczfvCkciDJyYRieb8cWiCQYhWSKe8ZJ8ab8AVjGQY5SyFcD4fiuWreQYNQKDIgQGLyUA%3D%3D%3D
//  "Absolute Gas" by FullDeck and Missing a Few Cards
//  Thermos; brassica
// Takes about 53 seconds on my machine
// 36 seconds with new thermo
// 36 seconds with newer thermo; no differences, but I'm not surprised on this puzzle tbh

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/thermometerModule.h"

#include <chrono>

using namespace std;

int main()
{

    auto start = chrono::high_resolution_clock::now();

    //Gave it a little bit of a head start because of the guaranteed thermometer start
    vector<vector<int>> board = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 0, 0, 0, 0, 0, 0, 0, 0},
        {3, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 4, 0, 0, 0, 0, 0, 0, 0},
        {0, 5, 0, 0, 0, 0, 0, 0, 0},
        {0, 6, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 7, 0, 0, 0, 0, 0, 0},
        {0, 0, 8, 0, 0, 0, 0, 0, 0},
        {0, 0, 9, 0, 0, 0, 0, 0, 0}};

    vector<vector<pair<int, int>>> thermometerData = {
        {{0, 0}, {1, 0}, {2, 0}, {3, 1}, {4, 1}, {5, 1}, {6, 2}, {7, 2}, {8, 2}},
        {{0, 3}, {1, 3}, {2, 3}, {3, 4}, {4, 4}, {5, 4}, {6, 5}, {7, 5}},
        {{0, 6}, {1, 6}, {2, 6}, {3, 7}, {4, 7}, {5, 7}, {6, 8}}};

    // Define constraints
    vector<Constraint> constraints;

    // Add Sudoku basic constraint
    Constraint basicConstraint;
    basicConstraint.check = isSafe;
    basicConstraint.data = nullptr;
    constraints.push_back(basicConstraint);

    // Add thermometer constraint
    Constraint thermometerConstraint;
    thermometerConstraint.check = isThermometersSafe_StrictlyIncreasing;
    thermometerConstraint.data = &thermometerData;
    constraints.push_back(thermometerConstraint);

    // Solve the Sudoku puzzle
    if (solveModularSudoku(board, constraints))
    {
        cout << "Solution:\n";
        printBoard(board);
    }
    else
    {
        cout << "No solution exists!\n";
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "Time taken: " << duration.count() << " seconds" << endl;
}