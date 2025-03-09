#include "OurCPSC3120Tree.h"
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <queue>
#include <string>
using namespace std;

OurCPSC3120Tree::OurCPSC3120Tree()
{
  value = 0;
  parent = NULL;
  left = NULL;
  right = NULL;
}

OurCPSC3120Tree::OurCPSC3120Tree(OurCPSC3120Tree &t)
{
  setParent(*t.getParent());
  setLeft(*t.getLeft());
  setRight(*t.getRight());
  setValue(t.getValue());
}

OurCPSC3120Tree::~OurCPSC3120Tree()
{
}

int OurCPSC3120Tree::getValue()
{
  return value;
}

OurCPSC3120Tree *OurCPSC3120Tree::getLeft()
{
  return left;
}

OurCPSC3120Tree *OurCPSC3120Tree::getRight()
{
  return right;
}

OurCPSC3120Tree *OurCPSC3120Tree::getParent()
{
  return parent;
}

void OurCPSC3120Tree::setValue(int v)
{
  value = v;
  return;
}

void OurCPSC3120Tree::setParent(OurCPSC3120Tree &newParent)
{
  parent = &newParent;
}

void OurCPSC3120Tree::setLeft(OurCPSC3120Tree &newLeft)
{
  left = &newLeft;
  return;
}

void OurCPSC3120Tree::setRight(OurCPSC3120Tree &newRight)
{
  right = &newRight;
  return;
}

void OurCPSC3120Tree::printInfo()
{
  cerr << this << endl;
  cerr << "---------------" << endl;
  cerr << "Value : " << getValue() << endl;
  cerr << "Parent: " << getParent() << endl;
  cerr << "Left  : " << getLeft() << endl;
  cerr << "Right : " << getRight() << endl;
  cerr << "Height: " << getHeight() << endl;
  cerr << "===============" << endl;
  if (getLeft())
  {
    getLeft()->printInfo();
  }
  if (getRight())
  {
    getRight()->printInfo();
  }
}

void OurCPSC3120Tree::printPreorder()
{
  cout << getValue();
  if (getLeft() && getLeft()->getValue())
  {
    cout << ",";
    getLeft()->printPreorder();
  }
  if (getRight() && getRight()->getValue())
  {
    cout << ",";
    getRight()->printPreorder();
  }

  return;
}

void OurCPSC3120Tree::printInorder()
{
  if (getLeft() && getLeft()->getValue())
  {
    getLeft()->printInorder();
    cout << ",";
  }
  cout << getValue();
  if (getRight() && getRight()->getValue())
  {
    cout << ",";
    getRight()->printInorder();
  }

  return;
}

void OurCPSC3120Tree::printPostorder()
{
  if (getLeft() && getLeft()->getValue())
  {
    getLeft()->printPostorder();
    cout << ",";
  }
  if (getRight() && getRight()->getValue())
  {
    getRight()->printPostorder();
    cout << ",";
  }
  cout << getValue();
  return;
}

void OurCPSC3120Tree::saveAll(std::ofstream &file)
{
  printPreorderFile(file);
  file << std::endl;
  printInorderFile(file);
  file << std::endl;
  printPostorderFile(file);
  file << std::endl;
  file.close();
}

void OurCPSC3120Tree::printPreorderFile(std::ofstream &file)
{
  file << getValue();
  if (getLeft() && getLeft()->getValue())
  {
    file << ",";
    getLeft()->printPreorderFile(file);
  }
  if (getRight() && getRight()->getValue())
  {
    file << ",";
    getRight()->printPreorderFile(file);
  }

  return;
}

void OurCPSC3120Tree::printInorderFile(std::ofstream &file)
{
  if (getLeft() && getLeft()->getValue())
  {
    getLeft()->printInorderFile(file);
    file << ",";
  }
  file << getValue();
  if (getRight() && getRight()->getValue())
  {
    file << ",";
    getRight()->printInorderFile(file);
  }

  return;
}

void OurCPSC3120Tree::printPostorderFile(std::ofstream &file)
{
  if (getLeft() && getLeft()->getValue())
  {
    getLeft()->printPostorderFile(file);
    file << ",";
  }
  if (getRight() && getRight()->getValue())
  {
    getRight()->printPostorderFile(file);
    file << ",";
  }
  file << getValue();
  return;
}

void OurCPSC3120Tree::insert(int val)
{
  if (!getValue())
  {
    setValue(val);
  }
  else if (val < getValue() && !getLeft())
  {
    left = new OurCPSC3120Tree();
    getLeft()->setParent(*this);
    getLeft()->insert(val);
  }
  else if (val > getValue() && !getRight())
  {
    right = new OurCPSC3120Tree();
    getRight()->setParent(*this);
    getRight()->insert(val);
  }
  else if (val < getValue())
  {
    getLeft()->insert(val);
  }
  else
  {
    getRight()->insert(val);
  }

  balanceTree();

  return;
}

int OurCPSC3120Tree::getHeight()
{
  if (!getValue())
  {
    return 0;
  }
  if (!getLeft() && !getRight())
  {
    return 1;
  }
  if (getLeft() && !getRight())
  {
    return getLeft()->getHeight() + 1;
  }
  if (!getLeft() && getRight())
  {
    return getRight()->getHeight() + 1;
  }

  return fmax(getLeft()->getHeight(), getRight()->getHeight()) + 1;
}

void OurCPSC3120Tree::balanceTree()
{
  if (!getRight() && getLeft() && !getLeft()->getLeft() && getLeft()->getHeight() >= 2)
  {
    rotateLeftRight();
    return;
  }
  if (!getRight() && getLeft() && getLeft()->getLeft() && getLeft()->getRight() && getLeft()->getRight()->getHeight() > getLeft()->getLeft()->getHeight())
  {
    rotateLeftRight();
    return;
  }
  if (!getLeft() && getRight() && !getRight()->getRight() && getRight()->getHeight() >= 2)
  {
    rotateRightLeft();
    return;
  }
  if (!getLeft() && getRight() && getRight()->getRight() && getRight()->getLeft() && getRight()->getLeft()->getHeight() > getRight()->getRight()->getHeight())
  {
    rotateRightLeft();
    return;
  }
  if (!getLeft() && getRight() && getRight()->getHeight() >= 2)
  {
    rotateLeft();
    return;
  }
  if (getLeft() && getRight() && getRight()->getHeight() >= (getLeft()->getHeight() + 2))
  {
    rotateLeft();
    return;
  }
  if (!getRight() && getLeft() && getLeft()->getHeight() >= 2)
  {
    rotateRight();
    return;
  }
  if (getRight() && getLeft() && getLeft()->getHeight() >= (getRight()->getHeight() + 2))
  {
    rotateRight();
    return;
  }
}

void OurCPSC3120Tree::rotateLeftRight()
{
  getLeft()->rotateLeft();
  rotateRight();
  return;
}

void OurCPSC3120Tree::rotateRightLeft()
{
  getRight()->rotateRight();
  rotateLeft();
  return;
}

void OurCPSC3120Tree::p(string s)
{
  return;
  std::cout << s << std::endl;

  // Open the file in truncate mode
  std::ofstream file;
  file.open("output.txt", std::ios::trunc);
  file << s << std::endl;

  // Find the root of the tree if this node is part of a larger tree
  OurCPSC3120Tree *superParent = this;
  while (superParent->getParent())
  {
    superParent = superParent->getParent();
  }

  // Get the height of the tree
  int height = superParent->getHeight();
  int maxWidth = pow(2, height) - 1; // Maximum nodes at the last level

  // Find the longest node value length for later formatting
  int longestValue = 0;
  std::queue<OurCPSC3120Tree *> tempQueue;
  tempQueue.push(superParent);
  while (!tempQueue.empty())
  {
    OurCPSC3120Tree *node = tempQueue.front();
    tempQueue.pop();
    if (node)
    {
      int nodeValueLength = std::to_string(node->getValue()).length();
      if (nodeValueLength > longestValue)
        longestValue = nodeValueLength;

      tempQueue.push(node->getLeft());
      tempQueue.push(node->getRight());
    }
  }

  // Queue for level order traversal
  std::queue<OurCPSC3120Tree *> q;
  q.push(superParent);

  // Formatting each level
  int level = 0;
  while (!q.empty())
  {
    int levelSize = q.size();
    int spacesBetween = (pow(2, height - level) - 1) * longestValue;
    int leadingSpaces = spacesBetween / 2;

    // Print leading spaces for this level
    std::string line(leadingSpaces, ' ');
    std::string lineFile = line;

    for (int i = 0; i < levelSize; ++i)
    {
      OurCPSC3120Tree *node = q.front();
      q.pop();

      if (node)
      {
        // Print the value, padded to longestValue
        std::string valueStr = std::to_string(node->getValue());
        valueStr = std::string(longestValue - valueStr.length(), '0') + valueStr;

        line += valueStr;
        lineFile += valueStr;

        q.push(node->getLeft());
        q.push(node->getRight());
      }
      else
      {
        // Print placeholder spaces for null nodes
        line += std::string(longestValue, ' ');
        lineFile += std::string(longestValue, ' ');

        q.push(nullptr); // Push null children to maintain structure
        q.push(nullptr);
      }

      if (i < levelSize - 1)
      {
        line += std::string(spacesBetween, ' ');
        lineFile += std::string(spacesBetween, ' ');
      }
    }

    // Trim trailing spaces for better formatting
    line.erase(line.find_last_not_of(' ') + 1);
    lineFile.erase(lineFile.find_last_not_of(' ') + 1);

    std::cout << line << std::endl;
    file << lineFile << std::endl;

    // Stop processing empty levels (avoid infinite loop)
    bool allNull = true;
    std::queue<OurCPSC3120Tree *> tempQueue = q;
    while (!tempQueue.empty())
    {
      if (tempQueue.front())
      {
        allNull = false;
        break;
      }
      tempQueue.pop();
    }
    if (allNull)
      break;

    ++level;
  }

  file.close();
}

void OurCPSC3120Tree::rotateLeft()
{
  p("rotateLeft before, on value: " + std::to_string(value));
  if (!right)
    return; // Can't rotate left if there's no right child

  // new node called rootToLeft
  OurCPSC3120Tree *rootToLeft = new OurCPSC3120Tree();
  rootToLeft->setValue(value);

  // set the value of the current node to the value of the right child
  value = right->value;

  // insert the rootToLeft node between the current node and the left child
  rootToLeft->setParent(*this);
  if (left)
  {
    rootToLeft->setLeft(*left);
    left->setParent(*rootToLeft);
  }
  setLeft(*rootToLeft);

  // set the right child of the rootToLeft node to the left child of the right node
  if (right->left)
  {
    rootToLeft->setRight(*right->left);
    right->left->setParent(*rootToLeft);
    right->left = nullptr;
  }

  // set the right child of the current node to the right child of the right node
  if (right->right)
  {
    setRight(*right->right);
    right->setParent(*this);
  }
  else
  {
    right = nullptr;
  }

  p("rotateLeft after, on value: " + std::to_string(value));
}

void OurCPSC3120Tree::rotateRight()
{
  p("rotateRight before, on value: " + std::to_string(value));
  if (!left)
    return; // Can't rotate right if there's no left child

  // example tree: B (root) -> D(R) A(L); D->C(L) E(R); E->F(R)
  // we don't want a new root, we want to make the current node the root and rotate around that
  // If we're balancing node B, we make a new node with value B,
  // then we set the value of node B to the value of node A

  // new node called rootToRight
  OurCPSC3120Tree *rootToRight = new OurCPSC3120Tree();
  rootToRight->setValue(value);

  // set the value of the current node to the value of the left child
  value = left->value;

  // insert the rootToRight node between the current node and the right child
  rootToRight->setParent(*this);
  if (right)
  {
    rootToRight->setRight(*right);
    right->setParent(*rootToRight);
  }
  setRight(*rootToRight);

  // set the left child of the rootToRight node to the right child of the left node
  if (left->right)
  {
    rootToRight->setLeft(*left->right);
    left->right->setParent(*rootToRight);
    left->right = nullptr;
  }

  // set the left child of the current node to the left child of the left node
  if (left->left)
  {
    setLeft(*left->left);
    left->setParent(*this);
  }
  else
  {
    left = nullptr;
  }

  p("rotateRight after, on value: " + std::to_string(value));
}

void OurCPSC3120Tree::deleteNode(int val)
{
  if (val < value)
  {
    if (left)
      left->deleteNode(val);
    return;
  }
  else if (val > value)
  {
    if (right)
      right->deleteNode(val);
    return;
  }
  else
  {
    p("deleteNode before, on value: " + std::to_string(value) + " deleting: " + std::to_string(val));
  }

  bool twochildren = false;
  if (!parent)
  {
    // if we're deleting the root node
    if (!left && !right) // No children
    {
      value = -1;
      return;
    }
    else if (left && !right) // One left child
    {
      value = left->value;
      left->deleteNode(left->value);
      return;
    }
    else if (!left && right) // One right child
    {
      value = right->value;
      right->deleteNode(right->value);
      return;
    }
    else // Two children
    {
      OurCPSC3120Tree *successor = right;
      while (successor->left)
        successor = successor->left;
      value = successor->value;
      successor->deleteNode(successor->value);
      twochildren = true;
      return;
    }
  }
  else
  {
    // Non root node
    if (!left && !right)
    { // No children
      if (parent->left == this)
        parent->left = nullptr;
      else
        parent->right = nullptr;
    }
    else if (left && !right)
    { // Only left child
      if (parent->left == this)
        parent->left = left;
      else
        parent->right = left;

      left->parent = parent;
    }
    else if (!left && right)
    { // Only right child
      if (parent->left == this)
        parent->left = right;
      else
        parent->right = right;

      right->parent = parent;
    }
    else
    { // Two children
      OurCPSC3120Tree *successor = right;
      while (successor->left)
        successor = successor->left;
      value = successor->value;
      // std::cout << "delete successor for two child node: " + std::to_string(successor->value) << std::endl;
      successor->deleteNode(successor->value);
      twochildren = true;
      p("post successor deletion");
      return; //
    }
    parent->balanceTree();
  }

  if (!twochildren)
    delete this;
}
