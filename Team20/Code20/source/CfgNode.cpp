#include "CfgNode.h"

CfgNode::CfgNode(CfgNode::Type type, PkbTables::LINE_NO entryPoint,
                 PkbTables::LINE_NO exitPoint) {
  this->type = type;
  this->entryPoint = entryPoint;
  this->exitPoint = exitPoint;
}

void CfgNode::deriveCfg(std::shared_ptr<CfgNode> &node_ptr,
                        PkbTables::LINE_NOS lineNosUnordered,
                        TABLES_TUPLE tablesTuple) {
  // Get the tables from tablesTuple.
  KeysTable<PkbTables::LINE_NO, PkbTables::FOLLOWS> followTableClosed =
      std::get<0>(tablesTuple);
  KeysTable<PkbTables::PARENT, PkbTables::LINE_NOS> parentTablePseudoinverted =
      std::get<1>(tablesTuple);
  PkbTables::STATEMENT_TYPE_TABLE statementTypeTable = std::get<2>(tablesTuple);

  // Get unordered set of Statements.
  PkbTables::LINE_NO lineNo =
      *(std::min_element(lineNosUnordered.begin(), lineNosUnordered.end()));
  PkbTables::FOLLOWS statementsUnordered =
      followTableClosed.map[lineNo];  // Only the Statements that follow lineNo.
  statementsUnordered.insert(lineNo); // All Statements in the StatementList.

  // Order the Statements.
  std::set<PkbTables::LINE_NO> statements;
  std::copy(statementsUnordered.begin(), statementsUnordered.end(),
            std::inserter(statements, statements.end()));

  // Generate the first of the nodes.
  // Guaranteed to exist because statements.size() >= 1,
  // and a statement is either a basic block, if statement, or while statement.

  // Keep track of the statements iterator between node generations.
  auto it = statements.begin();

  // Increment the statements iterator.
  PkbTables::StatementType statementType = statementTypeTable.map[lineNo];
  if (statementType == PkbTables::StatementType::If) {
    deriveCfgIf(node_ptr, lineNo, tablesTuple);
    it++;
  } else if (statementType == PkbTables::StatementType::While) {
    deriveCfgWhile(node_ptr, lineNo, tablesTuple);
    it++;
  } else {
    /* First statement belongs to a basic block.
    Find the exitpoint of the basic block, which is the statement before the
    first found if- or while-statement, if either an if- or while-statement is
    found.
    */
    // The entrypoint is the current line number.
    PkbTables::LINE_NO entryPoint = lineNo;
    // This is the exitpoint if no if-statements or while-statements are
    // found.
    PkbTables::LINE_NO exitPoint = *(std::prev(statements.end()));
    for (; it != statements.end(); ++it) {
      lineNo = *it;
      statementType = statementTypeTable.map[lineNo];
      if ((statementType == PkbTables::StatementType::If) ||
          (statementType == PkbTables::StatementType::While)) {
        exitPoint = *std::prev(it);
        break;
      }
    }
    node_ptr = std::make_shared<CfgNode>(CfgNode::Type::BasicBlock, entryPoint,
                                         exitPoint);

    // If `it` is iterator to the element following the last element,
    // then do not continue to the next for-loop, because there will be an
    // exception: cannot increment `it`.
    if (it == statements.end()) {
      return;
    }
  }

  // The previously generated node.
  // Changes just before the next node is generated.
  std::shared_ptr<CfgNode> prev = std::shared_ptr<CfgNode>(node_ptr);

  // Generate the rest of the nodes.
  for (; it != statements.end(); ++it) {
    lineNo = *it;
    statementType = statementTypeTable.map[lineNo];
    if (statementType == PkbTables::StatementType::If) {
      deriveCfgIf(prev->ifStatement, lineNo, tablesTuple);
      prev = prev->ifStatement;
    } else if (statementType == PkbTables::StatementType::While) {
      deriveCfgWhile(prev->whileStatement, lineNo, tablesTuple);
      prev = prev->whileStatement;
    } else {
      /* First statement belongs to a basic block.
      Find the exitpoint of the basic block, which is the statement before the
      first found if- or while-statement, if either an if- or while-statement is
      found.
      */
      // The entrypoint is the current line number.
      PkbTables::LINE_NO entryPoint = lineNo;
      // This is the exitpoint if no if-statements or while-statements are
      // found.
      PkbTables::LINE_NO exitPoint = *(std::prev(statements.end()));
      for (; it != statements.end(); ++it) {
        lineNo = *it;
        statementType = statementTypeTable.map[lineNo];
        if ((statementType == PkbTables::StatementType::If) ||
            (statementType == PkbTables::StatementType::While)) {
          exitPoint = *std::prev(it);
          break;
        }
      }
      prev->basicBlock = std::make_shared<CfgNode>(CfgNode::Type::BasicBlock,
                                                   entryPoint, exitPoint);
      prev = prev->basicBlock;

      // If `it` is iterator to the element following the last element,
      // then do not continue to the next for-loop, because there will be an
      // exception: cannot increment `it`.
      if (it == statements.end()) {
        break;
      }
    }
  }
}

void CfgNode::deriveCfgIf(std::shared_ptr<CfgNode> &node_ptr,
                          PkbTables::LINE_NO lineNo, TABLES_TUPLE tablesTuple) {
  // Get the tables from tablesTuple.
  KeysTable<PkbTables::LINE_NO, PkbTables::FOLLOWS> followTableClosed =
      std::get<0>(tablesTuple);
  KeysTable<PkbTables::PARENT, PkbTables::LINE_NOS> parentTablePseudoinverted =
      std::get<1>(tablesTuple);
  PkbTables::STATEMENT_TYPE_TABLE statementTypeTable = std::get<2>(tablesTuple);

  node_ptr =
      std::make_shared<CfgNode>(CfgNode::Type::IfStatement, lineNo, lineNo);

  PkbTables::CHILDREN children = parentTablePseudoinverted.map[lineNo];
  PkbTables::FOLLOW thenBlockStatementFirst =
      *(std::min_element(children.begin(), children.end()));
  PkbTables::FOLLOWS thenBlockStatements =
      followTableClosed
          .map[thenBlockStatementFirst]; // Only the Statements that follow
                                         // thenBlockStatementFirst.
  thenBlockStatements.insert(
      thenBlockStatementFirst); // All Statements in the thenBlock.
  std::shared_ptr<CfgNode> thenBlock =
      std::make_shared<CfgNode>(CfgNode::Type::ThenBlock,
                                *(std::min_element(thenBlockStatements.begin(),
                                                   thenBlockStatements.end())),
                                *(std::max_element(thenBlockStatements.begin(),
                                                   thenBlockStatements.end())));
  deriveCfg(thenBlock->nestedBlock, thenBlockStatements, tablesTuple);
  thenBlock->exitPoint = getBlockExitPoint(thenBlock->nestedBlock);

  PkbTables::CHILDREN elseBlockStatements =
      CfgNode::setDifference(children, thenBlockStatements);
  std::shared_ptr<CfgNode> elseBlock =
      std::make_shared<CfgNode>(CfgNode::Type::ElseBlock,
                                *(std::min_element(elseBlockStatements.begin(),
                                                   elseBlockStatements.end())),
                                *(std::max_element(elseBlockStatements.begin(),
                                                   elseBlockStatements.end())));
  deriveCfg(elseBlock->nestedBlock, elseBlockStatements, tablesTuple);
  elseBlock->exitPoint = getBlockExitPoint(elseBlock->nestedBlock);

  node_ptr->thenBlock = thenBlock;
  node_ptr->elseBlock = elseBlock;
}

void CfgNode::deriveCfgWhile(std::shared_ptr<CfgNode> &node_ptr,
                             PkbTables::LINE_NO lineNo,
                             TABLES_TUPLE tablesTuple) {
  // Get the tables from tablesTuple.
  KeysTable<PkbTables::LINE_NO, PkbTables::FOLLOWS> followTableClosed =
      std::get<0>(tablesTuple);
  KeysTable<PkbTables::PARENT, PkbTables::LINE_NOS> parentTablePseudoinverted =
      std::get<1>(tablesTuple);
  PkbTables::STATEMENT_TYPE_TABLE statementTypeTable = std::get<2>(tablesTuple);

  node_ptr =
      std::make_shared<CfgNode>(CfgNode::Type::WhileStatement, lineNo, lineNo);

  PkbTables::CHILDREN whileBlockStatements =
      parentTablePseudoinverted.map[lineNo];
  std::shared_ptr<CfgNode> whileBlock = std::make_shared<CfgNode>(
      CfgNode::Type::WhileBlock, *(whileBlockStatements.begin()),
      *(std::prev(whileBlockStatements.end())));
  deriveCfg(whileBlock->nestedBlock, whileBlockStatements, tablesTuple);
  whileBlock->exitPoint = getBlockExitPoint(whileBlock->nestedBlock);

  node_ptr->whileBlock = whileBlock;
}

PkbTables::LINE_NO
CfgNode::getBlockExitPoint(std::shared_ptr<CfgNode> node_ptr) {
  PkbTables::LINE_NO lastExitPoint = node_ptr->exitPoint;
  if (node_ptr->nestedBlock) {
    lastExitPoint =
        std::max(lastExitPoint, getBlockExitPoint(node_ptr->nestedBlock));
  }
  if (node_ptr->basicBlock) {
    lastExitPoint =
        std::max(lastExitPoint, getBlockExitPoint(node_ptr->basicBlock));
  }
  if (node_ptr->thenBlock) {
    lastExitPoint =
        std::max(lastExitPoint, getBlockExitPoint(node_ptr->thenBlock));
  }
  if (node_ptr->elseBlock) {
    lastExitPoint =
        std::max(lastExitPoint, getBlockExitPoint(node_ptr->elseBlock));
  }
  if (node_ptr->whileBlock) {
    lastExitPoint =
        std::max(lastExitPoint, getBlockExitPoint(node_ptr->whileBlock));
  }
  if (node_ptr->ifStatement) {
    lastExitPoint =
        std::max(lastExitPoint, getBlockExitPoint(node_ptr->ifStatement));
  }
  if (node_ptr->whileStatement) {
    lastExitPoint =
        std::max(lastExitPoint, getBlockExitPoint(node_ptr->whileStatement));
  }
  return lastExitPoint;
}
