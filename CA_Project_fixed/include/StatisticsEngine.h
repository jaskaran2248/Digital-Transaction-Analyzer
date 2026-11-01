#pragma once

#include "Transaction.h"
#include <vector>
#include <stdexcept>

/**
 * @brief Provides statistical calculations over a transaction dataset.
 */
class StatisticsEngine {
public:
    StatisticsEngine(const std::vector<Transaction> &transactions);

    size_t totalTransactions() const;
    size_t successfulTransactions() const;
    size_t failedTransactions() const;
    size_t pendingTransactions() const;
    size_t refundedTransactions() const;
    int64_t totalCredit() const;
    int64_t totalDebit() const;
    int64_t netBalance() const;
    double averageTransactionAmount() const;
    const Transaction& highestTransaction() const;
    const Transaction& lowestTransaction() const;
    // Additional summary methods can be added as needed.

private:
    const std::vector<Transaction> &transactions_;
};