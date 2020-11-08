#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestAffectsBipQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests =
      SetUpTests::SetUpTests(pkb, SetUpTests::TestNumber::D);

  TEST_METHOD(AffectsBipLineNumberAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.c24, setUpTests.c25));
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipLineNumberAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.c21, setUpTests.a) ==
        LINE_SET{setUpTests.l23, setUpTests.l31});
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipLineNumberAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.c21, setUpTests.stmt) ==
        LINE_SET{setUpTests.l23, setUpTests.l31});
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipLineNumberAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.c30,
                                                       setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.c24,
                                                       setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.c20,
                                                       setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStatementAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.a, setUpTests.c22) ==
        LINE_SET{setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStatementAndStatement) {
    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.i, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.a, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.r, setUpTests.a) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStatementAndStmt) {
    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.c, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.p, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.w, setUpTests.stmt) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStatementAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.a,
                                                       setUpTests.underscore) ==
                   LINE_SET{setUpTests.l20, setUpTests.l21, setUpTests.l22,
                            setUpTests.l23, setUpTests.l24, setUpTests.l25,
                            setUpTests.l26, setUpTests.l27, setUpTests.l28,
                            setUpTests.l29, setUpTests.l30});

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(
                       setUpTests.p, setUpTests.underscore) == LINE_SET());

    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(
                       setUpTests.r, setUpTests.underscore) == LINE_SET());

    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(
                       setUpTests.c, setUpTests.underscore) == LINE_SET());

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStmtAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.stmt, setUpTests.c20) ==
        LINE_SET{setUpTests.l22, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStmtAndStatement) {
    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.stmt, setUpTests.i) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.stmt, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBip(setUpTests.stmt, setUpTests.r) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStmtAndStmt) {} // namespace UnitTesting

  TEST_METHOD(AffectsBipStmtAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.stmt,
                                                       setUpTests.underscore) ==
                   LINE_SET{setUpTests.l20, setUpTests.l21, setUpTests.l22,
                            setUpTests.l23, setUpTests.l24, setUpTests.l25,
                            setUpTests.l26, setUpTests.l27, setUpTests.l28,
                            setUpTests.l29, setUpTests.l30});

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipUnderscoreAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.underscore,
                                                       setUpTests.c23));
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.underscore,
                                                       setUpTests.c27));
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.underscore,
                                                       setUpTests.c30));
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipUnderscoreAndStatement) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.underscore,
                                                       setUpTests.a) ==
                   LINE_SET{setUpTests.l20, setUpTests.l21, setUpTests.l22,
                            setUpTests.l23, setUpTests.l24, setUpTests.l25,
                            setUpTests.l26, setUpTests.l27, setUpTests.l28,
                            setUpTests.l29, setUpTests.l30, setUpTests.l31});

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(
                       setUpTests.underscore, setUpTests.c) == LINE_SET());

    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(
                       setUpTests.underscore, setUpTests.r) == LINE_SET());

    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(
                       setUpTests.underscore, setUpTests.i) == LINE_SET());

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipUnderscoreAndStmt) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.underscore,
                                                       setUpTests.stmt) ==
                   LINE_SET{setUpTests.l20, setUpTests.l21, setUpTests.l22,
                            setUpTests.l23, setUpTests.l24, setUpTests.l25,
                            setUpTests.l26, setUpTests.l27, setUpTests.l28,
                            setUpTests.l29, setUpTests.l30, setUpTests.l31});
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBip(setUpTests.underscore,
                                                       setUpTests.underscore));

  } // namespace UnitTesting

  /*
   * AffectsBipStar query tests
   */
  TEST_METHOD(AffectsBipStarLineNumberAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(setUpTests.c24,
                                                           setUpTests.c25));
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(setUpTests.c20,
                                                           setUpTests.c29));
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarLineNumberAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBipStar(setUpTests.c23, setUpTests.a) ==
        LINE_SET{setUpTests.l31});
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarLineNumberAndStmt) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(setUpTests.c23,
                                                           setUpTests.stmt) ==
                   LINE_SET{setUpTests.l31});
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarLineNumberAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
        setUpTests.c30, setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
        setUpTests.c24, setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
        setUpTests.c20, setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarStatementAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBipStar(setUpTests.a, setUpTests.c24) ==
        LINE_SET{setUpTests.l20, setUpTests.l22, setUpTests.l24});
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarStatementAndStatement) {
    // wrong statement types
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBipStar(setUpTests.i, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBipStar(setUpTests.a, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBipStar(setUpTests.r, setUpTests.a) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBipStar(setUpTests.c, setUpTests.c) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBipStar(setUpTests.w, setUpTests.p) ==
        LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarStatementAndStmt) {
    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(setUpTests.c,
                                                           setUpTests.stmt) ==
                   LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(setUpTests.r,
                                                           setUpTests.stmt) ==
                   LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(setUpTests.i,
                                                           setUpTests.stmt) ==
                   LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarStatementAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBipStar(setUpTests.a,
                                                setUpTests.underscore) ==
        LINE_SET{setUpTests.l20, setUpTests.l21, setUpTests.l22, setUpTests.l23,
                 setUpTests.l24, setUpTests.l25, setUpTests.l26, setUpTests.l27,
                 setUpTests.l28, setUpTests.l29, setUpTests.l30});

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
                       setUpTests.p, setUpTests.underscore) == LINE_SET());

    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
                       setUpTests.r, setUpTests.underscore) == LINE_SET());

    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
                       setUpTests.c, setUpTests.underscore) == LINE_SET());

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarStmtAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(setUpTests.stmt,
                                                           setUpTests.c21) ==
                   LINE_SET{setUpTests.l28, setUpTests.l20, setUpTests.l22,
                            setUpTests.l24, setUpTests.l25, setUpTests.l26,
                            setUpTests.l27, setUpTests.l29, setUpTests.l30});
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarStmtAndStatement) {
    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(setUpTests.stmt,
                                                           setUpTests.i) ==
                   LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(setUpTests.stmt,
                                                           setUpTests.c) ==
                   LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(setUpTests.stmt,
                                                           setUpTests.r) ==
                   LINE_LINE_PAIRS(LINE_VECTOR(), LINE_VECTOR()));

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarStmtAndStmt) {} // namespace UnitTesting

  TEST_METHOD(AffectsBipStarStmtAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->affectsBipStar(setUpTests.stmt,
                                                setUpTests.underscore) ==
        LINE_SET{setUpTests.l20, setUpTests.l21, setUpTests.l22, setUpTests.l23,
                 setUpTests.l24, setUpTests.l25, setUpTests.l26, setUpTests.l27,
                 setUpTests.l28, setUpTests.l29, setUpTests.l30});
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarUnderscoreAndLineNumber) {
    Assert::IsFalse(pkb.getQueryInterface()->affectsBipStar(
        setUpTests.underscore, setUpTests.c13));
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
        setUpTests.underscore, setUpTests.c24));
  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarUnderscoreAndStatement) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
                       setUpTests.underscore, setUpTests.a) ==
                   LINE_SET{setUpTests.l20, setUpTests.l21, setUpTests.l22,
                            setUpTests.l23, setUpTests.l24, setUpTests.l25,
                            setUpTests.l26, setUpTests.l27, setUpTests.l28,
                            setUpTests.l29, setUpTests.l30, setUpTests.l31});

    // wrong statement types
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
                       setUpTests.underscore, setUpTests.c) == LINE_SET());

    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
                       setUpTests.underscore, setUpTests.r) == LINE_SET());

    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
                       setUpTests.underscore, setUpTests.i) == LINE_SET());

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarUnderscoreAndStmt) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
                       setUpTests.underscore, setUpTests.stmt) ==
                   LINE_SET{setUpTests.l20, setUpTests.l21, setUpTests.l22,
                            setUpTests.l23, setUpTests.l24, setUpTests.l25,
                            setUpTests.l26, setUpTests.l27, setUpTests.l28,
                            setUpTests.l29, setUpTests.l30, setUpTests.l31});

  } // namespace UnitTesting

  TEST_METHOD(AffectsBipStarUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->affectsBipStar(
        setUpTests.underscore, setUpTests.underscore));
  } // namespace UnitTesting
};
} // namespace UnitTesting
