#include <iostream>

#include "PQLEvaluator.h"

/* TODO: Implement entrypoint function
   1. Handle selection of unseen synonyms
   2. Call PKB API
   3. Convert PKB result to ClauseResult
 */
std::unordered_set<VALUE> evaluateParsedQuery(ParsedQuery pq) {
  return std::unordered_set<VALUE>();
}

ClauseResult::ClauseResult(SYMBOL synonym, std::vector<VALUE> values)
    : synonym(synonym), values(values){};

SYMBOL ClauseResult::getSynonym() { return synonym; }

std::vector<VALUE> ClauseResult::getValues() { return values; }

VALUE ClauseResult::getValueAt(int i) { return values[i]; }

int ClauseResult::getLength() { return values.size(); }

EvaluationTable::EvaluationTable(std::vector<SYMBOL> declaredSynonyms) {
  if (declaredSynonyms.size() == 0) {
    throw "Invalid: Cannot create an EvaluationTable with no synonyms";
  }

  synonyms = std::unordered_set<SYMBOL>(declaredSynonyms.begin(),
                                        declaredSynonyms.end());
  values = generateValuesMap(synonyms);
};

std::unordered_map<SYMBOL, std::vector<VALUE>>
EvaluationTable::generateValuesMap(std::unordered_set<SYMBOL> synonyms) {
  std::unordered_map<SYMBOL, std::vector<VALUE>> newValues;
  for (auto &synonym : synonyms) {
    newValues.insert({synonym, {}});
  }
  return newValues;
}

void EvaluationTable::add(CLAUSE_RESULTS clauseResults) {
  // Validate clause results
  if (clauseResults.size() == 0) {
    throw "Invalid: No clause results to add";
  }
  for (auto &clauseResult : clauseResults) {
    if (synonyms.find(clauseResult.getSynonym()) == synonyms.end()) {
      throw "Invalid: Clause result synonym mismatch";
    }
    if (clauseResult.getLength() != clauseResults[0].getLength()) {
      throw "Invalid: Clause result length mismatch";
    }
  }

  // Add results and terminate early if table is empty
  if (seen.empty()) {
    for (auto &clauseResult : clauseResults) {
      values[clauseResult.getSynonym()] = clauseResult.getValues();
      seen.insert(clauseResult.getSynonym());
    }
    rows = clauseResults[0].getLength();
    return;
  }

  // Sort clause results into seen and unseen
  std::vector<ClauseResult *> seenClauseResults;
  std::vector<ClauseResult *> unseenClauseResults;
  for (auto &clauseResult : clauseResults) {
    if (seen.find(clauseResult.getSynonym()) != seen.end()) {
      seenClauseResults.push_back(&clauseResult);
    } else {
      unseenClauseResults.push_back(&clauseResult);
    }
  }

  // Transient table that will store new values
  std::unordered_map<SYMBOL, std::vector<VALUE>> newValues =
      generateValuesMap(synonyms);
  int newRows = 0;

  // Iterate over each result, each existing row in the table
  for (int resultIndex = 0; resultIndex < clauseResults[0].getLength();
       resultIndex++) {
    for (int valuesIndex = 0; valuesIndex < rowCount(); valuesIndex++) {
      // Check if values of seen columns match
      bool isMatch = true;
      for (auto &seenClauseResult : seenClauseResults) {
        std::vector<VALUE> seenColumn = values[seenClauseResult->getSynonym()];
        if (seenColumn[valuesIndex] !=
            seenClauseResult->getValueAt(resultIndex)) {
          isMatch = false;
          break;
        }
      }
      // Push cross product into new table if seen columns match
      if (isMatch) {
        newRows += 1;
        for (auto &synonym : seen) {
          newValues[synonym].push_back(values[synonym][valuesIndex]);
        }
        for (auto &unseenClauseResult : unseenClauseResults) {
          newValues[unseenClauseResult->getSynonym()].push_back(
              unseenClauseResult->getValueAt(resultIndex));
        }
      }
    }
  }

  // Mark all synonyms in this batch of results as seen
  for (auto &clauseResult : clauseResults) {
    seen.insert(clauseResult.getSynonym());
  }

  // Complete operation by replacing the values table
  values = newValues;
  rows = newRows;
}

std::unordered_set<VALUE> EvaluationTable::select(SYMBOL synonym) {
  std::vector<VALUE> &selected = values[synonym];
  return std::unordered_set<VALUE>(selected.begin(), selected.end());
}

std::vector<VALUE> EvaluationTable::selectColumn(SYMBOL synonym) {
  return values[synonym];
}

int EvaluationTable::rowCount() { return rows; }
