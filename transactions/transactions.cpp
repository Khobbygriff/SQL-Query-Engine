#include "transactions.h"

TransactionManager::TransactionManager(std::unordered_map<std::string, Table> &tables_)
    : tables(tables_) {}

void TransactionManager::begin() { in_transaction = true; }

void TransactionManager::commit() {
    for (auto &[table, row] : pending_inserts) {
        tables[table].push_back(row);
    }
    pending_inserts.clear();
    in_transaction = false;
}

void TransactionManager::rollback() {
    pending_inserts.clear();
    in_transaction = false;
}

void TransactionManager::insert(const std::string &table, const Row &row) {
    if (in_transaction) {
        pending_inserts.emplace_back(table, row);
    } else {
        tables[table].push_back(row);
    }
}

bool TransactionManager::active() const { return in_transaction; }
