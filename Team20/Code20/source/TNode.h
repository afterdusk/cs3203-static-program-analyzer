#pragma once

#include <memory>
#include <string>

/** @brief A class that is either the AST representing a SIMPLE expr, or a
 * sub-AST representing a sub-expr that is contained within the expr. */
class TNode {
public:
  /** @brief A class that enumerates all operators that can appear in a SIMPLE
   * expr. */
  enum class Op {
    Unknown, /**< Enum value representing nothing. */
    Plus,    /**< Enum value representing '+'. */
    Minus,   /**< Enum value representing '-'. */
    Times,   /**< Enum value representing '*'. */
    Divide,  /**< Enum value representing '/'. */
    Modulo,  /**< Enum value representing '%'. */
  };

  TNode::Op op = TNode::Op::Unknown; /**< The operator between sub-exprs, not
                          TNode::Op::Unknown if TNode is constructed by Node. */
  std::string value = ""; /**< The value of leaf of a binary tree, nonempty
                               if TNode is constructed by Leaf. */
  std::shared_ptr<TNode> left =
      nullptr; /**< The left child of TNode, initialized by frontend
  if TNode is constructed by Node. */
  std::shared_ptr<TNode> right =
      nullptr; /**< The right child of TNode, initialized by frontend
  if TNode is constructed by Node. */

  TNode();                  /**< An empty TNode. */
  TNode(TNode::Op op);      /**< A node of a binary tree with value op, and two
                        children      nodes of type TNode: left, and right. */
  TNode(std::string value); /**< A leaf of a binary tree with value of type
                                 std::string. */

  /** @brief Tree equality relation.
  @param lhs Tree. Assume lhs has at least one leaf, and all leaves and nodes
  have values.
  @param rhs Tree to compare lhs tree to. Assume rhs has at least one leaf, and
  all leaves and nodes have values.
  @return True if all values in nodes and leaves of lhs are the same as rhs.
  */
  friend bool operator==(const TNode &lhs, const TNode &rhs);

  /** @brief Subtree relation.
  @param lhs Tree. Assume lhs has at least one leaf, and all leaves and nodes
  have values.
  @param rhs Tree to compare lhs tree to. Assume rhs has at least one leaf, and
  all leaves and nodes have values.
  @return True if rhs is a subtree of lhs.
  */
  friend bool operator>=(const TNode &lhs, const TNode &rhs);
};
