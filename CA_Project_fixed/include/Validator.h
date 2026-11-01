#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>

/**
 * @brief Simple input validation and normalization utilities.
 */
class Validator {
public:
    static bool isValidDateTime(const std::string &dt);
    static bool isValidAmount(const std::string &amountStr);
    static bool isValidTransactionType(const std::string &type);
    static bool isValidPaymentMethod(const std::string &method);
    static bool isValidCategory(const std::string &category);
    static bool isValidStatus(const std::string &status);
    static std::string formatPaiseToRupee(int64_t paise);

    // Normalizers (case-insensitive & synonym mapping)
    static std::string normalizeType(const std::string &type);
    static std::string normalizePaymentMethod(const std::string &method);
    static std::string normalizeCategory(const std::string &category);
    static std::string normalizeStatus(const std::string &status);
};
