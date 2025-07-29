#ifndef JOIN_H
#define JOIN_H

#include "../common/common.h"
#include <memory>

class NestedLoopJoinOperator : public Operator {
    std::shared_ptr<Operator> left;
    std::shared_ptr<Operator> right;
    std::string left_col;
    std::string right_col;
    Row current_left;
    bool initial_pass = true;

    Value getValue(Row &row, const std::string &col);

public:
    NestedLoopJoinOperator(std::shared_ptr<Operator> l, std::shared_ptr<Operator> r, std::string lcol, std::string rcol);
    Row next() override;
    void reset() override;
};

#endif
