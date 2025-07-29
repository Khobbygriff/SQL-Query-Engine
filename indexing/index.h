#ifndef INDEXING_H
#define INDEXING_H

#include "../common/common.h"

struct Index {
    std::string column;
    std::unordered_map<Value, std::vector<size_t>> index;
};

class IndexManager {
    std::unordered_map<std::string, Index> indexes;

public:
    void createIndex(const std::string &table, const std::string &column, Table &data);
    std::vector<size_t> lookup(const std::string &table, const std::string &column, const Value &value);
};

#endif
