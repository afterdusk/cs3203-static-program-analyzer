#include "TNode.h"

TNode::TNode() {}

TNode::TNode(Op op) : op(op) {}

TNode::TNode(std::string value) : value(value) {}

bool operator==(const TNode &lhs, const TNode &rhs) {
  if (lhs.op == Op::Unknown) { // lhs is a leaf.
    return lhs.value == rhs.value;
  } else { // lhs is a node.
    return lhs.op == rhs.op && lhs.left == rhs.left && lhs.right == rhs.right;
  }
}

bool operator>=(const TNode &lhs, const TNode &rhs) {
  if (lhs == rhs) { // the trees are equal.
    return true;
  } else if (lhs.op ==
             Op::Unknown) { // the trees are not equal, and lhs is a leaf.
    return false;
  } else { // the trees are not equal, and lhs is a node.
    return (*lhs.left) >= rhs ||
           (*lhs.right) >= rhs; // therefore it's safe to dereference lhs.left
                                // and lhs.right.
  }
}
