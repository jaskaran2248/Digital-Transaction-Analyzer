#include "StatisticsEngine.h"
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <limits>

StatisticsEngine::StatisticsEngine(const std::vector<Transaction> &transactions)
    : transactions_(transactions) {}

size_t StatisticsEngine::totalTransactions() const { return transactions_.size(); }

size_t StatisticsEngine::successfulTransactions() const {
    return std::count_if(transactions_.begin(), transactions_.end(), [](const Transaction &t){ return t.status == "Successful"; });
}

size_t StatisticsEngine::failedTransactions() const {
    return std::count_if(transactions_.begin(), transactions_.end(), [](const Transaction &t){ return t.status == "Failed"; });
}

size_t StatisticsEngine::pendingTransactions() const {
    return std::count_if(transactions_.begin(), transactions_.end(), [](const Transaction &t){ return t.status == "Pending"; });
}

size_t StatisticsEngine::refundedTransactions() const {
    return std::count_if(transactions_.begin(), transactions_.end(), [](const Transaction &t){ return t.status == "Refunded"; });
}

int64_t StatisticsEngine::totalCredit() const {
    return std::accumulate(transactions_.begin(), transactions_.end(), int64_t(0), [](int64_t acc, const Transaction &t){ return t.type == "Credit" ? acc + t.amountPaise : acc; });
}

int64_t StatisticsEngine::totalDebit() const {
    return std::accumulate(transactions_.begin(), transactions_.end(), int64_t(0), [](int64_t acc, const Transaction &t){ return t.type == "Debit" ? acc + t.amountPaise : acc; });
}

int64_t StatisticsEngine::netBalance() const { return totalCredit() - totalDebit(); }

double StatisticsEngine::averageTransactionAmount() const {
    if (transactions_.empty()) return 0.0;
    int64_t total = std::accumulate(transactions_.begin(), transactions_.end(), int64_t(0), [](int64_t acc, const Transaction &t){ return acc + t.amountPaise; });
    return (double)total / transactions_.size() / 100.0; // rupees
}

const Transaction& StatisticsEngine::highestTransaction() const {
    auto it = std::max_element(transactions_.begin(), transactions_.end(), [](const Transaction &a, const Transaction &b){ return a.amountPaise < b.amountPaise; });
    if (it == transactions_.end()) throw std::runtime_error("No transactions");
    return *it;
}

const Transaction& StatisticsEngine::lowestTransaction() const {
    auto it = std::min_element(transactions_.begin(), transactions_.end(), [](const Transaction &a, const Transaction &b){ return a.amountPaise < b.amountPaise; });
    if (it == transactions_.end()) throw std::runtime_error("No transactions");
    return *it;
}
