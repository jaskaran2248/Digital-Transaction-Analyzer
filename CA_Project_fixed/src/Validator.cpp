#include "Validator.h"
#include <algorithm>
#include <vector>
#include <cctype>
#include <chrono>
#include <iomanip>
#include <sstream>

static std::string toLowerTrim(std::string s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
    for (char &c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

bool Validator::isValidDateTime(const std::string &dt) {
    if (dt.size() != 19) return false;
    std::istringstream ss(dt);
    std::tm tm = {};
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) return false;
    char extra;
    if (ss >> extra) return false;
    if (tm.tm_year < 0) return false;
    if (tm.tm_mon < 0 || tm.tm_mon > 11) return false;
    if (tm.tm_mday < 1 || tm.tm_mday > 31) return false;
    if (tm.tm_hour < 0 || tm.tm_hour > 23) return false;
    if (tm.tm_min < 0 || tm.tm_min > 59) return false;
    if (tm.tm_sec < 0 || tm.tm_sec > 60) return false;
    return true;
}

bool Validator::isValidAmount(const std::string &amountStr) {
    if (amountStr.empty()) return false;
    try {
        size_t idx = 0;
        double val = std::stod(amountStr, &idx);
        while (idx < amountStr.size() && std::isspace(static_cast<unsigned char>(amountStr[idx]))) {
            ++idx;
        }
        return idx == amountStr.size() && val >= 0;
    } catch (...) {
        return false;
    }
}

bool Validator::isValidTransactionType(const std::string &type) {
    return !normalizeType(type).empty();
}

bool Validator::isValidPaymentMethod(const std::string &method) {
    return !normalizePaymentMethod(method).empty();
}

bool Validator::isValidCategory(const std::string &category) {
    return !normalizeCategory(category).empty();
}

bool Validator::isValidStatus(const std::string &status) {
    return !normalizeStatus(status).empty();
}

std::string Validator::formatPaiseToRupee(int64_t paise) {
    std::ostringstream oss;
    oss << "₹" << std::fixed << std::setprecision(2) << (paise / 100.0);
    return oss.str();
}

std::string Validator::normalizeType(const std::string &type) {
    std::string s = toLowerTrim(type);
    if (s == "credit" || s == "cr" || s == "c") return "Credit";
    if (s == "debit" || s == "dr" || s == "d") return "Debit";
    return "";
}

std::string Validator::normalizePaymentMethod(const std::string &method) {
    std::string s = toLowerTrim(method);
    if (s.empty()) return "UPI";
    if (s == "upi" || s == "gpay" || s == "paytm" || s == "phonepe" || s == "google pay" || s == "online" || s == "net banking" || s == "netbanking")
        return "UPI";
    if (s == "bank transfer" || s == "bank" || s == "transfer" || s == "neft" || s == "rtgs" || s == "imps")
        return "Bank Transfer";
    if (s == "debit card" || s == "card" || s == "dc")
        return "Debit Card";
    if (s == "credit card" || s == "cc")
        return "Credit Card";
    if (s == "other")
        return "Other";
    return "Other";
}

std::string Validator::normalizeCategory(const std::string &category) {
    std::string s = toLowerTrim(category);
    if (s.empty()) return "Shopping";
    if (s == "shopping" || s == "shop" || s == "store" || s == "online" || s == "e-commerce" || s == "cloth" || s == "clothes")
        return "Shopping";
    if (s == "food" || s == "dining" || s == "restaurant" || s == "cafe" || s == "dinner" || s == "lunch" || s == "swiggy" || s == "zomato")
        return "Food";
    if (s == "education" || s == "study" || s == "school" || s == "college" || s == "fee" || s == "fees" || s == "course" || s == "books")
        return "Education";
    if (s == "travel" || s == "trip" || s == "cab" || s == "fuel" || s == "petrol" || s == "diesel" || s == "uber" || s == "ola" || s == "flight" || s == "train")
        return "Travel";
    if (s == "bills" || s == "bill" || s == "electricity" || s == "water" || s == "recharge" || s == "rent" || s == "wifi" || s == "utility")
        return "Bills";
    if (s == "healthcare" || s == "health" || s == "medical" || s == "medicine" || s == "hospital" || s == "doctor")
        return "Healthcare";
    if (s == "salary" || s == "income" || s == "stipend")
        return "Salary";
    if (s == "business" || s == "work" || s == "office" || s == "client")
        return "Business";
    if (s == "refund" || s == "cashback")
        return "Refund";
    if (s == "other")
        return "Other";
    return "Other";
}

std::string Validator::normalizeStatus(const std::string &status) {
    std::string s = toLowerTrim(status);
    if (s.empty()) return "Successful";
    if (s == "successful" || s == "success" || s == "done" || s == "ok" || s == "pass" || s == "completed" || s == "s")
        return "Successful";
    if (s == "pending" || s == "wait" || s == "processing" || s == "in progress" || s == "p")
        return "Pending";
    if (s == "failed" || s == "fail" || s == "failure" || s == "rejected" || s == "error" || s == "f")
        return "Failed";
    if (s == "refunded" || s == "refund" || s == "returned" || s == "r")
        return "Refunded";
    return "";
}
