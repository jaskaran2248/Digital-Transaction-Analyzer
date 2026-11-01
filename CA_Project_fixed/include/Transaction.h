#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <ctime>

/**
 * @file Transaction.h
 * @brief Data model for a single transaction.
 */

/**
 * @struct Transaction
 * @brief Represents a financial transaction record.
 *
 * Amounts are stored in integer paise (1 rupee = 100 paise) to
 * avoid floating point inaccuracies.
 */
struct Transaction {
    int id = 0;                  // Unique transaction ID
    std::string datetime;      // ISO 8601 format: YYYY-MM-DD HH:MM:SS
    std::string sender;
    std::string receiver;
    int64_t amountPaise = 0;      // Amount in paise
    std::string type;          // "Credit" or "Debit"
    std::string method;        // UPI, Bank Transfer, Debit Card, etc.
    std::string category;      // Shopping, Food, etc.
    std::string status;        // Successful, Pending, Failed, Refunded
    std::string refNumber;
    std::string description;

    /**
     * Convert amount to a formatted string (₹x.xx).
     * @return Formatted amount string.
     */
    std::string getAmountString() const;

    /**
     * Parse a CSV line into a Transaction. Expects fields in the order:
     * id,datetime,sender,receiver,amount, type, method, category, status, refNumber, description
     * Amount must be in rupees with optional decimal (.xx).
     * @param csvLine
     * @return Transaction instance.
     */
    static Transaction fromCSV(const std::string &csvLine);

    /**
     * Serialize the transaction to a CSV line.
     * @return CSV formatted string.
     */
    std::string toCSV() const;
};
