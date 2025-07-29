#include "operators.h"
#include <stdexcept>

TableScanOperator::TableScanOperator(Table &t) : table(t) {}

Row TableScanOperator::next() {
    if (current_row < table.size()) {
        return table[current_row++];
    }
    return {};
}

void TableScanOperator::reset() {
    current_row = 0;
}

IndexScanOperator::IndexScanOperator(Table &t, const std::vector<size_t> &indices)
    : table(t), row_indices(indices) {}

Row IndexScanOperator::next() {
    if (current_index < row_indices.size()) {
        return table[row_indices[current_index++]];
    }
    return {};
}

void IndexScanOperator::reset() {
    current_index = 0;
}

ProjectionOperator::ProjectionOperator(std::shared_ptr<Operator> in, const std::vector<std::string> &cols)
    : input(in), columns(cols) {}

Row ProjectionOperator::next() {
    Row row = input->next();
    if (row.empty()) return {};

    Row result;
    for (const std::string &col : columns) {
        if (row.count(col)) {
            result[col] = row.at(col);
        } else {
            size_t dot = col.find('.');
            if (dot != std::string::npos) {
                std::string unqualified = col.substr(dot + 1);
                if (row.count(unqualified)) {
                    result[col] = row.at(unqualified);
                }
            }
        }
    }
    return result;
}

SelectionOperator::SelectionOperator(std::shared_ptr<Operator> in, const std::string &col, const std::string &op, const Value &val)
    : input(in), condition_col(col), condition_op(op), condition_val(val) {}

bool SelectionOperator::evaluate(const Row &row) {
    Value actual_val;
    if (row.count(condition_col)) {
        actual_val = row.at(condition_col);
    } else {
        size_t dot = condition_col.find('.');
        if (dot != std::string::npos) {
            std::string unqualified = condition_col.substr(dot + 1);
            actual_val = row.at(unqualified);
        } else {
            throw std::runtime_error("Column not found in selection: " + condition_col);
        }
    }

    if (condition_op == "=") {
        return actual_val == condition_val;
    } else if (condition_op == ">") {
        if (std::holds_alternative<double>(actual_val) && std::holds_alternative<double>(condition_val))
            return std::get<double>(actual_val) > std::get<double>(condition_val);
        if (std::holds_alternative<int>(actual_val) && std::holds_alternative<int>(condition_val))
            return std::get<int>(actual_val) > std::get<int>(condition_val);
        if (std::holds_alternative<double>(actual_val) && std::holds_alternative<int>(condition_val))
            return std::get<double>(actual_val) > std::get<int>(condition_val);
        if (std::holds_alternative<int>(actual_val) && std::holds_alternative<double>(condition_val))
            return std::get<int>(actual_val) > std::get<double>(condition_val);
    }
    return false;
}

Row SelectionOperator::next() {
    while (true) {
        Row row = input->next();
        if (row.empty()) return {};
        if (evaluate(row)) return row;
    }
}