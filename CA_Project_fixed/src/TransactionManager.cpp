#include "TransactionManager.h"
#include <filesystem>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <iomanip>

TransactionManager::TransactionManager() : dataFilePath_("data/transactions.csv") {
    loadFromFile();
}

TransactionManager::~TransactionManager() {
    saveToFile();
}

void TransactionManager::loadFromFile() {
    std::filesystem::create_directories("data");
    std::filesystem::create_directories("backups");
    std::ifstream file(dataFilePath_);
    if (!file.is_open()) {
        // If file does not exist, start with empty list
        return;
    }
    std::string line;
    std::vector<Transaction> temp;
    bool headerSkipped = false;
    while (std::getline(file, line)) {
        if (!headerSkipped) {
            // Skip header line
            headerSkipped = true;
            continue;
        }
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        try {
            Transaction t = Transaction::fromCSV(line);
            temp.push_back(t);
        } catch (const std::exception &e) {
            std::cerr << "Warning: Skipping invalid row: " << line << " Error: " << e.what() << std::endl;
        }
    }
    transactions_ = std::move(temp);
}

void TransactionManager::saveToFile() const {
    // Backup existing file
    std::filesystem::create_directories("data");
    std::filesystem::create_directories("backups");
    {
        std::ifstream src(dataFilePath_);
        if (src.good()) {
            // Get timestamp
            auto now = std::chrono::system_clock::now();
            std::time_t now_t = std::chrono::system_clock::to_time_t(now);
            std::tm tm;
#ifdef _WIN32
            localtime_s(&tm, &now_t);
#else
            localtime_r(&now_t, &tm);
#endif
            std::ostringstream oss;
            oss << "backups/transactions_backup_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".csv";
            std::ofstream dst(oss.str(), std::ios::binary);
            dst << src.rdbuf();
        }
    } // Ensure src is closed before opening out

    std::ofstream out(dataFilePath_, std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << dataFilePath_ << std::endl;
        return;
    }
    // Write header
    out << "ID,DateTime,Sender,Receiver,Amount,Type,Method,Category,Status,RefNumber,Description" << '\n';
    for (const auto &t : transactions_) {
        out << t.toCSV() << '\n';
    }
}

int TransactionManager::addTransaction(const Transaction &t) {
    Transaction newT = t;
    newT.id = generateUniqueId();
    transactions_.push_back(newT);
    return newT.id;
}

std::optional<Transaction> TransactionManager::getById(int id) const {
    auto it = std::find_if(transactions_.begin(), transactions_.end(), [id](const Transaction &t){ return t.id == id; });
    if (it != transactions_.end()) {
        return *it;
    }
    return std::nullopt;
}

bool TransactionManager::updateTransaction(int id, const Transaction &newData) {
    auto idx = findIndexById(id);
    if (!idx) return false;
    Transaction updated = newData;
    updated.id = id; // preserve id
    transactions_[*idx] = updated;
    return true;
}

bool TransactionManager::deleteTransaction(int id) {
    auto idx = findIndexById(id);
    if (!idx) return false;
    transactions_.erase(transactions_.begin() + *idx);
    return true;
}

const std::vector<Transaction>& TransactionManager::all() const {
    return transactions_;
}

int TransactionManager::generateUniqueId() {
    int maxId = 0;
    for (const auto &t : transactions_) {
        if (t.id > maxId) maxId = t.id;
    }
    return maxId + 1;
}

std::optional<size_t> TransactionManager::findIndexById(int id) const {
    for (size_t i=0;i<transactions_.size();++i) {
        if (transactions_[i].id == id) return i;
    }
    return std::nullopt;
}

std::vector<int> TransactionManager::filter(const std::string &senderReceiverFilter,
                                            const std::string &methodFilter,
                                            const std::string &categoryFilter,
                                            const std::string &statusFilter,
                                            const std::string &minDate,
                                            const std::string &maxDate,
                                            double minAmount,
                                            double maxAmount) const {
    std::vector<int> indices;
    for (size_t i=0;i<transactions_.size();++i) {
        const auto &t = transactions_[i];
        bool ok = true;
        if (!senderReceiverFilter.empty()) {
            if (t.sender.find(senderReceiverFilter)==std::string::npos && t.receiver.find(senderReceiverFilter)==std::string::npos) {
                ok = false;
            }
        }
        if (!methodFilter.empty() && t.method != methodFilter) ok = false;
        if (!categoryFilter.empty() && t.category != categoryFilter) ok = false;
        if (!statusFilter.empty() && t.status != statusFilter) ok = false;
        if (!minDate.empty() && t.datetime < minDate) ok = false;
        if (!maxDate.empty() && t.datetime > maxDate) ok = false;
        double amt = t.amountPaise / 100.0;
        if (minAmount >= 0 && amt < minAmount) ok = false;
        if (maxAmount >= 0 && amt > maxAmount) ok = false;
        if (ok) indices.push_back(static_cast<int>(i));
    }
    return indices;
}

void TransactionManager::sortBy(const std::string &field, bool ascending) {
    auto comp = [&](const Transaction &a, const Transaction &b){
        if (field=="date") return ascending ? a.datetime < b.datetime : a.datetime > b.datetime;
        if (field=="amount") return ascending ? a.amountPaise < b.amountPaise : a.amountPaise > b.amountPaise;
        if (field=="id") return ascending ? a.id < b.id : a.id > b.id;
        if (field=="type") return ascending ? a.type < b.type : a.type > b.type;
        return false;   // unknown field: must NOT return true (breaks strict weak ordering)
    };
    std::stable_sort(transactions_.begin(), transactions_.end(), comp);
}

void TransactionManager::importFromFile(const std::string &csvPath) {
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open import file: " << csvPath << std::endl;
        return;
    }
    std::string line;
    bool headerSkipped = false;
    int added = 0;
    while (std::getline(file, line)) {
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        try {
            Transaction t = Transaction::fromCSV(line);
            // Check for duplicate ID
            if (getById(t.id)) {
                // Assign new ID
                t.id = generateUniqueId();
            }
            transactions_.push_back(t);
            ++added;
        } catch (const std::exception &e) {
            std::cerr << "Warning: Skipping invalid row in import: " << line << std::endl;
        }
    }
    std::cout << "Imported " << added << " transactions." << std::endl;
}

void TransactionManager::exportToFile(const std::string &csvPath) const {
    std::filesystem::path p(csvPath);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }
    std::ofstream out(csvPath, std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Error: Could not open file for export: " << csvPath << std::endl;
        return;
    }
    out << "ID,DateTime,Sender,Receiver,Amount,Type,Method,Category,Status,RefNumber,Description" << '\n';
    for (const auto &t : transactions_) {
        out << t.toCSV() << '\n';
    }
}
