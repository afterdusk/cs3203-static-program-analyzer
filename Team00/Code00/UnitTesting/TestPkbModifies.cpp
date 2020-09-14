#include "CppUnitTest.h"
#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestModifiesQuery) {
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

    pkb.addModifies(l1, VAR_TABLE_INDEXES{vx});
    pkb.addModifies(l2, VAR_TABLE_INDEXES{vy});
    pkb.addModifies(l3, VAR_TABLE_INDEXES{vy});
    pkb.addModifies(l4, VAR_TABLE_INDEXES{vy});
    pkb.addModifies(l5, VAR_TABLE_INDEXES{vy});
    pkb.addModifies(l7, paux);
    pkb.addModifies(l8, VAR_TABLE_INDEXES{vx});
    pkb.addModifies(l9, VAR_TABLE_INDEXES{vr});
    pkb.addModifies(l10, VAR_TABLE_INDEXES{vm});
    pkb.addModifies(l11, pcomplicate);
    pkb.addModifies(l12, VAR_TABLE_INDEXES{vq});
    pkb.addModifies(l13, VAR_TABLE_INDEXES{vt});
    pkb.addModifies(l14, VAR_TABLE_INDEXES{vq, vt});
    pkb.addModifies(l15, VAR_TABLE_INDEXES{vq, vt});
    pkb.addModifies(l16, VAR_TABLE_INDEXES{vt});
    pkb.addModifies(l17, VAR_TABLE_INDEXES{vt});
    pkb.addModifies(l18, VAR_TABLE_INDEXES{vt});
    pkb.addModifies(l19, VAR_TABLE_INDEXES{vq});
    pkb.addModifies(l20, VAR_TABLE_INDEXES{vq});
    pkb.addModifies(l23, VAR_TABLE_INDEXES{vq});
    pkb.addModifies(l24, VAR_TABLE_INDEXES{vq});
    pkb.addModifies(l25, pextra);

    pkb.addModifiesProc(paux, VAR_TABLE_INDEXES{vy, vx});
    pkb.addModifiesProc(pmain, VAR_TABLE_INDEXES{vx, vr, vm, vy, vq, vt});
    pkb.addModifiesProc(pcomplicate, VAR_TABLE_INDEXES{vq, vt});
  }
  TEST_METHOD(ModifiesLineNumberAndString) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.modifies(c4, stry)); // assign statement
    Assert::IsTrue(pkbQueryInterface.modifies(c5, stry)); // read statement

    Assert::IsTrue(
        pkbQueryInterface.modifies(c11, strq)); // modifies in call procedure

    Assert::IsFalse(pkbQueryInterface.modifies(
        c11, stry)); // call procedure doesnt modifies input var
    Assert::IsFalse(
        pkbQueryInterface.modifies(c6, strx)); // print doesn't modify var
    Assert::IsFalse(
        pkbQueryInterface.modifies(c4, strr)); // incorrect var modified
  }                                            // namespace UnitTesting

  TEST_METHOD(ModifiesLineNumberAndVariable) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.modifies(c4, variable) ==
                   STRING_SET{"y"}); // on assign statement
    Assert::IsTrue(pkbQueryInterface.modifies(c14, variable) ==
                   STRING_SET{"q", "t"}); // on while statement
    Assert::IsTrue(pkbQueryInterface.modifies(c7, variable) ==
                   STRING_SET{"x", "y"}); // on call statement
  }                                       // namespace UnitTesting

  TEST_METHOD(ModifiesLineNumberAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.modifies(c3, underscore)); // on while statement
    Assert::IsTrue(
        pkbQueryInterface.modifies(c7, underscore)); // on call statement
    Assert::IsTrue(
        pkbQueryInterface.modifies(c12, underscore)); // on read statement

    Assert::IsFalse(
        pkbQueryInterface.modifies(c26, underscore)); // on print statement
  }                                                   // namespace UnitTesting

  TEST_METHOD(ModifiesStmtAndString) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.modifies(stmt, strm) == STRING_SET{l10});
    Assert::IsTrue(pkbQueryInterface.modifies(stmt, strx) ==
                   STRING_SET{l1, l7, l8});

    Assert::IsTrue(pkbQueryInterface.modifies(stmt, strk) ==
                   STRING_SET{}); // string var not modified
  }                               // namespace UnitTesting

  TEST_METHOD(ModifiesStmtAndVariable) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.modifies(stmt, variable) ==
        STRING_PAIRS{STRING_VECTOR{l9,  l1,  l2,  l3,  l4,  l5,  l7,  l7,  l8,
                                   l10, l11, l11, l12, l13, l14, l14, l15, l15,
                                   l16, l17, l18, l20, l19, l23, l24},
                     STRING_VECTOR{"r", "x", "y", "y", "y", "y", "y", "x", "x",
                                   "m", "q", "t", "q", "t", "q", "t", "q", "t",
                                   "t", "t", "t", "q", "q", "q", "q"}});

  } // namespace UnitTesting

  TEST_METHOD(ModifiesStmtAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.modifies(stmt, underscore) ==
                   STRING_SET{l1,  l2,  l3,  l4,  l5,  l7,  l8,
                              l9,  l10, l11, l12, l13, l14, l15,
                              l16, l17, l18, l19, l20, l23, l24});
  } // namespace UnitTesting

  TEST_METHOD(ModifiesStatementAndString) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.modifies(a, strx) ==
                   STRING_SET{}); // no assignment modifies var x
    Assert::IsTrue(pkbQueryInterface.modifies(w, strt) == STRING_SET{l14, l17});
    Assert::IsTrue(pkbQueryInterface.modifies(c, strx) == STRING_SET{l7});
  } // namespace UnitTesting

  TEST_METHOD(ModifiesStatementAndVariable) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.modifies(a, variable) ==
                   STRING_PAIRS(STRING_VECTOR{l4, l24, l10, l18, l16, l20},
                                STRING_VECTOR{"y", "q", "m", "t", "t", "q"}));
  } // namespace UnitTesting

  TEST_METHOD(ModifiesStatementAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.modifies(w, underscore) ==
                   STRING_SET{l3, l14, l17, l23});
    Assert::IsTrue(pkbQueryInterface.modifies(c, underscore) ==
                   STRING_SET{l7, l11});

  } // namespace UnitTesting

  TEST_METHOD(ModifiesStringAndString) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.modifies(strpmain, strx)); // proc directly modifies
    Assert::IsTrue(pkbQueryInterface.modifies(
        strpmain, strt)); // proc indirectly modify through call

    Assert::IsFalse(pkbQueryInterface.modifies(
        strpmain, strk)); // proc does not indirectly modify through nested call
    Assert::IsFalse(
        pkbQueryInterface.modifies(strpextra, strk)); // proc doesn't modify
  }                                                   // namespace UnitTesting

  TEST_METHOD(ModifiesStringAndVariable) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.modifies(strpaux, variable) ==
        STRING_SET{"x", "y"}); // no calls, only vars modified directly
    Assert::IsTrue(
        pkbQueryInterface.modifies(strpmain, variable) ==
        STRING_SET{"x", "y", "r", "q", "t",
                   "m"}); // proc main modifies all vars directly & indirectly
  }                       // namespace UnitTesting

  TEST_METHOD(ModifiesStringAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.modifies(strpaux, underscore));
    Assert::IsTrue(pkbQueryInterface.modifies(strpmain, underscore));
    Assert::IsTrue(pkbQueryInterface.modifies(strpcomplicate, underscore));

    Assert::IsFalse(pkbQueryInterface.modifies(
        strpextra, underscore)); // proc does not modify any vars
  }                              // namespace UnitTesting

  TEST_METHOD(ModifiesProcedureAndString) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.modifies(procedure, strx) ==
                   STRING_SET{"aux", "main"});
    Assert::IsTrue(pkbQueryInterface.modifies(procedure, strq) ==
                   STRING_SET{"main", "complicate"});
  } // namespace UnitTesting

  TEST_METHOD(ModifiesProcedureAndVariable) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(
        pkbQueryInterface.modifies(procedure, variable) ==
        STRING_PAIRS(
            STRING_VECTOR{"aux", "aux", "main", "main", "main", "main", "main",
                          "main", "complicate", "complicate"},
            STRING_VECTOR{"y", "x", "x", "r", "m", "y", "q", "t", "q", "t"}));
  } // namespace UnitTesting

  TEST_METHOD(ModifiesProcedureAndUnderscore) {
    PkbQueryInterface pkbQueryInterface = PkbQueryInterface(pkb);

    Assert::IsTrue(pkbQueryInterface.modifies(procedure, underscore) ==
                   STRING_SET{"aux", "main", "complicate"});
  } // namespace UnitTesting
};

} // namespace UnitTesting
