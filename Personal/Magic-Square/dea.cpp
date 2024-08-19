#include <iostream>
#include <vector>
#include <cmath>
#include <string>

//DEA: doubly even alternator
//doubly even squares require this pattern for determining insertion type. 

using namespace std;
const int SIZE = 12;

const string GREEN = "\033[0;102m";
const string RED = "\033[0;101m";
const string RESET = "\033[0m";

int statement(int i, int j) {
    return ( ( (i+1) / 2) + ( (j+1) / 2) + 1)%2;
}

int main() {
    cout<<"Making vector\n";
    vector<vector<int>> test;
    for(int i = 0; i<SIZE; ++i) {
        vector<int> row(SIZE, 0);
        test.push_back(row);
        for(int j = 0; j<SIZE; ++j) {
            test[i][j] = statement(i,j);
        }
    }
    cout<<"Printing vector\n";
    for(int i = 0; i<SIZE; ++i) {
        for(int j = 0; j<SIZE; ++j) {
            if(test[i][j]==1) {
                cout<<GREEN;
            } else {
                cout<<RED;
            }
            cout<<test[i][j]<<" ";
            cout<<RESET;
        }
        cout<<endl;
    }

}