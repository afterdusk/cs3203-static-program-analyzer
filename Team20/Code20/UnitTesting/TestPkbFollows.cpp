#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestFollowsQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests =
      SetUpTests::SetUpTests(pkb, SetUpTests::TestNumber::A);

  TEST_METHOD(FollowsLineNumberAndLineNumber) {
    // Valid follows
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c1, setUpTests.c2));
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c2, setUpTests.c3));
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c3, setUpTests.c6));
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c4, setUpTests.c5));
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c7, setUpTests.c8));

    // Invalid follows
    Assert::IsFalse(
        pkb.getQueryInterface()->follows(setUpTests.c1, setUpTests.c3));
    Assert::IsFalse(
        pkb.getQueryInterface()->follows(setUpTests.c1, setUpTests.c5));
    Assert::IsFalse(
        pkb.getQueryInterface()->follows(setUpTests.c1, setUpTests.c6));
    Assert::IsFalse(
        pkb.getQueryInterface()->follows(setUpTests.c1, setUpTests.c7));
    Assert::IsFalse(
        pkb.getQueryInterface()->follows(setUpTests.c2, setUpTests.c4));
    Assert::IsFalse(
        pkb.getQueryInterface()->follows(setUpTests.c2, setUpTests.c8));
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndStatement) {
    // Valid follows when following statement type is true, returns statement
    // line number.
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c1, setUpTests.r) == LINE_SET{setUpTests.l2});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c2, setUpTests.w) == LINE_SET{setUpTests.l3});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c3, setUpTests.p) == LINE_SET{setUpTests.l6});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c4, setUpTests.r) == LINE_SET{setUpTests.l5});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c7, setUpTests.r) == LINE_SET{setUpTests.l8});

    // Invalid follows when following statement type is not true and returns 0.
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c7, setUpTests.w) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c1, setUpTests.a) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c3, setUpTests.r) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c4, setUpTests.c) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c2, setUpTests.i) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndStmt) {
    // Valid follows returns following line number else 0.
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c1, setUpTests.stmt) ==
        LINE_SET{setUpTests.l2});
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c2, setUpTests.stmt) ==
        LINE_SET{setUpTests.l3});
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c3, setUpTests.stmt) ==
        LINE_SET{setUpTests.l6});
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c4, setUpTests.stmt) ==
        LINE_SET{setUpTests.l5});
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c7, setUpTests.stmt) ==
        LINE_SET{setUpTests.l8});
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c8, setUpTests.stmt) ==
        LINE_SET{setUpTests.l9});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c999, setUpTests.stmt) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c1, setUpTests.underscore));
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c2, setUpTests.underscore));
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c3, setUpTests.underscore));
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c4, setUpTests.underscore));
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c7, setUpTests.underscore));
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.c8, setUpTests.underscore));

    Assert::IsFalse(pkb.getQueryInterface()->follows(setUpTests.c999,
                                                     setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndLineNumber) {
    // Valid follows if statement type of previous line number from given
    // lineNumber is correct, returns the statement line number. Else returns 0.
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.a, setUpTests.c1) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.r, setUpTests.c2) == LINE_SET{setUpTests.l1});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.a, setUpTests.c2) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.r, setUpTests.c3) == LINE_SET{setUpTests.l2});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.w, setUpTests.c4) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.a, setUpTests.c5) == LINE_SET{setUpTests.l4});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.w, setUpTests.c6) == LINE_SET{setUpTests.l3});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.i, setUpTests.c6) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.a, setUpTests.c7) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.c, setUpTests.c8) == LINE_SET{setUpTests.l7});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.r, setUpTests.c999) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndStatement) {
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->follows(setUpTests.r, setUpTests.r)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l1, setUpTests.l8, setUpTests.l12},
            LINE_VECTOR{setUpTests.l2, setUpTests.l9, setUpTests.l13})));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndStmt) {
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->follows(setUpTests.r, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l1, setUpTests.l9, setUpTests.l2,
                        setUpTests.l13, setUpTests.l8, setUpTests.l12},
            LINE_VECTOR{setUpTests.l2, setUpTests.l10, setUpTests.l3,
                        setUpTests.l14, setUpTests.l9, setUpTests.l13})));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.r, setUpTests.underscore) ==
        LINE_SET{setUpTests.l1, setUpTests.l2, setUpTests.l8, setUpTests.l9,
                 setUpTests.l12, setUpTests.l13});
  } // namespace UnitTesting
  TEST_METHOD(FollowsStmtAndLineNumber) {
    // Valid follows returns previous line number from given lineNumber, else 0.
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.stmt, setUpTests.c1) == LINE_SET());
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.stmt, setUpTests.c2) ==
        LINE_SET{setUpTests.l1});
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.stmt, setUpTests.c3) ==
        LINE_SET{setUpTests.l2});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.stmt, setUpTests.c4) == LINE_SET());
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.stmt, setUpTests.c5) ==
        LINE_SET{setUpTests.l4});
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.stmt, setUpTests.c6) ==
        LINE_SET{setUpTests.l3});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.stmt, setUpTests.c7) == LINE_SET());
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.stmt, setUpTests.c8) ==
        LINE_SET{setUpTests.l7});
    Assert::IsTrue(pkb.getQueryInterface()->follows(
                       setUpTests.stmt, setUpTests.c999) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsStmtAndStatement) {
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->follows(
                       setUpTests.stmt, setUpTests.r)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l8, setUpTests.l1, setUpTests.l4,
                                   setUpTests.l12, setUpTests.l7},
                       LINE_VECTOR{setUpTests.l9, setUpTests.l2, setUpTests.l5,
                                   setUpTests.l13, setUpTests.l8})));

  } // namespace UnitTesting

  TEST_METHOD(FollowsStmtAndStmt) {
    Assert::IsTrue(
        convertToOrderedList(pkb.getQueryInterface()->follows(
            setUpTests.stmt, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l1, setUpTests.l2, setUpTests.l3,
                        setUpTests.l4, setUpTests.l7, setUpTests.l8,
                        setUpTests.l9, setUpTests.l10, setUpTests.l12,
                        setUpTests.l13, setUpTests.l14, setUpTests.l15,
                        setUpTests.l16, setUpTests.l17},
            LINE_VECTOR{setUpTests.l2, setUpTests.l3, setUpTests.l6,
                        setUpTests.l5, setUpTests.l8, setUpTests.l9,
                        setUpTests.l10, setUpTests.l11, setUpTests.l13,
                        setUpTests.l14, setUpTests.l25, setUpTests.l23,
                        setUpTests.l17, setUpTests.l19})));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStmtAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->follows(setUpTests.stmt,
                                                    setUpTests.underscore) ==
                   LINE_SET{setUpTests.l1, setUpTests.l2, setUpTests.l3,
                            setUpTests.l4, setUpTests.l7, setUpTests.l8,
                            setUpTests.l9, setUpTests.l10, setUpTests.l12,
                            setUpTests.l13, setUpTests.l14, setUpTests.l15,
                            setUpTests.l16, setUpTests.l17});
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.underscore, setUpTests.c5));
    Assert::IsTrue(pkb.getQueryInterface()->follows(setUpTests.underscore,
                                                    setUpTests.c17));

    Assert::IsFalse(
        pkb.getQueryInterface()->follows(setUpTests.underscore, setUpTests.c4));
    Assert::IsFalse(
        pkb.getQueryInterface()->follows(setUpTests.underscore, setUpTests.c1));
    Assert::IsFalse(pkb.getQueryInterface()->follows(setUpTests.underscore,
                                                     setUpTests.c16));
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.underscore, setUpTests.a) ==
        LINE_SET{setUpTests.l10});
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.underscore, setUpTests.w) ==
        LINE_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                 setUpTests.l23});
    Assert::IsTrue(
        pkb.getQueryInterface()->follows(setUpTests.underscore, setUpTests.r) ==
        LINE_SET{setUpTests.l2, setUpTests.l5, setUpTests.l8, setUpTests.l9,
                 setUpTests.l13});
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndStmt) {
    Assert::IsTrue(pkb.getQueryInterface()->follows(setUpTests.underscore,
                                                    setUpTests.stmt) ==
                   LINE_SET{setUpTests.l2, setUpTests.l3, setUpTests.l6,
                            setUpTests.l5, setUpTests.l8, setUpTests.l9,
                            setUpTests.l10, setUpTests.l11, setUpTests.l13,
                            setUpTests.l14, setUpTests.l25, setUpTests.l23,
                            setUpTests.l17, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->follows(setUpTests.underscore,
                                                    setUpTests.underscore));
  } // namespace UnitTesting

  /*
   * FollowsStar query tests
   */
  TEST_METHOD(FollowsStarLineNumberAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
        setUpTests.c1, setUpTests.c6)); // valid within same proc
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
        setUpTests.c16, setUpTests.c19)); // inside if container

    Assert::IsFalse(pkb.getQueryInterface()->followsStar(
        setUpTests.c1,
        setUpTests.c4)); // within same proc but going into container
    Assert::IsFalse(pkb.getQueryInterface()->followsStar(
        setUpTests.c1, setUpTests.c7)); // different proc
    Assert::IsFalse(pkb.getQueryInterface()->followsStar(
        setUpTests.c20, setUpTests.c21)); // from if to else
  }                                       // namespace UnitTesting

  TEST_METHOD(FollowsStarLineNumberAndStatement) {
    // Valid followsStar when following statement type is true, returns
    // statement line number.
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.c1, setUpTests.r) == LINE_SET{setUpTests.l2});
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.c2, setUpTests.w) == LINE_SET{setUpTests.l3});
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.c3, setUpTests.p) == LINE_SET{setUpTests.l6});
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.c4, setUpTests.r) == LINE_SET{setUpTests.l5});
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.c7, setUpTests.r) ==
        LINE_SET{setUpTests.l8, setUpTests.l9});
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.c7, setUpTests.c) ==
        LINE_SET{setUpTests.l11});

    // Invalid followsStar when following statement type is not true and returns
    // 0.
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.c7, setUpTests.w) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.c1, setUpTests.a) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.c3, setUpTests.r) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.c4, setUpTests.c) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.c2, setUpTests.i) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarLineNumberAndStmt) {
    // Valid followsStar returns following line number else 0.
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.c1, setUpTests.stmt) ==
        LINE_SET{setUpTests.l2, setUpTests.l3, setUpTests.l6});
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.c3, setUpTests.stmt) ==
        LINE_SET{setUpTests.l6});
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.c16, setUpTests.stmt) ==
        LINE_SET{setUpTests.l17, setUpTests.l19});
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.c7, setUpTests.stmt) ==
        LINE_SET{setUpTests.l8, setUpTests.l9, setUpTests.l10, setUpTests.l11});
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.c999, setUpTests.stmt) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarLineNumberAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.c1,
                                                        setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.c3,
                                                        setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
        setUpTests.c4, setUpTests.underscore)); // inside while
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.c7,
                                                        setUpTests.underscore));

    Assert::IsFalse(pkb.getQueryInterface()->followsStar(
        setUpTests.c6, setUpTests.underscore)); // last line in proc
    Assert::IsFalse(pkb.getQueryInterface()->followsStar(
        setUpTests.c18, setUpTests.underscore)); // last line in while container
    Assert::IsFalse(pkb.getQueryInterface()->followsStar(
        setUpTests.c999, setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStatementAndLineNumber) {
    // Valid followsStar if statement type of previous line number from given
    // lineNumber is correct, returns the statement line number. Else returns 0.
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.a,
                                                        setUpTests.c1) ==
                   LINE_SET()); // start of line
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.c, setUpTests.c11) ==
        LINE_SET{setUpTests.l7});
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.r, setUpTests.c6) ==
        LINE_SET{setUpTests.l1, setUpTests.l2});
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.a, setUpTests.c19) ==
        LINE_SET{setUpTests.l16});
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.r,
                                                        setUpTests.c999) ==
                   LINE_SET()); // invalid line number
  }                             // namespace UnitTesting

  TEST_METHOD(FollowsStarStatementAndStatement) {
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->followsStar(
                       setUpTests.r, setUpTests.w)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l1, setUpTests.l2, setUpTests.l13,
                                   setUpTests.l12},
                       LINE_VECTOR{setUpTests.l3, setUpTests.l3, setUpTests.l14,
                                   setUpTests.l14})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->followsStar(setUpTests.c, setUpTests.c)) ==
        convertToOrderedList(LINE_LINE_PAIRS(LINE_VECTOR{setUpTests.l7},
                                             LINE_VECTOR{setUpTests.l11})));
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->followsStar(setUpTests.c, setUpTests.a)) ==
        convertToOrderedList(LINE_LINE_PAIRS(LINE_VECTOR{setUpTests.l7},
                                             LINE_VECTOR{setUpTests.l10})));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStatementAndStmt) {
    Assert::IsTrue(convertToOrderedList(pkb.getQueryInterface()->followsStar(
                       setUpTests.c, setUpTests.stmt)) ==
                   convertToOrderedList(LINE_LINE_PAIRS(
                       LINE_VECTOR{setUpTests.l7, setUpTests.l7, setUpTests.l7,
                                   setUpTests.l7},
                       LINE_VECTOR{setUpTests.l8, setUpTests.l9, setUpTests.l10,
                                   setUpTests.l11})));
    Assert::IsTrue(
        convertToOrderedList(pkb.getQueryInterface()->followsStar(
            setUpTests.w, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l3, setUpTests.l14, setUpTests.l17},
            LINE_VECTOR{setUpTests.l6, setUpTests.l25, setUpTests.l19})));
    Assert::IsTrue(
        convertToOrderedList(pkb.getQueryInterface()->followsStar(
            setUpTests.r, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l1, setUpTests.l1, setUpTests.l1,
                        setUpTests.l9, setUpTests.l9, setUpTests.l2,
                        setUpTests.l2, setUpTests.l13, setUpTests.l13,
                        setUpTests.l8, setUpTests.l8, setUpTests.l8,
                        setUpTests.l12, setUpTests.l12, setUpTests.l12},
            LINE_VECTOR{
                setUpTests.l2,
                setUpTests.l3,
                setUpTests.l6,
                setUpTests.l10,
                setUpTests.l11,
                setUpTests.l3,
                setUpTests.l6,
                setUpTests.l14,
                setUpTests.l25,
                setUpTests.l9,
                setUpTests.l10,
                setUpTests.l11,
                setUpTests.l13,
                setUpTests.l14,
                setUpTests.l25,
            })));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStatementAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
                       setUpTests.r, setUpTests.underscore) ==
                   LINE_SET{setUpTests.l1, setUpTests.l2, setUpTests.l8,
                            setUpTests.l9, setUpTests.l12, setUpTests.l13});
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStmtAndLineNumber) {
    // Valid followsStar returns previous line number from given lineNumber,
    // else 0.
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.stmt,
                                                        setUpTests.c1) ==
                   LINE_SET()); // first line in proc
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.stmt, setUpTests.c25) ==
        LINE_SET{setUpTests.l12, setUpTests.l13, setUpTests.l14});
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.stmt,
                                                        setUpTests.c21) ==
                   LINE_SET()); // inside else block with 1 line
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.stmt, setUpTests.c19) ==
        LINE_SET{setUpTests.l16, setUpTests.l17}); // inside if block
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.stmt,
                                                        setUpTests.c999) ==
                   LINE_SET()); // invalid line number
  }                             // namespace UnitTesting

  TEST_METHOD(FollowsStarStmtAndStatement) {

    Assert::IsTrue(
        convertToOrderedList(pkb.getQueryInterface()->followsStar(
            setUpTests.stmt, setUpTests.a)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l9, setUpTests.l8, setUpTests.l7},
            LINE_VECTOR{setUpTests.l10, setUpTests.l10, setUpTests.l10})));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStmtAndStmt) {
    Assert::IsTrue(
        convertToOrderedList(pkb.getQueryInterface()->followsStar(
            setUpTests.stmt, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{
                setUpTests.l9,  setUpTests.l9,  setUpTests.l1,  setUpTests.l1,
                setUpTests.l1,  setUpTests.l10, setUpTests.l2,  setUpTests.l2,
                setUpTests.l3,  setUpTests.l4,  setUpTests.l7,  setUpTests.l7,
                setUpTests.l7,  setUpTests.l7,  setUpTests.l8,  setUpTests.l8,
                setUpTests.l8,  setUpTests.l12, setUpTests.l12, setUpTests.l12,
                setUpTests.l13, setUpTests.l13, setUpTests.l14, setUpTests.l15,
                setUpTests.l16, setUpTests.l16, setUpTests.l17},
            LINE_VECTOR{
                setUpTests.l10, setUpTests.l11, setUpTests.l2,  setUpTests.l3,
                setUpTests.l6,  setUpTests.l11, setUpTests.l3,  setUpTests.l6,
                setUpTests.l6,  setUpTests.l5,  setUpTests.l8,  setUpTests.l9,
                setUpTests.l10, setUpTests.l11, setUpTests.l9,  setUpTests.l10,
                setUpTests.l11, setUpTests.l13, setUpTests.l14, setUpTests.l25,
                setUpTests.l14, setUpTests.l25, setUpTests.l25, setUpTests.l23,
                setUpTests.l17, setUpTests.l19, setUpTests.l19})));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStmtAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->followsStar(setUpTests.stmt,
                                             setUpTests.underscore) ==
        LINE_SET{setUpTests.l1, setUpTests.l2, setUpTests.l3, setUpTests.l4,
                 setUpTests.l7, setUpTests.l8, setUpTests.l9, setUpTests.l10,
                 setUpTests.l12, setUpTests.l13, setUpTests.l14, setUpTests.l15,
                 setUpTests.l16, setUpTests.l17});
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarUnderscoreAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
        setUpTests.underscore, setUpTests.c5)); // last line of while container
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(
        setUpTests.underscore, setUpTests.c6)); // last line of proc
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.underscore,
                                                        setUpTests.c17));

    Assert::IsFalse(pkb.getQueryInterface()->followsStar(
        setUpTests.underscore,
        setUpTests.c4)); // first line inside while container
    Assert::IsFalse(pkb.getQueryInterface()->followsStar(
        setUpTests.underscore, setUpTests.c1)); // first line of proc
    Assert::IsFalse(pkb.getQueryInterface()->followsStar(
        setUpTests.underscore,
        setUpTests.c16)); // first line inside if container
    Assert::IsFalse(pkb.getQueryInterface()->followsStar(
        setUpTests.underscore, setUpTests.c21)); // first line inside else
  }                                              // namespace UnitTesting
  TEST_METHOD(FollowsStarUnderscoreAndStatement) {
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.underscore,
                                                        setUpTests.a) ==
                   LINE_SET{setUpTests.l10});
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.underscore,
                                                        setUpTests.w) ==
                   LINE_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                            setUpTests.l23});
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.underscore,
                                                        setUpTests.r) ==
                   LINE_SET{setUpTests.l2, setUpTests.l5, setUpTests.l8,
                            setUpTests.l9, setUpTests.l13});
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarUnderscoreAndStmt) {
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.underscore,
                                                        setUpTests.stmt) ==
                   LINE_SET{setUpTests.l2, setUpTests.l3, setUpTests.l6,
                            setUpTests.l5, setUpTests.l8, setUpTests.l9,
                            setUpTests.l10, setUpTests.l11, setUpTests.l13,
                            setUpTests.l14, setUpTests.l25, setUpTests.l23,
                            setUpTests.l17, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->followsStar(setUpTests.underscore,
                                                        setUpTests.underscore));
  } // namespace UnitTesting
};
} // namespace UnitTesting
