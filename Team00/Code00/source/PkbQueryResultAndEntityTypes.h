#pragma once

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
  TNode value;
};

template <typename T> class Result {
public:
  T value;
  Result(T value) { this->value = value; }
};
