#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestSelectAndPatternQuery) {

public:
  SetUpTests setUpTests;
  TEST_METHOD_INITIALIZE(SetUpPkb) { setUpTests = SetUpTests::SetUpTests(); }
  TEST_METHOD(SelectVariable) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.select(setUpTests.variable) ==
                   STRING_SET{"x", "y", "r", "m", "q", "t", "k"});
  } // namespace UnitTesting

  TEST_METHOD(SelectProcedure) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.select(setUpTests.procedure) ==
                   STRING_SET{"main", "aux", "extra", "complicate"});
  } // namespace UnitTesting

  TEST_METHOD(SelectStatement) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.select(setUpTests.r) ==
                   STRING_SET{setUpTests.l1, setUpTests.l2, setUpTests.l8,
                              setUpTests.l9, setUpTests.l12, setUpTests.l13,
                              setUpTests.l5});
    Assert::IsTrue(setUpTests.pkbQueryInterface.select(setUpTests.c) ==
                   STRING_SET{setUpTests.l7, setUpTests.l11, setUpTests.l25});
    Assert::IsTrue(setUpTests.pkbQueryInterface.select(setUpTests.stmt) ==
                   STRING_SET{setUpTests.l1,  setUpTests.l2,  setUpTests.l3,
                              setUpTests.l4,  setUpTests.l5,  setUpTests.l6,
                              setUpTests.l7,  setUpTests.l8,  setUpTests.l9,
                              setUpTests.l10, setUpTests.l11, setUpTests.l12,
                              setUpTests.l13, setUpTests.l14, setUpTests.l15,
                              setUpTests.l16, setUpTests.l17, setUpTests.l18,
                              setUpTests.l19, setUpTests.l20, setUpTests.l21,
                              setUpTests.l22, setUpTests.l23, setUpTests.l24,
                              setUpTests.l25, setUpTests.l26});
  } // namespace UnitTesting
  TEST_METHOD(SelectConstant) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.select(setUpTests.constant) ==
        STRING_SET{"11111111111111111111111111111111111111", "5", "1", "0"});
  } // namespace UnitTesting
  TEST_METHOD(Match) {
    String varq, vary;
    varq.name = "q";
    vary.name = "y";

    AST qminus1;
    TNode T3 = TNode(Op::Minus);
    T3.left = new TNode("q");
    T3.right = new TNode("1");
    qminus1 = T3;

    AST nodex = TNode("x");
    AST const1 = TNode("1");

    PatternSpec spec1 = PatternSpec{PatternMatchType::CompleteMatch};
    spec1.value = &qminus1;

    PatternSpec spec2 = PatternSpec{PatternMatchType::SubTreeMatch};
    spec2.value = &nodex;

    PatternSpec spec3 = PatternSpec{PatternMatchType::SubTreeMatch};
    spec3.value = &const1;

    /*
     * Pattern matching with string on LHS
     */
    // complete match
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.match(setUpTests.a, varq, spec1) ==
        STRING_SET{setUpTests.l20, setUpTests.l24});

    // partial match
    Assert::IsTrue(setUpTests.pkbQueryInterface.match(
                       setUpTests.a, vary, spec2) == STRING_SET{setUpTests.l4});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.match(setUpTests.a, varq, spec3) ==
        STRING_SET{setUpTests.l24, setUpTests.l20});

    /*
     * Pattern matching with underscore on LHS
     */
    // complete match
    Assert::IsTrue(setUpTests.pkbQueryInterface.match(
                       setUpTests.a, setUpTests.underscore, spec1) ==
                   STRING_SET{setUpTests.l20, setUpTests.l24});

    // partial match
    Assert::IsTrue(setUpTests.pkbQueryInterface.match(
                       setUpTests.a, setUpTests.underscore, spec2) ==
                   STRING_SET{setUpTests.l4, setUpTests.l10});
    Assert::IsTrue(setUpTests.pkbQueryInterface.match(
                       setUpTests.a, setUpTests.underscore, spec3) ==
                   STRING_SET{setUpTests.l24, setUpTests.l20, setUpTests.l16,
                              setUpTests.l18});

    /*
     * Pattern matching with synonym variable on LHS
     */
    // complete match
    Assert::IsTrue(setUpTests.pkbQueryInterface.match(
                       setUpTests.a, setUpTests.variable, spec1) ==
                   STRING_PAIRS(STRING_VECTOR{setUpTests.l24, setUpTests.l20},
                                STRING_VECTOR{"q", "q"}));

    // partial match
    Assert::IsTrue(setUpTests.pkbQueryInterface.match(
                       setUpTests.a, setUpTests.variable, spec2) ==
                   STRING_PAIRS(STRING_VECTOR{setUpTests.l4, setUpTests.l10},
                                STRING_VECTOR{"y", "m"}));
    Assert::IsTrue(setUpTests.pkbQueryInterface.match(
                       setUpTests.a, setUpTests.variable, spec3) ==
                   STRING_PAIRS(STRING_VECTOR{setUpTests.l24, setUpTests.l18,
                                              setUpTests.l16, setUpTests.l20},
                                STRING_VECTOR{"q", "t", "t", "q"}));

  } // namespace UnitTesting
};
} // namespace UnitTesting
