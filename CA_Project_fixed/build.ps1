$cxx = "C:\msys64\ucrt64\bin\g++.exe"
if (-not (Test-Path $cxx)) {
    $cxx = (Get-Command g++ -ErrorAction SilentlyContinue).Source
    if (-not $cxx) {
        Write-Error "No C++ compiler found. Please install MSYS2 UCRT64 GCC or add g++ to PATH."
        exit 1
    }
}

Write-Host "Building DigitalTransactionAnalyzer using $cxx..." -ForegroundColor Cyan
& $cxx -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/ReportGenerator.cpp src/StatisticsEngine.cpp src/Transaction.cpp src/TransactionManager.cpp src/Validator.cpp -o DigitalTransactionAnalyzer.exe

if ($LASTEXITCODE -eq 0) {
    Write-Host "[SUCCESS] Built DigitalTransactionAnalyzer.exe successfully!" -ForegroundColor Green
} else {
    Write-Host "[FAILED] Build failed with exit code $LASTEXITCODE" -ForegroundColor Red
    exit $LASTEXITCODE
}
