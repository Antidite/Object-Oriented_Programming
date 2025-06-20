# Makefile for building and running the test application

# Compiler and flags
CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Wall -I. -IMy_code -ITeacher_code

# Allow overriding allocator mode: 1=StudentAllocator, 2=std::allocator
# Default to mode 1
ALLOCATOR_MODE ?= 1
CXXFLAGS       += -DALLOCATOR_MODE=$(ALLOCATOR_MODE)

# Target executable
TARGET := test_app

# Source files
SRCS := testallocator.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: all build clean run1 run2 runall

# Default: run both modes
all: runall

# Clean up build artifacts
clean:
	rm -f test_app

# Compile & run with allocator mode 1
env1 = -std=c++17 -O3 -Wall -I. -IMy_code -ITeacher_code -DALLOCATOR_MODE=1
run1:
	$(CXX) $(env1) testallocator.cpp -o test_app
	@echo "=== Running with mode 1 (StudentAllocator) ==="
	@./test_app

# Compile & run with allocator mode 2
env2 = -std=c++17 -O3 -Wall -I. -IMy_code -ITeacher_code -DALLOCATOR_MODE=2
run2:
	$(CXX) $(env2) testallocator.cpp -o test_app
	@echo "=== Running with mode 2 (std::allocator) ==="
	@./test_app

# Run both modes sequentially
runall: clean run1 run2
	@echo "=== Completed runall: modes 1 and 2 ==="


