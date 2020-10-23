#include "Dispatchers.h"

SelectAttributeDispatcher::SelectAttributeDispatcher(
    TokenType type, Element element, PkbQueryInterface *queryHandler)
    : ClauseDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(PqlToken{type, element.synonym}));
  symbols.push_back(elementAttrToSymbol(type, element).value());
}

int SelectAttributeDispatcher::dispatchPriority() { return 2; }

EvaluationTable SelectAttributeDispatcher::resultDispatch() {
  if (Statement *entity = std::get_if<Statement>(&pkbParameters[0])) {
    return toEvaluationTable(handler->selectAttribute(*entity));
  }
  throw "Invalid: Parameters provided do not return values";
}
