#ifndef DATABASE_H
#define DATABASE_H

#include "parser.h"
#include "planner.h"
#include "transaction.h"
#include "indexing.h"
#include "engine.h"

class Database {
    std::unordered_map<std::string, Table> tables;
    IndexManager index_mgr;
    SQLParser parser;
    QueryPlanner planner;
    ExecutionEngine engine;
    TransactionManager txn_mgr;

public:
    Database();
    void createTable(const std::string &name, const std::vector<std::string> &columns);
    void insert(const std::string &table, const Row &row);
    void createIndex(const std::string &table, const std::string &column);
    void execute(const std::string &query);
    void beginTransaction();
    void commit();
    void rollback();
    bool inTransaction() const;
};

#endif