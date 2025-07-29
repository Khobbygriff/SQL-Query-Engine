#ifndef AGGREGATION_H
#define AGGREGATION_H

#include "common.h"
#include <memory>

class AggregateOperator : public Operator {
    std::shared_ptr<Operator> input;
    std::string agg_function;
    std::string column;
    bool done = false;

public:
    AggregateOperator(std::shared_ptr<Operator> in, const std::string &func, const std::string &col);
    Row next() override;
    void reset() override;
};

#endif
