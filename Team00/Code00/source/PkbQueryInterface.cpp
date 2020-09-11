#pragma once

#include "PkbQueryInterface.h"
#include "PKB.h"

// Query API for pattern matching
// Result<StmtNumberAndStringPairs> PkbQueryInterface::match(Statement
// statement,
//                                                Variable variable,
//                                                PatternSpec spec) {}
// STRING_SET PkbQueryInterface::match(Statement statement,
//                                                Underscore underscore,
//                                                PatternSpec spec) {}
// Need tests
STRING_SET PkbQueryInterface::match(Statement statement, String varName,
                                    PatternSpec spec) {
  STRING_SET statementLines = invertStatementTypeTable.map[statement.type];
  // loop statementLines, check each line on modifiesTable, if got line modifies
  // that varName, go to astTable get AST of that line, do ASTcheck with spec
  return STRING_SET();
}

// Query API for normal select
// Result<StringList> PkbQueryInterface::select(Variable var) {}
// Need tests
STRING_SET PkbQueryInterface::select(Statement statement) {
  if (statement.type == StatementType::NONE) {
    return stmtTableIndexes;
  } else {
    return invertStatementTypeTable.map[statement.type];
  }
}
// Result<StringList> PkbQueryInterface::select(Procedure proc) {}

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
  return followLine == LINE_NO() ? false : true;
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
  return prevLine == LINE_NO() ? false : true;
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
  return followTable.size() > 0 ? true : false;
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
  return children.size() > 0 ? true : false;
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
  return parent != LINE_NO() ? true : false;
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
  return childrenTable.map.size() > 0 ? true : false;
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
// Need tests
bool PkbQueryInterface::uses(LineNumber line, String variable) {
  VAR_TABLE_INDEX inputVarIndex = varTable.map[variable.name];
  USES use = usesTable.map[line.number];
  auto *vars = std::get_if<VAR_TABLE_INDEXES>(&use);
  auto *proc = std::get_if<PROC_TABLE_INDEX>(&use);

  // if uses value at given line is a list in variable indexes
  if (vars != NULL) {
    VAR_TABLE_INDEXES indexes = *vars;

    return (indexes.find(inputVarIndex) != indexes.end()) ? true : false;
  }

  // if uses value at given line is a procedure index
  if (proc != NULL) {
    PROC_TABLE_INDEX procIndex = *proc;
    VAR_TABLE_INDEXES indexes = usesProcTable.map[procIndex];

    return (indexes.find(inputVarIndex) != indexes.end()) ? true : false;
  }

  // if input line doesnt exist in use table
  return false;
}
