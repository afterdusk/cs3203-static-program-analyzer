#include "Dispatchers.h"

SelectDispatcher::SelectDispatcher(TokenType token, SYMBOL synonym,
                                   PkbQueryInterface *queryHandler)
    : ClauseDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(PqlToken{token, synonym}));
}

int SelectDispatcher::dispatchPriority() { return 1; }

EvaluationTable SelectDispatcher::resultDispatch() {
  if (Statement *entity = std::get_if<Statement>(&pkbParameters[0])) {
    return toEvaluationTable(handler->select(*entity));
  }
  if (Procedure *entity = std::get_if<Procedure>(&pkbParameters[0])) {
    return toEvaluationTable(handler->select(*entity));
  }
  if (Variable *entity = std::get_if<Variable>(&pkbParameters[0])) {
    return toEvaluationTable(handler->select(*entity));
  }
  if (Constant *entity = std::get_if<Constant>(&pkbParameters[0])) {
    return toEvaluationTable(handler->select(*entity));
  }
  throw "Invalid: Parameters provided do not return values";
}
