#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <fstream>
#include <iostream>
#include <list>
#include <string>
// include your other headers here
#include "AbstractWrapper.h"
#include "PQL.h"
#include "PQLEvaluator.h"
#include "Parser.h"
#include "PkbTables.h"

class TestWrapper : public AbstractWrapper {
private:
  PkbTables pkbTables;

  // method that read program fron the given file
  virtual std::string read(std::string filename);

public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);

  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string> &results);
};

#endif
