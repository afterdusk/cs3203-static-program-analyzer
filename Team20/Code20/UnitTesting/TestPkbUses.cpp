#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestUsesQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests = SetUpTests::SetUpTests(pkb);

  TEST_METHOD(UsesLineNumberAndString) {
    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.c4, setUpTests.strx));
    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.c4, setUpTests.stry));

    Assert::IsTrue(pkb.getQueryInterface()->uses(
        setUpTests.c11, setUpTests.strq)); // used in call procedure

    Assert::IsFalse(pkb.getQueryInterface()->uses(
        setUpTests.c11,
        setUpTests.stry)); // call procedure doesnt use input var
    Assert::IsFalse(pkb.getQueryInterface()->uses(
        setUpTests.c2, setUpTests.stry)); // line doesnt use
    Assert::IsFalse(
        pkb.getQueryInterface()->uses(setUpTests.c4,
                                      setUpTests.strr)); // incorrect var used
  } // namespace UnitTesting

  TEST_METHOD(UsesLineNumberAndVariable) {

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.c4, setUpTests.variable) ==
        NAME_SET{"x", "y"}); // on assign statement
    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.c3, setUpTests.variable) ==
        NAME_SET{"x", "y"}); // on while statement
    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.c7, setUpTests.variable) ==
        NAME_SET{"x", "y"}); // on call statement
  }                          // namespace UnitTesting

  TEST_METHOD(UsesLineNumberAndUnderscore) {

    Assert::IsTrue(pkb.getQueryInterface()->uses(
        setUpTests.c3, setUpTests.underscore)); // on while statement
    Assert::IsTrue(pkb.getQueryInterface()->uses(
        setUpTests.c7, setUpTests.underscore)); // on call statement
    Assert::IsTrue(pkb.getQueryInterface()->uses(
        setUpTests.c26, setUpTests.underscore)); // on print statement

    Assert::IsFalse(pkb.getQueryInterface()->uses(
        setUpTests.c12, setUpTests.underscore)); // on read statement
  }                                              // namespace UnitTesting

  TEST_METHOD(UsesStmtAndString) {

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.stmt, setUpTests.stry) ==
        LINE_SET{setUpTests.l3, setUpTests.l4, setUpTests.l7, setUpTests.l28,
                 setUpTests.l29, setUpTests.l30, setUpTests.l31, setUpTests.l32,
                 setUpTests.l33});
    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.stmt, setUpTests.strr) ==
        LINE_SET{setUpTests.l10, setUpTests.l29, setUpTests.l31, setUpTests.l32,
                 setUpTests.l33});

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.stmt, setUpTests.strm) ==
        LINE_SET{}); // var not used

  } // namespace UnitTesting

  TEST_METHOD(UsesStmtAndVariable) {

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.stmt, setUpTests.variable) ==
        LINE_NAME_PAIRS{
            LINE_VECTOR{
                setUpTests.l11, setUpTests.l11, setUpTests.l11, setUpTests.l3,
                setUpTests.l3,  setUpTests.l4,  setUpTests.l4,  setUpTests.l14,
                setUpTests.l14, setUpTests.l6,  setUpTests.l15, setUpTests.l15,
                setUpTests.l7,  setUpTests.l7,  setUpTests.l10, setUpTests.l10,
                setUpTests.l16, setUpTests.l17, setUpTests.l17, setUpTests.l18,
                setUpTests.l19, setUpTests.l20, setUpTests.l21, setUpTests.l22,
                setUpTests.l23, setUpTests.l24, setUpTests.l25, setUpTests.l26,
                setUpTests.l27, setUpTests.l28, setUpTests.l28, setUpTests.l29,
                setUpTests.l29, setUpTests.l29, setUpTests.l29, setUpTests.l29,
                setUpTests.l29, setUpTests.l30, setUpTests.l30, setUpTests.l30,
                setUpTests.l31, setUpTests.l31, setUpTests.l31, setUpTests.l31,
                setUpTests.l31, setUpTests.l31, setUpTests.l32, setUpTests.l32,
                setUpTests.l32, setUpTests.l32, setUpTests.l32, setUpTests.l32,
                setUpTests.l33, setUpTests.l33, setUpTests.l33, setUpTests.l33,
                setUpTests.l33, setUpTests.l33},
            NAME_VECTOR{"q", "t", "k", "x", "y", "x", "y", "q", "t", "x",
                        "q", "t", "y", "x", "x", "r", "t", "q", "t", "t",
                        "q", "q", "q", "t", "q", "q", "k", "k", "k", "y",
                        "x", "x", "r", "y", "q", "t", "k", "k", "y", "x",
                        "k", "y", "q", "x", "r", "t", "x", "r", "y", "q",
                        "t", "k", "k", "y", "q", "x", "r", "t"}});
  } // namespace UnitTesting

  TEST_METHOD(UsesStmtAndUnderscore) {

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.stmt, setUpTests.underscore) ==
        LINE_SET{setUpTests.l3,  setUpTests.l4,  setUpTests.l6,  setUpTests.l7,
                 setUpTests.l10, setUpTests.l11, setUpTests.l14, setUpTests.l15,
                 setUpTests.l16, setUpTests.l17, setUpTests.l18, setUpTests.l19,
                 setUpTests.l20, setUpTests.l21, setUpTests.l22, setUpTests.l23,
                 setUpTests.l24, setUpTests.l25, setUpTests.l26, setUpTests.l27,
                 setUpTests.l28, setUpTests.l29, setUpTests.l30, setUpTests.l31,
                 setUpTests.l32, setUpTests.l33});

  } // namespace UnitTesting

  TEST_METHOD(UsesStatementAndString) {

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.a, setUpTests.strx) ==
        LINE_SET{setUpTests.l4, setUpTests.l10});
    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.w, setUpTests.strt) ==
        LINE_SET{setUpTests.l14, setUpTests.l17});
    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.c, setUpTests.strx) ==
        LINE_SET{setUpTests.l7, setUpTests.l28, setUpTests.l29, setUpTests.l30,
                 setUpTests.l31, setUpTests.l32, setUpTests.l33});

  } // namespace UnitTesting

  TEST_METHOD(UsesStatementAndVariable) {

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.a, setUpTests.variable) ==
        LINE_NAME_PAIRS(LINE_VECTOR{setUpTests.l4, setUpTests.l4,
                                    setUpTests.l20, setUpTests.l10,
                                    setUpTests.l10, setUpTests.l18,
                                    setUpTests.l16, setUpTests.l24},
                        NAME_VECTOR{"x", "y", "q", "x", "r", "t", "t", "q"}));

  } // namespace UnitTesting

  TEST_METHOD(UsesStatementAndUnderscore) {

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.w, setUpTests.underscore) ==
        LINE_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                 setUpTests.l23});

  } // namespace UnitTesting

  TEST_METHOD(UsesStringAndString) {

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.strpmain,
                                      setUpTests.strx)); // proc directly using
    Assert::IsTrue(pkb.getQueryInterface()->uses(
        setUpTests.strpmain,
        setUpTests.strt)); // proc indirectly using through call
    Assert::IsTrue(pkb.getQueryInterface()->uses(
        setUpTests.strpmain,
        setUpTests.strk)); // proc indirectly using through nested call

    Assert::IsFalse(pkb.getQueryInterface()->uses(
        setUpTests.strpaux, setUpTests.strr)); // proc doesn't use that var

  } // namespace UnitTesting

  TEST_METHOD(UsesStringAndVariable) {

    Assert::IsTrue(pkb.getQueryInterface()->uses(setUpTests.strpaux,
                                                 setUpTests.variable) ==
                   NAME_SET{"x", "y"}); // no calls, only vars used directly
    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.strpmain,
                                      setUpTests.variable) ==
        NAME_SET{"x", "y", "r", "q", "t",
                 "k"}); // proc main uses all vars directly & indirectly

  } // namespace UnitTesting

  TEST_METHOD(UsesStringAndUnderscore) {

    Assert::IsTrue(pkb.getQueryInterface()->uses(setUpTests.strpaux,
                                                 setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->uses(setUpTests.strpextra,
                                                 setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->uses(setUpTests.strpmain,
                                                 setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->uses(setUpTests.strpcomplicate,
                                                 setUpTests.underscore));

  } // namespace UnitTesting

  TEST_METHOD(UsesProcedureAndString) {

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.procedure, setUpTests.strx) ==
        NAME_SET{"aux", "main", "extratwo", "extrathree", "extrafour"});
    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.procedure, setUpTests.strk) ==
        NAME_SET{"main", "complicate", "extra", "extratwo", "extrathree",
                 "extrafour"});
  } // namespace UnitTesting

  TEST_METHOD(UsesProcedureAndVariable) {

    Assert::IsTrue(
        pkb.getQueryInterface()->uses(setUpTests.procedure,
                                      setUpTests.variable) ==
        NAME_NAME_PAIRS(
            NAME_VECTOR{"extrafour",  "extrafour",  "extrafour",  "extrafour",
                        "extrafour",  "extrafour",  "extra",      "aux",
                        "aux",        "main",       "main",       "main",
                        "main",       "main",       "main",       "complicate",
                        "complicate", "complicate", "extratwo",   "extratwo",
                        "extratwo",   "extrathree", "extrathree", "extrathree",
                        "extrathree", "extrathree", "extrathree"},
            NAME_VECTOR{"k", "y", "q", "x", "r", "t", "k", "y", "x",
                        "x", "r", "y", "q", "t", "k", "q", "t", "k",
                        "k", "y", "x", "k", "y", "q", "x", "r", "t"}));
  } // namespace UnitTesting

  TEST_METHOD(UsesProcedureAndUnderscore) {

    Assert::IsTrue(pkb.getQueryInterface()->uses(setUpTests.procedure,
                                                 setUpTests.underscore) ==
                   NAME_SET{"aux", "main", "complicate", "extra", "extratwo",
                            "extrathree", "extrafour"});

  } // namespace UnitTesting
};

} // namespace UnitTesting
