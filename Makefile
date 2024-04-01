CXX	:= g++-13

CXXFLAGS 	+= -std=c++17
CXXFLAGS 	+= -g3
CXXFLAGS 	+= -ggdb
CXXFLAGS 	+= -fPIC
CXXFLAGS 	+= -gdwarf-4
CXXFLAGS 	+= -Wall
CXXFLAGS 	+= -Wextra
CXXFLAGS 	+= -Wwrite-strings
CXXFLAGS 	+= -Wno-parentheses
CXXFLAGS 	+= -Wpedantic
CXXFLAGS 	+= -Warray-bounds
CXXFLAGS 	+= -Wconversion
CXXFLAGS 	+= -Wno-unused-function

BINDIR  	:= bin
TEST_BINDIR := tests/bin

SRCDIRS 	:= lexer repl token util
SRCS 		:= $(filter-out repl/repl.cpp, $(wildcard $(addsuffix /*.cpp, $(SRCDIRS))))

TESTDIR 	:= tests
TEST_SRCS 	:= $(wildcard $(TESTDIR)/*.cpp)

TARGET 		:= $(BINDIR)/repl
TEST_TARGET := $(TEST_BINDIR)/lexer_test

all: $(TARGET)

$(TARGET): repl/repl.cpp $(SRCS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): $(SRCS) $(TEST_SRCS) | $(TEST_BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TEST_TARGET)
	@echo "Running tests..."
	@./$<	

$(BINDIR) $(TEST_BINDIR):
	mkdir -p $@

rclean:
	rm -rf $(BINDIR)

tclean:
	rm -rf $(TEST_BINDIR)

fclean_repl:
	rm $(TARGET) 

fclean_test:
	rm $(TEST_TARGET)

.PHONY: all test clean fclean_repl fclean_test

.DELETE_ON_ERROR:
