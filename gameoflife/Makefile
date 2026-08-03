# Target executable name
TARGET = game_of_life

# Source files
SRCS = main.cpp

# Base compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# 1. Detect the Operating System
OS := $(shell uname -s)

# 2. Configure paths and flags based on the OS
ifeq ($(OS), Darwin)
    # --- macOS Setup ---
    CXX = clang++
    
    # Detect Apple Silicon (arm64) vs Intel (x86_64) for Homebrew paths
    ARCH := $(shell uname -m)
    ifeq ($(ARCH), arm64)
        HOMEBREW_PREFIX = /opt/homebrew
    else
        HOMEBREW_PREFIX = /usr/local
    endif
    
    INCLUDES = -I$(HOMEBREW_PREFIX)/include
    LDFLAGS = -L$(HOMEBREW_PREFIX)/lib
    LIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

else
    # --- Linux Setup ---
    CXX = g++
    INCLUDES = 
    LDFLAGS = 
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# Default rule to build the game
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Clean up build files
clean:
	rm -f $(TARGET)

# Build and immediately run the game
run: all
	./$(TARGET)

.PHONY: all clean run
