#pragma once

#include "PkbQueryEntityTypes.h"
#include "PkbQueryInterface.h"
#include "PkbTables.h"

class SetUpTests {

public:
  PkbTables pkbTables;
  PkbQueryInterface pkbQueryInterface;

  Statement w, r, a, c, i, p, stmt;
  PkbTables::LINE_NO l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12, l13,
      l14, l15, l16, l17, l18, l19, l20, l21, l22, l23, l24, l25, l26, l999;
  LineNumber c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15,
      c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c999;
  String strx, stry, strr, strm, strq, strt, strk, strpmain, strpaux, strpextra,
      strpcomplicate;
  Variable variable;
  Procedure procedure;
  Underscore underscore;
  Constant constant;
  Stmt STMT;
  PkbTables::AST l4ast, l10ast, l16ast, l18ast, l20ast, l24ast;

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
  SetUpTests();
};
