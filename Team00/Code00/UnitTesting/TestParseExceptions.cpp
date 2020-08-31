#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestParseExceptions){public : TEST_METHOD(TestNoProcedureException){
    NoProcedureException exception = NoProcedureException(10, "procedure1");
Assert::IsTrue(
    strcmp(exception.what(),
           "Error: Line 10: The procedure \"procedure1\" does not exist.") ==
    0);
} // namespace UnitTesting

TEST_METHOD(TestNoParenthesisException) {
  NoParenthesisException exception =
      NoParenthesisException(2, "if x == 0 {", '(');
  Assert::IsTrue(strcmp(exception.what(),
                        "Error: Line 2: There is a parenthesis '(' "
                        "missing on the line \"if x == 0 {\".") == 0);
}

TEST_METHOD(TestNoCurlyBracketException) {
  NoCurlyBracketException exception = NoCurlyBracketException(3, "x = 1;", '}');
  Assert::IsTrue(strcmp(exception.what(),
                        "Error: Line 3: There is a curly bracket '}' "
                        "missing on the line \"x = 1;\".") == 0);
}

TEST_METHOD(TestNoSemiColonException) {
  NoSemiColonException exception = NoSemiColonException(4, "call procedure1");
  Assert::IsTrue(strcmp(exception.what(),
                        "Error: Line 4: Semicolon is missing "
                        "on the line \"call procedure1\".") == 0);
}
}
;
}
