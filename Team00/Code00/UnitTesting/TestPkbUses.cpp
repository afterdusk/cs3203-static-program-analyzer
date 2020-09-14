#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestUsesQuery) {

public:
  SetUpTests setUpTests;

  TEST_METHOD_INITIALIZE(SetUpPkb) { setUpTests = SetUpTests::SetUpTests(); }
  TEST_METHOD(UsesLineNumberAndString) {
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.c4, setUpTests.strx));
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.c4, setUpTests.stry));

    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(
        setUpTests.c11, setUpTests.strq)); // used in call procedure

    Assert::IsFalse(setUpTests.pkbQueryInterface.uses(
        setUpTests.c11,
        setUpTests.stry)); // call procedure doesnt use input var
    Assert::IsFalse(setUpTests.pkbQueryInterface.uses(
        setUpTests.c2, setUpTests.stry)); // line doesnt use
    Assert::IsFalse(setUpTests.pkbQueryInterface.uses(
        setUpTests.c4, setUpTests.strr)); // incorrect var used
  }                                       // namespace UnitTesting

  TEST_METHOD(UsesLineNumberAndVariable) {

    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.c4, setUpTests.variable) ==
        STRING_SET{"x", "y"}); // on assign statement
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.c3, setUpTests.variable) ==
        STRING_SET{"x", "y"}); // on while statement
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.c7, setUpTests.variable) ==
        STRING_SET{"x", "y"}); // on call statement
  }                            // namespace UnitTesting

  TEST_METHOD(UsesLineNumberAndUnderscore) {

    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(
        setUpTests.c3, setUpTests.underscore)); // on while statement
    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(
        setUpTests.c7, setUpTests.underscore)); // on call statement
    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(
        setUpTests.c26, setUpTests.underscore)); // on print statement

    Assert::IsFalse(setUpTests.pkbQueryInterface.uses(
        setUpTests.c12, setUpTests.underscore)); // on read statement
  }                                              // namespace UnitTesting

  TEST_METHOD(UsesStmtAndString) {

    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.stmt, setUpTests.stry) ==
        STRING_SET{setUpTests.l3, setUpTests.l4, setUpTests.l7});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.stmt, setUpTests.strr) ==
        STRING_SET{setUpTests.l10});

    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.stmt, setUpTests.strm) ==
        STRING_SET{}); // setUpTests.string var not used

  } // namespace UnitTesting

  TEST_METHOD(UsesStmtAndVariable) {

    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.stmt,
                                          setUpTests.variable) ==
        STRING_PAIRS{
            STRING_VECTOR{
                setUpTests.l3,  setUpTests.l3,  setUpTests.l15, setUpTests.l15,
                setUpTests.l4,  setUpTests.l4,  setUpTests.l6,  setUpTests.l7,
                setUpTests.l7,  setUpTests.l10, setUpTests.l10, setUpTests.l11,
                setUpTests.l11, setUpTests.l11, setUpTests.l14, setUpTests.l14,
                setUpTests.l16, setUpTests.l17, setUpTests.l17, setUpTests.l21,
                setUpTests.l18, setUpTests.l20, setUpTests.l19, setUpTests.l22,
                setUpTests.l23, setUpTests.l24, setUpTests.l25, setUpTests.l26},
            STRING_VECTOR{"x", "y", "q", "t", "x", "y", "x", "y", "x", "x",
                          "r", "q", "t", "k", "q", "t", "t", "q", "t", "q",
                          "t", "q", "q", "t", "q", "q", "k", "k"}});
  } // namespace UnitTesting

  TEST_METHOD(UsesStmtAndUnderscore) {

    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(setUpTests.stmt,
                                                     setUpTests.underscore) ==
                   STRING_SET{setUpTests.l3, setUpTests.l4, setUpTests.l6,
                              setUpTests.l7, setUpTests.l10, setUpTests.l11,
                              setUpTests.l14, setUpTests.l15, setUpTests.l16,
                              setUpTests.l17, setUpTests.l18, setUpTests.l19,
                              setUpTests.l20, setUpTests.l21, setUpTests.l22,
                              setUpTests.l23, setUpTests.l24, setUpTests.l25,
                              setUpTests.l26});

  } // namespace UnitTesting

  TEST_METHOD(UsesStatementAndString) {

    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.a, setUpTests.strx) ==
        STRING_SET{setUpTests.l4, setUpTests.l10});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.w, setUpTests.strt) ==
        STRING_SET{setUpTests.l14, setUpTests.l17});
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.c, setUpTests.strx) ==
        STRING_SET{setUpTests.l7});

  } // namespace UnitTesting

  TEST_METHOD(UsesStatementAndVariable) {

    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.a, setUpTests.variable) ==
        STRING_PAIRS(STRING_VECTOR{setUpTests.l4, setUpTests.l4, setUpTests.l24,
                                   setUpTests.l10, setUpTests.l10,
                                   setUpTests.l18, setUpTests.l16,
                                   setUpTests.l20},
                     STRING_VECTOR{"x", "y", "q", "x", "r", "t", "t", "q"}));

  } // namespace UnitTesting

  TEST_METHOD(UsesStatementAndUnderscore) {

    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(setUpTests.w,
                                                     setUpTests.underscore) ==
                   STRING_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                              setUpTests.l23});

  } // namespace UnitTesting

  TEST_METHOD(UsesStringAndString) {

    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(
        setUpTests.strpmain, setUpTests.strx)); // proc directly using
    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(
        setUpTests.strpmain,
        setUpTests.strt)); // proc indirectly using through call
    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(
        setUpTests.strpmain,
        setUpTests.strk)); // proc indirectly using through nested call

    Assert::IsFalse(setUpTests.pkbQueryInterface.uses(
        setUpTests.strpaux, setUpTests.strr)); // proc doesn't use that var

  } // namespace UnitTesting

  TEST_METHOD(UsesStringAndVariable) {

    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(setUpTests.strpaux,
                                                     setUpTests.variable) ==
                   STRING_SET{"x", "y"}); // no calls, only vars used directly
    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.strpmain,
                                          setUpTests.variable) ==
        STRING_SET{"x", "y", "r", "q", "t",
                   "k"}); // proc main uses all vars directly & indirectly

  } // namespace UnitTesting

  TEST_METHOD(UsesStringAndUnderscore) {

    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(setUpTests.strpaux,
                                                     setUpTests.underscore));
    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(setUpTests.strpextra,
                                                     setUpTests.underscore));
    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(setUpTests.strpmain,
                                                     setUpTests.underscore));
    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(setUpTests.strpcomplicate,
                                                     setUpTests.underscore));

  } // namespace UnitTesting

  TEST_METHOD(UsesProcedureAndString) {

    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(setUpTests.procedure,
                                                     setUpTests.strx) ==
                   STRING_SET{"aux", "main"});
    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(setUpTests.procedure,
                                                     setUpTests.strk) ==
                   STRING_SET{"main", "complicate", "extra"});
  } // namespace UnitTesting

  TEST_METHOD(UsesProcedureAndVariable) {

    Assert::IsTrue(
        setUpTests.pkbQueryInterface.uses(setUpTests.procedure,
                                          setUpTests.variable) ==
        STRING_PAIRS(STRING_VECTOR{"aux", "aux", "main", "main", "main", "main",
                                   "main", "main", "complicate", "complicate",
                                   "complicate", "extra"},
                     STRING_VECTOR{"y", "x", "x", "r", "y", "q", "t", "k", "q",
                                   "t", "k", "k"}));
  } // namespace UnitTesting

  TEST_METHOD(UsesProcedureAndUnderscore) {

    Assert::IsTrue(setUpTests.pkbQueryInterface.uses(setUpTests.procedure,
                                                     setUpTests.underscore) ==
                   STRING_SET{"aux", "main", "complicate", "extra"});

  } // namespace UnitTesting
};

} // namespace UnitTesting
