#include "parser.h"
#include <sstream>
#include <stdexcept>

std::vector<std::string> SQLParser::tokenize(const std::string &sql) {
    std::vector<std::string> tokens;
    bool in_quotes = false;
    std::string current_token;

    for (char c : sql) {
        if (c == '\'') {
            in_quotes = !in_quotes;
            current_token += c;
        } else if (std::isspace(c) && !in_quotes) {
            if (!current_token.empty()) {
                tokens.push_back(current_token);
                current_token.clear();
            }
        } else if ((c == ',' || c == '=' || c == '>' || c == '(' || c == ')' || c == '.') && !in_quotes) {
            if (!current_token.empty()) {
                tokens.push_back(current_token);
                current_token.clear();
            }
            tokens.emplace_back(1, c);
        } else {
            current_token += c;
        }
    }

    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }

    return tokens;
}

std::shared_ptr<ASTNode> SQLParser::parse(const std::string &sql) {
    std::vector<std::string> tokens = tokenize(sql);
    if (tokens.empty() || tokens[0] != "SELECT") {
        throw std::runtime_error("Only SELECT queries are supported");
    }
    return parseSelect(tokens);
}

std::shared_ptr<ASTNode> SQLParser::parseSelect(const std::vector<std::string> &tokens) {
    auto root = std::make_shared<ASTNode>();
    root->type = NodeType::SELECT;
    size_t i = 1; // Skip SELECT

    auto columns = std::make_shared<ASTNode>();
    columns->type = NodeType::COLUMN;
    while (i < tokens.size() && tokens[i] != "FROM") {
        if (tokens[i] == ",") { i++; continue; }
        if (tokens[i] == "AVG" || tokens[i] == "SUM" || tokens[i] == "COUNT") {
            auto agg = std::make_shared<ASTNode>();
            agg->type = NodeType::AGGREGATE;
            agg->value = tokens[i];
            i += 2; // Skip '('
            agg->children.push_back(std::make_shared<ASTNode>(ASTNode{NodeType::COLUMN, {}, tokens[i]}));
            i += 2; // Skip ')'
            columns->children.push_back(agg);
        } else {
            std::string col_name = tokens[i];
            if (i + 2 < tokens.size() && tokens[i + 1] == "." && tokens[i + 2] != "FROM") {
                col_name += tokens[i + 1] + tokens[i + 2];
                i += 2;
            }
            auto col = std::make_shared<ASTNode>();
            col->type = NodeType::COLUMN;
            col->value = col_name;
            columns->children.push_back(col);
            i++;
        }
    }
    root->children.push_back(columns);

    if (i >= tokens.size() || tokens[i] != "FROM") {
        throw std::runtime_error("Expected FROM clause");
    }
    i++;
    
    auto from = std::make_shared<ASTNode>();
    from->type = NodeType::FROM;
    from->value = tokens[i++];
    root->children.push_back(from);

    if (i < tokens.size() && tokens[i] == "JOIN") {
        i++;
        auto join = std::make_shared<ASTNode>();
        join->type = NodeType::JOIN;
        join->value = tokens[i++];

        if (i < tokens.size() && tokens[i] == "ON") {
            i++;
            auto cond = std::make_shared<ASTNode>();
            cond->type = NodeType::CONDITION;
            cond->value = "=";

            std::string left_col = tokens[i];
            if (i + 2 < tokens.size() && tokens[i + 1] == "." && tokens[i + 2] != "=") {
                left_col += tokens[i + 1] + tokens[i + 2];
                i += 2;
            }
            i++;
            if (tokens[i] == "=") i++;

            std::string right_col = tokens[i];
            if (i + 2 < tokens.size() && tokens[i + 1] == "." && tokens[i + 2] != "WHERE") {
                right_col += tokens[i + 1] + tokens[i + 2];
                i += 2;
            }
            auto left_col_node = std::make_shared<ASTNode>();
            left_col_node->type = NodeType::COLUMN;
            left_col_node->value = left_col;
            auto right_col_node = std::make_shared<ASTNode>();
            right_col_node->type = NodeType::COLUMN;
            right_col_node->value = right_col;
            cond->children.push_back(left_col_node);
            cond->children.push_back(right_col_node);
            join->children.push_back(cond);
        }
        root->children.push_back(join);
        i++;
    }

    if (i < tokens.size() && tokens[i] == "WHERE") {
        i++;
        auto where = std::make_shared<ASTNode>();
        where->type = NodeType::WHERE;

        auto condition = std::make_shared<ASTNode>();
        condition->type = NodeType::CONDITION;
        condition->value = tokens[i + 1];

        auto left = std::make_shared<ASTNode>();
        left->type = NodeType::COLUMN;
        left->value = tokens[i];

        auto right = std::make_shared<ASTNode>();
        right->type = NodeType::VALUE;
        right->value = tokens[i + 2];

        condition->children.push_back(left);
        condition->children.push_back(right);
        where->children.push_back(condition);
        root->children.push_back(where);
    }

    return root;
}
