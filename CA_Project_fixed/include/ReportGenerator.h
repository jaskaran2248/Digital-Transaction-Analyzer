#pragma once

#include "Transaction.h"
#include "StatisticsEngine.h"
#include <string>
#include <vector>
#include <utility>
#include "Validator.h"

/**
 * @brief Generates textual reports from statistics.
 */
class ReportGenerator {
public:
    ReportGenerator(const std::vector<Transaction> &transactions);

    /**
     * Generate overall summary report.
     */
    std::string overallSummary() const;

    /**
     * Generate CSV report for a given metric.
     * @param header Title of the report
     * @param data Vector of pairs<key, value>
     */
    std::string csvReport(const std::string &header, const std::vector<std::pair<std::string, std::string>> &data) const;

    /**
     * Write report to a file.
     */
    void writeToFile(const std::string &filePath, const std::string &content) const;

private:
    const std::vector<Transaction> &transactions_;
};