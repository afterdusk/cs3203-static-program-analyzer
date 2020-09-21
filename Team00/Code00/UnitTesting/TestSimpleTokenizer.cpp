#include "CppUnitTest.h"
#include "stdafx.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

TEST_CLASS(TestSimpleTokenizer){public : TEST_METHOD(TestTokenize){
    std::string str =
        "if (!(x > 10)  \r\n  && (\t\t\tx %  5\n == 0)) {\n read x; y "
        "=  x         * 7;     ";
Tokenizer tokenizer(str);
std::vector<SimpleToken> output = tokenizer.tokenize();
std::vector<SimpleToken> correctResult;
correctResult.push_back(SimpleToken("if"));
correctResult.push_back(SimpleToken("("));
correctResult.push_back(SimpleToken("!"));
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
}
;
}
