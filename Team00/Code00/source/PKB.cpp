#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "PKB.h"

const VAR_TABLE &PKB::getVarTable() const { return this->varTable; }

const PROC_TABLE &PKB::getProcTable() const { return this->procTable; }

const USES_TABLE &PKB::getUsesTable() const { return this->usesTable; }

const USES_PROC_TABLE &PKB::getUsesProcTable() const {
  return this->usesProcTable;
}

const MODIFIES_TABLE &PKB::getModifiesTable() const {
  return this->modifiesTable;
}

const MODIFIES_PROC_TABLE &PKB::getModifiesProcTable() const {
  return this->modifiesProcTable;
}

const FOLLOW_TABLE &PKB::getFollowTable() const { return this->followTable; }

const PARENT_TABLE &PKB::getParentTable() const { return this->parentTable; }

const STATEMENT_PROC_TABLE &PKB::getStatementProcTable() const {
  return this->statementProcTable;
}

const STATEMENT_TYPE_TABLE &PKB::getStatementTypeTable() const {
  return this->statementTypeTable;
}

const ASSIGN_AST_TABLE &PKB::getAssignAstTable() const {
  return this->assignAstTable;
}

VAR_TABLE_INDEX PKB::addVar(VAR var) {
  VAR_TABLE_INDEX index = this->varTable.size() + 1; // offset index by 1
  if (this->varTable.insert({var, index})) {
    return index; // if insertion took place, return insertion index
  } else {
    return this->varTable.map[var]; // otherwise, return existing index
  }
}

PROC_TABLE_INDEX PKB::addProc(PROC proc) {
  PROC_TABLE_INDEX index = this->procTable.size() + 1; // offset index by 1
  if (this->procTable.insert({proc, index})) {
    return index; // if insertion took place, return insertion index
  } else {
    return this->procTable.map[proc]; // otherwise, return existing index
  }
}

void PKB::addUses(LINE_NO lineNo, USES uses) {
  this->usesTable.map[lineNo] = uses;
}

void PKB::addUsesProc(PROC_TABLE_INDEX procTableIndex,
                      VAR_TABLE_INDEXES varTableIndexes) {
  this->usesProcTable.map[procTableIndex] = varTableIndexes;
}

void PKB::addModifies(LINE_NO lineNo, MODIFIES modifies) {
  this->modifiesTable.map[lineNo] = modifies;
}

void PKB::addModifiesProc(PROC_TABLE_INDEX procTableIndex,
                          VAR_TABLE_INDEXES varTableIndexes) {
  this->modifiesProcTable.map[procTableIndex] = varTableIndexes;
}

void PKB::addFollow(LINE_NO lineNo, FOLLOW follow) {
  this->followTable.insert({lineNo, follow});
}

void PKB::addParent(CHILD child, PARENT parent) {
  this->parentTable.insert({child, parent});
}

void PKB::addStatementProc(LINE_NO lineNo, PROC statementProc) {
  this->statementProcTable.map[lineNo] = statementProc;
}

void PKB::addStatementType(LINE_NO lineNo, StatementType statementType) {
  this->statementTypeTable.map[lineNo] = statementType;
}

void PKB::addAssignAst(LINE_NO lineNo, AST ast) {
  this->assignAstTable.map[lineNo] = ast;
}
