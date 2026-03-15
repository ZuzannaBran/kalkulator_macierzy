CXX       := gcc
CXX_FLAGS := -std=c99 -ggdb -Wall

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   :=
EXECUTABLE  := main.exe
TESTS_EXE   := tests.exe

# We are separating those files here cause we need to compile
# Tests and Main separetaly
ALL_SRC     := $(wildcard $(SRC)/*.c)
APP_MAIN       := $(SRC)/main.c
TEST_MAIN      := $(SRC)/tests.c

CORE_LOGIC     := $(filter-out $(APP_MAIN) $(TEST_MAIN), $(ALL_SRC))
# Check for Windows_NT environment variable (cmd.exe/PowerShell)
IS_WINDOWS := 0
ifeq ($(OS),Windows_NT)
IS_WINDOWS := 1	# Confirmed Windows (cmd.exe/PowerShell)
endif

ifeq ($(IS_WINDOWS),1)
	DELETE := del
else
	DELETE := rm
endif

all : $(BIN) $(BIN)/$(EXECUTABLE)

$(BIN):
	mkdir $(BIN)

test: $(BIN) $(TEST_MAIN) $(CORE_LOGIC)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $(TEST_MAIN) $(CORE_LOGIC) -o $(BIN)/$(TESTS_EXE) $(LIBRARIES)
	@echo "Running tests..."
	@$(BIN)/$(TESTS_EXE)

run: all
	@$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(APP_MAIN) $(CORE_LOGIC)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	@$(DELETE) $(BIN)/$(EXECUTABLE)
