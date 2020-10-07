#pragma once

#include "PkbTables.h"
#include <algorithm>
#include <iterator>
#include <memory>
#include <set>
#include <stack>

/** @brief A node in the control flow graph.  */
class CfgNode {
public:
  /** @brief A class that enumerates all kinds of node types. */
  enum class Type {
    BasicBlock,     /**< Represents the StatementList of a basic block. */
    ThenBlock,      /**< Represents the StatementList of a then-block. */
    ElseBlock,      /**< Represents the StatementList of a else-block. */
    WhileBlock,     /**< Represents the StatementList of a while-block. */
    IfStatement,    /**< Represents a if-statement. */
    WhileStatement, /**< Represents a while-statement. */
  };
  /** @brief A tuple of tables. Makes passing tables between methods easier.
  This tuple consists of:
  - followTableClosed A table that maps a Statement to all Statements that
  appear after it in the program text if they are at the same nesting level, in
  the same StatementList.
  - parentTablePseudoinverted A table that maps a Statement to its
  Children.
  - statementTypeTable A table that maps a Statement to its StatementType.
  This mapping is a total function.
  */
  typedef std::tuple<KeysTable<PkbTables::LINE_NO, PkbTables::FOLLOWS>,
                     KeysTable<PkbTables::PARENT, PkbTables::LINE_NOS>,
                     PkbTables::STATEMENT_TYPE_TABLE>
      TABLES_TUPLE;

  /** The type of the node, initialized by the constructor. */
  CfgNode::Type type;

  /** The entry point of the node, initialized by the constructor. */
  PkbTables::LINE_NO entryPoint;

  /** The exit point of the node, initialized by the constructor. */
  PkbTables::LINE_NO exitPoint;

  /** CfgNode that this CfgNode nests, eventually initialized if this->type is:
   * CfgNode::Type::ThenBlock.
   * CfgNode::Type::ElseBlock.
   * CfgNode::Type::WhileBlock.
   */
  std::shared_ptr<CfgNode> nestedBlock;

  /** May eventually point to CfgNode that this CfgNode has an edge to. */
  std::shared_ptr<CfgNode> basicBlock;

  /** Eventually points to CfgNode that this CfgNode has an edge to, if
   * this->type is CfgNode::Type::IfStatement. */
  std::shared_ptr<CfgNode> thenBlock;

  /** Eventually points to CfgNode that this CfgNode has an edge to, if
   * this->type is CfgNode::Type::IfStatement. */
  std::shared_ptr<CfgNode> elseBlock;

  /** Eventually points to CfgNode that this CfgNode has an edge to, if
   * this->type is CfgNode::Type::WhileStatement. */
  std::shared_ptr<CfgNode> whileBlock;

  /** May eventually point to CfgNode that this CfgNode has an edge to. */
  std::shared_ptr<CfgNode> ifStatement;

  /** May eventually point to CfgNode that this CfgNode has an edge to. */
  std::shared_ptr<CfgNode> whileStatement;

  /**< Construct CfgNode with a CfgNode::Type. */
  CfgNode(CfgNode::Type type, PkbTables::LINE_NO entryPoint,
          PkbTables::LINE_NO exitPoint);

  /** @brief Derives the CFG for lineNosUnordered.
  @param node_ptr A reference to a pointer to a CfgNode, to be set to the
  pointer to the derived CFG.
  @param lineNosUnordered Line numbers to derive the CFG for.
  @param tablesTuple A tuple of tables.
  */
  static void deriveCfg(std::shared_ptr<CfgNode> &node_ptr,
                        PkbTables::LINE_NOS lineNosUnordered,
                        TABLES_TUPLE tablesTuple);

  /** @brief An method auxiliary to deriveCfg. Derives the CFG for an
  if-statement.
  @param node_ptr A reference to a pointer to a CfgNode, to be set to the
  pointer to the derived CFG.
  @param lineNo The line number of the if-statement.
  @param tablesTuple A tuple of tables.
  */
  static void deriveCfgIf(std::shared_ptr<CfgNode> &node_ptr,
                          PkbTables::LINE_NO lineNo, TABLES_TUPLE tablesTuple);

  /** @brief An method auxiliary to deriveCfg. Derives the CFG for an
  while-statement.
  @param node_ptr A reference to a pointer to a CfgNode, to be set to the
  pointer to the derived CFG.
  @param lineNo The line number of the while-statement.
  @param tablesTuple A tuple of tables.
  */
  static void deriveCfgWhile(std::shared_ptr<CfgNode> &node_ptr,
                             PkbTables::LINE_NO lineNo,
                             TABLES_TUPLE tablesTuple);

  /** @brief Gets the exitpoint of the last basic block in the block.
  @param node_ptr A reference to a pointer to a CfgNode, which is the block to
  be traversed on.
  @return The exitpoint of the last basic block.
  */
  static PkbTables::LINE_NO
  getBlockExitPoint(std::shared_ptr<CfgNode> node_ptr);

  /** @brief
  @param setOne The first set.
  @param setTwo The second set.
  @return The set difference setOne \ setTwo.
  */
  template <class T>
  static std::unordered_set<T> setDifference(std::unordered_set<T> setOne,
                                             std::unordered_set<T> setTwo);
};

template <class T>
static std::unordered_set<T>
CfgNode::setDifference(std::unordered_set<T> setOne,
                       std::unordered_set<T> setTwo) {
  std::unordered_set<T> result;
  for (T value : setOne) {
    if (setTwo.find(value) == setTwo.end()) {
      result.insert(value);
    }
  }
  return result;
}
