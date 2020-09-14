#pragma once

#include "PkbQueryInterface.h"
#include "Pkb.h"

// Query API for pattern matching
// Result<StmtNumberAndStringPairs> PkbQueryInterface::match(Statement
// statement,
//                                                Variable variable,
//                                                PatternSpec spec) {}
// STRING_SET PkbQueryInterface::match(Statement statement,
//                                                Underscore underscore,
//                                                PatternSpec spec) {}
// Need tests
STRING_SET PkbQueryInterface::match(Statement statement, String variable,
                                    PatternSpec spec) {
  STRING_SET result;
  VAR_TABLE_INDEX inputVarTableIndex = varTable.map[variable.name];

  // if input variable name doesn't exist in varTable.
  if (inputVarTableIndex == VAR_TABLE_INDEX()) {
    return result;
  }

  if (statement.type == StatementType::ASSIGN) {
    STRING_SET linesVarModifies = invertModifiesTable.map[inputVarTableIndex];

    for (auto line : linesVarModifies) {
      if (statementTypeTable.map[line] == StatementType::ASSIGN) {
        AST astOnLine = assignAstTable.map[line];

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
STRING_SET PkbQueryInterface::select(Variable var) { return varNamesSet; }
// Need tests
STRING_SET PkbQueryInterface::select(Statement statement) {
  if (statement.type == StatementType::NONE) {
    return stmtTableIndexes;
  } else {
    return invertStatementTypeTable.map[statement.type];
  }
}
STRING_SET PkbQueryInterface::select(Procedure proc) { return procNamesSet; }

// Query API for follows

bool PkbQueryInterface::follows(LineNumber line1, LineNumber line2) {
  bool result = followTable.map[line1.number] == line2.number;
  return result;
}

STRING_SET PkbQueryInterface::follows(LineNumber line, Statement statement) {
  FOLLOW followLine = followTable.map[line.number];

  if (statement.type == StatementType::NONE) {
    return followLine != LINE_NO() ? STRING_SET{followLine} : STRING_SET();
  } else {
    StatementType type = statementTypeTable.map[followLine];
    return followLine != LINE_NO() && statement.type == type
               ? STRING_SET{followLine}
               : STRING_SET();
  }
}

bool PkbQueryInterface::follows(LineNumber line, Underscore underscore) {
  FOLLOW followLine = followTable.map[line.number];
  return followLine != LINE_NO();
}

STRING_SET PkbQueryInterface::follows(Statement statement, LineNumber line) {
  LINE_NO prevLine = prevLineTable.map[line.number];

  if (statement.type == StatementType::NONE) {
    return prevLine != LINE_NO() ? STRING_SET{prevLine} : STRING_SET();
  } else {
    StatementType type = statementTypeTable.map[prevLine];

    return prevLine != LINE_NO() && statement.type == type
               ? STRING_SET{prevLine}
               : STRING_SET();
  }
}

STRING_PAIRS PkbQueryInterface::follows(Statement statement1,
                                        Statement statement2) {
  STRING_PAIRS result;

  // case 1: both statements are of type STMT
  if (statement1.type == StatementType::NONE &&
      statement2.type == StatementType::NONE) {
    result.first = followTable.keys;
    result.second = prevLineTable.keys;
  }

  // case 2: only statement1 is of type STMT
  else if (statement1.type == StatementType::NONE) {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement2.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      LINE_NO prevLine = prevLineTable.map[*it];
      if (prevLine != LINE_NO()) {
        result.first.push_back(prevLine);
        result.second.push_back(*it);
      }
      it++;
    }
  }

  // case 3: only statement2 is of type STMT
  else if (statement2.type == StatementType::NONE) {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement1.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      FOLLOW followLine = followTable.map[*it];
      if (followLine != LINE_NO()) {
        result.first.push_back(*it);
        result.second.push_back(followLine);
      }
      it++;
    }
  }

  // case 4: both statements are not of type STMT
  else if (statement1.type != StatementType::NONE &&
           statement2.type != StatementType::NONE) {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement1.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      FOLLOW followLine = followTable.map[*it];
      StatementType type = statementTypeTable.map[followLine];
      if (followLine != LINE_NO() && type == statement2.type) {
        result.first.push_back(*it);
        result.second.push_back(followLine);
      }
      it++;
    }
  }

  return result;
}

STRING_SET PkbQueryInterface::follows(Statement statement,
                                      Underscore underscore) {
  if (statement.type == StatementType::NONE) {
    return followTableIndexes;
  } else {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      if (followTable.map[*it] == LINE_NO()) {
        it = lineNumbers.erase(it);
      } else {
        it++;
      }
    }

    return lineNumbers;
  }
}

bool PkbQueryInterface::follows(Underscore underscore, LineNumber line) {
  LINE_NO prevLine = prevLineTable.map[line.number];
  return prevLine != LINE_NO();
}

STRING_SET PkbQueryInterface::follows(Underscore underscore,
                                      Statement statement) {
  if (statement.type == StatementType::NONE) {
    return prevLineTableIndexes;
  } else {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      if (prevLineTable.map[*it] == LINE_NO()) {
        it = lineNumbers.erase(it);
      } else {
        it++;
      }
    }
    return lineNumbers;
  }
}

bool PkbQueryInterface::follows(Underscore underscore1,
                                Underscore underscore2) {
  return followTable.size() > 0;
}

// Query API for followsStar

// Need tests
bool PkbQueryInterface::followsStar(LineNumber line1, LineNumber line2) {
  FOLLOWS followLines = closeFollowTable.map[line1.number];
  if (std::find(followLines.begin(), followLines.end(), line2.number) !=
      followLines.end()) { // update to find on sets
    return true;
  } else {
    return false;
  }
}
// Need tests
STRING_SET PkbQueryInterface::followsStar(LineNumber line,
                                          Statement statement) {
  STRING_SET followLines = closeFollowTable.map[line.number];

  if (statement.type == StatementType::NONE) {
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
// Need tests
STRING_SET PkbQueryInterface::followsStar(LineNumber line, Stmt stmt) {
  STRING_SET followLines = closeFollowTable.map[line.number];
  return followLines;
}

// Need tests
bool PkbQueryInterface::followsStar(LineNumber line, Underscore underscore) {
  return follows(line, underscore);
}

// Need test
bool PkbQueryInterface::followsStar(Underscore underscore, LineNumber line) {
  return follows(underscore, line);
}

STRING_PAIRS PkbQueryInterface::followsStar(Stmt stmt, Statement statement) {
  STRING_PAIRS result;
  STRING_SET statementLines = invertStatementTypeTable.map[statement.type];

  STRING_SET::iterator it = statementLines.begin();

  while (it != statementLines.end()) {
    STRING_SET prevLines = closePrevLineTable.map[*it];
    STRING_SET::iterator prevLineIt = prevLines.begin();

    while (prevLineIt != prevLines.end()) {
      result.first.push_back(*prevLineIt);
      result.second.push_back(*it);
      prevLineIt++;
    }
    it++;
  }
  return result;
}

// Query API for parent

bool PkbQueryInterface::parent(LineNumber line1, LineNumber line2) {
  bool result = parentTable.map[line2.number] == line1.number;
  return result;
}

STRING_SET PkbQueryInterface::parent(LineNumber line, Statement statement) {

  STRING_SET children = childrenTable.map[line.number];

  if (statement.type == StatementType::NONE) {
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

bool PkbQueryInterface::parent(LineNumber line, Underscore underscore) {
  STRING_SET children = childrenTable.map[line.number];
  return children.size() > 0;
}

STRING_SET PkbQueryInterface::parent(Statement statement, LineNumber line) {
  PARENT parent = parentTable.map[line.number];
  if (statement.type == StatementType::NONE) {
    return parent != LINE_NO() ? STRING_SET{parent} : STRING_SET();
  } else {
    StatementType type = statementTypeTable.map[parent];

    return parent != LINE_NO() && type == statement.type ? STRING_SET{parent}
                                                         : STRING_SET();
  }
}

STRING_PAIRS PkbQueryInterface::parent(Statement statement1,
                                       Statement statement2) {
  STRING_PAIRS result;

  // case 1: both statements are of type STMT
  if (statement1.type == StatementType::NONE &&
      statement2.type == StatementType::NONE) {
    STRING_SET parents = childrenTableIndexes;
    STRING_SET::iterator parentIt = parents.begin();

    while (parentIt != parents.end()) {
      CHILDREN children = childrenTable.map[*parentIt];
      STRING_SET::iterator childIt = children.begin();

      while (childIt != children.end()) {
        result.first.push_back(*parentIt);
        result.second.push_back(*childIt);
        childIt++;
      }

      parentIt++;
    }
  }

  // case 2: only statement1 is of type STMT
  else if (statement1.type == StatementType::NONE) {
    STRING_SET lineNumbers = invertStatementTypeTable.map[statement2.type];
    STRING_SET::iterator it = lineNumbers.begin();

    while (it != lineNumbers.end()) {
      PARENT parent = parentTable.map[*it];
      if (parent != LINE_NO()) {
        result.first.push_back(parent);
        result.second.push_back(*it);
      }
      it++;
    }
  }

  // case 3: only statement2 is of type STMT
  else if (statement2.type == StatementType::NONE) {
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

  // case 4: both statements are not of type STMT
  else if (statement1.type != StatementType::NONE &&
           statement2.type != StatementType::NONE) {
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

STRING_SET PkbQueryInterface::parent(Statement statement,
                                     Underscore underscore) {
  if (statement.type == StatementType::NONE) {
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

bool PkbQueryInterface::parent(Underscore underscore, LineNumber line) {
  PARENT parent = parentTable.map[line.number];
  return parent != LINE_NO();
}

STRING_SET PkbQueryInterface::parent(Underscore underscore,
                                     Statement statement) {
  if (statement.type == StatementType::NONE) {
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

bool PkbQueryInterface::parent(Underscore underscore1, Underscore underscore2) {
  return childrenTable.map.size() > 0;
}

// Query API for parentStar
// Need tests
bool PkbQueryInterface::parentStar(LineNumber line1, LineNumber line2) {
  PARENTS parentLines = closeParentTable.map[line2.number];
  if (std::find(parentLines.begin(), parentLines.end(), line1.number) !=
      parentLines.end()) {
    return true;
  } else {
    return false;
  }
}

// Need tests
bool PkbQueryInterface::parentStar(LineNumber line, Underscore underscore) {
  return parent(line, underscore);
}
// Need tests
bool PkbQueryInterface::parentStar(Underscore underscore, LineNumber line) {
  return parent(underscore, line);
}

// Query API for uses

bool PkbQueryInterface::uses(LineNumber line, String variable) {
  VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  STRING_SET usesLines = invertUsesTable.map[inputVarIndex];
  return usesLines.find(line.number) != usesLines.end();
}

STRING_SET PkbQueryInterface::uses(LineNumber line, Variable variable) {
  STRING_SET result;
  VAR_TABLE_INDEXES varIndexes = usesTableTransited.map[line.number];

  for (VAR_TABLE_INDEX index : varIndexes) {
    result.insert(invertVarTable.map[index]);
  }
  return result;
}

bool PkbQueryInterface::uses(LineNumber line, Underscore underscore) {
  return usesTableTransited.map[line.number].size() > 0;
}

STRING_SET PkbQueryInterface::uses(Statement statement, String variable) {
  VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  if (statement.type == StatementType::NONE) {
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

STRING_PAIRS PkbQueryInterface::uses(Statement statement, Variable variable) {
  STRING_PAIRS result;

  if (statement.type == StatementType::NONE) {
    for (auto entry : usesTableTransited.map) {
      LINE_NO line = entry.first;

      for (auto varIndex : entry.second) {
        result.first.push_back(line);
        result.second.push_back(invertVarTable.map[varIndex]);
      }
    }
  } else {
    STRING_SET statementLines = invertStatementTypeTable.map[statement.type];

    for (auto line : statementLines) {
      VAR_TABLE_INDEXES varIndexes = usesTableTransited.map[line];

      for (auto index : varIndexes) {
        result.first.push_back(line);
        result.second.push_back(invertVarTable.map[index]);
      }
    }
  }
  return result;
}

STRING_SET PkbQueryInterface::uses(Statement statement, Underscore underscore) {
  STRING_SET result;

  if (statement.type == StatementType::NONE) {
    for (auto entry : usesTableTransited.map) {
      if (entry.second.size() > 0) {
        result.insert(entry.first);
      }
    }
  } else {
    STRING_SET statementLines = invertStatementTypeTable.map[statement.type];

    for (LINE_NO line : statementLines) {
      if (usesTableTransited.map[line].size() > 0) {
        result.insert(line);
      }
    }
  }
  return result;
}

bool PkbQueryInterface::uses(String procedure, String variable) {
  PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];
  VAR_TABLE_INDEXES varIndexesUsedInProc = usesProcTable.map[inputProcIndex];

  return varIndexesUsedInProc.find(inputVarIndex) != varIndexesUsedInProc.end();
}

STRING_SET PkbQueryInterface::uses(String procedure, Variable variable) {
  STRING_SET result;
  PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  VAR_TABLE_INDEXES varIndexesUsedInProc = usesProcTable.map[inputProcIndex];

  for (VAR_TABLE_INDEX index : varIndexesUsedInProc) {
    result.insert(invertVarTable.map[index]);
  }
  return result;
}

bool PkbQueryInterface::uses(String procedure, Underscore underscore) {
  PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  return usesProcTable.map[inputProcIndex].size() > 0;
}

STRING_SET PkbQueryInterface::uses(Procedure procedure, String variable) {
  STRING_SET result;
  VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  // if input variable doesn't exist in varTable
  if (inputVarIndex == VAR_TABLE_INDEX()) {
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

STRING_PAIRS PkbQueryInterface::uses(Procedure procedure, Variable variable) {
  STRING_PAIRS result;

  for (auto entry : usesProcTable.map) {
    VAR_TABLE_INDEXES varIndexesInProc = entry.second;

    for (auto index : varIndexesInProc) {
      result.first.push_back(invertProcTable.map[entry.first]);
      result.second.push_back(invertVarTable.map[index]);
    }
  }
  return result;
}

STRING_SET PkbQueryInterface::uses(Procedure procedure, Underscore underscore) {
  STRING_SET result;

  for (auto entry : usesProcTable.map) {
    if (entry.second.size() > 0) {
      result.insert(invertProcTable.map[entry.first]);
    }
  }
  return result;
}

// Query API for modifies

bool PkbQueryInterface::modifies(LineNumber line, String variable) {
  VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  STRING_SET modifiesLines = invertModifiesTable.map[inputVarIndex];
  return modifiesLines.find(line.number) != modifiesLines.end();
}

STRING_SET PkbQueryInterface::modifies(LineNumber line, Variable variable) {
  STRING_SET result;
  VAR_TABLE_INDEXES varIndexes = modifiesTableTransited.map[line.number];

  for (VAR_TABLE_INDEX index : varIndexes) {
    result.insert(invertVarTable.map[index]);
  }
  return result;
}

bool PkbQueryInterface::modifies(LineNumber line, Underscore underscore) {
  return modifiesTableTransited.map[line.number].size() > 0;
}

STRING_SET PkbQueryInterface::modifies(Statement statement, String variable) {
  VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  if (statement.type == StatementType::NONE) {
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

STRING_PAIRS PkbQueryInterface::modifies(Statement statement,
                                         Variable variable) {
  STRING_PAIRS result;

  if (statement.type == StatementType::NONE) {
    for (auto entry : modifiesTableTransited.map) {
      LINE_NO line = entry.first;

      for (auto varIndex : entry.second) {
        result.first.push_back(line);
        result.second.push_back(invertVarTable.map[varIndex]);
      }
    }
  } else {
    STRING_SET statementLines = invertStatementTypeTable.map[statement.type];

    for (auto line : statementLines) {
      VAR_TABLE_INDEXES varIndexes = modifiesTableTransited.map[line];

      for (auto index : varIndexes) {
        result.first.push_back(line);
        result.second.push_back(invertVarTable.map[index]);
      }
    }
  }
  return result;
}

STRING_SET PkbQueryInterface::modifies(Statement statement,
                                       Underscore underscore) {
  STRING_SET result;

  if (statement.type == StatementType::NONE) {
    for (auto entry : modifiesTableTransited.map) {
      if (entry.second.size() > 0) {
        result.insert(entry.first);
      }
    }
  } else {
    STRING_SET statementLines = invertStatementTypeTable.map[statement.type];

    for (LINE_NO line : statementLines) {
      if (modifiesTableTransited.map[line].size() > 0) {
        result.insert(line);
      }
    }
  }
  return result;
}

bool PkbQueryInterface::modifies(String procedure, String variable) {
  PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];
  VAR_TABLE_INDEXES varIndexesModifiedInProc =
      modifiesProcTable.map[inputProcIndex];

  return varIndexesModifiedInProc.find(inputVarIndex) !=
         varIndexesModifiedInProc.end();
}

STRING_SET PkbQueryInterface::modifies(String procedure, Variable variable) {
  STRING_SET result;
  PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  VAR_TABLE_INDEXES varIndexesModifiesInProc =
      modifiesProcTable.map[inputProcIndex];

  for (VAR_TABLE_INDEX index : varIndexesModifiesInProc) {
    result.insert(invertVarTable.map[index]);
  }
  return result;
}

bool PkbQueryInterface::modifies(String procedure, Underscore underscore) {
  PROC_TABLE_INDEX inputProcIndex = procTable.map[procedure.name];
  return modifiesProcTable.map[inputProcIndex].size() > 0;
}

STRING_SET PkbQueryInterface::modifies(Procedure procedure, String variable) {
  STRING_SET result;
  VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];

  // if input variable doesn't exist in varTable
  if (inputVarIndex == VAR_TABLE_INDEX()) {
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

STRING_PAIRS PkbQueryInterface::modifies(Procedure procedure,
                                         Variable variable) {
  STRING_PAIRS result;

  for (auto entry : modifiesProcTable.map) {
    VAR_TABLE_INDEXES varIndexesInProc = entry.second;

    for (auto index : varIndexesInProc) {
      result.first.push_back(invertProcTable.map[entry.first]);
      result.second.push_back(invertVarTable.map[index]);
    }
  }
  return result;
}

STRING_SET PkbQueryInterface::modifies(Procedure procedure,
                                       Underscore underscore) {
  STRING_SET result;

  for (auto entry : modifiesProcTable.map) {
    if (entry.second.size() > 0) {
      result.insert(invertProcTable.map[entry.first]);
    }
  }
  return result;
}
