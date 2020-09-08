#pragma once

#include "PkbQueryInterface.h"
#include "PKB.h"

// Query API for pattern matching
// Result<StmtNumberAndStringPairs> PkbQueryInterface::match(Statement
// statement,
//                                                Variable variable,
//                                                PatternSpec spec) {}
// Result<StmtNumberList> PkbQueryInterface::match(Statement statement,
//                                                Underscore underscore,
//                                                PatternSpec spec) {}
// Result<StmtNumberList> PkbQueryInterface::match(Statement statement,
//                                                String varName,
//                                                PatternSpec spec) {}

// Query API for normal select
// Result<StringList> PkbQueryInterface::select(Variable) {}
// Result<StmtNumberList> PkbQueryInterface::select(Stmt) {}
// Result<StmtNumberList> PkbQueryInterface::select(Statement) {}
// Result<StringList> PkbQueryInterface::select(Procedure) {}

// Query API for follows

Result<bool> PkbQueryInterface::follows(LineNumber line1, LineNumber line2) {
  bool res = followTable.map[line1.number] == line2.number;
  return Result(res);
}

Result<StmtNumberList> PkbQueryInterface::follows(LineNumber line,
                                                  Statement statement) {
  FOLLOW followLine = followTable.map[line.number];

  StatementType type = statementTypeTable.map[followLine];
  return followLine != LINE_NO() && statement.type == type
             ? Result(StmtNumberList{followLine})
             : Result(StmtNumberList());
}

Result<StmtNumberList> PkbQueryInterface::follows(LineNumber line, Stmt stmt) {
  FOLLOW followLine = followTable.map[line.number];
  return followLine != LINE_NO() ? Result(StmtNumberList{followLine})
                                 : Result(StmtNumberList());
}

Result<bool> PkbQueryInterface::follows(LineNumber line,
                                        Underscore underscore) {
  FOLLOW followLine = followTable.map[line.number];
  return followLine == LINE_NO() ? Result(false) : Result(true);
}

Result<StmtNumberList> PkbQueryInterface::follows(Statement statement,
                                                  LineNumber line) {
  LINE_NO prevLine = prevLineTable.map[line.number];
  StatementType type = statementTypeTable.map[prevLine];

  return prevLine != LINE_NO() && statement.type == type
             ? Result(StmtNumberList{prevLine})
             : Result(StmtNumberList());
}

// Need tests
Result<StmtNumberPairs> PkbQueryInterface::follows(Statement statement1,
                                                   Statement statement2) {
  StmtNumberPairs result;
  StmtNumberList lineNumbers = invertStatementTypeTable.map[statement1.type];
  StmtNumberList::iterator it = lineNumbers.begin();

  while (it != lineNumbers.end()) {
    FOLLOW followLine = followTable.map[*it];
    StatementType type = statementTypeTable.map[followLine];
    if (followLine != LINE_NO() && type == statement2.type) {
      result.first.push_back(*it);
      result.second.push_back(followLine);
    } else {
      it++;
    }
  }

  return Result(result);
}
// Need tests
Result<StmtNumberPairs> PkbQueryInterface::follows(Statement statement,
                                                   Stmt stmt) {
  StmtNumberPairs result;
  StmtNumberList lineNumbers = invertStatementTypeTable.map[statement.type];
  StmtNumberList::iterator it = lineNumbers.begin();

  while (it != lineNumbers.end()) {
    FOLLOW followLine = followTable.map[*it];
    if (followLine != LINE_NO()) {
      result.first.push_back(*it);
      result.second.push_back(followLine);
    } else {
      it++;
    }
  }

  return Result(result);
}
// Need tests
Result<StmtNumberList> PkbQueryInterface::follows(Statement statement,
                                                  Underscore underscore) {
  StmtNumberList lineNumbers = invertStatementTypeTable.map[statement.type];
  StmtNumberList::iterator it = lineNumbers.begin();

  while (it != lineNumbers.end()) {
    if (followTable.map[*it] == LINE_NO()) {
      it = lineNumbers.erase(it);
    } else {
      it++;
    }
  }

  return Result(lineNumbers);
}

Result<StmtNumberList> PkbQueryInterface::follows(Stmt stmt, LineNumber line) {
  LINE_NO prevLine = prevLineTable.map[line.number];
  return prevLine != LINE_NO() ? Result(StmtNumberList{prevLine})
                               : Result(StmtNumberList());
}
// Need tests
Result<StmtNumberPairs> PkbQueryInterface::follows(Stmt stmt,
                                                   Statement statement) {
  StmtNumberPairs result;
  StmtNumberList lineNumbers = invertStatementTypeTable.map[statement.type];
  StmtNumberList::iterator it = lineNumbers.begin();

  while (it != lineNumbers.end()) {
    LINE_NO prevLine = prevLineTable.map[*it];
    if (prevLine != LINE_NO()) {
      result.first.push_back(prevLine);
      result.second.push_back(*it);
    } else {
      it++;
    }
  }

  return Result(result);
}

// Need tests
Result<bool> PkbQueryInterface::follows(Underscore underscore,
                                        LineNumber line) {
  LINE_NO prevLine = prevLineTable.map[line.number];
  return prevLine == LINE_NO() ? Result(false) : Result(true);
}
// Need tests
Result<StmtNumberList> PkbQueryInterface::follows(Underscore underscore,
                                                  Statement statement) {
  StmtNumberList lineNumbers = invertStatementTypeTable.map[statement.type];
  StmtNumberList::iterator it = lineNumbers.begin();

  while (it != lineNumbers.end()) {
    if (prevLineTable.map[*it] == LINE_NO()) {
      it = lineNumbers.erase(it);
    } else {
      it++;
    }
  }
  return Result(lineNumbers);
}

// Query API for followsStar
// Need tests
Result<bool> PkbQueryInterface::followsStar(LineNumber line1,
                                            LineNumber line2) {
  FOLLOWS followLines = closeFollowTable.map[line1.number];
  if (std::find(followLines.begin(), followLines.end(), line2.number) !=
      followLines.end()) {
    return Result(true);
  } else {
    return Result(false);
  }
}
// Need tests
Result<StmtNumberList> PkbQueryInterface::followsStar(LineNumber line,
                                                      Statement statement) {
  StmtNumberList followLines = closeFollowTable.map[line.number];
  StmtNumberList::iterator it = followLines.begin();

  while (it != followLines.end()) {
    if (statementTypeTable.map[*it] != statement.type) {
      it = followLines.erase(it);
    } else {
      it++;
    }
  }
  return Result(followLines);
}
// Need tests
Result<StmtNumberList> PkbQueryInterface::followsStar(LineNumber line,
                                                      Stmt stmt) {
  StmtNumberList followLines = closeFollowTable.map[line.number];
  return Result(followLines);
}
// Need tests
Result<bool> PkbQueryInterface::followsStar(LineNumber line,
                                            Underscore underscore) {
  return follows(line, underscore);
}

// Need test
Result<bool> PkbQueryInterface::followsStar(Underscore underscore,
                                            LineNumber line) {
  return follows(underscore, line);
}

// Query API for parent

Result<bool> PkbQueryInterface::parent(LineNumber line1, LineNumber line2) {
  bool res = parentTable.map[line2.number] == line1.number;
  return Result(res);
}

Result<StmtNumberList> PkbQueryInterface::parent(LineNumber line,
                                                 Statement statement) {
  StmtNumberList children = childrenTable.map[line.number];
  StmtNumberList::iterator it = children.begin();

  while (it != children.end()) {
    if (statementTypeTable.map[*it] != statement.type) {
      it = children.erase(it);
    } else {
      it++;
    }
  }

  return Result(children);
}

Result<StmtNumberList> PkbQueryInterface::parent(LineNumber line, Stmt stmt) {
  StmtNumberList children = childrenTable.map[line.number];
  return Result(children);
}

Result<bool> PkbQueryInterface::parent(LineNumber line, Underscore underscore) {
  StmtNumberList children = childrenTable.map[line.number];
  return children.size() > 0 ? Result(true) : Result(false);
}

Result<StmtNumberList> PkbQueryInterface::parent(Statement statement,
                                                 LineNumber line) {
  PARENT parent = parentTable.map[line.number];
  StatementType type = statementTypeTable.map[parent];

  return parent != LINE_NO() && type == statement.type
             ? Result(StmtNumberList{parent})
             : Result(StmtNumberList());
}

// Need tests
Result<StmtNumberList> PkbQueryInterface::parent(Stmt stmt, LineNumber line) {
  PARENT parent = parentTable.map[line.number];
  return parent != LINE_NO() ? Result(StmtNumberList{parent})
                             : Result(StmtNumberList());
}
// Need tests
Result<bool> PkbQueryInterface::parent(Underscore underscore, LineNumber line) {
  PARENT parent = parentTable.map[line.number];
  return parent != LINE_NO() ? Result(true) : Result(false);
}

// Query API for parentStar
// Need tests
Result<bool> PkbQueryInterface::parentStar(LineNumber line1, LineNumber line2) {
  PARENTS parentLines = closeParentTable.map[line2.number];
  if (std::find(parentLines.begin(), parentLines.end(), line1.number) !=
      parentLines.end()) {
    return Result(true);
  } else {
    return Result(false);
  }
}

// Need tests
Result<bool> PkbQueryInterface::parentStar(LineNumber line,
                                           Underscore underscore) {
  return parent(line, underscore);
}
// Need tests
Result<bool> PkbQueryInterface::parentStar(Underscore underscore,
                                           LineNumber line) {
  return parent(underscore, line);
}
