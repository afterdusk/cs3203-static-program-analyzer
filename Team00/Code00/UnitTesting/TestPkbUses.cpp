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
  PKB pkb;

  Statement w, r, a, c, i, p, stmt;
  LineNumber c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
      c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c999;
  String strx, stry, strr, strm, strq, strt, strk;
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

    c1.number = "1";
    c2.number = "2";
    c3.number = "3";
    c4.number = "4";
    c5.number = "5";
    c6.number = "6";
    c7.number = "7";
    c8.number = "8";
    c9.number = "9";
    c10.number = "10";
    c11.number = "11";
    c12.number = "12";
    c13.number = "13";
    c14.number = "14";
    c15.number = "15";
    c16.number = "16";
    c17.number = "17";
    c18.number = "18";
    c19.number = "19";
    c20.number = "20";
    c21.number = "21";
    c22.number = "22";
    c23.number = "23";
    c24.number = "24";
    c25.number = "25";
    c26.number = "26";
    c999.number = "999";

    pkb.addStatementType("1", StatementType::READ);
    pkb.addStatementType("2", StatementType::READ);
    pkb.addStatementType("3", StatementType::WHILE);
    pkb.addStatementType("4", StatementType::ASSIGN);
    pkb.addStatementType("5", StatementType::READ);
    pkb.addStatementType("6", StatementType::PRINT);
    pkb.addStatementType("7", StatementType::CALL);
    pkb.addStatementType("8", StatementType::READ);
    pkb.addStatementType("9", StatementType::READ);
    pkb.addStatementType("10", StatementType::ASSIGN);
    pkb.addStatementType("11", StatementType::CALL);
    pkb.addStatementType("12", StatementType::READ);
    pkb.addStatementType("13", StatementType::READ);
    pkb.addStatementType("14", StatementType::WHILE);
    pkb.addStatementType("15", StatementType::IF);
    pkb.addStatementType("16", StatementType::ASSIGN);
    pkb.addStatementType("17", StatementType::WHILE);
    pkb.addStatementType("18", StatementType::ASSIGN);
    pkb.addStatementType("19", StatementType::IF);
    pkb.addStatementType("20", StatementType::ASSIGN);
    pkb.addStatementType("21", StatementType::PRINT);
    pkb.addStatementType("22", StatementType::PRINT);
    pkb.addStatementType("23", StatementType::WHILE);
    pkb.addStatementType("24", StatementType::ASSIGN);
    pkb.addStatementType("25", StatementType::CALL);
    pkb.addStatementType("26", StatementType::PRINT);

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

    pkb.addUses("3", VAR_TABLE_INDEXES{vx, vy});
    pkb.addUses("4", VAR_TABLE_INDEXES{vx, vy});
    pkb.addUses("6", VAR_TABLE_INDEXES{vx});
    pkb.addUses("7", paux);
    pkb.addUses("10", VAR_TABLE_INDEXES{vx, vr});
    pkb.addUses("11", pcomplicate);
    pkb.addUses("14", VAR_TABLE_INDEXES{vq, vt});
    pkb.addUses("15", VAR_TABLE_INDEXES{vq, vt});
    pkb.addUses("16", VAR_TABLE_INDEXES{vt});
    pkb.addUses("17", VAR_TABLE_INDEXES{vq, vt});
    pkb.addUses("18", VAR_TABLE_INDEXES{vt});
    pkb.addUses("19", VAR_TABLE_INDEXES{vq});
    pkb.addUses("20", VAR_TABLE_INDEXES{vq});
    pkb.addUses("21", VAR_TABLE_INDEXES{vq});
    pkb.addUses("22", VAR_TABLE_INDEXES{vt});
    pkb.addUses("23", VAR_TABLE_INDEXES{vq});
    pkb.addUses("24", VAR_TABLE_INDEXES{vq});
    pkb.addUses("25", pextra);
    pkb.addUses("26", VAR_TABLE_INDEXES{vk});

    pkb.addUsesProc(paux, VAR_TABLE_INDEXES{vy, vx});
    pkb.addUsesProc(pmain, VAR_TABLE_INDEXES{vx, vr});
    pkb.addUsesProc(pcomplicate, VAR_TABLE_INDEXES{vq, vt});
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
};

} // namespace UnitTesting
