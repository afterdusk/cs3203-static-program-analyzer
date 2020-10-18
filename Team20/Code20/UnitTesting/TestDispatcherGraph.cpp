
#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestDispatcherGraph){
  public : TEST_METHOD(TestDispatcherGraph_AddDispatcher){
      SetUpDispatcherGraphTest setUpTests =
          SetUpDispatcherGraphTest::SetUpDispatcherGraphTest();
DispatcherGraph graph;
Assert::IsFalse(graph.contains(setUpTests.mergingCd));
for (auto const &symbol : setUpTests.mergingCd->getSymbols()) {
  Assert::IsFalse(graph.contains(symbol));
}
graph.addDispatcher(setUpTests.mergingCd);
Assert::IsTrue(setUpTests.mergingCd);
for (auto const &symbol : setUpTests.mergingCd->getSymbols()) {
  Assert::IsTrue(graph.contains(symbol));
}
} // namespace UnitTesting
TEST_METHOD(TestDispatcherGraph_AddDispatcherAlreadyExists_ThrowsException) {
  SetUpDispatcherGraphTest setUpTests =
      SetUpDispatcherGraphTest::SetUpDispatcherGraphTest();

  const auto clauseDispatcher = setUpTests.mergingCd;
  Assert::ExpectException<const char *>([clauseDispatcher] {
    DispatcherGraph dg;
    dg.addDispatcher(clauseDispatcher);
    dg.addDispatcher(clauseDispatcher);
  });
}
TEST_METHOD(TestDispatcherGraph_MergeWillMergeTwoGraphs) {
  SetUpDispatcherGraphTest setUpTests =
      SetUpDispatcherGraphTest::SetUpDispatcherGraphTest();
  setUpTests.dg1.merge(setUpTests.dg2, setUpTests.mergingCd);

  Assert::IsTrue(setUpTests.dg1 == setUpTests.dg3);
}
TEST_METHOD(
    TestDispatcherGraph_MergeTwoGraphsWithCommonSynonym_ThrowsException) {

  Assert::ExpectException<const char *>([] {
    SetUpDispatcherGraphTest setUpTests =
        SetUpDispatcherGraphTest::SetUpDispatcherGraphTest();
    DispatcherGraph initialGraph = setUpTests.dg1;
    initialGraph.addDispatcher(setUpTests.mergingCd);
    initialGraph.merge(setUpTests.dg2, setUpTests.anotherMergingCd);
  });
}

TEST_METHOD(
    TestDispatcherGraph_MergeTwoGraphsWithDispatcherNoCommonSynonym_ThrowsException) {

  Assert::ExpectException<const char *>([] {
    SetUpDispatcherGraphTest setUpTests =
        SetUpDispatcherGraphTest::SetUpDispatcherGraphTest();
    setUpTests.dg1.merge(setUpTests.dg2, setUpTests.noCommonSymbolsWithDg1Cd);
  });
}
}
;
}
; // namespace UnitTesting
