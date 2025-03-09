#include "cpsc3120homework01part02.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <time.h>

using namespace std;

int main( int argc , char* argv[] ){
  string word;
  cin >> word;

  cout << "Before: ";
  cout << word << endl;
  
  try{
    clock_t start = clock();
    removeConsonants( word );
    clock_t end = clock();
    cout << "After : ";
    cout << word << endl;
    fprintf( stderr , "removeConsonants took %.7f seconds.\n" , 1.0 * ( end - start ) / CLOCKS_PER_SEC );
  }
  catch( exception e ){
    cerr << "An error occurred." << endl;
  }
}
