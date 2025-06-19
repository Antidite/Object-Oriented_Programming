# Makefile for building and running the test application

# Compiler and flags
CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -I. -IMy_code -ITeacher_code

# Allow overriding allocator mode: 1=StudentAllocator, 2=std::allocator
# Default to mode 1
ALLOCATOR_MODE ?= 1
CXXFLAGS       += -DALLOCATOR_MODE=$(ALLOCATOR_MODE)

# Target executable
TARGET := test_app

# Source files
SRCS := testallocator.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean run1 run2 runall

# Build the application
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile sources
%.o: %.cpp allocator_selector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Run with allocator mode 1
run1: clean all
	@echo "Running with mode 1 (StudentAllocator)"
	@./$(TARGET)

# Run with allocator mode 2
run2: clean all ALLOCATOR_MODE=2
	@echo "Running with mode 2 (std::allocator)"
	@./$(TARGET)

# Run both modes sequentially
runall: run1 run2
	@echo "Completed both run1 and run2"
