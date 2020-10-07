#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestModifiesQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests =
      SetUpTests::SetUpTests(pkb, SetUpTests::TestNumber::A);

  TEST_METHOD(ModifiesLineNumberAndString) {

    Assert::IsTrue(pkb.getQueryInterface()->modifies(
        setUpTests.c4, setUpTests.stry)); // assign statement
    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.c5,
                                          setUpTests.stry)); // read statement

    Assert::IsTrue(pkb.getQueryInterface()->modifies(
        setUpTests.c11, setUpTests.strq)); // modifies in call procedure

    Assert::IsFalse(pkb.getQueryInterface()->modifies(
        setUpTests.c11,
        setUpTests.stry)); // call procedure doesnt modifies input var
    Assert::IsFalse(pkb.getQueryInterface()->modifies(
        setUpTests.c6, setUpTests.strx)); // print doesn't modify var
    Assert::IsFalse(pkb.getQueryInterface()->modifies(
        setUpTests.c4, setUpTests.strr)); // incorrect var modified
  }                                       // namespace UnitTesting

  TEST_METHOD(ModifiesLineNumberAndVariable) {

    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.c4, setUpTests.variable) ==
        NAME_SET{"y"}); // on assign statement
    Assert::IsTrue(pkb.getQueryInterface()->modifies(setUpTests.c14,
                                                     setUpTests.variable) ==
                   NAME_SET{"q", "t"}); // on while statement
    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.c7, setUpTests.variable) ==
        NAME_SET{"x", "y"}); // on call statement
  }                          // namespace UnitTesting

  TEST_METHOD(ModifiesLineNumberAndUnderscore) {

    Assert::IsTrue(pkb.getQueryInterface()->modifies(
        setUpTests.c3, setUpTests.underscore)); // on while statement
    Assert::IsTrue(pkb.getQueryInterface()->modifies(
        setUpTests.c7, setUpTests.underscore)); // on call statement
    Assert::IsTrue(pkb.getQueryInterface()->modifies(
        setUpTests.c12, setUpTests.underscore)); // on read statement

    Assert::IsFalse(pkb.getQueryInterface()->modifies(
        setUpTests.c26, setUpTests.underscore)); // on print statement
  }                                              // namespace UnitTesting

  TEST_METHOD(ModifiesStmtAndString) {

    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.stmt, setUpTests.strm) ==
        LINE_SET{setUpTests.l10, setUpTests.l29, setUpTests.l31, setUpTests.l32,
                 setUpTests.l33});
    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.stmt, setUpTests.strx) ==
        LINE_SET{setUpTests.l1, setUpTests.l7, setUpTests.l8, setUpTests.l28,
                 setUpTests.l29, setUpTests.l30, setUpTests.l31, setUpTests.l32,
                 setUpTests.l33});

    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.stmt, setUpTests.strk) ==
        LINE_SET{}); // string var not modified
  }                  // namespace UnitTesting

  TEST_METHOD(ModifiesStmtAndVariable) {

    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.stmt,
                                          setUpTests.variable) ==
        LINE_NAME_PAIRS{
            LINE_VECTOR{
                setUpTests.l9,  setUpTests.l1,  setUpTests.l2,  setUpTests.l3,
                setUpTests.l4,  setUpTests.l5,  setUpTests.l7,  setUpTests.l7,
                setUpTests.l8,  setUpTests.l10, setUpTests.l11, setUpTests.l11,
                setUpTests.l12, setUpTests.l13, setUpTests.l14, setUpTests.l14,
                setUpTests.l15, setUpTests.l15, setUpTests.l16, setUpTests.l17,
                setUpTests.l18, setUpTests.l19, setUpTests.l20, setUpTests.l23,
                setUpTests.l24, setUpTests.l28, setUpTests.l28, setUpTests.l29,
                setUpTests.l29, setUpTests.l29, setUpTests.l29, setUpTests.l29,
                setUpTests.l29, setUpTests.l30, setUpTests.l30, setUpTests.l31,
                setUpTests.l31, setUpTests.l31, setUpTests.l31, setUpTests.l31,
                setUpTests.l31, setUpTests.l32, setUpTests.l32, setUpTests.l32,
                setUpTests.l32, setUpTests.l32, setUpTests.l32, setUpTests.l33,
                setUpTests.l33, setUpTests.l33, setUpTests.l33, setUpTests.l33,
                setUpTests.l33},
            NAME_VECTOR{"r", "x", "y", "y", "y", "y", "y", "x", "x", "m", "q",
                        "t", "q", "t", "q", "t", "q", "t", "t", "t", "t", "q",
                        "q", "q", "q", "y", "x", "x", "r", "m", "y", "q", "t",
                        "y", "x", "y", "q", "x", "r", "m", "t", "x", "r", "m",
                        "y", "q", "t", "y", "q", "x", "r", "m", "t"}});

  } // namespace UnitTesting

  TEST_METHOD(ModifiesStmtAndUnderscore) {

    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.stmt,
                                          setUpTests.underscore) ==
        LINE_SET{setUpTests.l1,  setUpTests.l2,  setUpTests.l3,  setUpTests.l4,
                 setUpTests.l5,  setUpTests.l7,  setUpTests.l8,  setUpTests.l9,
                 setUpTests.l10, setUpTests.l11, setUpTests.l12, setUpTests.l13,
                 setUpTests.l14, setUpTests.l15, setUpTests.l16, setUpTests.l17,
                 setUpTests.l18, setUpTests.l19, setUpTests.l20, setUpTests.l23,
                 setUpTests.l24, setUpTests.l28, setUpTests.l29, setUpTests.l30,
                 setUpTests.l31, setUpTests.l32, setUpTests.l33});
  } // namespace UnitTesting

  TEST_METHOD(ModifiesStatementAndString) {

    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.a, setUpTests.strx) ==
        LINE_SET{}); // no assignment modifies var x
    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.w, setUpTests.strt) ==
        LINE_SET{setUpTests.l14, setUpTests.l17});
    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.c, setUpTests.strx) ==
        LINE_SET{setUpTests.l7, setUpTests.l28, setUpTests.l29, setUpTests.l30,
                 setUpTests.l31, setUpTests.l32, setUpTests.l33});
  } // namespace UnitTesting

  TEST_METHOD(ModifiesStatementAndVariable) {

    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.a, setUpTests.variable) ==
        LINE_NAME_PAIRS(LINE_VECTOR{setUpTests.l4, setUpTests.l20,
                                    setUpTests.l10, setUpTests.l18,
                                    setUpTests.l16, setUpTests.l24},
                        NAME_VECTOR{"y", "q", "m", "t", "t", "q"}));
  } // namespace UnitTesting

  TEST_METHOD(ModifiesStatementAndUnderscore) {

    Assert::IsTrue(pkb.getQueryInterface()->modifies(setUpTests.w,
                                                     setUpTests.underscore) ==
                   LINE_SET{setUpTests.l3, setUpTests.l14, setUpTests.l17,
                            setUpTests.l23});
    Assert::IsTrue(pkb.getQueryInterface()->modifies(setUpTests.c,
                                                     setUpTests.underscore) ==
                   LINE_SET{setUpTests.l7, setUpTests.l11, setUpTests.l28,
                            setUpTests.l29, setUpTests.l30, setUpTests.l31,
                            setUpTests.l32, setUpTests.l33});

  } // namespace UnitTesting

  TEST_METHOD(ModifiesStringAndString) {

    Assert::IsTrue(pkb.getQueryInterface()->modifies(
        setUpTests.strpmain, setUpTests.strx)); // proc directly modifies
    Assert::IsTrue(pkb.getQueryInterface()->modifies(
        setUpTests.strpmain,
        setUpTests.strt)); // proc indirectly modify through call

    Assert::IsFalse(pkb.getQueryInterface()->modifies(
        setUpTests.strpmain, setUpTests.strk)); // proc does not indirectly
                                                // modify through nested call
    Assert::IsFalse(pkb.getQueryInterface()->modifies(
        setUpTests.strpextra, setUpTests.strk)); // proc doesn't modify
  }                                              // namespace UnitTesting

  TEST_METHOD(ModifiesStringAndVariable) {

    Assert::IsTrue(pkb.getQueryInterface()->modifies(setUpTests.strpaux,
                                                     setUpTests.variable) ==
                   NAME_SET{"x", "y"}); // no calls, only vars modified directly
    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.strpmain,
                                          setUpTests.variable) ==
        NAME_SET{"x", "y", "r", "q", "t",
                 "m"}); // proc main modifies all vars directly & indirectly
  }                     // namespace UnitTesting

  TEST_METHOD(ModifiesStringAndUnderscore) {

    Assert::IsTrue(pkb.getQueryInterface()->modifies(setUpTests.strpaux,
                                                     setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->modifies(setUpTests.strpmain,
                                                     setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->modifies(setUpTests.strpcomplicate,
                                                     setUpTests.underscore));

    Assert::IsFalse(pkb.getQueryInterface()->modifies(
        setUpTests.strpextra,
        setUpTests.underscore)); // proc does not modify any vars
  }                              // namespace UnitTesting

  TEST_METHOD(ModifiesProcedureAndString) {

    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.procedure,
                                          setUpTests.strx) ==
        NAME_SET{"aux", "main", "extratwo", "extrathree", "extrafour"});
    Assert::IsTrue(pkb.getQueryInterface()->modifies(setUpTests.procedure,
                                                     setUpTests.strq) ==
                   NAME_SET{"main", "complicate", "extrathree", "extrafour"});
  } // namespace UnitTesting

  TEST_METHOD(ModifiesProcedureAndVariable) {

    Assert::IsTrue(
        pkb.getQueryInterface()->modifies(setUpTests.procedure,
                                          setUpTests.variable) ==
        NAME_NAME_PAIRS(
            NAME_VECTOR{"extrafour",  "extrafour",  "extrafour",  "extrafour",
                        "extrafour",  "extrafour",  "aux",        "aux",
                        "main",       "main",       "main",       "main",
                        "main",       "main",       "complicate", "complicate",
                        "extratwo",   "extratwo",   "extrathree", "extrathree",
                        "extrathree", "extrathree", "extrathree", "extrathree"},
            NAME_VECTOR{"y", "q", "x", "r", "m", "t", "y", "x",
                        "x", "r", "m", "y", "q", "t", "q", "t",
                        "y", "x", "y", "q", "x", "r", "m", "t"}));
  } // namespace UnitTesting

  TEST_METHOD(ModifiesProcedureAndUnderscore) {

    Assert::IsTrue(pkb.getQueryInterface()->modifies(setUpTests.procedure,
                                                     setUpTests.underscore) ==
                   NAME_SET{"aux", "main", "complicate", "extratwo",
                            "extrathree", "extrafour"});
  } // namespace UnitTesting
};

} // namespace UnitTesting
