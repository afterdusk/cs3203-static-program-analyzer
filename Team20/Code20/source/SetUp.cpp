#include "SetUp.h"

SetUpTests::SetUpTests(Pkb &pkb, TestNumber testNumber) {
  pkbTables = pkb.getTables();

  w.type = Pkb::StatementType::While;
  r.type = Pkb::StatementType::Read;
  a.type = Pkb::StatementType::Assign;
  c.type = Pkb::StatementType::Call;
  i.type = Pkb::StatementType::If;
  p.type = Pkb::StatementType::Print;

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
  strpextratwo.name = "extratwo";
  strpextrathree.name = "extrathree";
  strpextrafour.name = "extrafour";

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
  l27 = 27;
  l28 = 28;
  l29 = 29;
  l30 = 30;
  l31 = 31;
  l32 = 32;
  l33 = 33;
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
  c27.number = l27;
  c28.number = l28;
  c29.number = l29;
  c30.number = l30;
  c31.number = l31;
  c999.number = l999;

  if (testNumber == TestNumber::A) {
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
    procedure extratwo {
    27  call extra;
    28  call aux;}
    procedure extrathree {
    29  call main;
    30  call extratwo;}
    procedure extrafour {
    31  call extrathree;
    32  call main;
    33  call extrathree;}
    */

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
    pkbTables->addStatementType(l27, Pkb::StatementType::Call);
    pkbTables->addStatementType(l28, Pkb::StatementType::Call);
    pkbTables->addStatementType(l29, Pkb::StatementType::Call);
    pkbTables->addStatementType(l30, Pkb::StatementType::Call);
    pkbTables->addStatementType(l31, Pkb::StatementType::Call);
    pkbTables->addStatementType(l32, Pkb::StatementType::Call);
    pkbTables->addStatementType(l33, Pkb::StatementType::Call);

    Pkb::VAR vx = "x", vy = "y", vr = "r", vm = "m", vq = "q", vt = "t",
             vk = "k";

    pkbTables->addVar(vx);
    pkbTables->addVar(vy);
    pkbTables->addVar(vr);
    pkbTables->addVar(vm);
    pkbTables->addVar(vq);
    pkbTables->addVar(vt);
    pkbTables->addVar(vk);

    Pkb::PROC paux = "aux", pmain = "main", pcomplicate = "complicate",
              pextra = "extra", pextratwo = "extratwo",
              pextrathree = "extrathree", pextrafour = "extrafour";

    pkbTables->addProc(paux);
    pkbTables->addProc(pmain);
    pkbTables->addProc(pcomplicate);
    pkbTables->addProc(pextra);
    pkbTables->addProc(pextratwo);
    pkbTables->addProc(pextrathree);
    pkbTables->addProc(pextrafour);

    pkbTables->addCall(pmain, paux);
    pkbTables->addCall(pmain, pcomplicate);
    pkbTables->addCall(pcomplicate, pextra);
    pkbTables->addCall(pextratwo, pextra);
    pkbTables->addCall(pextratwo, paux);
    pkbTables->addCall(pextrathree, pmain);
    pkbTables->addCall(pextrathree, pextratwo);
    pkbTables->addCall(pextrafour, pextrathree);
    pkbTables->addCall(pextrafour, pmain);
    pkbTables->addCall(pextrafour, pextrathree);

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
    pkbTables->addUses(l27, pextra);
    pkbTables->addUses(l28, paux);
    pkbTables->addUses(l29, pmain);
    pkbTables->addUses(l30, pextratwo);
    pkbTables->addUses(l31, pextrathree);
    pkbTables->addUses(l32, pmain);
    pkbTables->addUses(l33, pextrathree);

    pkbTables->addUsesProc(paux, Pkb::VARS{vy, vx});
    pkbTables->addUsesProc(pmain, Pkb::VARS{vx, vr, vy, vq, vt, vk});
    pkbTables->addUsesProc(pcomplicate, Pkb::VARS{vq, vt, vk});
    pkbTables->addUsesProc(pextra, Pkb::VARS{vk});
    pkbTables->addUsesProc(pextratwo, Pkb::VARS{vk, vy, vx});
    pkbTables->addUsesProc(pextrathree, Pkb::VARS{vk, vy, vx, vr, vq, vt, vk});
    pkbTables->addUsesProc(pextrafour, Pkb::VARS{vk, vy, vx, vr, vq, vt, vk});

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
    pkbTables->addModifies(l27, pextra);
    pkbTables->addModifies(l28, paux);
    pkbTables->addModifies(l29, pmain);
    pkbTables->addModifies(l30, pextratwo);
    pkbTables->addModifies(l31, pextrathree);
    pkbTables->addModifies(l32, pmain);
    pkbTables->addModifies(l33, pextrathree);

    pkbTables->addModifiesProc(paux, Pkb::VARS{vy, vx});
    pkbTables->addModifiesProc(pmain, Pkb::VARS{vx, vr, vm, vy, vq, vt});
    pkbTables->addModifiesProc(pcomplicate, Pkb::VARS{vq, vt});
    pkbTables->addModifiesProc(pextratwo, Pkb::VARS{vy, vx});
    pkbTables->addModifiesProc(pextrathree, Pkb::VARS{vy, vx, vr, vm, vq, vt});
    pkbTables->addModifiesProc(pextrafour, Pkb::VARS{vy, vx, vr, vm, vq, vt});

    pkbTables->addConditionVars(l3, Pkb::VARS{vy});
    pkbTables->addConditionVars(l14, Pkb::VARS{vq, vt});
    pkbTables->addConditionVars(l15, Pkb::VARS{vt});
    pkbTables->addConditionVars(l17, Pkb::VARS{vt, vq});
    pkbTables->addConditionVars(l19, Pkb::VARS{vq});
    pkbTables->addConditionVars(l23, Pkb::VARS{vq});

    Pkb::AST T3 = std::make_shared<TNode>(TNode::Op::Divide);
    T3->left = std::make_unique<TNode>(TNode("y"));
    T3->right = std::make_unique<TNode>(TNode("x"));
    l4ast = T3;

    Pkb::AST T6 = std::make_shared<TNode>(TNode::Op::Plus);
    T6->left = std::make_unique<TNode>(TNode("x"));
    T6->right = std::make_unique<TNode>(TNode("r"));
    l10ast = T6;

    Pkb::AST T9 = std::make_shared<TNode>(TNode::Op::Minus);
    T9->left = std::make_unique<TNode>(TNode("t"));
    T9->right = std::make_unique<TNode>(TNode("1"));
    l16ast = T9;

    Pkb::AST T12 = std::make_shared<TNode>(TNode::Op::Minus);
    T12->left = std::make_unique<TNode>(TNode("t"));
    T12->right = std::make_unique<TNode>(TNode("1"));
    l18ast = T12;

    Pkb::AST T15 = std::make_shared<TNode>(TNode::Op::Minus);
    T15->left = std::make_unique<TNode>(TNode("q"));
    T15->right = std::make_unique<TNode>(TNode("1"));
    l20ast = T15;

    Pkb::AST T18 = std::make_shared<TNode>(TNode::Op::Minus);
    T18->left = std::make_unique<TNode>(TNode("q"));
    T18->right = std::make_unique<TNode>(TNode("1"));
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

    pkbTables->addNext(l1, l2);
    pkbTables->addNext(l2, l3);
    pkbTables->addNext(l3, l4);
    pkbTables->addNext(l3, l6);
    pkbTables->addNext(l4, l5);
    pkbTables->addNext(l5, l3);
    pkbTables->addNext(l7, l8);
    pkbTables->addNext(l8, l9);
    pkbTables->addNext(l9, l10);
    pkbTables->addNext(l10, l11);
    pkbTables->addNext(l12, l13);
    pkbTables->addNext(l13, l14);
    pkbTables->addNext(l14, l15);
    pkbTables->addNext(l14, l25);
    pkbTables->addNext(l15, l16);
    pkbTables->addNext(l15, l22);
    pkbTables->addNext(l16, l17);
    pkbTables->addNext(l17, l18);
    pkbTables->addNext(l17, l19);
    pkbTables->addNext(l18, l17);
    pkbTables->addNext(l19, l20);
    pkbTables->addNext(l19, l21);
    pkbTables->addNext(l20, l23);
    pkbTables->addNext(l21, l23);
    pkbTables->addNext(l22, l23);
    pkbTables->addNext(l23, l24);
    pkbTables->addNext(l23, l14);
    pkbTables->addNext(l24, l23);

    pkbTables->addStatementProc(l1, paux);
    pkbTables->addStatementProc(l2, paux);
    pkbTables->addStatementProc(l3, paux);
    pkbTables->addStatementProc(l4, paux);
    pkbTables->addStatementProc(l5, paux);
    pkbTables->addStatementProc(l6, paux);
    pkbTables->addStatementProc(l7, pmain);
    pkbTables->addStatementProc(l8, pmain);
    pkbTables->addStatementProc(l9, pmain);
    pkbTables->addStatementProc(l10, pmain);
    pkbTables->addStatementProc(l11, pmain);
    pkbTables->addStatementProc(l12, pcomplicate);
    pkbTables->addStatementProc(l13, pcomplicate);
    pkbTables->addStatementProc(l14, pcomplicate);
    pkbTables->addStatementProc(l15, pcomplicate);
    pkbTables->addStatementProc(l16, pcomplicate);
    pkbTables->addStatementProc(l17, pcomplicate);
    pkbTables->addStatementProc(l18, pcomplicate);
    pkbTables->addStatementProc(l19, pcomplicate);
    pkbTables->addStatementProc(l20, pcomplicate);
    pkbTables->addStatementProc(l21, pcomplicate);
    pkbTables->addStatementProc(l22, pcomplicate);
    pkbTables->addStatementProc(l23, pcomplicate);
    pkbTables->addStatementProc(l24, pcomplicate);
    pkbTables->addStatementProc(l25, pcomplicate);
    pkbTables->addStatementProc(l26, pextra);
    pkbTables->addStatementProc(l27, pextratwo);
    pkbTables->addStatementProc(l28, pextratwo);
    pkbTables->addStatementProc(l29, pextrathree);
    pkbTables->addStatementProc(l30, pextrathree);
    pkbTables->addStatementProc(l31, pextrafour);
    pkbTables->addStatementProc(l32, pextrafour);
    pkbTables->addStatementProc(l33, pextrafour);

  } else if (testNumber == TestNumber::B) {
    /* To be tested: SIMPLE Program:
        procedure main {
    1     a = x;
    2     if (x == y) {
    3       x = y;
    4       while (x == y) {
    5         a = z; }
    6       a = x;
          } else {
    7       call aux; }
    8     a = z;
    9     a = z;
        }
        procedure aux {
    10    while (x == y) {
    11      if (x == y) {
    12        if (x == y) {
    13          print z;
              } else {
    14          while (x == y) {
    15            print z; }}
            } else {
    16        if (x == y) {
    17          while (x == y) {
    18            while (x == y) {
    19              print z; }}
              } else {
    20          read z; }}
    21      read z; }
        }
        procedure aux2 {
    22    if (x == y) {
    23      print z;
          } else {
    24      print z; }
        }
    */

    p0 = "main";
    p1 = "aux";
    p2 = "aux2";
    pkbTables->addProc(p0);
    pkbTables->addProc(p1);
    pkbTables->addProc(p2);

    pkbTables->addStatementProc(l1, p0);
    pkbTables->addStatementProc(l2, p0);
    pkbTables->addStatementProc(l3, p0);
    pkbTables->addStatementProc(l4, p0);
    pkbTables->addStatementProc(l5, p0);
    pkbTables->addStatementProc(l6, p0);
    pkbTables->addStatementProc(l7, p0);
    pkbTables->addStatementProc(l8, p0);
    pkbTables->addStatementProc(l9, p0);
    pkbTables->addStatementProc(l10, p1);
    pkbTables->addStatementProc(l11, p1);
    pkbTables->addStatementProc(l12, p1);
    pkbTables->addStatementProc(l13, p1);
    pkbTables->addStatementProc(l14, p1);
    pkbTables->addStatementProc(l15, p1);
    pkbTables->addStatementProc(l16, p1);
    pkbTables->addStatementProc(l17, p1);
    pkbTables->addStatementProc(l18, p1);
    pkbTables->addStatementProc(l19, p1);
    pkbTables->addStatementProc(l20, p1);
    pkbTables->addStatementProc(l21, p1);
    pkbTables->addStatementProc(l22, p2);
    pkbTables->addStatementProc(l23, p2);
    pkbTables->addStatementProc(l24, p2);

    pkbTables->addFollow(l1, l2);
    pkbTables->addFollow(l2, l8);
    pkbTables->addFollow(l3, l4);
    pkbTables->addFollow(l4, l6);
    pkbTables->addFollow(l8, l9);
    pkbTables->addFollow(l11, l21);

    pkbTables->addParent(l3, l2);
    pkbTables->addParent(l4, l2);
    pkbTables->addParent(l5, l4);
    pkbTables->addParent(l6, l2);
    pkbTables->addParent(l7, l2);
    pkbTables->addParent(l11, l10);
    pkbTables->addParent(l12, l11);
    pkbTables->addParent(l13, l12);
    pkbTables->addParent(l14, l12);
    pkbTables->addParent(l15, l14);
    pkbTables->addParent(l16, l11);
    pkbTables->addParent(l17, l16);
    pkbTables->addParent(l18, l17);
    pkbTables->addParent(l19, l18);
    pkbTables->addParent(l20, l16);
    pkbTables->addParent(l21, l10);
    pkbTables->addParent(l23, l22);
    pkbTables->addParent(l24, l22);

    pkbTables->addStatementType(l1, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l2, Pkb::StatementType::If);
    pkbTables->addStatementType(l3, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l4, Pkb::StatementType::While);
    pkbTables->addStatementType(l5, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l6, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l7, Pkb::StatementType::Call);
    pkbTables->addStatementType(l8, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l9, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l10, Pkb::StatementType::While);
    pkbTables->addStatementType(l11, Pkb::StatementType::If);
    pkbTables->addStatementType(l12, Pkb::StatementType::If);
    pkbTables->addStatementType(l13, Pkb::StatementType::Print);
    pkbTables->addStatementType(l14, Pkb::StatementType::While);
    pkbTables->addStatementType(l15, Pkb::StatementType::Print);
    pkbTables->addStatementType(l16, Pkb::StatementType::If);
    pkbTables->addStatementType(l17, Pkb::StatementType::While);
    pkbTables->addStatementType(l18, Pkb::StatementType::While);
    pkbTables->addStatementType(l19, Pkb::StatementType::Print);
    pkbTables->addStatementType(l20, Pkb::StatementType::Read);
    pkbTables->addStatementType(l21, Pkb::StatementType::Read);
    pkbTables->addStatementType(l22, Pkb::StatementType::If);
    pkbTables->addStatementType(l23, Pkb::StatementType::Print);
    pkbTables->addStatementType(l24, Pkb::StatementType::Print);
  } else if (testNumber == TestNumber::C) {
    /* Example
     procedure aux {
     1	y = q + q;
     2	call main;
     3	while (y != 0) {
     4		y = y / x;
     5		read y; }
     6	x = 1 + y;}
     procedure main {
     7	x = m + n;
     8	read x;
     9  print r;
     10 m = x + r;
     11 call complicate;}
     procedure complicate {
     12	x = q + 1 + r;
     13	y = m * x + c;
     14	while (q < t) {
     15		if (t % 2 == 0) {
     16			call extra;
     17			while (t > q + 1) {
     18				q = 2 * x - y;}
     19			if (q % 2 == 0) {
     20				q = q - 1 + t + y;}
                else {
     21             t = y + x - 1;}}
            else {
     22         print t;}
     23		while (q % 2 == 1) {
     24			q = q - 1;}}
     25	call extra;}
     procedure extra {
     26	a = k + c}
     */

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

    Pkb::VAR vx = "x", vy = "y", vr = "r", vm = "m", vq = "q", vt = "t",
             vk = "k", vn = "n", vc = "c", va = "a";
    Pkb::PROC paux = "aux", pmain = "main", pcomplicate = "complicate",
              pextra = "extra";

    pkbTables->addStatementType(l1, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l2, Pkb::StatementType::Call);
    pkbTables->addStatementType(l3, Pkb::StatementType::While);
    pkbTables->addStatementType(l4, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l5, Pkb::StatementType::Read);
    pkbTables->addStatementType(l6, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l7, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l8, Pkb::StatementType::Read);
    pkbTables->addStatementType(l9, Pkb::StatementType::Print);
    pkbTables->addStatementType(l10, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l11, Pkb::StatementType::Call);
    pkbTables->addStatementType(l12, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l13, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l14, Pkb::StatementType::While);
    pkbTables->addStatementType(l15, Pkb::StatementType::If);
    pkbTables->addStatementType(l16, Pkb::StatementType::Call);
    pkbTables->addStatementType(l17, Pkb::StatementType::While);
    pkbTables->addStatementType(l18, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l19, Pkb::StatementType::If);
    pkbTables->addStatementType(l20, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l21, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l22, Pkb::StatementType::Print);
    pkbTables->addStatementType(l23, Pkb::StatementType::While);
    pkbTables->addStatementType(l24, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l25, Pkb::StatementType::Call);
    pkbTables->addStatementType(l26, Pkb::StatementType::Assign);

    pkbTables->addStatementProc(l1, paux);
    pkbTables->addStatementProc(l2, paux);
    pkbTables->addStatementProc(l3, paux);
    pkbTables->addStatementProc(l4, paux);
    pkbTables->addStatementProc(l5, paux);
    pkbTables->addStatementProc(l6, paux);
    pkbTables->addStatementProc(l7, pmain);
    pkbTables->addStatementProc(l8, pmain);
    pkbTables->addStatementProc(l9, pmain);
    pkbTables->addStatementProc(l10, pmain);
    pkbTables->addStatementProc(l11, pmain);
    pkbTables->addStatementProc(l12, pcomplicate);
    pkbTables->addStatementProc(l13, pcomplicate);
    pkbTables->addStatementProc(l14, pcomplicate);
    pkbTables->addStatementProc(l15, pcomplicate);
    pkbTables->addStatementProc(l16, pcomplicate);
    pkbTables->addStatementProc(l17, pcomplicate);
    pkbTables->addStatementProc(l18, pcomplicate);
    pkbTables->addStatementProc(l19, pcomplicate);
    pkbTables->addStatementProc(l20, pcomplicate);
    pkbTables->addStatementProc(l21, pcomplicate);
    pkbTables->addStatementProc(l22, pcomplicate);
    pkbTables->addStatementProc(l23, pcomplicate);
    pkbTables->addStatementProc(l24, pcomplicate);
    pkbTables->addStatementProc(l25, pcomplicate);
    pkbTables->addStatementProc(l26, pextra);

    pkbTables->addNext(l1, l2);
    pkbTables->addNext(l2, l3);
    pkbTables->addNext(l3, l4);
    pkbTables->addNext(l3, l6);
    pkbTables->addNext(l4, l5);
    pkbTables->addNext(l5, l3);
    pkbTables->addNext(l7, l8);
    pkbTables->addNext(l8, l9);
    pkbTables->addNext(l9, l10);
    pkbTables->addNext(l10, l11);
    pkbTables->addNext(l12, l13);
    pkbTables->addNext(l13, l14);
    pkbTables->addNext(l14, l15);
    pkbTables->addNext(l14, l25);
    pkbTables->addNext(l15, l16);
    pkbTables->addNext(l15, l22);
    pkbTables->addNext(l16, l17);
    pkbTables->addNext(l17, l18);
    pkbTables->addNext(l17, l19);
    pkbTables->addNext(l18, l17);
    pkbTables->addNext(l19, l20);
    pkbTables->addNext(l19, l21);
    pkbTables->addNext(l20, l23);
    pkbTables->addNext(l21, l23);
    pkbTables->addNext(l22, l23);
    pkbTables->addNext(l23, l24);
    pkbTables->addNext(l23, l14);
    pkbTables->addNext(l24, l23);

    pkbTables->addVar(vx);
    pkbTables->addVar(vy);
    pkbTables->addVar(vr);
    pkbTables->addVar(vm);
    pkbTables->addVar(vq);
    pkbTables->addVar(vt);
    pkbTables->addVar(vk);
    pkbTables->addVar(vn);
    pkbTables->addVar(vc);
    pkbTables->addVar(va);

    pkbTables->addProc(paux);
    pkbTables->addProc(pmain);
    pkbTables->addProc(pcomplicate);
    pkbTables->addProc(pextra);

    pkbTables->addCall(paux, pmain);
    pkbTables->addCall(pmain, pcomplicate);
    pkbTables->addCall(pcomplicate, pextra);

    pkbTables->addUses(l1, Pkb::VARS{vq});
    pkbTables->addUses(l2, Pkb::PROC{pmain});
    pkbTables->addUses(l3, Pkb::VARS{vx, vy});
    pkbTables->addUses(l4, Pkb::VARS{vx, vy});
    pkbTables->addUses(l6, Pkb::VARS{vy});
    pkbTables->addUses(l7, Pkb::VARS{vm, vn});
    pkbTables->addUses(l9, Pkb::VARS{vr});
    pkbTables->addUses(l10, Pkb::VARS{vx, vr});
    pkbTables->addUses(l11, Pkb::PROC{pcomplicate});
    pkbTables->addUses(l12, Pkb::VARS{vq, vr});
    pkbTables->addUses(l13, Pkb::VARS{vx, vm, vc});
    pkbTables->addUses(l14, Pkb::VARS{vq, vt, vx, vy});
    pkbTables->addUses(l15, Pkb::VARS{vq, vt, vx, vy});
    pkbTables->addUses(l16, Pkb::PROC{pextra});
    pkbTables->addUses(l17, Pkb::VARS{vq, vt, vx, vy});
    pkbTables->addUses(l18, Pkb::VARS{vx, vy});
    pkbTables->addUses(l19, Pkb::VARS{vq, vt, vx, vy});
    pkbTables->addUses(l20, Pkb::VARS{vq, vt, vy});
    pkbTables->addUses(l21, Pkb::VARS{vx, vy});
    pkbTables->addUses(l22, Pkb::VARS{vt});
    pkbTables->addUses(l23, Pkb::VARS{vq});
    pkbTables->addUses(l24, Pkb::VARS{vq});
    pkbTables->addUses(l25, Pkb::PROC{pextra});
    pkbTables->addUses(l26, Pkb::VARS{vk, vc});

    pkbTables->addUsesProc(paux, Pkb::VARS{vq, vt, vk, vc, vm, vx, vy, vr, vn});
    pkbTables->addUsesProc(pmain,
                           Pkb::VARS{vq, vt, vk, vc, vm, vx, vy, vr, vn});
    pkbTables->addUsesProc(pcomplicate,
                           Pkb::VARS{vq, vt, vk, vc, vm, vx, vy, vr});
    pkbTables->addUsesProc(pextra, Pkb::VARS{vk, vc});

    pkbTables->addModifies(l1, Pkb::VARS{vy});
    pkbTables->addModifies(l2, Pkb::PROC{pmain});
    pkbTables->addModifies(l3, Pkb::VARS{vy});
    pkbTables->addModifies(l4, Pkb::VARS{vy});
    pkbTables->addModifies(l5, Pkb::VARS{vy});
    pkbTables->addModifies(l6, Pkb::VARS{vx});
    pkbTables->addModifies(l7, Pkb::VARS{vx});
    pkbTables->addModifies(l8, Pkb::VARS{vx});
    pkbTables->addModifies(l10, Pkb::VARS{vm});
    pkbTables->addModifies(l11, Pkb::PROC{pcomplicate});
    pkbTables->addModifies(l12, Pkb::VARS{vx});
    pkbTables->addModifies(l13, Pkb::VARS{vy});
    pkbTables->addModifies(l14, Pkb::VARS{vq, vt});
    pkbTables->addModifies(l15, Pkb::VARS{vq, vt});
    pkbTables->addModifies(l16, Pkb::PROC{pextra});
    pkbTables->addModifies(l17, Pkb::VARS{vq});
    pkbTables->addModifies(l18, Pkb::VARS{vq});
    pkbTables->addModifies(l19, Pkb::VARS{vq, vt});
    pkbTables->addModifies(l20, Pkb::VARS{vq});
    pkbTables->addModifies(l21, Pkb::VARS{vt});
    pkbTables->addModifies(l23, Pkb::VARS{vq});
    pkbTables->addModifies(l24, Pkb::VARS{vq});
    pkbTables->addModifies(l25, Pkb::PROC{pextra});
    pkbTables->addModifies(l26, Pkb::VARS{va});

    pkbTables->addModifiesProc(paux, Pkb::VARS{vq, vt, vy, vx, va, vm});
    pkbTables->addModifiesProc(pmain, Pkb::VARS{vq, vt, vy, vx, va, vm});
    pkbTables->addModifiesProc(pcomplicate, Pkb::VARS{vq, vt, vy, vx, va});
    pkbTables->addModifiesProc(pextra, Pkb::VARS{va});

    pkbTables->addStatementProc(l1, paux);
    pkbTables->addStatementProc(l2, paux);
    pkbTables->addStatementProc(l3, paux);
    pkbTables->addStatementProc(l4, paux);
    pkbTables->addStatementProc(l5, paux);
    pkbTables->addStatementProc(l6, paux);
    pkbTables->addStatementProc(l7, pmain);
    pkbTables->addStatementProc(l8, pmain);
    pkbTables->addStatementProc(l9, pmain);
    pkbTables->addStatementProc(l10, pmain);
    pkbTables->addStatementProc(l11, pmain);
    pkbTables->addStatementProc(l12, pcomplicate);
    pkbTables->addStatementProc(l13, pcomplicate);
    pkbTables->addStatementProc(l14, pcomplicate);
    pkbTables->addStatementProc(l15, pcomplicate);
    pkbTables->addStatementProc(l16, pcomplicate);
    pkbTables->addStatementProc(l17, pcomplicate);
    pkbTables->addStatementProc(l18, pcomplicate);
    pkbTables->addStatementProc(l19, pcomplicate);
    pkbTables->addStatementProc(l20, pcomplicate);
    pkbTables->addStatementProc(l21, pcomplicate);
    pkbTables->addStatementProc(l22, pcomplicate);
    pkbTables->addStatementProc(l23, pcomplicate);
    pkbTables->addStatementProc(l24, pcomplicate);
    pkbTables->addStatementProc(l25, pcomplicate);
    pkbTables->addStatementProc(l26, pextra);

  } else if (testNumber == TestNumber::D) {
    /* To be tested: SIMPLE Program:
    procedure main {
    1   call p1;
    2   call entry;
    3   call body;
    4   call exit;
    5   call p8;}

    procedure body {
    6    call entryBody;
    7    call p1;
    8    while (x == x) {
    9    	if (x == x) then {
    10    		call p2;
    11    		while (x == x) {
    12    			call p3;}
    13    		if (x == x) then {
    14    			call p4;} else {
    15    	                call p5;}} else {
    16                call p6;}
    17    	while (x == x) {
    18    		call p7;}}
    19  call exitBody;}

    20  procedure entryBody {x = x;}
    21  procedure exitBody {x = x;}
    22  procedure entry {x = x;}
    23  procedure exit {x = x;}
    24  procedure p1 {x = x;}
    25  procedure p2 {x = x;}
    26  procedure p3 {x = x;}
    27  procedure p4 {x = x;}
    28  procedure p5 {x = x;}
    29  procedure p6 {x = x;}
    30  procedure p7 {x = x;}
    31  procedure p8 {x = x;}
    */

    Pkb::PROC paux = "aux", pmain = "main", pcomplicate = "complicate",
              pextra = "extra", pentryBody = "entryBody", pbody = "body",
              pentry = "entry", pexit = "exit", p1 = "p1", p2 = "p2", p3 = "p3",
              p4 = "p4", p5 = "p5", p6 = "p6", p7 = "p7", p8 = "p8",
              pexitBody = "exitBody";

    pkbTables->addStatementProc(l1, pmain);
    pkbTables->addStatementProc(l2, pmain);
    pkbTables->addStatementProc(l3, pmain);
    pkbTables->addStatementProc(l4, pmain);
    pkbTables->addStatementProc(l5, pmain);
    pkbTables->addStatementProc(l6, pbody);
    pkbTables->addStatementProc(l7, pbody);
    pkbTables->addStatementProc(l8, pbody);
    pkbTables->addStatementProc(l9, pbody);
    pkbTables->addStatementProc(l10, pbody);
    pkbTables->addStatementProc(l11, pbody);
    pkbTables->addStatementProc(l12, pbody);
    pkbTables->addStatementProc(l13, pbody);
    pkbTables->addStatementProc(l14, pbody);
    pkbTables->addStatementProc(l15, pbody);
    pkbTables->addStatementProc(l16, pbody);
    pkbTables->addStatementProc(l17, pbody);
    pkbTables->addStatementProc(l18, pbody);
    pkbTables->addStatementProc(l19, pbody);
    pkbTables->addStatementProc(l20, pentryBody);
    pkbTables->addStatementProc(l21, pexitBody);
    pkbTables->addStatementProc(l22, pentry);
    pkbTables->addStatementProc(l23, pexit);
    pkbTables->addStatementProc(l24, p1);
    pkbTables->addStatementProc(l25, p2);
    pkbTables->addStatementProc(l26, p3);
    pkbTables->addStatementProc(l27, p4);
    pkbTables->addStatementProc(l28, p5);
    pkbTables->addStatementProc(l29, p6);
    pkbTables->addStatementProc(l30, p7);
    pkbTables->addStatementProc(l31, p8);

    pkbTables->addStatementType(l1, Pkb::StatementType::Call);
    pkbTables->addStatementType(l2, Pkb::StatementType::Call);
    pkbTables->addStatementType(l3, Pkb::StatementType::Call);
    pkbTables->addStatementType(l4, Pkb::StatementType::Call);
    pkbTables->addStatementType(l5, Pkb::StatementType::Call);
    pkbTables->addStatementType(l6, Pkb::StatementType::Call);
    pkbTables->addStatementType(l7, Pkb::StatementType::Call);
    pkbTables->addStatementType(l8, Pkb::StatementType::While);
    pkbTables->addStatementType(l9, Pkb::StatementType::If);
    pkbTables->addStatementType(l10, Pkb::StatementType::Call);
    pkbTables->addStatementType(l11, Pkb::StatementType::While);
    pkbTables->addStatementType(l12, Pkb::StatementType::Call);
    pkbTables->addStatementType(l13, Pkb::StatementType::If);
    pkbTables->addStatementType(l14, Pkb::StatementType::Call);
    pkbTables->addStatementType(l15, Pkb::StatementType::Call);
    pkbTables->addStatementType(l16, Pkb::StatementType::Call);
    pkbTables->addStatementType(l17, Pkb::StatementType::While);
    pkbTables->addStatementType(l18, Pkb::StatementType::Call);
    pkbTables->addStatementType(l19, Pkb::StatementType::Call);
    pkbTables->addStatementType(l20, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l21, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l22, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l23, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l24, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l25, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l26, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l27, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l28, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l29, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l30, Pkb::StatementType::Assign);
    pkbTables->addStatementType(l31, Pkb::StatementType::Assign);

    pkbTables->addNext(l1, l2);
    pkbTables->addNext(l2, l3);
    pkbTables->addNext(l3, l4);
    pkbTables->addNext(l4, l5);
    pkbTables->addNext(l6, l7);
    pkbTables->addNext(l7, l8);
    pkbTables->addNext(l8, l9);
    pkbTables->addNext(l8, l19);
    pkbTables->addNext(l9, l10);
    pkbTables->addNext(l9, l16);
    pkbTables->addNext(l10, l11);
    pkbTables->addNext(l11, l12);
    pkbTables->addNext(l11, l13);
    pkbTables->addNext(l12, l11);
    pkbTables->addNext(l13, l14);
    pkbTables->addNext(l13, l15);
    pkbTables->addNext(l14, l17);
    pkbTables->addNext(l15, l17);
    pkbTables->addNext(l16, l17);
    pkbTables->addNext(l17, l18);
    pkbTables->addNext(l17, l8);
    pkbTables->addNext(l18, l17);

    /* Same as addNext. */
    pkbTables->addNextBip(l1, l2);
    pkbTables->addNextBip(l2, l3);
    pkbTables->addNextBip(l3, l4);
    pkbTables->addNextBip(l4, l5);
    pkbTables->addNextBip(l6, l7);
    pkbTables->addNextBip(l7, l8);
    pkbTables->addNextBip(l8, l9);
    pkbTables->addNextBip(l8, l19);
    pkbTables->addNextBip(l9, l10);
    pkbTables->addNextBip(l9, l16);
    pkbTables->addNextBip(l10, l11);
    pkbTables->addNextBip(l11, l12);
    pkbTables->addNextBip(l11, l13);
    pkbTables->addNextBip(l12, l11);
    pkbTables->addNextBip(l13, l14);
    pkbTables->addNextBip(l13, l15);
    pkbTables->addNextBip(l14, l17);
    pkbTables->addNextBip(l15, l17);
    pkbTables->addNextBip(l16, l17);
    pkbTables->addNextBip(l17, l18);
    pkbTables->addNextBip(l17, l8);
    pkbTables->addNextBip(l18, l17);

    /* Between procedures. */
    pkbTables->addNextBip(l1, l24);
    pkbTables->addNextBip(l24, l2);

    pkbTables->addNextBip(l2, l22);
    pkbTables->addNextBip(l22, l3);

    pkbTables->addNextBip(l3, l6);

    pkbTables->addNextBip(l4, l23);
    pkbTables->addNextBip(l23, l5);

    pkbTables->addNextBip(l5, l31);

    pkbTables->addNextBip(l6, l20);
    pkbTables->addNextBip(l20, l7);

    pkbTables->addNextBip(l7, l24);
    pkbTables->addNextBip(l24, l8);

    pkbTables->addNextBip(l19, l21);
    pkbTables->addNextBip(l21, l20);

    pkbTables->addNextBip(l10, l25);
    pkbTables->addNextBip(l25, l11);

    pkbTables->addNextBip(l12, l26);
    pkbTables->addNextBip(l26, l13);

    pkbTables->addNextBip(l14, l27);
    pkbTables->addNextBip(l27, l15);

    pkbTables->addNextBip(l15, l28);
    pkbTables->addNextBip(l28, l16);

    pkbTables->addNextBip(l16, l29);
    pkbTables->addNextBip(l29, l17);

    pkbTables->addNextBip(l18, l30);
    pkbTables->addNextBip(l30, l19);

    Pkb::VAR vx = "x";
    pkbTables->addVar(vx);

    pmain = "main";
    pentry = "entry";
    pexit = "exit";
    pbody = "body";
    p1 = "p1";
    p2 = "p2";
    p3 = "p3";
    p4 = "p4";
    p5 = "p5";
    p6 = "p6";
    p7 = "p7";
    p8 = "p8";

    pkbTables->addProc(pentryBody);
    pkbTables->addProc(pexitBody);
    pkbTables->addProc(pmain);
    pkbTables->addProc(pentry);
    pkbTables->addProc(pexit);
    pkbTables->addProc(pbody);
    pkbTables->addProc(p1);
    pkbTables->addProc(p2);
    pkbTables->addProc(p3);
    pkbTables->addProc(p4);
    pkbTables->addProc(p5);
    pkbTables->addProc(p6);
    pkbTables->addProc(p7);
    pkbTables->addProc(p8);

    pkbTables->addCall(pmain, p1);
    pkbTables->addCall(pmain, pentry);
    pkbTables->addCall(pmain, pbody);
    pkbTables->addCall(pmain, pexit);
    pkbTables->addCall(pmain, p8);

    pkbTables->addCall(pbody, pentryBody);
    pkbTables->addCall(pbody, p1);
    pkbTables->addCall(pbody, p2);
    pkbTables->addCall(pbody, p3);
    pkbTables->addCall(pbody, p4);
    pkbTables->addCall(pbody, p5);
    pkbTables->addCall(pbody, p6);
    pkbTables->addCall(pbody, p7);
    pkbTables->addCall(pbody, pexitBody);

    pkbTables->addUses(l1, Pkb::PROC{p1});
    pkbTables->addUses(l2, Pkb::PROC{pentry});
    pkbTables->addUses(l3, Pkb::PROC{pbody});
    pkbTables->addUses(l4, Pkb::PROC{pexit});
    pkbTables->addUses(l5, Pkb::PROC{p8});
    pkbTables->addUses(l6, Pkb::PROC{pentryBody});
    pkbTables->addUses(l7, Pkb::PROC{p1});
    pkbTables->addUses(l8, Pkb::VARS{vx});
    pkbTables->addUses(l9, Pkb::VARS{vx});
    pkbTables->addUses(l10, Pkb::PROC{p2});
    pkbTables->addUses(l11, Pkb::VARS{vx});
    pkbTables->addUses(l12, Pkb::PROC{p3});
    pkbTables->addUses(l13, Pkb::VARS{vx});
    pkbTables->addUses(l14, Pkb::PROC{p4});
    pkbTables->addUses(l15, Pkb::PROC{p5});
    pkbTables->addUses(l16, Pkb::PROC{p6});
    pkbTables->addUses(l17, Pkb::VARS{vx});
    pkbTables->addUses(l18, Pkb::PROC{p7});
    pkbTables->addUses(l19, Pkb::PROC{pexitBody});
    pkbTables->addUses(l20, Pkb::VARS{vx});
    pkbTables->addUses(l21, Pkb::VARS{vx});
    pkbTables->addUses(l22, Pkb::VARS{vx});
    pkbTables->addUses(l23, Pkb::VARS{vx});
    pkbTables->addUses(l24, Pkb::VARS{vx});
    pkbTables->addUses(l25, Pkb::VARS{vx});
    pkbTables->addUses(l26, Pkb::VARS{vx});
    pkbTables->addUses(l27, Pkb::VARS{vx});
    pkbTables->addUses(l28, Pkb::VARS{vx});
    pkbTables->addUses(l29, Pkb::VARS{vx});
    pkbTables->addUses(l30, Pkb::VARS{vx});
    pkbTables->addUses(l31, Pkb::VARS{vx});

    pkbTables->addUsesProc(pentryBody, Pkb::VARS{vx});
    pkbTables->addUsesProc(pexitBody, Pkb::VARS{vx});
    pkbTables->addUsesProc(pmain, Pkb::VARS{vx});
    pkbTables->addUsesProc(pentry, Pkb::VARS{vx});
    pkbTables->addUsesProc(pexit, Pkb::VARS{vx});
    pkbTables->addUsesProc(pbody, Pkb::VARS{vx});
    pkbTables->addUsesProc(p1, Pkb::VARS{vx});
    pkbTables->addUsesProc(p2, Pkb::VARS{vx});
    pkbTables->addUsesProc(p3, Pkb::VARS{vx});
    pkbTables->addUsesProc(p4, Pkb::VARS{vx});
    pkbTables->addUsesProc(p5, Pkb::VARS{vx});
    pkbTables->addUsesProc(p6, Pkb::VARS{vx});
    pkbTables->addUsesProc(p7, Pkb::VARS{vx});
    pkbTables->addUsesProc(p8, Pkb::VARS{vx});

    /* Same as addUses */
    pkbTables->addModifies(l1, Pkb::PROC{p1});
    pkbTables->addModifies(l2, Pkb::PROC{pentry});
    pkbTables->addModifies(l3, Pkb::PROC{pbody});
    pkbTables->addModifies(l4, Pkb::PROC{pexit});
    pkbTables->addModifies(l5, Pkb::PROC{p8});
    pkbTables->addModifies(l6, Pkb::PROC{pentryBody});
    pkbTables->addModifies(l7, Pkb::PROC{p1});
    pkbTables->addModifies(l8, Pkb::VARS{vx});
    pkbTables->addModifies(l9, Pkb::VARS{vx});
    pkbTables->addModifies(l10, Pkb::PROC{p2});
    pkbTables->addModifies(l11, Pkb::VARS{vx});
    pkbTables->addModifies(l12, Pkb::PROC{p3});
    pkbTables->addModifies(l13, Pkb::VARS{vx});
    pkbTables->addModifies(l14, Pkb::PROC{p4});
    pkbTables->addModifies(l15, Pkb::PROC{p5});
    pkbTables->addModifies(l16, Pkb::PROC{p6});
    pkbTables->addModifies(l17, Pkb::VARS{vx});
    pkbTables->addModifies(l18, Pkb::PROC{p7});
    pkbTables->addModifies(l19, Pkb::PROC{pexitBody});
    pkbTables->addModifies(l20, Pkb::VARS{vx});
    pkbTables->addModifies(l21, Pkb::VARS{vx});
    pkbTables->addModifies(l22, Pkb::VARS{vx});
    pkbTables->addModifies(l23, Pkb::VARS{vx});
    pkbTables->addModifies(l24, Pkb::VARS{vx});
    pkbTables->addModifies(l25, Pkb::VARS{vx});
    pkbTables->addModifies(l26, Pkb::VARS{vx});
    pkbTables->addModifies(l27, Pkb::VARS{vx});
    pkbTables->addModifies(l28, Pkb::VARS{vx});
    pkbTables->addModifies(l29, Pkb::VARS{vx});
    pkbTables->addModifies(l30, Pkb::VARS{vx});
    pkbTables->addModifies(l31, Pkb::VARS{vx});

    /* Same as addUsesProc */
    pkbTables->addModifiesProc(pentryBody, Pkb::VARS{vx});
    pkbTables->addModifiesProc(pexitBody, Pkb::VARS{vx});
    pkbTables->addModifiesProc(pmain, Pkb::VARS{vx});
    pkbTables->addModifiesProc(pentry, Pkb::VARS{vx});
    pkbTables->addModifiesProc(pexit, Pkb::VARS{vx});
    pkbTables->addModifiesProc(pbody, Pkb::VARS{vx});
    pkbTables->addModifiesProc(p1, Pkb::VARS{vx});
    pkbTables->addModifiesProc(p2, Pkb::VARS{vx});
    pkbTables->addModifiesProc(p3, Pkb::VARS{vx});
    pkbTables->addModifiesProc(p4, Pkb::VARS{vx});
    pkbTables->addModifiesProc(p5, Pkb::VARS{vx});
    pkbTables->addModifiesProc(p6, Pkb::VARS{vx});
    pkbTables->addModifiesProc(p7, Pkb::VARS{vx});
    pkbTables->addModifiesProc(p8, Pkb::VARS{vx});
  }

  pkbTables->deriveTables();
}

SetUpDispatcherGraphTest::SetUpDispatcherGraphTest() {

  dg1 = DispatcherGraph();

  ParsedRelationship pr1{TokenType::FOLLOWS, PqlToken{TokenType::IF, "i"},
                         PqlToken{TokenType::STMT, "s"}};
  ClauseDispatcher *cd1 = ClauseDispatcher::FromRelationship(pr1, nullptr);
  dg1.addDispatcher(cd1);
  ParsedRelationship pr2{TokenType::FOLLOWS, PqlToken{TokenType::STMT, "s"},
                         PqlToken{TokenType::WHILE, "w"}};
  ClauseDispatcher *cd2 = ClauseDispatcher::FromRelationship(pr2, nullptr);
  dg1.addDispatcher(cd2);
  /// ====================================================================

  dg2 = DispatcherGraph();
  ParsedRelationship pr3{TokenType::PARENT, PqlToken{TokenType::IF, "i2"},
                         PqlToken{TokenType::STMT, "s2"}};
  ClauseDispatcher *cd3 = ClauseDispatcher::FromRelationship(pr3, nullptr);
  dg2.addDispatcher(cd3);

  ParsedRelationship pr4{TokenType::PARENT, PqlToken{TokenType::STMT, "s2"},
                         PqlToken{TokenType::WHILE, "w2"}};
  ClauseDispatcher *cd4 = ClauseDispatcher::FromRelationship(pr4, nullptr);
  dg2.addDispatcher(cd4);

  /// ====================================================================
  ParsedRelationship mergingRelationship{TokenType::NEXT,
                                         PqlToken{TokenType::STMT, "s"},
                                         PqlToken{TokenType::STMT, "s2"}};
  mergingCd = ClauseDispatcher::FromRelationship(mergingRelationship, nullptr);
  dg3.addDispatcher(cd1);
  dg3.addDispatcher(cd2);
  dg3.addDispatcher(mergingCd);
  dg3.addDispatcher(cd3);
  dg3.addDispatcher(cd4);

  // ====================================================================
  ParsedRelationship anotherMergingRelationship{
      TokenType::FOLLOWS, PqlToken{TokenType::STMT, "s"},
      PqlToken{TokenType::STMT, "s2"}};
  anotherMergingCd =
      ClauseDispatcher::FromRelationship(anotherMergingRelationship, nullptr);

  // ====================================================================
  ParsedRelationship unrelatedRelationship{TokenType::FOLLOWS,
                                           PqlToken{TokenType::STMT, "s3"},
                                           PqlToken{TokenType::STMT, "s2"}};
  noCommonSymbolsWithDg1Cd =
      ClauseDispatcher::FromRelationship(unrelatedRelationship, nullptr);
}
