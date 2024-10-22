#pragma once

#include "ClauseDispatcher.h"

class SelectDispatcher : public ClauseDispatcher {
public:
  SelectDispatcher(TokenType type, SYMBOL synonym,
                   PkbQueryInterface *queryHandler);
  int dispatchPriority();
  EvaluationTable resultDispatch();
};

class SelectAttributeDispatcher : public ClauseDispatcher {
public:
  SelectAttributeDispatcher(TokenType type, Element element,
                            PkbQueryInterface *queryHandler);
  int dispatchPriority();
  EvaluationTable resultDispatch();
};

class FollowsDispatcher : public ClauseDispatcher {
public:
  FollowsDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                    PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class FollowsStarDispatcher : public ClauseDispatcher {
public:
  FollowsStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                        PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class ParentDispatcher : public ClauseDispatcher {
public:
  ParentDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                   PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class ParentStarDispatcher : public ClauseDispatcher {
public:
  ParentStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                       PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class UsesDispatcher : public ClauseDispatcher {
public:
  UsesDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                 PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class ModifiesDispatcher : public ClauseDispatcher {
public:
  ModifiesDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                     PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class CallsDispatcher : public ClauseDispatcher {
public:
  CallsDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                  PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class CallsStarDispatcher : public ClauseDispatcher {
public:
  CallsStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                      PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class NextDispatcher : public ClauseDispatcher {
public:
  NextDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                 PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class NextStarDispatcher : public ClauseDispatcher {
public:
  NextStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                     PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class AffectsDispatcher : public ClauseDispatcher {
public:
  AffectsDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                    PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class AffectsStarDispatcher : public ClauseDispatcher {
public:
  AffectsStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                        PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class NextBipDispatcher : public ClauseDispatcher {
public:
  NextBipDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                    PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class NextBipStarDispatcher : public ClauseDispatcher {
public:
  NextBipStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                        PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class AffectsBipDispatcher : public ClauseDispatcher {
public:
  AffectsBipDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                       PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class AffectsBipStarDispatcher : public ClauseDispatcher {
public:
  AffectsBipStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                           PkbQueryInterface *queryHandler);
  int dispatchPriority();
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class PatternDispatcher : public ClauseDispatcher {
public:
  PatternDispatcher(ParsedPattern pp, PkbQueryInterface *queryHandler);
  int dispatchPriority();
  EvaluationTable resultDispatch();
};

class WithDispatcher : public ClauseDispatcher {
protected:
  typedef std::pair<std::vector<VALUE>, std::vector<VALUE>> VALUE_ATTR_PAIR;
  typedef std::vector<VALUE> JUST_VALUE;
  typedef std::variant<JUST_VALUE, VALUE_ATTR_PAIR> COMPARABLE;

  JUST_VALUE toValue(LINE_SET &lineSet);
  JUST_VALUE toValue(NAME_SET &nameSet);
  VALUE_ATTR_PAIR toValueAttrPair(LINE_NAME_PAIRS &lineNamePairs);
  COMPARABLE getComparable(TokenType type, AttributeRefType attr,
                           PKB_PARAM pkbParam);

  WithDispatcher(PkbQueryInterface *queryHandler);

public:
  bool willReturnBoolean();
};

class WithElementPairDispatcher : public WithDispatcher {
private:
  std::pair<Element, Element> elements;
  std::pair<TokenType, TokenType> types;

public:
  WithElementPairDispatcher(TokenType firstType, Element firstElement,
                            TokenType secondType, Element secondElement,
                            PkbQueryInterface *queryHandler);
  int dispatchPriority();
  EvaluationTable resultDispatch();
};

class WithElementRawPairDispatcher : public WithDispatcher {
private:
  Element element;
  TokenType type;
  VALUE rawValue;

public:
  WithElementRawPairDispatcher(TokenType elementType, Element element,
                               PqlToken value, PkbQueryInterface *queryHandler);
  int dispatchPriority();
  EvaluationTable resultDispatch();
};
