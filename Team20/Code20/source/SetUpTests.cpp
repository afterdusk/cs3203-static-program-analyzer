#include "SetUpTests.h"

SetUpTests::SetUpTests(Pkb &pkb) {
  pkbTables = pkb.getTables();

  w.type = Pkb::StatementType::While;
  r.type = Pkb::StatementType::Read;
  a.type = Pkb::StatementType::Assign;
  c.type = Pkb::StatementType::Call;
  i.type = Pkb::StatementType::If;
  p.type = Pkb::StatementType::Print;
  stmt.type = Pkb::StatementType::None;

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

  l1 = 1;
  l2 = 2;
  l3 = 3;
  l4 = 4;
  l5 = 5;
  l6 = 6;
  l7 = 7;
  l8 = 8;
  l9 = 9;
  l10 = 10;
  l11 = 11;
  l12 = 12;
  l13 = 13;
  l14 = 14;
  l15 = 15;
  l16 = 16;
  l17 = 17;
  l18 = 18;
  l19 = 19;
  l20 = 20;
  l21 = 21;
  l22 = 22;
  l23 = 23;
  l24 = 24;
  l25 = 25;
  l26 = 26;
  l999 = 999;

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

  pkbTables->addFollow(l1, l2);
  pkbTables->addFollow(l2, l3);
  pkbTables->addFollow(l3, l6);
  pkbTables->addFollow(l4, l5);
  pkbTables->addFollow(l7, l8);
  pkbTables->addFollow(l8, l9);
  pkbTables->addFollow(l9, l10);
  pkbTables->addFollow(l10, l11);
  pkbTables->addFollow(l12, l13);
  pkbTables->addFollow(l13, l14);
  pkbTables->addFollow(l14, l25);
  pkbTables->addFollow(l15, l23);
  pkbTables->addFollow(l16, l17);
  pkbTables->addFollow(l17, l19);

  pkbTables->addParent(l4, l3);
  pkbTables->addParent(l5, l3);
  pkbTables->addParent(l15, l14);
  pkbTables->addParent(l23, l14);
  pkbTables->addParent(l16, l15);
  pkbTables->addParent(l17, l15);
  pkbTables->addParent(l19, l15);
  pkbTables->addParent(l22, l15);
  pkbTables->addParent(l18, l17);
  pkbTables->addParent(l20, l19);
  pkbTables->addParent(l21, l19);
  pkbTables->addParent(l24, l23);

  pkbTables->addStatementType(l1, Pkb::StatementType::Read);
  pkbTables->addStatementType(l2, Pkb::StatementType::Read);
  pkbTables->addStatementType(l3, Pkb::StatementType::While);
  pkbTables->addStatementType(l4, Pkb::StatementType::Assign);
  pkbTables->addStatementType(l5, Pkb::StatementType::Read);
  pkbTables->addStatementType(l6, Pkb::StatementType::Print);
  pkbTables->addStatementType(l7, Pkb::StatementType::Call);
  pkbTables->addStatementType(l8, Pkb::StatementType::Read);
  pkbTables->addStatementType(l9, Pkb::StatementType::Read);
  pkbTables->addStatementType(l10, Pkb::StatementType::Assign);
  pkbTables->addStatementType(l11, Pkb::StatementType::Call);
  pkbTables->addStatementType(l12, Pkb::StatementType::Read);
  pkbTables->addStatementType(l13, Pkb::StatementType::Read);
  pkbTables->addStatementType(l14, Pkb::StatementType::While);
  pkbTables->addStatementType(l15, Pkb::StatementType::If);
  pkbTables->addStatementType(l16, Pkb::StatementType::Assign);
  pkbTables->addStatementType(l17, Pkb::StatementType::While);
  pkbTables->addStatementType(l18, Pkb::StatementType::Assign);
  pkbTables->addStatementType(l19, Pkb::StatementType::If);
  pkbTables->addStatementType(l20, Pkb::StatementType::Assign);
  pkbTables->addStatementType(l21, Pkb::StatementType::Print);
  pkbTables->addStatementType(l22, Pkb::StatementType::Print);
  pkbTables->addStatementType(l23, Pkb::StatementType::While);
  pkbTables->addStatementType(l24, Pkb::StatementType::Assign);
  pkbTables->addStatementType(l25, Pkb::StatementType::Call);
  pkbTables->addStatementType(l26, Pkb::StatementType::Print);

  Pkb::VAR vx = "x";
  Pkb::VAR vy = "y";
  Pkb::VAR vr = "r";
  Pkb::VAR vm = "m";
  Pkb::VAR vq = "q";
  Pkb::VAR vt = "t";
  Pkb::VAR vk = "k";

  pkbTables->addVar(vx);
  pkbTables->addVar(vy);
  pkbTables->addVar(vr);
  pkbTables->addVar(vm);
  pkbTables->addVar(vq);
  pkbTables->addVar(vt);
  pkbTables->addVar(vk);

  Pkb::PROC paux = "aux";
  Pkb::PROC pmain = "main";
  Pkb::PROC pcomplicate = "complicate";
  Pkb::PROC pextra = "extra";

  pkbTables->addProc(paux);
  pkbTables->addProc(pmain);
  pkbTables->addProc(pcomplicate);
  pkbTables->addProc(pextra);

  pkbTables->addUses(l3, Pkb::VARS{vx, vy});
  pkbTables->addUses(l4, Pkb::VARS{vx, vy});
  pkbTables->addUses(l6, Pkb::VARS{vx});
  pkbTables->addUses(l7, paux);
  pkbTables->addUses(l10, Pkb::VARS{vx, vr});
  pkbTables->addUses(l11, pcomplicate);
  pkbTables->addUses(l14, Pkb::VARS{vq, vt});
  pkbTables->addUses(l15, Pkb::VARS{vq, vt});
  pkbTables->addUses(l16, Pkb::VARS{vt});
  pkbTables->addUses(l17, Pkb::VARS{vq, vt});
  pkbTables->addUses(l18, Pkb::VARS{vt});
  pkbTables->addUses(l19, Pkb::VARS{vq});
  pkbTables->addUses(l20, Pkb::VARS{vq});
  pkbTables->addUses(l21, Pkb::VARS{vq});
  pkbTables->addUses(l22, Pkb::VARS{vt});
  pkbTables->addUses(l23, Pkb::VARS{vq});
  pkbTables->addUses(l24, Pkb::VARS{vq});
  pkbTables->addUses(l25, pextra);
  pkbTables->addUses(l26, Pkb::VARS{vk});

  pkbTables->addUsesProc(paux, Pkb::VARS{vy, vx});
  pkbTables->addUsesProc(pmain, Pkb::VARS{vx, vr, vy, vq, vt, vk});
  pkbTables->addUsesProc(pcomplicate, Pkb::VARS{vq, vt, vk});
  pkbTables->addUsesProc(pextra, Pkb::VARS{vk});

  pkbTables->addModifies(l1, Pkb::VARS{vx});
  pkbTables->addModifies(l2, Pkb::VARS{vy});
  pkbTables->addModifies(l3, Pkb::VARS{vy});
  pkbTables->addModifies(l4, Pkb::VARS{vy});
  pkbTables->addModifies(l5, Pkb::VARS{vy});
  pkbTables->addModifies(l7, paux);
  pkbTables->addModifies(l8, Pkb::VARS{vx});
  pkbTables->addModifies(l9, Pkb::VARS{vr});
  pkbTables->addModifies(l10, Pkb::VARS{vm});
  pkbTables->addModifies(l11, pcomplicate);
  pkbTables->addModifies(l12, Pkb::VARS{vq});
  pkbTables->addModifies(l13, Pkb::VARS{vt});
  pkbTables->addModifies(l14, Pkb::VARS{vq, vt});
  pkbTables->addModifies(l15, Pkb::VARS{vq, vt});
  pkbTables->addModifies(l16, Pkb::VARS{vt});
  pkbTables->addModifies(l17, Pkb::VARS{vt});
  pkbTables->addModifies(l18, Pkb::VARS{vt});
  pkbTables->addModifies(l19, Pkb::VARS{vq});
  pkbTables->addModifies(l20, Pkb::VARS{vq});
  pkbTables->addModifies(l23, Pkb::VARS{vq});
  pkbTables->addModifies(l24, Pkb::VARS{vq});
  pkbTables->addModifies(l25, pextra);

  pkbTables->addModifiesProc(paux, Pkb::VARS{vy, vx});
  pkbTables->addModifiesProc(pmain, Pkb::VARS{vx, vr, vm, vy, vq, vt});
  pkbTables->addModifiesProc(pcomplicate, Pkb::VARS{vq, vt});

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

  pkbTables->addAssignAst(l4, l4ast);
  pkbTables->addAssignAst(l10, l10ast);
  pkbTables->addAssignAst(l16, l16ast);
  pkbTables->addAssignAst(l18, l18ast);
  pkbTables->addAssignAst(l20, l20ast);
  pkbTables->addAssignAst(l24, l24ast);

  pkbTables->addConstant("1");
  pkbTables->addConstant("5");
  pkbTables->addConstant("11111111111111111111111111111111111111");
  pkbTables->addConstant("5");
  pkbTables->addConstant("0");

  pkbTables->deriveTables();
}
