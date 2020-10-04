#include "GUIWrapper.h"

#include <iostream>

// a default constructor
GUIWrapper::GUIWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  pkb = Pkb();
}

// read SIMPLE program from the given filename
std::string GUIWrapper::read(std::string filename) {
  std::ifstream program(filename);
  if (!(program.is_open())) {
    std::cout << "Unable to open SIMPLE program file." << std::endl;
    exit(1);
  }
  std::string input((std::istreambuf_iterator<char>(program)),
                    (std::istreambuf_iterator<char>()));
  return input;
}

// method for parsing the SIMPLE source
void GUIWrapper::parse(std::string filename) {
  try {
    Parser parser(read(filename), pkb.getTables());
    parser.parse();
    std::cout << "parsed " << filename << std::endl;
  } catch (ParseException &p) {
    std::cout << p.what() << std::endl;
  }
}

// method to evaluating a query
void GUIWrapper::evaluate(std::string query, std::list<std::string> &results) {
  std::cout << "query=  " << query << std::endl;
  try {
    Pql::evaluate(Pql::parse(Pql::lex(query)), pkb.getQueryInterface(),
                  results);
  } catch (...) {
    results.clear();
  }
  std::string separator;
  for (auto &result : results) {
    std::cout << separator << result;
    separator = ",";
  }
}
