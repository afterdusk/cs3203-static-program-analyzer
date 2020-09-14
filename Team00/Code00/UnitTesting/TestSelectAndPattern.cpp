#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestSelectAndPatternQuery) {

public:
  SetUpTests setUpTests;

  TEST_METHOD_INITIALIZE(SetUpPkb) { setUpTests = SetUpTests::SetUpTests(); }

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
  } // namespace UnitTesting
};
} // namespace UnitTesting
