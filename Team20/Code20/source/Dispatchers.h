#pragma once

#include "PqlEvaluator.h"

class SelectDispatcher : public ClauseDispatcher {
public:
  SelectDispatcher(PqlToken token, PkbQueryInterface *queryHandler);
  EvaluationTable resultDispatch();
};

class FollowsDispatcher : public ClauseDispatcher {
public:
  FollowsDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                    PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class FollowsStarDispatcher : public ClauseDispatcher {
public:
  FollowsStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                        PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class ParentDispatcher : public ClauseDispatcher {
public:
  ParentDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                   PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class ParentStarDispatcher : public ClauseDispatcher {
public:
  ParentStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                       PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class UsesDispatcher : public ClauseDispatcher {
public:
  UsesDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                 PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class ModifiesDispatcher : public ClauseDispatcher {
public:
  ModifiesDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                     PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  EvaluationTable resultDispatch();
};

class PatternDispatcher : public ClauseDispatcher {
public:
  PatternDispatcher::PatternDispatcher(ParsedPattern pp,
                                       PkbQueryInterface *handler);
  EvaluationTable resultDispatch();
};
