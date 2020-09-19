#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestParentQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests = SetUpTests::SetUpTests(pkb);
  PkbQueryInterface &pkbQueryInterface = pkb.getQueryInterface();

  TEST_METHOD(ParentLineNumberAndLineNumber) {

    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c3, setUpTests.c4));
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c3, setUpTests.c5));
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c14, setUpTests.c15));
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c14, setUpTests.c23));
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c15, setUpTests.c16));
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c15, setUpTests.c19));
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c15, setUpTests.c22));
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c17, setUpTests.c18));
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c19, setUpTests.c21));
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c23, setUpTests.c24));

    Assert::IsFalse(pkbQueryInterface.parent(setUpTests.c999, setUpTests.c1));
    Assert::IsFalse(pkbQueryInterface.parent(setUpTests.c15, setUpTests.c999));
    Assert::IsFalse(pkbQueryInterface.parent(setUpTests.c15, setUpTests.c23));
    Assert::IsFalse(pkbQueryInterface.parent(setUpTests.c15, setUpTests.c21));
    Assert::IsFalse(pkbQueryInterface.parent(setUpTests.c14, setUpTests.c16));
    Assert::IsFalse(pkbQueryInterface.parent(setUpTests.c14, setUpTests.c22));
    Assert::IsFalse(pkbQueryInterface.parent(setUpTests.c16, setUpTests.c18));

  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndStatement) {

    // Valid parent constant returns line numbers that matches statement type.
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c3, setUpTests.a) ==
                   STRING_SET{setUpTests.l4});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c3, setUpTests.r) ==
                   STRING_SET{setUpTests.l5});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c14, setUpTests.i) ==
                   STRING_SET{setUpTests.l15});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c14, setUpTests.w) ==
                   STRING_SET{setUpTests.l23});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c15, setUpTests.a) ==
                   STRING_SET{setUpTests.l16});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c15, setUpTests.w) ==
                   STRING_SET{setUpTests.l17});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c15, setUpTests.i) ==
                   STRING_SET{setUpTests.l19});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c17, setUpTests.a) ==
                   STRING_SET{setUpTests.l18});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c19, setUpTests.a) ==
                   STRING_SET{setUpTests.l20});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c19, setUpTests.p) ==
                   STRING_SET{setUpTests.l21});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c23, setUpTests.a) ==
                   STRING_SET{setUpTests.l24});

    // Invalid parent constants that aren't parents, return empty vector;
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c2, setUpTests.r) ==
                   STRING_SET()); // constant on read
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c4, setUpTests.a) ==
                   STRING_SET()); // constant on assign
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c21, setUpTests.p) ==
                   STRING_SET()); // constant on print
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c11, setUpTests.c) ==
                   STRING_SET()); // constant on call

  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndStmt) {

    // Valid parent
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c3, setUpTests.stmt) ==
                   STRING_SET{setUpTests.l4, setUpTests.l5});

    // Invalid parent
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.c16, setUpTests.stmt) ==
                   STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndUnderscore) {

    // Valid parent
    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.c3, setUpTests.underscore));

    // Invalid parent
    Assert::IsFalse(
        pkbQueryInterface.parent(setUpTests.c16, setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(ParentStatementAndLineNumber) {

    // Valid parent
    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.w, setUpTests.c4) ==
        STRING_SET{setUpTests.l3}); // LineNumber is in container and Parent
                                    // setUpTests.stmt type is correct

    // Invalid parent
    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.i, setUpTests.c4) ==
        STRING_SET()); // Parent setUpTests.stmt is not the correct type
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.w, setUpTests.c13) ==
                   STRING_SET()); // LineNumber does not belong in any container
  }                               // namespace UnitTesting

  TEST_METHOD(ParentStatementAndStatement) {

    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.i, setUpTests.a) ==
                   STRING_PAIRS(STRING_VECTOR{setUpTests.l15, setUpTests.l19},
                                STRING_VECTOR{setUpTests.l16, setUpTests.l20}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStatementAndStmt) {

    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.i, setUpTests.stmt) ==
                   STRING_PAIRS(STRING_VECTOR{setUpTests.l15, setUpTests.l15,
                                              setUpTests.l15, setUpTests.l15,
                                              setUpTests.l19, setUpTests.l19},
                                STRING_VECTOR{setUpTests.l16, setUpTests.l17,
                                              setUpTests.l19, setUpTests.l22,
                                              setUpTests.l20, setUpTests.l21}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStatementAndUnderscore) {

    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.w, setUpTests.underscore) ==
        STRING_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                   setUpTests.l23});
    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.i, setUpTests.underscore) ==
        STRING_SET{setUpTests.l15, setUpTests.l19});
    Assert::IsTrue(pkbQueryInterface.parent(
                       setUpTests.c, setUpTests.underscore) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndLineNumber) {

    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.stmt, setUpTests.c15) ==
                   STRING_SET{setUpTests.l14});
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.stmt, setUpTests.c23) ==
                   STRING_SET{setUpTests.l14});

    // line number is not setUpTests.a child
    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.stmt, setUpTests.c14) ==
                   STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndStatement) {

    Assert::IsTrue(pkbQueryInterface.parent(setUpTests.stmt, setUpTests.a) ==
                   STRING_PAIRS(STRING_VECTOR{setUpTests.l3, setUpTests.l23,
                                              setUpTests.l17, setUpTests.l15,
                                              setUpTests.l19},
                                STRING_VECTOR{setUpTests.l4, setUpTests.l24,
                                              setUpTests.l18, setUpTests.l16,
                                              setUpTests.l20}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndStmt) {

    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.stmt, setUpTests.stmt) ==
        STRING_PAIRS(
            STRING_VECTOR{setUpTests.l3, setUpTests.l3, setUpTests.l23,
                          setUpTests.l14, setUpTests.l14, setUpTests.l15,
                          setUpTests.l15, setUpTests.l15, setUpTests.l15,
                          setUpTests.l17, setUpTests.l19, setUpTests.l19},
            STRING_VECTOR{setUpTests.l4, setUpTests.l5, setUpTests.l24,
                          setUpTests.l15, setUpTests.l23, setUpTests.l16,
                          setUpTests.l17, setUpTests.l19, setUpTests.l22,
                          setUpTests.l18, setUpTests.l20, setUpTests.l21}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndUnderscore) {

    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.stmt, setUpTests.underscore) ==
        STRING_SET{setUpTests.l23, setUpTests.l3, setUpTests.l14,
                   setUpTests.l15, setUpTests.l17, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndLineNumber) {

    // line number is setUpTests.a child
    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.underscore, setUpTests.c15));

    // line number is not setUpTests.a child
    Assert::IsFalse(
        pkbQueryInterface.parent(setUpTests.underscore, setUpTests.c14));
    Assert::IsFalse(
        pkbQueryInterface.parent(setUpTests.underscore, setUpTests.c1));
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndStatement) {

    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.underscore, setUpTests.w) ==
        STRING_SET{setUpTests.l23, setUpTests.l17});
    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.underscore, setUpTests.a) ==
        STRING_SET{setUpTests.l4, setUpTests.l16, setUpTests.l18,
                   setUpTests.l20, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndStmt) {

    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.underscore, setUpTests.stmt) ==
        STRING_SET{setUpTests.l4, setUpTests.l5, setUpTests.l15, setUpTests.l23,
                   setUpTests.l16, setUpTests.l17, setUpTests.l19,
                   setUpTests.l22, setUpTests.l18, setUpTests.l20,
                   setUpTests.l21, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndUnderscore) {
    Assert::IsTrue(
        pkbQueryInterface.parent(setUpTests.underscore, setUpTests.underscore));
  } // namespace UnitTesting

  /*
   * ParentStar query tests
   */
  TEST_METHOD(ParentStarLineNumberAndLineNumber) {
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.c3, setUpTests.c4)); // 1 level
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c14,
                                                setUpTests.c23)); // 1 level
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c14,
                                                setUpTests.c17)); // 2 levels
    Assert::IsTrue(pkbQueryInterface.parentStar(
        setUpTests.c14,
        setUpTests.c22)); // 2 levels but in else block
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c14,
                                                setUpTests.c20)); // 3 levels
    Assert::IsTrue(pkbQueryInterface.parentStar(
        setUpTests.c14,
        setUpTests.c21)); // 3 levels but in else block

    Assert::IsFalse(
        pkbQueryInterface.parentStar(setUpTests.c999,
                                     setUpTests.c1)); // invalid line number
    Assert::IsFalse(
        pkbQueryInterface.parentStar(setUpTests.c15,
                                     setUpTests.c999)); // invalid line number
    Assert::IsFalse(pkbQueryInterface.parentStar(
        setUpTests.c15,
        setUpTests.c23)); // not parent of each other
    Assert::IsFalse(pkbQueryInterface.parentStar(
        setUpTests.c21,
        setUpTests.c15)); // child - parent instead of parent - child
  }                       // namespace UnitTesting

  TEST_METHOD(ParentStarLineNumberAndStatement) {
    // Valid parentStar constant returns line numbers that matches statement
    // type.
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c3, setUpTests.a) ==
                   STRING_SET{setUpTests.l4});
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c14, setUpTests.i) ==
                   STRING_SET{setUpTests.l15, setUpTests.l19});
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c14, setUpTests.w) ==
                   STRING_SET{setUpTests.l23, setUpTests.l17});
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c14, setUpTests.a) ==
                   STRING_SET{setUpTests.l16, setUpTests.l18, setUpTests.l20,
                              setUpTests.l24});
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c14, setUpTests.p) ==
                   STRING_SET{setUpTests.l21,
                              setUpTests.l22}); // children from else blocks

    // Invalid parentStar constants that aren't parentStars, return empty
    // vector;
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c2, setUpTests.r) ==
                   STRING_SET()); // constant on read
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c4, setUpTests.a) ==
                   STRING_SET()); // constant on assign
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c21,
                                                setUpTests.p) ==
                   STRING_SET()); // constant on print
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c11,
                                                setUpTests.c) ==
                   STRING_SET()); // constant on call
  }                               // namespace UnitTesting

  TEST_METHOD(ParentStarLineNumberAndStmt) {
    // Valid parentStar
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.c3, setUpTests.stmt) ==
        STRING_SET{setUpTests.l4, setUpTests.l5}); // only 1 level
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.c14, setUpTests.stmt) ==
        STRING_SET{setUpTests.l15, setUpTests.l23, setUpTests.l16,
                   setUpTests.l17, setUpTests.l19, setUpTests.l18,
                   setUpTests.l20, setUpTests.l21, setUpTests.l22,
                   setUpTests.l24}); // nested 3 levels
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.c15, setUpTests.stmt) ==
        STRING_SET{setUpTests.l16, setUpTests.l17, setUpTests.l19,
                   setUpTests.l18, setUpTests.l20, setUpTests.l21,
                   setUpTests.l22}); // nested 2 levels

    // Invalid parentStar
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.c16,
                                                setUpTests.stmt) ==
                   STRING_SET()); // constant is not a parent statement
  }                               // namespace UnitTesting

  TEST_METHOD(ParentStarLineNumberAndUnderscore) {
    // Valid parentStar
    Assert::IsTrue(pkbQueryInterface.parentStar(
        setUpTests.c3, setUpTests.underscore)); // only 1 level
    Assert::IsTrue(pkbQueryInterface.parentStar(
        setUpTests.c14, setUpTests.underscore)); // has 3 nested levels
    Assert::IsTrue(pkbQueryInterface.parentStar(
        setUpTests.c15, setUpTests.underscore)); // has 2 nested levels

    // Invalid parentStar
    Assert::IsFalse(pkbQueryInterface.parentStar(
        setUpTests.c16,
        setUpTests.underscore)); // constant is not a parent statement
  }                              // namespace UnitTesting

  TEST_METHOD(ParentStarStatementAndLineNumber) {
    // Valid parentStar
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.w, setUpTests.c4) ==
                   STRING_SET{setUpTests.l3}); // 1 level
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.w, setUpTests.c18) ==
        STRING_SET{setUpTests.l14, setUpTests.l17}); // nested 3 levels
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.w, setUpTests.c20) ==
        STRING_SET{
            setUpTests.l14}); // nested 3 levels but called within if statement
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.w, setUpTests.c21) ==
        STRING_SET{
            setUpTests.l14}); // nested 3 levels but called within else block

    // Invalid parentStar
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.i, setUpTests.c4) ==
        STRING_SET()); // Only has 1 level and parent is not correct type
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.w,
                                                setUpTests.c13) ==
                   STRING_SET()); // LineNumber does not belong in any container
  }                               // namespace UnitTesting

  TEST_METHOD(ParentStarStatementAndStatement) {
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.i, setUpTests.a) ==
                   STRING_PAIRS(STRING_VECTOR{setUpTests.l15, setUpTests.l15,
                                              setUpTests.l19, setUpTests.l15},
                                STRING_VECTOR{setUpTests.l18, setUpTests.l16,
                                              setUpTests.l20, setUpTests.l20}));
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.w, setUpTests.a) ==
                   STRING_PAIRS(STRING_VECTOR{setUpTests.l3, setUpTests.l23,
                                              setUpTests.l14, setUpTests.l17,
                                              setUpTests.l14, setUpTests.l14,
                                              setUpTests.l14},
                                STRING_VECTOR{setUpTests.l4, setUpTests.l24,
                                              setUpTests.l24, setUpTests.l18,
                                              setUpTests.l18, setUpTests.l16,
                                              setUpTests.l20}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStatementAndStmt) {
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.i, setUpTests.stmt) ==
        STRING_PAIRS(
            STRING_VECTOR{setUpTests.l15, setUpTests.l15, setUpTests.l15,
                          setUpTests.l15, setUpTests.l15, setUpTests.l15,
                          setUpTests.l15, setUpTests.l19, setUpTests.l19},
            STRING_VECTOR{setUpTests.l16, setUpTests.l17, setUpTests.l19,
                          setUpTests.l20, setUpTests.l22, setUpTests.l18,
                          setUpTests.l21, setUpTests.l20, setUpTests.l21}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStatementAndUnderscore) {

    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.w, setUpTests.underscore) ==
        STRING_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                   setUpTests.l23});
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.i, setUpTests.underscore) ==
        STRING_SET{setUpTests.l15, setUpTests.l19});
    Assert::IsTrue(pkbQueryInterface.parentStar(
                       setUpTests.c, setUpTests.underscore) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStmtAndLineNumber) {

    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.stmt, setUpTests.c18) ==
        STRING_SET{setUpTests.l14, setUpTests.l15,
                   setUpTests.l17}); // nested 3 levels
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.stmt, setUpTests.c21) ==
        STRING_SET{
            setUpTests.l14, setUpTests.l15,
            setUpTests.l19}); // nested 3 levels, constant inside else block

    // line number is not a child
    Assert::IsTrue(pkbQueryInterface.parentStar(
                       setUpTests.stmt, setUpTests.c14) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStmtAndStatement) {
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.stmt, setUpTests.a) ==
        STRING_PAIRS(
            STRING_VECTOR{setUpTests.l3, setUpTests.l23, setUpTests.l14,
                          setUpTests.l17, setUpTests.l15, setUpTests.l14,
                          setUpTests.l15, setUpTests.l14, setUpTests.l19,
                          setUpTests.l15, setUpTests.l14},
            STRING_VECTOR{setUpTests.l4, setUpTests.l24, setUpTests.l24,
                          setUpTests.l18, setUpTests.l18, setUpTests.l18,
                          setUpTests.l16, setUpTests.l16, setUpTests.l20,
                          setUpTests.l20, setUpTests.l20}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStmtAndStmt) {
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.stmt, setUpTests.stmt) ==
        STRING_PAIRS(
            STRING_VECTOR{
                setUpTests.l23, setUpTests.l3,  setUpTests.l3,  setUpTests.l14,
                setUpTests.l14, setUpTests.l14, setUpTests.l14, setUpTests.l14,
                setUpTests.l14, setUpTests.l14, setUpTests.l14, setUpTests.l14,
                setUpTests.l14, setUpTests.l15, setUpTests.l15, setUpTests.l15,
                setUpTests.l15, setUpTests.l15, setUpTests.l15, setUpTests.l15,
                setUpTests.l17, setUpTests.l19, setUpTests.l19},
            STRING_VECTOR{
                setUpTests.l24, setUpTests.l4,  setUpTests.l5,  setUpTests.l15,
                setUpTests.l23, setUpTests.l16, setUpTests.l17, setUpTests.l19,
                setUpTests.l20, setUpTests.l22, setUpTests.l21, setUpTests.l18,
                setUpTests.l24, setUpTests.l16, setUpTests.l17, setUpTests.l19,
                setUpTests.l20, setUpTests.l22, setUpTests.l18, setUpTests.l21,
                setUpTests.l18, setUpTests.l20, setUpTests.l21}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarStmtAndUnderscore) {
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.stmt, setUpTests.underscore) ==
        STRING_SET{setUpTests.l23, setUpTests.l3, setUpTests.l14,
                   setUpTests.l15, setUpTests.l17, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(ParentStarUnderscoreAndLineNumber) {

    // line number is a child
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.underscore, setUpTests.c15));
    Assert::IsTrue(pkbQueryInterface.parentStar(
        setUpTests.underscore, setUpTests.c21)); // child inside else block

    // line number is not a child
    Assert::IsFalse(
        pkbQueryInterface.parentStar(setUpTests.underscore, setUpTests.c14));
    Assert::IsFalse(
        pkbQueryInterface.parentStar(setUpTests.underscore, setUpTests.c1));
  } // namespace UnitTesting

  TEST_METHOD(ParentStarUnderscoreAndStatement) {
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.underscore, setUpTests.w) ==
        STRING_SET{setUpTests.l23, setUpTests.l17});
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.underscore, setUpTests.a) ==
        STRING_SET{setUpTests.l4, setUpTests.l16, setUpTests.l18,
                   setUpTests.l20, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentStarUnderscoreAndStmt) {
    Assert::IsTrue(
        pkbQueryInterface.parentStar(setUpTests.underscore, setUpTests.stmt) ==
        STRING_SET{setUpTests.l4, setUpTests.l5, setUpTests.l15, setUpTests.l23,
                   setUpTests.l16, setUpTests.l17, setUpTests.l19,
                   setUpTests.l22, setUpTests.l18, setUpTests.l20,
                   setUpTests.l21, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentStarUnderscoreAndUnderscore) {
    Assert::IsTrue(pkbQueryInterface.parentStar(setUpTests.underscore,
                                                setUpTests.underscore));
  } // namespace UnitTesting
};

} // namespace UnitTesting
