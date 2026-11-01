#include "Transaction.h"
#include <vector>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <stdexcept>

std::string Transaction::getAmountString() const {
    std::ostringstream oss;
    oss << "₹" << std::fixed << std::setprecision(2) << (amountPaise / 100.0);
    return oss.str();
}

Transaction Transaction::fromCSV(const std::string &csvLine) {
    // Simple split by comma. Assumes no commas inside fields.
    std::string line = csvLine;
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
    std::vector<std::string> parts;
    std::string token;
    for (char c : line) {            // keeps empty fields, incl. a trailing one
        if (c == ',') { parts.push_back(token); token.clear(); }
        else token += c;
    }
    parts.push_back(token);
    if (parts.size() != 11) {
        throw std::runtime_error("CSV row has wrong number of fields: " + csvLine);
    }
    Transaction t;
    try {
        t.id = std::stoi(parts[0]);
    } catch (...) {
        throw std::runtime_error("Invalid ID in CSV: " + parts[0]);
    }
    t.datetime = parts[1];
    t.sender = parts[2];
    t.receiver = parts[3];
    // amount in rupees with optional decimal
    double amt;
    try { amt = std::stod(parts[4]); }
    catch (...) { throw std::runtime_error("Invalid amount in CSV: " + parts[4]); }
    t.amountPaise = static_cast<int64_t>(std::llround(amt * 100));
    t.type = parts[5];
    t.method = parts[6];
    t.category = parts[7];
    t.status = parts[8];
    t.refNumber = parts[9];
    t.description = parts[10];
    return t;
}

std::string Transaction::toCSV() const {
    std::ostringstream oss;
    oss << id << ',' << datetime << ',' << sender << ',' << receiver << ','
        << std::fixed << std::setprecision(2) << (amountPaise / 100.0) << ','
        << type << ',' << method << ',' << category << ',' << status << ','
        << refNumber << ',' << description;
    return oss.str();
}
