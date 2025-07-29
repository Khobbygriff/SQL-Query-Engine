#ifndef PLANNER_H
#define PLANNER_H

#include "common.h"
#include "indexing.h"
#include "parser.h"

class QueryPlanner {
    IndexManager &index_mgr;
    std::unordered_map<std::string, Table> &tables;

public:
    QueryPlanner(IndexManager &im, std::unordered_map<std::string, Table> &t);
    std::shared_ptr<Operator> createQueryPlan(std::shared_ptr<ASTNode> ast);
    Value parseValue(const std::string &s);
};

#endif
