#include "PkbTableTransformers.h"
void PkbTableTransformers::addNext(PkbTables::NEXT_TABLE &nextTable,
                                   PkbTables::LINE_NO lineNo,
                                   PkbTables::NEXT next,
                                   PkbTables::StatementType statementType) {
  if ((next != PkbTables::LINE_NO()) &&
      (statementType != PkbTables::StatementType::If)) {
    if (!nextTable.insert({lineNo, {next}})) {
      // If key already mapped, then insert value into existing mapped
      // unordered_set.
      nextTable.map[lineNo].insert(next);
    }
  }
}

PkbTables::NEXT_TABLE PkbTableTransformers::deriveNextTable(
    PkbTables::FOLLOW_TABLE followTable,
    PkbTables::STATEMENT_TYPE_TABLE statementTypeTable) {
  // The immediate block that the Statement corresponding to the iterator is
  // in.
  enum class ContextBlock {
    Proc,
    Then,
    Else,
    While,
  };
  typedef bool ContextIsFirstNestedStatement;
  typedef std::tuple<ContextBlock, PkbTables::LINE_NO, PkbTables::LINE_NO,
                     ContextIsFirstNestedStatement>
      CONTEXT;

  PkbTables::NEXT_TABLE nextTable; // NextTable to be derived.
  // The stack of contexts that the Statement corresponding to the iterator is
  // in. May change just before the next iteration.
  std::stack<CONTEXT> contextStack = std::stack<CONTEXT>();
  // Start with the Proc context with a dummy line numbers.
  contextStack.push(CONTEXT(ContextBlock::Proc, PkbTables::LINE_NO(),
                            PkbTables::LINE_NO(), false));

  // Iterate through all Statements everywhere.
  std::vector<PkbTables::LINE_NO> lineNos = statementTypeTable.keys;
  for (auto it = lineNos.begin(); it != lineNos.end(); ++it) {
    PkbTables::LINE_NO lineNo = it[0]; // The current line number.
    PkbTables::StatementType statementType = statementTypeTable.map[lineNo];
    PkbTables::FOLLOW follow =
        followTable
            .map[lineNo]; // May not exist (may have the value: FOLLOW()).

    CONTEXT context = contextStack.top();
    ContextBlock contextBlock = std::get<0>(context);
    PkbTables::LINE_NO contextLineNoFirst = std::get<1>(context);
    PkbTables::LINE_NO contextLineNoSecond = std::get<2>(context);
    ContextIsFirstNestedStatement contextIsFirstNestedStatement =
        std::get<3>(context);
    if (contextIsFirstNestedStatement) {
      addNext(nextTable, contextLineNoFirst, lineNo,
              statementType); // Hack to always insert into
                              // nextTable.
      // Set contextIsFirstNestedStatement to false.
      contextStack.pop();
      contextStack.push(CONTEXT(contextBlock, contextLineNoFirst,
                                contextLineNoSecond, false));
    }

    if (follow != PkbTables::FOLLOW()) {
      /* `follow` exists.  */
      addNext(nextTable, lineNo, follow, statementType);

    } else {
      /* `follow` does not exist.
       * This means `lineNo` is the last Statement of the StatementList.
       * This means `lineNo` is either:
       * 1. the last Statement of a then-block.
       * 2. the last Statement of a else-block.
       * 3. the last Statement of a while-block.
       * 4. the last Statement of the procedure.
       */
      CONTEXT context = contextStack.top();
      ContextBlock contextBlock = std::get<0>(context);
      PkbTables::LINE_NO contextLineNoFirst = std::get<1>(context);
      PkbTables::LINE_NO contextLineNoSecond = std::get<2>(context);
      switch (contextBlock) {
      case ContextBlock::Then:
        addNext(nextTable, lineNo, contextLineNoSecond, statementType);
        // Change to the Else context and preserve the line numbers.
        contextStack.pop();
        contextStack.push(CONTEXT(ContextBlock::Else, contextLineNoFirst,
                                  contextLineNoSecond, true));
        break;

      case ContextBlock::Else:
        addNext(nextTable, lineNo, contextLineNoSecond, statementType);
        // Leave the context.
        contextStack.pop();
        break;

      case ContextBlock::While:
        addNext(nextTable, lineNo, contextLineNoSecond, statementType);
        // Leave the context.
        contextStack.pop();
        break;

      case ContextBlock::Proc:
        // Do nothing.
        break;
      }
    }

    switch (statementType) {
    case PkbTables::StatementType::If:
      /* `lineNo` is an if-statement.
       * Let the current iteration be i, let there be n Statements in the
       then-block, and let there be m Statements in the else-block.
       * The following are true:
       * Next(`lineNo` of iteration i, `lineNo` of iteration i+1)
       * Next(`lineNo` of iteration i, `lineNo` of iteration i+n+1)

       * The SIMPLE source looks like this (here we use tokens from the CSG
       for SIMPLE to give visual context):
       * 'if' `lineNo` of iteration i 'then' '{'
       *   `lineNo` of iteration i+1
       *   `lineNo` of iteration i+2
       *   ...
       *   `lineNo` of iteration i+n
       * '}' 'else' '{'
       *   `lineNo` of iteration i+n+1
       *   `lineNo` of iteration i+n+2
       *   ...
       *   `lineNo` of iteration i+n+m
       * '}'
       * `follow` of iteration i = `lineNo` of iteration i+n+m+1

       * where `follow` of iteration i may not exist.
       * If `follow` of iteration i exists, then the following are true:
       * Next(`lineNo` of iteration i+n, `lineNo` of iteration i+n+m+1)
       * Next(`lineNo` of iteration i+n+m, `lineNo` of iteration i+n+m+1)
       */
      /* Enter the Then context with `lineNo` as the first line number and
      `follow` as the second line number. If `follow` does not exist, then let
      it be the second line number of the previous context.
      */
      if (follow == PkbTables::FOLLOW()) {
        PkbTables::LINE_NO contextLineNoSecond = std::get<2>(context);
        follow = contextLineNoSecond;
      }
      contextStack.push(CONTEXT(ContextBlock::Then, lineNo, follow, true));
      break;

    case PkbTables::StatementType::While:
      /* `lineNo` is a while-statement.
       * Let the current iteration be i, and let there be n Statements in the
       while-block.
       * The following are true:
       * Next(`lineNo` of iteration i, `lineNo` of iteration i+1)

       * The SIMPLE source looks like this (here we use tokens from the CSG
       for SIMPLE to give visual context):
       * 'while' `lineNo` of iteration i '{'
       *   `lineNo` of iteration i+1
       *   `lineNo` of iteration i+2
       *   ...
       *   `lineNo` of iteration i+n
       * '}'
       * `follow` of iteration i = `lineNo` of iteration i+n+1

       * where `follow` of iteration i may not exist.
       * If `follow` of iteration i exists, then the following is true:
           * Next(`lineNo` of iteration i+n, `lineNo` of iteration i)
       */
      // Enter the While context with a `lineNo` as the first and second line
      // numbers.
      contextStack.push(CONTEXT(ContextBlock::While, lineNo, lineNo, true));
      break;
    }
  }

  // Terminate.
  return nextTable;
}

KeysTable<std::size_t, std::unordered_set<std::size_t>>
PkbTableTransformers::closeWarshall(
    KeysTable<std::size_t, std::unordered_set<std::size_t>> keysTable) {
  // Convert reachability matrix into a KeysTable.
  std::vector<std::vector<bool>> reachability =
      closeWarshallReachabilityMatrix(keysTable);
  KeysTable<std::size_t, std::unordered_set<std::size_t>> closed;
  std::size_t n = reachability.size();
  for (std::size_t i = 0; i < n; ++i) {
    // For all destination nodes,
    for (std::size_t j = 0; j < n; ++j) {
      if (reachability[i][j]) {
        if (!closed.insert({i, {j}})) {
          // If key already mapped, then insert value into existing mapped
          // unordered_set.
          closed.map[i].insert(j);
        }
      }
    }
  }
  return closed;
}

std::vector<std::vector<bool>>
PkbTableTransformers::closeWarshallReachabilityMatrix(
    KeysTable<std::size_t, std::unordered_set<std::size_t>> keysTable) {

  // Maximum value in keysTable
  std::size_t n =
      *(std::max_element(keysTable.keys.begin(), keysTable.keys.end()));
  for (std::size_t key : keysTable.keys) {
    n = std::max(n, *(std::max_element(keysTable.map[key].begin(),
                                       keysTable.map[key].end())));
  }

  std::vector<std::vector<bool>> reachability(n + 1,
                                              std::vector<bool>(n + 1, false));
  for (std::size_t key : keysTable.keys) {
    std::unordered_set<std::size_t> values = keysTable.map[key];
    for (std::size_t value : values) {
      reachability[key][value] = true;
    }
  }

  // For all intermediate nodes,
  for (std::size_t k = 0; k < n; ++k) {
    // For all source nodes,
    for (std::size_t i = 0; i < n; ++i) {
      // For all destination nodes,
      for (std::size_t j = 0; j < n; ++j) {
        reachability[i][j] =
            reachability[i][j] || (reachability[i][k] && reachability[k][j]);
      }
    }
  }

  return reachability;
}
