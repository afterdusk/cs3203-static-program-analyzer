#include "CfgNode.h"

CfgNode::CfgNode(std::size_t entryPoint, std::size_t exitPoint) {
  this->entryPoint = entryPoint;
  this->exitPoint = exitPoint;
}
