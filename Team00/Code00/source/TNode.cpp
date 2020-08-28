#include "TNode.h"

TNode::TNode(Op op) : op(op) {}

TNode::TNode(std::string value) : value(value) {}
