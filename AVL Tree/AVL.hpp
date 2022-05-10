#ifndef __AVL_HPP_
#define __AVL_HPP_
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;

class AVL {
private:
  class Node {
    Node *parent, *left, *right;
    int height;
    string element;

  public:
    Node(const string& e, Node *parent, Node *left, Node *right);
    
    Node*  getParent() const;
    Node*  getLeft() const;
    Node*  getRight() const;
    string getElement() const;
    int    getHeight() const;
  
    void setLeft(Node *);
    void setRight(Node *);
    void setParent(Node *);
    void setElement(string e);
     
    bool isLeft() const;
    bool isRight() const;
    int  rightChildHeight() const;
    int  leftChildHeight() const;
    int  updateHeight();
    bool isBalanced();
  };
private:
  int   size;
  Node* root;
  
public:
    
    class Iterator {
    public:
      stack<Node* > nodeStack;//
      Iterator(Node* );//
      Iterator& operator++();
      Iterator operator++(int a);
      string operator*(); 
      bool operator!=(Iterator it);
      bool operator==(Iterator it);
    };
    
  Iterator begin() const;  
  Iterator end() const;
  
  static const int MAX_HEIGHT_DIFF = 1;
  int count;//
  AVL();
  AVL(const AVL& );
  ~AVL();//
  bool contains(string e);
  bool add(string e);
  Node* rightRotate(Node* );
  Node* leftRotate(Node* );
  Node* insert(Node* ,string& e);
  
  bool rmv(string e);
  void print2DotFile(char *filename)const;
  void pre_order(std::ostream& out)const;
  
  void getPreorder(Node* ,string& e)const;//
  void getDotFile(Node* ,string& e)const;//
  
  friend std::ostream& operator<<(std::ostream& out, const AVL& tree);  
  AVL& operator =(const AVL& avl);
  AVL  operator +(const AVL& avl);
  AVL& operator +=(const AVL& avl);
  AVL& operator +=(const string& e);
  AVL& operator -=(const string& e);
  AVL  operator +(const string& e);
  AVL  operator -(const string& e);
};

#endif
