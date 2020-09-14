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
  Pkb pkb;

  Statement w, r, a, c, i, p, stmt;
  LINE_NO l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13, l14, l15, l16,
      l17, l18, l19, l20, l21, l22, l23, l24, l25, l26, l999;
  LineNumber c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
      c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c999;
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

    pkb.addParent(l4, l3);
    pkb.addParent(l5, l3);
    pkb.addParent(l15, l14);
    pkb.addParent(l23, l14);
    pkb.addParent(l16, l15);
    pkb.addParent(l17, l15);
    pkb.addParent(l19, l15);
    pkb.addParent(l22, l15);
    pkb.addParent(l18, l17);
    pkb.addParent(l20, l19);
    pkb.addParent(l21, l19);
    pkb.addParent(l24, l23);

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
  TEST_METHOD(ParentLineNumberAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.parent(c3, c4));
    Assert::IsTrue(pkbQueryInterface.parent(c3, c5));
    Assert::IsTrue(pkbQueryInterface.parent(c14, c15));
    Assert::IsTrue(pkbQueryInterface.parent(c14, c23));
    Assert::IsTrue(pkbQueryInterface.parent(c15, c16));
    Assert::IsTrue(pkbQueryInterface.parent(c15, c19));
    Assert::IsTrue(pkbQueryInterface.parent(c15, c22));
    Assert::IsTrue(pkbQueryInterface.parent(c17, c18));
    Assert::IsTrue(pkbQueryInterface.parent(c19, c21));
    Assert::IsTrue(pkbQueryInterface.parent(c23, c24));

    Assert::IsFalse(pkbQueryInterface.parent(c999, c1));
    Assert::IsFalse(pkbQueryInterface.parent(c15, c999));
    Assert::IsFalse(pkbQueryInterface.parent(c15, c23));
    Assert::IsFalse(pkbQueryInterface.parent(c15, c21));
    Assert::IsFalse(pkbQueryInterface.parent(c14, c16));
    Assert::IsFalse(pkbQueryInterface.parent(c14, c22));
    Assert::IsFalse(pkbQueryInterface.parent(c16, c18));

  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid parent constant returns line numbers that matches statement type.
    Assert::IsTrue(pkbQueryInterface.parent(c3, a) == STRING_SET{l4});
    Assert::IsTrue(pkbQueryInterface.parent(c3, r) == STRING_SET{l5});
    Assert::IsTrue(pkbQueryInterface.parent(c14, i) == STRING_SET{l15});
    Assert::IsTrue(pkbQueryInterface.parent(c14, w) == STRING_SET{l23});
    Assert::IsTrue(pkbQueryInterface.parent(c15, a) == STRING_SET{l16});
    Assert::IsTrue(pkbQueryInterface.parent(c15, w) == STRING_SET{l17});
    Assert::IsTrue(pkbQueryInterface.parent(c15, i) == STRING_SET{l19});
    Assert::IsTrue(pkbQueryInterface.parent(c17, a) == STRING_SET{l18});
    Assert::IsTrue(pkbQueryInterface.parent(c19, a) == STRING_SET{l20});
    Assert::IsTrue(pkbQueryInterface.parent(c19, p) == STRING_SET{l21});
    Assert::IsTrue(pkbQueryInterface.parent(c23, a) == STRING_SET{l24});

    // Invalid parent constants that aren't parents, return empty vector;
    Assert::IsTrue(pkbQueryInterface.parent(c2, r) ==
                   STRING_SET()); // constant on read
    Assert::IsTrue(pkbQueryInterface.parent(c4, a) ==
                   STRING_SET()); // constant on assign
    Assert::IsTrue(pkbQueryInterface.parent(c21, p) ==
                   STRING_SET()); // constant on print
    Assert::IsTrue(pkbQueryInterface.parent(c11, c) ==
                   STRING_SET()); // constant on call

  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndStmt) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid parent
    Assert::IsTrue(pkbQueryInterface.parent(c3, stmt) == STRING_SET{l4, l5});

    // Invalid parent
    Assert::IsTrue(pkbQueryInterface.parent(c16, stmt) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentLineNumberAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid parent
    Assert::IsTrue(pkbQueryInterface.parent(c3, underscore));

    // Invalid parent
    Assert::IsFalse(pkbQueryInterface.parent(c16, underscore));
  } // namespace UnitTesting

  TEST_METHOD(ParentStatementAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // Valid parent
    Assert::IsTrue(
        pkbQueryInterface.parent(w, c4) ==
        STRING_SET{
            l3}); // LineNumber is in container and Parent stmt type is correct

    // Invalid parent
    Assert::IsTrue(pkbQueryInterface.parent(i, c4) ==
                   STRING_SET()); // Parent stmt is not the correct type
    Assert::IsTrue(pkbQueryInterface.parent(w, c13) ==
                   STRING_SET()); // LineNumber does not belong in any container
  }                               // namespace UnitTesting

  TEST_METHOD(ParentStatementAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.parent(i, a) ==
        STRING_PAIRS(STRING_VECTOR{l15, l19}, STRING_VECTOR{l16, l20}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStatementAndStmt) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.parent(i, stmt) ==
                   STRING_PAIRS(STRING_VECTOR{l15, l15, l15, l15, l19, l19},
                                STRING_VECTOR{l16, l17, l19, l22, l20, l21}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStatementAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.parent(w, underscore) ==
                   STRING_SET{l3, l14, l17, l23});
    Assert::IsTrue(pkbQueryInterface.parent(i, underscore) ==
                   STRING_SET{l15, l19});
    Assert::IsTrue(pkbQueryInterface.parent(c, underscore) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.parent(stmt, c15) == STRING_SET{l14});

    // line number is not a child
    Assert::IsTrue(pkbQueryInterface.parent(stmt, c14) == STRING_SET());
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.parent(stmt, a) ==
                   STRING_PAIRS(STRING_VECTOR{l3, l23, l17, l15, l19},
                                STRING_VECTOR{l4, l24, l18, l16, l20}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndStmt) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.parent(stmt, stmt) ==
                   STRING_PAIRS(STRING_VECTOR{l3, l3, l23, l14, l14, l15, l15,
                                              l15, l15, l17, l19, l19},
                                STRING_VECTOR{l4, l5, l24, l15, l23, l16, l17,
                                              l19, l22, l18, l20, l21}));
  } // namespace UnitTesting

  TEST_METHOD(ParentStmtAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.parent(stmt, underscore) ==
                   STRING_SET{l23, l3, l14, l15, l17, l19});
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndLineNumber) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    // line number is a child
    Assert::IsTrue(pkbQueryInterface.parent(underscore, c15));

    // line number is not a child
    Assert::IsFalse(pkbQueryInterface.parent(underscore, c14));
    Assert::IsFalse(pkbQueryInterface.parent(underscore, c1));
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndStatement) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.parent(underscore, w) ==
                   STRING_SET{l23, l17});
    Assert::IsTrue(pkbQueryInterface.parent(underscore, a) ==
                   STRING_SET{l4, l16, l18, l20, l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndStmt) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.parent(underscore, stmt) ==
        STRING_SET{l4, l5, l15, l23, l16, l17, l19, l22, l18, l20, l21, l24});
  } // namespace UnitTesting

  TEST_METHOD(ParentUnderscoreAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.parent(underscore, underscore));
  } // namespace UnitTesting
};

} // namespace UnitTesting
