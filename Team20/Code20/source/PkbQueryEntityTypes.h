#pragma once

#include "PkbTables.h"
#include "SimpleExprParserWrapper.h"
#include "SimpleTokenizer.h"
#include <optional>
#include <string>
#include <utility>
#include <vector>

enum class PatternMatchType { CompleteMatch, SubTreeMatch, Any };

struct Entity {};
struct Variable : public Entity {};
struct Constant : public Entity {};
struct Procedure : public Entity {};
struct Underscore : public Entity {};
struct LineNumber {
  PkbTables::LINE_NO number;
};
struct String : public Entity {
  std::string name;
};
struct Statement : public Entity {
  std::optional<PkbTables::StatementType> type;
};
struct PatternSpec {
  PatternMatchType type;
  TNode *value = nullptr;
  PatternSpec(PatternMatchType specifiedTokenType)
      : type{specifiedTokenType}, value{nullptr} {}
  PatternSpec(PatternMatchType specifiedTokenType, std::string stringValue)
      : type{specifiedTokenType}, value{parseStringToTNode(stringValue)} {}
  bool operator==(const PatternSpec &other) const {
    if (value == NULL && other.value == NULL) {
      return type == other.type;
    } else if (value == NULL || other.value == NULL) {
      return false;
    }
    return type == other.type && (*value) == (*other.value);
  }
  TNode *parseStringToTNode(std::string expression) {
    const auto tokens = Tokenizer(expression).tokenize();
    const auto node = new TNode();
    SimpleExprParserWrapper(tokens, 0, node).parse();
    return node;
  }
};
