#include "cpsc3120homework04part01.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace std;

int main( int argc , char* argv[] ){
  vector<int> values;
  int val;
  while( cin >> val ){
    values.push_back( val );
  }

  try{
    fprintf( stdout , "The longest increasing subsequence has a length of %d.\n" , longestIncreasingSubsequence( values ) );
  }
  catch( exception e ){
    cerr << "An error occurred." << endl;
  }
}
