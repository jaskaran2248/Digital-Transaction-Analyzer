#pragma once

#include "Transaction.h"
#include <vector>
#include <string>
#include <optional>

/**
 * @brief Manages a collection of transactions, providing CRUD and query operations.
 */
class TransactionManager {
public:
    TransactionManager();
    ~TransactionManager();

    /**
     * Load transactions from the CSV file specified at construction.
     */
    void loadFromFile();

    /**
     * Save current transactions to CSV file.
     */
    void saveToFile() const;

    /**
     * Add a new transaction. Generates and returns a unique ID.
     */
    int addTransaction(const Transaction &t);

    /**
     * Retrieve a transaction by ID. Returns std::nullopt if not found.
     */
    std::optional<Transaction> getById(int id) const;

    /**
     * Update a transaction by ID. Returns true if succeeded.
     */
    bool updateTransaction(int id, const Transaction &newData);

    /**
     * Delete a transaction by ID. Returns true if deleted.
     */
    bool deleteTransaction(int id);

    /**
     * Return all transactions.
     */
    const std::vector<Transaction>& all() const;

    /**
     * Filtering and sorting helper: returns indices of matching records.
     */
    std::vector<int> filter(const std::string &senderReceiverFilter = "",
                            const std::string &methodFilter = "",
                            const std::string &categoryFilter = "",
                            const std::string &statusFilter = "",
                            const std::string &minDate = "",
                            const std::string &maxDate = "",
                            double minAmount = -1,
                            double maxAmount = -1) const;

    /**
     * Sort by field name: "date", "amount", "id", "type".
     * order: true = ascending, false = descending.
     */
    void sortBy(const std::string &field, bool ascending = true);

    /**
     * Import from CSV file (separate from data file). Skips invalid rows.
     */
    void importFromFile(const std::string &csvPath);

    /**
     * Export to CSV file.
     */
    void exportToFile(const std::string &csvPath) const;

    /**
     * Generate a new unique transaction ID.
     */
    int generateUniqueId();

    /**
     * Total transaction count.
     */
    size_t count() const { return transactions_.size(); }

private:
    std::vector<Transaction> transactions_;
    std::string dataFilePath_;

    // Helper to find index by id.
    std::optional<size_t> findIndexById(int id) const;
};