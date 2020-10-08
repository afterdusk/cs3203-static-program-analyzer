#include "Dispatchers.h"

WithDispatcher::WithDispatcher(PkbQueryInterface *queryHandler)
    : ClauseDispatcher(queryHandler){};

WithDispatcher::COMPARABLE WithDispatcher::getComparable(TokenType type,
                                                         AttributeRefType attr,
                                                         PKB_PARAM pkbParam) {
  switch (type) {
  case TokenType::CALL:
    if (attr == AttributeRefType::PROCNAME) {
      LINE_NAME_PAIRS &lineNamePairs =
          handler->getStmtLineAndName(std::get<Statement>(pkbParam));
      return toValueAttrPair(lineNamePairs);
    }
    [[fallthrough]];
  case TokenType::PRINT:
  case TokenType::READ:
    if (attr == AttributeRefType::VARNAME) {
      LINE_NAME_PAIRS &lineNamePairs =
          handler->getStmtLineAndName(std::get<Statement>(pkbParam));
      return toValueAttrPair(lineNamePairs);
    }
    [[fallthrough]];
  case TokenType::PROG_LINE:
  case TokenType::STMT:
  case TokenType::WHILE:
  case TokenType::IF:
  case TokenType::ASSIGN: {
    LINE_SET &lineSet = handler->select(std::get<Statement>(pkbParam));
    return toValue(lineSet);
  }
  case TokenType::PROCEDURE: {
    NAME_SET &nameSet = handler->select(std::get<Procedure>(pkbParam));
    return toValue(nameSet);
  }
  case TokenType::CONSTANT: {
    NAME_SET &nameSet = handler->select(std::get<Constant>(pkbParam));
    return toValue(nameSet);
  }
  case TokenType::VARIABLE: {
    NAME_SET &lineSet = handler->select(std::get<Variable>(pkbParam));
    return toValue(lineSet);
  }
  }

  throw "Invalid: Comparable cannot be built";
};

WithDispatcher::JUST_VALUE WithDispatcher::toValue(LINE_SET &lineSet) {
  JUST_VALUE attr;
  for (PkbTables::LINE_NO lineNumber : lineSet) {
    attr.push_back(toString(lineNumber));
  }
  return attr;
};

WithDispatcher::JUST_VALUE WithDispatcher::toValue(NAME_SET &nameSet) {
  JUST_VALUE attr;
  attr.insert(attr.end(), nameSet.begin(), nameSet.end());
  return attr;
};

WithDispatcher::VALUE_ATTR_PAIR
WithDispatcher::toValueAttrPair(LINE_NAME_PAIRS &lineNamePairs) {
  std::vector<VALUE> first;

  for (auto &value : lineNamePairs.first) {
    first.push_back(toString(value));
  }

  return VALUE_ATTR_PAIR{first, lineNamePairs.second};
};

bool WithDispatcher::willReturnBoolean() { return false; }
