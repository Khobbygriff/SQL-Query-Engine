#ifndef PARSER_H
#define PARSER_H

#include "../common/common.h"
#include <string>
#include <vector>
#include <memory>

enum class NodeType {
    SELECT, FROM, WHERE, JOIN, COLUMN, VALUE, CONDITION, AGGREGATE
};

struct ASTNode {
    NodeType type;
    std::vector<std::shared_ptr<ASTNode>> children;
    std::string value;
};

class SQLParser {
public:
    std::shared_ptr<ASTNode> parse(const std::string &sql);
private:
    std::vector<std::string> tokenize(const std::string &sql);
    std::shared_ptr<ASTNode> parseSelect(const std::vector<std::string> &tokens);
};

#endif
