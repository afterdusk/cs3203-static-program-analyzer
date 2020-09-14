#pragma once

#include "PkbQueryInterface.h"
#include "Pkb.h"

// Query API for pattern matching
STRING_PAIRS PkbQueryInterface::match(Statement statement, Variable variable,
                                      PatternSpec spec) {
  STRING_PAIRS result;

  if (statement.type == StatementType::ASSIGN) {
    STRING_SET assignLines = invertStatementTypeTable.map[statement.type];

    if (spec.type == PatternMatchType::Any) {
      for (auto line : assignLines) {
        VAR_TABLE_INDEXES varIndexes =
            std::get<VAR_TABLE_INDEXES>(modifiesTable.map[line]);

        // Since line is an assignment statement, there must be only one index
        // in varIndexes.
        VAR_TABLE_INDEX modifiedVarIndex = *varIndexes.begin();

        result.first.push_back(line);
        result.second.push_back(invertVarTable.map[modifiedVarIndex]);
      }
    } else if (spec.type == PatternMatchType::CompleteMatch) {
      for (auto line : assignLines) {
        AST astOnLine = assignAstTable.map[line];

        if (astOnLine == *spec.value) {
          VAR_TABLE_INDEXES varIndexes =
              std::get<VAR_TABLE_INDEXES>(modifiesTable.map[line]);

          // Since line is an assignment statement, there must be only one index
          // in varIndexes.
          VAR_TABLE_INDEX modifiedVarIndex = *varIndexes.begin();

          result.first.push_back(line);
          result.second.push_back(invertVarTable.map[modifiedVarIndex]);
        }
      }
    } else if (spec.type == PatternMatchType::SubTreeMatch) {
      for (auto line : assignLines) {
        AST astOnLine = assignAstTable.map[line];

        if (astOnLine >= *spec.value) {
          VAR_TABLE_INDEXES varIndexes =
              std::get<VAR_TABLE_INDEXES>(modifiesTable.map[line]);

          // Since line is an assignment statement, there must be only one index
          // in varIndexes.
          VAR_TABLE_INDEX modifiedVarIndex = *varIndexes.begin();

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

STRING_SET PkbQueryInterface::match(Statement statement, Underscore underscore,
                                    PatternSpec spec) {
  STRING_SET result;

  if (statement.type == StatementType::ASSIGN) {
    STRING_SET assignLines = invertStatementTypeTable.map[statement.type];

    if (spec.type == PatternMatchType::Any) {
      // if no matching is required, then return assignment lines without
      // processing.
      return assignLines;
    } else {
      for (auto line : assignLines) {
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
          // if pattern spec type is not assigned, return an empty set.
          // (update) throw exception instead when exception format is received
          return result;
        }
      }
    }
  }

  return result;
}

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

STRING_SET PkbQueryInterface::select(Constant constant) { return constTable; }

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

  // case 1: both statements are of type NONE
  if (statement1.type == StatementType::NONE &&
      statement2.type == StatementType::NONE) {
    result.first = followTable.keys;
    result.second = prevLineTable.keys;
  }

  // case 2: only statement1 is of type NONE
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

  // case 3: only statement2 is of type NONE
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

  // case 4: both statements are not of type NONE
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

bool PkbQueryInterface::followsStar(LineNumber line1, LineNumber line2) {
  FOLLOWS followLines = closeFollowTable.map[line1.number];
  return followLines.find(line2.number) != followLines.end();
}

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

bool PkbQueryInterface::followsStar(LineNumber line, Underscore underscore) {
  return follows(line, underscore);
}

STRING_SET PkbQueryInterface::followsStar(Statement statement,
                                          LineNumber line) {
  STRING_SET prevLines = closePrevLineTable.map[line.number];

  if (statement.type == StatementType::NONE) {
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

STRING_PAIRS PkbQueryInterface::followsStar(Statement statement1,
                                            Statement statement2) {
  STRING_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == StatementType::NONE &&
      statement2.type == StatementType::NONE) {
    for (auto entry : closeFollowTable.map) {
      LINE_NO line = entry.first;

      for (auto followLine : entry.second) {
        result.first.push_back(line);
        result.second.push_back(followLine);
      }
    }
  }

  // case 2: only statement1 is of type NONE
  else if (statement1.type == StatementType::NONE) {
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
  else if (statement2.type == StatementType::NONE) {
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
  else if (statement1.type != StatementType::NONE &&
           statement2.type != StatementType::NONE) {
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

STRING_SET PkbQueryInterface::followsStar(Statement statement,
                                          Underscore underscore) {
  return follows(statement, underscore);
}

bool PkbQueryInterface::followsStar(Underscore underscore, LineNumber line) {
  return follows(underscore, line);
}

STRING_SET PkbQueryInterface::followsStar(Underscore underscore,
                                          Statement statement) {
  return follows(underscore, statement);
}

bool PkbQueryInterface::followsStar(Underscore underscore1,
                                    Underscore underscore2) {
  return follows(underscore1, underscore2);
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

  // case 1: both statements are of type NONE
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

  // case 2: only statement1 is of type NONE
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

  // case 3: only statement2 is of type NONE
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

  // case 4: both statements are not of type NONE
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
  return parentTable.map.size() > 0;
}

// Query API for parentStar

bool PkbQueryInterface::parentStar(LineNumber line1, LineNumber line2) {
  PARENTS parentLines = closeParentTable.map[line2.number];
  return parentLines.find(line1.number) != parentLines.end();
}

STRING_SET PkbQueryInterface::parentStar(LineNumber line, Statement statement) {
  STRING_SET descendantLines = closeChildrenTable.map[line.number];

  if (statement.type == StatementType::NONE) {
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

bool PkbQueryInterface::parentStar(LineNumber line, Underscore underscore) {
  return parent(line, underscore);
}

STRING_SET PkbQueryInterface::parentStar(Statement statement, LineNumber line) {
  PARENTS parentLines = closeParentTable.map[line.number];

  if (statement.type == StatementType::NONE) {
    return parentLines;
  } else {
    PARENTS::iterator it = parentLines.begin();
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

STRING_PAIRS PkbQueryInterface::parentStar(Statement statement1,
                                           Statement statement2) {
  STRING_PAIRS result;

  // case 1: both statements are of type NONE
  if (statement1.type == StatementType::NONE &&
      statement2.type == StatementType::NONE) {
    for (auto entry : closeChildrenTable.map) {
      PARENT parentLine = entry.first;

      for (auto descendant : entry.second) {
        result.first.push_back(parentLine);
        result.second.push_back(descendant);
      }
    }
  }

  // case 2: only statement1 is of type NONE
  else if (statement1.type == StatementType::NONE) {
    STRING_SET statementLines = invertStatementTypeTable.map[statement2.type];

    for (auto child : statementLines) {
      PARENTS ancestors = closeParentTable.map[child];

      for (auto ancestor : ancestors) {
        result.first.push_back(ancestor);
        result.second.push_back(child);
      }
    }
  }

  // case 3: only statement2 is of type NONE
  else if (statement2.type == StatementType::NONE) {
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
  else if (statement1.type != StatementType::NONE &&
           statement2.type != StatementType::NONE) {
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

STRING_SET PkbQueryInterface::parentStar(Statement statement,
                                         Underscore underscore) {
  return parent(statement, underscore);
}

bool PkbQueryInterface::parentStar(Underscore underscore, LineNumber line) {
  return parent(underscore, line);
}

STRING_SET PkbQueryInterface::parentStar(Underscore underscore,
                                         Statement statement) {
  return parent(underscore, statement);
}

bool PkbQueryInterface::parentStar(Underscore underscore1,
                                   Underscore underscore2) {
  return parent(underscore1, underscore2);
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
