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

const Pkb::NEXTS_TABLE &Pkb::getNextsTable() const { return this->nextsTable; }

const Pkb::NEXT_BIPS_TABLE &Pkb::getNextBipsTable() const {
  return this->nextBipsTable;
}

const Pkb::CFG_NODE_TABLE &Pkb::getCfgNodeTable() const {
  return this->cfgNodeTable;
}

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
  this->assignAstTable.insert(
      ASSIGN_AST_TABLE::value_type(lineNo, std::move(ast)));
}

void Pkb::addConditionVars(LINE_NO lineNo, VARS vars) {
  this->conditionVarsTable.insert({lineNo, vars});
}

void Pkb::addConstant(CONSTANT constant) {
  this->constantTable.insert(constant);
}

void Pkb::addCall(PROC proc, CALL call) {
  if (!this->callsTable.insert({proc, {call}})) {
    // If key already mapped, then insert value into existing mapped
    // unordered_set.
    this->callsTable.map[proc].insert(call);
  }
}

void Pkb::addNext(LINE_NO lineNo, NEXT next) {
  if (!this->nextsTable.insert({lineNo, {next}})) {
    // If key already mapped, then insert value into existing mapped
    // unordered_set.
    nextsTable.map[lineNo].insert(next);
  }
}

void Pkb::addNextBip(LINE_NO lineNo, NEXT_BIP nextBip) {
  if (!this->nextBipsTable.insert({lineNo, {nextBip}})) {
    // If key already mapped, then insert value into existing mapped
    // unordered_set.
    nextBipsTable.map[lineNo].insert(nextBip);
  }
}

void Pkb::addCfgNode(LINE_NO lineNo, CFG_NODE cfgNode) {
  this->cfgNodeTable.insert(
      CFG_NODE_TABLE::value_type(lineNo, std::move(cfgNode)));
}

void Pkb::deriveTables() {
  this->prevLineTable = PkbTableTransformers::invert(this->followTable);
  this->childrenTable = PkbTableTransformers::pseudoinvert(this->parentTable);
  this->invertStatementTypeTable =
      PkbTableTransformers::pseudoinvert(this->statementTypeTable);
  this->invertStatementProcTable =
      PkbTableTransformers::pseudoinvert(this->statementProcTable);
  this->invertCallsTable =
      PkbTableTransformers::pseudoinvertFlattenKeys<PROC, CALL>(
          this->callsTable);
  this->invertNextsTable =
      PkbTableTransformers::pseudoinvertFlattenKeys<LINE_NO, NEXT>(
          this->nextsTable);

  this->closeFollowTable = PkbTableTransformers::close(this->followTable);
  this->closeParentTable = PkbTableTransformers::close(this->parentTable);
  this->closePrevLineTable = PkbTableTransformers::close(prevLineTable);
  this->closeChildrenTable =
      PkbTableTransformers::closeFlatten<PARENT>(childrenTable);
  this->closeCallsTable =
      PkbTableTransformers::closeFlatten<CALL>(this->callsTable);
  this->closeInvertCallsTable =
      PkbTableTransformers::closeFlatten<CALL>(invertCallsTable);

  this->usesTableTransited =
      PkbTableTransformers::transit(this->usesTable, this->usesProcTable);
  this->invertUsesTable =
      PkbTableTransformers::pseudoinvertFlattenKeys<LINE_NO, VAR>(
          usesTableTransited);
  this->modifiesTableTransited = PkbTableTransformers::transit(
      this->modifiesTable, this->modifiesProcTable);
  this->invertModifiesTable =
      PkbTableTransformers::pseudoinvertFlattenKeys<LINE_NO, VAR>(
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
  this->nextTableIndexes =
      LINE_SET(nextsTable.keys.begin(), nextsTable.keys.end());
  this->invertNextsTableIndexes =
      LINE_SET(invertNextsTable.keys.begin(), invertNextsTable.keys.end());

  this->callsTableIndexesProcNames =
      NAME_SET(callsTable.keys.begin(), callsTable.keys.end());
  this->invertCallsTableIndexesProcNames =
      NAME_SET(invertCallsTable.keys.begin(), invertCallsTable.keys.end());

  // clear cache after all tables are derived to ensure cache is empty at start
  // of query.
  this->clearCache();
}

void Pkb::deriveAllNextBipRelatedTables() {
  this->nextBipTable = deriveNextBipTable();
  this->invertNextBipTable =
      PkbTableTransformers::pseudoinvertFlattenKeys<LINE_NO, LINE_NO>(
          this->nextBipTable);
  this->nextBipTableIndexes =
      LINE_SET(nextBipTable.keys.begin(), nextBipTable.keys.end());
  this->invertNextBipTableIndexes =
      LINE_SET(invertNextBipTable.keys.begin(), invertNextBipTable.keys.end());
  this->areAllNextBipRelatedTablesDerived = true;
}

void Pkb::deriveAllCloseNextBipRelatedTables() {
  this->closeNextBipTable = deriveCloseNextBipTable();
  this->closeInvertNextBipTable =
      PkbTableTransformers::pseudoinvertFlattenKeys<LINE_NO, LINE_NO>(
          this->closeNextBipTable);
  this->areAllCloseNextBipRelatedTablesDerived = true;
}

KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NOS> Pkb::deriveNextBipTable() {
  KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NOS> nextBipTable;

  for (auto entry : nextBipsTable.map) {
    NEXTS nexts;

    for (NEXT_BIP nextBip : entry.second) {
      if (std::holds_alternative<NEXT>(nextBip)) {
        nexts.insert(std::get<NEXT>(nextBip));
      } else if (std::holds_alternative<
                     std::tuple<LINE_NO, CallBranch, CALL_BRANCH_LABEL>>(
                     nextBip)) {
        std::tuple tuple =
            std::get<std::tuple<LINE_NO, CallBranch, CALL_BRANCH_LABEL>>(
                nextBip);
        nexts.insert(std::get<0>(tuple));
      }
    }
    nextBipTable.insert(std::pair(entry.first, nexts));
  }
  return nextBipTable;
}

KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NOS>
Pkb::deriveCloseNextBipTable() {
  KeysTable<PkbTables::LINE_NO, PkbTables::LINE_NOS> closeNextBipTable;
  LINE_NOS allCallStmts = invertStatementTypeTable.map[StatementType::Call];

  for (LINE_NO line : statementTypeTable.keys) {
    LINE_SET closeNextBips;
    LINE_NOS callStmtsVisited;

    // if stmt is a call, then include all lines in the called procedure as
    // well.
    if (statementTypeTable.map[line] == StatementType::Call) {
      PROC calledProc = std::get<PROC>(usesTable.map[line]);
      closeNextBips.merge(getAllStmtsOfTransitiveCall(calledProc));
    }

    // get transitive next bip of current line up to the end of the procedure
    // the line is in.
    closeNextBips.merge(getTransitiveNextBip(line));

    // get transitive next bip of lines that call the procedure the initial line
    // is in.
    PROC procOfLine = statementProcTable.map[line];

    for (LINE_NO callStmt : allCallStmts) {
      PROC procCalledOnStmt = std::get<PROC>(usesTable.map[callStmt]);
      if (procCalledOnStmt == procOfLine) {
        callStmtsVisited.insert(callStmt);
        closeNextBips.merge(getTransitiveNextBip(callStmt));
      }
    }

    // get transitive next bip of all lines that directly/indirectly call the
    // procedure the initial line is in.
    PROCS allProcsThatTransitivelyCallProcOfLine =
        closeInvertCallsTable.map[procOfLine];

    for (PROC proc : allProcsThatTransitivelyCallProcOfLine) {
      for (LINE_NO callStmt : allCallStmts) {
        PROC procCalledOnStmt = std::get<PROC>(usesTable.map[callStmt]);
        if (procCalledOnStmt == proc) {
          callStmtsVisited.insert(callStmt);
          closeNextBips.merge(getTransitiveNextBip(callStmt));
        }
      }
    }

    closeNextBipTable.insert(std::pair(line, closeNextBips));
  }
  return closeNextBipTable;
}

LINE_SET Pkb::getTransitiveNextBip(LINE_NO line) {
  PROCS procsVisited;
  LINE_SET result;
  NEXTS transitiveNexts = getTransitiveNextStatements(line, {});

  for (NEXT next : transitiveNexts) {
    if (statementTypeTable.map[next] == StatementType::Call) {
      PROC calledProc = std::get<PROC>(usesTable.map[next]);

      if (procsVisited.find(calledProc) == procsVisited.end()) {
        procsVisited.insert(calledProc);

        PROCS transitiveCalls = closeCallsTable.map[calledProc];
        procsVisited.merge(transitiveCalls);
        result.merge(getAllStmtsOfTransitiveCall(calledProc));
      }
    }
  }
  result.merge(transitiveNexts);
  return result;
}

LINE_SET Pkb::getAllStmtsOfTransitiveCall(PROC proc) {
  LINE_SET result;

  // add all stmts of the input proc.
  LINE_NOS stmtsInProc = invertStatementProcTable.map[proc];
  result.merge(stmtsInProc);
  PROCS transitiveCalls = closeCallsTable.map[proc];

  for (PROC proc : transitiveCalls) {
    // add all stmts of transitive procs called.
    LINE_NOS stmtsInTransitiveProc = invertStatementProcTable.map[proc];
    result.merge(stmtsInTransitiveProc);
  }
  return result;
}

// API for clearing cache after query

void Pkb::clearCache() {
  closeNextsTableCache.map.clear();
  closeInvertNextsTableCache.map.clear();
  affectsTableCache.map.clear();
  closeAffectsTableCache.map.clear();
  invertAffectsTableCache.map.clear();
  closeInvertAffectsTableCache.map.clear();

  isCloseNextsTableCached = false;
  isCloseInvertNextsTableCached = false;
  isAffectsTableCached = false;
  isCloseAffectsTableCached = false;
  isInvertAffectsTableCached = false;
  isCloseInvertAffectsTableCached = false;
}

// API for Pql to get attributes

LINE_NAME_PAIRS Pkb::selectAttribute(Statement statement) {
  LINE_NAME_PAIRS result;

  if (!statement.type.has_value()) {
    throw "getStmtLineAndName must have a statement type assigned";
  }

  if (invertStatementTypeTable.map.find(statement.type.value()) !=
      invertStatementTypeTable.map.end()) {
    LINE_SET stmtLines = invertStatementTypeTable.map[statement.type.value()];

    // Map access is not checked in all three cases because if the stmt line
    // exists, it must have been added into the respective
    // usesTable/modifiesTable.
    if (statement.type.value() == StatementType::Call) {
      for (LINE_NO line : stmtLines) {
        PROC proc = std::get<PROC>(usesTable.map[line]);
        result.first.push_back(line);
        result.second.push_back(proc);
      }
    } else if (statement.type.value() == StatementType::Print) {
      for (LINE_NO line : stmtLines) {
        // Since line is a print stmt, there must be only 1 variable,
        // the variable being printed.
        VAR var = *std::get<VARS>(usesTable.map[line]).begin();
        result.first.push_back(line);
        result.second.push_back(var);
      }
    } else if (statement.type.value() == StatementType::Read) {
      for (LINE_NO line : stmtLines) {
        // Since line is a read stmt, there must be only 1 variable,
        // variable being read.
        VAR var = *std::get<VARS>(modifiesTable.map[line]).begin();
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
  if (!statement.type.has_value()) {
    throw "Pattern matching statement must have a type assigned";
  }

  LINE_NAME_PAIRS result;

  if (statement.type.value() == StatementType::Assign) {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET assignLines =
          invertStatementTypeTable.map[statement.type.value()];

      if (spec.type == PatternMatchType::Any) {
        for (auto line : assignLines) {
          if (modifiesTable.map.find(line) != modifiesTable.map.end()) {
            VARS vars = std::get<VARS>(modifiesTable.map[line]);

            // Since line is an assignment statement, there must be only one
            // var in vars.
            VAR modifiedVar = *vars.begin();

            result.first.push_back(line);
            result.second.push_back(modifiedVar);
          }
        }
      } else if (spec.type == PatternMatchType::CompleteMatch) {
        for (auto line : assignLines) {
          if (assignAstTable.map.find(line) != assignAstTable.map.end()) {
            TNode *astOnLine = assignAstTable.map[line].get();

            if (*astOnLine == *spec.value) {
              if (modifiesTable.map.find(line) != modifiesTable.map.end()) {
                VARS vars = std::get<VARS>(modifiesTable.map[line]);

                // Since line is an assignment statement, there must be only one
                // var in vars.
                VAR modifiedVar = *vars.begin();

                result.first.push_back(line);
                result.second.push_back(modifiedVar);
              }
            }
          }
        }
      } else if (spec.type == PatternMatchType::SubTreeMatch) {
        for (auto line : assignLines) {
          if (assignAstTable.map.find(line) != assignAstTable.map.end()) {
            TNode *astOnLine = assignAstTable.map[line].get();

            if (*astOnLine >= *spec.value) {
              if (modifiesTable.map.find(line) != modifiesTable.map.end()) {
                VARS vars = std::get<VARS>(modifiesTable.map[line]);

                // Since line is an assignment statement, there must be only one
                // var in vars.
                VAR modifiedVar = *vars.begin();

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
  } else if (statement.type.value() == StatementType::If ||
             statement.type.value() == StatementType::While) {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS stmtLines = invertStatementTypeTable.map[statement.type.value()];

      for (LINE_NO line : stmtLines) {
        if (conditionVarsTable.map.find(line) != conditionVarsTable.map.end()) {
          VARS varsInCondition = conditionVarsTable.map[line];

          for (VAR var : varsInCondition) {
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
  if (!statement.type.has_value()) {
    throw "Pattern matching statement must have a type assigned";
  }

  LINE_SET result;

  if (statement.type.value() == StatementType::Assign) {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET assignLines =
          invertStatementTypeTable.map[statement.type.value()];

      if (spec.type == PatternMatchType::Any) {
        // if no matching is required, then return assignment lines without
        // processing.
        return assignLines;
      } else {
        for (auto line : assignLines) {
          if (assignAstTable.map.find(line) != assignAstTable.map.end()) {
            TNode *astOnLine = assignAstTable.map[line].get();

            if (spec.type == PatternMatchType::CompleteMatch) {
              if (*astOnLine == *spec.value) {
                result.insert(line);
              }
            } else if (spec.type == PatternMatchType::SubTreeMatch) {
              if (*astOnLine >= *spec.value) {
                result.insert(line);
              }
            } else {
              throw "Error PatternSpec type is not assigned";
            }
          }
        }
      }
    }
  } else if (statement.type.value() == StatementType::If ||
             statement.type.value() == StatementType::While) {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      result = invertStatementTypeTable.map[statement.type.value()];
    }
  }

  return result;
}

LINE_SET Pkb::match(Statement statement, String variable, PatternSpec spec) {
  if (!statement.type.has_value()) {
    throw "Pattern matching statement must have a type assigned";
  }

  LINE_SET result;

  if (statement.type.value() == StatementType::Assign) {
    if (invertModifiesTable.map.find(variable.name) !=
        invertModifiesTable.map.end()) {
      LINE_SET linesVarModifies = invertModifiesTable.map[variable.name];

      for (auto line : linesVarModifies) {
        // map access is not checked here because if a line modified by var
        // exists, its type would have been mapped in statementTypeTable
        if (statementTypeTable.map[line] == StatementType::Assign) {
          if (assignAstTable.map.find(line) != assignAstTable.map.end()) {
            TNode *astOnLine = assignAstTable.map[line].get();

            if (spec.type == PatternMatchType::CompleteMatch) {
              if (*astOnLine == *spec.value) {
                result.insert(line);
              }
            } else if (spec.type == PatternMatchType::SubTreeMatch) {
              if (*astOnLine >= *spec.value) {
                result.insert(line);
              }
            } else {
              result.insert(line);
            }
          }
        }
      }
    }
  } else if (statement.type.value() == StatementType::If ||
             statement.type.value() == StatementType::While) {
    // check that variable exists first.
    if (varTable.find(variable.name) != varTable.end()) {
      if (invertStatementTypeTable.map.find(statement.type.value()) !=
          invertStatementTypeTable.map.end()) {
        LINE_NOS stmtLines =
            invertStatementTypeTable.map[statement.type.value()];

        for (LINE_NO line : stmtLines) {
          if (conditionVarsTable.map.find(line) !=
              conditionVarsTable.map.end()) {
            VARS varsInCondition = conditionVarsTable.map[line];

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
  if (!statement.type.has_value()) {
    return stmtTableIndexes;
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      return invertStatementTypeTable.map[statement.type.value()];
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
  if (followTable.map.find(line.number) != followTable.map.end()) {
    FOLLOW followLine = followTable.map[line.number];

    if (!statement.type.has_value()) {
      return followLine != LINE_NO() ? LINE_SET{followLine} : LINE_SET();
    } else {
      // map access is not checked here because if a follow line exists, its
      // type would have been mapped in statementTypeTable
      StatementType type = statementTypeTable.map[followLine];
      return followLine != LINE_NO() && statement.type.value() == type
                 ? LINE_SET{followLine}
                 : LINE_SET();
    }
  }
  return LINE_SET();
}

bool Pkb::follows(LineNumber line, Underscore underscore) {
  if (followTable.map.find(line.number) != followTable.map.end()) {
    return followTable.map[line.number] != LINE_NO();
  }
  return false;
}

LINE_SET Pkb::follows(Statement statement, LineNumber line) {
  if (prevLineTable.map.find(line.number) != prevLineTable.map.end()) {
    LINE_NO prevLine = prevLineTable.map[line.number];

    if (!statement.type.has_value()) {
      return prevLine != LINE_NO() ? LINE_SET{prevLine} : LINE_SET();
    } else {
      // map access is not checked here because if a prev line exists, its
      // type would have been mapped in statementTypeTable
      StatementType type = statementTypeTable.map[prevLine];
      return prevLine != LINE_NO() && statement.type.value() == type
                 ? LINE_SET{prevLine}
                 : LINE_SET();
    }
  }
  return LINE_SET();
}

LINE_LINE_PAIRS Pkb::follows(Statement statement1, Statement statement2) {
  LINE_LINE_PAIRS result;

  // case 1: both statements are stmts
  if (!statement1.type.has_value() && !statement2.type.has_value()) {
    result.first = followTable.keys;
    result.second = prevLineTable.keys;
  }

  // case 2: only statement1 is stmt
  else if (!statement1.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement2.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers =
          invertStatementTypeTable.map[statement2.type.value()];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (prevLineTable.map.find(*it) != prevLineTable.map.end()) {
          LINE_NO prevLine = prevLineTable.map[*it];
          if (prevLine != LINE_NO()) {
            result.first.push_back(prevLine);
            result.second.push_back(*it);
          }
        }
        it++;
      }
    }
  }

  // case 3: only statement2 is stmt
  else if (!statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers =
          invertStatementTypeTable.map[statement1.type.value()];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (followTable.map.find(*it) != followTable.map.end()) {
          FOLLOW followLine = followTable.map[*it];
          if (followLine != LINE_NO()) {
            result.first.push_back(*it);
            result.second.push_back(followLine);
          }
        }
        it++;
      }
    }
  }

  // case 4: both statements are not stmts
  else if (statement1.type.has_value() && statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers =
          invertStatementTypeTable.map[statement1.type.value()];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (followTable.map.find(*it) != followTable.map.end()) {
          FOLLOW followLine = followTable.map[*it];
          // map access is not checked here because if a follow line exists, its
          // type would have been mapped in statementTypeTable
          StatementType type = statementTypeTable.map[followLine];
          if (followLine != LINE_NO() && type == statement2.type) {
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
  if (!statement.type.has_value()) {
    return followTableIndexes;
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers =
          invertStatementTypeTable.map[statement.type.value()];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (followTable.map.find(*it) != followTable.map.end()) {
          if (followTable.map[*it] == LINE_NO()) {
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
    return prevLineTable.map[line.number] != LINE_NO();
  }
  return false;
}

LINE_SET Pkb::follows(Underscore underscore, Statement statement) {
  if (!statement.type.has_value()) {
    return prevLineTableIndexes;
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers =
          invertStatementTypeTable.map[statement.type.value()];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (prevLineTable.map.find(*it) != prevLineTable.map.end()) {
          if (prevLineTable.map[*it] == LINE_NO()) {
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
    FOLLOWS followLines = closeFollowTable.map[line1.number];
    return followLines.find(line2.number) != followLines.end();
  }
  return false;
}

LINE_SET Pkb::followsStar(LineNumber line, Statement statement) {
  if (closeFollowTable.map.find(line.number) != closeFollowTable.map.end()) {
    LINE_SET followLines = closeFollowTable.map[line.number];

    if (!statement.type.has_value()) {
      return followLines;
    } else {
      LINE_SET::iterator it = followLines.begin();

      while (it != followLines.end()) {
        // map access is not checked here because if a follow line exists, its
        // type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type.value()) {
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
  if (closePrevLineTable.map.find(line.number) !=
      closePrevLineTable.map.end()) {
    LINE_SET prevLines = closePrevLineTable.map[line.number];

    if (!statement.type.has_value()) {
      return prevLines;
    } else {
      LINE_SET::iterator it = prevLines.begin();

      while (it != prevLines.end()) {
        // map access is not checked here because if a prev line exists, its
        // type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type.value()) {
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
  LINE_LINE_PAIRS result;

  // case 1: both statements are stmts
  if (!statement1.type.has_value() && !statement2.type.has_value()) {
    for (auto entry : closeFollowTable.map) {
      LINE_NO line = entry.first;

      for (auto followLine : entry.second) {
        result.first.push_back(line);
        result.second.push_back(followLine);
      }
    }
  }

  // case 2: only statement1 is stmt
  else if (!statement1.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement2.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines =
          invertStatementTypeTable.map[statement2.type.value()];

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

  // case 3: only statement2 is stmt
  else if (!statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines =
          invertStatementTypeTable.map[statement1.type.value()];

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

  // case 4: both statements are not stmts
  else if (statement1.type.has_value() && statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines =
          invertStatementTypeTable.map[statement1.type.value()];

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
  if (childrenTable.map.find(line.number) != childrenTable.map.end()) {
    LINE_SET children = childrenTable.map[line.number];

    if (!statement.type.has_value()) {
      return children;
    } else {
      LINE_SET::iterator it = children.begin();

      while (it != children.end()) {
        // map access is not checked here because if a child line exists, its
        // type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type.value()) {
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
  if (parentTable.map.find(line.number) != parentTable.map.end()) {
    PARENT parent = parentTable.map[line.number];
    if (!statement.type.has_value()) {
      return parent != LINE_NO() ? LINE_SET{parent} : LINE_SET();
    } else {
      // map access is not checked here because if the parent line exists, its
      // type would have been mapped in statementTypeTable
      StatementType type = statementTypeTable.map[parent];
      return parent != LINE_NO() && type == statement.type.value()
                 ? LINE_SET{parent}
                 : LINE_SET();
    }
  }
  return LINE_SET();
}

LINE_LINE_PAIRS Pkb::parent(Statement statement1, Statement statement2) {
  LINE_LINE_PAIRS result;

  // case 1: both statements are stmts
  if (!statement1.type.has_value() && !statement2.type.has_value()) {
    LINE_SET parents = childrenTableIndexes;
    LINE_SET::iterator parentIt = parents.begin();

    while (parentIt != parents.end()) {
      // map access is not checked here because childrenTableIndexes contains
      // the keys of childrenTable which ensures that the key already exists
      // before accessing
      CHILDREN children = childrenTable.map[*parentIt];
      LINE_SET::iterator childIt = children.begin();

      while (childIt != children.end()) {
        result.first.push_back(*parentIt);
        result.second.push_back(*childIt);
        childIt++;
      }
      parentIt++;
    }
  }

  // case 2: only statement1 is stmt
  else if (!statement1.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement2.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET lineNumbers =
          invertStatementTypeTable.map[statement2.type.value()];
      LINE_SET::iterator it = lineNumbers.begin();

      while (it != lineNumbers.end()) {
        if (parentTable.map.find(*it) != parentTable.map.end()) {
          PARENT parent = parentTable.map[*it];
          if (parent != LINE_NO()) {
            result.first.push_back(parent);
            result.second.push_back(*it);
          }
        }
        it++;
      }
    }
  }

  // case 3: only statement2 is stmt
  else if (!statement2.type.has_value()) {
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

  // case 4: both statements are not stmts
  else if (statement1.type.has_value() && statement2.type.has_value()) {
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
  if (!statement.type.has_value()) {
    return childrenTableIndexes;
  } else {
    LINE_SET parents = childrenTableIndexes;
    LINE_SET::iterator it = parents.begin();

    while (it != parents.end()) {
      // map access is not checked here because if a parent line exists, its
      // type would have been mapped in statementTypeTable
      if (statementTypeTable.map[*it] != statement.type.value()) {
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
    PARENT parent = parentTable.map[line.number];
    return parent != LINE_NO();
  }
  return false;
}

LINE_SET Pkb::parent(Underscore underscore, Statement statement) {
  if (!statement.type.has_value()) {
    return parentTableIndexes;
  } else {
    LINE_SET children = parentTableIndexes;
    LINE_SET::iterator it = children.begin();

    while (it != children.end()) {
      // map access is not checked here because if a child line exists, its
      // type would have been mapped in statementTypeTable
      if (statementTypeTable.map[*it] != statement.type.value()) {
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
    PARENTS parentLines = closeParentTable.map[line2.number];
    return parentLines.find(line1.number) != parentLines.end();
  }
  return false;
}

LINE_SET Pkb::parentStar(LineNumber line, Statement statement) {
  if (closeChildrenTable.map.find(line.number) !=
      closeChildrenTable.map.end()) {
    LINE_SET descendantLines = closeChildrenTable.map[line.number];

    if (!statement.type.has_value()) {
      return descendantLines;
    } else {
      LINE_SET::iterator it = descendantLines.begin();
      while (it != descendantLines.end()) {
        // map access is not checked here because if a descendant line exists,
        // its type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type.value()) {
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
  if (closeParentTable.map.find(line.number) != closeParentTable.map.end()) {
    PARENTS parentLines = closeParentTable.map[line.number];

    if (!statement.type.has_value()) {
      return parentLines;
    } else {
      PARENTS::iterator it = parentLines.begin();
      while (it != parentLines.end()) {
        // map access is not checked here because if a parent line exists, its
        // type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type.value()) {
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
  LINE_LINE_PAIRS result;

  // case 1: both statements are stmts
  if (!statement1.type.has_value() && !statement2.type.has_value()) {
    for (auto entry : closeChildrenTable.map) {
      PARENT parentLine = entry.first;

      for (auto descendant : entry.second) {
        result.first.push_back(parentLine);
        result.second.push_back(descendant);
      }
    }
  }

  // case 2: only statement1 is stmt
  else if (!statement1.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement2.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines =
          invertStatementTypeTable.map[statement2.type.value()];

      for (auto child : statementLines) {
        if (closeParentTable.map.find(child) != closeParentTable.map.end()) {
          PARENTS ancestors = closeParentTable.map[child];

          for (auto ancestor : ancestors) {
            result.first.push_back(ancestor);
            result.second.push_back(child);
          }
        }
      }
    }
  }

  // case 3: only statement2 is stmt
  else if (!statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines =
          invertStatementTypeTable.map[statement1.type.value()];

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
  else if (statement1.type.has_value() && statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement2.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines =
          invertStatementTypeTable.map[statement2.type.value()];

      for (auto child : statementLines) {
        if (closeParentTable.map.find(child) != closeParentTable.map.end()) {
          LINE_SET ancestors = closeParentTable.map[child];

          for (auto ancestor : ancestors) {
            if (statementTypeTable.map[ancestor] == statement1.type.value()) {
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
  if (invertUsesTable.map.find(variable.name) != invertUsesTable.map.end()) {
    if (!statement.type.has_value()) {
      return invertUsesTable.map[variable.name];
    } else {
      LINE_SET usesLines = invertUsesTable.map[variable.name];
      LINE_SET::iterator it = usesLines.begin();

      while (it != usesLines.end()) {
        // map access is not checked here because if a line that uses variable
        // exists, its type would have been mapped in statementTypeTable
        if (statementTypeTable.map[*it] != statement.type.value()) {
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
  LINE_NAME_PAIRS result;

  if (!statement.type.has_value()) {
    for (auto entry : usesTableTransited.map) {
      LINE_NO line = entry.first;

      for (VAR var : entry.second) {
        result.first.push_back(line);
        result.second.push_back(var);
      }
    }
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines =
          invertStatementTypeTable.map[statement.type.value()];

      for (auto line : statementLines) {
        if (usesTableTransited.map.find(line) != usesTableTransited.map.end()) {
          VARS vars = usesTableTransited.map[line];

          for (VAR var : vars) {
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
  LINE_SET result;

  if (!statement.type.has_value()) {
    for (auto entry : usesTableTransited.map) {
      if (entry.second.size() > 0) {
        result.insert(entry.first);
      }
    }
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines =
          invertStatementTypeTable.map[statement.type.value()];

      for (LINE_NO line : statementLines) {
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
    VARS varsUsedInProc = usesProcTable.map[procedure.name];
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
    for (VAR var : entry.second) {
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
  if (invertModifiesTable.map.find(variable.name) !=
      invertModifiesTable.map.end()) {
    if (!statement.type.has_value()) {
      return invertModifiesTable.map[variable.name];
    } else {
      LINE_SET modifiesLines = invertModifiesTable.map[variable.name];
      LINE_SET::iterator it = modifiesLines.begin();

      while (it != modifiesLines.end()) {
        if (statementTypeTable.map[*it] != statement.type.value()) {
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
  LINE_NAME_PAIRS result;

  if (!statement.type.has_value()) {
    for (auto entry : modifiesTableTransited.map) {
      LINE_NO line = entry.first;

      for (VAR var : entry.second) {
        result.first.push_back(line);
        result.second.push_back(var);
      }
    }
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines =
          invertStatementTypeTable.map[statement.type.value()];

      for (auto line : statementLines) {
        if (modifiesTableTransited.map.find(line) !=
            modifiesTableTransited.map.end()) {
          VARS vars = modifiesTableTransited.map[line];

          for (VAR var : vars) {
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
  LINE_SET result;

  if (!statement.type.has_value()) {
    for (auto entry : modifiesTableTransited.map) {
      if (entry.second.size() > 0) {
        result.insert(entry.first);
      }
    }
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET statementLines =
          invertStatementTypeTable.map[statement.type.value()];

      for (LINE_NO line : statementLines) {
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
    VARS varsModifiedInProc = modifiesProcTable.map[procedure.name];
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
    for (VAR var : entry.second) {
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
    CALLS calls = callsTable.map[procedureName1.name];
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
    for (CALL call : entry.second) {
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
    CALLS calls = closeCallsTable.map[procedureName1.name];
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

bool Pkb::callsStar(String procedureName, Underscore underscore) {
  return calls(procedureName, underscore);
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
    for (CALL call : entry.second) {
      result.first.push_back(entry.first);
      result.second.push_back(call);
    }
  }
  return result;
}

NAME_SET Pkb::callsStar(Procedure procedure, Underscore underscore) {
  return calls(procedure, underscore);
}

bool Pkb::callsStar(Underscore underscore, String procedureName) {
  return calls(underscore, procedureName);
}
NAME_SET Pkb::callsStar(Underscore underscore, Procedure procedure) {
  return calls(underscore, procedure);
}

bool Pkb::callsStar(Underscore underscore1, Underscore underscore2) {
  return calls(underscore1, underscore2);
}

// Query API for next

bool Pkb::next(LineNumber line1, LineNumber line2) {
  if (nextsTable.map.find(line1.number) != nextsTable.map.end()) {
    LINE_NOS nextLines = nextsTable.map[line1.number];
    return nextLines.find(line2.number) != nextLines.end();
  }
  return false;
}

LINE_SET Pkb::next(LineNumber line, Statement statement) {
  LINE_SET result;

  if (nextsTable.map.find(line.number) != nextsTable.map.end()) {
    LINE_NOS nextLines = nextsTable.map[line.number];

    if (!statement.type.has_value()) {
      result = nextLines;
    } else {
      for (LINE_NO next : nextLines) {
        if (statementTypeTable.map[next] == statement.type.value()) {
          result.insert(next);
        }
      }
    }
  }
  return result;
}

bool Pkb::next(LineNumber line, Underscore underscore) {
  if (nextsTable.map.find(line.number) != nextsTable.map.end()) {
    return nextsTable.map[line.number].size() > 0;
  }
  return false;
}

LINE_SET Pkb::next(Statement statement, LineNumber line) {
  LINE_SET result;

  if (invertNextsTable.map.find(line.number) != invertNextsTable.map.end()) {
    LINE_NOS prevLines = invertNextsTable.map[line.number];

    if (!statement.type.has_value()) {
      result = prevLines;
    } else {
      for (LINE_NO prev : prevLines) {
        if (statementTypeTable.map[prev] == statement.type.value()) {
          result.insert(prev);
        }
      }
    }
  }
  return result;
}

LINE_LINE_PAIRS Pkb::next(Statement statement1, Statement statement2) {
  LINE_LINE_PAIRS result;

  // case 1: both statements are stmts
  if (!statement1.type.has_value() && !statement2.type.has_value()) {
    for (auto entry : nextsTable.map) {
      for (NEXT next : entry.second) {
        result.first.push_back(entry.first);
        result.second.push_back(next);
      }
    }
  }

  // case 2: only statement1 is stmt
  else if (!statement1.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement2.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement2.type.value()];

      for (LINE_NO line : lines) {
        if (invertNextsTable.map.find(line) != invertNextsTable.map.end()) {
          LINE_NOS prevLines = invertNextsTable.map[line];

          for (LINE_NO prev : prevLines) {
            result.first.push_back(prev);
            result.second.push_back(line);
          }
        }
      }
    }
  }

  // case 3: only statement2 is stmt
  else if (!statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement1.type.value()];

      for (LINE_NO line : lines) {
        if (nextsTable.map.find(line) != nextsTable.map.end()) {
          NEXTS nextLines = nextsTable.map[line];

          for (NEXT next : nextLines) {
            result.first.push_back(line);
            result.second.push_back(next);
          }
        }
      }
    }
  }

  // case 4: both statements are not stmts
  else if (statement1.type.has_value() && statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement1.type.value()];

      for (LINE_NO line : lines) {
        if (nextsTable.map.find(line) != nextsTable.map.end()) {
          LINE_NOS nextLines = nextsTable.map[line];

          for (LINE_NO next : nextLines) {
            if (statementTypeTable.map[next] == statement2.type.value()) {
              result.first.push_back(line);
              result.second.push_back(next);
            }
          }
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::next(Statement statement, Underscore underscore) {
  LINE_SET result;

  if (!statement.type.has_value()) {
    result = nextTableIndexes;
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement.type.value()];

      for (LINE_NO line : lines) {
        if (nextsTable.map.find(line) != nextsTable.map.end()) {
          if (nextsTable.map[line].size() > 0) {
            result.insert(line);
          }
        }
      }
    }
  }
  return result;
}

bool Pkb::next(Underscore underscore, LineNumber line) {
  if (invertNextsTable.map.find(line.number) != invertNextsTable.map.end()) {
    return invertNextsTable.map[line.number].size() > 0;
  }
  return false;
}

LINE_SET Pkb::next(Underscore underscore, Statement statement) {
  LINE_SET result;

  if (!statement.type.has_value()) {
    result = invertNextsTableIndexes;
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement.type.value()];

      for (LINE_NO line : lines) {
        if (invertNextsTable.map.find(line) != invertNextsTable.map.end()) {
          if (invertNextsTable.map[line].size() > 0) {
            result.insert(line);
          }
        }
      }
    }
  }
  return result;
}

bool Pkb::next(Underscore underscore1, Underscore underscore2) {
  return nextsTable.size() > 0;
}

// Query API for nextStar

bool Pkb::nextStar(LineNumber line1, LineNumber line2) {
  NEXTS transitiveNexts = getTransitiveNextStatements(line1.number, {});

  // add to cache if it's not in cache
  if (closeNextsTableCache.map.find(line1.number) ==
      closeNextsTableCache.map.end()) {
    closeNextsTableCache.insert(std::pair(line1.number, transitiveNexts));
  }
  return transitiveNexts.find(line2.number) != transitiveNexts.end();
}

LINE_SET Pkb::nextStar(LineNumber line, Statement statement) {
  LINE_SET result;
  NEXTS transitiveNexts = getTransitiveNextStatements(line.number, {});

  // add to cache if it's not in cache
  if (closeNextsTableCache.map.find(line.number) ==
      closeNextsTableCache.map.end()) {
    closeNextsTableCache.insert(std::pair(line.number, transitiveNexts));
  }

  if (!statement.type.has_value()) {
    result = transitiveNexts;
  } else {
    for (NEXT next : transitiveNexts) {
      if (statementTypeTable.map[next] == statement.type.value()) {
        result.insert(next);
      }
    }
  }
  return result;
}

bool Pkb::nextStar(LineNumber line, Underscore underscore) {
  return next(line, underscore);
}

LINE_SET Pkb::nextStar(Statement statement, LineNumber line) {
  LINE_SET result;
  LINE_NOS transitivePrevs = getTransitivePrevStatements(line.number, {});

  // add to cache if it's not in cache
  if (closeInvertNextsTableCache.map.find(line.number) ==
      closeInvertNextsTableCache.map.end()) {
    closeInvertNextsTableCache.insert(std::pair(line.number, transitivePrevs));
  }

  if (!statement.type.has_value()) {
    result = transitivePrevs;
  } else {
    for (LINE_NO prev : transitivePrevs) {
      if (statementTypeTable.map[prev] == statement.type.value()) {
        result.insert(prev);
      }
    }
  }
  return result;
}

LINE_LINE_PAIRS Pkb::nextStar(Statement statement1, Statement statement2) {
  LINE_LINE_PAIRS result;

  // if closeNextsTableCache is not in cache, compute closeNextsTableCache and
  // cache it.
  if (!isCloseNextsTableCached) {
    for (auto entry : nextsTable.map) {
      // only need to compute and cache entry if its not already inside cache
      // table.
      if (closeNextsTableCache.map.find(entry.first) ==
          closeNextsTableCache.map.end()) {
        LINE_SET transitiveNexts = getTransitiveNextStatements(entry.first, {});
        closeNextsTableCache.insert(std::pair(entry.first, transitiveNexts));
      }
    }

    // if closeNextsTable is not in cache, then its inverse will not be in
    // cache too, so we compute and store.
    closeInvertNextsTableCache =
        PkbTableTransformers::pseudoinvertFlattenKeys<ASSIGNMENT, ASSIGNMENT>(
            closeNextsTableCache);

    isCloseNextsTableCached = true;
    isCloseInvertNextsTableCached = true;
  }

  // case 1: both statements are stmts
  if (!statement1.type.has_value() && !statement2.type.has_value()) {
    for (auto entry : closeNextsTableCache.map) {
      for (NEXT next : entry.second) {
        result.first.push_back(entry.first);
        result.second.push_back(next);
      }
    }
  }

  // case 2: only statement1 is stmt
  else if (!statement1.type.has_value()) {
    for (auto entry : closeNextsTableCache.map) {
      for (NEXT next : entry.second) {
        if (statementTypeTable.map[next] == statement2.type.value()) {
          result.first.push_back(entry.first);
          result.second.push_back(next);
        }
      }
    }
  }

  // case 3: only statement2 is stmt
  else if (!statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement1.type.value()];

      for (LINE_NO line : lines) {
        NEXTS nexts = closeNextsTableCache.map[line];

        for (NEXT next : nexts) {
          result.first.push_back(line);
          result.second.push_back(next);
        }
      }
    }
  }

  // case 4: both statements are not stmts
  else if (statement1.type.has_value() && statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement1.type.value()];

      for (LINE_NO line : lines) {
        NEXTS nexts = closeNextsTableCache.map[line];

        for (NEXT next : nexts) {
          if (statementTypeTable.map[next] == statement2.type.value()) {
            result.first.push_back(line);
            result.second.push_back(next);
          }
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::nextStar(Statement statement, Underscore underscore) {
  return next(statement, underscore);
}

bool Pkb::nextStar(Underscore underscore, LineNumber line) {
  return next(underscore, line);
}

LINE_SET Pkb::nextStar(Underscore underscore, Statement statement) {
  return next(underscore, statement);
}

bool Pkb::nextStar(Underscore underscore1, Underscore underscore2) {
  return next(underscore1, underscore2);
}

// HELPER FUNCTIONS FOR NEXTSTAR

LINE_SET Pkb::getTransitiveNextStatements(LINE_NO lineNo,
                                          LINE_NOS lineNosVisited) {
  LINE_SET result;

  // if transitiveNextStatements already cached, then retrieve from table
  if (closeNextsTableCache.map.find(lineNo) != closeNextsTableCache.map.end()) {
    result = closeNextsTableCache.map[lineNo];
  } else {
    if (nextsTable.map.find(lineNo) != nextsTable.map.end()) {
      NEXTS nexts = nextsTable.map[lineNo];

      for (NEXT next : nexts) {
        result.insert(next);
        if (lineNosVisited.find(next) == lineNosVisited.end()) {
          lineNosVisited.insert(next);
          LINE_SET transitiveStatements =
              getTransitiveNextStatements(next, lineNosVisited);
          result.merge(transitiveStatements);
          lineNosVisited.merge(transitiveStatements);
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::getTransitivePrevStatements(LINE_NO lineNo,
                                          LINE_NOS lineNosVisited) {
  LINE_SET result;

  // if transitivePrevStatements already cached, then retrieve from table
  if (closeInvertNextsTableCache.map.find(lineNo) !=
      closeInvertNextsTableCache.map.end()) {
    result = closeInvertNextsTableCache.map[lineNo];
  } else {
    if (invertNextsTable.map.find(lineNo) != invertNextsTable.map.end()) {
      LINE_NOS prevs = invertNextsTable.map[lineNo];

      for (LINE_NO prev : prevs) {
        result.insert(prev);
        if (lineNosVisited.find(prev) == lineNosVisited.end()) {
          lineNosVisited.insert(prev);
          LINE_SET transitiveStatements =
              getTransitivePrevStatements(prev, lineNosVisited);
          result.merge(transitiveStatements);
          lineNosVisited.merge(transitiveStatements);
        }
      }
    }
  }
  return result;
}

// Query API for affects

bool Pkb::affects(LineNumber line1, LineNumber line2) {
  LINE_SET affectedStatements = getAffectedStatements(line1.number);

  // add to cache if it's not in cache
  if (affectsTableCache.map.find(line1.number) == affectsTableCache.map.end()) {
    affectsTableCache.insert(std::pair(line1.number, affectedStatements));
  }

  return affectedStatements.find(line2.number) != affectedStatements.end();
}

LINE_SET Pkb::affects(LineNumber line, Statement statement) {
  LINE_SET result;
  if (!statement.type.has_value() ||
      statement.type.value() == StatementType::Assign) {
    result = getAffectedStatements(line.number);

    // add to cache if it's not in cache
    if (affectsTableCache.map.find(line.number) ==
        affectsTableCache.map.end()) {
      affectsTableCache.insert(std::pair(line.number, result));
    }
  }
  return result;
}

bool Pkb::affects(LineNumber line, Underscore underscore) {
  LINE_SET affectedStatements = getAffectedStatements(line.number);

  // add to cache if it's not in cache
  if (affectsTableCache.map.find(line.number) == affectsTableCache.map.end()) {
    affectsTableCache.insert(std::pair(line.number, affectedStatements));
  }

  return !affectedStatements.empty();
}

LINE_SET Pkb::affects(Statement statement, LineNumber line) {
  LINE_SET result;
  if (!statement.type.has_value() ||
      statement.type.value() == StatementType::Assign) {
    result = getAffectorStatements(line.number);

    // add to cache if it's not in cache
    if (invertAffectsTableCache.map.find(line.number) ==
        invertAffectsTableCache.map.end()) {
      invertAffectsTableCache.insert(std::pair(line.number, result));
    }
  }
  return result;
}

LINE_LINE_PAIRS Pkb::affects(Statement statement1, Statement statement2) {
  LINE_LINE_PAIRS result;

  if ((!statement1.type.has_value() ||
       statement1.type.value() == StatementType::Assign) &&
      (!statement2.type.has_value() ||
       statement2.type.value() == StatementType::Assign)) {
    if (invertStatementTypeTable.map.find(StatementType::Assign) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET assignments =
          invertStatementTypeTable.map[StatementType::Assign];

      // if affectsTable is not cached, compute table and cache it.
      if (!isAffectsTableCached) {
        for (LINE_NO line : assignments) {
          // only need to compute and cache entry if its not already inside
          // cache table.
          if (affectsTableCache.map.find(line) == affectsTableCache.map.end()) {
            LINE_SET affectedStatements = getAffectedStatements(line);
            affectsTableCache.insert(std::pair(line, affectedStatements));
          }
        }

        // if affectsTable is not cached, its inverse also won't be cached. So
        // compute and store it as well.
        invertAffectsTableCache = PkbTableTransformers::pseudoinvertFlattenKeys<
            ASSIGNMENT, ASSIGNMENT>(affectsTableCache);

        isAffectsTableCached = true;
        isInvertAffectsTableCached = true;
      }

      for (LINE_NO line : assignments) {
        LINE_SET affectedStatements = affectsTableCache.map[line];

        for (LINE_NO statement : affectedStatements) {
          result.first.push_back(line);
          result.second.push_back(statement);
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::affects(Statement statement, Underscore underscore) {
  LINE_SET result;

  if (!statement.type.has_value() ||
      statement.type.value() == StatementType::Assign) {
    if (invertStatementTypeTable.map.find(StatementType::Assign) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET assignments =
          invertStatementTypeTable.map[StatementType::Assign];

      for (LINE_NO line : assignments) {
        LINE_SET affectedStatements = getAffectedStatements(line);

        if (!affectedStatements.empty()) {
          result.insert(line);
        }

        // add into cache if its not in cache
        if (affectsTableCache.map.find(line) == affectsTableCache.map.end()) {
          affectsTableCache.insert(std::pair(line, affectedStatements));
        }
      }
    }
  }
  return result;
}

bool Pkb::affects(Underscore underscore, LineNumber line) {
  LINE_SET affectorStatements = getAffectorStatements(line.number);

  // add into cache if its not in cache
  if (invertAffectsTableCache.map.find(line.number) ==
      invertAffectsTableCache.map.end()) {
    invertAffectsTableCache.insert(std::pair(line.number, affectorStatements));
  }

  return !affectorStatements.empty();
}

LINE_SET Pkb::affects(Underscore underscore, Statement statement) {
  LINE_SET result;

  if (!statement.type.has_value() ||
      statement.type.value() == StatementType::Assign) {
    if (invertStatementTypeTable.map.find(StatementType::Assign) !=
        invertStatementTypeTable.map.end()) {
      LINE_SET assignments =
          invertStatementTypeTable.map[StatementType::Assign];

      for (LINE_NO line : assignments) {
        LINE_SET affectorStatements = getAffectorStatements(line);

        if (!affectorStatements.empty()) {
          result.insert(line);
        }

        // add into cache if its not in cache
        if (invertAffectsTableCache.map.find(line) ==
            invertAffectsTableCache.map.end()) {
          invertAffectsTableCache.insert(std::pair(line, affectorStatements));
        }
      }
    }
  }
  return result;
}

bool Pkb::affects(Underscore underscore1, Underscore underscore2) {
  if (invertStatementTypeTable.map.find(StatementType::Assign) !=
      invertStatementTypeTable.map.end()) {
    LINE_SET assignments = invertStatementTypeTable.map[StatementType::Assign];

    for (LINE_NO line : assignments) {
      LINE_SET affectedStatements = getAffectedStatements(line);

      // add into cache if its not in cache
      if (affectsTableCache.map.find(line) == affectsTableCache.map.end()) {
        affectsTableCache.insert(std::pair(line, affectedStatements));
      }

      if (!affectedStatements.empty()) {
        return true;
      }
    }
  }
  return false;
}

// HELPER FUNCTIONS FOR AFFECTS

LINE_SET Pkb::getAffectedStatements(LINE_NO lineNo) {
  LINE_SET result;

  if (statementTypeTable.map.find(lineNo) != statementTypeTable.map.end()) {
    if (statementTypeTable.map[lineNo] == StatementType::Assign) {
      // if affects statements for line has been cached, retrieve from table.
      if (affectsTableCache.map.find(lineNo) != affectsTableCache.map.end()) {
        result = affectsTableCache.map[lineNo];
      } else {
        // assignment statements can only modify 1 variable.
        VAR modifiedVar = *std::get<VARS>(modifiesTable.map[lineNo]).begin();
        NEXTS nexts = nextsTable.map[lineNo];
        for (NEXT next : nexts) {
          result.merge(getAffectedAux(modifiedVar, next, {}));
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::getAffectedAux(VAR modifiedVar, LINE_NO lineNo,
                             LINE_NOS lineNosVisited) {
  LINE_SET result;
  bool doesModifyModifiedVar = false;

  // Visit lineNo only if not already visited.
  if (lineNosVisited.find(lineNo) == lineNosVisited.end()) {
    lineNosVisited.insert(lineNo);
    StatementType statementType = statementTypeTable.map[lineNo];

    // Only add to result if lineNo is an assignment that uses the
    // modifiedVar.
    if (statementType == StatementType::Assign &&
        usesTable.map.find(lineNo) != usesTable.map.end()) {
      VARS varsUsedOnLine = std::get<VARS>(usesTable.map[lineNo]);
      if (varsUsedOnLine.find(modifiedVar) != varsUsedOnLine.end()) {
        result.insert(lineNo);
      }
    }

    if (modifiesTableTransited.map.find(lineNo) !=
        modifiesTableTransited.map.end()) {
      VARS modifiesVarsOnLine = modifiesTableTransited.map[lineNo];
      doesModifyModifiedVar =
          modifiesVarsOnLine.find(modifiedVar) != modifiesVarsOnLine.end();
    }

    /* Checking whether `lineNo` is an assign, read, or call, cannot be
    disentangled from checking whether `lineNo` modifies `modifiesVar`. These
    two checks cannot be written as two separate if-conditions, because the
    first if-condition skips over some `lineNo` when it should not.
    This:
    ```
    MODIFIES modifiesLineNo = modifiesTable.map[lineNo];
    VARS modifiesLineNoVars = std::get<VARS>(modifiesLineNo);
    if (modifiesLineNoVars.find(modifiesVar) != modifiesLineNoVars.end()) {
        StatementType statementType = statementTypeTable.map[lineNo];
        if (!((statementType == StatementType::Assign) ||
            (statementType == StatementType::Read) ||
            (statementType == StatementType::Call))) {
    ```
    misses `lineNo` if `lineNo` modifies `modifiesVar`, but is not an assign,
    read, or call. An example of such a `lineNo` is a while.
    This:
    ```
    StatementType statementType = statementTypeTable.map[lineNo];
    if (!((statementType == StatementType::Assign) ||
          (statementType == StatementType::Read) ||
          (statementType == StatementType::Call))) {
        MODIFIES modifiesLineNo = modifiesTable.map[lineNo];
        VARS modifiesLineNoVars = std::get<VARS>(modifiesLineNo);
        if (modifiesLineNoVars.find(modifiesVar) != modifiesLineNoVars.end()) {
    ```
    misses `lineNo` if `lineNo` is an assign, read, or call, but does not modify
    `modifiesVar`.
    So, these two checks must be written as a single if-condition. In the single
    if-condition, care must be taken not to have the two checks disentangled.
    So, the following is wrong for the same reasons given above:
    ```
    if ((modifiesLineNoVars.find(modifiesVar) != modifiesLineNoVars.end()) &&
        !((statementType == StatementType::Assign) ||
          (statementType == StatementType::Read) ||
          (statementType == StatementType::Call))) {
    ```
    */

    // Do not recurse if lineNo is of type Assign, Read or
    // Call that does modify the modifiedVar.
    if (!((statementType == StatementType::Assign && doesModifyModifiedVar) ||
          (statementType == StatementType::Read && doesModifyModifiedVar) ||
          (statementType == StatementType::Call && doesModifyModifiedVar))) {
      if (nextsTable.map.find(lineNo) != nextsTable.map.end()) {
        NEXTS nexts = nextsTable.map[lineNo];
        for (NEXT next : nexts) {
          result.merge(getAffectedAux(modifiedVar, next, lineNosVisited));
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::getAffectorStatements(LINE_NO lineNo) {
  LINE_SET result;

  if (statementTypeTable.map.find(lineNo) != statementTypeTable.map.end()) {
    if (statementTypeTable.map[lineNo] == StatementType::Assign &&
        usesTable.map.find(lineNo) != usesTable.map.end()) {
      // if affector statements for line has been cached, retrieve from table.
      if (invertAffectsTableCache.map.find(lineNo) !=
          invertAffectsTableCache.map.end()) {
        result = invertAffectsTableCache.map[lineNo];
      } else {
        VARS varsUsedOnLine = std::get<VARS>(usesTable.map[lineNo]);
        LINE_NOS prevs = invertNextsTable.map[lineNo];

        for (VAR var : varsUsedOnLine) {
          for (LINE_NO prev : prevs) {
            result.merge(getAffectorAux(var, prev, {}));
          }
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::getAffectorAux(VAR usedVar, LINE_NO lineNo,
                             LINE_NOS lineNosVisited) {
  LINE_SET result;
  bool doesModifyUsedVar = false;

  // Visit lineNo only if not already visited.
  if (lineNosVisited.find(lineNo) == lineNosVisited.end()) {
    lineNosVisited.insert(lineNo);
    StatementType statementType = statementTypeTable.map[lineNo];

    // Only add to result if lineNo is an assignment that modifies the
    // usedVar.
    if (statementType == StatementType::Assign &&
        modifiesTable.map.find(lineNo) != modifiesTable.map.end()) {
      // assignment statements can only modify 1 variable.
      VAR varModifiedOnLine =
          *std::get<VARS>(modifiesTable.map[lineNo]).begin();
      if (varModifiedOnLine == usedVar) {
        return LINE_SET{lineNo};
      }
    }

    if (modifiesTableTransited.map.find(lineNo) !=
        modifiesTableTransited.map.end()) {
      VARS modifiesVarsOnLine = modifiesTableTransited.map[lineNo];
      doesModifyUsedVar =
          modifiesVarsOnLine.find(usedVar) != modifiesVarsOnLine.end();
    }

    // Do not recurse if lineNo is of type Read or
    // Call that does modify the usedVar.
    // Recurse If lineNo is of type Assign: in recursed call, if lineNo modifies
    // usedVar, then terminate, else continue recursion.
    if (!((statementType == StatementType::Read && doesModifyUsedVar) ||
          (statementType == StatementType::Call && doesModifyUsedVar))) {
      if (invertNextsTable.map.find(lineNo) != invertNextsTable.map.end()) {
        LINE_NOS prevs = invertNextsTable.map[lineNo];
        for (LINE_NO prev : prevs) {
          result.merge(getAffectorAux(usedVar, prev, lineNosVisited));
        }
      }
    }
  }
  return result;
}

// Query API for affectsStar

bool Pkb::affectsStar(LineNumber line1, LineNumber line2) {
  LINE_SET transitiveAffectsStatements =
      getTransitiveAffectedStatements(line1.number, {});

  // add to cache if its not in cache
  if (closeAffectsTableCache.map.find(line1.number) ==
      closeAffectsTableCache.map.end()) {
    closeAffectsTableCache.insert(
        std::pair(line1.number, transitiveAffectsStatements));
  }

  return transitiveAffectsStatements.find(line2.number) !=
         transitiveAffectsStatements.end();
}

LINE_SET Pkb::affectsStar(LineNumber line, Statement statement) {
  LINE_SET result;
  if (!statement.type.has_value() ||
      statement.type.value() == StatementType::Assign) {
    result = getTransitiveAffectedStatements(line.number, {});

    // add to cache if its not in cache
    if (closeAffectsTableCache.map.find(line.number) ==
        closeAffectsTableCache.map.end()) {
      closeAffectsTableCache.insert(std::pair(line.number, result));
    }
  }
  return result;
}

bool Pkb::affectsStar(LineNumber line, Underscore underscore) {
  return affects(line, underscore);
}

LINE_SET Pkb::affectsStar(Statement statement, LineNumber line) {
  LINE_SET result;
  if (!statement.type.has_value() ||
      statement.type.value() == StatementType::Assign) {
    result = getTransitiveAffectorStatements(line.number, {});

    // add to cache if its not in cache
    if (closeInvertAffectsTableCache.map.find(line.number) ==
        closeInvertAffectsTableCache.map.end()) {
      closeInvertAffectsTableCache.insert(std::pair(line.number, result));
    }
  }
  return result;
}

LINE_LINE_PAIRS Pkb::affectsStar(Statement statement1, Statement statement2) {
  LINE_LINE_PAIRS result;

  if ((!statement1.type.has_value() ||
       statement1.type.value() == StatementType::Assign) &&
      (!statement2.type.has_value() ||
       statement2.type.value() == StatementType::Assign)) {
    if (invertStatementTypeTable.map.find(StatementType::Assign) !=
        invertStatementTypeTable.map.end()) {

      LINE_SET assignments =
          invertStatementTypeTable.map[StatementType::Assign];

      // if affectsTable is not cached, compute table and cache it.
      if (!isAffectsTableCached) {
        for (LINE_NO line : assignments) {
          // only need to compute and cache entry if its not already inside
          // cache table.
          if (affectsTableCache.map.find(line) == affectsTableCache.map.end()) {
            LINE_SET affectedStatements = getAffectedStatements(line);
            affectsTableCache.insert(std::pair(line, affectedStatements));
          }
        }

        // if affectsTable is not cached, its inverse also won't be cached. So
        // compute and store it as well.
        invertAffectsTableCache = PkbTableTransformers::pseudoinvertFlattenKeys<
            ASSIGNMENT, ASSIGNMENT>(affectsTableCache);

        isAffectsTableCached = true;
        isInvertAffectsTableCached = true;
      }

      // if closeAffectsTable is not in cache, compute closeAffectsTable and
      // cache it.
      if (!isCloseAffectsTableCached) {
        for (LINE_NO line : assignments) {
          // only need to compute and cache entry if its not already inside
          // cache table.
          if (closeAffectsTableCache.map.find(line) ==
              closeAffectsTableCache.map.end()) {
            LINE_SET transitiveAffectedStatements =
                getTransitiveAffectedStatements(line, {});
            closeAffectsTableCache.insert(
                std::pair(line, transitiveAffectedStatements));
          }
        }

        // if closeAffectsTable is not in cache, then its inverse will not be in
        // cache too, so we compute and store.
        closeInvertAffectsTableCache =
            PkbTableTransformers::pseudoinvertFlattenKeys<ASSIGNMENT,
                                                          ASSIGNMENT>(
                closeAffectsTableCache);

        isCloseAffectsTableCached = true;
        isCloseInvertAffectsTableCached = true;
      }

      for (auto entry : closeAffectsTableCache.map) {
        for (ASSIGNMENT assignment : entry.second) {
          result.first.push_back(entry.first);
          result.second.push_back(assignment);
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::affectsStar(Statement statement, Underscore underscore) {
  return affects(statement, underscore);
}

bool Pkb::affectsStar(Underscore underscore, LineNumber line) {
  return affects(underscore, line);
}

LINE_SET Pkb::affectsStar(Underscore underscore, Statement statement) {
  return affects(underscore, statement);
}

bool Pkb::affectsStar(Underscore underscore1, Underscore underscore2) {
  return affects(underscore1, underscore2);
}

// HELPER FUNCTIONS FOR AFFECTSSTAR

LINE_SET Pkb::getTransitiveAffectedStatements(LINE_NO lineNo,
                                              LINE_NOS lineNosVisited) {
  LINE_SET result;

  // if transitiveAffectedStatements already cached, then retrieve from table
  if (closeAffectsTableCache.map.find(lineNo) !=
      closeAffectsTableCache.map.end()) {
    result = closeAffectsTableCache.map[lineNo];
  } else {
    LINE_NOS affectedStatements = getAffectedStatements(lineNo);

    for (ASSIGNMENT assignment : affectedStatements) {
      result.insert(assignment);
      if (lineNosVisited.find(assignment) == lineNosVisited.end()) {
        lineNosVisited.insert(assignment);
        LINE_SET transitiveStatements =
            getTransitiveAffectedStatements(assignment, lineNosVisited);
        result.merge(transitiveStatements);
        lineNosVisited.merge(transitiveStatements);
      }
    }
  }
  return result;
}

LINE_SET Pkb::getTransitiveAffectorStatements(LINE_NO lineNo,
                                              LINE_NOS lineNosVisited) {
  LINE_SET result;

  // if transitiveAffectorStatements already cached, then retrieve from table
  if (closeInvertAffectsTableCache.map.find(lineNo) !=
      closeInvertAffectsTableCache.map.end()) {
    result = closeInvertAffectsTableCache.map[lineNo];
  } else {
    LINE_NOS affectorStatements = getAffectorStatements(lineNo);

    for (ASSIGNMENT assignment : affectorStatements) {
      result.insert(assignment);
      if (lineNosVisited.find(assignment) == lineNosVisited.end()) {
        lineNosVisited.insert(assignment);
        LINE_SET transitiveStatements =
            getTransitiveAffectorStatements(assignment, lineNosVisited);
        result.merge(transitiveStatements);
        lineNosVisited.merge(transitiveStatements);
      }
    }
  }
  return result;
}

// Query API for nextBip

bool Pkb::nextBip(LineNumber line1, LineNumber line2) {
  if (!areAllNextBipRelatedTablesDerived) {
    deriveAllNextBipRelatedTables();
  }

  if (nextBipTable.map.find(line1.number) != nextBipTable.map.end()) {
    LINE_NOS nextBipLines = nextBipTable.map[line1.number];
    return nextBipLines.find(line2.number) != nextBipLines.end();
  }
  return false;
}

LINE_SET Pkb::nextBip(LineNumber line, Statement statement) {
  if (!areAllNextBipRelatedTablesDerived) {
    deriveAllNextBipRelatedTables();
  }

  LINE_SET result;

  if (nextBipTable.map.find(line.number) != nextBipTable.map.end()) {
    LINE_NOS nextBipLines = nextBipTable.map[line.number];

    if (!statement.type.has_value()) {
      result = nextBipLines;
    } else {
      for (LINE_NO nextBip : nextBipLines) {
        if (statementTypeTable.map[nextBip] == statement.type.value()) {
          result.insert(nextBip);
        }
      }
    }
  }
  return result;
}

bool Pkb::nextBip(LineNumber line, Underscore underscore) {
  if (!areAllNextBipRelatedTablesDerived) {
    deriveAllNextBipRelatedTables();
  }

  if (nextBipTable.map.find(line.number) != nextBipTable.map.end()) {
    return nextBipTable.map[line.number].size() > 0;
  }
  return false;
}

LINE_SET Pkb::nextBip(Statement statement, LineNumber line) {
  if (!areAllNextBipRelatedTablesDerived) {
    deriveAllNextBipRelatedTables();
  }

  LINE_SET result;

  if (invertNextBipTable.map.find(line.number) !=
      invertNextBipTable.map.end()) {
    LINE_NOS prevBipLines = invertNextBipTable.map[line.number];

    if (!statement.type.has_value()) {
      result = prevBipLines;
    } else {
      for (LINE_NO prevBip : prevBipLines) {
        if (statementTypeTable.map[prevBip] == statement.type.value()) {
          result.insert(prevBip);
        }
      }
    }
  }
  return result;
}

LINE_LINE_PAIRS Pkb::nextBip(Statement statement1, Statement statement2) {
  if (!areAllNextBipRelatedTablesDerived) {
    deriveAllNextBipRelatedTables();
  }

  LINE_LINE_PAIRS result;

  // case 1: both statements are stmts
  if (!statement1.type.has_value() && !statement2.type.has_value()) {
    for (auto entry : nextBipTable.map) {
      for (NEXT nextBip : entry.second) {
        result.first.push_back(entry.first);
        result.second.push_back(nextBip);
      }
    }
  }

  // case 2: only statement1 is stmt
  else if (!statement1.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement2.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement2.type.value()];

      for (LINE_NO line : lines) {
        if (invertNextBipTable.map.find(line) != invertNextBipTable.map.end()) {
          LINE_NOS prevBipLines = invertNextBipTable.map[line];

          for (LINE_NO prevBip : prevBipLines) {
            result.first.push_back(prevBip);
            result.second.push_back(line);
          }
        }
      }
    }
  }

  // case 3: only statement2 is stmt
  else if (!statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement1.type.value()];

      for (LINE_NO line : lines) {
        if (nextBipTable.map.find(line) != nextBipTable.map.end()) {
          NEXTS nextBipLines = nextBipTable.map[line];

          for (NEXT nextBip : nextBipLines) {
            result.first.push_back(line);
            result.second.push_back(nextBip);
          }
        }
      }
    }
  }

  // case 4: both statements are not stmts
  else if (statement1.type.has_value() && statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement1.type.value()];

      for (LINE_NO line : lines) {
        if (nextBipTable.map.find(line) != nextBipTable.map.end()) {
          LINE_NOS nextBipLines = nextBipTable.map[line];

          for (LINE_NO nextBip : nextBipLines) {
            if (statementTypeTable.map[nextBip] == statement2.type.value()) {
              result.first.push_back(line);
              result.second.push_back(nextBip);
            }
          }
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::nextBip(Statement statement, Underscore underscore) {
  if (!areAllNextBipRelatedTablesDerived) {
    deriveAllNextBipRelatedTables();
  }

  LINE_SET result;

  if (!statement.type.has_value()) {
    result = nextBipTableIndexes;
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement.type.value()];

      for (LINE_NO line : lines) {
        if (nextBipTable.map.find(line) != nextBipTable.map.end()) {
          if (nextBipTable.map[line].size() > 0) {
            result.insert(line);
          }
        }
      }
    }
  }
  return result;
}

bool Pkb::nextBip(Underscore underscore, LineNumber line) {
  if (!areAllNextBipRelatedTablesDerived) {
    deriveAllNextBipRelatedTables();
  }

  if (invertNextBipTable.map.find(line.number) !=
      invertNextBipTable.map.end()) {
    return invertNextBipTable.map[line.number].size() > 0;
  }
  return false;
}

LINE_SET Pkb::nextBip(Underscore underscore, Statement statement) {
  if (!areAllNextBipRelatedTablesDerived) {
    deriveAllNextBipRelatedTables();
  }

  LINE_SET result;

  if (!statement.type.has_value()) {
    result = invertNextBipTableIndexes;
  } else {
    if (invertStatementTypeTable.map.find(statement.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement.type.value()];

      for (LINE_NO line : lines) {
        if (invertNextBipTable.map.find(line) != invertNextBipTable.map.end()) {
          if (invertNextBipTable.map[line].size() > 0) {
            result.insert(line);
          }
        }
      }
    }
  }
  return result;
}

bool Pkb::nextBip(Underscore underscore1, Underscore underscore2) {
  if (!areAllNextBipRelatedTablesDerived) {
    deriveAllNextBipRelatedTables();
  }

  return nextBipTable.size() > 0;
}

// Query API for nextBipStar

bool Pkb::nextBipStar(LineNumber line1, LineNumber line2) {
  if (!areAllCloseNextBipRelatedTablesDerived) {
    deriveAllCloseNextBipRelatedTables();
  }

  if (closeNextBipTable.map.find(line1.number) != closeNextBipTable.map.end()) {
    NEXTS transitiveNextBips = closeNextBipTable.map[line1.number];
    return transitiveNextBips.find(line2.number) != transitiveNextBips.end();
  }
  return false;
}

LINE_SET Pkb::nextBipStar(LineNumber line, Statement statement) {
  if (!areAllCloseNextBipRelatedTablesDerived) {
    deriveAllCloseNextBipRelatedTables();
  }

  LINE_SET result;

  if (closeNextBipTable.map.find(line.number) != closeNextBipTable.map.end()) {
    NEXTS transitiveNextBips = closeNextBipTable.map[line.number];

    if (!statement.type.has_value()) {
      result = transitiveNextBips;
    } else {
      for (NEXT nextBip : transitiveNextBips) {
        if (statementTypeTable.map[nextBip] == statement.type.value()) {
          result.insert(nextBip);
        }
      }
    }
  }
  return result;
}

bool Pkb::nextBipStar(LineNumber line, Underscore underscore) {
  return nextBip(line, underscore);
}

LINE_SET Pkb::nextBipStar(Statement statement, LineNumber line) {
  if (!areAllCloseNextBipRelatedTablesDerived) {
    deriveAllCloseNextBipRelatedTables();
  }

  LINE_SET result;

  if (closeInvertNextBipTable.map.find(line.number) !=
      closeInvertNextBipTable.map.end()) {
    LINE_NOS transitivePrevBips = closeInvertNextBipTable.map[line.number];

    if (!statement.type.has_value()) {
      result = transitivePrevBips;
    } else {
      for (LINE_NO prevBip : transitivePrevBips) {
        if (statementTypeTable.map[prevBip] == statement.type.value()) {
          result.insert(prevBip);
        }
      }
    }
  }
  return result;
}

LINE_LINE_PAIRS Pkb::nextBipStar(Statement statement1, Statement statement2) {
  if (!areAllCloseNextBipRelatedTablesDerived) {
    deriveAllCloseNextBipRelatedTables();
  }

  LINE_LINE_PAIRS result;

  // case 1: both statements are stmts
  if (!statement1.type.has_value() && !statement2.type.has_value()) {
    for (auto entry : closeNextBipTable.map) {
      for (NEXT nextBip : entry.second) {
        result.first.push_back(entry.first);
        result.second.push_back(nextBip);
      }
    }
  }

  // case 2: only statement1 is stmt
  else if (!statement1.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement2.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement2.type.value()];

      for (LINE_NO line : lines) {
        if (closeInvertNextBipTable.map.find(line) !=
            closeInvertNextBipTable.map.end()) {
          LINE_NOS prevBipLines = closeInvertNextBipTable.map[line];

          for (LINE_NO prevBip : prevBipLines) {
            result.first.push_back(prevBip);
            result.second.push_back(line);
          }
        }
      }
    }
  }

  // case 3: only statement2 is stmt
  else if (!statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement1.type.value()];

      for (LINE_NO line : lines) {
        if (closeNextBipTable.map.find(line) != closeNextBipTable.map.end()) {
          NEXTS transitiveNextBips = closeNextBipTable.map[line];

          for (NEXT nextBip : transitiveNextBips) {
            result.first.push_back(line);
            result.second.push_back(nextBip);
          }
        }
      }
    }
  }

  // case 4: both statements are not stmts
  else if (statement1.type.has_value() && statement2.type.has_value()) {
    if (invertStatementTypeTable.map.find(statement1.type.value()) !=
        invertStatementTypeTable.map.end()) {
      LINE_NOS lines = invertStatementTypeTable.map[statement1.type.value()];

      for (LINE_NO line : lines) {
        if (closeNextBipTable.map.find(line) != closeNextBipTable.map.end()) {
          NEXTS transitiveNextBips = closeNextBipTable.map[line];

          for (NEXT nextBip : transitiveNextBips) {
            if (statementTypeTable.map[nextBip] == statement2.type.value()) {
              result.first.push_back(line);
              result.second.push_back(nextBip);
            }
          }
        }
      }
    }
  }
  return result;
}

LINE_SET Pkb::nextBipStar(Statement statement, Underscore underscore) {
  return nextBip(statement, underscore);
}

bool Pkb::nextBipStar(Underscore underscore, LineNumber line) {
  return nextBip(underscore, line);
}

LINE_SET Pkb::nextBipStar(Underscore underscore, Statement statement) {
  return nextBip(underscore, statement);
}

bool Pkb::nextBipStar(Underscore underscore1, Underscore underscore2) {
  return nextBip(underscore1, underscore2);
}
