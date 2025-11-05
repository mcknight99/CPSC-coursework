#include <iostream>
#include <vector>
#include <cstdio>
#include "cpsc3120homework03part01.h"
using namespace std;

int main(int argc , char* argv[]){
  int n;
  cin >> n;
  vector<double> row(n,0);
  vector< vector<double> > matrix(n,row);
  int r , c , v;
  while( cin >> r >> c >> v ){
    int R = (int)(r);
    int C = (int)(c);
    matrix[R-1][C-1] = v * 1.0;
    matrix[C-1][R-1] = v * 1.0;
  }

  fprintf( stdout , "The value of the maximum spanning tree is % 4.1f\n" , maximumST( matrix ) );
  return 0;
}
