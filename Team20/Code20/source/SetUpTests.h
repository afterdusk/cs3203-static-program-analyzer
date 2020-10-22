#pragma once

#include "CfgNode.h"
#include "DispatcherGraph.h"
#include "Pkb.h"

class SetUpTests {

public:
  enum class TestNumber {
    A,
    B,
    C,
  };
  PkbTables *pkbTables;

  Statement w, r, a, c, i, p, stmt;
  Pkb::LINE_NO l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13, l14, l15,
      l16, l17, l18, l19, l20, l21, l22, l23, l24, l25, l26, l27, l28, l29, l30,
      l31, l32, l33, l999;
  Pkb::PROC p0, p1, p2;

  LineNumber c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
      c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c999;
  String strx, stry, strr, strm, strq, strt, strk, strpmain, strpaux, strpextra,
      strpextratwo, strpextrathree, strpextrafour, strpcomplicate;
  Variable variable;
  Procedure procedure;
  Underscore underscore;
  Constant constant;
  Pkb::AST l4ast, l10ast, l16ast, l18ast, l20ast, l24ast;

  /* @brief Initializes some public variables in SetUpTests. */
  SetUpTests(Pkb &pkb, TestNumber testNumber);
};
class SetUpDispatcherGraphTest {
public:
  DispatcherGraph dg1;
  DispatcherGraph dg2;
  DispatcherGraph dg3;
  ClauseDispatcher *mergingCd;
  ClauseDispatcher *anotherMergingCd;
  ClauseDispatcher *noCommonSymbolsWithDg1Cd;

  SetUpDispatcherGraphTest();
};

template <typename T, typename U>
static std::list<std::pair<T, U>>
convertToOrderedList(std::pair<std::vector<T>, std::vector<U>> pairs) {
  std::list<std::pair<T, U>> ls;

  for (int i = 0; i < pairs.first.size(); i++) {
    ls.push_back({pairs.first[i], pairs.second[i]});
  }
  ls.sort();
  return ls;
}
