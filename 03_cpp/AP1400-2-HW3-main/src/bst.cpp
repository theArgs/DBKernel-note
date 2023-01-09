#include "bst.h"

#include <queue>
#include <iomanip>

std::ostream &operator<<(std::ostream &os, const BST::Node &node) {
  os << std::left << std::setw(16) << &node << "=> value:" << std::setw(10) << node.value
     << "left:" << std::setw(20) << node.left << "right:" << std::setw(16) << node.right;
  return os;
}

std::ostream &operator<<(std::ostream &os, const BST &bst) {
  os << std::string(80, '*') << std::endl;
  bst.bfs([&os](BST::Node *&node) {
    os << *node << std::endl;
  });
  os << std::string(80, '*') << std::endl;
  return os;
}

BST::Node::Node() : value(0), left(nullptr), right(nullptr) {}
BST::Node::Node(int value, Node *left, Node *right) :
    value(value), left(left), right(right) {}
BST::Node::Node(const Node &node) :
    value(node.value), left(node.left), right(node.right) {}

BST::Node::operator int() const {
  return value;
}

BST::Node*& BST::get_root() {
  return root;
}

void BST::bfs(std::function<void(Node *&node)> func) const {
  std::queue<Node *> que;
  if (root) que.push(root);
  while (!que.empty()) {
    Node *tmp = que.front();
    que.pop();
    func(tmp);
    if (tmp->left) que.push(tmp->left);
    if (tmp->right) que.push(tmp->right);
  }
}

size_t BST::length() {
  size_t length = 0;
  bfs( [&length](BST::Node*& node) {
    ++length;
  });
  return length;
}

bool BST::add_node(int value) {
  BST::Node** t = find_node(value);
  if (t && *t) {
    delete t;
    return false;
  }
  if (!root) {
    root = new Node(value);
  } else {
    Node *tmp = root;
    while (tmp) {
      if (value > tmp->value && tmp->right == nullptr) {
        tmp->right = new Node(value);
        break;
      } else if (value > tmp->value) {
        tmp = tmp->right;
      } else if (value < tmp->value && tmp->left == nullptr) {
        tmp->left = new Node(value);
        break;
      } else {
        tmp = tmp->left;
      }
    } 
  }
  return true;
}

BST::Node** BST::find_node(int value) {
  if (!root) return nullptr;
  Node *tmp = root;
  while (tmp) {
    if (value > tmp->value) {
      tmp = tmp->right;
    } else if (value < tmp->value) {
      tmp = tmp->left;
    } else {
      return new Node*(tmp);
    }
  }
  return nullptr;
}