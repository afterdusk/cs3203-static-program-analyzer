#include "SetUpTests.h"

SetUpTests::SetUpTests() {
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
  pkb.addModifiesProc(pmain, VAR_TABLE_INDEXES{vx, vr, vm});
  pkb.addModifiesProc(pcomplicate, VAR_TABLE_INDEXES{vq, vt});

  TNode T3 = TNode(Op::Divide);
  T3.left = new TNode("y");
  T3.right = new TNode("x");
  l4ast = T3;

  TNode T6 = TNode(Op::Plus);
  T6.left = new TNode("x");
  T6.right = new TNode("r");
  l10ast = T6;

  TNode T9 = TNode(Op::Minus);
  T9.left = new TNode("t");
  T9.right = new TNode("1");
  l16ast = T9;

  TNode T12 = TNode(Op::Minus);
  T12.left = new TNode("t");
  T12.right = new TNode("1");
  l18ast = T12;

  TNode T15 = TNode(Op::Minus);
  T15.left = new TNode("q");
  T15.right = new TNode("1");
  l20ast = T15;

  TNode T18 = TNode(Op::Minus);
  T18.left = new TNode("q");
  T18.right = new TNode("1");
  l24ast = T18;

  pkb.addAssignAst(l4, l4ast);
  pkb.addAssignAst(l10, l10ast);
  pkb.addAssignAst(l16, l16ast);
  pkb.addAssignAst(l18, l18ast);
  pkb.addAssignAst(l20, l20ast);
  pkb.addAssignAst(l24, l24ast);
  pkbQueryInterface = PkbQueryInterface(pkb);
}
