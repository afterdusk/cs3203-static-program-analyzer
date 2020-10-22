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
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c18, setUpTests.c20));
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c18, setUpTests.c24));
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c21, setUpTests.c20));
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c24, setUpTests.c20));
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c24, setUpTests.c24));

    Assert::IsFalse(pkb.getQueryInterface()->affects(
        setUpTests.c1,
        setUpTests.c4)); // there's a call in between that modifies
    Assert::IsFalse(pkb.getQueryInterface()->affects(
        setUpTests.c6, setUpTests.c12)); // can't go across procedures
    Assert::IsFalse(pkb.getQueryInterface()->affects(
        setUpTests.c7,
        setUpTests.c10)); // there's a read in between that modifies
  }                       // namespace UnitTesting

  TEST_METHOD(AffectsLineNumberAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c12, setUpTests.a) ==
        LINE_SET{setUpTests.l18, setUpTests.l21, setUpTests.l13});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c13, setUpTests.a) ==
        LINE_SET{setUpTests.l18, setUpTests.l20, setUpTests.l21});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c18, setUpTests.a) ==
        LINE_SET{setUpTests.l20, setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c20, setUpTests.a) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c24, setUpTests.a) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});

    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c7, setUpTests.c) ==
        LINE_SET()); // there's a read that modifies, right after statement

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c18, setUpTests.c) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c18, setUpTests.w) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c18, setUpTests.r) == LINE_SET());

    // line number is not a statement
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c19, setUpTests.a) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c16, setUpTests.a) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c8, setUpTests.a) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(AffectsLineNumberAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c12, setUpTests.stmt) ==
        LINE_SET{setUpTests.l18, setUpTests.l21, setUpTests.l13});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c13, setUpTests.stmt) ==
        LINE_SET{setUpTests.l18, setUpTests.l20, setUpTests.l21});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c18, setUpTests.stmt) ==
        LINE_SET{setUpTests.l20, setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c20, setUpTests.stmt) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c24, setUpTests.stmt) ==
        LINE_SET{setUpTests.l24, setUpTests.l20});
  } // namespace UnitTesting

  TEST_METHOD(AffectsLineNumberAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c12,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c13,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c18,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c20,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c21,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.c24,
                                                    setUpTests.underscore));

    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.c1, setUpTests.underscore));
    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.c4, setUpTests.underscore));
    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.c6, setUpTests.underscore));
    Assert::IsFalse(pkb.getQueryInterface()->affects(setUpTests.c26,
                                                     setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(AffectsStatementAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.c21) ==
        LINE_SET{setUpTests.l12, setUpTests.l13});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.c24) ==
        LINE_SET{setUpTests.l20, setUpTests.l18, setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.c13) ==
        LINE_SET{setUpTests.l12});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.c20) ==
        LINE_SET{setUpTests.l13, setUpTests.l20, setUpTests.l18, setUpTests.l21,
                 setUpTests.l24});

    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.a, setUpTests.c6) == LINE_SET());

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c, setUpTests.c18) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.p, setUpTests.c20) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.r, setUpTests.c21) == LINE_SET());
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

    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.i, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.r, setUpTests.a) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
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

    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.c, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.p, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.w, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
  } // namespace UnitTesting

  TEST_METHOD(AffectsStatementAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.a, setUpTests.underscore) ==
        LINE_SET{setUpTests.l12, setUpTests.l13, setUpTests.l18, setUpTests.l20,
                 setUpTests.l21, setUpTests.l24});

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.p, setUpTests.underscore) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.r, setUpTests.underscore) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.c, setUpTests.underscore) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(AffectsStmtAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.c21) ==
        LINE_SET{setUpTests.l12, setUpTests.l13});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.c24) ==
        LINE_SET{setUpTests.l20, setUpTests.l18, setUpTests.l24});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.c13) ==
        LINE_SET{setUpTests.l12});
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.c20) ==
        LINE_SET{setUpTests.l13, setUpTests.l20, setUpTests.l18, setUpTests.l21,
                 setUpTests.l24});

    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.stmt, setUpTests.c6) == LINE_SET());
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

    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.i) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.stmt, setUpTests.r) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));
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
  } // namespace UnitTesting

  TEST_METHOD(AffectsStmtAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.stmt,
                                                    setUpTests.underscore) ==
                   LINE_SET{setUpTests.l12, setUpTests.l13, setUpTests.l18,
                            setUpTests.l20, setUpTests.l21, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(AffectsUnderscoreAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.c13));
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.c18));
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.c20));
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.c21));
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.c24));

    Assert::IsFalse(
        pkb.getQueryInterface()->affects(setUpTests.underscore, setUpTests.c6));
    Assert::IsFalse(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                     setUpTests.c10));
    Assert::IsFalse(pkb.getQueryInterface()->affects(
        setUpTests.underscore, setUpTests.c8)); // line number is not assignment
    Assert::IsFalse(pkb.getQueryInterface()->affects(
        setUpTests.underscore, setUpTests.c26)); // only line in procedure
  }                                              // namespace UnitTesting

  TEST_METHOD(AffectsUnderscoreAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affects(setUpTests.underscore, setUpTests.a) ==
        LINE_SET{setUpTests.l13, setUpTests.l18, setUpTests.l20, setUpTests.l21,
                 setUpTests.l24});

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.underscore, setUpTests.c) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.underscore, setUpTests.r) == LINE_SET());
    Assert::IsTrue(pkb.getQueryInterface()->affects(
                       setUpTests.underscore, setUpTests.i) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(AffectsUnderscoreAndStmt) {
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.stmt) ==
                   LINE_SET{setUpTests.l13, setUpTests.l18, setUpTests.l20,
                            setUpTests.l21, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(AffectsUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affects(setUpTests.underscore,
                                                    setUpTests.underscore));
  } // namespace UnitTesting

  /*
   * AffectsStar query tests
   */
  TEST_METHOD(AffectsStarLineNumberAndLineNumber) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarLineNumberAndStatement) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarLineNumberAndStmt) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarLineNumberAndUnderscore) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStatementAndLineNumber) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStatementAndStatement) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStatementAndStmt) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStatementAndUnderscore) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStmtAndLineNumber) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStmtAndStatement) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStmtAndStmt) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarStmtAndUnderscore) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarUnderscoreAndLineNumber) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarUnderscoreAndStatement) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarUnderscoreAndStmt) {
    Assert::IsTrue(false);
  } // namespace UnitTesting

  TEST_METHOD(AffectsStarUnderscoreAndUnderscore) {
    Assert::IsTrue(false);
  } // namespace UnitTesting
};
} // namespace UnitTesting
