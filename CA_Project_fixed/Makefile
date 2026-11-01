CXX ?= C:/msys64/ucrt64/bin/g++
ifeq ($(wildcard $(CXX)*),)
CXX = g++
endif

CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC = src/main.cpp src/ReportGenerator.cpp src/StatisticsEngine.cpp src/Transaction.cpp src/TransactionManager.cpp src/Validator.cpp
TARGET = DigitalTransactionAnalyzer.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@

run: all
	./$(TARGET)

test: all
	./$(TARGET) --run-tests

clean:
	cmd /c del /f /q $(TARGET) 2>nul || rm -f $(TARGET)
