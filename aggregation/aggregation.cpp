#include "aggregation.h"

AggregateOperator::AggregateOperator(std::shared_ptr<Operator> in, const std::string &func, const std::string &col)
    : input(in), agg_function(func), column(col) {}

Row AggregateOperator::next() {
    if (done) return {};

    double sum = 0;
    int count = 0;

    for (Row row = input->next(); !row.empty(); row = input->next()) {
        if (!row.count(column)) {
            size_t dot = column.find('.');
            if (dot != std::string::npos) {
                std::string unqualified = column.substr(dot + 1);
                if (row.count(unqualified)) {
                    if (agg_function == "SUM" || agg_function == "AVG") {
                        if (std::holds_alternative<double>(row.at(unqualified))) {
                            sum += std::get<double>(row.at(unqualified));
                        } else if (std::holds_alternative<int>(row.at(unqualified))) {
                            sum += std::get<int>(row.at(unqualified));
                        }
                    }
                    count++;
                    continue;
                }
            }
            continue;
        }
        if (agg_function == "SUM" || agg_function == "AVG") {
            if (std::holds_alternative<double>(row.at(column))) {
                sum += std::get<double>(row.at(column));
            } else if (std::holds_alternative<int>(row.at(column))) {
                sum += std::get<int>(row.at(column));
            }
        }
        count++;
    }

    Row result;
    if (agg_function == "COUNT") {
        result[agg_function] = count;
    } else if (agg_function == "SUM") {
        result[agg_function] = sum;
    } else if (agg_function == "AVG") {
        result[agg_function] = (count > 0) ? sum / count : 0;
    }

    done = true;
    return result;
}

void AggregateOperator::reset() {
    input->reset();
    done = false;
}
