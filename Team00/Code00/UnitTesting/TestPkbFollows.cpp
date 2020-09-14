#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestFollowsQuery) {

public:
  SetUpTests setUpTests;

  TEST_METHOD_INITIALIZE(SetUpPkb) { setUpTests = SetUpTests::SetUpTests(); }

  TEST_METHOD(FollowsLineNumberAndLineNumber) {
    // Valid follows
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c1, setUpTests.c2));
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c2, setUpTests.c3));
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c3, setUpTests.c6));
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c4, setUpTests.c5));
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c7, setUpTests.c8));

    // Invalid follows
    Assert::IsFalse(
        setUpTests.pkbQueryInterface.follows(setUpTests.c1, setUpTests.c3));
    Assert::IsFalse(
        setUpTests.pkbQueryInterface.follows(setUpTests.c1, setUpTests.c5));
    Assert::IsFalse(
        setUpTests.pkbQueryInterface.follows(setUpTests.c1, setUpTests.c6));
    Assert::IsFalse(
        setUpTests.pkbQueryInterface.follows(setUpTests.c1, setUpTests.c7));
    Assert::IsFalse(
        setUpTests.pkbQueryInterface.follows(setUpTests.c2, setUpTests.c4));
    Assert::IsFalse(
        setUpTests.pkbQueryInterface.follows(setUpTests.c2, setUpTests.c8));
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndStatement) {
    // Valid follows when following statement type is true, returns statement
    // line number.
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c1, setUpTests.r) ==
        STRING_SET{setUpTests.l2});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c2, setUpTests.w) ==
        STRING_SET{setUpTests.l3});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c3, setUpTests.p) ==
        STRING_SET{setUpTests.l6});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c4, setUpTests.r) ==
        STRING_SET{setUpTests.l5});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c7, setUpTests.r) ==
        STRING_SET{setUpTests.l8});

    // Invalid follows when following statement type is not true and returns 0.
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.c7, setUpTests.w) == STRING_SET());
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.c1, setUpTests.a) == STRING_SET());
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.c3, setUpTests.r) == STRING_SET());
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.c4, setUpTests.c) == STRING_SET());
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.c2, setUpTests.i) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndStmt) {
    // Valid follows returns following line number else 0.
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c1, setUpTests.stmt) ==
        STRING_SET{setUpTests.l2});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c2, setUpTests.stmt) ==
        STRING_SET{setUpTests.l3});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c3, setUpTests.stmt) ==
        STRING_SET{setUpTests.l6});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c4, setUpTests.stmt) ==
        STRING_SET{setUpTests.l5});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c7, setUpTests.stmt) ==
        STRING_SET{setUpTests.l8});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c8, setUpTests.stmt) ==
        STRING_SET{setUpTests.l9});
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.c999, setUpTests.stmt) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndUnderscore) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.c1,
                                                        setUpTests.underscore));
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.c2,
                                                        setUpTests.underscore));
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.c3,
                                                        setUpTests.underscore));
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.c4,
                                                        setUpTests.underscore));
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.c7,
                                                        setUpTests.underscore));
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.c8,
                                                        setUpTests.underscore));

    Assert::IsFalse(setUpTests.pkbQueryInterface.follows(
        setUpTests.c999, setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndLineNumber) {
    // Valid follows if statement type of previous line number from given
    // lineNumber is correct, returns the statement line number. Else returns 0.
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.a, setUpTests.c1) == STRING_SET());
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.r, setUpTests.c2) ==
        STRING_SET{setUpTests.l1});
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.a, setUpTests.c2) == STRING_SET());
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.r, setUpTests.c3) ==
        STRING_SET{setUpTests.l2});
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.w, setUpTests.c4) == STRING_SET());
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.a, setUpTests.c5) ==
        STRING_SET{setUpTests.l4});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.w, setUpTests.c6) ==
        STRING_SET{setUpTests.l3});
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.i, setUpTests.c6) == STRING_SET());
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.a, setUpTests.c7) == STRING_SET());
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.c, setUpTests.c8) ==
        STRING_SET{setUpTests.l7});
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.r, setUpTests.c999) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndStatement) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.r, setUpTests.r) ==
        STRING_PAIRS(
            STRING_VECTOR{setUpTests.l1, setUpTests.l8, setUpTests.l12},
            STRING_VECTOR{setUpTests.l2, setUpTests.l9, setUpTests.l13}));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndStmt) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.r, setUpTests.stmt) ==
        STRING_PAIRS(
            STRING_VECTOR{setUpTests.l1, setUpTests.l9, setUpTests.l2,
                          setUpTests.l13, setUpTests.l8, setUpTests.l12},
            STRING_VECTOR{setUpTests.l2, setUpTests.l10, setUpTests.l3,
                          setUpTests.l14, setUpTests.l9, setUpTests.l13}));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndUnderscore) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.r, setUpTests.underscore) ==
                   STRING_SET{setUpTests.l1, setUpTests.l2, setUpTests.l8,
                              setUpTests.l9, setUpTests.l12, setUpTests.l13});
  } // namespace UnitTesting
  TEST_METHOD(FollowsStmtAndLineNumber) {
    // Valid follows returns previous line number from given lineNumber, else 0.
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.stmt, setUpTests.c1) == STRING_SET());
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.stmt, setUpTests.c2) ==
        STRING_SET{setUpTests.l1});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.stmt, setUpTests.c3) ==
        STRING_SET{setUpTests.l2});
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.stmt, setUpTests.c4) == STRING_SET());
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.stmt, setUpTests.c5) ==
        STRING_SET{setUpTests.l4});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.stmt, setUpTests.c6) ==
        STRING_SET{setUpTests.l3});
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.stmt, setUpTests.c7) == STRING_SET());
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.stmt, setUpTests.c8) ==
        STRING_SET{setUpTests.l7});
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(
                       setUpTests.stmt, setUpTests.c999) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsStmtAndStatement) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.stmt, setUpTests.r) ==
        STRING_PAIRS(STRING_VECTOR{setUpTests.l8, setUpTests.l1, setUpTests.l12,
                                   setUpTests.l4, setUpTests.l7},
                     STRING_VECTOR{setUpTests.l9, setUpTests.l2, setUpTests.l13,
                                   setUpTests.l5, setUpTests.l8}));

  } // namespace UnitTesting

  TEST_METHOD(FollowsStmtAndStmt) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.stmt,
                                             setUpTests.stmt) ==
        STRING_PAIRS(
            STRING_VECTOR{setUpTests.l1, setUpTests.l2, setUpTests.l3,
                          setUpTests.l4, setUpTests.l7, setUpTests.l8,
                          setUpTests.l9, setUpTests.l10, setUpTests.l12,
                          setUpTests.l13, setUpTests.l14, setUpTests.l15,
                          setUpTests.l16, setUpTests.l17},
            STRING_VECTOR{setUpTests.l2, setUpTests.l3, setUpTests.l6,
                          setUpTests.l5, setUpTests.l8, setUpTests.l9,
                          setUpTests.l10, setUpTests.l11, setUpTests.l13,
                          setUpTests.l14, setUpTests.l25, setUpTests.l23,
                          setUpTests.l17, setUpTests.l19}));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStmtAndUnderscore) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.follows(setUpTests.stmt,
                                             setUpTests.underscore) ==
        STRING_SET{setUpTests.l1, setUpTests.l2, setUpTests.l3, setUpTests.l4,
                   setUpTests.l7, setUpTests.l8, setUpTests.l9, setUpTests.l10,
                   setUpTests.l12, setUpTests.l13, setUpTests.l14,
                   setUpTests.l15, setUpTests.l16, setUpTests.l17});
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndLineNumber) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.underscore,
                                                        setUpTests.c5));
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.underscore,
                                                        setUpTests.c17));

    Assert::IsFalse(setUpTests.pkbQueryInterface.follows(setUpTests.underscore,
                                                         setUpTests.c4));
    Assert::IsFalse(setUpTests.pkbQueryInterface.follows(setUpTests.underscore,
                                                         setUpTests.c1));
    Assert::IsFalse(setUpTests.pkbQueryInterface.follows(setUpTests.underscore,
                                                         setUpTests.c16));
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndStatement) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.underscore,
                                                        setUpTests.a) ==
                   STRING_SET{setUpTests.l10});
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.underscore,
                                                        setUpTests.w) ==
                   STRING_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                              setUpTests.l23});
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.underscore,
                                                        setUpTests.r) ==
                   STRING_SET{setUpTests.l2, setUpTests.l5, setUpTests.l8,
                              setUpTests.l9, setUpTests.l13});
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndStmt) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.underscore,
                                                        setUpTests.stmt) ==
                   STRING_SET{setUpTests.l2, setUpTests.l3, setUpTests.l6,
                              setUpTests.l5, setUpTests.l8, setUpTests.l9,
                              setUpTests.l10, setUpTests.l11, setUpTests.l13,
                              setUpTests.l14, setUpTests.l25, setUpTests.l23,
                              setUpTests.l17, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndUnderscore) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.follows(setUpTests.underscore,
                                                        setUpTests.underscore));
  } // namespace UnitTesting

  /*
   * FollowsStar query tests
   */
  TEST_METHOD(FollowsStarLineNumberAndLineNumber) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c1, setUpTests.c6)); // valid within same proc
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c16, setUpTests.c19)); // inside if container

    Assert::IsFalse(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c1,
        setUpTests.c4)); // within same proc but going into container
    Assert::IsFalse(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c1, setUpTests.c7)); // different proc
    Assert::IsFalse(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c20, setUpTests.c21)); // from if to else
  }                                       // namespace UnitTesting

  TEST_METHOD(FollowsStarLineNumberAndStatement) {
    // Valid followsStar when following statement type is true, returns
    // statement line number.
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.c1, setUpTests.r) ==
        STRING_SET{setUpTests.l2});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.c2, setUpTests.w) ==
        STRING_SET{setUpTests.l3});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.c3, setUpTests.p) ==
        STRING_SET{setUpTests.l6});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.c4, setUpTests.r) ==
        STRING_SET{setUpTests.l5});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.c7, setUpTests.r) ==
        STRING_SET{setUpTests.l8, setUpTests.l9});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.c7, setUpTests.c) ==
        STRING_SET{setUpTests.l11});

    // Invalid followsStar when following statement type is not true and returns
    // 0.
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
                       setUpTests.c7, setUpTests.w) == STRING_SET());
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
                       setUpTests.c1, setUpTests.a) == STRING_SET());
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
                       setUpTests.c3, setUpTests.r) == STRING_SET());
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
                       setUpTests.c4, setUpTests.c) == STRING_SET());
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
                       setUpTests.c2, setUpTests.i) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarLineNumberAndStmt) {
    // Valid followsStar returns following line number else 0.
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.c1,
                                                            setUpTests.stmt) ==
                   STRING_SET{setUpTests.l2, setUpTests.l3, setUpTests.l6});
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.c3,
                                                            setUpTests.stmt) ==
                   STRING_SET{setUpTests.l6});
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.c16,
                                                            setUpTests.stmt) ==
                   STRING_SET{setUpTests.l17, setUpTests.l19});
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.c7,
                                                            setUpTests.stmt) ==
                   STRING_SET{setUpTests.l8, setUpTests.l9, setUpTests.l10,
                              setUpTests.l11});
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
                       setUpTests.c999, setUpTests.stmt) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarLineNumberAndUnderscore) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c1, setUpTests.underscore));
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c3, setUpTests.underscore));
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c4, setUpTests.underscore)); // inside while
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c7, setUpTests.underscore));

    Assert::IsFalse(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c6, setUpTests.underscore)); // last line in proc
    Assert::IsFalse(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c18, setUpTests.underscore)); // last line in while container
    Assert::IsFalse(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.c999, setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStatementAndLineNumber) {
    // Valid followsStar if statement type of previous line number from given
    // lineNumber is correct, returns the statement line number. Else returns 0.
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.a, setUpTests.c1) ==
        STRING_SET()); // start of line
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.c,
                                                            setUpTests.c11) ==
                   STRING_SET{setUpTests.l7});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.r, setUpTests.c6) ==
        STRING_SET{setUpTests.l1, setUpTests.l2});
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.a,
                                                            setUpTests.c19) ==
                   STRING_SET{setUpTests.l16});
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.r,
                                                            setUpTests.c999) ==
                   STRING_SET()); // invalid line number
  }                               // namespace UnitTesting

  TEST_METHOD(FollowsStarStatementAndStatement) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.r, setUpTests.w) ==
        STRING_PAIRS(STRING_VECTOR{setUpTests.l1, setUpTests.l2, setUpTests.l13,
                                   setUpTests.l12},
                     STRING_VECTOR{setUpTests.l3, setUpTests.l3, setUpTests.l14,
                                   setUpTests.l14}));
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.c, setUpTests.c) ==
        STRING_PAIRS(STRING_VECTOR{setUpTests.l7},
                     STRING_VECTOR{setUpTests.l11}));
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.c, setUpTests.a) ==
        STRING_PAIRS(STRING_VECTOR{setUpTests.l7},
                     STRING_VECTOR{setUpTests.l10}));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStatementAndStmt) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.c,
                                                            setUpTests.stmt) ==
                   STRING_PAIRS(STRING_VECTOR{setUpTests.l7, setUpTests.l7,
                                              setUpTests.l7, setUpTests.l7},
                                STRING_VECTOR{setUpTests.l8, setUpTests.l11,
                                              setUpTests.l9, setUpTests.l10}));
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.w,
                                                 setUpTests.stmt) ==
        STRING_PAIRS(
            STRING_VECTOR{setUpTests.l3, setUpTests.l14, setUpTests.l17},
            STRING_VECTOR{setUpTests.l6, setUpTests.l25, setUpTests.l19}));
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.r,
                                                 setUpTests.stmt) ==
        STRING_PAIRS(STRING_VECTOR{setUpTests.l1, setUpTests.l1, setUpTests.l1,
                                   setUpTests.l9, setUpTests.l9, setUpTests.l2,
                                   setUpTests.l2, setUpTests.l13,
                                   setUpTests.l13, setUpTests.l8, setUpTests.l8,
                                   setUpTests.l8, setUpTests.l12,
                                   setUpTests.l12, setUpTests.l12},
                     STRING_VECTOR{
                         setUpTests.l2,
                         setUpTests.l3,
                         setUpTests.l6,
                         setUpTests.l10,
                         setUpTests.l11,
                         setUpTests.l3,
                         setUpTests.l6,
                         setUpTests.l25,
                         setUpTests.l14,
                         setUpTests.l10,
                         setUpTests.l9,
                         setUpTests.l11,
                         setUpTests.l13,
                         setUpTests.l14,
                         setUpTests.l25,
                     }));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStatementAndUnderscore) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
                       setUpTests.r, setUpTests.underscore) ==
                   STRING_SET{setUpTests.l1, setUpTests.l2, setUpTests.l8,
                              setUpTests.l9, setUpTests.l12, setUpTests.l13});
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStmtAndLineNumber) {
    // Valid followsStar returns previous line number from given lineNumber,
    // else 0.
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.stmt,
                                                            setUpTests.c1) ==
                   STRING_SET()); // first line in proc
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.stmt,
                                                            setUpTests.c25) ==
                   STRING_SET{setUpTests.l12, setUpTests.l13, setUpTests.l14});
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.stmt,
                                                            setUpTests.c21) ==
                   STRING_SET()); // inside else block with 1 line
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.stmt,
                                                 setUpTests.c19) ==
        STRING_SET{setUpTests.l16, setUpTests.l17}); // inside if block
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(setUpTests.stmt,
                                                            setUpTests.c999) ==
                   STRING_SET()); // invalid line number
  }                               // namespace UnitTesting

  TEST_METHOD(FollowsStarStmtAndStatement) {

    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.stmt,
                                                 setUpTests.a) ==
        STRING_PAIRS(
            STRING_VECTOR{setUpTests.l9, setUpTests.l8, setUpTests.l7},
            STRING_VECTOR{setUpTests.l10, setUpTests.l10, setUpTests.l10}));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStmtAndStmt) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.stmt,
                                                 setUpTests.stmt) ==
        STRING_PAIRS(
            STRING_VECTOR{
                setUpTests.l10, setUpTests.l9,  setUpTests.l9,  setUpTests.l1,
                setUpTests.l1,  setUpTests.l1,  setUpTests.l2,  setUpTests.l2,
                setUpTests.l3,  setUpTests.l4,  setUpTests.l7,  setUpTests.l7,
                setUpTests.l7,  setUpTests.l7,  setUpTests.l8,  setUpTests.l8,
                setUpTests.l8,  setUpTests.l12, setUpTests.l12, setUpTests.l12,
                setUpTests.l13, setUpTests.l13, setUpTests.l14, setUpTests.l15,
                setUpTests.l16, setUpTests.l16, setUpTests.l17},
            STRING_VECTOR{
                setUpTests.l11, setUpTests.l10, setUpTests.l11, setUpTests.l2,
                setUpTests.l3,  setUpTests.l6,  setUpTests.l3,  setUpTests.l6,
                setUpTests.l6,  setUpTests.l5,  setUpTests.l8,  setUpTests.l11,
                setUpTests.l9,  setUpTests.l10, setUpTests.l10, setUpTests.l9,
                setUpTests.l11, setUpTests.l13, setUpTests.l14, setUpTests.l25,
                setUpTests.l25, setUpTests.l14, setUpTests.l25, setUpTests.l23,
                setUpTests.l17, setUpTests.l19, setUpTests.l19}));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStmtAndUnderscore) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.stmt,
                                                 setUpTests.underscore) ==
        STRING_SET{setUpTests.l1, setUpTests.l2, setUpTests.l3, setUpTests.l4,
                   setUpTests.l7, setUpTests.l8, setUpTests.l9, setUpTests.l10,
                   setUpTests.l12, setUpTests.l13, setUpTests.l14,
                   setUpTests.l15, setUpTests.l16, setUpTests.l17});
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarUnderscoreAndLineNumber) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.underscore, setUpTests.c5)); // last line of while container
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.underscore, setUpTests.c6)); // last line of proc
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.underscore, setUpTests.c17));

    Assert::IsFalse(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.underscore,
        setUpTests.c4)); // first line inside while container
    Assert::IsFalse(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.underscore, setUpTests.c1)); // first line of proc
    Assert::IsFalse(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.underscore,
        setUpTests.c16)); // first line inside if container
    Assert::IsFalse(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.underscore, setUpTests.c21)); // first line inside else
  }                                              // namespace UnitTesting
  TEST_METHOD(FollowsStarUnderscoreAndStatement) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
                       setUpTests.underscore, setUpTests.a) ==
                   STRING_SET{setUpTests.l10});
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
                       setUpTests.underscore, setUpTests.w) ==
                   STRING_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                              setUpTests.l23});
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
                       setUpTests.underscore, setUpTests.r) ==
                   STRING_SET{setUpTests.l2, setUpTests.l5, setUpTests.l8,
                              setUpTests.l9, setUpTests.l13});
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarUnderscoreAndStmt) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.followsStar(setUpTests.underscore,
                                                 setUpTests.stmt) ==
        STRING_SET{setUpTests.l2, setUpTests.l3, setUpTests.l6, setUpTests.l5,
                   setUpTests.l8, setUpTests.l9, setUpTests.l10, setUpTests.l11,
                   setUpTests.l13, setUpTests.l14, setUpTests.l25,
                   setUpTests.l23, setUpTests.l17, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarUnderscoreAndUnderscore) {
    Assert::IsTrue(setUpTests.pkbQueryInterface.followsStar(
        setUpTests.underscore, setUpTests.underscore));
  } // namespace UnitTesting
};
} // namespace UnitTesting
