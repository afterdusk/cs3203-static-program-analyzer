#pragma once

#include "PQLEvaluator.h"

class SelectDispatcher : public ClauseDispatcher {
public:
  SelectDispatcher(PqlToken token, PkbQueryInterface *queryHandler);
  ClauseResult resultDispatch();
};

class FollowsDispatcher : public ClauseDispatcher {
public:
  FollowsDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                    PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  ClauseResult resultDispatch();
};

class FollowsStarDispatcher : public ClauseDispatcher {
public:
  FollowsStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                        PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  ClauseResult resultDispatch();
};

class ParentDispatcher : public ClauseDispatcher {
public:
  ParentDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                   PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  ClauseResult resultDispatch();
};

class ParentStarDispatcher : public ClauseDispatcher {
public:
  ParentStarDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                       PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  ClauseResult resultDispatch();
};

class UsesDispatcher : public ClauseDispatcher {
public:
  UsesDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                 PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  ClauseResult resultDispatch();
};

class ModifiesDispatcher : public ClauseDispatcher {
public:
  ModifiesDispatcher(PqlToken firstArgument, PqlToken secondArgument,
                     PkbQueryInterface *queryHandler);
  bool booleanDispatch();
  ClauseResult resultDispatch();
};

class PatternDispatcher : public ClauseDispatcher {
public:
  PatternDispatcher::PatternDispatcher(ParsedPattern pp,
                                       PkbQueryInterface *handler);
  ClauseResult resultDispatch();
};
