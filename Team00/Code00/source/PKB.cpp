#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "PKB.h"

template <class Key, class T>
bool KeysTable<Key, T>::insert(const KeysTable::value_type &value) {
  this->keys.push_back(std::get<const Key>(value));
  return std::get<bool>(this->map.insert(value));
}
template <class Key, class T>
typename std::unordered_map<Key, T>::size_type KeysTable<Key, T>::size() {
  return this->map.size();
}

template <class Key, class T> void InvertibleTable<Key, T>::invert() {
  for (Key key : this->keys) {
    T value = this->map[key];
    this->mapInverted.insert({value, key});
  }
}

template <class Key, class T> void ClosableTable<Key, T>::close() {
  for (Key key : this->keys) {
    T value = this->map[key];
    this->mapClosed.insert({key, {value}});
  }
  for (Key key : this->keys) {
    std::vector<T> values = this->mapClosed[key];
    for (T value : values) {
      auto p1 = {key, value};
      auto p2 = this->mapClosed.find(value);
      if (p2 != this->mapClosed.end()) {
        std::vector<T> v2 = p2->second;
        this->mapClosed[key].insert(this->mapClosed[key].end(), v2.begin(),
                                    v2.end());
      }
    }
  }
}

template <class Key, class T>
void PseudoInvertibleTable<Key, T>::pseudoInvert() {
  for (Key key : this->keys) {
    T value = this->map[key];
    auto p = this->mapPseudoInverted.find(value);
    if (p == this->mapPseudoInverted.end()) {
      this->mapPseudoInverted.insert({value, {key}});
    } else {
      this->mapPseudoInverted[value].push_back(key);
    }
  }
}

void PKB::invertVarTable() { this->varTable.invert(); }

void PKB::invertFollowTable() { this->followTable.invert(); }

void PKB::closeFollowTable() { this->followTable.close(); }

void PKB::pseudoInvertParentTable() { this->parentTable.pseudoInvert(); }

void PKB::closeParentTable() { this->parentTable.close(); }

VAR_TABLE_INDEX PKB::addVar(VAR var) {
  VAR_TABLE_INDEX index = this->varTable.size();
  if (this->varTable.insert({var, index})) {
    return index; // if insertion took place, return insertion index
  } else {
    return this->varTable.map[var]; // otherwise, return existing index
  }
}

VAR PKB::getVar(VAR_TABLE_INDEX varTableIndex) {
  return this->varTable.mapInverted[varTableIndex];
}

VAR_TABLE_INDEX PKB::getVarTableIndex(VAR var) {
  return this->varTable.map[var];
}

PROC_TABLE_INDEX PKB::addProc(PROC proc) {
  PROC_TABLE_INDEX index = this->procTable.size();
  this->procTable.push_back(proc);
  return index;
}

PROC PKB::getProc(PROC_TABLE_INDEX index) { return this->procTable[index]; }

void PKB::addUses(LINE_NO lineNo, USES uses) { this->usesTable[lineNo] = uses; }

USES PKB::getUses(LINE_NO lineNo) { return this->usesTable[lineNo]; }

void PKB::addUsesProc(PROC_TABLE_INDEX procTableIndex,
                      VAR_TABLE_INDEXES varTableIndexes) {
  this->usesProcTable[procTableIndex] = varTableIndexes;
}

VAR_TABLE_INDEXES PKB::getUsesProc(PROC_TABLE_INDEX procTableIndex) {
  return this->usesProcTable[procTableIndex];
}

void PKB::addModifies(LINE_NO lineNo, MODIFIES modifies) {
  this->modifiesTable[lineNo] = modifies;
}

MODIFIES PKB::getModifies(LINE_NO lineNo) {
  return this->modifiesTable[lineNo];
}

void PKB::addModifiesProc(PROC_TABLE_INDEX procTableIndex,
                          VAR_TABLE_INDEXES varTableIndexes) {
  this->modifiesProcTable[procTableIndex] = varTableIndexes;
}

VAR_TABLE_INDEXES PKB::getModifiesProc(PROC_TABLE_INDEX procTableIndex) {
  return this->modifiesProcTable[procTableIndex];
}

void PKB::addFollow(LINE_NO lineNo, FOLLOW follow) {
  this->followTable.insert({lineNo, follow});
}

FOLLOW PKB::getFollow(LINE_NO lineNo) { return this->followTable.map[lineNo]; }

LINE_NO PKB::getFollowLineNo(FOLLOW follow) {
  return this->followTable.mapInverted[follow];
}

FOLLOWS PKB::getFollowStar(LINE_NO lineNo) {
  return this->followTable.mapClosed[lineNo];
}

void PKB::addParent(CHILD child, PARENT parent) {
  this->parentTable.insert({child, parent});
}

PARENT PKB::getParent(CHILD child) { return this->parentTable.map[child]; }

CHILDREN PKB::getParentChildren(PARENT parent) {
  return this->parentTable.mapPseudoInverted[parent];
}

PARENTS PKB::getParentStar(LINE_NO lineNo) {
  return this->parentTable.mapClosed[lineNo];
}

void PKB::addStatementProc(LINE_NO lineNo, PROC statementProc) {
  this->statementProcTable[lineNo] = statementProc;
}

PROC PKB::getStatementProc(LINE_NO lineNo) {
  return this->statementProcTable[lineNo];
}

void PKB::addStatementType(LINE_NO lineNo, StatementType statementType) {
  this->statementTypeTable[lineNo] = statementType;
}

StatementType PKB::getStatementType(LINE_NO lineNo) {
  return this->statementTypeTable[lineNo];
}

void PKB::addAssignAst(LINE_NO lineNo, AST ast) {
  this->assignAstTable[lineNo] = ast;
}

AST PKB::getAssignAst(LINE_NO lineNo) { return this->assignAstTable[lineNo]; }
