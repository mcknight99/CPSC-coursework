#include "OurCPSC3120Tree.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  OurCPSC3120Tree tree;

  try
  {
    int n;
    while (cin >> n)
    {
      tree.insert(n);
    }
    std::cout << "Test results: \n";
    tree.printPreorder();
    cout << endl;
    tree.printInorder();
    cout << endl;
    tree.printPostorder();
    cout << endl;
  }
  catch (exception e)
  {
    cerr << "An error occurred in printing" << endl;
  }

  return 0;
}
