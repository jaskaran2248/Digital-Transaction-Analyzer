# Digital Transaction Analyzer & Statistical Reporting System

A C++17 application for managing digital transaction records, performing statistical calculations, and generating summary reports.

---

## Requirements
- Windows 10/11
- C++17 compatible compiler:
  - Recommended: **MSYS2 UCRT64 GCC** (`C:\msys64\ucrt64\bin\g++.exe`), which is already installed.
  - *Note:* Older MinGW (GCC 6.3) lacks C++17 `<optional>` and `<filesystem>` support.

---

## How to Build

### Option 1: Double-click or run `build.bat` (Command Prompt)
```cmd
build.bat
```

### Option 2: PowerShell script
```powershell
.\build.ps1
```

### Option 3: VS Code
- Open `F:\CA_Project_fixed` in VS Code.
- Press **Ctrl + Shift + B** to run the build task.

### Option 4: Direct Command Line
```powershell
& "C:\msys64\ucrt64\bin\g++.exe" -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/ReportGenerator.cpp src/StatisticsEngine.cpp src/Transaction.cpp src/TransactionManager.cpp src/Validator.cpp -o DigitalTransactionAnalyzer.exe
```

---

## How to Run

### Run Application:
```cmd
DigitalTransactionAnalyzer.exe
```

### Run Automated Unit Tests:
```cmd
DigitalTransactionAnalyzer.exe --run-tests
```

---

## Features
1. **List Transactions**: Display all loaded records with formatted amounts (₹).
2. **Add Transaction**: Interactive input with validation for dates, amounts, transaction types, methods, and categories.
3. **Delete Transaction**: Remove by ID with automatic error handling.
4. **Summary Report**: Calculate total credits, debits, net balance, averages, and display highest/lowest transactions. Saves report to `reports/summary.txt`.
5. **Export to CSV**: Export all current transactions to `reports/export.csv`.
6. **Automatic Backup**: Creates timestamped backup CSV in `backups/` every time changes are persisted.
