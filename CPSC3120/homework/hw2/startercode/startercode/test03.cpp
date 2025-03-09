#include "OurCPSC3120Tree.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  OurCPSC3120Tree tree;

  try
  {
    int n;
    bool del = false;
    while (cin >> n)
    {
      if (n < 0)
      {
        del = !del;
        continue;
      }

      if (!del)
      {
        tree.insert(n);
      }
      else
      {
        tree.deleteNode(n);
      }
    }
    tree.printInorder();
    cout << endl;
  }
  catch (exception e)
  {
    cerr << "An error occurred in printing" << endl;
  }

  return 0;
}
