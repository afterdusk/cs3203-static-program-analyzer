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

const Pkb::CONDITION_VARS_TABLE &Pkb::getConditionVarsTable() const {
  return this->conditionVarsTable;
}

const Pkb::CONSTANT_TABLE &Pkb::getConstantTable() const {
  return this->constantTable;
}

const Pkb::CALLS_TABLE &Pkb::getCallsTable() const { return this->callsTable; }

void Pkb::addVar(VAR var) { this->varTable.insert(var); }

void Pkb::addProc(PROC proc) { this->procTable.insert(proc); }

void Pkb::addUses(LINE_NO lineNo, USES uses) {
  this->usesTable.insert({lineNo, uses});
}

void Pkb::addUsesProc(PROC proc, VARS vars) {
  this->usesProcTable.insert({proc, vars});
}

void Pkb::addModifies(LINE_NO lineNo, MODIFIES modifies) {
  this->modifiesTable.insert({lineNo, modifies});
}

void Pkb::addModifiesProc(PROC proc, VARS vars) {
  this->modifiesProcTable.insert({proc, vars});
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

void Pkb::addConditionVars(LINE_NO lineNo, VARS vars) {
  this->conditionVarsTable.insert({lineNo, vars});
}

void Pkb::addConstant(CONSTANT constant) {
  this->constantTable.insert(constant);
}

void Pkb::addCall(PROC proc, CALL call) {
  if (!this->callsTable.insert({proc, {call}})) {
    // if `proc` mapped, then insert `call` into existing mapped unordered_set.
    this->callsTable.map[proc].insert(call);
  }
}

void Pkb::deriveTables() {
  this->prevLineTable = PkbTableTransformers::invert(this->followTable);
  this->childrenTable = PkbTableTransformers::pseudoinvert(this->parentTable);
  this->invertStatementTypeTable =
      PkbTableTransformers::pseudoinvert(this->statementTypeTable);
  this->invertStatementProcTable =
      PkbTableTransformers::pseudoinvert(this->statementProcTable);
  this->invertCallsTable =
      PkbTableTransformers::pseudoinvertFlattenKeys<Pkb::PROC, Pkb::CALL>(
          this->callsTable);

  this->closeFollowTable = PkbTableTransformers::close(this->followTable);
  this->closeParentTable = PkbTableTransformers::close(this->parentTable);
  this->closePrevLineTable = PkbTableTransformers::close(prevLineTable);
  this->closeChildrenTable =
      PkbTableTransformers::closeFlatten<PARENT>(childrenTable);
  this->closeCallsTable =
      PkbTableTransformers::closeFlatten<Pkb::CALL>(this->callsTable);
  this->closeInvertCallsTable =
      PkbTableTransformers::closeFlatten<Pkb::CALL>(invertCallsTable);

  this->usesTableTransited =
      PkbTableTransformers::transit(this->usesTable, this->usesProcTable);
  this->invertUsesTable =
      PkbTableTransformers::pseudoinvertFlattenKeys<Pkb::LINE_NO, Pkb::VAR>(
          usesTableTransited);
  this->modifiesTableTransited = PkbTableTransformers::transit(
      this->modifiesTable, this->modifiesProcTable);
  this->invertModifiesTable =
      PkbTableTransformers::pseudoinvertFlattenKeys<Pkb::LINE_NO, Pkb::VAR>(
          modifiesTableTransited);

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
  this->callsTableIndexesProcNames =
      NAME_SET(callsTable.keys.begin(), callsTable.keys.end());
  this->invertCallsTableIndexesProcNames =
      NAME_SET(invertCallsTable.keys.begin(), invertCallsTable.keys.end());
}

// API for PQL to get attributes

LINE_NAME_PAIRS Pkb::getStmtLineAndName(Statement statement) {
  LINE_NAME_PAIRS result;

  if (invertStatementTypeTable.map.find(statement.type) !=
      invertStatementTypeTable.map.end()) {
    LINE_SET stmtLines = invertStatementTypeTable.map[statement.type];

    // Map access is not checked in all three cases because if the stmt line
    // exists, it must have been added into the respective
    // usesTable/modifiesTable.
    if (statement.type == PkbTables::StatementType::Call) {
      for (PkbTables::LINE_NO line : stmtLines) {
        PkbTables::PROC proc = std::get<Pkb::PROC>(usesTable.map[line]);
        result.first.push_back(line);
        result.second.push_back(proc);
      }
    } else if (statement.type == PkbTables::StatementType::Print) {
      for (PkbTables::LINE_NO line : stmtLines) {
        // Since line is a print stmt, there must be only 1 variable,
        // the variable being printed.
        PkbTables::VAR var = *std::get<Pkb::VARS>(usesTable.map[line]).begin();
        result.first.push_back(line);
        result.second.push_back(var);
      }
    } else if (statement.type == PkbTables::StatementType::Read) {
      for (PkbTables::LINE_NO line : stmtLines) {
        // Since line is a read stmt, there must be only 1 variable,
        // variable being read.
        PkbTables::VAR var =
            *std::get<Pkb::VARS>(modifiesTable.map[line]).begin();
        result.first.push_back(line);
        result.second.push_back(var);
      }
    } else {
      throw "Statement type is not supported in getStmtLineAndName";
    }
  }

  return result;
}

// Query API for pattern matching

LINE_NAME_PAIRS Pkb::match(Statement statement, Variable variable,
                           PatternSpec spec) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_NAME_PAIRS result;

  if (statement.type == PkbTables::StatementType::Assign) {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET assignLines = invertStatementTypeTable.map[statement.type];

      if (spec.type == PatternMatchType::Any) {
        for (auto line : assignLines) {
          if (modifiesTable.map.find(line) != modifiesTable.map.end()) {
            PkbTables::VARS vars =
                std::get<PkbTables::VARS>(modifiesTable.map[line]);

            // Since line is an assignment statement, there must be only one
            // var in vars.
            PkbTables::VAR modifiedVar = *vars.begin();

            result.first.push_back(line);
            result.second.push_back(modifiedVar);
          }
        }
      } else if (spec.type == PatternMatchType::CompleteMatch) {
        for (auto line : assignLines) {
          if (assignAstTable.map.find(line) != assignAstTable.map.end()) {
            PkbTables::AST astOnLine = assignAstTable.map[line];

            if (astOnLine == *spec.value) {
              if (modifiesTable.map.find(line) != modifiesTable.map.end()) {
                PkbTables::VARS vars =
                    std::get<PkbTables::VARS>(modifiesTable.map[line]);

                // Since line is an assignment statement, there must be only one
                // var in vars.
                PkbTables::VAR modifiedVar = *vars.begin();

                result.first.push_back(line);
                result.second.push_back(modifiedVar);
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
                PkbTables::VARS vars =
                    std::get<PkbTables::VARS>(modifiesTable.map[line]);

                // Since line is an assignment statement, there must be only one
                // var in vars.
                PkbTables::VAR modifiedVar = *vars.begin();

                result.first.push_back(line);
                result.second.push_back(modifiedVar);
              }
            }
          }
        }
      } else {
        throw "Error PatternSpec type is not assigned";
      }
    }
  } else if (statement.type == PkbTables::StatementType::If ||
             statement.type == PkbTables::StatementType::While) {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      PkbTables::LINE_NOS stmtLines =
          invertStatementTypeTable.map[statement.type];

      for (PkbTables::LINE_NO line : stmtLines) {
        if (conditionVarsTable.map.find(line) != conditionVarsTable.map.end()) {
          PkbTables::VARS varsInCondition = conditionVarsTable.map[line];

          for (PkbTables::VAR var : varsInCondition) {
            result.first.push_back(line);
            result.second.push_back(var);
          }
        }
      }
    }
  }

  return result;
}

LINE_SET Pkb::match(Statement statement, Underscore underscore,
                    PatternSpec spec) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_SET result;

  if (statement.type == PkbTables::StatementType::Assign) {
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
  } else if (statement.type == PkbTables::StatementType::If ||
             statement.type == PkbTables::StatementType::While) {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      result = invertStatementTypeTable.map[statement.type];
    }
  }

  return result;
}

LINE_SET Pkb::match(Statement statement, String variable, PatternSpec spec) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_SET result;

  if (statement.type == PkbTables::StatementType::Assign) {
    if (invertModifiesTable.map.find(variable.name) !=
        invertModifiesTable.map.end()) {
      LINE_SET linesVarModifies = invertModifiesTable.map[variable.name];

      for (auto line : linesVarModifies) {
        // map access is not checked here because if a line modified by var
        // exists, its type would have been mapped in statementTypeTable
        if (statementTypeTable.map[line] == PkbTables::StatementType::Assign) {
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
  } else if (statement.type == PkbTables::StatementType::If ||
             statement.type == PkbTables::StatementType::While) {
    // check that variable exists first.
    if (varTable.find(variable.name) != varTable.end()) {
      if (invertStatementTypeTable.map.find(statement.type) !=
          invertStatementTypeTable.map.end()) {
        PkbTables::LINE_NOS stmtLines =
            invertStatementTypeTable.map[statement.type];

        for (PkbTables::LINE_NO line : stmtLines) {
          if (conditionVarsTable.map.find(line) !=
              conditionVarsTable.map.end()) {
            PkbTables::VARS varsInCondition = conditionVarsTable.map[line];

            if (varsInCondition.find(variable.name) != varsInCondition.end()) {
              result.insert(line);
            }
          }
        }
      }
    }
  }

  return result;
}

// Query API for normal select

NAME_SET Pkb::select(Variable var) { return varTable; }

LINE_SET Pkb::select(Statement statement) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (statement.type == PkbTables::StatementType::None) {
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

NAME_SET Pkb::select(Procedure proc) { return procTable; }

CONST_SET Pkb::select(Constant constant) { return constantTable; }

// Query API for follows

bool Pkb::follows(LineNumber line1, LineNumber line2) {
  if (followTable.map.find(line1.number) != followTable.map.end()) {
    return followTable.map[line1.number] == line2.number;
  }
  return false;
}

LINE_SET Pkb::follows(LineNumber line, Statement statement) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (followTable.map.find(line.number) != followTable.map.end()) {
    PkbTables::FOLLOW followLine = followTable.map[line.number];

    if (statement.type == PkbTables::StatementType::None) {
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
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (prevLineTable.map.find(line.number) != prevLineTable.map.end()) {
    PkbTables::LINE_NO prevLine = prevLineTable.map[line.number];

    if (statement.type == PkbTables::StatementType::None) {
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
  if (statement1.type == PkbTables::StatementType::NotSet ||
      statement2.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_LINE_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == PkbTables::StatementType::None &&
      statement2.type == PkbTables::StatementType::None) {
    result.first = followTable.keys;
    result.second = prevLineTable.keys;
  }

  // case 2: only statement1 is of type NONE
  else if (statement1.type == PkbTables::StatementType::None) {
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
  else if (statement2.type == PkbTables::StatementType::None) {
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
  else if (statement1.type != PkbTables::StatementType::None &&
           statement2.type != PkbTables::StatementType::None) {
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
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (statement.type == PkbTables::StatementType::None) {
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
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (statement.type == PkbTables::StatementType::None) {
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
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (closeFollowTable.map.find(line.number) != closeFollowTable.map.end()) {
    LINE_SET followLines = closeFollowTable.map[line.number];

    if (statement.type == PkbTables::StatementType::None) {
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

LINE_SET Pkb::followsStar(Statement statement, LineNumber line) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (closePrevLineTable.map.find(line.number) !=
      closePrevLineTable.map.end()) {
    LINE_SET prevLines = closePrevLineTable.map[line.number];

    if (statement.type == PkbTables::StatementType::None) {
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
  if (statement1.type == PkbTables::StatementType::NotSet ||
      statement2.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_LINE_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == PkbTables::StatementType::None &&
      statement2.type == PkbTables::StatementType::None) {
    for (auto entry : closeFollowTable.map) {
      PkbTables::LINE_NO line = entry.first;

      for (auto followLine : entry.second) {
        result.first.push_back(line);
        result.second.push_back(followLine);
      }
    }
  }

  // case 2: only statement1 is of type NONE
  else if (statement1.type == PkbTables::StatementType::None) {
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
  else if (statement2.type == PkbTables::StatementType::None) {
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
  else if (statement1.type != PkbTables::StatementType::None &&
           statement2.type != PkbTables::StatementType::None) {
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

// Query API for parent

bool Pkb::parent(LineNumber line1, LineNumber line2) {
  if (parentTable.map.find(line2.number) != parentTable.map.end()) {
    return parentTable.map[line2.number] == line1.number;
  }
  return false;
}

LINE_SET Pkb::parent(LineNumber line, Statement statement) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (childrenTable.map.find(line.number) != childrenTable.map.end()) {
    LINE_SET children = childrenTable.map[line.number];

    if (statement.type == PkbTables::StatementType::None) {
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
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (parentTable.map.find(line.number) != parentTable.map.end()) {
    PkbTables::PARENT parent = parentTable.map[line.number];
    if (statement.type == PkbTables::StatementType::None) {
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
  if (statement1.type == PkbTables::StatementType::NotSet ||
      statement2.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_LINE_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == PkbTables::StatementType::None &&
      statement2.type == PkbTables::StatementType::None) {
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
  else if (statement1.type == PkbTables::StatementType::None) {
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

  // case 3: only statement2 is of type None
  else if (statement2.type == PkbTables::StatementType::None) {
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

  // case 4: both statements are not of type None
  else if (statement1.type != PkbTables::StatementType::None &&
           statement2.type != PkbTables::StatementType::None) {
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
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (statement.type == PkbTables::StatementType::None) {
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
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (statement.type == PkbTables::StatementType::None) {
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
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (closeChildrenTable.map.find(line.number) !=
      closeChildrenTable.map.end()) {
    LINE_SET descendantLines = closeChildrenTable.map[line.number];

    if (statement.type == PkbTables::StatementType::None) {
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

LINE_SET Pkb::parentStar(Statement statement, LineNumber line) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (closeParentTable.map.find(line.number) != closeParentTable.map.end()) {
    PkbTables::PARENTS parentLines = closeParentTable.map[line.number];

    if (statement.type == PkbTables::StatementType::None) {
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
  if (statement1.type == PkbTables::StatementType::NotSet ||
      statement2.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_LINE_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == PkbTables::StatementType::None &&
      statement2.type == PkbTables::StatementType::None) {
    for (auto entry : closeChildrenTable.map) {
      PkbTables::PARENT parentLine = entry.first;

      for (auto descendant : entry.second) {
        result.first.push_back(parentLine);
        result.second.push_back(descendant);
      }
    }
  }

  // case 2: only statement1 is of type NONE
  else if (statement1.type == PkbTables::StatementType::None) {
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
  else if (statement2.type == PkbTables::StatementType::None) {
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
  else if (statement1.type != PkbTables::StatementType::None &&
           statement2.type != PkbTables::StatementType::None) {
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

// Query API for uses

bool Pkb::uses(LineNumber line, String variable) {
  if (invertUsesTable.map.find(variable.name) != invertUsesTable.map.end()) {
    LINE_SET usesLines = invertUsesTable.map[variable.name];
    return usesLines.find(line.number) != usesLines.end();
  }
  return false;
}

NAME_SET Pkb::uses(LineNumber line, Variable variable) {
  NAME_SET result;
  if (usesTableTransited.map.find(line.number) !=
      usesTableTransited.map.end()) {
    result = usesTableTransited.map[line.number];
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
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (invertUsesTable.map.find(variable.name) != invertUsesTable.map.end()) {
    if (statement.type == PkbTables::StatementType::None) {
      return invertUsesTable.map[variable.name];
    } else {
      LINE_SET usesLines = invertUsesTable.map[variable.name];
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
  return LINE_SET();
}

LINE_NAME_PAIRS Pkb::uses(Statement statement, Variable variable) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_NAME_PAIRS result;

  if (statement.type == PkbTables::StatementType::None) {
    for (auto entry : usesTableTransited.map) {
      PkbTables::LINE_NO line = entry.first;

      for (PkbTables::VAR var : entry.second) {
        result.first.push_back(line);
        result.second.push_back(var);
      }
    }
  } else {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement.type];

      for (auto line : statementLines) {
        if (usesTableTransited.map.find(line) != usesTableTransited.map.end()) {
          PkbTables::VARS vars = usesTableTransited.map[line];

          for (PkbTables::VAR var : vars) {
            result.first.push_back(line);
            result.second.push_back(var);
          }
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::uses(Statement statement, Underscore underscore) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_SET result;

  if (statement.type == PkbTables::StatementType::None) {
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
  if (usesProcTable.map.find(procedure.name) != usesProcTable.map.end()) {
    PkbTables::VARS varsUsedInProc = usesProcTable.map[procedure.name];
    return varsUsedInProc.find(variable.name) != varsUsedInProc.end();
  }
  return false;
}

NAME_SET Pkb::uses(String procedure, Variable variable) {
  NAME_SET result;

  if (usesProcTable.map.find(procedure.name) != usesProcTable.map.end()) {
    result = usesProcTable.map[procedure.name];
  }
  return result;
}

bool Pkb::uses(String procedure, Underscore underscore) {
  if (usesProcTable.map.find(procedure.name) != usesProcTable.map.end()) {
    return usesProcTable.map[procedure.name].size() > 0;
  }
  return false;
}

NAME_SET Pkb::uses(Procedure procedure, String variable) {
  NAME_SET result;

  if (varTable.find(variable.name) != varTable.end()) {
    for (auto entry : usesProcTable.map) {
      if (entry.second.find(variable.name) != entry.second.end()) {
        result.insert(entry.first);
      }
    }
  }
  return result;
}

NAME_NAME_PAIRS Pkb::uses(Procedure procedure, Variable variable) {
  NAME_NAME_PAIRS result;

  for (auto entry : usesProcTable.map) {
    for (PkbTables::VAR var : entry.second) {
      result.first.push_back(entry.first);
      result.second.push_back(var);
    }
  }
  return result;
}

NAME_SET Pkb::uses(Procedure procedure, Underscore underscore) {
  NAME_SET result;

  for (auto entry : usesProcTable.map) {
    if (entry.second.size() > 0) {
      result.insert(entry.first);
    }
  }
  return result;
}

// Query API for modifies

bool Pkb::modifies(LineNumber line, String variable) {
  if (invertModifiesTable.map.find(variable.name) !=
      invertModifiesTable.map.end()) {
    LINE_SET modifiesLines = invertModifiesTable.map[variable.name];
    return modifiesLines.find(line.number) != modifiesLines.end();
  }
  return false;
}

NAME_SET Pkb::modifies(LineNumber line, Variable variable) {
  NAME_SET result;

  if (modifiesTableTransited.map.find(line.number) !=
      modifiesTableTransited.map.end()) {
    result = modifiesTableTransited.map[line.number];
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
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  if (invertModifiesTable.map.find(variable.name) !=
      invertModifiesTable.map.end()) {
    if (statement.type == PkbTables::StatementType::None) {
      return invertModifiesTable.map[variable.name];
    } else {
      LINE_SET modifiesLines = invertModifiesTable.map[variable.name];
      LINE_SET::iterator it = modifiesLines.begin();

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
  return LINE_SET();
}

LINE_NAME_PAIRS Pkb::modifies(Statement statement, Variable variable) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_NAME_PAIRS result;

  if (statement.type == PkbTables::StatementType::None) {
    for (auto entry : modifiesTableTransited.map) {
      PkbTables::LINE_NO line = entry.first;

      for (PkbTables::VAR var : entry.second) {
        result.first.push_back(line);
        result.second.push_back(var);
      }
    }
  } else {
    if (invertStatementTypeTable.map.find(statement.type) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines = invertStatementTypeTable.map[statement.type];

      for (auto line : statementLines) {
        if (modifiesTableTransited.map.find(line) !=
            modifiesTableTransited.map.end()) {
          PkbTables::VARS vars = modifiesTableTransited.map[line];

          for (PkbTables::VAR var : vars) {
            result.first.push_back(line);
            result.second.push_back(var);
          }
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::modifies(Statement statement, Underscore underscore) {
  if (statement.type == PkbTables::StatementType::NotSet) {
    throw "Error statement type is not assigned";
  }

  LINE_SET result;

  if (statement.type == PkbTables::StatementType::None) {
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
  if (modifiesProcTable.map.find(procedure.name) !=
      modifiesProcTable.map.end()) {
    PkbTables::VARS varsModifiedInProc = modifiesProcTable.map[procedure.name];
    return varsModifiedInProc.find(variable.name) != varsModifiedInProc.end();
  }
  return false;
}

NAME_SET Pkb::modifies(String procedure, Variable variable) {
  NAME_SET result;

  if (modifiesProcTable.map.find(procedure.name) !=
      modifiesProcTable.map.end()) {
    result = modifiesProcTable.map[procedure.name];
  }
  return result;
}

bool Pkb::modifies(String procedure, Underscore underscore) {
  if (modifiesProcTable.map.find(procedure.name) !=
      modifiesProcTable.map.end()) {
    return modifiesProcTable.map[procedure.name].size() > 0;
  }
  return false;
}

NAME_SET Pkb::modifies(Procedure procedure, String variable) {
  NAME_SET result;

  if (varTable.find(variable.name) != varTable.end()) {
    for (auto entry : modifiesProcTable.map) {
      if (entry.second.find(variable.name) != entry.second.end()) {
        result.insert(entry.first);
      }
    }
  }
  return result;
}

NAME_NAME_PAIRS Pkb::modifies(Procedure procedure, Variable variable) {
  NAME_NAME_PAIRS result;

  for (auto entry : modifiesProcTable.map) {
    for (PkbTables::VAR var : entry.second) {
      result.first.push_back(entry.first);
      result.second.push_back(var);
    }
  }
  return result;
}

NAME_SET Pkb::modifies(Procedure procedure, Underscore underscore) {
  NAME_SET result;

  for (auto entry : modifiesProcTable.map) {
    if (entry.second.size() > 0) {
      result.insert(entry.first);
    }
  }
  return result;
}

// Query API for calls

bool Pkb::calls(String procedureName1, String procedureName2) {
  if (callsTable.map.find(procedureName1.name) != callsTable.map.end()) {
    PkbTables::CALLS calls = callsTable.map[procedureName1.name];
    return calls.find(procedureName2.name) != calls.end();
  }
  return false;
}

NAME_SET Pkb::calls(String procedureName, Procedure procedure) {
  NAME_SET result;

  if (callsTable.map.find(procedureName.name) != callsTable.map.end()) {
    result = callsTable.map[procedureName.name];
  }
  return result;
}

bool Pkb::calls(String procedureName, Underscore underscore) {
  if (callsTable.map.find(procedureName.name) != callsTable.map.end()) {
    return callsTable.map[procedureName.name].size() > 0;
  }
  return false;
}

NAME_SET Pkb::calls(Procedure procedure, String procedureName) {
  NAME_SET result;

  if (invertCallsTable.map.find(procedureName.name) !=
      invertCallsTable.map.end()) {
    result = invertCallsTable.map[procedureName.name];
  }
  return result;
}

NAME_NAME_PAIRS Pkb::calls(Procedure procedure1, Procedure procedure2) {
  NAME_NAME_PAIRS result;

  for (auto entry : callsTable.map) {
    for (PkbTables::CALL call : entry.second) {
      result.first.push_back(entry.first);
      result.second.push_back(call);
    }
  }
  return result;
}

NAME_SET Pkb::calls(Procedure procedure, Underscore underscore) {
  return callsTableIndexesProcNames;
}

bool Pkb::calls(Underscore underscore, String procedureName) {
  if (invertCallsTable.map.find(procedureName.name) !=
      invertCallsTable.map.end()) {
    return invertCallsTable.map[procedureName.name].size() > 0;
  }
  return false;
}

NAME_SET Pkb::calls(Underscore underscore, Procedure procedure) {
  return invertCallsTableIndexesProcNames;
}

bool Pkb::calls(Underscore underscore1, Underscore underscore2) {
  return callsTable.map.size() > 0;
}

// Query API for callsStar

bool Pkb::callsStar(String procedureName1, String procedureName2) {
  if (closeCallsTable.map.find(procedureName1.name) !=
      closeCallsTable.map.end()) {
    PkbTables::CALLS calls = closeCallsTable.map[procedureName1.name];
    return calls.find(procedureName2.name) != calls.end();
  }
  return false;
}

NAME_SET Pkb::callsStar(String procedureName, Procedure procedure) {
  NAME_SET result;

  if (closeCallsTable.map.find(procedureName.name) !=
      closeCallsTable.map.end()) {
    result = closeCallsTable.map[procedureName.name];
  }
  return result;
}

NAME_SET Pkb::callsStar(Procedure procedure, String procedureName) {
  NAME_SET result;

  if (closeInvertCallsTable.map.find(procedureName.name) !=
      closeInvertCallsTable.map.end()) {
    result = closeInvertCallsTable.map[procedureName.name];
  }
  return result;
}
NAME_NAME_PAIRS Pkb::callsStar(Procedure procedure1, Procedure procedure2) {
  NAME_NAME_PAIRS result;

  for (auto entry : closeCallsTable.map) {
    for (PkbTables::CALL call : entry.second) {
      result.first.push_back(entry.first);
      result.second.push_back(call);
    }
  }
  return result;
}
