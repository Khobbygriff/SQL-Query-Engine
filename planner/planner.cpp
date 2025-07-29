#include "../planner/planner.h"
#include "../common/common.h"
#include "../operators/operators.h"
#include "../aggregation/aggregation.h"
#include "../indexing/index.h"
#include "../join/join.h"
#include "../parser/parser.h"
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <iostream>
using namespace std;

QueryPlanner::QueryPlanner(IndexManager &im, std::unordered_map<std::string, Table> &t)
    : index_mgr(im), tables(t) {}

std::shared_ptr<Operator> QueryPlanner::createQueryPlan(std::shared_ptr<ASTNode> ast) {
    std::string table_name = ast->children[1]->value;
    Table &table = tables.at(table_name);
    std::shared_ptr<Operator> scan_op = std::make_shared<TableScanOperator>(table);

    std::shared_ptr<Operator> join_op;
    if (ast->children.size() > 2 && ast->children[2]->type == NodeType::JOIN) {
        auto join = ast->children[2];
        std::string right_table = join->value;
        Table &right = tables.at(right_table);
        auto cond = join->children[0];
        std::string left_col = cond->children[0]->value;
        std::string right_col = cond->children[1]->value;
        join_op = std::make_shared<NestedLoopJoinOperator>(scan_op, std::make_shared<TableScanOperator>(right), left_col, right_col);
        scan_op = join_op;
    }

    size_t where_idx = ast->children.size();
    for (size_t idx = 2; idx < ast->children.size(); ++idx) {
        if (ast->children[idx]->type == NodeType::WHERE) {
            where_idx = idx;
            break;
        }
    }
    if (where_idx < ast->children.size()) {
        auto where = ast->children[where_idx];
        auto condition = where->children[0];
        std::string col = condition->children[0]->value;
        std::string op = condition->value;
        Value val = parseValue(condition->children[1]->value);

        std::vector<size_t> indices = index_mgr.lookup(table_name, col, val);
        if (!indices.empty()) {
            scan_op = std::make_shared<IndexScanOperator>(table, indices);
        } else {
            scan_op = std::make_shared<SelectionOperator>(scan_op, col, op, val);
        }
    }

    std::vector<std::string> columns;
    bool has_agg = false;
    std::string agg_func, agg_col;
    for (auto &col_node : ast->children[0]->children) {
        if (col_node->type == NodeType::AGGREGATE) {
            has_agg = true;
            agg_func = col_node->value;
            agg_col = col_node->children[0]->value;
        } else {
            columns.push_back(col_node->value);
        }
    }

    if (has_agg) {
        return std::make_shared<AggregateOperator>(scan_op, agg_func, agg_col);
    } else {
        return std::make_shared<ProjectionOperator>(scan_op, columns);
    }
}

Value QueryPlanner::parseValue(const std::string &s) {
    try {
        if (s.find('.') != std::string::npos) {
            return std::stod(s);
        }
        return std::stoi(s);
    } catch (...) {
        return s;
    }
}
