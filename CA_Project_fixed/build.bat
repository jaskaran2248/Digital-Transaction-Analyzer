@echo off
setlocal
echo ========================================
echo Building DigitalTransactionAnalyzer
echo ========================================

set CXX=C:\msys64\ucrt64\bin\g++.exe
if not exist "%CXX%" (
    where g++ >nul 2>nul
    if %ERRORLEVEL% EQU 0 (
        set CXX=g++
    ) else (
        echo [ERROR] No C++ compiler found. Please install MSYS2 UCRT64 or MinGW.
        exit /b 1
    )
)

echo Using compiler: %CXX%
"%CXX%" -std=c++17 -Wall -Wextra -Iinclude src\main.cpp src\ReportGenerator.cpp src\StatisticsEngine.cpp src\Transaction.cpp src\TransactionManager.cpp src\Validator.cpp -o DigitalTransactionAnalyzer.exe

if %ERRORLEVEL% EQU 0 (
    echo [SUCCESS] Built DigitalTransactionAnalyzer.exe successfully!
) else (
    echo [FAILED] Compilation failed with error code %ERRORLEVEL%.
    exit /b %ERRORLEVEL%
)
endlocal
