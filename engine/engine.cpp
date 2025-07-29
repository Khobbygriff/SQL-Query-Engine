#include "engine.h"
#include <iostream>

void ExecutionEngine::execute(std::shared_ptr<Operator> root) {
    for (Row row = root->next(); !row.empty(); row = root->next()) {
        for (const auto &[col, val] : row) {
            if (std::holds_alternative<int>(val)) {
                std::cout << col << ": " << std::get<int>(val) << "\t";
            } else if (std::holds_alternative<double>(val)) {
                std::cout << col << ": " << std::get<double>(val) << "\t";
            } else if (std::holds_alternative<std::string>(val)) {
                std::cout << col << ": " << std::get<std::string>(val) << "\t";
            }
        }
        std::cout << std::endl;
    }
}