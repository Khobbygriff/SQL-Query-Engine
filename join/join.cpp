#include "join.h"
#include <stdexcept>

NestedLoopJoinOperator::NestedLoopJoinOperator(std::shared_ptr<Operator> l, std::shared_ptr<Operator> r, std::string lcol, std::string rcol)
    : left(l), right(r), left_col(lcol), right_col(rcol) {}

Value NestedLoopJoinOperator::getValue(Row &row, const std::string &col) {
    if (row.count(col)) return row.at(col);
    size_t dot = col.find('.');
    if (dot != std::string::npos) {
        std::string unqualified = col.substr(dot + 1);
        if (row.count(unqualified)) return row.at(unqualified);
    }
    throw std::runtime_error("Column not found in join: " + col);
}

Row NestedLoopJoinOperator::next() {
    while (true) {
        if (initial_pass || current_left.empty()) {
            current_left = left->next();
            if (current_left.empty()) return {};
            right->reset();
            initial_pass = false;
        }

        Row right_row = right->next();
        if (right_row.empty()) {
            initial_pass = true;
            continue;
        }

        Value left_val = getValue(current_left, left_col);
        Value right_val = getValue(right_row, right_col);

        if (left_val == right_val) {
            Row merged;
            for (auto &[k, v] : current_left) merged[k] = v;
            for (auto &[k, v] : right_row) merged[k] = v;
            return merged;
        }
    }
}

void NestedLoopJoinOperator::reset() {
    left->reset();
    right->reset();
    current_left = {};
    initial_pass = true;
}
