#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestNextQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests =
      SetUpTests::SetUpTests(pkb, SetUpTests::TestNumber::A);

  TEST_METHOD(NextLineNumberAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.c1, setUpTests.c2)); // arg1 is normal stmt
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.c3,
        setUpTests.c4)); // arg1 is while stmt, arg2 is stmt inside while
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.c3,
        setUpTests.c6)); // arg1 is normal stmt, arg2 is stmt after while
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.c15,
        setUpTests.c16)); // arg1 is if stmt, arg2 is line in then block
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.c15,
        setUpTests.c22)); // arg1 is normal stmt, arg2 is line in else block
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.c20,
        setUpTests.c23)); // arg1 is last stmt of nested if block
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.c21,
        setUpTests.c23)); // arg1 is last stmt of nested else block
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.c22, setUpTests.c23)); // arg1 is last stmt of else block

    Assert::IsFalse(pkb.getQueryInterface()->next(
        setUpTests.c6, setUpTests.c7)); // different procedure
    Assert::IsFalse(pkb.getQueryInterface()->next(
        setUpTests.c21, setUpTests.c22)); // arg1 is last line in then block,
                                          // arg2 is first line in else block
  }                                       // namespace UnitTesting

  TEST_METHOD(NextLineNumberAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c12, setUpTests.r) ==
        LINE_SET{setUpTests.l13});
    Assert::IsTrue(pkb.getQueryInterface()->next(setUpTests.c12,
                                                 setUpTests.a) == LINE_SET());
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c24, setUpTests.w) ==
        LINE_SET{setUpTests.l23});
    Assert::IsTrue(pkb.getQueryInterface()->next(setUpTests.c24,
                                                 setUpTests.p) == LINE_SET());
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c14, setUpTests.c) ==
        LINE_SET{setUpTests.l25});
  } // namespace UnitTesting

  TEST_METHOD(NextLineNumberAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c14, setUpTests.stmt) ==
        LINE_SET{setUpTests.l15, setUpTests.l25}); // line of while statement
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c15, setUpTests.stmt) ==
        LINE_SET{setUpTests.l16, setUpTests.l22}); // line of if statement
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c20, setUpTests.stmt) ==
        LINE_SET{setUpTests.l23}); // last line inside then block
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c25, setUpTests.stmt) ==
        LINE_SET()); // last line of procedure
  }                  // namespace UnitTesting

  TEST_METHOD(NextLineNumberAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c13, setUpTests.underscore));
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c14, setUpTests.underscore));
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c18, setUpTests.underscore));
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c21, setUpTests.underscore));

    Assert::IsFalse(pkb.getQueryInterface()->next(
        setUpTests.c11, setUpTests.underscore)); // last line of procedure
  }                                              // namespace UnitTesting

  TEST_METHOD(NextStatementAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.p, setUpTests.c23) ==
        LINE_SET{setUpTests.l21,
                 setUpTests.l22}); // line after some nested blocks
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.a, setUpTests.c23) ==
        LINE_SET{setUpTests.l20,
                 setUpTests.l24}); // line after some nested blocks
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c, setUpTests.c23) ==
        LINE_SET()); // line after some nested blocks
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.w, setUpTests.c18) ==
        LINE_SET{setUpTests.l17}); // inside while block
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c, setUpTests.c18) ==
        LINE_SET()); // inside while block
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.i, setUpTests.c20) ==
        LINE_SET{setUpTests.l19}); // inside then block
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.p, setUpTests.c20) ==
        LINE_SET()); // inside then block
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.i, setUpTests.c21) ==
        LINE_SET{setUpTests.l19}); // inside else block
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.a, setUpTests.c21) ==
        LINE_SET()); // inside else block
    Assert::IsTrue(pkb.getQueryInterface()->next(setUpTests.a, setUpTests.c1) ==
                   LINE_SET()); // first line in procedure
  }                             // namespace UnitTesting

  TEST_METHOD(NextStatementAndStatement) {
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->next(
                       setUpTests.i, setUpTests.a)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l15, setUpTests.l19},
                       LINE_VECTOR{setUpTests.l16, setUpTests.l20})));
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->next(
                       setUpTests.i, setUpTests.p)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l15, setUpTests.l19},
                       LINE_VECTOR{setUpTests.l22, setUpTests.l21})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->next(setUpTests.w, setUpTests.a)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l17, setUpTests.l23},
            LINE_VECTOR{setUpTests.l4, setUpTests.l18, setUpTests.l24})));
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->next(
                       setUpTests.a, setUpTests.w)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l18, setUpTests.l24,
                                   setUpTests.l20, setUpTests.l16},
                       LINE_VECTOR{setUpTests.l17, setUpTests.l23,
                                   setUpTests.l23, setUpTests.l17})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->next(setUpTests.w, setUpTests.w)) ==
        convertToOrderedList(LINE_LINE_PAIRS(LINE_VECTOR{setUpTests.l23},
                                             LINE_VECTOR{setUpTests.l14})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->next(setUpTests.r, setUpTests.r)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l1, setUpTests.l8, setUpTests.l12},
            LINE_VECTOR{setUpTests.l2, setUpTests.l9, setUpTests.l13})));
  } // namespace UnitTesting

  TEST_METHOD(NextStatementAndStmt) {
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->next(
                       setUpTests.i, setUpTests.stmt)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l15, setUpTests.l15,
                                   setUpTests.l19, setUpTests.l19},
                       LINE_VECTOR{setUpTests.l16, setUpTests.l22,
                                   setUpTests.l20, setUpTests.l21})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->next(setUpTests.w, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l3, setUpTests.l14,
                        setUpTests.l14, setUpTests.l17, setUpTests.l17,
                        setUpTests.l23, setUpTests.l23},
            LINE_VECTOR{setUpTests.l4, setUpTests.l6, setUpTests.l15,
                        setUpTests.l25, setUpTests.l18, setUpTests.l19,
                        setUpTests.l24, setUpTests.l14})));
  } // namespace UnitTesting

  TEST_METHOD(NextStatementAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.c, setUpTests.underscore) ==
        LINE_SET{setUpTests.l7});
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.p, setUpTests.underscore) ==
        LINE_SET{setUpTests.l21, setUpTests.l22});
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.w, setUpTests.underscore) ==
        LINE_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                 setUpTests.l23});
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.a, setUpTests.underscore) ==
        LINE_SET{setUpTests.l4, setUpTests.l10, setUpTests.l16, setUpTests.l18,
                 setUpTests.l20, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(NextStmtAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.stmt, setUpTests.c23) ==
        LINE_SET{setUpTests.l20, setUpTests.l21, setUpTests.l22,
                 setUpTests.l24}); // line after some nested blocks
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.stmt, setUpTests.c18) ==
        LINE_SET{setUpTests.l17}); // inside while block
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.stmt, setUpTests.c20) ==
        LINE_SET{setUpTests.l19}); // inside then block
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.stmt, setUpTests.c21) ==
        LINE_SET{setUpTests.l19}); // inside else block

    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.stmt, setUpTests.c12) ==
        LINE_SET()); // first line in procedure

  } // namespace UnitTesting

  TEST_METHOD(NextStmtAndStatement) {
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->next(
                       setUpTests.stmt, setUpTests.r)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l1, setUpTests.l4, setUpTests.l7,
                                   setUpTests.l8, setUpTests.l12},
                       LINE_VECTOR{setUpTests.l2, setUpTests.l5, setUpTests.l8,
                                   setUpTests.l9, setUpTests.l13})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->next(setUpTests.stmt, setUpTests.a)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l9, setUpTests.l15,
                        setUpTests.l17, setUpTests.l19, setUpTests.l23},
            LINE_VECTOR{setUpTests.l4, setUpTests.l10, setUpTests.l16,
                        setUpTests.l18, setUpTests.l20, setUpTests.l24})));
  } // namespace UnitTesting

  TEST_METHOD(NextStmtAndStmt) {
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->next(setUpTests.stmt, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{
                setUpTests.l1,  setUpTests.l2,  setUpTests.l3,  setUpTests.l3,
                setUpTests.l4,  setUpTests.l5,  setUpTests.l7,  setUpTests.l8,
                setUpTests.l9,  setUpTests.l10, setUpTests.l12, setUpTests.l13,
                setUpTests.l14, setUpTests.l14, setUpTests.l15, setUpTests.l15,
                setUpTests.l16, setUpTests.l17, setUpTests.l17, setUpTests.l18,
                setUpTests.l19, setUpTests.l19, setUpTests.l20, setUpTests.l21,
                setUpTests.l22, setUpTests.l23, setUpTests.l23, setUpTests.l24},
            LINE_VECTOR{setUpTests.l2,  setUpTests.l3,  setUpTests.l4,
                        setUpTests.l6,  setUpTests.l5,  setUpTests.l3,
                        setUpTests.l8,  setUpTests.l9,  setUpTests.l10,
                        setUpTests.l11, setUpTests.l13, setUpTests.l14,
                        setUpTests.l15, setUpTests.l25, setUpTests.l16,
                        setUpTests.l22, setUpTests.l17, setUpTests.l18,
                        setUpTests.l19, setUpTests.l17, setUpTests.l20,
                        setUpTests.l21, setUpTests.l23, setUpTests.l23,
                        setUpTests.l23, setUpTests.l24, setUpTests.l14,
                        setUpTests.l23})));
  } // namespace UnitTesting

  TEST_METHOD(NextStmtAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.stmt, setUpTests.underscore) ==
        LINE_SET{setUpTests.l1,  setUpTests.l2,  setUpTests.l3,  setUpTests.l4,
                 setUpTests.l5,  setUpTests.l7,  setUpTests.l8,  setUpTests.l9,
                 setUpTests.l10, setUpTests.l12, setUpTests.l13, setUpTests.l14,
                 setUpTests.l15, setUpTests.l16, setUpTests.l17, setUpTests.l18,
                 setUpTests.l19, setUpTests.l20, setUpTests.l21, setUpTests.l22,
                 setUpTests.l23, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(NextUnderscoreAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.underscore, setUpTests.c16)); // first line inside then block
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.underscore, setUpTests.c21)); // first line inside else block
    Assert::IsTrue(pkb.getQueryInterface()->next(
        setUpTests.underscore,
        setUpTests.c15)); // first line inside while block

    Assert::IsFalse(pkb.getQueryInterface()->next(
        setUpTests.underscore, setUpTests.c12)); // first line in procedure
  }                                              // namespace UnitTesting

  TEST_METHOD(NextUnderscoreAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.underscore, setUpTests.c) ==
        LINE_SET{setUpTests.l11, setUpTests.l25});
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.underscore, setUpTests.r) ==
        LINE_SET{setUpTests.l2, setUpTests.l5, setUpTests.l8, setUpTests.l9,
                 setUpTests.l13});
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.underscore, setUpTests.a) ==
        LINE_SET{setUpTests.l4, setUpTests.l10, setUpTests.l16, setUpTests.l18,
                 setUpTests.l20, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(NextUnderscoreAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->next(setUpTests.underscore, setUpTests.stmt) ==
        LINE_SET{setUpTests.l2,  setUpTests.l3,  setUpTests.l4,  setUpTests.l5,
                 setUpTests.l6,  setUpTests.l8,  setUpTests.l9,  setUpTests.l10,
                 setUpTests.l11, setUpTests.l13, setUpTests.l14, setUpTests.l15,
                 setUpTests.l16, setUpTests.l17, setUpTests.l18, setUpTests.l19,
                 setUpTests.l20, setUpTests.l21, setUpTests.l22, setUpTests.l23,
                 setUpTests.l24, setUpTests.l25});
  } // namespace UnitTesting

  TEST_METHOD(NextUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->next(setUpTests.underscore,
                                                 setUpTests.underscore));
  } // namespace UnitTesting

  /*
   * NextStar query tests
   */
  TEST_METHOD(NextStarLineNumberAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c1, setUpTests.c2)); // arg1 is normal stmt
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c3,
        setUpTests.c4)); // arg1 is while stmt, arg2 is stmt inside while
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c3,
        setUpTests.c6)); // arg1 is normal stmt, arg2 is stmt after while
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c15,
        setUpTests.c16)); // arg1 is if stmt, arg2 is line in then block
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c15,
        setUpTests.c22)); // arg1 is normal stmt, arg2 is line in else block
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c20,
        setUpTests.c23)); // arg1 is last stmt of nested if block
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c21,
        setUpTests.c23)); // arg1 is last stmt of nested else block
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c22, setUpTests.c23)); // arg1 is last stmt of else block
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c21, setUpTests.c22)); // arg1 is last line in then block,
                                          // arg2 is first line in else block
                                          // but both are inside a while loop
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c12,
        setUpTests.c18)); // transiting into nested if and while loops
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c20, setUpTests.c22)); // from then block to else block with
                                          // overarching while loop
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.c22,
        setUpTests.c20)); // going backwards from else block into then block
                          // with overarching while loop

    Assert::IsFalse(pkb.getQueryInterface()->nextStar(
        setUpTests.c6, setUpTests.c7)); // different procedure
    Assert::IsFalse(pkb.getQueryInterface()->nextStar(
        setUpTests.c4, setUpTests.c2)); // cannot go backwards without an
                                        // overarching while loop

  } // namespace UnitTesting

  TEST_METHOD(NextStarLineNumberAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c1, setUpTests.r) ==
        LINE_SET{setUpTests.l2, setUpTests.l5});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c7, setUpTests.c) ==
        LINE_SET{setUpTests.l11});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c12, setUpTests.c) ==
        LINE_SET{setUpTests.l25});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c21, setUpTests.c) ==
        LINE_SET{setUpTests.l25});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c13, setUpTests.a) ==
        LINE_SET{setUpTests.l16, setUpTests.l18, setUpTests.l20,
                 setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c22, setUpTests.a) ==
        LINE_SET{setUpTests.l16, setUpTests.l18, setUpTests.l20,
                 setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c25, setUpTests.a) ==
        LINE_SET()); // last stmt in procedure
  }                  // namespace UnitTesting

  TEST_METHOD(NextStarLineNumberAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c20, setUpTests.stmt) ==
        LINE_SET{setUpTests.l14, setUpTests.l15, setUpTests.l16, setUpTests.l17,
                 setUpTests.l18, setUpTests.l19, setUpTests.l20, setUpTests.l21,
                 setUpTests.l22, setUpTests.l23, setUpTests.l24,
                 setUpTests.l25});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c22, setUpTests.stmt) ==
        LINE_SET{setUpTests.l14, setUpTests.l15, setUpTests.l16, setUpTests.l17,
                 setUpTests.l18, setUpTests.l19, setUpTests.l20, setUpTests.l21,
                 setUpTests.l22, setUpTests.l23, setUpTests.l24,
                 setUpTests.l25});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c12, setUpTests.stmt) ==
        LINE_SET{setUpTests.l13, setUpTests.l14, setUpTests.l15, setUpTests.l16,
                 setUpTests.l17, setUpTests.l18, setUpTests.l19, setUpTests.l20,
                 setUpTests.l21, setUpTests.l22, setUpTests.l23, setUpTests.l24,
                 setUpTests.l25});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.c25, setUpTests.stmt) ==
        LINE_SET()); // last line of procedure
  }                  // namespace UnitTesting

  TEST_METHOD(NextStarLineNumberAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.c13,
                                                     setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.c14,
                                                     setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.c18,
                                                     setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.c21,
                                                     setUpTests.underscore));

    Assert::IsFalse(pkb.getQueryInterface()->nextStar(
        setUpTests.c11, setUpTests.underscore)); // last line of procedure
  }                                              // namespace UnitTesting

  TEST_METHOD(NextStarStatementAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.p, setUpTests.c16) ==
        LINE_SET{setUpTests.l21, setUpTests.l22});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.a, setUpTests.c14) ==
        LINE_SET{setUpTests.l20, setUpTests.l16, setUpTests.l18,
                 setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.a, setUpTests.c21) ==
        LINE_SET{setUpTests.l20, setUpTests.l16, setUpTests.l18,
                 setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.a, setUpTests.c25) ==
        LINE_SET{setUpTests.l20, setUpTests.l16, setUpTests.l18,
                 setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.r, setUpTests.c21) ==
        LINE_SET{setUpTests.l12, setUpTests.l13});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.r, setUpTests.c25) ==
        LINE_SET{setUpTests.l12, setUpTests.l13});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.i, setUpTests.c25) ==
        LINE_SET{setUpTests.l15, setUpTests.l19});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.i, setUpTests.c19) ==
        LINE_SET{setUpTests.l15, setUpTests.l19});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.w, setUpTests.c25) ==
        LINE_SET{setUpTests.l14, setUpTests.l17, setUpTests.l23});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.w, setUpTests.c14) ==
        LINE_SET{setUpTests.l14, setUpTests.l17, setUpTests.l23});

    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.a, setUpTests.c7) ==
        LINE_SET()); // first line in procedure
  }                  // namespace UnitTesting

  TEST_METHOD(NextStarStatementAndStatement) {
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->nextStar(
                       setUpTests.r, setUpTests.c)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l8, setUpTests.l9, setUpTests.l12,
                                   setUpTests.l13},
                       LINE_VECTOR{setUpTests.l11, setUpTests.l11,
                                   setUpTests.l25, setUpTests.l25})));
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->nextStar(
                       setUpTests.p, setUpTests.p)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l21, setUpTests.l21,
                                   setUpTests.l22, setUpTests.l22},
                       LINE_VECTOR{setUpTests.l21, setUpTests.l22,
                                   setUpTests.l21, setUpTests.l22})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->nextStar(setUpTests.a, setUpTests.a)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l4, setUpTests.l16, setUpTests.l16,
                        setUpTests.l16, setUpTests.l16, setUpTests.l18,
                        setUpTests.l18, setUpTests.l18, setUpTests.l18,
                        setUpTests.l20, setUpTests.l20, setUpTests.l20,
                        setUpTests.l20, setUpTests.l24, setUpTests.l24,
                        setUpTests.l24, setUpTests.l24},
            LINE_VECTOR{setUpTests.l4, setUpTests.l16, setUpTests.l18,
                        setUpTests.l20, setUpTests.l24, setUpTests.l16,
                        setUpTests.l18, setUpTests.l20, setUpTests.l24,
                        setUpTests.l16, setUpTests.l18, setUpTests.l20,
                        setUpTests.l24, setUpTests.l16, setUpTests.l18,
                        setUpTests.l20, setUpTests.l24})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->nextStar(setUpTests.w, setUpTests.w)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l14, setUpTests.l14,
                        setUpTests.l14, setUpTests.l17, setUpTests.l17,
                        setUpTests.l17, setUpTests.l23, setUpTests.l23,
                        setUpTests.l23},
            LINE_VECTOR{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                        setUpTests.l23, setUpTests.l14, setUpTests.l17,
                        setUpTests.l23, setUpTests.l14, setUpTests.l17,
                        setUpTests.l23})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->nextStar(setUpTests.w, setUpTests.p)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                        setUpTests.l23, setUpTests.l14, setUpTests.l17,
                        setUpTests.l23},
            LINE_VECTOR{setUpTests.l6, setUpTests.l21, setUpTests.l21,
                        setUpTests.l21, setUpTests.l22, setUpTests.l22,
                        setUpTests.l22})));
  } // namespace UnitTesting

  TEST_METHOD(NextStarStatementAndStmt) {
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->nextStar(
                       setUpTests.c, setUpTests.stmt)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l7, setUpTests.l7, setUpTests.l7,
                                   setUpTests.l7},
                       LINE_VECTOR{setUpTests.l8, setUpTests.l9, setUpTests.l10,
                                   setUpTests.l11})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->nextStar(setUpTests.p, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{
                setUpTests.l21, setUpTests.l21, setUpTests.l21, setUpTests.l21,
                setUpTests.l21, setUpTests.l21, setUpTests.l21, setUpTests.l21,
                setUpTests.l21, setUpTests.l21, setUpTests.l21, setUpTests.l21,
                setUpTests.l22, setUpTests.l22, setUpTests.l22, setUpTests.l22,
                setUpTests.l22, setUpTests.l22, setUpTests.l22, setUpTests.l22,
                setUpTests.l22, setUpTests.l22, setUpTests.l22, setUpTests.l22,
            },
            LINE_VECTOR{setUpTests.l14, setUpTests.l15, setUpTests.l16,
                        setUpTests.l17, setUpTests.l18, setUpTests.l19,
                        setUpTests.l20, setUpTests.l21, setUpTests.l22,
                        setUpTests.l23, setUpTests.l24, setUpTests.l25,
                        setUpTests.l14, setUpTests.l15, setUpTests.l16,
                        setUpTests.l17, setUpTests.l18, setUpTests.l19,
                        setUpTests.l20, setUpTests.l21, setUpTests.l22,
                        setUpTests.l23, setUpTests.l24, setUpTests.l25})));

  } // namespace UnitTesting

  TEST_METHOD(NextStarStatementAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.c,
                                                     setUpTests.underscore) ==
                   LINE_SET{setUpTests.l7});
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.p,
                                                     setUpTests.underscore) ==
                   LINE_SET{setUpTests.l21, setUpTests.l22});
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.w,
                                                     setUpTests.underscore) ==
                   LINE_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                            setUpTests.l23});
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.a,
                                                     setUpTests.underscore) ==
                   LINE_SET{setUpTests.l4, setUpTests.l10, setUpTests.l16,
                            setUpTests.l18, setUpTests.l20, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(NextStarStmtAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.stmt, setUpTests.c4) ==
        LINE_SET{setUpTests.l1, setUpTests.l2, setUpTests.l3, setUpTests.l4,
                 setUpTests.l5});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.stmt, setUpTests.c13) ==
        LINE_SET{setUpTests.l12});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.stmt, setUpTests.c14) ==
        LINE_SET{setUpTests.l12, setUpTests.l13, setUpTests.l14, setUpTests.l15,
                 setUpTests.l16, setUpTests.l17, setUpTests.l18, setUpTests.l19,
                 setUpTests.l20, setUpTests.l21, setUpTests.l22, setUpTests.l23,
                 setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.stmt, setUpTests.c20) ==
        LINE_SET{setUpTests.l12, setUpTests.l13, setUpTests.l14, setUpTests.l15,
                 setUpTests.l16, setUpTests.l17, setUpTests.l18, setUpTests.l19,
                 setUpTests.l20, setUpTests.l21, setUpTests.l22, setUpTests.l23,
                 setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.stmt, setUpTests.c25) ==
        LINE_SET{setUpTests.l12, setUpTests.l13, setUpTests.l14, setUpTests.l15,
                 setUpTests.l16, setUpTests.l17, setUpTests.l18, setUpTests.l19,
                 setUpTests.l20, setUpTests.l21, setUpTests.l22, setUpTests.l23,
                 setUpTests.l24}); // last line in procedure

    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.stmt, setUpTests.c12) ==
        LINE_SET()); // first line in procedure

  } // namespace UnitTesting

  TEST_METHOD(NextStarStmtAndStatement) {
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->nextStar(
                       setUpTests.stmt, setUpTests.r)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l1, setUpTests.l1, setUpTests.l2,
                                   setUpTests.l3, setUpTests.l4, setUpTests.l5,
                                   setUpTests.l7, setUpTests.l7, setUpTests.l8,
                                   setUpTests.l12},
                       LINE_VECTOR{setUpTests.l2, setUpTests.l5, setUpTests.l5,
                                   setUpTests.l5, setUpTests.l5, setUpTests.l5,
                                   setUpTests.l8, setUpTests.l9, setUpTests.l9,
                                   setUpTests.l13})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->nextStar(setUpTests.stmt, setUpTests.c)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l7, setUpTests.l8, setUpTests.l9,
                        setUpTests.l10, setUpTests.l12, setUpTests.l13,
                        setUpTests.l14, setUpTests.l15, setUpTests.l16,
                        setUpTests.l17, setUpTests.l18, setUpTests.l19,
                        setUpTests.l20, setUpTests.l21, setUpTests.l22,
                        setUpTests.l23, setUpTests.l24},
            LINE_VECTOR{setUpTests.l11, setUpTests.l11, setUpTests.l11,
                        setUpTests.l11, setUpTests.l25, setUpTests.l25,
                        setUpTests.l25, setUpTests.l25, setUpTests.l25,
                        setUpTests.l25, setUpTests.l25, setUpTests.l25,
                        setUpTests.l25, setUpTests.l25, setUpTests.l25,
                        setUpTests.l25, setUpTests.l25})));
  } // namespace UnitTesting

  TEST_METHOD(NextStarStmtAndStmt) {
    Assert::IsTrue(
        convertToOrderedList(pkb.getQueryInterface()->nextStar(
            setUpTests.stmt, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{
                setUpTests.l1,  setUpTests.l1,  setUpTests.l1,  setUpTests.l1,
                setUpTests.l1,  setUpTests.l2,  setUpTests.l2,  setUpTests.l2,
                setUpTests.l2,  setUpTests.l3,  setUpTests.l3,  setUpTests.l3,
                setUpTests.l3,  setUpTests.l4,  setUpTests.l4,  setUpTests.l4,
                setUpTests.l4,  setUpTests.l5,  setUpTests.l5,  setUpTests.l5,
                setUpTests.l5,  setUpTests.l7,  setUpTests.l7,  setUpTests.l7,
                setUpTests.l7,  setUpTests.l8,  setUpTests.l8,  setUpTests.l8,
                setUpTests.l9,  setUpTests.l9,  setUpTests.l10, setUpTests.l12,
                setUpTests.l12, setUpTests.l12, setUpTests.l12, setUpTests.l12,
                setUpTests.l12, setUpTests.l12, setUpTests.l12, setUpTests.l12,
                setUpTests.l12, setUpTests.l12, setUpTests.l12, setUpTests.l12,
                setUpTests.l13, setUpTests.l13, setUpTests.l13, setUpTests.l13,
                setUpTests.l13, setUpTests.l13, setUpTests.l13, setUpTests.l13,
                setUpTests.l13, setUpTests.l13, setUpTests.l13, setUpTests.l13,
                setUpTests.l14, setUpTests.l14, setUpTests.l14, setUpTests.l14,
                setUpTests.l14, setUpTests.l14, setUpTests.l14, setUpTests.l14,
                setUpTests.l14, setUpTests.l14, setUpTests.l14, setUpTests.l14,
                setUpTests.l15, setUpTests.l15, setUpTests.l15, setUpTests.l15,
                setUpTests.l15, setUpTests.l15, setUpTests.l15, setUpTests.l15,
                setUpTests.l15, setUpTests.l15, setUpTests.l15, setUpTests.l15,
                setUpTests.l16, setUpTests.l16, setUpTests.l16, setUpTests.l16,
                setUpTests.l16, setUpTests.l16, setUpTests.l16, setUpTests.l16,
                setUpTests.l16, setUpTests.l16, setUpTests.l16, setUpTests.l16,
                setUpTests.l17, setUpTests.l17, setUpTests.l17, setUpTests.l17,
                setUpTests.l17, setUpTests.l17, setUpTests.l17, setUpTests.l17,
                setUpTests.l17, setUpTests.l17, setUpTests.l17, setUpTests.l17,
                setUpTests.l18, setUpTests.l18, setUpTests.l18, setUpTests.l18,
                setUpTests.l18, setUpTests.l18, setUpTests.l18, setUpTests.l18,
                setUpTests.l18, setUpTests.l18, setUpTests.l18, setUpTests.l18,
                setUpTests.l19, setUpTests.l19, setUpTests.l19, setUpTests.l19,
                setUpTests.l19, setUpTests.l19, setUpTests.l19, setUpTests.l19,
                setUpTests.l19, setUpTests.l19, setUpTests.l19, setUpTests.l19,
                setUpTests.l20, setUpTests.l20, setUpTests.l20, setUpTests.l20,
                setUpTests.l20, setUpTests.l20, setUpTests.l20, setUpTests.l20,
                setUpTests.l20, setUpTests.l20, setUpTests.l20, setUpTests.l20,
                setUpTests.l21, setUpTests.l21, setUpTests.l21, setUpTests.l21,
                setUpTests.l21, setUpTests.l21, setUpTests.l21, setUpTests.l21,
                setUpTests.l21, setUpTests.l21, setUpTests.l21, setUpTests.l21,
                setUpTests.l22, setUpTests.l22, setUpTests.l22, setUpTests.l22,
                setUpTests.l22, setUpTests.l22, setUpTests.l22, setUpTests.l22,
                setUpTests.l22, setUpTests.l22, setUpTests.l22, setUpTests.l22,
                setUpTests.l23, setUpTests.l23, setUpTests.l23, setUpTests.l23,
                setUpTests.l23, setUpTests.l23, setUpTests.l23, setUpTests.l23,
                setUpTests.l23, setUpTests.l23, setUpTests.l23, setUpTests.l23,
                setUpTests.l24, setUpTests.l24, setUpTests.l24, setUpTests.l24,
                setUpTests.l24, setUpTests.l24, setUpTests.l24, setUpTests.l24,
                setUpTests.l24, setUpTests.l24, setUpTests.l24, setUpTests.l24},
            LINE_VECTOR{setUpTests.l2,  setUpTests.l3,  setUpTests.l4,
                        setUpTests.l5,  setUpTests.l6,  setUpTests.l3,
                        setUpTests.l4,  setUpTests.l5,  setUpTests.l6,
                        setUpTests.l3,  setUpTests.l4,  setUpTests.l5,
                        setUpTests.l6,  setUpTests.l3,  setUpTests.l4,
                        setUpTests.l5,  setUpTests.l6,  setUpTests.l3,
                        setUpTests.l4,  setUpTests.l5,  setUpTests.l6,
                        setUpTests.l8,  setUpTests.l9,  setUpTests.l10,
                        setUpTests.l11, setUpTests.l9,  setUpTests.l10,
                        setUpTests.l11, setUpTests.l10, setUpTests.l11,
                        setUpTests.l11, setUpTests.l13, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25, setUpTests.l14,
                        setUpTests.l15, setUpTests.l16, setUpTests.l17,
                        setUpTests.l18, setUpTests.l19, setUpTests.l20,
                        setUpTests.l21, setUpTests.l22, setUpTests.l23,
                        setUpTests.l24, setUpTests.l25})));

  } // namespace UnitTesting

  TEST_METHOD(NextStarStmtAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.stmt,
                                          setUpTests.underscore) ==
        LINE_SET{setUpTests.l1,  setUpTests.l2,  setUpTests.l3,  setUpTests.l4,
                 setUpTests.l5,  setUpTests.l7,  setUpTests.l8,  setUpTests.l9,
                 setUpTests.l10, setUpTests.l12, setUpTests.l13, setUpTests.l14,
                 setUpTests.l15, setUpTests.l16, setUpTests.l17, setUpTests.l18,
                 setUpTests.l19, setUpTests.l20, setUpTests.l21, setUpTests.l22,
                 setUpTests.l23, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(NextStarUnderscoreAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.underscore, setUpTests.c16)); // first line inside then block
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.underscore, setUpTests.c21)); // first line inside else block
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(
        setUpTests.underscore,
        setUpTests.c15)); // first line inside while block

    Assert::IsFalse(pkb.getQueryInterface()->nextStar(
        setUpTests.underscore, setUpTests.c12)); // first line in procedure
  }                                              // namespace UnitTesting

  TEST_METHOD(NextStarUnderscoreAndStatement) {
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.underscore,
                                                     setUpTests.c) ==
                   LINE_SET{setUpTests.l11, setUpTests.l25});
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.underscore,
                                                     setUpTests.r) ==
                   LINE_SET{setUpTests.l2, setUpTests.l5, setUpTests.l8,
                            setUpTests.l9, setUpTests.l13});
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.underscore,
                                                     setUpTests.a) ==
                   LINE_SET{setUpTests.l4, setUpTests.l10, setUpTests.l16,
                            setUpTests.l18, setUpTests.l20, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(NextStarUnderscoreAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextStar(setUpTests.underscore,
                                          setUpTests.stmt) ==
        LINE_SET{setUpTests.l2,  setUpTests.l3,  setUpTests.l4,  setUpTests.l5,
                 setUpTests.l6,  setUpTests.l8,  setUpTests.l9,  setUpTests.l10,
                 setUpTests.l11, setUpTests.l13, setUpTests.l14, setUpTests.l15,
                 setUpTests.l16, setUpTests.l17, setUpTests.l18, setUpTests.l19,
                 setUpTests.l20, setUpTests.l21, setUpTests.l22, setUpTests.l23,
                 setUpTests.l24, setUpTests.l25});
  } // namespace UnitTesting

  TEST_METHOD(NextStarUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->nextStar(setUpTests.underscore,
                                                     setUpTests.underscore));
  } // namespace UnitTesting
};
} // namespace UnitTesting
