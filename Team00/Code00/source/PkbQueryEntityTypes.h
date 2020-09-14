#pragma once

#include "ExprParserWrapper.h"
#include "Pkb.h"
#include "Tokenizer.h"
#include <string>
#include <utility>
#include <vector>

enum class PatternMatchType { CompleteMatch, SubTreeMatch, Any };

struct Entity {};
struct Stmt : public Entity {};
struct Variable : public Entity {};
struct Procedure : public Entity {};
struct Underscore : public Entity {};
struct LineNumber {
  LINE_NO number;
};
struct String : public Entity {
  std::string name;
};
struct Statement : public Entity {
  StatementType type;
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
    ExprParserWrapper(tokens, 0, node).parse();
    return node;
  }
};
