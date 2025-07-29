#ifndef PLANNER_H
#define PLANNER_H

#include "../common/common.h"
#include "../indexing/index.h"
#include "../operators/operators.h"
#include "../aggregation/aggregation.h"
#include "../join/join.h"
#include "../parser/parser.h"
#include <memory>
#include <unordered_map>

class QueryPlanner {
    IndexManager &index_mgr;
    std::unordered_map<std::string, Table> &tables;

public:
    QueryPlanner(IndexManager &im, std::unordered_map<std::string, Table> &t);
    std::shared_ptr<Operator> createQueryPlan(std::shared_ptr<ASTNode> ast);
    Value parseValue(const std::string &s);
};

#endif
