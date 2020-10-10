#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestSelectAndPatternQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests =
      SetUpTests::SetUpTests(pkb, SetUpTests::TestNumber::A);

  TEST_METHOD(SelectVariable) {
    Assert::IsTrue(pkb.getQueryInterface()->select(setUpTests.variable) ==
                   NAME_SET{"x", "y", "r", "m", "q", "t", "k"});
  } // namespace UnitTesting

  TEST_METHOD(SelectProcedure) {
    Assert::IsTrue(pkb.getQueryInterface()->select(setUpTests.procedure) ==
                   NAME_SET{"main", "aux", "extra", "complicate", "extratwo",
                            "extrathree", "extrafour"});
  } // namespace UnitTesting

  TEST_METHOD(SelectStatement) {
    Assert::IsTrue(pkb.getQueryInterface()->select(setUpTests.r) ==
                   LINE_SET{setUpTests.l1, setUpTests.l2, setUpTests.l8,
                            setUpTests.l9, setUpTests.l12, setUpTests.l13,
                            setUpTests.l5});
    Assert::IsTrue(pkb.getQueryInterface()->select(setUpTests.c) ==
                   LINE_SET{setUpTests.l7, setUpTests.l11, setUpTests.l25,
                            setUpTests.l27, setUpTests.l28, setUpTests.l29,
                            setUpTests.l30, setUpTests.l31, setUpTests.l32,
                            setUpTests.l33});
    Assert::IsTrue(
        pkb.getQueryInterface()->select(setUpTests.stmt) ==
        LINE_SET{setUpTests.l1,  setUpTests.l2,  setUpTests.l3,  setUpTests.l4,
                 setUpTests.l5,  setUpTests.l6,  setUpTests.l7,  setUpTests.l8,
                 setUpTests.l9,  setUpTests.l10, setUpTests.l11, setUpTests.l12,
                 setUpTests.l13, setUpTests.l14, setUpTests.l15, setUpTests.l16,
                 setUpTests.l17, setUpTests.l18, setUpTests.l19, setUpTests.l20,
                 setUpTests.l21, setUpTests.l22, setUpTests.l23, setUpTests.l24,
                 setUpTests.l25, setUpTests.l26, setUpTests.l27, setUpTests.l28,
                 setUpTests.l29, setUpTests.l30, setUpTests.l31, setUpTests.l32,
                 setUpTests.l33});
  } // namespace UnitTesting
  TEST_METHOD(SelectConstant) {
    Assert::IsTrue(
        pkb.getQueryInterface()->select(setUpTests.constant) ==
        PkbTables::CONSTANT_TABLE{"11111111111111111111111111111111111111", "5",
                                  "1", "0"});
  } // namespace UnitTesting

  TEST_METHOD(GetStmtLineAndName) {
    Assert::IsTrue(pkb.getQueryInterface()->getStmtLineAndName(setUpTests.r) ==
                   LINE_NAME_PAIRS(
                       LINE_VECTOR{
                           setUpTests.l1,
                           setUpTests.l9,
                           setUpTests.l2,
                           setUpTests.l5,
                           setUpTests.l13,
                           setUpTests.l8,
                           setUpTests.l12,
                       },
                       NAME_VECTOR{"x", "r", "y", "y", "t", "x", "q"}));

    Assert::IsTrue(pkb.getQueryInterface()->getStmtLineAndName(setUpTests.p) ==
                   LINE_NAME_PAIRS(LINE_VECTOR{setUpTests.l6, setUpTests.l22,
                                               setUpTests.l21, setUpTests.l26},
                                   NAME_VECTOR{"x", "t", "q", "k"}));

    Assert::IsTrue(
        pkb.getQueryInterface()->getStmtLineAndName(setUpTests.c) ==
        LINE_NAME_PAIRS(
            LINE_VECTOR{setUpTests.l31, setUpTests.l7, setUpTests.l27,
                        setUpTests.l11, setUpTests.l25, setUpTests.l28,
                        setUpTests.l29, setUpTests.l30, setUpTests.l32,
                        setUpTests.l33},
            NAME_VECTOR{"extrathree", "aux", "extra", "complicate", "extra",
                        "aux", "main", "extratwo", "main", "extrathree"}));
  } // namespace UnitTesting

  TEST_METHOD(MatchAssign) {
    String varq, vary;
    varq.name = "q";
    vary.name = "y";

    PkbTables::AST qminus1 = std::make_shared<TNode>(TNode::Op::Minus);
    qminus1->left = std::make_shared<TNode>("q");
    qminus1->right = std::make_shared<TNode>("1");

    PkbTables::AST nodex = std::make_shared<TNode>("x");
    PkbTables::AST const1 = std::make_shared<TNode>("1");

    PatternSpec spec1 = PatternSpec{PatternMatchType::CompleteMatch};
    spec1.value = qminus1;

    PatternSpec spec2 = PatternSpec{PatternMatchType::SubTreeMatch};
    spec2.value = nodex;

    PatternSpec spec3 = PatternSpec{PatternMatchType::SubTreeMatch};
    spec3.value = const1;

    /*
     * Pattern matching with string on LHS
     */
    // complete match
    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.a, varq, spec1) ==
                   LINE_SET{setUpTests.l20, setUpTests.l24});

    // partial match
    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.a, vary, spec2) ==
                   LINE_SET{setUpTests.l4});
    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.a, varq, spec3) ==
                   LINE_SET{setUpTests.l24, setUpTests.l20});

    /*
     * Pattern matching with underscore on LHS
     */
    // complete match
    Assert::IsTrue(pkb.getQueryInterface()->match(
                       setUpTests.a, setUpTests.underscore, spec1) ==
                   LINE_SET{setUpTests.l20, setUpTests.l24});

    // partial match
    Assert::IsTrue(pkb.getQueryInterface()->match(
                       setUpTests.a, setUpTests.underscore, spec2) ==
                   LINE_SET{setUpTests.l4, setUpTests.l10});
    Assert::IsTrue(pkb.getQueryInterface()->match(
                       setUpTests.a, setUpTests.underscore, spec3) ==
                   LINE_SET{setUpTests.l24, setUpTests.l20, setUpTests.l16,
                            setUpTests.l18});

    /*
     * Pattern matching with synonym variable on LHS
     */
    // complete match
    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.a,
                                                  setUpTests.variable, spec1) ==
                   LINE_NAME_PAIRS(LINE_VECTOR{setUpTests.l20, setUpTests.l24},
                                   NAME_VECTOR{"q", "q"}));

    // partial match
    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.a,
                                                  setUpTests.variable, spec2) ==
                   LINE_NAME_PAIRS(LINE_VECTOR{setUpTests.l4, setUpTests.l10},
                                   NAME_VECTOR{"y", "m"}));
    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.a,
                                                  setUpTests.variable, spec3) ==
                   LINE_NAME_PAIRS(LINE_VECTOR{setUpTests.l20, setUpTests.l18,
                                               setUpTests.l16, setUpTests.l24},
                                   NAME_VECTOR{"q", "t", "t", "q"}));

  } // namespace UnitTesting

  TEST_METHOD(MatchIf) {
    String vart, varq, varx;
    vart.name = "t";
    varq.name = "q";
    varx.name = "x";
    PatternSpec spec = PatternSpec{PatternMatchType::Any};

    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.i, vart, spec) ==
                   LINE_SET{setUpTests.l15});
    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.i,
                                                  setUpTests.variable, spec) ==
                   LINE_NAME_PAIRS(LINE_VECTOR{setUpTests.l15, setUpTests.l19},
                                   NAME_VECTOR{"t", "q"}));
    Assert::IsTrue(pkb.getQueryInterface()->match(
                       setUpTests.i, setUpTests.underscore, spec) ==
                   LINE_SET{setUpTests.l15, setUpTests.l19});

    // var x is not in any IF statement's conditions
    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.i, varx, spec) ==
                   LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(MatchWhile) {
    String vart, varq, varx;
    vart.name = "t";
    varq.name = "q";
    varx.name = "x";
    PatternSpec spec = PatternSpec{PatternMatchType::Any};

    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.w, vart, spec) ==
                   LINE_SET{setUpTests.l14, setUpTests.l17});
    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.w,
                                                  setUpTests.variable, spec) ==
                   LINE_NAME_PAIRS(LINE_VECTOR{setUpTests.l3, setUpTests.l14,
                                               setUpTests.l14, setUpTests.l17,
                                               setUpTests.l17, setUpTests.l23},
                                   NAME_VECTOR{"y", "q", "t", "t", "q", "q"}));
    Assert::IsTrue(pkb.getQueryInterface()->match(
                       setUpTests.w, setUpTests.underscore, spec) ==
                   LINE_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                            setUpTests.l23});

    // var x is not in any WHILE statement's conditions
    Assert::IsTrue(pkb.getQueryInterface()->match(setUpTests.w, varx, spec) ==
                   LINE_SET());
  } // namespace UnitTesting
};
} // namespace UnitTesting
