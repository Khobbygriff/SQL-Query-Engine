#include "database.h"

Database::Database() : planner(index_mgr, tables), txn_mgr(tables) {}

void Database::createTable(const std::string &name, const std::vector<std::string> &columns) {
    tables[name] = {};
}

void Database::insert(const std::string &table, const Row &row) {
    txn_mgr.insert(table, row);
}

void Database::createIndex(const std::string &table, const std::string &column) {
    index_mgr.createIndex(table, column, tables[table]);
}

void Database::execute(const std::string &query) {
    std::shared_ptr<ASTNode> ast = parser.parse(query);
    std::shared_ptr<Operator> plan = planner.createQueryPlan(ast);
    engine.execute(plan);
}

void Database::beginTransaction() { txn_mgr.begin(); }
void Database::commit() { txn_mgr.commit(); }
void Database::rollback() { txn_mgr.rollback(); }
bool Database::inTransaction() const { return txn_mgr.active(); }