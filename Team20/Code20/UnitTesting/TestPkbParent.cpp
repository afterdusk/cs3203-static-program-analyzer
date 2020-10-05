#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestParentQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests = SetUpTests::SetUpTests(pkb);

  TEST_METHOD(ParentLineNumberAndLineNumber) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c3, setUpTests.c4));
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c3, setUpTests.c5));
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c14, setUpTests.c15));
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c14, setUpTests.c23));
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c15, setUpTests.c16));
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c15, setUpTests.c19));
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c15, setUpTests.c22));
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c17, setUpTests.c18));
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c19, setUpTests.c21));
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c23, setUpTests.c24));

    Assert::IsFalse(
        pkb.getQueryInterface()->parent(setUpTests.c999, setUpTests.c1));
    Assert::IsFalse(
        pkb.getQueryInterface()->parent(setUpTests.c15, setUpTests.c999));
    Assert::IsFalse(
        pkb.getQueryInterface()->parent(setUpTests.c15, setUpTests.c23));
    Assert::IsFalse(
        pkb.getQueryInterface()->parent(setUpTests.c15, setUpTests.c21));
    Assert::IsFalse(
        pkb.getQueryInterface()->parent(setUpTests.c14, setUpTests.c16));
    Assert::IsFalse(
        pkb.getQueryInterface()->parent(setUpTests.c14, setUpTests.c22));
    Assert::IsFalse(
        pkb.getQueryInterface()->parent(setUpTests.c16, setUpTests.c18));

  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndStatement) {

    // Valid parent constant returns line numbers that matches statement type.
    Assert::IsTrue(pkb.getQueryInterface()->parent(
                       setUpTests.c3, setUpTests.a) == LINE_SET{setUpTests.l4});
    Assert::IsTrue(pkb.getQueryInterface()->parent(
                       setUpTests.c3, setUpTests.r) == LINE_SET{setUpTests.l5});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c14, setUpTests.i) ==
        LINE_SET{setUpTests.l15});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c14, setUpTests.w) ==
        LINE_SET{setUpTests.l23});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c15, setUpTests.a) ==
        LINE_SET{setUpTests.l16});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c15, setUpTests.w) ==
        LINE_SET{setUpTests.l17});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c15, setUpTests.i) ==
        LINE_SET{setUpTests.l19});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c17, setUpTests.a) ==
        LINE_SET{setUpTests.l18});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c19, setUpTests.a) ==
        LINE_SET{setUpTests.l20});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c19, setUpTests.p) ==
        LINE_SET{setUpTests.l21});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c23, setUpTests.a) ==
        LINE_SET{setUpTests.l24});

    // Invalid parent constants that aren't parents, return empty set
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c2, setUpTests.r) ==
        LINE_SET()); // constant on read
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c4, setUpTests.a) ==
        LINE_SET()); // constant on assign
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c21, setUpTests.p) ==
        LINE_SET()); // constant on print
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c11, setUpTests.c) ==
        LINE_SET()); // constant on call

  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndStmt) {

    // Valid parent
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c3, setUpTests.stmt) ==
        LINE_SET{setUpTests.l4, setUpTests.l5});

    // Invalid parent
    Assert::IsTrue(pkb.getQueryInterface()->parent(
                       setUpTests.c16, setUpTests.stmt) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndProgLine) {

    // Valid parent
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c3, setUpTests.n) ==
        LINE_SET{setUpTests.l4, setUpTests.l5});

    // Invalid parent
    Assert::IsTrue(pkb.getQueryInterface()->parent(setUpTests.c16,
                                                   setUpTests.n) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndUnderscore) {

    // Valid parent
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.c3, setUpTests.underscore));

    // Invalid parent
    Assert::IsFalse(
        pkb.getQueryInterface()->parent(setUpTests.c16, setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(ParentStatementAndLineNumber) {

    // Valid parent
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.w, setUpTests.c4) ==
        LINE_SET{setUpTests.l3}); // LineNumber is in container and Parent
                                  // stmt type is correct

    // Invalid parent
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.i, setUpTests.c4) ==
        LINE_SET()); // Parent stmt is not the correct type
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.w, setUpTests.c13) ==
        LINE_SET()); // LineNumber does not belong in any container
  }                  // namespace UnitTesting

  TEST_METHOD(ParentStatementAndStatement) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.i, setUpTests.a) ==
        LINE_LINE_PAIRS(LINE_VECTOR{setUpTests.l19, setUpTests.l15},
                        LINE_VECTOR{setUpTests.l20, setUpTests.l16}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStatementAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.i, setUpTests.stmt) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{
                setUpTests.l19,
                setUpTests.l19,
                setUpTests.l15,
                setUpTests.l15,
                setUpTests.l15,
                setUpTests.l15,
            },
            LINE_VECTOR{
                setUpTests.l20,
                setUpTests.l21,
                setUpTests.l16,
                setUpTests.l17,
                setUpTests.l19,
                setUpTests.l22,
            }));
  } // namespace UnitTesting

  TEST_METHOD(ParentStatementAndProgLine) {
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.i, setUpTests.n) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{
                setUpTests.l19,
                setUpTests.l19,
                setUpTests.l15,
                setUpTests.l15,
                setUpTests.l15,
                setUpTests.l15,
            },
            LINE_VECTOR{
                setUpTests.l20,
                setUpTests.l21,
                setUpTests.l16,
                setUpTests.l17,
                setUpTests.l19,
                setUpTests.l22,
            }));
  } // namespace UnitTesting
  TEST_METHOD(ParentStatementAndUnderscore) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.w, setUpTests.underscore) ==
        LINE_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                 setUpTests.l23});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.i, setUpTests.underscore) ==
        LINE_SET{setUpTests.l15, setUpTests.l19});
    Assert::IsTrue(pkb.getQueryInterface()->parent(
                       setUpTests.c, setUpTests.underscore) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndLineNumber) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.stmt, setUpTests.c15) ==
        LINE_SET{setUpTests.l14});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.stmt, setUpTests.c23) ==
        LINE_SET{setUpTests.l14});

    // line number is not a child
    Assert::IsTrue(pkb.getQueryInterface()->parent(
                       setUpTests.stmt, setUpTests.c14) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentProgLineAndLineNumber) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.n, setUpTests.c15) ==
        LINE_SET{setUpTests.l14});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.n, setUpTests.c23) ==
        LINE_SET{setUpTests.l14});

    // line number is not a child
    Assert::IsTrue(pkb.getQueryInterface()->parent(
                       setUpTests.n, setUpTests.c14) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndStatement) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.stmt, setUpTests.a) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l19, setUpTests.l17,
                        setUpTests.l15, setUpTests.l23},
            LINE_VECTOR{setUpTests.l4, setUpTests.l20, setUpTests.l18,
                        setUpTests.l16, setUpTests.l24}));
  } // namespace UnitTesting

  TEST_METHOD(ParentProgLineAndStatement) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.n, setUpTests.a) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l19, setUpTests.l17,
                        setUpTests.l15, setUpTests.l23},
            LINE_VECTOR{setUpTests.l4, setUpTests.l20, setUpTests.l18,
                        setUpTests.l16, setUpTests.l24}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndStmt) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.stmt, setUpTests.stmt) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{
                setUpTests.l3,
                setUpTests.l3,
                setUpTests.l19,
                setUpTests.l19,
                setUpTests.l14,
                setUpTests.l14,
                setUpTests.l15,
                setUpTests.l15,
                setUpTests.l15,
                setUpTests.l15,
                setUpTests.l23,
                setUpTests.l17,
            },
            LINE_VECTOR{
                setUpTests.l4,
                setUpTests.l5,
                setUpTests.l20,
                setUpTests.l21,
                setUpTests.l15,
                setUpTests.l23,
                setUpTests.l16,
                setUpTests.l17,
                setUpTests.l19,
                setUpTests.l22,
                setUpTests.l24,
                setUpTests.l18,
            }));
  } // namespace UnitTesting

  TEST_METHOD(ParentProgLineAndProgLine) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.n, setUpTests.n) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{
                setUpTests.l3,
                setUpTests.l3,
                setUpTests.l19,
                setUpTests.l19,
                setUpTests.l14,
                setUpTests.l14,
                setUpTests.l15,
                setUpTests.l15,
                setUpTests.l15,
                setUpTests.l15,
                setUpTests.l23,
                setUpTests.l17,
            },
            LINE_VECTOR{
                setUpTests.l4,
                setUpTests.l5,
                setUpTests.l20,
                setUpTests.l21,
                setUpTests.l15,
                setUpTests.l23,
                setUpTests.l16,
                setUpTests.l17,
                setUpTests.l19,
                setUpTests.l22,
                setUpTests.l24,
                setUpTests.l18,
            }));
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndUnderscore) {

    Assert::IsTrue(pkb.getQueryInterface()->parent(setUpTests.stmt,
                                                   setUpTests.underscore) ==
                   LINE_SET{setUpTests.l23, setUpTests.l3, setUpTests.l14,
                            setUpTests.l15, setUpTests.l17, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(ParentProgLineAndUnderscore) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.n, setUpTests.underscore) ==
        LINE_SET{setUpTests.l23, setUpTests.l3, setUpTests.l14, setUpTests.l15,
                 setUpTests.l17, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndLineNumber) {

    // line number is a child
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.underscore, setUpTests.c15));

    // line number is not a child
    Assert::IsFalse(
        pkb.getQueryInterface()->parent(setUpTests.underscore, setUpTests.c14));
    Assert::IsFalse(
        pkb.getQueryInterface()->parent(setUpTests.underscore, setUpTests.c1));
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndStatement) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.underscore, setUpTests.w) ==
        LINE_SET{setUpTests.l23, setUpTests.l17});
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.underscore, setUpTests.a) ==
        LINE_SET{setUpTests.l4, setUpTests.l16, setUpTests.l18, setUpTests.l20,
                 setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndStmt) {
    Assert::IsTrue(pkb.getQueryInterface()->parent(setUpTests.underscore,
                                                   setUpTests.stmt) ==
                   LINE_SET{setUpTests.l4, setUpTests.l5, setUpTests.l15,
                            setUpTests.l23, setUpTests.l16, setUpTests.l17,
                            setUpTests.l19, setUpTests.l22, setUpTests.l18,
                            setUpTests.l20, setUpTests.l21, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndProgLine) {
    Assert::IsTrue(
        pkb.getQueryInterface()->parent(setUpTests.underscore, setUpTests.n) ==
        LINE_SET{setUpTests.l4, setUpTests.l5, setUpTests.l15, setUpTests.l23,
                 setUpTests.l16, setUpTests.l17, setUpTests.l19, setUpTests.l22,
                 setUpTests.l18, setUpTests.l20, setUpTests.l21,
                 setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->parent(setUpTests.underscore,
                                                   setUpTests.underscore));
  } // namespace UnitTesting

  /*
   * ParentStar query tests
   */
  TEST_METHOD(ParentStarLineNumberAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
        setUpTests.c3, setUpTests.c4)); // 1 level
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c14,
                                            setUpTests.c23)); // 1 level
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c14,
                                            setUpTests.c17)); // 2 levels
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
        setUpTests.c14,
        setUpTests.c22)); // 2 levels but in else block
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c14,
                                            setUpTests.c20)); // 3 levels
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
        setUpTests.c14,
        setUpTests.c21)); // 3 levels but in else block

    Assert::IsFalse(pkb.getQueryInterface()->parentStar(
        setUpTests.c999,
        setUpTests.c1)); // invalid line number
    Assert::IsFalse(pkb.getQueryInterface()->parentStar(
        setUpTests.c15,
        setUpTests.c999)); // invalid line number
    Assert::IsFalse(pkb.getQueryInterface()->parentStar(
        setUpTests.c15,
        setUpTests.c23)); // not parent of each other
    Assert::IsFalse(pkb.getQueryInterface()->parentStar(
        setUpTests.c21,
        setUpTests.c15)); // child - parent instead of parent - child
  }                       // namespace UnitTesting

  TEST_METHOD(ParentStarLineNumberAndStatement) {
    // Valid parentStar constant returns line numbers that match statement type.
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
                       setUpTests.c3, setUpTests.a) == LINE_SET{setUpTests.l4});
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c14, setUpTests.i) ==
        LINE_SET{setUpTests.l15, setUpTests.l19});
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c14, setUpTests.w) ==
        LINE_SET{setUpTests.l23, setUpTests.l17});
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c14, setUpTests.a) ==
        LINE_SET{setUpTests.l16, setUpTests.l18, setUpTests.l20,
                 setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c14, setUpTests.p) ==
        LINE_SET{setUpTests.l21, setUpTests.l22}); // children from else blocks

    // Invalid parentStar constants that aren't parents, return empty set
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c2, setUpTests.r) ==
        LINE_SET()); // constant on read
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c4, setUpTests.a) ==
        LINE_SET()); // constant on assign
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.c21,
                                                       setUpTests.p) ==
                   LINE_SET()); // constant on print
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.c11,
                                                       setUpTests.c) ==
                   LINE_SET()); // constant on call
  }                             // namespace UnitTesting

  TEST_METHOD(ParentStarLineNumberAndStmt) {
    // Valid parentStar
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c3, setUpTests.stmt) ==
        LINE_SET{setUpTests.l4, setUpTests.l5}); // only 1 level
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c14, setUpTests.stmt) ==
        LINE_SET{setUpTests.l15, setUpTests.l23, setUpTests.l16, setUpTests.l17,
                 setUpTests.l19, setUpTests.l18, setUpTests.l20, setUpTests.l21,
                 setUpTests.l22, setUpTests.l24}); // nested 3 levels
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c15, setUpTests.stmt) ==
        LINE_SET{setUpTests.l16, setUpTests.l17, setUpTests.l19, setUpTests.l18,
                 setUpTests.l20, setUpTests.l21,
                 setUpTests.l22}); // nested 2 levels

    // Invalid parentStar
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.c16,
                                                       setUpTests.stmt) ==
                   LINE_SET()); // constant is not a parent statement
  }                             // namespace UnitTesting

  TEST_METHOD(ParentStarLineNumberAndProgLine) {
    // Valid parentStar
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c3, setUpTests.n) ==
        LINE_SET{setUpTests.l4, setUpTests.l5}); // only 1 level
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c14, setUpTests.n) ==
        LINE_SET{setUpTests.l15, setUpTests.l23, setUpTests.l16, setUpTests.l17,
                 setUpTests.l19, setUpTests.l18, setUpTests.l20, setUpTests.l21,
                 setUpTests.l22, setUpTests.l24}); // nested 3 levels
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.c15, setUpTests.n) ==
        LINE_SET{setUpTests.l16, setUpTests.l17, setUpTests.l19, setUpTests.l18,
                 setUpTests.l20, setUpTests.l21,
                 setUpTests.l22}); // nested 2 levels

    // Invalid parentStar
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.c16,
                                                       setUpTests.n) ==
                   LINE_SET()); // constant is not a parent statement
  }                             // namespace UnitTesting

  TEST_METHOD(ParentStarLineNumberAndUnderscore) {
    // Valid parentStar
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
        setUpTests.c3, setUpTests.underscore)); // only 1 level
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
        setUpTests.c14, setUpTests.underscore)); // has 3 nested levels
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
        setUpTests.c15, setUpTests.underscore)); // has 2 nested levels

    // Invalid parentStar
    Assert::IsFalse(pkb.getQueryInterface()->parentStar(
        setUpTests.c16,
        setUpTests.underscore)); // constant is not a parent statement
  }                              // namespace UnitTesting

  TEST_METHOD(ParentStarStatementAndLineNumber) {
    // Valid parentStar
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.w, setUpTests.c4) ==
        LINE_SET{setUpTests.l3}); // 1 level
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.w, setUpTests.c18) ==
        LINE_SET{setUpTests.l14, setUpTests.l17}); // nested 3 levels
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.w, setUpTests.c20) ==
        LINE_SET{
            setUpTests.l14}); // nested 3 levels but called within if statement
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.w, setUpTests.c21) ==
        LINE_SET{
            setUpTests.l14}); // nested 3 levels but called within else block

    // Invalid parentStar
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.i, setUpTests.c4) ==
        LINE_SET()); // Only has 1 level and parent is not correct type
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.w,
                                                       setUpTests.c13) ==
                   LINE_SET()); // LineNumber does not belong in any container
  }                             // namespace UnitTesting

  TEST_METHOD(ParentStarStatementAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.i, setUpTests.a) ==
        LINE_LINE_PAIRS(LINE_VECTOR{setUpTests.l19, setUpTests.l15,
                                    setUpTests.l15, setUpTests.l15},
                        LINE_VECTOR{setUpTests.l20, setUpTests.l20,
                                    setUpTests.l18, setUpTests.l16}));
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.w, setUpTests.a) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                        setUpTests.l14, setUpTests.l14, setUpTests.l23,
                        setUpTests.l14},
            LINE_VECTOR{setUpTests.l4, setUpTests.l20, setUpTests.l18,
                        setUpTests.l18, setUpTests.l16, setUpTests.l24,
                        setUpTests.l24}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStatementAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.i, setUpTests.stmt) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l15, setUpTests.l15, setUpTests.l15,
                        setUpTests.l15, setUpTests.l15, setUpTests.l15,
                        setUpTests.l15, setUpTests.l19, setUpTests.l19},
            LINE_VECTOR{setUpTests.l16, setUpTests.l17, setUpTests.l19,
                        setUpTests.l22, setUpTests.l18, setUpTests.l20,
                        setUpTests.l21, setUpTests.l20, setUpTests.l21}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStatementAndProgLine) {
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.i, setUpTests.n) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l15, setUpTests.l15, setUpTests.l15,
                        setUpTests.l15, setUpTests.l15, setUpTests.l15,
                        setUpTests.l15, setUpTests.l19, setUpTests.l19},
            LINE_VECTOR{setUpTests.l16, setUpTests.l17, setUpTests.l19,
                        setUpTests.l22, setUpTests.l18, setUpTests.l20,
                        setUpTests.l21, setUpTests.l20, setUpTests.l21}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStatementAndUnderscore) {

    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.w,
                                                       setUpTests.underscore) ==
                   LINE_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                            setUpTests.l23});
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.i,
                                                       setUpTests.underscore) ==
                   LINE_SET{setUpTests.l15, setUpTests.l19});
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
                       setUpTests.c, setUpTests.underscore) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStmtAndLineNumber) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.stmt, setUpTests.c18) ==
        LINE_SET{setUpTests.l14, setUpTests.l15,
                 setUpTests.l17}); // nested 3 levels
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.stmt, setUpTests.c21) ==
        LINE_SET{
            setUpTests.l14, setUpTests.l15,
            setUpTests.l19}); // nested 3 levels, constant inside else block

    // line number is not a child
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
                       setUpTests.stmt, setUpTests.c14) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStarProgLineAndLineNumber) {

    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.n, setUpTests.c18) ==
        LINE_SET{setUpTests.l14, setUpTests.l15,
                 setUpTests.l17}); // nested 3 levels
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.n, setUpTests.c21) ==
        LINE_SET{
            setUpTests.l14, setUpTests.l15,
            setUpTests.l19}); // nested 3 levels, constant inside else block

    // line number is not a child
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
                       setUpTests.n, setUpTests.c14) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStmtAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.stmt, setUpTests.a) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l19, setUpTests.l15,
                        setUpTests.l14, setUpTests.l17, setUpTests.l15,
                        setUpTests.l14, setUpTests.l15, setUpTests.l14,
                        setUpTests.l23, setUpTests.l14},
            LINE_VECTOR{
                setUpTests.l4,
                setUpTests.l20,
                setUpTests.l20,
                setUpTests.l20,
                setUpTests.l18,
                setUpTests.l18,
                setUpTests.l18,
                setUpTests.l16,
                setUpTests.l16,
                setUpTests.l24,
                setUpTests.l24,
            }));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarProgLineAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.n, setUpTests.a) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l19, setUpTests.l15,
                        setUpTests.l14, setUpTests.l17, setUpTests.l15,
                        setUpTests.l14, setUpTests.l15, setUpTests.l14,
                        setUpTests.l23, setUpTests.l14},
            LINE_VECTOR{
                setUpTests.l4,
                setUpTests.l20,
                setUpTests.l20,
                setUpTests.l20,
                setUpTests.l18,
                setUpTests.l18,
                setUpTests.l18,
                setUpTests.l16,
                setUpTests.l16,
                setUpTests.l24,
                setUpTests.l24,
            }));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStmtAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.stmt, setUpTests.stmt) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{
                setUpTests.l19, setUpTests.l19, setUpTests.l3,  setUpTests.l3,
                setUpTests.l14, setUpTests.l14, setUpTests.l14, setUpTests.l14,
                setUpTests.l14, setUpTests.l14, setUpTests.l14, setUpTests.l14,
                setUpTests.l14, setUpTests.l14, setUpTests.l23, setUpTests.l15,
                setUpTests.l15, setUpTests.l15, setUpTests.l15, setUpTests.l15,
                setUpTests.l15, setUpTests.l15, setUpTests.l17,
            },
            LINE_VECTOR{
                setUpTests.l20, setUpTests.l21, setUpTests.l4,  setUpTests.l5,
                setUpTests.l23, setUpTests.l15, setUpTests.l16, setUpTests.l17,
                setUpTests.l19, setUpTests.l22, setUpTests.l18, setUpTests.l20,
                setUpTests.l21, setUpTests.l24, setUpTests.l24, setUpTests.l16,
                setUpTests.l17, setUpTests.l19, setUpTests.l22, setUpTests.l18,
                setUpTests.l20, setUpTests.l21, setUpTests.l18,
            }));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarProgLineAndProgLine) {
    Assert::IsTrue(
        pkb.getQueryInterface()->parentStar(setUpTests.n, setUpTests.n) ==
        LINE_LINE_PAIRS(
            LINE_VECTOR{
                setUpTests.l19, setUpTests.l19, setUpTests.l3,  setUpTests.l3,
                setUpTests.l14, setUpTests.l14, setUpTests.l14, setUpTests.l14,
                setUpTests.l14, setUpTests.l14, setUpTests.l14, setUpTests.l14,
                setUpTests.l14, setUpTests.l14, setUpTests.l23, setUpTests.l15,
                setUpTests.l15, setUpTests.l15, setUpTests.l15, setUpTests.l15,
                setUpTests.l15, setUpTests.l15, setUpTests.l17,
            },
            LINE_VECTOR{
                setUpTests.l20, setUpTests.l21, setUpTests.l4,  setUpTests.l5,
                setUpTests.l23, setUpTests.l15, setUpTests.l16, setUpTests.l17,
                setUpTests.l19, setUpTests.l22, setUpTests.l18, setUpTests.l20,
                setUpTests.l21, setUpTests.l24, setUpTests.l24, setUpTests.l16,
                setUpTests.l17, setUpTests.l19, setUpTests.l22, setUpTests.l18,
                setUpTests.l20, setUpTests.l21, setUpTests.l18,
            }));
  } // namespace UnitTesting
  TEST_METHOD(ParentStarStmtAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.stmt,
                                                       setUpTests.underscore) ==
                   LINE_SET{setUpTests.l23, setUpTests.l3, setUpTests.l14,
                            setUpTests.l15, setUpTests.l17, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(ParentStarProgLineAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.n,
                                                       setUpTests.underscore) ==
                   LINE_SET{setUpTests.l23, setUpTests.l3, setUpTests.l14,
                            setUpTests.l15, setUpTests.l17, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(ParentStarUnderscoreAndLineNumber) {

    // line number is a child
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.underscore,
                                                       setUpTests.c15));
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(
        setUpTests.underscore, setUpTests.c21)); // child inside else block

    // line number is not a child
    Assert::IsFalse(pkb.getQueryInterface()->parentStar(setUpTests.underscore,
                                                        setUpTests.c14));
    Assert::IsFalse(pkb.getQueryInterface()->parentStar(setUpTests.underscore,
                                                        setUpTests.c1));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarUnderscoreAndStatement) {
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.underscore,
                                                       setUpTests.w) ==
                   LINE_SET{setUpTests.l23, setUpTests.l17});
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.underscore,
                                                       setUpTests.a) ==
                   LINE_SET{setUpTests.l4, setUpTests.l16, setUpTests.l18,
                            setUpTests.l20, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentStarUnderscoreAndStmt) {
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.underscore,
                                                       setUpTests.stmt) ==
                   LINE_SET{setUpTests.l4, setUpTests.l5, setUpTests.l15,
                            setUpTests.l23, setUpTests.l16, setUpTests.l17,
                            setUpTests.l19, setUpTests.l22, setUpTests.l18,
                            setUpTests.l20, setUpTests.l21, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentStarUnderscoreAndProgLine) {
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.underscore,
                                                       setUpTests.n) ==
                   LINE_SET{setUpTests.l4, setUpTests.l5, setUpTests.l15,
                            setUpTests.l23, setUpTests.l16, setUpTests.l17,
                            setUpTests.l19, setUpTests.l22, setUpTests.l18,
                            setUpTests.l20, setUpTests.l21, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentStarUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->parentStar(setUpTests.underscore,
                                                       setUpTests.underscore));
  } // namespace UnitTesting
};

} // namespace UnitTesting
