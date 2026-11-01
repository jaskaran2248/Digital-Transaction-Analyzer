# Digital Transaction Analyzer – Project Implementation Plan

## Context
The user requested a fully functional C++17 application named *Digital Transaction Record Analyzer and Statistical Reporting System* that performs CRUD operations on transaction records, calculates statistical metrics, generates reports, and displays a terminal‑based dashboard. The project must compile on Windows with only a standard C++ compiler, store data in CSV files, and include automated tests.

The plan outlines the file structure, major components, data model, persistence strategy, user interface, statistical calculations, and testing strategy.

## Recommended Approach
1. **Create a clean, modular directory layout** as described in the prompt, with `include/`, `src/`, `data/`, `reports/`, `tests/`, and `backups/` directories.
2. **Define the core data model** in `include/Transaction.h` – a `struct Transaction` holding all required fields. Use an integer type (`int64_t`) for amounts in paise to avoid floating‑point errors.
3. **Build a `TransactionManager`** that manages an in‑memory `std::vector<Transaction>`. This class provides CRUD, filtering, sorting, import/export, and unique‑ID generation.
4. **Use a `CSVHandler`** for reading/writing CSV files with basic quoting support. It delegates parsing to `Transaction`’s `fromCSV` and `toCSV` helpers.
5. **Implement a `StatisticsEngine`** that takes a reference to the transaction list and computes all required metrics.
6. **Create a `ReportGenerator`** that formats statistics into CSV or plain‑text files, and a `Dashboard` class that renders the interactive menu and ASCII charts.
7. **Implement a simple `Validator`** to check input formats (date, amount, enums, etc.).
8. **Add automated tests** in `tests/test_main.cpp`. Use a minimal test harness (assert macros) to keep the project dependency‑free.
9. **Provide build scripts** (`Makefile` and a `CMakeLists.txt`) and a comprehensive `README.md` with compilation, usage, and test instructions.
10. **Include a sample CSV** in `data/transactions.csv` with 30 fictional records.
11. **Add a backup routine** that moves the existing CSV to `backups/` before any overwrite.

### Critical Files & Their Roles
- `include/Transaction.h` – data model + CSV serialization helpers.
- `src/Transaction.cpp` – implementation of `Transaction` helpers.
- `include/TransactionManager.h` / `src/TransactionManager.cpp` – CRUD, filtering, sorting, import/export.
- `include/CSVHandler.h` / `src/CSVHandler.cpp` – file I/O.
- `include/StatisticsEngine.h` / `src/StatisticsEngine.cpp` – statistical calculations.
- `include/ReportGenerator.h` / `src/ReportGenerator.cpp` – report formatting and file writing.
- `include/Dashboard.h` / `src/Dashboard.cpp` – menu, user interaction, ASCII charts.
- `include/Validator.h` – input validation.
- `src/main.cpp` – application entry point.
- `tests/test_main.cpp` – unit tests.
- `CMakeLists.txt` – build configuration.
- `Makefile` – make target.
- `README.md` – documentation.
- `data/transactions.csv` – sample data.

### Verification Plan
1. **Compilation Test** – run `g++ -std=c++17 -Iinclude src/*.cpp -o DigitalTransactionAnalyzer.exe` and check for no errors.
2. **Unit Tests** – execute `./DigitalTransactionAnalyzer.exe --run-tests` (the main will detect the flag) and confirm all tests pass.
3. **Functional Test** – launch the program, go through each menu option (dashboard, add, view, search, update, delete, filter, sort, import, export, analysis, reports, charts, backup, load sample, exit). Verify data persistence after restarting.
4. **Statistical Accuracy** – pre‑calculate expected values for the sample CSV and compare report output.
5. **Backup Check** – modify the CSV, run backup option, and verify a copy exists with the correct timestamp.
6. **CSV Import Validation** – try importing a malformed file and confirm only valid rows are kept.

All verification steps can be performed manually in a Windows terminal or automated with a script that captures exit codes.

---
*End of plan.*