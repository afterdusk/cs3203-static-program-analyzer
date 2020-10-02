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
  this->prevLineTable = PkbTableTransformers::invert(this->followTable);
  this->childrenTable = PkbTableTransformers::pseudoinvert(this->parentTable);
  this->invertStatementTypeTable =
      PkbTableTransformers::pseudoinvert(this->statementTypeTable);
  this->invertStatementProcTable =
      PkbTableTransformers::pseudoinvert(this->statementProcTable);
  this->invertVarTable = PkbTableTransformers::invert(this->varTable);
  this->invertProcTable = PkbTableTransformers::invert(this->procTable);

  this->closeFollowTable = PkbTableTransformers::close(this->followTable);
  this->closeParentTable = PkbTableTransformers::close(this->parentTable);
  this->closePrevLineTable = PkbTableTransformers::close(prevLineTable);
  this->closeChildrenTable =
      PkbTableTransformers::closeFlatten<PARENT>(childrenTable);

  this->usesTableTransited =
      pkbTableTransformers.transit(this->usesTable, this->usesProcTable);
  this->invertUsesTable = PkbTableTransformers::pseudoinvertFlattenKeys<
      Pkb::LINE_NO, Pkb::VAR_TABLE_INDEX>(usesTableTransited);
  this->modifiesTableTransited = pkbTableTransformers.transit(
      this->modifiesTable, this->modifiesProcTable);
  this->invertModifiesTable = PkbTableTransformers::pseudoinvertFlattenKeys<
      Pkb::LINE_NO, Pkb::VAR_TABLE_INDEX>(modifiesTableTransited);

  this->stmtTableIndexes =
      LINE_SET(statementTypeTable.keys.begin(), statementTypeTable.keys.end());
  this->followTableIndexes =
      LINE_SET(followTable.keys.begin(), followTable.keys.end());
  this->parentTableIndexes =
      LINE_SET(parentTable.keys.begin(), parentTable.keys.end());
  this->prevLineTableIndexes =
      LINE_SET(prevLineTable.keys.begin(), prevLineTable.keys.end());
  this->childrenTableIndexes =
      LINE_SET(childrenTable.keys.begin(), childrenTable.keys.end());

  this->varNamesSet = NAME_SET(varTable.keys.begin(), varTable.keys.end());
  this->procNamesSet = NAME_SET(procTable.keys.begin(), procTable.keys.end());
}

// Query API for pattern matching
LINE_NAME_PAIRS Pkb::match(Statement statement, Variable variable,
                           PatternSpec spec) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_NAME_PAIRS result;

  if (statement.type == PkbTables::StatementType::ASSIGN) {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET assignLines = invertStatementTypeTable.map[statement.type];

      if (spec.type == PatternMatchType::Any) {
        for (auto line : assignLines) {
          if (modifiesTable.map.find(line) != modifiesTable.map.end()) {
            PkbTables::VAR_TABLE_INDEXES varIndexes =
                std::get<PkbTables::VAR_TABLE_INDEXES>(modifiesTable.map[line]);

            // Since line is an assignment statement, there must be only one
            // index in varIndexes.
            PkbTables::VAR_TABLE_INDEX modifiedVarIndex = *varIndexes.begin();

            result.first.push_back(line);
            // map access is not checked here because if a var index exists, its
            // variable name would have been mapped in varTable and thus mapped
            // in invertVarTable
            result.second.push_back(invertVarTable.map[modifiedVarIndex]);
          }
        }
      } else if (spec.type == PatternMatchType::CompleteMatch) {
        for (auto line : assignLines) {
          if (assignAstTable.map.find(line) != assignAstTable.map.end()) {
            PkbTables::AST astOnLine = assignAstTable.map[line];

            if (astOnLine == *spec.value) {
              if (modifiesTable.map.find(line) != modifiesTable.map.end()) {
                PkbTables::VAR_TABLE_INDEXES varIndexes =
                    std::get<PkbTables::VAR_TABLE_INDEXES>(
                        modifiesTable.map[line]);

                // Since line is an assignment statement, there must be only one
                // index in varIndexes.
                PkbTables::VAR_TABLE_INDEX modifiedVarIndex =
                    *varIndexes.begin();

                result.first.push_back(line);
                // map access is not checked here because if a var index exists,
                // its variable name would have been mapped in varTable and thus
                // mapped in invertVarTable
                result.second.push_back(invertVarTable.map[modifiedVarIndex]);
              }
            }
          }
        }
      } else if (spec.type == PatternMatchType::SubTreeMatch) {
        for (auto line : assignLines) {
          if (assignAstTable.map.find(line) != assignAstTable.map.end()) {
            PkbTables::AST astOnLine = assignAstTable.map[line];

            if (astOnLine >= *spec.value) {
              if (modifiesTable.map.find(line) != modifiesTable.map.end()) {
                PkbTables::VAR_TABLE_INDEXES varIndexes =
                    std::get<PkbTables::VAR_TABLE_INDEXES>(
                        modifiesTable.map[line]);

                // Since line is an assignment statement, there must be only one
                // index in varIndexes.
                PkbTables::VAR_TABLE_INDEX modifiedVarIndex =
                    *varIndexes.begin();

                result.first.push_back(line);
                // map access is not checked here because if a var index exists,
                // its variable name would have been mapped in varTable and thus
                // mapped in invertVarTable
                result.second.push_back(invertVarTable.map[modifiedVarIndex]);
              }
            }
          }
        }
      } else {
        throw "Error PatternSpec type is not assigned";
      }
    }
  }
  return result;
}

LINE_SET Pkb::match(Statement statement, Underscore underscore,
                    PatternSpec spec) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_SET result;

  if (statement.type == PkbTables::StatementType::ASSIGN) {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET assignLines = invertStatementTypeTable.map[statement.type];

      if (spec.type == PatternMatchType::Any) {
        // if no matching is required, then return assignment lines without
        // processing.
        return assignLines;
      } else {
        for (auto line : assignLines) {
          if (assignAstTable.map.find(line) != assignAstTable.map.end()) {
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
              throw "Error PatternSpec type is not assigned";
            }
          }
        }
      }
    }
  }

  return result;
}

LINE_SET Pkb::match(Statement statement, String variable, PatternSpec spec) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_SET result;

  if (statement.type == PkbTables::StatementType::ASSIGN) {
    if (varTable.map.find(variable.name) != varTable.map.end()) {
      PkbTables::VAR_TABLE_INDEX inputVarTableIndex =
          varTable.map[variable.name];

      if (invertModifiesTable.map.find(inputVarTableIndex) !=
          invertModifiesTable.map.end()) {
        LINE_SET linesVarModifies = invertModifiesTable.map[inputVarTableIndex];

        for (auto line : linesVarModifies) {
          // map access is not checked here because if a line modified by var
          // exists, its type would have been mapped in statementTypeTable
          if (statementTypeTable.map[line] ==
              PkbTables::StatementType::ASSIGN) {
            if (assignAstTable.map.find(line) != assignAstTable.map.end()) {
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
      }
    }
  }

  return result;
}

// Query API for normal select
NAME_SET Pkb::select(Variable var) { return varNamesSet; }

LINE_SET Pkb::select(Statement statement) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (statement.type == PkbTables::StatementType::NONE) {
    return stmtTableIndexes;
  } else {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      return invertStatementTypeTable.map[statement.type];
    } else {
      return LINE_SET();
    }
  }
}

NAME_SET Pkb::select(Procedure proc) { return procNamesSet; }

PkbTables::CONSTANT_TABLE Pkb::select(Constant constant) {
  return constantTable;
}

// Query API for follows

bool Pkb::follows(LineNumber line1, LineNumber line2) {
  if (followTable.map.find(line1.number) != followTable.map.end()) {
    return followTable.map[line1.number] == line2.number;
  }
  return false;
}

LINE_SET Pkb::follows(LineNumber line, Statement statement) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (followTable.map.find(line.number) != followTable.map.end()) {
    PkbTables::FOLLOW followLine = followTable.map[line.number];

    if (statement.type == PkbTables::StatementType::NONE) {
      return followLine != PkbTables::LINE_NO() ? LINE_SET{followLine}
                                                : LINE_SET();
    } else {
      // map access is not checked here because if a follow line exists, its
      // type would have been mapped in statementTypeTable
      PkbTables::StatementType type = statementTypeTable.map[followLine];
      return followLine != PkbTables::LINE_NO() && statement.type == type
                 ? LINE_SET{followLine}
                 : LINE_SET();
    }
  }
  return LINE_SET();
}

bool Pkb::follows(LineNumber line, Underscore underscore) {
  if (followTable.map.find(line.number) != followTable.map.end()) {
    return followTable.map[line.number] != PkbTables::LINE_NO();
  }
  return false;
}

LINE_SET Pkb::follows(Statement statement, LineNumber line) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (prevLineTable.map.find(line.number) != prevLineTable.map.end()) {
    PkbTables::LINE_NO prevLine = prevLineTable.map[line.number];

    if (statement.type == PkbTables::StatementType::NONE) {
      return prevLine != PkbTables::LINE_NO() ? LINE_SET{prevLine} : LINE_SET();
    } else {
      // map access is not checked here because if a prev line exists, its
      // type would have been mapped in statementTypeTable
      PkbTables::StatementType type = statementTypeTable.map[prevLine];
      return prevLine != PkbTables::LINE_NO() && statement.type == type
                 ? LINE_SET{prevLine}
                 : LINE_SET();
    }
  }
  return LINE_SET();
}

LINE_LINE_PAIRS Pkb::follows(Statement statement1, Statement statement2) {
  if (statement1.type == PkbTables::StatementType::NOTSET ||
      statement2.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_LINE_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == PkbTables::StatementType::NONE &&
      statement2.type == PkbTables::StatementType::NONE) {
    result.first = followTable.keys;
    result.second = prevLineTable.keys;
  }

  // case 2: only statement1 is of type NONE
  else if (statement1.type == PkbTables::StatementType::NONE) {
    if (invertStatementTypeTable.map.find(statement2.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers = invertStatementTypeTable.map[statement2.type];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (prevLineTable.map.find(*it) != prevLineTable.map.end()) {
          PkbTables::LINE_NO prevLine = prevLineTable.map[*it];
          if (prevLine != PkbTables::LINE_NO()) {
            result.first.push_back(prevLine);
            result.second.push_back(*it);
          }
        }
        it++;
      }
    }
  }

  // case 3: only statement2 is of type NONE
  else if (statement2.type == PkbTables::StatementType::NONE) {
    if (invertStatementTypeTable.map.find(statement1.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers = invertStatementTypeTable.map[statement1.type];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (followTable.map.find(*it) != followTable.map.end()) {
          PkbTables::FOLLOW followLine = followTable.map[*it];
          if (followLine != PkbTables::LINE_NO()) {
            result.first.push_back(*it);
            result.second.push_back(followLine);
          }
        }
        it++;
      }
    }
  }

  // case 4: both statements are not of type NONE
  else if (statement1.type != PkbTables::StatementType::NONE &&
           statement2.type != PkbTables::StatementType::NONE) {
    if (invertStatementTypeTable.map.find(statement1.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers = invertStatementTypeTable.map[statement1.type];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (followTable.map.find(*it) != followTable.map.end()) {
          PkbTables::FOLLOW followLine = followTable.map[*it];
          // map access is not checked here because if a follow line exists, its
          // type would have been mapped in statementTypeTable
          PkbTables::StatementType type = statementTypeTable.map[followLine];
          if (followLine != PkbTables::LINE_NO() && type == statement2.type) {
            result.first.push_back(*it);
            result.second.push_back(followLine);
          }
        }
        it++;
      }
    }
  }

  return result;
}

LINE_SET Pkb::follows(Statement statement, Underscore underscore) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (statement.type == PkbTables::StatementType::NONE) {
    return followTableIndexes;
  } else {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers = invertStatementTypeTable.map[statement.type];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (followTable.map.find(*it) != followTable.map.end()) {
          if (followTable.map[*it] == PkbTables::LINE_NO()) {
            it = lineNumbers.erase(it);
          } else {
            it++;
          }
        } else {
          it = lineNumbers.erase(it);
        }
      }

      return lineNumbers;
    }
  }
  return LINE_SET();
}

bool Pkb::follows(Underscore underscore, LineNumber line) {
  if (prevLineTable.map.find(line.number) != prevLineTable.map.end()) {
    return prevLineTable.map[line.number] != PkbTables::LINE_NO();
  }
  return false;
}

LINE_SET Pkb::follows(Underscore underscore, Statement statement) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (statement.type == PkbTables::StatementType::NONE) {
    return prevLineTableIndexes;
  } else {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers = invertStatementTypeTable.map[statement.type];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (prevLineTable.map.find(*it) != prevLineTable.map.end()) {
          if (prevLineTable.map[*it] == PkbTables::LINE_NO()) {
            it = lineNumbers.erase(it);
          } else {
            it++;
          }
        } else {
          it = lineNumbers.erase(it);
        }
      }
      return lineNumbers;
    }
  }
  return LINE_SET();
}

bool Pkb::follows(Underscore underscore1, Underscore underscore2) {
  return followTable.size() > 0;
}

// Query API for followsStar

bool Pkb::followsStar(LineNumber line1, LineNumber line2) {
  if (closeFollowTable.map.find(line1.number) != closeFollowTable.map.end()) {
    PkbTables::FOLLOWS followLines = closeFollowTable.map[line1.number];
    return followLines.find(line2.number) != followLines.end();
  }
  return false;
}

LINE_SET Pkb::followsStar(LineNumber line, Statement statement) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (closeFollowTable.map.find(line.number) != closeFollowTable.map.end()) {
    LINE_SET followLines = closeFollowTable.map[line.number];

    if (statement.type == PkbTables::StatementType::NONE) {
      return followLines;
    } else {
      LINE_SET::iterator it = followLines.begin();

      while (it != followLines.end()) {
        // map access is not checked here because if a follow line exists, its
        // type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type) {
          it = followLines.erase(it);
        } else {
          it++;
        }
      }
      return followLines;
    }
  }
  return LINE_SET();
}

bool Pkb::followsStar(LineNumber line, Underscore underscore) {
  return follows(line, underscore);
}

LINE_SET Pkb::followsStar(Statement statement, LineNumber line) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (closePrevLineTable.map.find(line.number) !=
      closePrevLineTable.map.end()) {
    LINE_SET prevLines = closePrevLineTable.map[line.number];

    if (statement.type == PkbTables::StatementType::NONE) {
      return prevLines;
    } else {
      LINE_SET::iterator it = prevLines.begin();

      while (it != prevLines.end()) {
        // map access is not checked here because if a prev line exists, its
        // type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type) {
          it = prevLines.erase(it);
        } else {
          it++;
        }
      }
      return prevLines;
    }
  }
  return LINE_SET();
}

LINE_LINE_PAIRS Pkb::followsStar(Statement statement1, Statement statement2) {
  if (statement1.type == PkbTables::StatementType::NOTSET ||
      statement2.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_LINE_PAIRS result;

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
    if (invertStatementTypeTable.map.find(statement2.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement2.type];

      for (auto line : statementLines) {
        if (closePrevLineTable.map.find(line) != closePrevLineTable.map.end()) {
          LINE_SET prevLines = closePrevLineTable.map[line];

          for (auto prev : prevLines) {
            result.first.push_back(prev);
            result.second.push_back(line);
          }
        }
      }
    }
  }

  // case 3: only statement2 is of type NONE
  else if (statement2.type == PkbTables::StatementType::NONE) {
    if (invertStatementTypeTable.map.find(statement1.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement1.type];

      for (auto line : statementLines) {
        if (closeFollowTable.map.find(line) != closeFollowTable.map.end()) {
          LINE_SET followLines = closeFollowTable.map[line];

          for (auto follow : followLines) {
            result.first.push_back(line);
            result.second.push_back(follow);
          }
        }
      }
    }
  }

  // case 4: both statements are not of type NONE
  else if (statement1.type != PkbTables::StatementType::NONE &&
           statement2.type != PkbTables::StatementType::NONE) {
    if (invertStatementTypeTable.map.find(statement1.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement1.type];

      for (auto line : statementLines) {
        if (closeFollowTable.map.find(line) != closeFollowTable.map.end()) {
          LINE_SET followLines = closeFollowTable.map[line];

          for (auto follow : followLines) {
            // map access is not checked here because if a follow line exists,
            // its type would have been mapped in statementTypeTable
            if (statementTypeTable.map[follow] == statement2.type) {
              result.first.push_back(line);
              result.second.push_back(follow);
            }
          }
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::followsStar(Statement statement, Underscore underscore) {
  return follows(statement, underscore);
}

bool Pkb::followsStar(Underscore underscore, LineNumber line) {
  return follows(underscore, line);
}

LINE_SET Pkb::followsStar(Underscore underscore, Statement statement) {
  return follows(underscore, statement);
}

bool Pkb::followsStar(Underscore underscore1, Underscore underscore2) {
  return follows(underscore1, underscore2);
}

// Query API for parent

bool Pkb::parent(LineNumber line1, LineNumber line2) {
  if (parentTable.map.find(line2.number) != parentTable.map.end()) {
    return parentTable.map[line2.number] == line1.number;
  }
  return false;
}

LINE_SET Pkb::parent(LineNumber line, Statement statement) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (childrenTable.map.find(line.number) != childrenTable.map.end()) {
    LINE_SET children = childrenTable.map[line.number];

    if (statement.type == PkbTables::StatementType::NONE) {
      return children;
    } else {
      LINE_SET::iterator it = children.begin();

      while (it != children.end()) {
        // map access is not checked here because if a child line exists, its
        // type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type) {
          it = children.erase(it);
        } else {
          it++;
        }
      }
      return children;
    }
  }
  return LINE_SET();
}

bool Pkb::parent(LineNumber line, Underscore underscore) {
  if (childrenTable.map.find(line.number) != childrenTable.map.end()) {
    return childrenTable.map[line.number].size() > 0;
  }
  return false;
}

LINE_SET Pkb::parent(Statement statement, LineNumber line) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (parentTable.map.find(line.number) != parentTable.map.end()) {
    PkbTables::PARENT parent = parentTable.map[line.number];
    if (statement.type == PkbTables::StatementType::NONE) {
      return parent != PkbTables::LINE_NO() ? LINE_SET{parent} : LINE_SET();
    } else {
      // map access is not checked here because if the parent line exists, its
      // type would have been mapped in statementTypeTable
      PkbTables::StatementType type = statementTypeTable.map[parent];
      return parent != PkbTables::LINE_NO() && type == statement.type
                 ? LINE_SET{parent}
                 : LINE_SET();
    }
  }
  return LINE_SET();
}

LINE_LINE_PAIRS Pkb::parent(Statement statement1, Statement statement2) {
  if (statement1.type == PkbTables::StatementType::NOTSET ||
      statement2.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_LINE_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == PkbTables::StatementType::NONE &&
      statement2.type == PkbTables::StatementType::NONE) {
    LINE_SET parents = childrenTableIndexes;
    LINE_SET::iterator parentIt = parents.begin();

    while (parentIt != parents.end()) {
      // map access is not checked here because childrenTableIndexes contains
      // the keys of childrenTable which ensures that the key already exists
      // before accessing
      PkbTables::CHILDREN children = childrenTable.map[*parentIt];
      LINE_SET::iterator childIt = children.begin();

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
    if (invertStatementTypeTable.map.find(statement2.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers = invertStatementTypeTable.map[statement2.type];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (parentTable.map.find(*it) != parentTable.map.end()) {
          PkbTables::PARENT parent = parentTable.map[*it];
          if (parent != PkbTables::LINE_NO()) {
            result.first.push_back(parent);
            result.second.push_back(*it);
          }
        }
        it++;
      }
    }
  }

  // case 3: only statement2 is of type NONE
  else if (statement2.type == PkbTables::StatementType::NONE) {
    LINE_SET parents = childrenTableIndexes;
    LINE_SET::iterator parentIt = parents.begin();

    while (parentIt != parents.end()) {
      if (statementTypeTable.map[*parentIt] == statement1.type) {
        // map access is not checked here because childrenTableIndexes contains
        // the keys of childrenTable which ensures that the key already exists
        // before accessing
        LINE_SET children = childrenTable.map[*parentIt];
        LINE_SET::iterator childIt = children.begin();

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
    LINE_SET parents = childrenTableIndexes;
    LINE_SET::iterator parentIt = parents.begin();

    while (parentIt != parents.end()) {
      if (statementTypeTable.map[*parentIt] == statement1.type) {
        // map access is not checked here because childrenTableIndexes contains
        // the keys of childrenTable which ensures that the key already exists
        // before accessing
        LINE_SET children = childrenTable.map[*parentIt];
        LINE_SET::iterator childIt = children.begin();

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

LINE_SET Pkb::parent(Statement statement, Underscore underscore) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (statement.type == PkbTables::StatementType::NONE) {
    return childrenTableIndexes;
  } else {
    LINE_SET parents = childrenTableIndexes;
    LINE_SET::iterator it = parents.begin();

    while (it != parents.end()) {
      // map access is not checked here because if a parent line exists, its
      // type would have been mapped in statementTypeTable
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
  if (parentTable.map.find(line.number) != parentTable.map.end()) {
    PkbTables::PARENT parent = parentTable.map[line.number];
    return parent != PkbTables::LINE_NO();
  }
  return false;
}

LINE_SET Pkb::parent(Underscore underscore, Statement statement) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (statement.type == PkbTables::StatementType::NONE) {
    return parentTableIndexes;
  } else {
    LINE_SET children = parentTableIndexes;
    LINE_SET::iterator it = children.begin();

    while (it != children.end()) {
      // map access is not checked here because if a child line exists, its
      // type would have been mapped in statementTypeTable
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
  if (closeParentTable.map.find(line2.number) != closeParentTable.map.end()) {
    PkbTables::PARENTS parentLines = closeParentTable.map[line2.number];
    return parentLines.find(line1.number) != parentLines.end();
  }
  return false;
}

LINE_SET Pkb::parentStar(LineNumber line, Statement statement) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (closeChildrenTable.map.find(line.number) !=
      closeChildrenTable.map.end()) {
    LINE_SET descendantLines = closeChildrenTable.map[line.number];

    if (statement.type == PkbTables::StatementType::NONE) {
      return descendantLines;
    } else {
      LINE_SET::iterator it = descendantLines.begin();
      while (it != descendantLines.end()) {
        // map access is not checked here because if a descendant line exists,
        // its type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type) {
          it = descendantLines.erase(it);
        } else {
          it++;
        }
      }
      return descendantLines;
    }
  }
  return LINE_SET();
}

bool Pkb::parentStar(LineNumber line, Underscore underscore) {
  return parent(line, underscore);
}

LINE_SET Pkb::parentStar(Statement statement, LineNumber line) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (closeParentTable.map.find(line.number) != closeParentTable.map.end()) {
    PkbTables::PARENTS parentLines = closeParentTable.map[line.number];

    if (statement.type == PkbTables::StatementType::NONE) {
      return parentLines;
    } else {
      PkbTables::PARENTS::iterator it = parentLines.begin();
      while (it != parentLines.end()) {
        // map access is not checked here because if a parent line exists, its
        // type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type) {
          it = parentLines.erase(it);
        } else {
          it++;
        }
      }
      return parentLines;
    }
  }
  return LINE_SET();
}

LINE_LINE_PAIRS Pkb::parentStar(Statement statement1, Statement statement2) {
  if (statement1.type == PkbTables::StatementType::NOTSET ||
      statement2.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_LINE_PAIRS result;

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
    if (invertStatementTypeTable.map.find(statement2.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement2.type];

      for (auto child : statementLines) {
        if (closeParentTable.map.find(child) != closeParentTable.map.end()) {
          PkbTables::PARENTS ancestors = closeParentTable.map[child];

          for (auto ancestor : ancestors) {
            result.first.push_back(ancestor);
            result.second.push_back(child);
          }
        }
      }
    }
  }

  // case 3: only statement2 is of type NONE
  else if (statement2.type == PkbTables::StatementType::NONE) {
    if (invertStatementTypeTable.map.find(statement1.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement1.type];

      for (auto parent : statementLines) {
        if (closeChildrenTable.map.find(parent) !=
            closeChildrenTable.map.end()) {
          LINE_SET descendants = closeChildrenTable.map[parent];

          for (auto descendant : descendants) {
            result.first.push_back(parent);
            result.second.push_back(descendant);
          }
        }
      }
    }
  }

  // case 4: both statements are not type NONE
  else if (statement1.type != PkbTables::StatementType::NONE &&
           statement2.type != PkbTables::StatementType::NONE) {
    if (invertStatementTypeTable.map.find(statement2.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement2.type];

      for (auto child : statementLines) {
        if (closeParentTable.map.find(child) != closeParentTable.map.end()) {
          LINE_SET ancestors = closeParentTable.map[child];

          for (auto ancestor : ancestors) {
            if (statementTypeTable.map[ancestor] == statement1.type) {
              result.first.push_back(ancestor);
              result.second.push_back(child);
            }
          }
        }
      }
    }
  }

  return result;
}

LINE_SET Pkb::parentStar(Statement statement, Underscore underscore) {
  return parent(statement, underscore);
}

bool Pkb::parentStar(Underscore underscore, LineNumber line) {
  return parent(underscore, line);
}

LINE_SET Pkb::parentStar(Underscore underscore, Statement statement) {
  return parent(underscore, statement);
}

bool Pkb::parentStar(Underscore underscore1, Underscore underscore2) {
  return parent(underscore1, underscore2);
}

// Query API for uses

bool Pkb::uses(LineNumber line, String variable) {
  if (varTable.map.find(variable.name) != varTable.map.end()) {
    PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

    if (invertUsesTable.map.find(inputVarIndex) != invertUsesTable.map.end()) {
      LINE_SET usesLines = invertUsesTable.map[inputVarIndex];
      return usesLines.find(line.number) != usesLines.end();
    }
  }
  return false;
}

NAME_SET Pkb::uses(LineNumber line, Variable variable) {
  NAME_SET result;
  if (usesTableTransited.map.find(line.number) !=
      usesTableTransited.map.end()) {
    PkbTables::VAR_TABLE_INDEXES varIndexes =
        usesTableTransited.map[line.number];

    for (PkbTables::VAR_TABLE_INDEX index : varIndexes) {
      // map access is not checked here because if a var index exists, its
      // variable name would have been mapped in varTable and thus mapped in
      // invertVarTable
      result.insert(invertVarTable.map[index]);
    }
  }
  return result;
}

bool Pkb::uses(LineNumber line, Underscore underscore) {
  if (usesTableTransited.map.find(line.number) !=
      usesTableTransited.map.end()) {
    return usesTableTransited.map[line.number].size() > 0;
  }
  return false;
}

LINE_SET Pkb::uses(Statement statement, String variable) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (varTable.map.find(variable.name) != varTable.map.end()) {
    PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];
    if (invertUsesTable.map.find(inputVarIndex) != invertUsesTable.map.end()) {
      if (statement.type == PkbTables::StatementType::NONE) {
        return invertUsesTable.map[inputVarIndex];
      } else {
        LINE_SET usesLines = invertUsesTable.map[inputVarIndex];
        LINE_SET::iterator it = usesLines.begin();

        while (it != usesLines.end()) {
          // map access is not checked here because if a line that uses variable
          // exists, its type would have been mapped in statementTypeTable
          if (statementTypeTable.map[*it] != statement.type) {
            it = usesLines.erase(it);
          } else {
            it++;
          }
        }
        return usesLines;
      }
    }
  }
  return LINE_SET();
}

LINE_NAME_PAIRS Pkb::uses(Statement statement, Variable variable) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_NAME_PAIRS result;

  if (statement.type == PkbTables::StatementType::NONE) {
    for (auto entry : usesTableTransited.map) {
      PkbTables::LINE_NO line = entry.first;

      for (auto varIndex : entry.second) {
        result.first.push_back(line);
        // map access is not checked here because if a var index exists, its
        // variable name would have been mapped in varTable and thus mapped in
        // invertVarTable
        result.second.push_back(invertVarTable.map[varIndex]);
      }
    }
  } else {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement.type];

      for (auto line : statementLines) {
        if (usesTableTransited.map.find(line) != usesTableTransited.map.end()) {
          PkbTables::VAR_TABLE_INDEXES varIndexes =
              usesTableTransited.map[line];

          for (auto index : varIndexes) {
            result.first.push_back(line);
            // map access is not checked here because if a var index exists, its
            // variable name would have been mapped in varTable and thus mapped
            // in invertVarTable
            result.second.push_back(invertVarTable.map[index]);
          }
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::uses(Statement statement, Underscore underscore) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_SET result;

  if (statement.type == PkbTables::StatementType::NONE) {
    for (auto entry : usesTableTransited.map) {
      if (entry.second.size() > 0) {
        result.insert(entry.first);
      }
    }
  } else {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement.type];

      for (PkbTables::LINE_NO line : statementLines) {
        if (usesTableTransited.map.find(line) != usesTableTransited.map.end()) {
          if (usesTableTransited.map[line].size() > 0) {
            result.insert(line);
          }
        }
      }
    }
  }
  return result;
}

bool Pkb::uses(String procedure, String variable) {
  if (procTable.map.find(procedure.name) != procTable.map.end() &&
      varTable.map.find(variable.name) != varTable.map.end()) {
    PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
    PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

    if (usesProcTable.map.find(inputProcIndex) != usesProcTable.map.end()) {
      PkbTables::VAR_TABLE_INDEXES varIndexesUsedInProc =
          usesProcTable.map[inputProcIndex];
      return varIndexesUsedInProc.find(inputVarIndex) !=
             varIndexesUsedInProc.end();
    }
  }
  return false;
}

NAME_SET Pkb::uses(String procedure, Variable variable) {
  NAME_SET result;

  if (procTable.map.find(procedure.name) != procTable.map.end()) {
    PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];

    if (usesProcTable.map.find(inputProcIndex) != usesProcTable.map.end()) {
      PkbTables::VAR_TABLE_INDEXES varIndexesUsesInProc =
          usesProcTable.map[inputProcIndex];
      for (PkbTables::VAR_TABLE_INDEX index : varIndexesUsesInProc) {
        // map access is not checked here because if a var index exists, its
        // variable name would have been mapped in varTable and thus mapped in
        // invertVarTable
        result.insert(invertVarTable.map[index]);
      }
    }
  }
  return result;
}

bool Pkb::uses(String procedure, Underscore underscore) {
  if (procTable.map.find(procedure.name) != procTable.map.end()) {
    PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];

    if (usesProcTable.map.find(inputProcIndex) != usesProcTable.map.end()) {
      return usesProcTable.map[inputProcIndex].size() > 0;
    }
  }
  return false;
}

NAME_SET Pkb::uses(Procedure procedure, String variable) {
  NAME_SET result;

  if (varTable.map.find(variable.name) != varTable.map.end()) {
    PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];
    for (auto entry : usesProcTable.map) {
      if (entry.second.find(inputVarIndex) != entry.second.end()) {
        // map access is not checked here because if a proc index (entry.first
        // value) exists, its procedure name would have been mapped in procTable
        // and thus mapped in invertProcTable
        result.insert(invertProcTable.map[entry.first]);
      }
    }
  }
  return result;
}

NAME_NAME_PAIRS Pkb::uses(Procedure procedure, Variable variable) {
  NAME_NAME_PAIRS result;

  for (auto entry : usesProcTable.map) {
    PkbTables::VAR_TABLE_INDEXES varIndexesInProc = entry.second;

    for (auto index : varIndexesInProc) {
      // map access is not checked here because if a var index exists, its
      // variable name would have been mapped in varTable and thus mapped in
      // invertVarTable, same reason for invertProcTable map access
      result.first.push_back(invertProcTable.map[entry.first]);
      result.second.push_back(invertVarTable.map[index]);
    }
  }
  return result;
}

NAME_SET Pkb::uses(Procedure procedure, Underscore underscore) {
  NAME_SET result;

  for (auto entry : usesProcTable.map) {
    if (entry.second.size() > 0) {
      // map access is not checked here because if a proc index (entry.first
      // value) exists, its procedure name would have been mapped in procTable
      // and thus mapped in invertProcTable
      result.insert(invertProcTable.map[entry.first]);
    }
  }
  return result;
}

// Query API for modifies

bool Pkb::modifies(LineNumber line, String variable) {
  if (varTable.map.find(variable.name) != varTable.map.end()) {
    PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

    if (invertModifiesTable.map.find(inputVarIndex) !=
        invertModifiesTable.map.end()) {
      LINE_SET modifiesLines = invertModifiesTable.map[inputVarIndex];
      return modifiesLines.find(line.number) != modifiesLines.end();
    }
  }
  return false;
}

NAME_SET Pkb::modifies(LineNumber line, Variable variable) {
  NAME_SET result;
  if (modifiesTableTransited.map.find(line.number) !=
      modifiesTableTransited.map.end()) {
    PkbTables::VAR_TABLE_INDEXES varIndexes =
        modifiesTableTransited.map[line.number];

    for (PkbTables::VAR_TABLE_INDEX index : varIndexes) {
      // map access is not checked here because if a var index exists, its
      // variable name would have been mapped in varTable and thus mapped in
      // invertVarTable
      result.insert(invertVarTable.map[index]);
    }
  }
  return result;
}

bool Pkb::modifies(LineNumber line, Underscore underscore) {
  if (modifiesTableTransited.map.find(line.number) !=
      modifiesTableTransited.map.end()) {
    return modifiesTableTransited.map[line.number].size() > 0;
  }
  return false;
}

LINE_SET Pkb::modifies(Statement statement, String variable) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  if (varTable.map.find(variable.name) != varTable.map.end()) {
    PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];
    if (invertModifiesTable.map.find(inputVarIndex) !=
        invertModifiesTable.map.end()) {
      if (statement.type == PkbTables::StatementType::NONE) {
        return invertModifiesTable.map[inputVarIndex];
      } else {
        LINE_SET modifiesLines = invertModifiesTable.map[inputVarIndex];
        LINE_SET::iterator it = modifiesLines.begin();

        while (it != modifiesLines.end()) {
          // map access is not checked here because if a line that modifies
          // variable exists, its type would have been mapped in
          // statementTypeTable
          if (statementTypeTable.map[*it] != statement.type) {
            it = modifiesLines.erase(it);
          } else {
            it++;
          }
        }
        return modifiesLines;
      }
    }
  }
  return LINE_SET();
}

LINE_NAME_PAIRS Pkb::modifies(Statement statement, Variable variable) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_NAME_PAIRS result;

  if (statement.type == PkbTables::StatementType::NONE) {
    for (auto entry : modifiesTableTransited.map) {
      PkbTables::LINE_NO line = entry.first;

      for (auto varIndex : entry.second) {
        result.first.push_back(line);
        // map access is not checked here because if a var index exists, its
        // variable name would have been mapped in varTable and thus mapped in
        // invertVarTable
        result.second.push_back(invertVarTable.map[varIndex]);
      }
    }
  } else {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement.type];

      for (auto line : statementLines) {
        if (modifiesTableTransited.map.find(line) !=
            modifiesTableTransited.map.end()) {
          PkbTables::VAR_TABLE_INDEXES varIndexes =
              modifiesTableTransited.map[line];

          for (auto index : varIndexes) {
            result.first.push_back(line);
            // map access is not checked here because if a var index exists, its
            // variable name would have been mapped in varTable and thus mapped
            // in invertVarTable
            result.second.push_back(invertVarTable.map[index]);
          }
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::modifies(Statement statement, Underscore underscore) {
  if (statement.type == PkbTables::StatementType::NOTSET) {
    throw "Error statement type is not assigned";
  }

  LINE_SET result;

  if (statement.type == PkbTables::StatementType::NONE) {
    for (auto entry : modifiesTableTransited.map) {
      if (entry.second.size() > 0) {
        result.insert(entry.first);
      }
    }
  } else {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement.type];

      for (PkbTables::LINE_NO line : statementLines) {
        if (modifiesTableTransited.map.find(line) !=
            modifiesTableTransited.map.end()) {
          if (modifiesTableTransited.map[line].size() > 0) {
            result.insert(line);
          }
        }
      }
    }
  }
  return result;
}

bool Pkb::modifies(String procedure, String variable) {
  if (procTable.map.find(procedure.name) != procTable.map.end() &&
      varTable.map.find(variable.name) != varTable.map.end()) {
    PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
    PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

    if (modifiesProcTable.map.find(inputProcIndex) !=
        modifiesProcTable.map.end()) {
      PkbTables::VAR_TABLE_INDEXES varIndexesModifiedInProc =
          modifiesProcTable.map[inputProcIndex];
      return varIndexesModifiedInProc.find(inputVarIndex) !=
             varIndexesModifiedInProc.end();
    }
  }
  return false;
}

NAME_SET Pkb::modifies(String procedure, Variable variable) {
  NAME_SET result;

  if (procTable.map.find(procedure.name) != procTable.map.end()) {
    PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];

    if (modifiesProcTable.map.find(inputProcIndex) !=
        modifiesProcTable.map.end()) {
      PkbTables::VAR_TABLE_INDEXES varIndexesModifiesInProc =
          modifiesProcTable.map[inputProcIndex];
      for (PkbTables::VAR_TABLE_INDEX index : varIndexesModifiesInProc) {
        // map access is not checked here because if a var index exists, its
        // variable name would have been mapped in varTable and thus mapped in
        // invertVarTable
        result.insert(invertVarTable.map[index]);
      }
    }
  }
  return result;
}

bool Pkb::modifies(String procedure, Underscore underscore) {
  if (procTable.map.find(procedure.name) != procTable.map.end()) {
    PkbTables::PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];

    if (modifiesProcTable.map.find(inputProcIndex) !=
        modifiesProcTable.map.end()) {
      return modifiesProcTable.map[inputProcIndex].size() > 0;
    }
  }
  return false;
}

NAME_SET Pkb::modifies(Procedure procedure, String variable) {
  NAME_SET result;

  if (varTable.map.find(variable.name) != varTable.map.end()) {
    PkbTables::VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];
    for (auto entry : modifiesProcTable.map) {
      if (entry.second.find(inputVarIndex) != entry.second.end()) {
        // map access is not checked here because if a proc index (entry.first
        // value) exists, its procedure name would have been mapped in procTable
        // and thus mapped in invertProcTable
        result.insert(invertProcTable.map[entry.first]);
      }
    }
  }
  return result;
}

NAME_NAME_PAIRS Pkb::modifies(Procedure procedure, Variable variable) {
  NAME_NAME_PAIRS result;

  for (auto entry : modifiesProcTable.map) {
    PkbTables::VAR_TABLE_INDEXES varIndexesInProc = entry.second;

    for (auto index : varIndexesInProc) {
      // map access is not checked here because if a var index exists, its
      // variable name would have been mapped in varTable and thus mapped in
      // invertVarTable, same reason for invertProcTable map access
      result.first.push_back(invertProcTable.map[entry.first]);
      result.second.push_back(invertVarTable.map[index]);
    }
  }
  return result;
}

NAME_SET Pkb::modifies(Procedure procedure, Underscore underscore) {
  NAME_SET result;

  for (auto entry : modifiesProcTable.map) {
    if (entry.second.size() > 0) {
      // map access is not checked here because if a proc index (entry.first
      // value) exists, its procedure name would have been mapped in procTable
      // and thus mapped in invertProcTable
      result.insert(invertProcTable.map[entry.first]);
    }
  }
  return result;
}
