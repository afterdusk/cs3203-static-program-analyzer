#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestParentQuery) {
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

    pkb.addParent(4, 3);
    pkb.addParent(5, 3);
    pkb.addParent(15, 14);
    pkb.addParent(23, 14);
    pkb.addParent(16, 15);
    pkb.addParent(17, 15);
    pkb.addParent(19, 15);
    pkb.addParent(22, 15);
    pkb.addParent(18, 17);
    pkb.addParent(20, 19);
    pkb.addParent(21, 19);
    pkb.addParent(24, 23);

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
  TEST_METHOD(ParentLineNumberAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.parent(c3, c4).value);
    Assert::IsTrue(pkbQueryInterface.parent(c3, c5).value);
    Assert::IsTrue(pkbQueryInterface.parent(c14, c15).value);
    Assert::IsTrue(pkbQueryInterface.parent(c14, c23).value);
    Assert::IsTrue(pkbQueryInterface.parent(c15, c16).value);
    Assert::IsTrue(pkbQueryInterface.parent(c15, c19).value);
    Assert::IsTrue(pkbQueryInterface.parent(c15, c22).value);
    Assert::IsTrue(pkbQueryInterface.parent(c17, c18).value);
    Assert::IsTrue(pkbQueryInterface.parent(c19, c21).value);
    Assert::IsTrue(pkbQueryInterface.parent(c23, c24).value);

    Assert::IsFalse(pkbQueryInterface.parent(c999, c1).value);
    Assert::IsFalse(pkbQueryInterface.parent(c15, c999).value);
    Assert::IsFalse(pkbQueryInterface.parent(c15, c23).value);
    Assert::IsFalse(pkbQueryInterface.parent(c15, c21).value);
    Assert::IsFalse(pkbQueryInterface.parent(c14, c16).value);
    Assert::IsFalse(pkbQueryInterface.parent(c14, c22).value);
    Assert::IsFalse(pkbQueryInterface.parent(c16, c18).value);

  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid parent constant returns line numbers that matches statement type.
    Assert::IsTrue(pkbQueryInterface.parent(c3, a).value == StmtNumberList{4});
    Assert::IsTrue(pkbQueryInterface.parent(c3, r).value == StmtNumberList{5});
    Assert::IsTrue(pkbQueryInterface.parent(c14, i).value ==
                   StmtNumberList{15});
    Assert::IsTrue(pkbQueryInterface.parent(c14, w).value ==
                   StmtNumberList{23});
    Assert::IsTrue(pkbQueryInterface.parent(c15, a).value ==
                   StmtNumberList{16});
    Assert::IsTrue(pkbQueryInterface.parent(c15, w).value ==
                   StmtNumberList{17});
    Assert::IsTrue(pkbQueryInterface.parent(c15, i).value ==
                   StmtNumberList{19});
    Assert::IsTrue(pkbQueryInterface.parent(c17, a).value ==
                   StmtNumberList{18});
    Assert::IsTrue(pkbQueryInterface.parent(c19, a).value ==
                   StmtNumberList{20});
    Assert::IsTrue(pkbQueryInterface.parent(c19, p).value ==
                   StmtNumberList{21});
    Assert::IsTrue(pkbQueryInterface.parent(c23, a).value ==
                   StmtNumberList{24});

    // Invalid parent constants that aren't parents, return empty vector;
    Assert::IsTrue(pkbQueryInterface.parent(c2, r).value ==
                   StmtNumberList()); // constant on read
    Assert::IsTrue(pkbQueryInterface.parent(c4, a).value ==
                   StmtNumberList()); // constant on assign
    Assert::IsTrue(pkbQueryInterface.parent(c21, p).value ==
                   StmtNumberList()); // constant on print
    Assert::IsTrue(pkbQueryInterface.parent(c11, c).value ==
                   StmtNumberList()); // constant on call

  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndStmt) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid parent
    Assert::IsTrue(pkbQueryInterface.parent(c3, stmt).value ==
                   StmtNumberList{4, 5});

    // Invalid parent
    Assert::IsTrue(pkbQueryInterface.parent(c16, stmt).value ==
                   StmtNumberList());
  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid parent
    Assert::IsTrue(pkbQueryInterface.parent(c3, underscore).value);

    // Invalid parent
    Assert::IsFalse(pkbQueryInterface.parent(c16, underscore).value);
  } // namespace UnitTesting

  TEST_METHOD(ParentStatementAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid parent
    Assert::IsTrue(
        pkbQueryInterface.parent(w, c4).value ==
        StmtNumberList{
            3}); // LineNumber is in container and Parent stmt type is correct

    // Invalid parent
    Assert::IsTrue(pkbQueryInterface.parent(i, c4).value ==
                   StmtNumberList()); // Parent stmt is not the correct type
    Assert::IsTrue(
        pkbQueryInterface.parent(w, c13).value ==
        StmtNumberList()); // LineNumber does not belong in any container
  }                        // namespace UnitTesting
};

} // namespace UnitTesting
