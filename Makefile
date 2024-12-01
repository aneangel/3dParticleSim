# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -Iinclude
LDFLAGS = -lglfw -lGLEW -lGL -lGLU

# Source and output directories
SRC_DIR = src
BIN_DIR = build
TARGET = $(BIN_DIR)/program

# Source files and object files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files into object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(BIN_DIR)/*

# Phony targets
.PHONY: all clean