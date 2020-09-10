#include "CppUnitTest.h"
#include "stdafx.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestTokenizer){public : TEST_METHOD(TestTokenize){
    std::string str =
        "if (!(x > 10)  \r\n  && (\t\t\tx %  5\n == 0)) {\n read x; y "
        "=  x         * 7;     ";
// std::string str = "p";
Tokenizer tokenizer(str);
std::vector<Token> output = tokenizer.tokenize();
std::vector<Token> correctResult;
correctResult.push_back(Token("if"));
correctResult.push_back(Token("("));
correctResult.push_back(Token("!"));
correctResult.push_back(Token("("));
correctResult.push_back(Token("x"));
correctResult.push_back(Token(">"));
correctResult.push_back(Token("10"));
correctResult.push_back(Token(")"));
correctResult.push_back(Token("&&"));
correctResult.push_back(Token("("));
correctResult.push_back(Token("x"));
correctResult.push_back(Token("%"));
correctResult.push_back(Token("5"));
correctResult.push_back(Token("=="));
correctResult.push_back(Token("0"));
correctResult.push_back(Token(")"));
correctResult.push_back(Token(")"));
correctResult.push_back(Token("{"));
correctResult.push_back(Token("read"));
correctResult.push_back(Token("x"));
correctResult.push_back(Token(";"));
correctResult.push_back(Token("y"));
correctResult.push_back(Token("="));
correctResult.push_back(Token("x"));
correctResult.push_back(Token("*"));
correctResult.push_back(Token("7"));
correctResult.push_back(Token(";"));
Assert::IsTrue(output == correctResult);
} // namespace UnitTesting
}
;
}
