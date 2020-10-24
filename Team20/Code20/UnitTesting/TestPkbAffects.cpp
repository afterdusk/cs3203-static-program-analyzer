#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestAffectsQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests =
      SetUpTests::SetUpTests(pkb, SetUpTests::TestNumber::C);

  TEST_METHOD(AffectsLineNumberAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c12, setUpTests.c18));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c18, setUpTests.c20));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c18, setUpTests.c24));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c21, setUpTests.c20));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c24, setUpTests.c20));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c24, setUpTests.c24));
    pkb.getQueryInterface()->clearCache();

    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.c1, setUpTests.c4));
    pkb.getQueryInterface()
        ->clearCache(); // there's a call in between that modifies
    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.c6, setUpTests.c12));
    pkb.getQueryInterface()->clearCache(); // can't go across procedures
    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.c7, setUpTests.c10));
    pkb.getQueryInterface()
        ->clearCache(); // there's a read in between that modifies
  }                     // namespace UnitTesting

  TEST_METHOD(AffectsLineNumberAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c12, setUpTests.a) ==
        LINE_SET{setUpTests.l18, setUpTests.l21, setUpTests.l13});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c13, setUpTests.a) ==
        LINE_SET{setUpTests.l18, setUpTests.l20, setUpTests.l21});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c18, setUpTests.a) ==
        LINE_SET{setUpTests.l20, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c20, setUpTests.a) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c24, setUpTests.a) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
    pkb.getQueryInterface()->clearCache();

    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c7, setUpTests.c) == LINE_SET());
    pkb.getQueryInterface()
        ->clearCache(); // there's a read that modifies, right after statement

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c18, setUpTests.c) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c18, setUpTests.w) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c18, setUpTests.r) == LINE_SET());
    pkb.getQueryInterface()->clearCache();

    // line number is not a statement
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c19, setUpTests.a) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c16, setUpTests.a) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c8, setUpTests.a) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsLineNumberAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c12, setUpTests.stmt) ==
        LINE_SET{setUpTests.l18, setUpTests.l21, setUpTests.l13});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c13, setUpTests.stmt) ==
        LINE_SET{setUpTests.l18, setUpTests.l20, setUpTests.l21});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c18, setUpTests.stmt) ==
        LINE_SET{setUpTests.l20, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c20, setUpTests.stmt) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c24, setUpTests.stmt) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsLineNumberAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c12,
                                                    setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c13,
                                                    setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c18,
                                                    setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c20,
                                                    setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c21,
                                                    setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c24,
                                                    setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();

    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.c1, setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.c4, setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.c6, setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(pkb.getQueryInterface()->affects(setUpTests.c26,
                                                     setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStatementAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.c21) ==
        LINE_SET{setUpTests.l12, setUpTests.l13});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.c24) ==
        LINE_SET{setUpTests.l20, setUpTests.l18, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.c13) ==
        LINE_SET{setUpTests.l12});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.c20) ==
        LINE_SET{setUpTests.l13, setUpTests.l20, setUpTests.l18, setUpTests.l21,
                 setUpTests.l24});
    pkb.getQueryInterface()->clearCache();

    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.a, setUpTests.c6) == LINE_SET());
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c, setUpTests.c18) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.p, setUpTests.c20) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.r, setUpTests.c21) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStatementAndStatement) {
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.a)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l12, setUpTests.l12, setUpTests.l12,
                        setUpTests.l13, setUpTests.l13, setUpTests.l13,
                        setUpTests.l18, setUpTests.l18, setUpTests.l20,
                        setUpTests.l20, setUpTests.l21, setUpTests.l24,
                        setUpTests.l24},
            LINE_VECTOR{setUpTests.l13, setUpTests.l18, setUpTests.l21,
                        setUpTests.l18, setUpTests.l20, setUpTests.l21,
                        setUpTests.l20, setUpTests.l24, setUpTests.l24,
                        setUpTests.l20, setUpTests.l20, setUpTests.l20,
                        setUpTests.l24})));
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.i, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.r, setUpTests.a) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStatementAndStmt) {
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l12, setUpTests.l12, setUpTests.l12,
                        setUpTests.l13, setUpTests.l13, setUpTests.l13,
                        setUpTests.l18, setUpTests.l18, setUpTests.l20,
                        setUpTests.l20, setUpTests.l21, setUpTests.l24,
                        setUpTests.l24},
            LINE_VECTOR{setUpTests.l13, setUpTests.l18, setUpTests.l21,
                        setUpTests.l18, setUpTests.l20, setUpTests.l21,
                        setUpTests.l20, setUpTests.l24, setUpTests.l24,
                        setUpTests.l20, setUpTests.l20, setUpTests.l20,
                        setUpTests.l24})));
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.p, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.w, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStatementAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.underscore) ==
        LINE_SET{setUpTests.l12, setUpTests.l13, setUpTests.l18, setUpTests.l20,
                 setUpTests.l21, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.p, setUpTests.underscore) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.r, setUpTests.underscore) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c, setUpTests.underscore) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStmtAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.c21) ==
        LINE_SET{setUpTests.l12, setUpTests.l13});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.c24) ==
        LINE_SET{setUpTests.l20, setUpTests.l18, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.c13) ==
        LINE_SET{setUpTests.l12});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.c20) ==
        LINE_SET{setUpTests.l13, setUpTests.l20, setUpTests.l18, setUpTests.l21,
                 setUpTests.l24});
    pkb.getQueryInterface()->clearCache();

    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.stmt, setUpTests.c6) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStmtAndStatement) {
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.a)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l12, setUpTests.l12, setUpTests.l12,
                        setUpTests.l13, setUpTests.l13, setUpTests.l13,
                        setUpTests.l18, setUpTests.l18, setUpTests.l20,
                        setUpTests.l20, setUpTests.l21, setUpTests.l24,
                        setUpTests.l24},
            LINE_VECTOR{setUpTests.l13, setUpTests.l18, setUpTests.l21,
                        setUpTests.l18, setUpTests.l20, setUpTests.l21,
                        setUpTests.l20, setUpTests.l24, setUpTests.l24,
                        setUpTests.l20, setUpTests.l20, setUpTests.l20,
                        setUpTests.l24})));
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.i) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.r) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStmtAndStmt) {
    Assert::IsTrue(
        convertToOrderedList(pkb.getQueryInterface()->affects(
            setUpTests.stmt, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l12, setUpTests.l12, setUpTests.l12,
                        setUpTests.l13, setUpTests.l13, setUpTests.l13,
                        setUpTests.l18, setUpTests.l18, setUpTests.l20,
                        setUpTests.l20, setUpTests.l21, setUpTests.l24,
                        setUpTests.l24},
            LINE_VECTOR{setUpTests.l13, setUpTests.l18, setUpTests.l21,
                        setUpTests.l18, setUpTests.l20, setUpTests.l21,
                        setUpTests.l20, setUpTests.l24, setUpTests.l24,
                        setUpTests.l20, setUpTests.l20, setUpTests.l20,
                        setUpTests.l24})));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStmtAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.stmt,
                                                    setUpTests.underscore) ==
                   LINE_SET{setUpTests.l12, setUpTests.l13, setUpTests.l18,
                            setUpTests.l20, setUpTests.l21, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsUnderscoreAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.c13));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.c18));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.c20));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.c21));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.c24));
    pkb.getQueryInterface()->clearCache();

    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.underscore, setUpTests.c6));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                     setUpTests.c10));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.underscore, setUpTests.c8));
    pkb.getQueryInterface()->clearCache(); // line number is not assignment
    Assert::IsFalse(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                     setUpTests.c26));
    pkb.getQueryInterface()->clearCache(); // only line in procedure
  }                                        // namespace UnitTesting

  TEST_METHOD(AffectsUnderscoreAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.underscore, setUpTests.a) ==
        LINE_SET{setUpTests.l13, setUpTests.l18, setUpTests.l20, setUpTests.l21,
                 setUpTests.l24});
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.underscore, setUpTests.c) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.underscore, setUpTests.r) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.underscore, setUpTests.i) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsUnderscoreAndStmt) {
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.stmt) ==
                   LINE_SET{setUpTests.l13, setUpTests.l18, setUpTests.l20,
                            setUpTests.l21, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  /*
   * AffectsStar query tests
   */
  TEST_METHOD(AffectsStarLineNumberAndLineNumber) {
    // same queries as affects
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c12, setUpTests.c18));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c18, setUpTests.c20));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c18, setUpTests.c24));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c21, setUpTests.c20));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c24, setUpTests.c20));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c24, setUpTests.c24));
    pkb.getQueryInterface()->clearCache();

    // transitive affects queries
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c12, setUpTests.c24));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c13, setUpTests.c24));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c21, setUpTests.c24));
    pkb.getQueryInterface()->clearCache();

    Assert::IsFalse(
        pkb.getQueryInterface()->affectsStar(setUpTests.c21, setUpTests.c18));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(
        pkb.getQueryInterface()->affectsStar(setUpTests.c18, setUpTests.c12));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(
        pkb.getQueryInterface()->affectsStar(setUpTests.c21, setUpTests.c13));
    pkb.getQueryInterface()->clearCache();

    Assert::IsFalse(
        pkb.getQueryInterface()->affectsStar(setUpTests.c1, setUpTests.c4));
    pkb.getQueryInterface()
        ->clearCache(); // there's a call in between that modifies
    Assert::IsFalse(
        pkb.getQueryInterface()->affectsStar(setUpTests.c6, setUpTests.c12));
    pkb.getQueryInterface()->clearCache(); // can't go across procedures
    Assert::IsFalse(
        pkb.getQueryInterface()->affectsStar(setUpTests.c7, setUpTests.c10));
    pkb.getQueryInterface()
        ->clearCache(); // there's a read in between that modifies
  }                     // namespace UnitTesting

  TEST_METHOD(AffectsStarLineNumberAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c12, setUpTests.a) ==
        LINE_SET{setUpTests.l18, setUpTests.l21, setUpTests.l13, setUpTests.l20,
                 setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c13, setUpTests.a) ==
        LINE_SET{setUpTests.l18, setUpTests.l20, setUpTests.l21,
                 setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c18, setUpTests.a) ==
        LINE_SET{setUpTests.l20, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c20, setUpTests.a) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c24, setUpTests.a) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
    pkb.getQueryInterface()->clearCache();

    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.c7, setUpTests.c) == LINE_SET());
    pkb.getQueryInterface()
        ->clearCache(); // there's a read that modifies, right after statement

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.c18, setUpTests.c) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.c18, setUpTests.w) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.c18, setUpTests.r) == LINE_SET());
    pkb.getQueryInterface()->clearCache();

    // line number is not a statement
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.c19, setUpTests.a) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.c16, setUpTests.a) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.c8, setUpTests.a) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarLineNumberAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c12, setUpTests.stmt) ==
        LINE_SET{setUpTests.l18, setUpTests.l21, setUpTests.l13, setUpTests.l20,
                 setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c13, setUpTests.stmt) ==
        LINE_SET{setUpTests.l18, setUpTests.l20, setUpTests.l21,
                 setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c18, setUpTests.stmt) ==
        LINE_SET{setUpTests.l20, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c20, setUpTests.stmt) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c24, setUpTests.stmt) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarLineNumberAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.c12,
                                                        setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.c13,
                                                        setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.c18,
                                                        setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.c20,
                                                        setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.c21,
                                                        setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.c24,
                                                        setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();

    Assert::IsFalse(pkb.getQueryInterface()->affectsStar(
        setUpTests.c1, setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(pkb.getQueryInterface()->affectsStar(
        setUpTests.c4, setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(pkb.getQueryInterface()->affectsStar(
        setUpTests.c6, setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(pkb.getQueryInterface()->affectsStar(
        setUpTests.c26, setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStatementAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.a, setUpTests.c21) ==
        LINE_SET{setUpTests.l12, setUpTests.l13});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.a, setUpTests.c24) ==
        LINE_SET{setUpTests.l20, setUpTests.l18, setUpTests.l24, setUpTests.l21,
                 setUpTests.l12, setUpTests.l13});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.a, setUpTests.c13) ==
        LINE_SET{setUpTests.l12});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.a, setUpTests.c20) ==
        LINE_SET{setUpTests.l13, setUpTests.l20, setUpTests.l18, setUpTests.l21,
                 setUpTests.l24, setUpTests.l12});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.a, setUpTests.c18) ==
        LINE_SET{setUpTests.l12, setUpTests.l13});
    pkb.getQueryInterface()->clearCache();

    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.a, setUpTests.c6) == LINE_SET());
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.c, setUpTests.c18) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.p, setUpTests.c20) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.r, setUpTests.c21) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStatementAndStatement) {
    Assert::IsTrue(
        convertToOrderedList(
            pkb.getQueryInterface()->affectsStar(setUpTests.a, setUpTests.a)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l12, setUpTests.l12, setUpTests.l12,
                        setUpTests.l12, setUpTests.l12, setUpTests.l13,
                        setUpTests.l13, setUpTests.l13, setUpTests.l13,
                        setUpTests.l18, setUpTests.l18, setUpTests.l20,
                        setUpTests.l20, setUpTests.l21, setUpTests.l21,
                        setUpTests.l24, setUpTests.l24},
            LINE_VECTOR{setUpTests.l13, setUpTests.l18, setUpTests.l21,
                        setUpTests.l20, setUpTests.l24, setUpTests.l18,
                        setUpTests.l20, setUpTests.l21, setUpTests.l24,
                        setUpTests.l20, setUpTests.l24, setUpTests.l24,
                        setUpTests.l20, setUpTests.l20, setUpTests.l24,
                        setUpTests.l20, setUpTests.l24})));
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.i, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.a, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.r, setUpTests.a) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.w, setUpTests.p) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStatementAndStmt) {
    Assert::IsTrue(
        convertToOrderedList(pkb.getQueryInterface()->affectsStar(
            setUpTests.a, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l12, setUpTests.l12, setUpTests.l12,
                        setUpTests.l12, setUpTests.l12, setUpTests.l13,
                        setUpTests.l13, setUpTests.l13, setUpTests.l13,
                        setUpTests.l18, setUpTests.l18, setUpTests.l20,
                        setUpTests.l20, setUpTests.l21, setUpTests.l21,
                        setUpTests.l24, setUpTests.l24},
            LINE_VECTOR{setUpTests.l13, setUpTests.l18, setUpTests.l21,
                        setUpTests.l20, setUpTests.l24, setUpTests.l18,
                        setUpTests.l20, setUpTests.l21, setUpTests.l24,
                        setUpTests.l20, setUpTests.l24, setUpTests.l24,
                        setUpTests.l20, setUpTests.l20, setUpTests.l24,
                        setUpTests.l20, setUpTests.l24})));
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.c, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.r, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.i, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStatementAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.a, setUpTests.underscore) ==
                   LINE_SET{setUpTests.l12, setUpTests.l13, setUpTests.l18,
                            setUpTests.l20, setUpTests.l21, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.p, setUpTests.underscore) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.r, setUpTests.underscore) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.c, setUpTests.underscore) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStmtAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.stmt, setUpTests.c21) ==
        LINE_SET{setUpTests.l12, setUpTests.l13});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.stmt, setUpTests.c24) ==
        LINE_SET{setUpTests.l20, setUpTests.l18, setUpTests.l24, setUpTests.l21,
                 setUpTests.l12, setUpTests.l13});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.stmt, setUpTests.c13) ==
        LINE_SET{setUpTests.l12});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.stmt, setUpTests.c20) ==
        LINE_SET{setUpTests.l13, setUpTests.l20, setUpTests.l18, setUpTests.l21,
                 setUpTests.l24, setUpTests.l12});
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.stmt, setUpTests.c18) ==
        LINE_SET{setUpTests.l12, setUpTests.l13});
    pkb.getQueryInterface()->clearCache();

    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.stmt, setUpTests.c6) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.stmt, setUpTests.c12) == LINE_SET());
    pkb.getQueryInterface()->clearCache();

    // line number is not an assignment statement
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.stmt, setUpTests.c9) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.stmt, setUpTests.c23) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.stmt, setUpTests.c25) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStmtAndStatement) {
    Assert::IsTrue(
        convertToOrderedList(pkb.getQueryInterface()->affectsStar(
            setUpTests.stmt, setUpTests.a)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l12, setUpTests.l12, setUpTests.l12,
                        setUpTests.l12, setUpTests.l12, setUpTests.l13,
                        setUpTests.l13, setUpTests.l13, setUpTests.l13,
                        setUpTests.l18, setUpTests.l18, setUpTests.l20,
                        setUpTests.l20, setUpTests.l21, setUpTests.l21,
                        setUpTests.l24, setUpTests.l24},
            LINE_VECTOR{setUpTests.l13, setUpTests.l18, setUpTests.l21,
                        setUpTests.l20, setUpTests.l24, setUpTests.l18,
                        setUpTests.l20, setUpTests.l21, setUpTests.l24,
                        setUpTests.l20, setUpTests.l24, setUpTests.l24,
                        setUpTests.l20, setUpTests.l20, setUpTests.l24,
                        setUpTests.l20, setUpTests.l24})));
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.stmt, setUpTests.i) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.stmt, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsStar(setUpTests.stmt, setUpTests.r) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStmtAndStmt) {
    Assert::IsTrue(
        convertToOrderedList(pkb.getQueryInterface()->affectsStar(
            setUpTests.stmt, setUpTests.stmt)) ==
        convertToOrderedList(LINE_LINE_PAIRS(
            LINE_VECTOR{setUpTests.l12, setUpTests.l12, setUpTests.l12,
                        setUpTests.l12, setUpTests.l12, setUpTests.l13,
                        setUpTests.l13, setUpTests.l13, setUpTests.l13,
                        setUpTests.l18, setUpTests.l18, setUpTests.l20,
                        setUpTests.l20, setUpTests.l21, setUpTests.l21,
                        setUpTests.l24, setUpTests.l24},
            LINE_VECTOR{setUpTests.l13, setUpTests.l18, setUpTests.l21,
                        setUpTests.l20, setUpTests.l24, setUpTests.l18,
                        setUpTests.l20, setUpTests.l21, setUpTests.l24,
                        setUpTests.l20, setUpTests.l24, setUpTests.l24,
                        setUpTests.l20, setUpTests.l20, setUpTests.l24,
                        setUpTests.l20, setUpTests.l24})));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStmtAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.stmt, setUpTests.underscore) ==
                   LINE_SET{setUpTests.l12, setUpTests.l13, setUpTests.l18,
                            setUpTests.l20, setUpTests.l21, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarUnderscoreAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                        setUpTests.c13));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                        setUpTests.c18));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                        setUpTests.c20));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                        setUpTests.c21));
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                        setUpTests.c24));
    pkb.getQueryInterface()->clearCache();

    Assert::IsFalse(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                         setUpTests.c6));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                         setUpTests.c10));
    pkb.getQueryInterface()->clearCache();
    Assert::IsFalse(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                         setUpTests.c8));
    pkb.getQueryInterface()->clearCache(); // line number is not assignment
    Assert::IsFalse(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                         setUpTests.c26));
    pkb.getQueryInterface()->clearCache(); // only line in procedure
  }                                        // namespace UnitTesting

  TEST_METHOD(AffectsStarUnderscoreAndStatement) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                        setUpTests.a) ==
                   LINE_SET{setUpTests.l13, setUpTests.l18, setUpTests.l20,
                            setUpTests.l21, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.underscore, setUpTests.c) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.underscore, setUpTests.r) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(
                       setUpTests.underscore, setUpTests.i) == LINE_SET());
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarUnderscoreAndStmt) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                        setUpTests.stmt) ==
                   LINE_SET{setUpTests.l13, setUpTests.l18, setUpTests.l20,
                            setUpTests.l21, setUpTests.l24});
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsStar(setUpTests.underscore,
                                                        setUpTests.underscore));
    pkb.getQueryInterface()->clearCache();
  } // namespace UnitTesting
};
} // namespace UnitTesting
