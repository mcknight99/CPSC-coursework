// https://sudokupad.app/8brbv8xf8p
// "Thermo Dat" by FullDeck and Missing a Few Cards
// Disjoint Groups // Thermometers
// This took 36 mins 40 sec to solve on my computer with a blank board, so be patient...
// I estimate if the top left was a 9, it could take about an hour and a half to solve in worst case.
// Woah only a bit more than 24 minutes with new thermo
// Same results actually with newer thermo, just over 24 minutes

#include <iostream>
#include "../solvemodularsudoku.h"
#include "../modules/thermometerModule.h"
#include "../modules/disjointModule.h"

#include <chrono>

using namespace std;

int main() {

    auto start = chrono::high_resolution_clock::now();

    // This was my first time testing multi ended thermometers as well as the new disjoint module, so I added a few known values to speed up the test to make sure both worked
    // Thankfully it both worked first try, taking about 20 seconds
    /*
    vector<vector<int>> board = {
        {4, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 2, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 4, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 0, 0, 0, 0, 0, 0, 3, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};
        */
    //But the original puzzle is all zeroes
    vector<vector<int>> board = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}};

    vector<vector<pair<int, int>>> thermometerData = { 
        //This puzzle uses multi-ended thermometers, but this works fairly well in code as you can just branch off of another thermometer
        {{1,1},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6}},
        {{0,5},{1,5},{1,4}},
        {{1,7},{1,8},{2,8},{3,8},{4,8},{5,8},{6,8}},
        {{5,8},{5,7}},
        {{4,1},{4,0},{3,0},{3,1}},
        {{7,0},{7,1}},
        {{7,7},{8,7},{8,6},{8,5},{8,4},{8,3},{8,2}},
        {{8,3},{7,3}}    
    };

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

    // Add disjoint groups constraint
    Constraint disjointConstraint;
    disjointConstraint.check = isDisjointSafe;
    disjointConstraint.data = nullptr;
    constraints.push_back(disjointConstraint);

    // Solve the Sudoku puzzle
    if (solveModularSudoku(board, constraints))
    {
        cout << "Solution:\n";
        printBoard(board);
    }
    else
    {
        cout << "No solution exists\n";
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "Time taken: " << duration.count() << " seconds" << endl;
    
}