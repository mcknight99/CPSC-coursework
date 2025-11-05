#include "cpsc3120homework04part02.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>

using namespace std;

int main( int argc , char* argv[] ){

  vector<unsigned long long int> votes;
  vector<int> electoralCollegeVotes;
  vector<string> abbr;
  unsigned long long int v;
  int ev;
  string ab;
  while( cin >> ab >> v >> ev ){
    votes.push_back( v );
    electoralCollegeVotes.push_back( ev );
    abbr.push_back( ab );
  }
  
  try{
    fprintf( stdout , "Minimum required votes: %llu\n" , minimumVotes( votes , electoralCollegeVotes , abbr ) );
  }
  catch( exception e ){
    cerr << "An error occurred." << endl;
  }
}
