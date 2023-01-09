#ifndef BST_H
#define BST_H

#include <functional>
#include <initializer_list>
#include <iostream>

class BST {
 public:
  class Node {
   public:
    friend std::ostream &operator<<(std::ostream &os, const BST::Node &node);
    Node();
    Node(int value, Node *left = nullptr, Node *right = nullptr);
    Node(const Node &node);

    operator int() const;
    int value;
    Node *left;
    Node *right;
  };

  friend std::ostream &operator<<(std::ostream &os, const BST &bst);

  Node*& get_root();
  void bfs(std::function<void(Node*& node)> func) const;
  size_t length();
  bool add_node(int value);
  Node **find_node(int value);
  Node **find_parrent(int value);
  Node **find_successor(int value);
  bool delete_node(int value);

  BST();
  BST(const BST &bst);
  BST(BST &&bst);
  BST(std::initializer_list<int> values);
  ~BST();

  BST &operator=(const BST &bst);
  BST &operator=(BST &&bst);

  BST &operator++();
  BST operator++(int);

 private:
  Node *root;
};

#endif //BST_H