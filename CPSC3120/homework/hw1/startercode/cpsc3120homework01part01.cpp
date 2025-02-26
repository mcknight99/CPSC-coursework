#include "cpsc3120homework01part01.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

int maxSubSlow( const vector<int> &numbers ){
  //textbook instructions for maxsubslow:
  // input: n element array A of numbers, indexed from 1 to n. 
  // output: maximum subarray sum of array A
  // m<-0 the maximum found so far
  // for j = 1 to n do 
  //   for k = j to n do
  //    sum=0 // the next partial sum we are computing
  //    for i = j to k do
  //      sum=sum + A[i]
  //      if sum > m then m = sum
  //  return m
  int m = 0;
  for (int j = 0; j < numbers.size(); ++j){
    for (int k = j; k < numbers.size(); ++k){
      int sum = 0;
      for (int i = j; i <= k; ++i){
        sum += numbers[i];
        if (sum > m){
          m = sum;
        }
      }
    }
  }
  return m;
}

int maxSubFaster( const vector<int> &numbers ){
  // textbook instructions for maxsubfaster:
  // input: n element array A of numbers, indexed from 1 to n.
  // output: maximum subarray sum of array A
  // S_0 = 0 // the initial prefix sum
  // for i = 1 to n do
  //   S_i = S_{i-1} + A[i]
  // m = 0 // the maximum found so far
  // for j = 1 to n do
  //   for k = j to n do
  //     sum = S_k - S_{j-1}
  //     if sum > m then m = sum
  // return m
  int n = numbers.size();
  vector<int> S(n+1, 0); // initialize all with zero, ensures S_0 = 0, replacing S_i as we go
  for (int i = 0; i < n; ++i){
    S[i+1] = S[i] + numbers[i];
  }
  int m = 0;
  for (int j = 0; j < n; ++j){
    for (int k = j; k < n; ++k){
      int sum = S[k+1] - S[j];
      if (sum > m){
        m = sum;
      }
    }
  }
  return m;
}

int maxSubFastest( const vector<int> &numbers ){
  // textbook instructions for maxsubfastest:
  // input: n element array A of numbers, indexed from 1 to n.
  // output: maximum subarray sum of array A
  // M_0 = 0 // the initial prefix maximum
  // for t = 1 to n do
  //   M_t = max(0, M_{t-1} + A[t])
  // m = 0 // the maximum found so far
  // for t = 1 to n do
  // m = max(m, M_t)
  // return m
  int n = numbers.size();
  vector<int> M(n+1, 0); // initialize all with zero, ensures M_0 = 0, replacing M_t as we go
  int m = 0;

  // compute M_t
  // for (int t = 1; t <= n; ++t){
  //   M[t] = max(0, M[t-1] + numbers[t]);
  // }

  // // compute m
  // for (int t = 1; t <= n; ++t){
  //   m = max(m, M[t]);
  // }

  //combine the two steps
  // let's also track j and k, the indices of the max subarray
  // using a tempStart and tempENd variable, we can track the start and end of the current subarray
  // tempStart is the index of the last zero plus one
  // tempEnd is the index of the next known zero OR the end of the array
  // if we find a new max, we update j and k
  int j = 0;
  int k = 0;
  for (int t = 1; t <= n; ++t){
    M[t] = max(0, M[t-1] + numbers[t]);
    m = max(m, M[t]);
  }

  // print M
  for (int i = 0; i < M.size(); ++i){
    cout << M[i] << " ";
  }
  cout << endl;





  return m;
}
