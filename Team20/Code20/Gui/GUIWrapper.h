#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include "Pkb.h"
#include "Pql.h"
#include "PqlEvaluator.h"
#include "SimpleParser.h"
#include <fstream>
#include <iostream>
#include <list>
#include <string>

class GUIWrapper {
private:
  Pkb pkb;

  // method that read program fron the given file
  virtual std::string read(std::string filename);

public:
  // default constructor
  GUIWrapper();

  // destructor
  ~GUIWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);

  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string> &results);
};

#endif
