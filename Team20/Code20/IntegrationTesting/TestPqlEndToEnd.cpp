#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
TEST_CLASS(TestPqlEndToEnd) {
public:
  Pkb pkb;
  SetUpTests setUpTests =
      SetUpTests::SetUpTests(pkb, SetUpTests::TestNumber::A);

  TEST_METHOD(TestPqlEndToEnd_TupleSelectWithFollows) {
    const auto query =
        "assign a; read r; call c; Select <c, a> such that Follows(r, a)";
    std::list<std::string> actual;
    Pql::evaluate(Pql::parse(Pql::lex(query)), pkb.getQueryInterface(), actual);
    std::list<std::string> expected = {"31 10", "7 10",  "27 10", "11 10",
                                       "25 10", "28 10", "29 10", "30 10",
                                       "32 10", "33 10"};
    Assert::IsTrue(actual == expected);
  }
  TEST_METHOD(TestPqlEndToEnd_BooleanSelectWithFollows) {
    const auto query =
        "assign a; read r; call c; Select BOOLEAN such that Follows(r, a)";
    std::list<std::string> actual;
    Pql::evaluate(Pql::parse(Pql::lex(query)), pkb.getQueryInterface(), actual);
    std::list<std::string> expected = {"TRUE"};
    Assert::IsTrue(actual == expected);
  }
};
} // namespace IntegrationTesting
