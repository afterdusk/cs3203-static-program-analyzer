#include "ClauseDispatcher.h"
#include "Dispatchers.h"

ClauseDispatcher::ClauseDispatcher(PkbQueryInterface *queryHandler) {
  handler = queryHandler;
}

std::string ClauseDispatcher::toString(PkbTables::LINE_NO lineNumber) {
  std::stringstream stream;
  stream << lineNumber;
  return stream.str();
}

PkbTables::LINE_NO ClauseDispatcher::toLineNumber(std::string lineNumberStr) {
  PkbTables::LINE_NO lineNumber;
  std::stringstream stream(lineNumberStr);
  stream >> lineNumber;
  return lineNumber;
}

ClauseDispatcher *ClauseDispatcher::FromElement(TokenType type, Element element,
                                                PkbQueryInterface *handler) {
  std::optional<SYMBOL> maybeSymbol = elementAttrToSymbol(type, element);
  if (maybeSymbol.has_value()) {
    return new SelectAttributeDispatcher(type, element, handler);
  }
  return new SelectDispatcher(type, element.synonym, handler);
}

ClauseDispatcher *
ClauseDispatcher::FromRelationship(ParsedRelationship pr,
                                   PkbQueryInterface *handler) {
  switch (pr.relationship) {
  case TokenType::FOLLOWS:
    return new FollowsDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::FOLLOWS_T:
    return new FollowsStarDispatcher(pr.firstArgument, pr.secondArgument,
                                     handler);
  case TokenType::PARENT:
    return new ParentDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::PARENT_T:
    return new ParentStarDispatcher(pr.firstArgument, pr.secondArgument,
                                    handler);
  case TokenType::USES:
    return new UsesDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::MODIFIES:
    return new ModifiesDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::CALL:
    return new CallsDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::CALL_T:
    return new CallsStarDispatcher(pr.firstArgument, pr.secondArgument,
                                   handler);
  case TokenType::NEXT:
    return new NextDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::NEXT_T:
    return new NextStarDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::AFFECTS:
    return new AffectsDispatcher(pr.firstArgument, pr.secondArgument, handler);
  case TokenType::AFFECTS_T:
    return new AffectsStarDispatcher(pr.firstArgument, pr.secondArgument,
                                     handler);
  default:
    throw "Invalid: Relationship not implemented";
  }
}

ClauseDispatcher *ClauseDispatcher::FromPattern(ParsedPattern pp,
                                                PkbQueryInterface *handler) {
  return new PatternDispatcher(pp, handler);
}

ClauseDispatcher *ClauseDispatcher::FromWith(
    std::pair<Reference, Reference> pw,
    std::unordered_map<std::string, TokenType> declarations,
    PkbQueryInterface *queryHandler) {
  if (pw.first.referenceType == ReferenceType::ELEMENT &&
      pw.second.referenceType == ReferenceType::ELEMENT) {
    TokenType firstType = declarations[pw.first.element.synonym];
    TokenType secondType = declarations[pw.second.element.synonym];
    return new WithElementPairDispatcher(firstType, pw.first.element,
                                         secondType, pw.second.element,
                                         queryHandler);
  }
  if (pw.first.referenceType == ReferenceType::ELEMENT &&
      pw.second.referenceType == ReferenceType::RAW_VALUE) {
    TokenType type = declarations[pw.first.element.synonym];
    return new WithElementRawPairDispatcher(type, pw.first.element,
                                            pw.second.pqlToken, queryHandler);
  }
  if (pw.first.referenceType == ReferenceType::RAW_VALUE &&
      pw.second.referenceType == ReferenceType::ELEMENT) {
    TokenType type = declarations[pw.second.element.synonym];
    return new WithElementRawPairDispatcher(type, pw.second.element,
                                            pw.first.pqlToken, queryHandler);
  }
  throw "Invalid: With comparison not implemented";
}

ClauseDispatcher::PKB_PARAM ClauseDispatcher::toParam(PqlToken token) {
  switch (token.type) {
  case TokenType::VARIABLE:
    symbols.push_back(token.value);
    return Variable{};
  case TokenType::PROCEDURE:
    symbols.push_back(token.value);
    return Procedure{};
  case TokenType::CONSTANT:
    symbols.push_back(token.value);
    return Constant{};
  case TokenType::UNDERSCORE:
    return Underscore{};
  case TokenType::NUMBER:
    return LineNumber{toLineNumber(token.value)};
  case TokenType::STRING: {
    String s;
    s.name = token.value;
    return s;
  }
  case TokenType::PROG_LINE:
  case TokenType::STMT: {
    symbols.push_back(token.value);
    Statement s;
    return s;
  }
  case TokenType::READ: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::Read;
    return s;
  }
  case TokenType::PRINT: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::Print;
    return s;
  }
  case TokenType::ASSIGN: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::Assign;
    return s;
  }
  case TokenType::CALL: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::Call;
    return s;
  }
  case TokenType::WHILE: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::While;
    return s;
  }
  case TokenType::IF: {
    symbols.push_back(token.value);
    Statement s;
    s.type = PkbTables::StatementType::If;
    return s;
  }
  default:
    throw "Invalid: No PkbTables param for token";
  }
}

EvaluationTable ClauseDispatcher::toEvaluationTable(NAME_SET &set) {
  std::vector<VALUE> &column = std::vector<VALUE>();
  column.insert(column.end(), set.begin(), set.end());
  return EvaluationTable(new TABLE({{symbols[0], column}}));
}

EvaluationTable ClauseDispatcher::toEvaluationTable(LINE_SET &set) {
  std::vector<VALUE> &column = std::vector<VALUE>();
  for (PkbTables::LINE_NO lineNumber : set) {
    column.push_back(toString(lineNumber));
  }
  return EvaluationTable(new TABLE({{symbols[0], column}}));
}

EvaluationTable ClauseDispatcher::toEvaluationTable(NAME_NAME_PAIRS &pairs) {
  std::vector<VALUE> &first = pairs.first;
  std::vector<VALUE> &second = pairs.second;

  // Handle special case where symbols are the same
  if (symbols[0] == symbols[1]) {
    std::vector<VALUE> &column = std::vector<VALUE>();
    for (std::vector<int>::size_type i = 0; i < first.size(); i++) {
      if (first[i] == second[i]) {
        column.push_back(first[i]);
      }
    }
    return EvaluationTable(new TABLE({{symbols[0], column}}));
  }

  return EvaluationTable(
      new TABLE({{symbols[0], first}, {symbols[1], second}}));
}

EvaluationTable ClauseDispatcher::toEvaluationTable(LINE_LINE_PAIRS &pairs) {
  std::vector<VALUE> first;
  std::vector<VALUE> second;

  for (auto &value : pairs.first) {
    first.push_back(toString(value));
  }
  for (auto &value : pairs.second) {
    second.push_back(toString(value));
  }

  NAME_NAME_PAIRS nameNamePairs = {first, second};
  return toEvaluationTable(nameNamePairs);
}

EvaluationTable ClauseDispatcher::toEvaluationTable(LINE_NAME_PAIRS &pairs) {
  std::vector<VALUE> first;

  for (auto &value : pairs.first) {
    first.push_back(toString(value));
  }

  NAME_NAME_PAIRS nameNamePairs = {first, pairs.second};
  return toEvaluationTable(nameNamePairs);
}

std::vector<SYMBOL> ClauseDispatcher::getSymbols() { return symbols; }

bool ClauseDispatcher::willReturnBoolean() {
  for (auto &param : pkbParameters) {
    if (std::holds_alternative<Statement>(param) ||
        std::holds_alternative<Procedure>(param) ||
        std::holds_alternative<Variable>(param)) {
      return false;
    }
  }
  return true;
}

int ClauseDispatcher::dispatchPriority() {
  throw "Invalid: dispatchPriority not callable on this instance";
}

bool ClauseDispatcher::booleanDispatch() {
  throw "Invalid: booleanDispatch not callable on this instance";
}

EvaluationTable ClauseDispatcher::resultDispatch() {
  throw "Invalid: resultsDispatch not callable on this instance";
}
