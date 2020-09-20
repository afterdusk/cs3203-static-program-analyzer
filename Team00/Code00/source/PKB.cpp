#include "Pkb.h"

PkbQueryInterface *Pkb::getQueryInterface() { return this; }

PkbTables *Pkb::getTables() { return this; }

const Pkb::VAR_TABLE &Pkb::getVarTable() const { return this->varTable; }

const Pkb::PROC_TABLE &Pkb::getProcTable() const { return this->procTable; }

const Pkb::USES_TABLE &Pkb::getUsesTable() const { return this->usesTable; }

const Pkb::USES_PROC_TABLE &Pkb::getUsesProcTable() const {
  return this->usesProcTable;
}

const Pkb::MODIFIES_TABLE &Pkb::getModifiesTable() const {
  return this->modifiesTable;
}

const Pkb::MODIFIES_PROC_TABLE &Pkb::getModifiesProcTable() const {
  return this->modifiesProcTable;
}

const Pkb::FOLLOW_TABLE &Pkb::getFollowTable() const {
  return this->followTable;
}

const Pkb::PARENT_TABLE &Pkb::getParentTable() const {
  return this->parentTable;
}

const Pkb::STATEMENT_PROC_TABLE &Pkb::getStatementProcTable() const {
  return this->statementProcTable;
}

const Pkb::STATEMENT_TYPE_TABLE &Pkb::getStatementTypeTable() const {
  return this->statementTypeTable;
}

const Pkb::ASSIGN_AST_TABLE &Pkb::getAssignAstTable() const {
  return this->assignAstTable;
}

const Pkb::CONSTANT_TABLE &Pkb::getConstantTable() const {
  return this->constantTable;
}

const Pkb::CALLS_TABLE &Pkb::getCallsTable() const { return this->callsTable; }

Pkb::VAR_TABLE_INDEX Pkb::addVar(VAR var) {
  VAR_TABLE_INDEX index = this->varTable.size() + 1; // offset index by 1
  if (this->varTable.insert({var, index})) {
    return index; // if insertion took place, return insertion index
  } else {
    return this->varTable.map[var]; // otherwise, return existing index
  }
}

Pkb::PROC_TABLE_INDEX Pkb::addProc(PROC proc) {
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

void Pkb::deriveTables() {
  this->prevLineTable = this->invert(this->getFollowTable());
  this->childrenTable = this->pseudoinvert(this->getParentTable());
  this->invertStatementTypeTable =
      this->pseudoinvert(this->getStatementTypeTable());
  this->invertStatementProcTable =
      this->pseudoinvert(this->getStatementProcTable());
  this->invertVarTable = this->invert(this->getVarTable());
  this->invertProcTable = this->invert(this->getProcTable());

  this->closeFollowTable = this->close(this->getFollowTable());
  this->closeParentTable = this->close(this->getParentTable());
  this->closePrevLineTable = this->close(prevLineTable);
  this->closeChildrenTable = this->closeFlatten<PARENT>(childrenTable);

  this->usesTableTransited =
      this->transit(this->getUsesTable(), this->getUsesProcTable());
  this->invertUsesTable =
      this->pseudoinvertFlattenKeys<Pkb::LINE_NO, Pkb::VAR_TABLE_INDEX>(
          usesTableTransited);
  this->modifiesTableTransited =
      this->transit(this->getModifiesTable(), this->getModifiesProcTable());
  this->invertModifiesTable =
      this->pseudoinvertFlattenKeys<Pkb::LINE_NO, Pkb::VAR_TABLE_INDEX>(
          modifiesTableTransited);

  this->stmtTableIndexes = STRING_SET(statementTypeTable.keys.begin(),
                                      statementTypeTable.keys.end());
  this->followTableIndexes =
      STRING_SET(followTable.keys.begin(), followTable.keys.end());
  this->parentTableIndexes =
      STRING_SET(parentTable.keys.begin(), parentTable.keys.end());
  this->prevLineTableIndexes =
      STRING_SET(prevLineTable.keys.begin(), prevLineTable.keys.end());
  this->childrenTableIndexes =
      STRING_SET(childrenTable.keys.begin(), childrenTable.keys.end());

  this->varNamesSet = STRING_SET(varTable.keys.begin(), varTable.keys.end());
  this->procNamesSet = STRING_SET(procTable.keys.begin(), procTable.keys.end());
}

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

// Query API for pattern matching
STRING_PAIRS Pkb::match(Statement statement, Variable variable,
                        PatternSpec spec) {
  STRING_PAIRS result;

  if (statement.type == PkbTables::StatementType::ASSIGN) {
    STRING_SET assignLines = invertStatementTypeTable.map[statement.type];

    if (spec.type == PatternMatchType::Any) {
      for (auto line : assignLines) {
        PkbTables::VAR_TABLE_INDEXES varIndexes =
            std::get<PkbTables::VAR_TABLE_INDEXES>(modifiesTable.map[line]);

        // Since line is an assignment statement, there must be only one index
        // in varIndexes.
        PkbTables::VAR_TABLE_INDEX modifiedVarIndex = *varIndexes.begin();

        result.first.push_back(line);
        result.second.push_back(invertVarTable.map[modifiedVarIndex]);
      }
    } else if (spec.type == PatternMatchType::CompleteMatch) {
      for (auto line : assignLines) {
        PkbTables::AST astOnLine = assignAstTable.map[line];

        if (astOnLine == *spec.value) {
          PkbTables::VAR_TABLE_INDEXES varIndexes =
              std::get<PkbTables::VAR_TABLE_INDEXES>(modifiesTable.map[line]);

          // Since line is an assignment statement, there must be only one index
          // in varIndexes.
          PkbTables::VAR_TABLE_INDEX modifiedVarIndex = *varIndexes.begin();

          result.first.push_back(line);
          result.second.push_back(invertVarTable.map[modifiedVarIndex]);
        }
      }
    } else if (spec.type == PatternMatchType::SubTreeMatch) {
      for (auto line : assignLines) {
        PkbTables::AST astOnLine = assignAstTable.map[line];

        if (astOnLine >= *spec.value) {
          PkbTables::VAR_TABLE_INDEXES varIndexes =
              std::get<PkbTables::VAR_TABLE_INDEXES>(modifiesTable.map[line]);

          // Since line is an assignment statement, there must be only one index
          // in varIndexes.
          PkbTables::VAR_TABLE_INDEX modifiedVarIndex = *varIndexes.begin();

          result.first.push_back(line);
          result.second.push_back(invertVarTable.map[modifiedVarIndex]);
        }
      }
    } else {
      // if pattern spec type is not assigned, return an empty set.
      // (update) throw exception instead when exception format is received
      return result;
    }
  }
  return result;
}

STRING_SET Pkb::match(Statement statement, Underscore underscore,
                      PatternSpec spec) {
  STRING_SET result;

  if (statement.type == PkbTables::StatementType::ASSIGN) {
    STRING_SET assignLines = invertStatementTypeTable.map[statement.type];

    if (spec.type == PatternMatchType::Any) {
      // if no matching is required, then return assignment lines without
      // processing.
      return assignLines;
    } else {
      for (auto line : assignLines) {
        PkbTables::AST astOnLine = assignAstTable.map[line];

        if (spec.type == PatternMatchType::CompleteMatch) {
          if (astOnLine == *spec.value) {
            result.insert(line);
          }
        } else if (spec.type == PatternMatchType::SubTreeMatch) {
          if (astOnLine >= *spec.value) {
            result.insert(line);
          }
        } else {
          // if pattern spec type is not assigned, return an empty set.
          // (update) throw exception instead when exception format is received
          return result;
        }
      }
    }
  }

  return result;
}

STRING_SET Pkb::match(Statement statement, String variable, PatternSpec spec) {
  STRING_SET result;
  PkbTables::VAR_TABLE_INDEX inputVarTableIndex = varTable.map[variable.name];

  // if input variable name doesn't exist in varTable.
  if (inputVarTableIndex == PkbTables::VAR_TABLE_INDEX()) {
    return result;
  }

  if (statement.type == PkbTables::StatementType::ASSIGN) {
    STRING_SET linesVarModifies = invertModifiesTable.map[inputVarTableIndex];

    for (auto line : linesVarModifies) {
      if (statementTypeTable.map[line] == PkbTables::StatementType::ASSIGN) {
        PkbTables::AST astOnLine = assignAstTable.map[line];

        if (spec.type == PatternMatchType::CompleteMatch) {
          if (astOnLine == *spec.value) {
            result.insert(line);
          }
        } else if (spec.type == PatternMatchType::SubTreeMatch) {
          if (astOnLine >= *spec.value) {
            result.insert(line);
          }
        } else {
          result.insert(line);
        }
      }
    }
  }

  return result;
}

// Query API for normal select
STRING_SET Pkb::select(Variable var) { return varNamesSet; }
// Need tests
STRING_SET Pkb::select(Statement statement) {
  if (statement.type == PkbTables::StatementType::NONE) {
    return stmtTableIndexes;
  } else {
    return invertStatementTypeTable.map[statement.type];
  }
}

STRING_SET Pkb::select(Procedure proc) { return procNamesSet; }

STRING_SET Pkb::select(Constant constant) { return constantTable; }

// Query API for follows

bool Pkb::follows(LineNumber line1, LineNumber line2) {
  bool result = followTable.map[line1.number] == line2.number;
  return result;
}

STRING_SET Pkb::follows(LineNumber line, Statement statement) {
  PkbTables::FOLLOW followLine = followTable.map[line.number];

  if (statement.type == PkbTables::StatementType::NONE) {
    return followLine != PkbTables::LINE_NO() ? STRING_SET{followLine}
                                              : STRING_SET();
  } else {
    PkbTables::StatementType type = statementTypeTable.map[followLine];
    return followLine != PkbTables::LINE_NO() && statement.type == type
               ? STRING_SET{followLine}
               : STRING_SET();
  }
}

bool Pkb::follows(LineNumber line, Underscore underscore) {
  PkbTables::FOLLOW followLine = followTable.map[line.number];
  return followLine != PkbTables::LINE_NO();
}

STRING_SET Pkb::follows(Statement statement, LineNumber line) {
  PkbTables::LINE_NO prevLine = prevLineTable.map[line.number];

  if (statement.type == PkbTables::StatementType::NONE) {
    return prevLine != PkbTables::LINE_NO() ? STRING_SET{prevLine}
                                            : STRING_SET();
  } else {
    PkbTables::StatementType type = statementTypeTable.map[prevLine];

    return prevLine != PkbTables::LINE_NO() && statement.type == type
               ? STRING_SET{prevLine}
               : STRING_SET();
  }
}

STRING_PAIRS Pkb::follows(Statement statement1, Statement statement2) {
  STRING_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == PkbTables::StatementType::NONE &&
      statement2.type == PkbTables::StatementType::NONE) {
    result.first = followTable.keys;
    result.second = prevLineTable.keys;
  }

  // case 2: only statement1 is of type NONE
  else if (statement1.type == PkbTables::StatementType::NONE) {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement2.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      PkbTables::LINE_NO prevLine = prevLineTable.map[*it];
      if (prevLine != PkbTables::LINE_NO()) {
        result.first.push_back(prevLine);
        result.second.push_back(*it);
      }
      it++;
    }
  }

  // case 3: only statement2 is of type NONE
  else if (statement2.type == PkbTables::StatementType::NONE) {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement1.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      PkbTables::FOLLOW followLine = followTable.map[*it];
      if (followLine != PkbTables::LINE_NO()) {
        result.first.push_back(*it);
        result.second.push_back(followLine);
      }
      it++;
    }
  }

  // case 4: both statements are not of type NONE
  else if (statement1.type != PkbTables::StatementType::NONE &&
           statement2.type != PkbTables::StatementType::NONE) {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement1.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      PkbTables::FOLLOW followLine = followTable.map[*it];
      PkbTables::StatementType type = statementTypeTable.map[followLine];
      if (followLine != PkbTables::LINE_NO() && type == statement2.type) {
        result.first.push_back(*it);
        result.second.push_back(followLine);
      }
      it++;
    }
  }

  return result;
}

STRING_SET Pkb::follows(Statement statement, Underscore underscore) {
  if (statement.type == PkbTables::StatementType::NONE) {
    return followTableIndexes;
  } else {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      if (followTable.map[*it] == PkbTables::LINE_NO()) {
        it = lineNumbers.erase(it);
      } else {
        it++;
      }
    }

    return lineNumbers;
  }
}

bool Pkb::follows(Underscore underscore, LineNumber line) {
  PkbTables::LINE_NO prevLine = prevLineTable.map[line.number];
  return prevLine != PkbTables::LINE_NO();
}

STRING_SET Pkb::follows(Underscore underscore, Statement statement) {
  if (statement.type == PkbTables::StatementType::NONE) {
    return prevLineTableIndexes;
  } else {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      if (prevLineTable.map[*it] == PkbTables::LINE_NO()) {
        it = lineNumbers.erase(it);
      } else {
        it++;
      }
    }
    return lineNumbers;
  }
}

bool Pkb::follows(Underscore underscore1, Underscore underscore2) {
  return followTable.size() > 0;
}

// Query API for followsStar

bool Pkb::followsStar(LineNumber line1, LineNumber line2) {
  PkbTables::FOLLOWS followLines = closeFollowTable.map[line1.number];
  return followLines.find(line2.number) != followLines.end();
}

STRING_SET Pkb::followsStar(LineNumber line, Statement statement) {
  STRING_SET followLines = closeFollowTable.map[line.number];

  if (statement.type == PkbTables::StatementType::NONE) {
    return followLines;
  } else {
    STRING_SET::iterator it = followLines.begin();

    while (it != followLines.end()) {
      if (statementTypeTable.map[*it] != statement.type) {
        it = followLines.erase(it);
      } else {
        it++;
      }
    }
    return followLines;
  }
}

bool Pkb::followsStar(LineNumber line, Underscore underscore) {
  return follows(line, underscore);
}

STRING_SET Pkb::followsStar(Statement statement, LineNumber line) {
  STRING_SET prevLines = closePrevLineTable.map[line.number];

  if (statement.type == PkbTables::StatementType::NONE) {
    return prevLines;
  } else {
    STRING_SET::iterator it = prevLines.begin();

    while (it != prevLines.end()) {
      if (statementTypeTable.map[*it] != statement.type) {
        it = prevLines.erase(it);
      } else {
        it++;
      }
    }
    return prevLines;
  }
}

STRING_PAIRS Pkb::followsStar(Statement statement1, Statement statement2) {
  STRING_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == PkbTables::StatementType::NONE &&
      statement2.type == PkbTables::StatementType::NONE) {
    for (auto entry : closeFollowTable.map) {
      PkbTables::LINE_NO line = entry.first;

      for (auto followLine : entry.second) {
        result.first.push_back(line);
        result.second.push_back(followLine);
      }
    }
  }

  // case 2: only statement1 is of type NONE
  else if (statement1.type == PkbTables::StatementType::NONE) {
    STRING_SET statementLines = invertStatementTypeTable.map[statement2.type];

    for (auto line : statementLines) {
      STRING_SET prevLines = closePrevLineTable.map[line];

      for (auto prev : prevLines) {
        result.first.push_back(prev);
        result.second.push_back(line);
      }
    }
  }

  // case 3: only statement2 is of type NONE
  else if (statement2.type == PkbTables::StatementType::NONE) {
    STRING_SET statementLines = invertStatementTypeTable.map[statement1.type];

    for (auto line : statementLines) {
      STRING_SET followLines = closeFollowTable.map[line];

      for (auto follow : followLines) {
        result.first.push_back(line);
        result.second.push_back(follow);
      }
    }
  }

  // case 4: both statements are not of type NONE
  else if (statement1.type != PkbTables::StatementType::NONE &&
           statement2.type != PkbTables::StatementType::NONE) {
    STRING_SET statementLines = invertStatementTypeTable.map[statement1.type];

    for (auto line : statementLines) {
      STRING_SET followLines = closeFollowTable.map[line];

      for (auto follow : followLines) {
        if (statementTypeTable.map[follow] == statement2.type) {
          result.first.push_back(line);
          result.second.push_back(follow);
        }
      }
    }
  }

  return result;
}

STRING_SET Pkb::followsStar(Statement statement, Underscore underscore) {
  return follows(statement, underscore);
}

bool Pkb::followsStar(Underscore underscore, LineNumber line) {
  return follows(underscore, line);
}

STRING_SET Pkb::followsStar(Underscore underscore, Statement statement) {
  return follows(underscore, statement);
}

bool Pkb::followsStar(Underscore underscore1, Underscore underscore2) {
  return follows(underscore1, underscore2);
}

// Query API for parent

bool Pkb::parent(LineNumber line1, LineNumber line2) {
  bool result = parentTable.map[line2.number] == line1.number;
  return result;
}

STRING_SET Pkb::parent(LineNumber line, Statement statement) {

  STRING_SET children = childrenTable.map[line.number];

  if (statement.type == PkbTables::StatementType::NONE) {
    return children;
  } else {
    STRING_SET::iterator it = children.begin();

    while (it != children.end()) {
      if (statementTypeTable.map[*it] != statement.type) {
        it = children.erase(it);
      } else {
        it++;
      }
    }

    return children;
  }
}

bool Pkb::parent(LineNumber line, Underscore underscore) {
  STRING_SET children = childrenTable.map[line.number];
  return children.size() > 0;
}

STRING_SET Pkb::parent(Statement statement, LineNumber line) {
  PkbTables::PARENT parent = parentTable.map[line.number];
  if (statement.type == PkbTables::StatementType::NONE) {
    return parent != PkbTables::LINE_NO() ? STRING_SET{parent} : STRING_SET();
  } else {
    PkbTables::StatementType type = statementTypeTable.map[parent];

    return parent != PkbTables::LINE_NO() && type == statement.type
               ? STRING_SET{parent}
               : STRING_SET();
  }
}

STRING_PAIRS Pkb::parent(Statement statement1, Statement statement2) {
  STRING_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == PkbTables::StatementType::NONE &&
      statement2.type == PkbTables::StatementType::NONE) {
    STRING_SET parents = childrenTableIndexes;
    STRING_SET::iterator parentIt = parents.begin();

    while (parentIt != parents.end()) {
      PkbTables::CHILDREN children = childrenTable.map[*parentIt];
      STRING_SET::iterator childIt = children.begin();

      while (childIt != children.end()) {
        result.first.push_back(*parentIt);
        result.second.push_back(*childIt);
        childIt++;
      }

      parentIt++;
    }
  }

  // case 2: only statement1 is of type NONE
  else if (statement1.type == PkbTables::StatementType::NONE) {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement2.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      PkbTables::PARENT parent = parentTable.map[*it];
      if (parent != PkbTables::LINE_NO()) {
        result.first.push_back(parent);
        result.second.push_back(*it);
      }
      it++;
    }
  }

  // case 3: only statement2 is of type NONE
  else if (statement2.type == PkbTables::StatementType::NONE) {
    STRING_SET parents = childrenTableIndexes;
    STRING_SET::iterator parentIt = parents.begin();

    while (parentIt != parents.end()) {
      if (statementTypeTable.map[*parentIt] == statement1.type) {
        STRING_SET children = childrenTable.map[*parentIt];
        STRING_SET::iterator childIt = children.begin();

        while (childIt != children.end()) {
          result.first.push_back(*parentIt);
          result.second.push_back(*childIt);
          childIt++;
        }
      }

      parentIt++;
    }
  }

  // case 4: both statements are not of type NONE
  else if (statement1.type != PkbTables::StatementType::NONE &&
           statement2.type != PkbTables::StatementType::NONE) {
    STRING_SET parents = childrenTableIndexes;
    STRING_SET::iterator parentIt = parents.begin();

    while (parentIt != parents.end()) {
      if (statementTypeTable.map[*parentIt] == statement1.type) {
        STRING_SET children = childrenTable.map[*parentIt];
        STRING_SET::iterator childIt = children.begin();

        while (childIt != children.end()) {
          if (statementTypeTable.map[*childIt] == statement2.type) {
            result.first.push_back(*parentIt);
            result.second.push_back(*childIt);
          }

          childIt++;
        }
      }

      parentIt++;
    }
  }

  return result;
}

STRING_SET Pkb::parent(Statement statement, Underscore underscore) {
  if (statement.type == PkbTables::StatementType::NONE) {
    return childrenTableIndexes;
  } else {
    STRING_SET parents = childrenTableIndexes;
    STRING_SET::iterator it = parents.begin();

    while (it != parents.end()) {
      if (statementTypeTable.map[*it] != statement.type) {
        it = parents.erase(it);
      } else {
        it++;
      }
    }

    return parents;
  }
}

bool Pkb::parent(Underscore underscore, LineNumber line) {
  PkbTables::PARENT parent = parentTable.map[line.number];
  return parent != PkbTables::LINE_NO();
}

STRING_SET Pkb::parent(Underscore underscore, Statement statement) {
  if (statement.type == PkbTables::StatementType::NONE) {
    return parentTableIndexes;
  } else {
    STRING_SET children = parentTableIndexes;
    STRING_SET::iterator it = children.begin();

    while (it != children.end()) {
      if (statementTypeTable.map[*it] != statement.type) {
        it = children.erase(it);
      } else {
        it++;
      }
    }

    return children;
  }
}

bool Pkb::parent(Underscore underscore1, Underscore underscore2) {
  return parentTable.map.size() > 0;
}

// Query API for parentStar

bool Pkb::parentStar(LineNumber line1, LineNumber line2) {
  PkbTables::PARENTS parentLines = closeParentTable.map[line2.number];
  return parentLines.find(line1.number) != parentLines.end();
}

STRING_SET Pkb::parentStar(LineNumber line, Statement statement) {
  STRING_SET descendantLines = closeChildrenTable.map[line.number];

  if (statement.type == PkbTables::StatementType::NONE) {
    return descendantLines;
  } else {
    STRING_SET::iterator it = descendantLines.begin();
    while (it != descendantLines.end()) {
      if (statementTypeTable.map[*it] != statement.type) {
        it = descendantLines.erase(it);
      } else {
        it++;
      }
    }
    return descendantLines;
  }
}

bool Pkb::parentStar(LineNumber line, Underscore underscore) {
  return parent(line, underscore);
}

STRING_SET Pkb::parentStar(Statement statement, LineNumber line) {
  PkbTables::PARENTS parentLines = closeParentTable.map[line.number];

  if (statement.type == PkbTables::StatementType::NONE) {
    return parentLines;
  } else {
    PkbTables::PARENTS::iterator it = parentLines.begin();
    while (it != parentLines.end()) {
      if (statementTypeTable.map[*it] != statement.type) {
        it = parentLines.erase(it);
      } else {
        it++;
      }
    }
    return parentLines;
  }
}

STRING_PAIRS Pkb::parentStar(Statement statement1, Statement statement2) {
  STRING_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == PkbTables::StatementType::NONE &&
      statement2.type == PkbTables::StatementType::NONE) {
    for (auto entry : closeChildrenTable.map) {
      PkbTables::PARENT parentLine = entry.first;

      for (auto descendant : entry.second) {
        result.first.push_back(parentLine);
        result.second.push_back(descendant);
      }
    }
  }

  // case 2: only statement1 is of type NONE
  else if (statement1.type == PkbTables::StatementType::NONE) {
    STRING_SET statementLines = invertStatementTypeTable.map[statement2.type];

    for (auto child : statementLines) {
      PkbTables::PARENTS ancestors = closeParentTable.map[child];

      for (auto ancestor : ancestors) {
        result.first.push_back(ancestor);
        result.second.push_back(child);
      }
    }
  }

  // case 3: only statement2 is of type NONE
  else if (statement2.type == PkbTables::StatementType::NONE) {
    STRING_SET statementLines = invertStatementTypeTable.map[statement1.type];

    for (auto parent : statementLines) {
      STRING_SET descendants = closeChildrenTable.map[parent];

      for (auto descendant : descendants) {
        result.first.push_back(parent);
        result.second.push_back(descendant);
      }
    }
  }

  // case 4: both statements are not type NONE
  else if (statement1.type != PkbTables::StatementType::NONE &&
           statement2.type != PkbTables::StatementType::NONE) {
    STRING_SET statementLines = invertStatementTypeTable.map[statement2.type];

    for (auto child : statementLines) {
      STRING_SET ancestors = closeParentTable.map[child];

      for (auto ancestor : ancestors) {
        if (statementTypeTable.map[ancestor] == statement1.type) {
          result.first.push_back(ancestor);
          result.second.push_back(child);
        }
      }
    }
  }

  return result;
}

STRING_SET Pkb::parentStar(Statement statement, Underscore underscore) {
  return parent(statement, underscore);
}

bool Pkb::parentStar(Underscore underscore, LineNumber line) {
  return parent(underscore, line);
}

STRING_SET Pkb::parentStar(Underscore underscore, Statement statement) {
  return parent(underscore, statement);
}

bool Pkb::parentStar(Underscore underscore1, Underscore underscore2) {
  return parent(underscore1, underscore2);
}

// Query API for uses

bool Pkb::uses(LineNumber line, String variable) {
  PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  STRING_SET usesLines = invertUsesTable.map[inputVarIndex];
  return usesLines.find(line.number) != usesLines.end();
}

STRING_SET Pkb::uses(LineNumber line, Variable variable) {
  STRING_SET result;
  PkbTables::VAR_TABLE_INDEXES varIndexes = usesTableTransited.map[line.number];

  for (PkbTables::VAR_TABLE_INDEX index : varIndexes) {
    result.insert(invertVarTable.map[index]);
  }
  return result;
}

bool Pkb::uses(LineNumber line, Underscore underscore) {
  return usesTableTransited.map[line.number].size() > 0;
}

STRING_SET Pkb::uses(Statement statement, String variable) {
  PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  if (statement.type == PkbTables::StatementType::NONE) {
    return invertUsesTable.map[inputVarIndex];
  } else {
    STRING_SET usesLines = invertUsesTable.map[inputVarIndex];
    STRING_SET::iterator it = usesLines.begin();

    while (it != usesLines.end()) {
      if (statementTypeTable.map[*it] != statement.type) {
        it = usesLines.erase(it);
      } else {
        it++;
      }
    }
    return usesLines;
  }
}

STRING_PAIRS Pkb::uses(Statement statement, Variable variable) {
  STRING_PAIRS result;

  if (statement.type == PkbTables::StatementType::NONE) {
    for (auto entry : usesTableTransited.map) {
      PkbTables::LINE_NO line = entry.first;

      for (auto varIndex : entry.second) {
        result.first.push_back(line);
        result.second.push_back(invertVarTable.map[varIndex]);
      }
    }
  } else {
    STRING_SET statementLines = invertStatementTypeTable.map[statement.type];

    for (auto line : statementLines) {
      PkbTables::VAR_TABLE_INDEXES varIndexes = usesTableTransited.map[line];

      for (auto index : varIndexes) {
        result.first.push_back(line);
        result.second.push_back(invertVarTable.map[index]);
      }
    }
  }
  return result;
}

STRING_SET Pkb::uses(Statement statement, Underscore underscore) {
  STRING_SET result;

  if (statement.type == PkbTables::StatementType::NONE) {
    for (auto entry : usesTableTransited.map) {
      if (entry.second.size() > 0) {
        result.insert(entry.first);
      }
    }
  } else {
    STRING_SET statementLines = invertStatementTypeTable.map[statement.type];

    for (PkbTables::LINE_NO line : statementLines) {
      if (usesTableTransited.map[line].size() > 0) {
        result.insert(line);
      }
    }
  }
  return result;
}

bool Pkb::uses(String procedure, String variable) {
  PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];
  PkbTables::VAR_TABLE_INDEXES varIndexesUsedInProc =
      usesProcTable.map[inputProcIndex];

  return varIndexesUsedInProc.find(inputVarIndex) != varIndexesUsedInProc.end();
}

STRING_SET Pkb::uses(String procedure, Variable variable) {
  STRING_SET result;
  PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  PkbTables::VAR_TABLE_INDEXES varIndexesUsedInProc =
      usesProcTable.map[inputProcIndex];

  for (PkbTables::VAR_TABLE_INDEX index : varIndexesUsedInProc) {
    result.insert(invertVarTable.map[index]);
  }
  return result;
}

bool Pkb::uses(String procedure, Underscore underscore) {
  PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  return usesProcTable.map[inputProcIndex].size() > 0;
}

STRING_SET Pkb::uses(Procedure procedure, String variable) {
  STRING_SET result;
  PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  // if input variable doesn't exist in varTable
  if (inputVarIndex == PkbTables::VAR_TABLE_INDEX()) {
    return result;
  } else {
    for (auto entry : usesProcTable.map) {
      if (entry.second.find(inputVarIndex) != entry.second.end()) {
        result.insert(invertProcTable.map[entry.first]);
      }
    }
    return result;
  }
}

STRING_PAIRS Pkb::uses(Procedure procedure, Variable variable) {
  STRING_PAIRS result;

  for (auto entry : usesProcTable.map) {
    PkbTables::VAR_TABLE_INDEXES varIndexesInProc = entry.second;

    for (auto index : varIndexesInProc) {
      result.first.push_back(invertProcTable.map[entry.first]);
      result.second.push_back(invertVarTable.map[index]);
    }
  }
  return result;
}

STRING_SET Pkb::uses(Procedure procedure, Underscore underscore) {
  STRING_SET result;

  for (auto entry : usesProcTable.map) {
    if (entry.second.size() > 0) {
      result.insert(invertProcTable.map[entry.first]);
    }
  }
  return result;
}

// Query API for modifies

bool Pkb::modifies(LineNumber line, String variable) {
  PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  STRING_SET modifiesLines = invertModifiesTable.map[inputVarIndex];
  return modifiesLines.find(line.number) != modifiesLines.end();
}

STRING_SET Pkb::modifies(LineNumber line, Variable variable) {
  STRING_SET result;
  PkbTables::VAR_TABLE_INDEXES varIndexes =
      modifiesTableTransited.map[line.number];

  for (PkbTables::VAR_TABLE_INDEX index : varIndexes) {
    result.insert(invertVarTable.map[index]);
  }
  return result;
}

bool Pkb::modifies(LineNumber line, Underscore underscore) {
  return modifiesTableTransited.map[line.number].size() > 0;
}

STRING_SET Pkb::modifies(Statement statement, String variable) {
  PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  if (statement.type == PkbTables::StatementType::NONE) {
    return invertModifiesTable.map[inputVarIndex];
  } else {
    STRING_SET modifiesLines = invertModifiesTable.map[inputVarIndex];
    STRING_SET::iterator it = modifiesLines.begin();

    while (it != modifiesLines.end()) {
      if (statementTypeTable.map[*it] != statement.type) {
        it = modifiesLines.erase(it);
      } else {
        it++;
      }
    }
    return modifiesLines;
  }
}

STRING_PAIRS Pkb::modifies(Statement statement, Variable variable) {
  STRING_PAIRS result;

  if (statement.type == PkbTables::StatementType::NONE) {
    for (auto entry : modifiesTableTransited.map) {
      PkbTables::LINE_NO line = entry.first;

      for (auto varIndex : entry.second) {
        result.first.push_back(line);
        result.second.push_back(invertVarTable.map[varIndex]);
      }
    }
  } else {
    STRING_SET statementLines = invertStatementTypeTable.map[statement.type];

    for (auto line : statementLines) {
      PkbTables::VAR_TABLE_INDEXES varIndexes =
          modifiesTableTransited.map[line];

      for (auto index : varIndexes) {
        result.first.push_back(line);
        result.second.push_back(invertVarTable.map[index]);
      }
    }
  }
  return result;
}

STRING_SET Pkb::modifies(Statement statement, Underscore underscore) {
  STRING_SET result;

  if (statement.type == PkbTables::StatementType::NONE) {
    for (auto entry : modifiesTableTransited.map) {
      if (entry.second.size() > 0) {
        result.insert(entry.first);
      }
    }
  } else {
    STRING_SET statementLines = invertStatementTypeTable.map[statement.type];

    for (PkbTables::LINE_NO line : statementLines) {
      if (modifiesTableTransited.map[line].size() > 0) {
        result.insert(line);
      }
    }
  }
  return result;
}

bool Pkb::modifies(String procedure, String variable) {
  PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];
  PkbTables::VAR_TABLE_INDEXES varIndexesModifiedInProc =
      modifiesProcTable.map[inputProcIndex];

  return varIndexesModifiedInProc.find(inputVarIndex) !=
         varIndexesModifiedInProc.end();
}

STRING_SET Pkb::modifies(String procedure, Variable variable) {
  STRING_SET result;
  PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  PkbTables::VAR_TABLE_INDEXES varIndexesModifiesInProc =
      modifiesProcTable.map[inputProcIndex];

  for (PkbTables::VAR_TABLE_INDEX index : varIndexesModifiesInProc) {
    result.insert(invertVarTable.map[index]);
  }
  return result;
}

bool Pkb::modifies(String procedure, Underscore underscore) {
  PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  return modifiesProcTable.map[inputProcIndex].size() > 0;
}

STRING_SET Pkb::modifies(Procedure procedure, String variable) {
  STRING_SET result;
  PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  // if input variable doesn't exist in varTable
  if (inputVarIndex == PkbTables::VAR_TABLE_INDEX()) {
    return result;
  } else {
    for (auto entry : modifiesProcTable.map) {
      if (entry.second.find(inputVarIndex) != entry.second.end()) {
        result.insert(invertProcTable.map[entry.first]);
      }
    }
    return result;
  }
}

STRING_PAIRS Pkb::modifies(Procedure procedure, Variable variable) {
  STRING_PAIRS result;

  for (auto entry : modifiesProcTable.map) {
    PkbTables::VAR_TABLE_INDEXES varIndexesInProc = entry.second;

    for (auto index : varIndexesInProc) {
      result.first.push_back(invertProcTable.map[entry.first]);
      result.second.push_back(invertVarTable.map[index]);
    }
  }
  return result;
}

STRING_SET Pkb::modifies(Procedure procedure, Underscore underscore) {
  STRING_SET result;

  for (auto entry : modifiesProcTable.map) {
    if (entry.second.size() > 0) {
      result.insert(invertProcTable.map[entry.first]);
    }
  }
  return result;
}
