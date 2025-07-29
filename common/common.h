#ifndef COMMON_H
#define COMMON_H

#include <variant>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

using Value = std::variant<int, double, std::string>;
using Row = std::unordered_map<std::string, Value>;
using Table = std::vector<Row>;

struct TableSchema {
    std::string name;
    std::vector<std::string> columns;
    std::unordered_map<std::string, std::string> column_types; // "INT", "DOUBLE", "STRING"
};

class Operator {
public:
    virtual Row next() = 0;
    virtual void reset() {}
    virtual ~Operator() = default;
};

#endif