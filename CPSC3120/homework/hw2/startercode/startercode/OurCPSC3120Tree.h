#ifndef OURCPSC3120TREE_H
#define OURCPSC3120TREE_H
using namespace std;
#include <iostream>
#include <string>

class OurCPSC3120Tree
{
public:
  OurCPSC3120Tree(void);
  OurCPSC3120Tree(OurCPSC3120Tree &);
  ~OurCPSC3120Tree(void);
  OurCPSC3120Tree *getParent(void);
  OurCPSC3120Tree *getLeft(void);
  OurCPSC3120Tree *getRight(void);
  int getValue(void);
  void setParent(OurCPSC3120Tree &);
  void setLeft(OurCPSC3120Tree &);
  void setRight(OurCPSC3120Tree &);
  void setValue(int);
  void insert(int);
  void printPreorder(void);
  void printInorder(void);
  void printPostorder(void);
  void rotateLeft(void);
  void rotateRight(void);
  void rotateLeftRight(void);
  void rotateRightLeft(void);
  void deleteNode(int);

  void p(string s);
  void printPreorderFile(std::ofstream &file);
  void printInorderFile(std::ofstream &file);
  void printPostorderFile(std::ofstream &file);
  void saveAll(std::ofstream &file);
private:
  int value;
  OurCPSC3120Tree *parent;
  OurCPSC3120Tree *left;
  OurCPSC3120Tree *right;
  int getHeight();
  void balanceTree();
  void printInfo();
};

#endif
