#include <iostream>
#include <vector>
#include <cstdio>
#include "cpsc3120homework03part02.h"
using namespace std;

int main(int argc , char* argv[]){
  int n;
  int i;
  cin >> n;
  vector<double> row(n,-1);
  vector< vector<double> > matrix(n,row);
  cin >> i;
  int r , c , v;
  while( cin >> r >> c >> v ){
    matrix[r-1][c-1] = v;
  }

  for( int i = 0; i < n; i++ ){
    matrix[i][i] = 0;
  }

  int dimensions = n;
  vector< vector<double> > sp = allPairsSP( matrix , i );
  for( int i = 0; i < dimensions; i++ ){
    for( int j = 0; j < dimensions; j++ ){
      fprintf( stdout , "% 5.1f " , sp[i][j] );
    }
    cout << endl;
  }
  return 0;
}
