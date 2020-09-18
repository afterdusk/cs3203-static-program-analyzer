#include "SetUpTests.h"

SetUpTests::SetUpTests() {
  w.type = PkbTables::StatementType::WHILE;
  r.type = PkbTables::StatementType::READ;
  a.type = PkbTables::StatementType::ASSIGN;
  c.type = PkbTables::StatementType::CALL;
  i.type = PkbTables::StatementType::IF;
  p.type = PkbTables::StatementType::PRINT;
  stmt.type = PkbTables::StatementType::NONE;

  stry.name = "y";
  strx.name = "x";
  strr.name = "r";
  strq.name = "q";
  strt.name = "t";
  strk.name = "k";
  strm.name = "m";

  strpmain.name = "main";
  strpextra.name = "extra";
  strpcomplicate.name = "complicate";
  strpaux.name = "aux";

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

  pkbTables.addFollow(l1, l2);
  pkbTables.addFollow(l2, l3);
  pkbTables.addFollow(l3, l6);
  pkbTables.addFollow(l4, l5);
  pkbTables.addFollow(l7, l8);
  pkbTables.addFollow(l8, l9);
  pkbTables.addFollow(l9, l10);
  pkbTables.addFollow(l10, l11);
  pkbTables.addFollow(l12, l13);
  pkbTables.addFollow(l13, l14);
  pkbTables.addFollow(l14, l25);
  pkbTables.addFollow(l15, l23);
  pkbTables.addFollow(l16, l17);
  pkbTables.addFollow(l17, l19);

  pkbTables.addParent(l4, l3);
  pkbTables.addParent(l5, l3);
  pkbTables.addParent(l15, l14);
  pkbTables.addParent(l23, l14);
  pkbTables.addParent(l16, l15);
  pkbTables.addParent(l17, l15);
  pkbTables.addParent(l19, l15);
  pkbTables.addParent(l22, l15);
  pkbTables.addParent(l18, l17);
  pkbTables.addParent(l20, l19);
  pkbTables.addParent(l21, l19);
  pkbTables.addParent(l24, l23);

  pkbTables.addStatementType(l1, PkbTables::StatementType::READ);
  pkbTables.addStatementType(l2, PkbTables::StatementType::READ);
  pkbTables.addStatementType(l3, PkbTables::StatementType::WHILE);
  pkbTables.addStatementType(l4, PkbTables::StatementType::ASSIGN);
  pkbTables.addStatementType(l5, PkbTables::StatementType::READ);
  pkbTables.addStatementType(l6, PkbTables::StatementType::PRINT);
  pkbTables.addStatementType(l7, PkbTables::StatementType::CALL);
  pkbTables.addStatementType(l8, PkbTables::StatementType::READ);
  pkbTables.addStatementType(l9, PkbTables::StatementType::READ);
  pkbTables.addStatementType(l10, PkbTables::StatementType::ASSIGN);
  pkbTables.addStatementType(l11, PkbTables::StatementType::CALL);
  pkbTables.addStatementType(l12, PkbTables::StatementType::READ);
  pkbTables.addStatementType(l13, PkbTables::StatementType::READ);
  pkbTables.addStatementType(l14, PkbTables::StatementType::WHILE);
  pkbTables.addStatementType(l15, PkbTables::StatementType::IF);
  pkbTables.addStatementType(l16, PkbTables::StatementType::ASSIGN);
  pkbTables.addStatementType(l17, PkbTables::StatementType::WHILE);
  pkbTables.addStatementType(l18, PkbTables::StatementType::ASSIGN);
  pkbTables.addStatementType(l19, PkbTables::StatementType::IF);
  pkbTables.addStatementType(l20, PkbTables::StatementType::ASSIGN);
  pkbTables.addStatementType(l21, PkbTables::StatementType::PRINT);
  pkbTables.addStatementType(l22, PkbTables::StatementType::PRINT);
  pkbTables.addStatementType(l23, PkbTables::StatementType::WHILE);
  pkbTables.addStatementType(l24, PkbTables::StatementType::ASSIGN);
  pkbTables.addStatementType(l25, PkbTables::StatementType::CALL);
  pkbTables.addStatementType(l26, PkbTables::StatementType::PRINT);

  PkbTables::VAR_TABLE_INDEX vx = pkbTables.addVar("x");
  PkbTables::VAR_TABLE_INDEX vy = pkbTables.addVar("y");
  PkbTables::VAR_TABLE_INDEX vr = pkbTables.addVar("r");
  PkbTables::VAR_TABLE_INDEX vm = pkbTables.addVar("m");
  PkbTables::VAR_TABLE_INDEX vq = pkbTables.addVar("q");
  PkbTables::VAR_TABLE_INDEX vt = pkbTables.addVar("t");
  PkbTables::VAR_TABLE_INDEX vk = pkbTables.addVar("k");

  PkbTables::PROC_TABLE_INDEX paux = pkbTables.addProc("aux");
  PkbTables::PROC_TABLE_INDEX pmain = pkbTables.addProc("main");
  PkbTables::PROC_TABLE_INDEX pcomplicate = pkbTables.addProc("complicate");
  PkbTables::PROC_TABLE_INDEX pextra = pkbTables.addProc("extra");

  pkbTables.addUses(l3, PkbTables::VAR_TABLE_INDEXES{vx, vy});
  pkbTables.addUses(l4, PkbTables::VAR_TABLE_INDEXES{vx, vy});
  pkbTables.addUses(l6, PkbTables::VAR_TABLE_INDEXES{vx});
  pkbTables.addUses(l7, paux);
  pkbTables.addUses(l10, PkbTables::VAR_TABLE_INDEXES{vx, vr});
  pkbTables.addUses(l11, pcomplicate);
  pkbTables.addUses(l14, PkbTables::VAR_TABLE_INDEXES{vq, vt});
  pkbTables.addUses(l15, PkbTables::VAR_TABLE_INDEXES{vq, vt});
  pkbTables.addUses(l16, PkbTables::VAR_TABLE_INDEXES{vt});
  pkbTables.addUses(l17, PkbTables::VAR_TABLE_INDEXES{vq, vt});
  pkbTables.addUses(l18, PkbTables::VAR_TABLE_INDEXES{vt});
  pkbTables.addUses(l19, PkbTables::VAR_TABLE_INDEXES{vq});
  pkbTables.addUses(l20, PkbTables::VAR_TABLE_INDEXES{vq});
  pkbTables.addUses(l21, PkbTables::VAR_TABLE_INDEXES{vq});
  pkbTables.addUses(l22, PkbTables::VAR_TABLE_INDEXES{vt});
  pkbTables.addUses(l23, PkbTables::VAR_TABLE_INDEXES{vq});
  pkbTables.addUses(l24, PkbTables::VAR_TABLE_INDEXES{vq});
  pkbTables.addUses(l25, pextra);
  pkbTables.addUses(l26, PkbTables::VAR_TABLE_INDEXES{vk});

  pkbTables.addUsesProc(paux, PkbTables::VAR_TABLE_INDEXES{vy, vx});
  pkbTables.addUsesProc(pmain,
                        PkbTables::VAR_TABLE_INDEXES{vx, vr, vy, vq, vt, vk});
  pkbTables.addUsesProc(pcomplicate, PkbTables::VAR_TABLE_INDEXES{vq, vt, vk});
  pkbTables.addUsesProc(pextra, PkbTables::VAR_TABLE_INDEXES{vk});

  pkbTables.addModifies(l1, PkbTables::VAR_TABLE_INDEXES{vx});
  pkbTables.addModifies(l2, PkbTables::VAR_TABLE_INDEXES{vy});
  pkbTables.addModifies(l3, PkbTables::VAR_TABLE_INDEXES{vy});
  pkbTables.addModifies(l4, PkbTables::VAR_TABLE_INDEXES{vy});
  pkbTables.addModifies(l5, PkbTables::VAR_TABLE_INDEXES{vy});
  pkbTables.addModifies(l7, paux);
  pkbTables.addModifies(l8, PkbTables::VAR_TABLE_INDEXES{vx});
  pkbTables.addModifies(l9, PkbTables::VAR_TABLE_INDEXES{vr});
  pkbTables.addModifies(l10, PkbTables::VAR_TABLE_INDEXES{vm});
  pkbTables.addModifies(l11, pcomplicate);
  pkbTables.addModifies(l12, PkbTables::VAR_TABLE_INDEXES{vq});
  pkbTables.addModifies(l13, PkbTables::VAR_TABLE_INDEXES{vt});
  pkbTables.addModifies(l14, PkbTables::VAR_TABLE_INDEXES{vq, vt});
  pkbTables.addModifies(l15, PkbTables::VAR_TABLE_INDEXES{vq, vt});
  pkbTables.addModifies(l16, PkbTables::VAR_TABLE_INDEXES{vt});
  pkbTables.addModifies(l17, PkbTables::VAR_TABLE_INDEXES{vt});
  pkbTables.addModifies(l18, PkbTables::VAR_TABLE_INDEXES{vt});
  pkbTables.addModifies(l19, PkbTables::VAR_TABLE_INDEXES{vq});
  pkbTables.addModifies(l20, PkbTables::VAR_TABLE_INDEXES{vq});
  pkbTables.addModifies(l23, PkbTables::VAR_TABLE_INDEXES{vq});
  pkbTables.addModifies(l24, PkbTables::VAR_TABLE_INDEXES{vq});
  pkbTables.addModifies(l25, pextra);

  pkbTables.addModifiesProc(paux, PkbTables::VAR_TABLE_INDEXES{vy, vx});
  pkbTables.addModifiesProc(
      pmain, PkbTables::VAR_TABLE_INDEXES{vx, vr, vm, vy, vq, vt});
  pkbTables.addModifiesProc(pcomplicate, PkbTables::VAR_TABLE_INDEXES{vq, vt});

  TNode T3 = TNode(TNode::Op::Divide);
  T3.left = new TNode("y");
  T3.right = new TNode("x");
  l4ast = T3;

  TNode T6 = TNode(TNode::Op::Plus);
  T6.left = new TNode("x");
  T6.right = new TNode("r");
  l10ast = T6;

  TNode T9 = TNode(TNode::Op::Minus);
  T9.left = new TNode("t");
  T9.right = new TNode("1");
  l16ast = T9;

  TNode T12 = TNode(TNode::Op::Minus);
  T12.left = new TNode("t");
  T12.right = new TNode("1");
  l18ast = T12;

  TNode T15 = TNode(TNode::Op::Minus);
  T15.left = new TNode("q");
  T15.right = new TNode("1");
  l20ast = T15;

  TNode T18 = TNode(TNode::Op::Minus);
  T18.left = new TNode("q");
  T18.right = new TNode("1");
  l24ast = T18;

  pkbTables.addAssignAst(l4, l4ast);
  pkbTables.addAssignAst(l10, l10ast);
  pkbTables.addAssignAst(l16, l16ast);
  pkbTables.addAssignAst(l18, l18ast);
  pkbTables.addAssignAst(l20, l20ast);
  pkbTables.addAssignAst(l24, l24ast);

  pkbTables.addConstant("1");
  pkbTables.addConstant("5");
  pkbTables.addConstant("11111111111111111111111111111111111111");
  pkbTables.addConstant("5");
  pkbTables.addConstant("0");

  pkbQueryInterface = PkbQueryInterface(pkbTables);
}
