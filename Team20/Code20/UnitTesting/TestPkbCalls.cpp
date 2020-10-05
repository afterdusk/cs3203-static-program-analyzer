#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestCallsQuery) {

public:
  Pkb pkb;
  SetUpTests setUpTests = SetUpTests::SetUpTests(pkb);

  TEST_METHOD(CallsStringAndString) {
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.strpmain,
                                                  setUpTests.strpcomplicate));
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.strpcomplicate,
                                                  setUpTests.strpextra));
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.strpextrafour,
                                                  setUpTests.strpextrathree));

    Assert::IsFalse(pkb.getQueryInterface()->calls(
        setUpTests.strpaux,
        setUpTests.strpmain)); // main calls aux instead of aux calling main
    Assert::IsFalse(pkb.getQueryInterface()->calls(
        setUpTests.strpmain,
        setUpTests.strpextra)); // main indirectly calls extra
  }                             // namespace UnitTesting

  TEST_METHOD(CallsStringAndProcedure) {
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.strpmain,
                                                  setUpTests.procedure) ==
                   NAME_SET{"aux", "complicate"});
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.strpcomplicate,
                                                  setUpTests.procedure) ==
                   NAME_SET{"extra"});
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.strpextrafour,
                                                  setUpTests.procedure) ==
                   NAME_SET{"extrathree", "main"});

    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.strpextra,
                                                  setUpTests.procedure) ==
                   NAME_SET()); // no calls within procedure extra
  }                             // namespace UnitTesting

  TEST_METHOD(CallsStringAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.strpmain,
                                                  setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.strpcomplicate,
                                                  setUpTests.underscore));

    Assert::IsFalse(pkb.getQueryInterface()->calls(
        setUpTests.strpaux,
        setUpTests.underscore)); // no calls within procedure aux
  }                              // namespace UnitTesting

  TEST_METHOD(CallsProcedureAndString) {
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.procedure,
                                                  setUpTests.strpmain) ==
                   NAME_SET{"extrathree", "extrafour"});
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.procedure,
                                                  setUpTests.strpcomplicate) ==
                   NAME_SET{"main"});
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.procedure,
                                                  setUpTests.strpextra) ==
                   NAME_SET{"complicate", "extratwo"});

    Assert::IsTrue(
        pkb.getQueryInterface()->calls(setUpTests.procedure,
                                       setUpTests.strpextrafour) ==
        NAME_SET()); // procedure extrafour not called by other procedures

  } // namespace UnitTesting

  TEST_METHOD(CallsProcedureAndProcedure) {
    Assert::IsTrue(
        pkb.getQueryInterface()->calls(setUpTests.procedure,
                                       setUpTests.procedure) ==
        NAME_NAME_PAIRS(
            NAME_VECTOR{"main", "main", "complicate", "extratwo", "extratwo",
                        "extrathree", "extrathree", "extrafour", "extrafour"},
            NAME_VECTOR{"aux", "complicate", "extra", "extra", "aux", "main",
                        "extratwo", "extrathree", "main"}));
  } // namespace UnitTesting

  TEST_METHOD(CallsProcedureAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->calls(setUpTests.procedure,
                                       setUpTests.underscore) ==
        NAME_SET{"main", "complicate", "extratwo", "extrathree", "extrafour"});
  } // namespace UnitTesting

  TEST_METHOD(CallsUnderscoreAndString) {
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.underscore,
                                                  setUpTests.strpextra));
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.underscore,
                                                  setUpTests.strpcomplicate));

    Assert::IsFalse(pkb.getQueryInterface()->calls(
        setUpTests.underscore,
        setUpTests
            .strpextrafour)); // extrafour is not called by any other procedures
  }                           // namespace UnitTesting

  TEST_METHOD(CallsUnderscoreAndProcedure) {
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.underscore,
                                                  setUpTests.procedure) ==
                   NAME_SET{"aux", "complicate", "extra", "extratwo",
                            "extrathree", "main"});
  } // namespace UnitTesting

  TEST_METHOD(CallsUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->calls(setUpTests.underscore,
                                                  setUpTests.underscore));
  } // namespace UnitTesting

  /*
   * CallsStar query tests
   */
  TEST_METHOD(CallsStarStringAndString) {
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(
        setUpTests.strpmain,
        setUpTests.strpcomplicate)); // direct call
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(
        setUpTests.strpmain,
        setUpTests.strpextra)); // indirect call
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(
        setUpTests.strpextrafour,
        setUpTests.strpaux)); // indirect call

    Assert::IsFalse(pkb.getQueryInterface()->callsStar(
        setUpTests.strpaux,
        setUpTests.strpextra)); // aux doesnt transitively call extra
  }

  TEST_METHOD(CallsStarStringAndProcedure) {
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(setUpTests.strpmain,
                                                      setUpTests.procedure) ==
                   NAME_SET{"aux", "complicate", "extra"});
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(setUpTests.strpextrafour,
                                                      setUpTests.procedure) ==
                   NAME_SET{"extrathree", "main", "extratwo", "extra", "aux",
                            "complicate"});
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(setUpTests.strpaux,
                                                      setUpTests.procedure) ==
                   NAME_SET()); // aux doesnt have any calls
  }                             // namespace UnitTesting

  TEST_METHOD(CallsStarStringAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(setUpTests.strpmain,
                                                      setUpTests.underscore));
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(setUpTests.strpcomplicate,
                                                      setUpTests.underscore));

    Assert::IsFalse(pkb.getQueryInterface()->callsStar(
        setUpTests.strpaux,
        setUpTests.underscore)); // no calls within procedure aux
  }                              // namespace UnitTesting

  TEST_METHOD(CallsStarProcedureAndString) {
    Assert::IsTrue(
        pkb.getQueryInterface()->callsStar(setUpTests.procedure,
                                           setUpTests.strpextra) ==
        NAME_SET{"main", "complicate", "extratwo", "extrathree", "extrafour"});
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(setUpTests.procedure,
                                                      setUpTests.strpaux) ==
                   NAME_SET{"main", "extratwo", "extrathree", "extrafour"});

    Assert::IsTrue(
        pkb.getQueryInterface()->callsStar(setUpTests.procedure,
                                           setUpTests.strpextrafour) ==
        NAME_SET()); // extrafour not called by any procedures
  }                  // namespace UnitTesting

  TEST_METHOD(CallsStarProcedureAndProcedure) {
    Assert::IsTrue(
        pkb.getQueryInterface()->callsStar(setUpTests.procedure,
                                           setUpTests.procedure) ==
        NAME_NAME_PAIRS(
            NAME_VECTOR{"main", "main", "main", "complicate", "extratwo",
                        "extratwo", "extrathree", "extrathree", "extrathree",
                        "extrathree", "extrathree", "extrafour", "extrafour",
                        "extrafour", "extrafour", "extrafour", "extrafour"},
            NAME_VECTOR{"aux", "extra", "complicate", "extra", "extra", "aux",
                        "main", "extratwo", "extra", "aux", "complicate",
                        "extrathree", "main", "extratwo", "aux", "extra",
                        "complicate"}));
  } // namespace UnitTesting

  TEST_METHOD(CallsStarProcedureAndUnderscore) {
    Assert::IsTrue(
        pkb.getQueryInterface()->callsStar(setUpTests.procedure,
                                           setUpTests.underscore) ==
        NAME_SET{"main", "complicate", "extratwo", "extrathree", "extrafour"});
  } // namespace UnitTesting

  TEST_METHOD(CallsStarUnderscoreAndString) {
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(setUpTests.underscore,
                                                      setUpTests.strpextra));
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(
        setUpTests.underscore, setUpTests.strpcomplicate));

    Assert::IsFalse(pkb.getQueryInterface()->callsStar(
        setUpTests.underscore,
        setUpTests
            .strpextrafour)); // extrafour is not called by any other procedures
  }                           // namespace UnitTesting

  TEST_METHOD(CallsStarUnderscoreAndProcedure) {
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(setUpTests.underscore,
                                                      setUpTests.procedure) ==
                   NAME_SET{"aux", "complicate", "extra", "extratwo",
                            "extrathree", "main"});
  } // namespace UnitTesting

  TEST_METHOD(CallsStarUnderscoreAndUnderscore) {
    Assert::IsTrue(pkb.getQueryInterface()->callsStar(setUpTests.underscore,
                                                      setUpTests.underscore));
  } // namespace UnitTesting
};

} // namespace UnitTesting
