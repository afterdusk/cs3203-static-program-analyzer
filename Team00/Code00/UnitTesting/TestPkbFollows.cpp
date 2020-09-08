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

  Statement w, r, a, c, i, p;
  LineNumber c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
      c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c999;
  Stmt stmt;
  Variable variable;
  Procedure procedure;
  Underscore underscore;

  TEST_METHOD_INITIALIZE(SetUpPkb) {
    w.type = StatementType::WHILE;
    r.type = StatementType::READ;
    a.type = StatementType::ASSIGN;
    c.type = StatementType::CALL;
    i.type = StatementType::IF;
    p.type = StatementType::PRINT;

    c1.number = 1;
    c2.number = 2;
    c3.number = 3;
    c4.number = 4;
    c5.number = 5;
    c6.number = 6;
    c7.number = 7;
    c8.number = 8;
    c9.number = 9;
    c10.number = 10;
    c11.number = 11;
    c12.number = 12;
    c13.number = 13;
    c14.number = 14;
    c15.number = 15;
    c16.number = 16;
    c17.number = 17;
    c18.number = 18;
    c19.number = 19;
    c20.number = 20;
    c21.number = 21;
    c22.number = 22;
    c23.number = 23;
    c24.number = 24;
    c25.number = 25;
    c26.number = 26;
    c999.number = 999;

    pkb.addFollow(1, 2);
    pkb.addFollow(2, 3);
    pkb.addFollow(3, 6);
    pkb.addFollow(4, 5);
    pkb.addFollow(7, 8);
    pkb.addFollow(8, 9);
    pkb.addFollow(9, 10);
    pkb.addFollow(10, 11);
    pkb.addFollow(12, 13);
    pkb.addFollow(13, 14);
    pkb.addFollow(14, 25);
    pkb.addFollow(15, 23);
    pkb.addFollow(16, 17);
    pkb.addFollow(17, 19);

    pkb.addStatementType(1, StatementType::READ);
    pkb.addStatementType(2, StatementType::READ);
    pkb.addStatementType(3, StatementType::WHILE);
    pkb.addStatementType(4, StatementType::ASSIGN);
    pkb.addStatementType(5, StatementType::READ);
    pkb.addStatementType(6, StatementType::PRINT);
    pkb.addStatementType(7, StatementType::CALL);
    pkb.addStatementType(8, StatementType::READ);
    pkb.addStatementType(9, StatementType::READ);
    pkb.addStatementType(10, StatementType::ASSIGN);
    pkb.addStatementType(11, StatementType::CALL);
    pkb.addStatementType(12, StatementType::READ);
    pkb.addStatementType(13, StatementType::READ);
    pkb.addStatementType(14, StatementType::WHILE);
    pkb.addStatementType(15, StatementType::IF);
    pkb.addStatementType(16, StatementType::ASSIGN);
    pkb.addStatementType(17, StatementType::WHILE);
    pkb.addStatementType(18, StatementType::ASSIGN);
    pkb.addStatementType(19, StatementType::IF);
    pkb.addStatementType(20, StatementType::ASSIGN);
    pkb.addStatementType(21, StatementType::PRINT);
    pkb.addStatementType(22, StatementType::PRINT);
    pkb.addStatementType(23, StatementType::WHILE);
    pkb.addStatementType(24, StatementType::ASSIGN);
    pkb.addStatementType(25, StatementType::CALL);
    pkb.addStatementType(26, StatementType::PRINT);
  }
  TEST_METHOD(FollowsLineNumberAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid follows
    Assert::IsTrue(pkbQueryInterface.follows(c1, c2).value);
    Assert::IsTrue(pkbQueryInterface.follows(c2, c3).value);
    Assert::IsTrue(pkbQueryInterface.follows(c3, c6).value);
    Assert::IsTrue(pkbQueryInterface.follows(c4, c5).value);
    Assert::IsTrue(pkbQueryInterface.follows(c7, c8).value);

    // Invalid follows
    Assert::IsFalse(pkbQueryInterface.follows(c1, c3).value);
    Assert::IsFalse(pkbQueryInterface.follows(c1, c5).value);
    Assert::IsFalse(pkbQueryInterface.follows(c1, c6).value);
    Assert::IsFalse(pkbQueryInterface.follows(c1, c7).value);
    Assert::IsFalse(pkbQueryInterface.follows(c2, c4).value);
    Assert::IsFalse(pkbQueryInterface.follows(c2, c8).value);
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid follows when following statement type is true, returns statement
    // line number.
    Assert::IsTrue(pkbQueryInterface.follows(c1, r).value == StmtNumberList{2});
    Assert::IsTrue(pkbQueryInterface.follows(c2, w).value == StmtNumberList{3});
    Assert::IsTrue(pkbQueryInterface.follows(c3, p).value == StmtNumberList{6});
    Assert::IsTrue(pkbQueryInterface.follows(c4, r).value == StmtNumberList{5});
    Assert::IsTrue(pkbQueryInterface.follows(c7, r).value == StmtNumberList{8});

    // Invalid follows when following statement type is not true and returns 0.
    Assert::IsTrue(pkbQueryInterface.follows(c7, w).value == StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(c1, a).value == StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(c3, r).value == StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(c4, c).value == StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(c2, i).value == StmtNumberList());
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndStmt) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid follows returns following line number else 0.
    Assert::IsTrue(pkbQueryInterface.follows(c1, stmt).value ==
                   StmtNumberList{2});
    Assert::IsTrue(pkbQueryInterface.follows(c2, stmt).value ==
                   StmtNumberList{3});
    Assert::IsTrue(pkbQueryInterface.follows(c3, stmt).value ==
                   StmtNumberList{6});
    Assert::IsTrue(pkbQueryInterface.follows(c4, stmt).value ==
                   StmtNumberList{5});
    Assert::IsTrue(pkbQueryInterface.follows(c7, stmt).value ==
                   StmtNumberList{8});
    Assert::IsTrue(pkbQueryInterface.follows(c8, stmt).value ==
                   StmtNumberList{9});
    Assert::IsTrue(pkbQueryInterface.follows(c999, stmt).value ==
                   StmtNumberList());
  } // namespace UnitTesting

  TEST_METHOD(FollowsLineNumberAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.follows(c1, underscore).value);
    Assert::IsTrue(pkbQueryInterface.follows(c2, underscore).value);
    Assert::IsTrue(pkbQueryInterface.follows(c3, underscore).value);
    Assert::IsTrue(pkbQueryInterface.follows(c4, underscore).value);
    Assert::IsTrue(pkbQueryInterface.follows(c7, underscore).value);
    Assert::IsTrue(pkbQueryInterface.follows(c8, underscore).value);

    Assert::IsFalse(pkbQueryInterface.follows(c999, underscore).value);
  } // namespace UnitTesting

  TEST_METHOD(FollowsStatementAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid follows if statement type of previous line number from given
    // lineNumber is correct, returns the statement line number. Else returns 0.
    Assert::IsTrue(pkbQueryInterface.follows(a, c1).value == StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(r, c2).value == StmtNumberList{1});
    Assert::IsTrue(pkbQueryInterface.follows(a, c2).value == StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(r, c3).value == StmtNumberList{2});
    Assert::IsTrue(pkbQueryInterface.follows(w, c4).value == StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(a, c5).value == StmtNumberList{4});
    Assert::IsTrue(pkbQueryInterface.follows(w, c6).value == StmtNumberList{3});
    Assert::IsTrue(pkbQueryInterface.follows(i, c6).value == StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(a, c7).value == StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(c, c8).value == StmtNumberList{7});
    Assert::IsTrue(pkbQueryInterface.follows(r, c999).value ==
                   StmtNumberList());
  } // namespace UnitTesting
  TEST_METHOD(FollowsStmtAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid follows returns previous line number from given lineNumber, else 0.
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c1).value ==
                   StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c2).value ==
                   StmtNumberList{1});
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c3).value ==
                   StmtNumberList{2});
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c4).value ==
                   StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c5).value ==
                   StmtNumberList{4});
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c6).value ==
                   StmtNumberList{3});
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c7).value ==
                   StmtNumberList());
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c8).value ==
                   StmtNumberList{7});
    Assert::IsTrue(pkbQueryInterface.follows(stmt, c999).value ==
                   StmtNumberList());
  } // namespace UnitTesting
};

} // namespace UnitTesting
