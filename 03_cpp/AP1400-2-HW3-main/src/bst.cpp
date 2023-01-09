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

BST::Node** BST::find_parrent(int value) {
  if (!root) return nullptr;
  Node *tmp = root;
  while (tmp) {
    if (tmp->left && tmp->left->value == value) return new Node*(tmp);
    if (tmp->right && tmp->right->value == value) return new Node*(tmp);
    if (value > tmp->value) {
      tmp = tmp->right;
    } else {
      tmp = tmp->left;
    }
  }
  return nullptr;
}

BST::Node** BST::find_successor(int value) {
  BST::Node** contain = find_node(value);
  if (!contain) {
    return nullptr;
  }
  BST::Node* node = *contain;
  delete contain;

  Node *tmp = nullptr;
  if (node->left) {
    tmp = node->left;
    while (tmp->right) tmp = tmp->right;
  } else {
    tmp = node;
    bool jump;
    while (!(contain = find_parrent(value))) {
      Node *parent = *contain;
      value = parent->value;
      if (parent->right && parent->right->value == tmp->value) {
        jump = true;
      }
      tmp = parent;
      delete contain;
      if (jump) break;
    }
    if (!contain) tmp = nullptr;
  }
  return (tmp == nullptr ? nullptr : new Node*(tmp));
}

bool BST::delete_node(int value) {
  BST::Node** contain = find_node(value);
  if (!contain) {
    return false;
  }

  Node *tmp = *contain;
  delete contain;

  if (!tmp->left && !tmp->right) {
    if (!(contain = find_parrent(tmp->value))) {
      root = nullptr;
      return true;
    } else {
      Node *parrent = *contain;
      delete contain;
      if (parrent->left && parrent->left->value == tmp->value) parrent->left = nullptr;
      if (parrent->right && parrent->right->value == tmp->value) parrent->right = nullptr;
      return true;
    }
  } else if (tmp->left && tmp->right) {
    if (!(contain = find_successor(tmp->value))) {
      return false;
    }
    Node *successor = *contain;
    delete contain;
    delete_node(successor->value);
    tmp->value = successor->value;
    return true;
  } else {
    Node *sub = tmp->left ? tmp->left : tmp->right;
    if (!(contain = find_parrent(tmp->value))) {
      root = sub;
      return true;
    } else {
      Node *parrent = *contain;
      delete contain;
      if (parrent->left && parrent->left->value == tmp->value) parrent->left = sub;
      if (parrent->right && parrent->right->value == tmp->value) parrent->right = sub;
      return true;
    }
  }

  return false;
}

BST::BST() : root(nullptr) {}

BST::BST(const BST &bst) : root(nullptr) {
  std::vector<int> values;
  bst.bfs([&values](BST::Node*& node) {
    values.push_back(node->value);
  });
  for (int v : values) {
    add_node(v);
  }
}

BST::BST(BST &&bst) {
  root = bst.root;
  bst.root = nullptr;
}

BST &BST::operator=(const BST &bst) {
  if (root == bst.root) {
    return *this;
  }
  std::vector<Node*> nodes;
  bfs([&nodes](BST::Node*& node){nodes.push_back(node);});
  for(auto& node: nodes)
    delete node;

  std::vector<int> values;
  bst.bfs([&values](BST::Node*& node) {
    values.push_back(node->value);
  });
  for (int v : values) {
    add_node(v);
  }
  return *this;
}

BST &BST::operator=(BST &&bst) {
  if (root == bst.root) {
    return *this;
  }
  root = bst.root;
  bst.root = nullptr;
  return *this;
}

BST::BST(std::initializer_list<int> values) : root(nullptr) {
  for (int v : values) {
    add_node(v);
  }
}

BST &BST::operator++() {
  bfs([](BST::Node *&node) {
    ++(node->value);
  });
  return *this;
}

BST BST::operator++(int) {
  BST res = *this;
  bfs([](BST::Node *&node) {
    ++(node->value);
  });
  return res;
}

BST::~BST() {
  std::vector<Node*> nodes;
  bfs([&nodes](BST::Node*& node){nodes.push_back(node);});
  for(auto& node: nodes)
    delete node;
}