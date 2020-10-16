#pragma once

#include <memory>
#include <unordered_set>

/** @brief A node in the control flow graph.  */
class CfgNode {
public:
  std::size_t entryPoint; /**< The entry point of this node. */
  std::size_t exitPoint;  /**< The exit point of this node. */
  std::unordered_set<std::shared_ptr<CfgNode>>
      children; /**< Children of this node. */
  std::unordered_set<std::shared_ptr<CfgNode>>
      parents; /**< Parents of this node. */

  /** @brief Constructs CfgNode.
  @param entryPoint The entry point of this node.
  @param exitPoint The exit point of this node.
  */
  CfgNode(std::size_t entryPoint, std::size_t exitPoint);

  /** @brief Adds child as a child of parent, adds parent as a parent of child.
  @param parent The parent of child.
  @param child The child of parent.
  */
  static void addEdge(std::shared_ptr<CfgNode> parent,
                      std::shared_ptr<CfgNode> child) {
    parent->children.insert(child);
    child->parents.insert(parent);
  }
};
