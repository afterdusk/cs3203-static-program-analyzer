#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "PkbTables.h"

KeysTable<PkbTables::LINE_NO, PkbTables::VAR_TABLE_INDEXES> PkbTables::transit(
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

const PkbTables::VAR_TABLE &PkbTables::getVarTable() const {
  return this->varTable;
}

const PkbTables::PROC_TABLE &PkbTables::getProcTable() const {
  return this->procTable;
}

const PkbTables::USES_TABLE &PkbTables::getUsesTable() const {
  return this->usesTable;
}

const PkbTables::USES_PROC_TABLE &PkbTables::getUsesProcTable() const {
  return this->usesProcTable;
}

const PkbTables::MODIFIES_TABLE &PkbTables::getModifiesTable() const {
  return this->modifiesTable;
}

const PkbTables::MODIFIES_PROC_TABLE &PkbTables::getModifiesProcTable() const {
  return this->modifiesProcTable;
}

const PkbTables::FOLLOW_TABLE &PkbTables::getFollowTable() const {
  return this->followTable;
}

const PkbTables::PARENT_TABLE &PkbTables::getParentTable() const {
  return this->parentTable;
}

const PkbTables::STATEMENT_PROC_TABLE &
PkbTables::getStatementProcTable() const {
  return this->statementProcTable;
}

const PkbTables::STATEMENT_TYPE_TABLE &
PkbTables::getStatementTypeTable() const {
  return this->statementTypeTable;
}

const PkbTables::ASSIGN_AST_TABLE &PkbTables::getAssignAstTable() const {
  return this->assignAstTable;
}

const PkbTables::CONSTANT_TABLE &PkbTables::getConstantTable() const {
  return this->constantTable;
}

const PkbTables::CALLS_TABLE &PkbTables::getCallsTable() const {
  return this->callsTable;
}

PkbTables::VAR_TABLE_INDEX PkbTables::addVar(VAR var) {
  VAR_TABLE_INDEX index = this->varTable.size() + 1; // offset index by 1
  if (this->varTable.insert({var, index})) {
    return index; // if insertion took place, return insertion index
  } else {
    return this->varTable.map[var]; // otherwise, return existing index
  }
}

PkbTables::PROC_TABLE_INDEX PkbTables::addProc(PROC proc) {
  PROC_TABLE_INDEX index = this->procTable.size() + 1; // offset index by 1
  if (this->procTable.insert({proc, index})) {
    return index; // if insertion took place, return insertion index
  } else {
    return this->procTable.map[proc]; // otherwise, return existing index
  }
}

void PkbTables::addUses(LINE_NO lineNo, USES uses) {
  this->usesTable.insert({lineNo, uses});
}

void PkbTables::addUsesProc(PROC_TABLE_INDEX procTableIndex,
                            VAR_TABLE_INDEXES varTableIndexes) {
  this->usesProcTable.insert({procTableIndex, varTableIndexes});
}

void PkbTables::addModifies(LINE_NO lineNo, MODIFIES modifies) {
  this->modifiesTable.insert({lineNo, modifies});
}

void PkbTables::addModifiesProc(PROC_TABLE_INDEX procTableIndex,
                                VAR_TABLE_INDEXES varTableIndexes) {
  this->modifiesProcTable.insert({procTableIndex, varTableIndexes});
}

void PkbTables::addFollow(LINE_NO lineNo, FOLLOW follow) {
  this->followTable.insert({lineNo, follow});
}

void PkbTables::addParent(CHILD child, PARENT parent) {
  this->parentTable.insert({child, parent});
}

void PkbTables::addStatementProc(LINE_NO lineNo, PROC statementProc) {
  this->statementProcTable.insert({lineNo, statementProc});
}

void PkbTables::addStatementType(LINE_NO lineNo, StatementType statementType) {
  this->statementTypeTable.insert({lineNo, statementType});
}

void PkbTables::addAssignAst(LINE_NO lineNo, AST ast) {
  this->assignAstTable.insert({lineNo, ast});
}

void PkbTables::addConstant(CONSTANT constant) {
  this->constantTable.insert(constant);
}

void PkbTables::addCall(PROC_TABLE_INDEX pti, CALL call) {
  if (!this->callsTable.insert({pti, {call}})) {
    // if `pti` mapped, then insert `call` into existing mapped unordered_set.
    this->callsTable.map[pti].insert(call);
  }
}
