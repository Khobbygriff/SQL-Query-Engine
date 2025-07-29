#include "index.h"

void IndexManager::createIndex(const std::string &table, const std::string &column, Table &data) {
    Index idx;
    idx.column = column;
    for (size_t i = 0; i < data.size(); ++i) {
        idx.index[data[i][column]].push_back(i);
    }
    indexes[table + "." + column] = idx;
}

std::vector<size_t> IndexManager::lookup(const std::string &table, const std::string &column, const Value &value) {
    std::string key = table + "." + column;
    if (indexes.count(key)) {
        return indexes[key].index[value];
    }
    return {};
}
