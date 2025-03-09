#include "OurCPSC3120Tree.h"
#include <iostream>
#include <fstream>
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
        //std::cout<<"inserting: "<<n<<std::endl;
        tree.insert(n);
      }
      else
      {
        //std::cout<<"deleting: "<<n<<std::endl;
        tree.deleteNode(n);
      }
    }
    tree.printPreorder();
    cout << endl;
    tree.printInorder();
    cout << endl;
    tree.printPostorder();
    cout << endl;
    tree.p("manual test print");
    // open manualtest.output and saveAll
    std::ofstream file;
    file.open("tests/manualtest.output", std::ios::trunc);
    tree.saveAll(file);
    file.close();

  }
  catch (exception e)
  {
    cerr << "An error occurred in printing" << endl;
  }

  return 0;
}
