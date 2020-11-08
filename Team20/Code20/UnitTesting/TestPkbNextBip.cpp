#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestNextBipQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests =
      SetUpTests::SetUpTests(pkb, SetUpTests::TestNumber::D);

  TEST_METHOD(NextBipLineNumberAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.c1, setUpTests.c24));
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.c23, setUpTests.c5));
  } // namespace UnitTesting

  TEST_METHOD(NextBipLineNumberAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.c8, setUpTests.c) ==
        LINE_SET{setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(NextBipLineNumberAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.c8, setUpTests.stmt) ==
        LINE_SET{setUpTests.l9, setUpTests.l19});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.c28, setUpTests.stmt) ==
        LINE_SET{setUpTests.l16});
  } // namespace UnitTesting

  TEST_METHOD(NextBipLineNumberAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.c13,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.c14,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.c18,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.c21,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.c11,
                                                    setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(NextBipStatementAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.c, setUpTests.c23) ==
        LINE_SET{setUpTests.l4});
  } // namespace UnitTesting

  TEST_METHOD(NextBipStatementAndStatement) {} // namespace UnitTesting

  TEST_METHOD(NextBipStatementAndStmt) {} // namespace UnitTesting

  TEST_METHOD(NextBipStatementAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.c, setUpTests.underscore) ==
        LINE_SET{setUpTests.l1, setUpTests.l2, setUpTests.l3, setUpTests.l4,
                 setUpTests.l5, setUpTests.l6, setUpTests.l7, setUpTests.l10,
                 setUpTests.l12, setUpTests.l14, setUpTests.l15, setUpTests.l16,
                 setUpTests.l18, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(NextBipStmtAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.stmt, setUpTests.c23) ==
        LINE_SET{setUpTests.l4});

  } // namespace UnitTesting

  TEST_METHOD(NextBipStmtAndStatement) {} // namespace UnitTesting

  TEST_METHOD(NextBipStmtAndStmt) {} // namespace UnitTesting

  TEST_METHOD(NextBipStmtAndUnderscore) {} // namespace UnitTesting

  TEST_METHOD(NextBipUnderscoreAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.underscore,
                                                    setUpTests.c16));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.underscore,
                                                    setUpTests.c21));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.underscore,
                                                    setUpTests.c15));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.underscore,
                                                    setUpTests.c12));
  } // namespace UnitTesting

  TEST_METHOD(NextBipUnderscoreAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.underscore, setUpTests.c) ==
        LINE_SET{setUpTests.l2, setUpTests.l3, setUpTests.l4, setUpTests.l5,
                 setUpTests.l6, setUpTests.l7, setUpTests.l10, setUpTests.l12,
                 setUpTests.l14, setUpTests.l15, setUpTests.l16, setUpTests.l18,
                 setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(NextBipUnderscoreAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.underscore,
                                         setUpTests.stmt) ==
        LINE_SET{setUpTests.l2,  setUpTests.l10, setUpTests.l24, setUpTests.l16,
                 setUpTests.l3,  setUpTests.l6,  setUpTests.l22, setUpTests.l4,
                 setUpTests.l5,  setUpTests.l23, setUpTests.l7,  setUpTests.l20,
                 setUpTests.l8,  setUpTests.l9,  setUpTests.l19, setUpTests.l11,
                 setUpTests.l25, setUpTests.l12, setUpTests.l13, setUpTests.l26,
                 setUpTests.l14, setUpTests.l15, setUpTests.l17, setUpTests.l27,
                 setUpTests.l28, setUpTests.l29, setUpTests.l18, setUpTests.l30,
                 setUpTests.l31, setUpTests.l21});
  } // namespace UnitTesting

  TEST_METHOD(NextBipUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.underscore,
                                                    setUpTests.underscore));
  } // namespace UnitTesting

  /*
   * NextBipStar query tests
   */
  TEST_METHOD(NextBipStarLineNumberAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBipStar(setUpTests.c1, setUpTests.c22));
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBipStar(setUpTests.c3, setUpTests.c29));
  } // namespace UnitTesting

  TEST_METHOD(NextBipStarLineNumberAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBipStar(setUpTests.c5, setUpTests.a) ==
        LINE_SET{setUpTests.l31});
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBipStar(setUpTests.c4, setUpTests.a) ==
        LINE_SET{setUpTests.l23, setUpTests.l31});
  } // namespace UnitTesting

  TEST_METHOD(NextBipStarLineNumberAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBipStar(setUpTests.c4, setUpTests.stmt) ==
        LINE_SET{setUpTests.l23, setUpTests.l31, setUpTests.l5});
  } // namespace UnitTesting

  TEST_METHOD(NextBipStarLineNumberAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.c13,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.c14,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.c18,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.c21,
                                                    setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.c11,
                                                    setUpTests.underscore));
  } // namespace UnitTesting

  TEST_METHOD(NextBipStarStatementAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->nextBipStar(
                       setUpTests.p, setUpTests.c16) == LINE_SET());
  } // namespace UnitTesting

  TEST_METHOD(NextBipStarStatementAndStatement) {} // namespace UnitTesting

  TEST_METHOD(NextBipStarStatementAndStmt) {} // namespace UnitTesting

  TEST_METHOD(NextBipStarStatementAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.c, setUpTests.underscore) ==
        LINE_SET{setUpTests.l1, setUpTests.l2, setUpTests.l3, setUpTests.l4,
                 setUpTests.l5, setUpTests.l6, setUpTests.l7, setUpTests.l10,
                 setUpTests.l12, setUpTests.l14, setUpTests.l15, setUpTests.l16,
                 setUpTests.l18, setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(NextBipStarStmtAndLineNumber) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBipStar(setUpTests.stmt, setUpTests.c5) ==
        LINE_SET{setUpTests.l1,  setUpTests.l2,  setUpTests.l3,  setUpTests.l4,
                 setUpTests.l6,  setUpTests.l7,  setUpTests.l8,  setUpTests.l9,
                 setUpTests.l10, setUpTests.l11, setUpTests.l12, setUpTests.l13,
                 setUpTests.l14, setUpTests.l15, setUpTests.l16, setUpTests.l17,
                 setUpTests.l18, setUpTests.l19, setUpTests.l20, setUpTests.l21,
                 setUpTests.l22, setUpTests.l23, setUpTests.l24, setUpTests.l25,
                 setUpTests.l26, setUpTests.l27, setUpTests.l28, setUpTests.l29,
                 setUpTests.l30});
  } // namespace UnitTesting

  TEST_METHOD(NextBipStarStmtAndStatement) {} // namespace UnitTesting

  TEST_METHOD(NextBipStarStmtAndStmt) {} // namespace UnitTesting

  TEST_METHOD(NextBipStarStmtAndUnderscore) {} // namespace UnitTesting

  TEST_METHOD(NextBipStarUnderscoreAndLineNumber) {
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.underscore,
                                                    setUpTests.c16));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.underscore,
                                                    setUpTests.c21));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.underscore,
                                                    setUpTests.c15));
    Assert::IsTrue(pkb.getQueryInterface()->nextBip(setUpTests.underscore,
                                                    setUpTests.c12));
  } // namespace UnitTesting

  TEST_METHOD(NextBipStarUnderscoreAndStatement) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBip(setUpTests.underscore, setUpTests.c) ==
        LINE_SET{setUpTests.l2, setUpTests.l3, setUpTests.l4, setUpTests.l5,
                 setUpTests.l6, setUpTests.l7, setUpTests.l10, setUpTests.l12,
                 setUpTests.l14, setUpTests.l15, setUpTests.l16, setUpTests.l18,
                 setUpTests.l19});
  } // namespace UnitTesting

  TEST_METHOD(NextBipStarUnderscoreAndStmt) {
    Assert::IsTrue(
        pkb.getQueryInterface()->nextBipStar(setUpTests.underscore,
                                             setUpTests.stmt) ==
        LINE_SET{setUpTests.l2,  setUpTests.l10, setUpTests.l24, setUpTests.l16,
                 setUpTests.l3,  setUpTests.l6,  setUpTests.l22, setUpTests.l4,
                 setUpTests.l5,  setUpTests.l23, setUpTests.l7,  setUpTests.l20,
                 setUpTests.l8,  setUpTests.l9,  setUpTests.l19, setUpTests.l11,
                 setUpTests.l25, setUpTests.l12, setUpTests.l13, setUpTests.l26,
                 setUpTests.l14, setUpTests.l15, setUpTests.l17, setUpTests.l27,
                 setUpTests.l28, setUpTests.l29, setUpTests.l18, setUpTests.l30,
                 setUpTests.l31, setUpTests.l21});
  } // namespace UnitTesting

  TEST_METHOD(NextBipStarUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->nextBipStar(setUpTests.underscore,
                                                        setUpTests.underscore));
  } // namespace UnitTesting
};
} // namespace UnitTesting
