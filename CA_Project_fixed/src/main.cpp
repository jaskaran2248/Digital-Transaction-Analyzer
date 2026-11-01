#include <cmath>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "TransactionManager.h"
#include "ReportGenerator.h"
#include "Validator.h"

static std::string ask(const std::string &p) {
    std::cout << p;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

static void printAll(const TransactionManager &m) {
    const auto &txs = m.all();
    if (txs.empty()) {
        std::cout << "No transactions found.\n";
        return;
    }
    for (const auto &t : txs)
        std::cout << t.id << " | " << t.datetime << " | " << t.sender << " -> " << t.receiver
                  << " | " << t.getAmountString() << " | " << t.type << " | " << t.status << '\n';
}

static void runTests() {
    std::cout << "Running automated tests...\n";

    // Validator tests
    assert(Validator::isValidDateTime("2026-01-05 10:15:00"));
    assert(!Validator::isValidDateTime("invalid-date"));
    assert(!Validator::isValidDateTime("2026-01-05 10:15:00 extra"));
    assert(Validator::isValidAmount("100.50"));
    assert(!Validator::isValidAmount("-5"));
    assert(!Validator::isValidAmount("abc"));
    assert(!Validator::isValidAmount("100abc"));
    assert(Validator::isValidTransactionType("Credit"));
    assert(Validator::isValidTransactionType("debit"));
    assert(Validator::isValidPaymentMethod("UPI"));
    assert(Validator::isValidPaymentMethod("online"));
    assert(Validator::isValidCategory("Shopping"));
    assert(Validator::isValidCategory("food"));
    assert(Validator::isValidStatus("Successful"));
    assert(Validator::isValidStatus("success"));

    // Transaction parsing & formatting tests
    std::string csvRow = "1,2026-01-05 10:15:00,Amit,Shop A,1250.50,Debit,UPI,Shopping,Successful,REF001,Groceries";
    Transaction t = Transaction::fromCSV(csvRow);
    assert(t.id == 1);
    assert(t.amountPaise == 125050);
    assert(t.sender == "Amit");
    assert(t.toCSV() == csvRow);

    // StatisticsEngine tests
    std::vector<Transaction> list = { t };
    StatisticsEngine stats(list);
    assert(stats.totalTransactions() == 1);
    assert(stats.successfulTransactions() == 1);
    assert(stats.totalDebit() == 125050);
    assert(stats.totalCredit() == 0);
    assert(stats.netBalance() == -125050);

    std::cout << "All tests passed successfully!\n";
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--run-tests") {
        runTests();
        return 0;
    }

    TransactionManager mgr; // loads data/transactions.csv, saves on exit
    for (;;) {
        std::cout << "\n1) List  2) Add  3) Delete  4) Summary report  5) Export  0) Exit\n";
        std::string c = ask("> ");
        if (c == "0" || !std::cin) break;
        if (c == "1") {
            printAll(mgr);
        }
        else if (c == "2") {
            Transaction t;
            t.datetime = ask("DateTime (YYYY-MM-DD HH:MM:SS) [Press Enter for current]: ");
            if (t.datetime.empty()) {
                auto now = std::chrono::system_clock::now();
                std::time_t now_t = std::chrono::system_clock::to_time_t(now);
                std::tm tm;
#ifdef _WIN32
                localtime_s(&tm, &now_t);
#else
                localtime_r(&now_t, &tm);
#endif
                std::ostringstream dtoss;
                dtoss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
                t.datetime = dtoss.str();
                std::cout << "Using current DateTime: " << t.datetime << "\n";
            } else if (t.datetime.size() == 10) {
                t.datetime += " 00:00:00";
            }
            if (!Validator::isValidDateTime(t.datetime)) {
                std::cout << "[Error] Invalid DateTime format! Expected format: YYYY-MM-DD HH:MM:SS (e.g. 2026-10-26 12:45:46)\n";
                continue;
            }

            t.sender = ask("Sender: ");
            if (t.sender.empty()) t.sender = "Self";

            t.receiver = ask("Receiver: ");
            if (t.receiver.empty()) t.receiver = "Self";

            std::string amt = ask("Amount (rupees): ");
            if (!Validator::isValidAmount(amt)) {
                std::cout << "[Error] Invalid Amount! Please enter a valid number (e.g. 5000 or 150.50)\n";
                continue;
            }
            t.amountPaise = std::llround(std::stod(amt) * 100);

            std::string rawType = ask("Type (Credit/Debit): ");
            t.type = Validator::normalizeType(rawType);
            if (t.type.empty()) {
                std::cout << "[Error] Invalid Type! Allowed: Credit, Debit\n";
                continue;
            }

            std::string rawMethod = ask("Method (UPI, Bank Transfer, Debit Card, Credit Card, Other): ");
            t.method = Validator::normalizePaymentMethod(rawMethod);

            std::string rawCategory = ask("Category (Shopping, Food, Education, Travel, Bills, Healthcare, Salary, Business, Other): ");
            t.category = Validator::normalizeCategory(rawCategory);

            std::string rawStatus = ask("Status (Successful, Pending, Failed, Refunded): ");
            t.status = Validator::normalizeStatus(rawStatus);
            if (t.status.empty()) {
                std::cout << "[Error] Invalid Status! Allowed: Successful, Pending, Failed, Refunded\n";
                continue;
            }

            t.refNumber = ask("Ref number: ");
            if (t.refNumber.empty()) t.refNumber = "N/A";

            t.description = ask("Description: ");
            if (t.description.empty()) t.description = "-";

            int newId = mgr.addTransaction(t);
            std::cout << "Added successfully: ID " << newId << " | " << t.type << " | " << t.getAmountString() 
                      << " | " << t.method << " | " << t.category << " | " << t.status << "\n";
        }
        else if (c == "3") {
            try {
                std::string idStr = ask("ID: ");
                std::cout << (mgr.deleteTransaction(std::stoi(idStr)) ? "Deleted.\n" : "Not found.\n");
            }
            catch (...) {
                std::cout << "Invalid ID.\n";
            }
        }
        else if (c == "4") {
            ReportGenerator rep(mgr.all());
            std::string r = rep.overallSummary();
            std::cout << r;
            rep.writeToFile("reports/summary.txt", r);
            std::cout << "Saved summary to reports/summary.txt\n";
        }
        else if (c == "5") {
            mgr.exportToFile("reports/export.csv");
            std::cout << "Exported transactions to reports/export.csv\n";
        }
        else {
            std::cout << "Invalid option. Please enter 0-5.\n";
        }
    }
    return 0;
}
