#include "Dispatchers.h"

WithElementRawPairDispatcher::WithElementRawPairDispatcher(
    TokenType elementType, Element element, PqlToken value,
    PkbQueryInterface *queryHandler)
    : WithDispatcher(queryHandler) {
  pkbParameters.push_back(toParam(PqlToken{elementType, element.synonym}));
  this->element = element;
  this->type = elementType;
  this->rawValue = value.value;
}

EvaluationTable WithElementRawPairDispatcher::resultDispatch() {
  COMPARABLE comparable =
      getComparable(type, element.refType, pkbParameters[0]);
  TABLE *table = new TABLE;
  SYMBOL lhsSymbol = symbols[0];
  (*table)[lhsSymbol] = {};
  if (JUST_VALUE *lhs = std::get_if<JUST_VALUE>(&comparable)) {
    for (std::vector<VALUE>::size_type i = 0; i < lhs->size(); i++) {
      if ((*lhs)[i] == rawValue) {
        (*table)[lhsSymbol].push_back((*lhs)[i]);
      }
    }
  }
  if (VALUE_ATTR_PAIR *lhs = std::get_if<VALUE_ATTR_PAIR>(&comparable)) {
    SYMBOL lhsAttrSymbol = elementAttrToSymbol(type, element).value();
    (*table)[lhsAttrSymbol] = {};
    for (std::vector<VALUE>::size_type i = 0; i < lhs->second.size(); i++) {
      if (lhs->second[i] == rawValue) {
        (*table)[lhsSymbol].push_back(lhs->first[i]);
        (*table)[lhsAttrSymbol].push_back(lhs->second[i]);
      }
    }
  }
  return EvaluationTable(table);
}
