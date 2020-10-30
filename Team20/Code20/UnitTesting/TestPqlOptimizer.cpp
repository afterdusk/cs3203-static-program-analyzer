#include "CppUnitTest.h"
#include "stdafx.h"
#include <iostream>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestPqlOptimizer){public : TEST_METHOD(
    TestOptimizer_RewriteRelationshipWithSpecifiedStatementNumber){
    ParsedQuery input = {
        DECLARATIONS{
            {"w", TokenType::WHILE},
            {"s", TokenType::STMT},
        },
        RESULTS{PqlResultType::Tuple, {{"w", AttributeRefType::NONE}}},
        RELATIONSHIPS{
            ParsedRelationship{
                TokenType::PARENT,
                {TokenType::WHILE, "w"},
                {TokenType::STMT, "s"},
            },
        },
        PATTERNS{},
        WITHS{
            {
                Reference{Element{"w", AttributeRefType::STATEMENT_NUM}},
                Reference{PqlToken{TokenType::NUMBER, "1"}},
            },
        },
    };
auto actual = Pql::optimize(input);
ParsedQuery expected = {
    DECLARATIONS{{"w", TokenType::WHILE}, {"s", TokenType::STMT}},
    RESULTS{PqlResultType::Tuple, {{"w", AttributeRefType::NONE}}},
    RELATIONSHIPS{ParsedRelationship{
        TokenType::PARENT, {TokenType::NUMBER, "1"}, {TokenType::STMT, "s"}}},
    PATTERNS{},
    WITHS{
        {Reference{Element{"w", AttributeRefType::STATEMENT_NUM}},
         Reference{PqlToken{TokenType::NUMBER, "1"}}},
    },
};
Assert::IsTrue(expected == actual);
} // namespace UnitTesting
TEST_METHOD(TestOptimizer_RewriteRelationshipInvalidRawValue) {
  // NOTE: This test cases tests what should be the only impossible rewrite
  ParsedRelationship toTest = {
      TokenType::NEXT,
      {TokenType::CALL, "ca"},
      {TokenType::STMT, "s"},
  };
  ParsedQuery input = {
      DECLARATIONS{
          {"ca", TokenType::CALL},
          {"s", TokenType::STMT},
      },
      RESULTS{PqlResultType::Tuple, {{"ca", AttributeRefType::NONE}}},
      RELATIONSHIPS{toTest},
      PATTERNS{},
      WITHS{
          {
              Reference{Element{"ca", AttributeRefType::PROCNAME}},
              Reference{PqlToken{TokenType::STRING, "hello"}},
          },
      },
  };
  auto actual = Pql::optimize(input);
  Assert::IsTrue(actual.relationships == RELATIONSHIPS{toTest});
} // namespace UnitTesting
TEST_METHOD(TestOptimizer_RemoveRedundantWithsPositiveInput_RemoveWiths) {
  ParsedQuery input = {
      DECLARATIONS{
          {"w", TokenType::WHILE},
          {"s", TokenType::STMT},
      },
      RESULTS{
          PqlResultType::Tuple,
          {
              {"w", AttributeRefType::NONE},
          },
      },
      RELATIONSHIPS{
          ParsedRelationship{
              TokenType::PARENT,
              {TokenType::WHILE, "w"},
              {TokenType::STMT, "s"},
          },
      },
      PATTERNS{},
      WITHS{
          {
              Reference{PqlToken{TokenType::NUMBER, "1"}},
              Reference{PqlToken{TokenType::NUMBER, "1"}},
          },
          {
              Reference{PqlToken{TokenType::STRING, "2"}},
              Reference{PqlToken{TokenType::STRING, "2"}},
          },
      },
  };
  auto actual = Pql::optimize(input);
  ParsedQuery expected = {
      DECLARATIONS{{"w", TokenType::WHILE}, {"s", TokenType::STMT}},
      RESULTS{
          PqlResultType::Tuple,
          {
              {"w", AttributeRefType::NONE},
          },
      },
      RELATIONSHIPS{
          ParsedRelationship{
              TokenType::PARENT,
              {TokenType::WHILE, "w"},
              {TokenType::STMT, "s"},
          },
      },
      PATTERNS{},
      WITHS{},
  };
  Assert::IsTrue(expected.declarations == actual.declarations);
  Assert::IsTrue(expected.relationships == actual.relationships);
  Assert::IsTrue(expected.patterns == actual.patterns);
  Assert::IsTrue(expected.withs == actual.withs);
  Assert::IsTrue(expected == actual);
} // namespace UnitTesting
public:
TEST_METHOD(TestOptimizer_RemoveRedundantWithsNegativeInput_ThrowsException) {
  ParsedQuery input = {
      DECLARATIONS{{"w", TokenType::WHILE}, {"s", TokenType::STMT}},
      RESULTS{PqlResultType::Tuple, {{"w", AttributeRefType::NONE}}},
      RELATIONSHIPS{ParsedRelationship{
          TokenType::PARENT, {TokenType::WHILE, "w"}, {TokenType::STMT, "s"}}},
      PATTERNS{},
      WITHS{
          {Reference{PqlToken{TokenType::NUMBER, "1"}},
           Reference{PqlToken{TokenType::NUMBER, "2"}}},
      },
  };
  Assert::ExpectException<const char *>([input] { Pql::optimize(input); });
} // namespace UnitTesting
public:
TEST_METHOD(TestOptimizer_DeleteDuplicateClauses) {
  ParsedQuery input = {
      DECLARATIONS{
          {"a", TokenType::ASSIGN},
          {"a1", TokenType::ASSIGN},
          {"a2", TokenType::ASSIGN},
      },
      RESULTS{
          PqlResultType::Tuple,
          {
              {"w", AttributeRefType::NONE},
          },
      },
      RELATIONSHIPS{
          ParsedRelationship{
              TokenType::PARENT,
              {TokenType::WHILE, "w"},
              {TokenType::STMT, "s"},
          },
          ParsedRelationship{
              TokenType::PARENT,
              {TokenType::WHILE, "w"},
              {TokenType::STMT, "s"},
          },
      },
      PATTERNS{
          ParsedPattern{
              PqlToken{TokenType::AFFECTS, "a"},
              PqlToken{TokenType::UNDERSCORE},
              PatternSpec{PatternMatchType::Any},
          },
          ParsedPattern{
              PqlToken{TokenType::AFFECTS, "a"},
              PqlToken{TokenType::UNDERSCORE},
              PatternSpec{PatternMatchType::Any},
          },

      },
      WITHS{
          {Reference{Element{"w", AttributeRefType::STATEMENT_NUM}},
           Reference{PqlToken{TokenType::NUMBER, "1"}}},
          {Reference{Element{"w", AttributeRefType::STATEMENT_NUM}},
           Reference{PqlToken{TokenType::NUMBER, "1"}}},
      },
  };
  ParsedQuery expected = {
      DECLARATIONS{
          {"a", TokenType::ASSIGN},
          {"a1", TokenType::ASSIGN},
          {"a2", TokenType::ASSIGN},
      },
      RESULTS{PqlResultType::Tuple, {{"a", AttributeRefType::NONE}}},
      RELATIONSHIPS{ParsedRelationship{
          TokenType::PARENT, {TokenType::NUMBER, "1"}, {TokenType::STMT, "s"}}},
      PATTERNS{
          ParsedPattern{
              PqlToken{TokenType::AFFECTS, "a"},
              PqlToken{TokenType::UNDERSCORE},
              PatternSpec{PatternMatchType::Any},
          },
      },
      WITHS{
          {
              Reference{Element{"w", AttributeRefType::STATEMENT_NUM}},
              Reference{PqlToken{TokenType::NUMBER, "1"}},
          },
      },
  };
  auto actual = Pql::optimize(input);
  Assert::IsTrue(expected.declarations == actual.declarations);
  Assert::IsTrue(expected.relationships == actual.relationships);
  Assert::IsTrue(expected.patterns == actual.patterns);
  Assert::IsTrue(expected.withs == actual.withs);
} // namespace UnitTesting

TEST_METHOD(TestOptimizer_IdentifyImpossibleWiths_ThrowsException) {
  ParsedQuery input = {
      DECLARATIONS{
          {"w", TokenType::WHILE},
          {"s", TokenType::STMT},
          {"a", TokenType::ASSIGN},
      },
      RESULTS{PqlResultType::Tuple, {{"w", AttributeRefType::NONE}}},
      RELATIONSHIPS{ParsedRelationship{
          TokenType::PARENT,
          {TokenType::WHILE, "w"},
          {TokenType::STMT, "s"},
      }},
      PATTERNS{},
      WITHS{
          {
              Reference{Element{"w", AttributeRefType::STATEMENT_NUM}},
              Reference{Element{"a", AttributeRefType::STATEMENT_NUM}},
          },
      },
  };
  Assert::ExpectException<const char *>([input] { Pql::optimize(input); });
} // namespace UnitTesting
TEST_METHOD(
    TestOptimizer_IdentifyImpossibleWithsCompareStatmentWithAnEntity_NoChange) {
  ParsedQuery input = {
      DECLARATIONS{
          {"w", TokenType::WHILE},
          {"s", TokenType::STMT},
          {"a", TokenType::ASSIGN},
      },
      RESULTS{
          PqlResultType::Tuple,
          {{"w", AttributeRefType::NONE}},
      },
      RELATIONSHIPS{ParsedRelationship{
          TokenType::PARENT,
          {TokenType::WHILE, "w"},
          {TokenType::STMT, "s"},
      }},
      PATTERNS{},
      WITHS{
          {
              Reference{Element{"w", AttributeRefType::STATEMENT_NUM}},
              Reference{Element{"s", AttributeRefType::STATEMENT_NUM}},
          },
      },
  };
  auto actual = Pql::optimize(input);
  Assert::IsTrue(input.declarations == actual.declarations);
  Assert::IsTrue(input.relationships == actual.relationships);
  Assert::IsTrue(input.patterns == actual.patterns);
  Assert::IsTrue(input.withs == actual.withs);
}
}
;
}
