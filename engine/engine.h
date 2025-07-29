#ifndef ENGINE_H
#define ENGINE_H

#include "common.h"
#include <memory>

class ExecutionEngine {
public:
    void execute(std::shared_ptr<Operator> root);
};

#endif