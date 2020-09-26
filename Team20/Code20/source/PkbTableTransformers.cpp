#include "PkbTableTransformers.h"

KeysTable<PkbTables::LINE_NO, PkbTables::VAR_TABLE_INDEXES>
PkbTableTransformers::transit(
    KeysTable<PkbTables::LINE_NO, std::variant<PkbTables::VAR_TABLE_INDEXES,
                                               PkbTables::PROC_TABLE_INDEX>>
        table,
    KeysTable<PkbTables::PROC_TABLE_INDEX, PkbTables::VAR_TABLE_INDEXES>
        procTable) {
  KeysTable<PkbTables::LINE_NO, PkbTables::VAR_TABLE_INDEXES> mapTransited;
  for (PkbTables::LINE_NO key : table.keys) {
    std::variant<PkbTables::VAR_TABLE_INDEXES, PkbTables::PROC_TABLE_INDEX>
        tableValue = table.map[key];
    PkbTables::VAR_TABLE_INDEXES value;
    if (std::holds_alternative<PkbTables::PROC_TABLE_INDEX>(tableValue)) {
      value = procTable.map[std::get<PkbTables::PROC_TABLE_INDEX>(tableValue)];
    } else if (std::holds_alternative<PkbTables::VAR_TABLE_INDEXES>(
                   tableValue)) {
      value = std::get<PkbTables::VAR_TABLE_INDEXES>(tableValue);
    }
    mapTransited.insert({key, value});
  }
  return mapTransited;
}
