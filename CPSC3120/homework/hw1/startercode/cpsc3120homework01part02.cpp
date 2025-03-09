#include "cpsc3120homework01part02.h"
#include <string>

using namespace std;

void removeConsonants( string &word ){
  // y is not a vowel for our purposes
  string vowels = "aeiouAEIOU";
  for (int i = 0; i < word.length(); i++){
    if (vowels.find(word[i]) == string::npos){
      word.erase(i, 1);
      i--;
    }
  }
  return;
}
