#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestFollowsQuery) {
  /* Example
    procedure aux {
    1	read x;
    2	read y;
    3	while (y != 0) {
    4		y = y / x;
    5		read y; }
    6	print x;}
    procedure main {
    7	call aux;
    8	read x;
    9   read r;
    10  m = x + r;
    11  call complicate;}
    procedure complicate {
    12	read q;
    13	read t;
    14	while (q < t) {
    15		if (t % 2 == 0) {
    16			t = t - 1;
    17			while (t > q + 1) {
    18				t = t - 1;}
    19			if (q % 2 == 0) {
    20				q = q - 1;}
                else {
    21              print q;}}
            else {
    22          print t;}
    23		while (q % 2 == 1) {
    24			q = q - 1;}}
    25	call extra;}
    procedure extra {
    26	print k;}
     */

public:
  PKB pkb;

  Statement w, r, a, c, i, p, stmt;
  LINE_NO l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13, l14, l15, l16,
      l17, l18, l19, l20, l21, l22, l23, l24, l25, l26, l999;
  LineNumber c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
      c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c999;
  Variable variable;
  Procedure procedure;
  Underscore underscore;
  Stmt STMT;

  TEST_METHOD_INITIALIZE(SetUpPkb) {
    w.type = StatementType::WHILE;
    r.type = StatementType::READ;
    a.type = StatementType::ASSIGN;
    c.type = StatementType::CALL;
    i.type = StatementType::IF;
    p.type = StatementType::PRINT;
    stmt.type = StatementType::NONE;

    l1 = "1";
    l2 = "2";
    l3 = "3";
    l4 = "4";
    l5 = "5";
    l6 = "6";
    l7 = "7";
    l8 = "8";
    l9 = "9";
    l10 = "10";
    l11 = "11";
    l12 = "12";
    l13 = "13";
    l14 = "14";
    l15 = "15";
    l16 = "16";
    l17 = "17";
    l18 = "18";
    l19 = "19";
    l20 = "20";
    l21 = "21";
    l22 = "22";
    l23 = "23";
    l24 = "24";
    l25 = "25";
    l26 = "26";
    l999 = "999";

    c1.number = l1;
    c2.number = l2;
    c3.number = l3;
    c4.number = l4;
    c5.number = l5;
    c6.number = l6;
    c7.number = l7;
    c8.number = l8;
    c9.number = l9;
    c10.number = l10;
    c11.number = l11;
    c12.number = l12;
    c13.number = l13;
    c14.number = l14;
    c15.number = l15;
    c16.number = l16;
    c17.number = l17;
    c18.number = l18;
    c19.number = l19;
    c20.number = l20;
    c21.number = l21;
    c22.number = l22;
    c23.number = l23;
    c24.number = l24;
    c25.number = l25;
    c26.number = l26;
    c999.number = l999;

    pkb.addFollow(l1, l2);
    pkb.addFollow(l2, l3);
    pkb.addFollow(l3, l6);
    pkb.addFollow(l4, l5);
    pkb.addFollow(l7, l8);
    pkb.addFollow(l8, l9);
    pkb.addFollow(l9, l10);
    pkb.addFollow(l10, l11);
    pkb.addFollow(l12, l13);
    pkb.addFollow(l13, l14);
    pkb.addFollow(l14, l25);
    pkb.addFollow(l15, l23);
    pkb.addFollow(l16, l17);
    pkb.addFollow(l17, l19);

    pkb.addStatementType(l1, StatementType::READ);
    pkb.addStatementType(l2, StatementType::READ);
    pkb.addStatementType(l3, StatementType::WHILE);
    pkb.addStatementType(l4, StatementType::ASSIGN);
    pkb.addStatementType(l5, StatementType::READ);
    pkb.addStatementType(l6, StatementType::PRINT);
    pkb.addStatementType(l7, StatementType::CALL);
    pkb.addStatementType(l8, StatementType::READ);
    pkb.addStatementType(l9, StatementType::READ);
    pkb.addStatementType(l10, StatementType::ASSIGN);
    pkb.addStatementType(l11, StatementType::CALL);
    pkb.addStatementType(l12, StatementType::READ);
    pkb.addStatementType(l13, StatementType::READ);
    pkb.addStatementType(l14, StatementType::WHILE);
    pkb.addStatementType(l15, StatementType::IF);
    pkb.addStatementType(l16, StatementType::ASSIGN);
    pkb.addStatementType(l17, StatementType::WHILE);
    pkb.addStatementType(l18, StatementType::ASSIGN);
    pkb.addStatementType(l19, StatementType::IF);
    pkb.addStatementType(l20, StatementType::ASSIGN);
    pkb.addStatementType(l21, StatementType::PRINT);
    pkb.addStatementType(l22, StatementType::PRINT);
    pkb.addStatementType(l23, StatementType::WHILE);
    pkb.addStatementType(l24, StatementType::ASSIGN);
    pkb.addStatementType(l25, StatementType::CALL);
    pkb.addStatementType(l26, StatementType::PRINT);
  }
  TEST_METHOD(FollowsLineNumberAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid follows
    Assert::IsTrue(pkbQueryInterface.follows(c1, c2));
    Assert::IsTrue(pkbQueryInterface.follows(c2, c3));
    Assert::IsTrue(pkbQueryInterface.follows(c3, c6));
    Assert::IsTrue(pkbQueryInterface.follows(c4, c5));
    Assert::IsTrue(pkbQueryInterface.follows(c7, c8));

    // Invalid follows
    Assert::IsFalse(pkbQueryInterface.follows(c1, c3));
    Assert::IsFalse(pkbQueryInterface.follows(c1, c5));
    Assert::IsFalse(pkbQueryInterface.follows(c1, c6));
    Assert::IsFalse(pkbQueryInterface.follows(c1, c7));
    Assert::IsFalse(pkbQueryInterface.follows(c2, c4));
    Assert::IsFalse(pkbQueryInterface.follows(c2, c8));
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid follows when following statement type is true, returns statement
    // line number.
    Assert::IsTrue(pkbQueryInterface.follows(c1, r) == STRING_SET{l2});
    Assert::IsTrue(pkbQueryInterface.follows(c2, w) == STRING_SET{l3});
    Assert::IsTrue(pkbQueryInterface.follows(c3, p) == STRING_SET{l6});
    Assert::IsTrue(pkbQueryInterface.follows(c4, r) == STRING_SET{l5});
    Assert::IsTrue(pkbQueryInterface.follows(c7, r) == STRING_SET{l8});

    // Invalid follows when following statement type is not true and returns 0.
    Assert::IsTrue(pkbQueryInterface.follows(c7, w) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(c1, a) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(c3, r) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(c4, c) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(c2, i) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndStmt) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid follows returns following line number else 0.
    Assert::IsTrue(pkbQueryInterface.follows(c1, stmt) == STRING_SET{l2});
    Assert::IsTrue(pkbQueryInterface.follows(c2, stmt) == STRING_SET{l3});
    Assert::IsTrue(pkbQueryInterface.follows(c3, stmt) == STRING_SET{l6});
    Assert::IsTrue(pkbQueryInterface.follows(c4, stmt) == STRING_SET{l5});
    Assert::IsTrue(pkbQueryInterface.follows(c7, stmt) == STRING_SET{l8});
    Assert::IsTrue(pkbQueryInterface.follows(c8, stmt) == STRING_SET{l9});
    Assert::IsTrue(pkbQueryInterface.follows(c999, stmt) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(c1, underscore));
    Assert::IsTrue(pkbQueryInterface.follows(c2, underscore));
    Assert::IsTrue(pkbQueryInterface.follows(c3, underscore));
    Assert::IsTrue(pkbQueryInterface.follows(c4, underscore));
    Assert::IsTrue(pkbQueryInterface.follows(c7, underscore));
    Assert::IsTrue(pkbQueryInterface.follows(c8, underscore));

    Assert::IsFalse(pkbQueryInterface.follows(c999, underscore));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid follows if statement type of previous line number from given
    // lineNumber is correct, returns the statement line number. Else returns 0.
    Assert::IsTrue(pkbQueryInterface.follows(a, c1) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(r, c2) == STRING_SET{l1});
    Assert::IsTrue(pkbQueryInterface.follows(a, c2) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(r, c3) == STRING_SET{l2});
    Assert::IsTrue(pkbQueryInterface.follows(w, c4) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(a, c5) == STRING_SET{l4});
    Assert::IsTrue(pkbQueryInterface.follows(w, c6) == STRING_SET{l3});
    Assert::IsTrue(pkbQueryInterface.follows(i, c6) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(a, c7) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(c, c8) == STRING_SET{l7});
    Assert::IsTrue(pkbQueryInterface.follows(r, c999) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.follows(r, r) ==
        STRING_PAIRS(STRING_VECTOR{l1, l8, l12}, STRING_VECTOR{l2, l9, l13}));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndStmt) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(r, stmt) ==
                   STRING_PAIRS(STRING_VECTOR{l1, l9, l2, l13, l8, l12},
                                STRING_VECTOR{l2, l10, l3, l14, l9, l13}));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(r, underscore) ==
                   STRING_SET{l1, l2, l8, l9, l12, l13});
  } // namespace UnitTesting
  TEST_METHOD(FollowsStmtAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid follows returns previous line number from given lineNumber, else 0.
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c1) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c2) == STRING_SET{l1});
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c3) == STRING_SET{l2});
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c4) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c5) == STRING_SET{l4});
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c6) == STRING_SET{l3});
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c7) == STRING_SET());
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c8) == STRING_SET{l7});
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c999) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(FollowsStmtAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(stmt, r) ==
                   STRING_PAIRS(STRING_VECTOR{l8, l1, l12, l4, l7},
                                STRING_VECTOR{l9, l2, l13, l5, l8}));

  } // namespace UnitTesting

  TEST_METHOD(FollowsStmtAndStmt) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(stmt, stmt) ==
                   STRING_PAIRS(STRING_VECTOR{l1, l2, l3, l4, l7, l8, l9, l10,
                                              l12, l13, l14, l15, l16, l17},
                                STRING_VECTOR{l2, l3, l6, l5, l8, l9, l10, l11,
                                              l13, l14, l25, l23, l17, l19}));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStmtAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(stmt, underscore) ==
                   STRING_SET{l1, l2, l3, l4, l7, l8, l9, l10, l12, l13, l14,
                              l15, l16, l17});
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(underscore, c5));
    Assert::IsTrue(pkbQueryInterface.follows(underscore, c17));

    Assert::IsFalse(pkbQueryInterface.follows(underscore, c4));
    Assert::IsFalse(pkbQueryInterface.follows(underscore, c1));
    Assert::IsFalse(pkbQueryInterface.follows(underscore, c16));
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(underscore, a) == STRING_SET{l10});
    Assert::IsTrue(pkbQueryInterface.follows(underscore, w) ==
                   STRING_SET{l3, l14, l17, l23});
    Assert::IsTrue(pkbQueryInterface.follows(underscore, r) ==
                   STRING_SET{l2, l5, l8, l9, l13});
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndStmt) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(underscore, stmt) ==
                   STRING_SET{l2, l3, l6, l5, l8, l9, l10, l11, l13, l14, l25,
                              l23, l17, l19});
  } // namespace UnitTesting

  TEST_METHOD(FollowsUnderscoreAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(underscore, underscore));
  } // namespace UnitTesting

  TEST_METHOD(FollowsStarStmtAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.followsStar(STMT, a) ==
        STRING_PAIRS(STRING_VECTOR{l9, l8, l7}, STRING_VECTOR{l10, l10, l10}));
  } // namespace UnitTesting
};
} // namespace UnitTesting
