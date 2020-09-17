#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "Pkb.h"

KeysTable<LINE_NO, VAR_TABLE_INDEXES> Pkb::transit(
    KeysTable<LINE_NO, std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX>> table,
    KeysTable<PROC_TABLE_INDEX, VAR_TABLE_INDEXES> procTable) {
  KeysTable<LINE_NO, VAR_TABLE_INDEXES> mapTransited;
  for (LINE_NO key : table.keys) {
    std::variant<VAR_TABLE_INDEXES, PROC_TABLE_INDEX> tableValue =
        table.map[key];
    VAR_TABLE_INDEXES value;
    if (std::holds_alternative<PROC_TABLE_INDEX>(tableValue)) {
      value = procTable.map[std::get<PROC_TABLE_INDEX>(tableValue)];
    } else if (std::holds_alternative<VAR_TABLE_INDEXES>(tableValue)) {
      value = std::get<VAR_TABLE_INDEXES>(tableValue);
    }
    mapTransited.insert({key, value});
  }
  return mapTransited;
}

const VAR_TABLE &Pkb::getVarTable() const { return this->varTable; }

const PROC_TABLE &Pkb::getProcTable() const { return this->procTable; }

const USES_TABLE &Pkb::getUsesTable() const { return this->usesTable; }

const USES_PROC_TABLE &Pkb::getUsesProcTable() const {
  return this->usesProcTable;
}

const MODIFIES_TABLE &Pkb::getModifiesTable() const {
  return this->modifiesTable;
}

const MODIFIES_PROC_TABLE &Pkb::getModifiesProcTable() const {
  return this->modifiesProcTable;
}

const FOLLOW_TABLE &Pkb::getFollowTable() const { return this->followTable; }

const PARENT_TABLE &Pkb::getParentTable() const { return this->parentTable; }

const STATEMENT_PROC_TABLE &Pkb::getStatementProcTable() const {
  return this->statementProcTable;
}

const STATEMENT_TYPE_TABLE &Pkb::getStatementTypeTable() const {
  return this->statementTypeTable;
}

const ASSIGN_AST_TABLE &Pkb::getAssignAstTable() const {
  return this->assignAstTable;
}

const CONSTANT_TABLE &Pkb::getConstantTable() const {
  return this->constantTable;
}

const CALLS_TABLE &Pkb::getCallsTable() const { return this->callsTable; }

VAR_TABLE_INDEX Pkb::addVar(VAR var) {
  VAR_TABLE_INDEX index = this->varTable.size() + 1; // offset index by 1
  if (this->varTable.insert({var, index})) {
    return index; // if insertion took place, return insertion index
  } else {
    return this->varTable.map[var]; // otherwise, return existing index
  }
}

PROC_TABLE_INDEX Pkb::addProc(PROC proc) {
  PROC_TABLE_INDEX index = this->procTable.size() + 1; // offset index by 1
  if (this->procTable.insert({proc, index})) {
    return index; // if insertion took place, return insertion index
  } else {
    return this->procTable.map[proc]; // otherwise, return existing index
  }
}

void Pkb::addUses(LINE_NO lineNo, USES uses) {
  this->usesTable.insert({lineNo, uses});
}

void Pkb::addUsesProc(PROC_TABLE_INDEX procTableIndex,
                      VAR_TABLE_INDEXES varTableIndexes) {
  this->usesProcTable.insert({procTableIndex, varTableIndexes});
}

void Pkb::addModifies(LINE_NO lineNo, MODIFIES modifies) {
  this->modifiesTable.insert({lineNo, modifies});
}

void Pkb::addModifiesProc(PROC_TABLE_INDEX procTableIndex,
                          VAR_TABLE_INDEXES varTableIndexes) {
  this->modifiesProcTable.insert({procTableIndex, varTableIndexes});
}

void Pkb::addFollow(LINE_NO lineNo, FOLLOW follow) {
  this->followTable.insert({lineNo, follow});
}

void Pkb::addParent(CHILD child, PARENT parent) {
  this->parentTable.insert({child, parent});
}

void Pkb::addStatementProc(LINE_NO lineNo, PROC statementProc) {
  this->statementProcTable.insert({lineNo, statementProc});
}

void Pkb::addStatementType(LINE_NO lineNo, StatementType statementType) {
  this->statementTypeTable.insert({lineNo, statementType});
}

void Pkb::addAssignAst(LINE_NO lineNo, AST ast) {
  this->assignAstTable.insert({lineNo, ast});
}

void Pkb::addConstant(CONSTANT constant) {
  this->constantTable.insert(constant);
}

void Pkb::addCall(PROC_TABLE_INDEX pti, CALL call) {
  if (!this->callsTable.insert({pti, {call}})) {
    // if `pti` mapped, then insert `call` into existing mapped unordered_set.
    this->callsTable.map[pti].insert(call);
  }
}