#include "ReportGenerator.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

ReportGenerator::ReportGenerator(const std::vector<Transaction> &transactions)
    : transactions_(transactions) {}

std::string ReportGenerator::overallSummary() const {
    StatisticsEngine stats(transactions_);
    std::ostringstream oss;
    oss << "=== Overall Summary ===\n";
    oss << "Total Transactions: " << stats.totalTransactions() << '\n';
    oss << "Successful: " << stats.successfulTransactions() << '\n';
    oss << "Failed: " << stats.failedTransactions() << '\n';
    oss << "Pending: " << stats.pendingTransactions() << '\n';
    oss << "Refunded: " << stats.refundedTransactions() << '\n';
    oss << "Total Credit: " << Validator::formatPaiseToRupee(stats.totalCredit()) << '\n';
    oss << "Total Debit: " << Validator::formatPaiseToRupee(stats.totalDebit()) << '\n';
    oss << "Net Balance: " << Validator::formatPaiseToRupee(stats.netBalance()) << '\n';
    oss << "Average Amount: " << std::fixed << std::setprecision(2) << stats.averageTransactionAmount() << " INR\n";
    if (stats.totalTransactions() == 0) {
        oss << "No transactions recorded.\n";
        return oss.str();
    }
    const Transaction &highest = stats.highestTransaction();
    const Transaction &lowest = stats.lowestTransaction();
    oss << "Highest Transaction: ID " << highest.id << " Amount " << Validator::formatPaiseToRupee(highest.amountPaise) << '\n';
    oss << "Lowest Transaction: ID " << lowest.id << " Amount " << Validator::formatPaiseToRupee(lowest.amountPaise) << '\n';
    return oss.str();
}

std::string ReportGenerator::csvReport(const std::string &header, const std::vector<std::pair<std::string, std::string>> &data) const {
    std::ostringstream oss;
    oss << header << "\n";
    for (const auto &p : data) {
        oss << p.first << "," << p.second << "\n";
    }
    return oss.str();
}

void ReportGenerator::writeToFile(const std::string &filePath, const std::string &content) const {
    std::filesystem::path p(filePath);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }
    std::ofstream out(filePath);
    if (!out.is_open()) {
        std::cerr << "Error: Cannot write to report file: " << filePath << std::endl;
        return;
    }
    out << content;
}
