#include <string>

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

/** @brief A class that is either the AST representing a SIMPLE expr, or a
 * sub-AST representing a sub-expr that is contained within the expr. */
class TNode {
public:
  Op op = Op::Unknown; /**< The operator between sub-exprs, initialized if TNode
                          is constructed by Node. */
  std::string value = ""; /**< The value of leaf of a binary tree, nonempty
                               if TNode is constructed by Leaf. */
  TNode *left = nullptr;  /**< The left child of TNode, initialized by frontend
                             if TNode is constructed by Node. */
  TNode *right = nullptr; /**< The right child of TNode, initialized by frontend
                             if TNode is constructed by Node. */

  TNode(Op op); /**< A node of a binary tree with value op, and two children
                   nodes of type TNode: left, and right. */
  TNode(std::string value); /**< A leaf of a binary tree with value of type
                                 std::string. */
};
