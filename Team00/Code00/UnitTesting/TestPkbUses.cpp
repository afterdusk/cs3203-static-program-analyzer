#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestUsesQuery) {
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
  String strx, stry, strr, strm, strq, strt, strk, strpmain, strpaux, strpextra,
      strpcomplicate;
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

    strx.name = "x";
    stry.name = "y";
    strr.name = "r";
    strm.name = "m";
    strq.name = "q";
    strt.name = "t";
    strk.name = "k";

    strpmain.name = "main";
    strpaux.name = "aux";
    strpextra.name = "extra";
    strpcomplicate.name = "complicate";

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

    VAR_TABLE_INDEX vx = pkb.addVar("x");
    VAR_TABLE_INDEX vy = pkb.addVar("y");
    VAR_TABLE_INDEX vr = pkb.addVar("r");
    VAR_TABLE_INDEX vm = pkb.addVar("m");
    VAR_TABLE_INDEX vq = pkb.addVar("q");
    VAR_TABLE_INDEX vt = pkb.addVar("t");
    VAR_TABLE_INDEX vk = pkb.addVar("k");

    PROC_TABLE_INDEX paux = pkb.addProc("aux");
    PROC_TABLE_INDEX pmain = pkb.addProc("main");
    PROC_TABLE_INDEX pcomplicate = pkb.addProc("complicate");
    PROC_TABLE_INDEX pextra = pkb.addProc("extra");

    pkb.addUses(l3, VAR_TABLE_INDEXES{vx, vy});
    pkb.addUses(l4, VAR_TABLE_INDEXES{vx, vy});
    pkb.addUses(l6, VAR_TABLE_INDEXES{vx});
    pkb.addUses(l7, paux);
    pkb.addUses(l10, VAR_TABLE_INDEXES{vx, vr});
    pkb.addUses(l11, pcomplicate);
    pkb.addUses(l14, VAR_TABLE_INDEXES{vq, vt});
    pkb.addUses(l15, VAR_TABLE_INDEXES{vq, vt});
    pkb.addUses(l16, VAR_TABLE_INDEXES{vt});
    pkb.addUses(l17, VAR_TABLE_INDEXES{vq, vt});
    pkb.addUses(l18, VAR_TABLE_INDEXES{vt});
    pkb.addUses(l19, VAR_TABLE_INDEXES{vq});
    pkb.addUses(l20, VAR_TABLE_INDEXES{vq});
    pkb.addUses(l21, VAR_TABLE_INDEXES{vq});
    pkb.addUses(l22, VAR_TABLE_INDEXES{vt});
    pkb.addUses(l23, VAR_TABLE_INDEXES{vq});
    pkb.addUses(l24, VAR_TABLE_INDEXES{vq});
    pkb.addUses(l25, pextra);
    pkb.addUses(l26, VAR_TABLE_INDEXES{vk});

    pkb.addUsesProc(paux, VAR_TABLE_INDEXES{vy, vx});
    pkb.addUsesProc(pmain, VAR_TABLE_INDEXES{vx, vr, vy, vq, vt, vk});
    pkb.addUsesProc(pcomplicate, VAR_TABLE_INDEXES{vq, vt, vk});
    pkb.addUsesProc(pextra, VAR_TABLE_INDEXES{vk});
  }
  TEST_METHOD(UsesLineNumberAndString) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.uses(c4, strx));
    Assert::IsTrue(pkbQueryInterface.uses(c4, stry));

    Assert::IsTrue(pkbQueryInterface.uses(c11, strq)); // used in call procedure

    Assert::IsFalse(pkbQueryInterface.uses(
        c11, stry)); // call procedure doesnt use input var
    Assert::IsFalse(pkbQueryInterface.uses(c2, stry)); // line doesnt use
    Assert::IsFalse(pkbQueryInterface.uses(c4, strr)); // incorrect var used
  }                                                    // namespace UnitTesting

  TEST_METHOD(UsesLineNumberAndVariable) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.uses(c4, variable) ==
                   STRING_SET{"x", "y"}); // on assign statement
    Assert::IsTrue(pkbQueryInterface.uses(c3, variable) ==
                   STRING_SET{"x", "y"}); // on while statement
    Assert::IsTrue(pkbQueryInterface.uses(c7, variable) ==
                   STRING_SET{"x", "y"}); // on call statement
  }                                       // namespace UnitTesting

  TEST_METHOD(UsesLineNumberAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.uses(c3, underscore)); // on while statement
    Assert::IsTrue(pkbQueryInterface.uses(c7, underscore)); // on call statement
    Assert::IsTrue(
        pkbQueryInterface.uses(c26, underscore)); // on print statement

    Assert::IsFalse(
        pkbQueryInterface.uses(c12, underscore)); // on read statement
  }                                               // namespace UnitTesting

  TEST_METHOD(UsesStmtAndString) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.uses(stmt, stry) ==
                   STRING_SET{l3, l4, l7});
    Assert::IsTrue(pkbQueryInterface.uses(stmt, strr) == STRING_SET{l10});

    Assert::IsTrue(pkbQueryInterface.uses(stmt, strm) ==
                   STRING_SET{}); // string var not used

  } // namespace UnitTesting

  TEST_METHOD(UsesStmtAndVariable) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.uses(stmt, variable) ==
        STRING_PAIRS{
            STRING_VECTOR{l3,  l3,  l15, l15, l4,  l4,  l6,  l7,  l7,  l10,
                          l10, l11, l11, l11, l14, l14, l16, l17, l17, l21,
                          l18, l20, l19, l22, l23, l24, l25, l26},
            STRING_VECTOR{"x", "y", "q", "t", "x", "y", "x", "y", "x", "x",
                          "r", "q", "t", "k", "q", "t", "t", "q", "t", "q",
                          "t", "q", "q", "t", "q", "q", "k", "k"}});
  } // namespace UnitTesting

  TEST_METHOD(UsesStmtAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.uses(stmt, underscore) ==
                   STRING_SET{l3, l4, l6, l7, l10, l11, l14, l15, l16, l17, l18,
                              l19, l20, l21, l22, l23, l24, l25, l26});

  } // namespace UnitTesting

  TEST_METHOD(UsesStatementAndString) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.uses(a, strx) == STRING_SET{l4, l10});
    Assert::IsTrue(pkbQueryInterface.uses(w, strt) == STRING_SET{l14, l17});
    Assert::IsTrue(pkbQueryInterface.uses(c, strx) == STRING_SET{l7});

  } // namespace UnitTesting

  TEST_METHOD(UsesStatementAndVariable) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.uses(a, variable) ==
        STRING_PAIRS(STRING_VECTOR{l4, l4, l24, l10, l10, l18, l16, l20},
                     STRING_VECTOR{"x", "y", "q", "x", "r", "t", "t", "q"}));

  } // namespace UnitTesting

  TEST_METHOD(UsesStatementAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.uses(w, underscore) ==
                   STRING_SET{l3, l14, l17, l23});

  } // namespace UnitTesting

  TEST_METHOD(UsesStringAndString) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.uses(strpmain, strx)); // proc directly using
    Assert::IsTrue(pkbQueryInterface.uses(
        strpmain, strt)); // proc indirectly using through call
    Assert::IsTrue(pkbQueryInterface.uses(
        strpmain, strk)); // proc indirectly using through nested call

    Assert::IsFalse(
        pkbQueryInterface.uses(strpaux, strr)); // proc doesn't use that var

  } // namespace UnitTesting

  TEST_METHOD(UsesStringAndVariable) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.uses(strpaux, variable) ==
                   STRING_SET{"x", "y"}); // no calls, only vars used directly
    Assert::IsTrue(
        pkbQueryInterface.uses(strpmain, variable) ==
        STRING_SET{"x", "y", "r", "q", "t",
                   "k"}); // proc main uses all vars directly & indirectly

  } // namespace UnitTesting

  TEST_METHOD(UsesStringAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.uses(strpaux, underscore));
    Assert::IsTrue(pkbQueryInterface.uses(strpextra, underscore));
    Assert::IsTrue(pkbQueryInterface.uses(strpmain, underscore));
    Assert::IsTrue(pkbQueryInterface.uses(strpcomplicate, underscore));

  } // namespace UnitTesting

  TEST_METHOD(UsesProcedureAndString) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.uses(procedure, strx) ==
                   STRING_SET{"aux", "main"});
    Assert::IsTrue(pkbQueryInterface.uses(procedure, strk) ==
                   STRING_SET{"main", "complicate", "extra"});
  } // namespace UnitTesting

  TEST_METHOD(UsesProcedureAndVariable) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.uses(procedure, variable) ==
        STRING_PAIRS(STRING_VECTOR{"aux", "aux", "main", "main", "main", "main",
                                   "main", "main", "complicate", "complicate",
                                   "complicate", "extra"},
                     STRING_VECTOR{"y", "x", "x", "r", "y", "q", "t", "k", "q",
                                   "t", "k", "k"}));
  } // namespace UnitTesting

  TEST_METHOD(UsesProcedureAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.uses(procedure, underscore) ==
                   STRING_SET{"aux", "main", "complicate", "extra"});

  } // namespace UnitTesting
};

} // namespace UnitTesting
