#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "../common/common.h"

class TransactionManager {
    bool in_transaction = false;
    std::vector<std::pair<std::string, Row>> pending_inserts;
    std::unordered_map<std::string, Table> &tables;

public:
    TransactionManager(std::unordered_map<std::string, Table> &tables_);
    void begin();
    void commit();
    void rollback();
    void insert(const std::string &table, const Row &row);
    bool active() const;
};

#endif
