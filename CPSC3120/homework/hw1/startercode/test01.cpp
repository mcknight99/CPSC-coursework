#include "cpsc3120homework01part01.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <time.h>

using namespace std;

int main( int argc , char* argv[] ){
  int num;
  vector<int> numbers;
  while( cin >> num ){
    numbers.push_back( num );
  }

  try{
    clock_t start = clock();
    fprintf( stdout , "maxSubSlow    returned % 10d.\n" , maxSubSlow( numbers ) );
    clock_t slow = clock();
    fprintf( stdout , "maxSubFaster  returned % 10d.\n" , maxSubFaster( numbers ));
    clock_t faster = clock();
    fprintf( stdout , "maxSubFastest returned % 10d.\n" , maxSubFastest( numbers ));
    clock_t fastest = clock();

    fprintf( stderr , "maxSubSlow    took %.7f seconds.\n" , 1.0*( slow - start ) / CLOCKS_PER_SEC );
    fprintf( stderr , "maxSubFaster  took %.7f seconds.\n" , 1.0*( faster - slow ) / CLOCKS_PER_SEC );
    fprintf( stderr , "maxSubFastest took %.7f seconds.\n" , 1.0*( fastest - faster ) / CLOCKS_PER_SEC );
  }
  catch( exception e ){
    cerr << "An error occurred." << endl;
  }
}
