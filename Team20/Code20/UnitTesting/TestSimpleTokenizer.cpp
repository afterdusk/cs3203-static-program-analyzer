#include "CppUnitTest.h"
#include "stdafx.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestSimpleTokenizer){public : TEST_METHOD(TestTokenize){
    std::string str = "if (\r\n(x>10)&&(\t\t\tx %  5\n == 0)) {\n read x; y "
                      "=  x         * 7;     ";
Tokenizer tokenizer(str);
std::vector<SimpleToken> output = tokenizer.tokenize();
std::vector<SimpleToken> correctResult;
correctResult.push_back(SimpleToken("if"));
correctResult.push_back(SimpleToken("("));
correctResult.push_back(SimpleToken("("));
correctResult.push_back(SimpleToken("x"));
correctResult.push_back(SimpleToken(">"));
correctResult.push_back(SimpleToken("10"));
correctResult.push_back(SimpleToken(")"));
correctResult.push_back(SimpleToken("&&"));
correctResult.push_back(SimpleToken("("));
correctResult.push_back(SimpleToken("x"));
correctResult.push_back(SimpleToken("%"));
correctResult.push_back(SimpleToken("5"));
correctResult.push_back(SimpleToken("=="));
correctResult.push_back(SimpleToken("0"));
correctResult.push_back(SimpleToken(")"));
correctResult.push_back(SimpleToken(")"));
correctResult.push_back(SimpleToken("{"));
correctResult.push_back(SimpleToken("read"));
correctResult.push_back(SimpleToken("x"));
correctResult.push_back(SimpleToken(";"));
correctResult.push_back(SimpleToken("y"));
correctResult.push_back(SimpleToken("="));
correctResult.push_back(SimpleToken("x"));
correctResult.push_back(SimpleToken("*"));
correctResult.push_back(SimpleToken("7"));
correctResult.push_back(SimpleToken(";"));
Assert::IsTrue(output == correctResult);
} // namespace UnitTesting

TEST_METHOD(TestInvalidProgram) {
  std::string str(
      "procedure proc{ 10x = 1; if(a>=@#) then { read a;} else{ read b;}");
  Tokenizer tokenizer(str);
  try {
    tokenizer.tokenize();
    Assert::Fail();
  } catch (IllegalExpressionException &i) {
    Assert::IsTrue(
        std::strcmp(i.what(),
                    "Error: There is illegal expression(s) \"10x\", \"@#\". "
                    "Please check syntax grammar for SIMPLE program.") == 0);
  } catch (std::exception &e) {
    ignore(e);
    Assert::Fail();
  }
}
}
;
}
