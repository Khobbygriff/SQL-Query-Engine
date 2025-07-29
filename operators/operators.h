#ifndef OPERATORS_H
#define OPERATORS_H

#include "../common/common.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <memory>

class TableScanOperator : public Operator {
    Table &table;
    size_t current_row = 0;

public:
    TableScanOperator(Table &t);
    Row next() override;
    void reset() override;
};

class IndexScanOperator : public Operator {
    Table &table;
    std::vector<size_t> row_indices;
    size_t current_index = 0;

public:
    IndexScanOperator(Table &t, const std::vector<size_t> &indices);
    Row next() override;
    void reset() override;
};

class ProjectionOperator : public Operator {
    std::shared_ptr<Operator> input;
    std::vector<std::string> columns;

public:
    ProjectionOperator(std::shared_ptr<Operator> in, const std::vector<std::string> &cols);
    Row next() override;
};

class SelectionOperator : public Operator {
    std::shared_ptr<Operator> input;
    std::string condition_col;
    std::string condition_op;
    Value condition_val;

    bool evaluate(const Row &row);

public:
    SelectionOperator(std::shared_ptr<Operator> in, const std::string &col, const std::string &op, const Value &val);
    Row next() override;
};

#endif