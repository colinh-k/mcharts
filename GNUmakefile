# NOTE:
#  this makefile uses $(shell ...), which is
#  a function specific to GNU make, so the
#  makefile name reflects this fact

########################################################
# compilation commands
########################################################
CC=gcc
CXX=g++


########################################################
# top-level directories
########################################################
INCL_DIR=include
SRC_DIR=src
BUILD_DIR=build
BIN_DIR=$(BUILD_DIR)/bin
OBJ_DIR=$(BUILD_DIR)/obj


########################################################
# compilation flags
########################################################
CXXFLAGS=-Wall -Werror -g -std=c++11
CCFLAGS=-Wall -Werror -g -std=c17
# use ':=' to evaluate this symbol once via shell function
LDLIBS := $(shell pkg-config --cflags --libs libxml-2.0 libcurl)
# LDLIBS := $(shell curl-config --libs)


########################################################
# source/header and object files
########################################################
OBJS=$(OBJ_DIR)/main.o
# SRCS=$(wildcard $(SRC_DIR)/*.c);
SRCS=$(SRC_DIR)/main.c


########################################################
# main executable built by the target 'all'
########################################################
TARGET=$(BIN_DIR)/mcharts


########################################################
# main build targets
########################################################
all: $(TARGET)

# compile all the src objects together
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

# compiles standalone files that have a .c source and cooresponding .h header
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCL_DIR)/%.h | $(OBJ_DIR)
	$(CC) $(CCFLAGS) -o $@ -c $<

# compiles standalone files that only have a .c source
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CCFLAGS) -o $@ -c $<


########################################################
# extra targets
########################################################
leaks:
	@leaks --atExit -- ./$(TARGET)
debug:
	@lldb ./$(TARGET)
run:
	@./$(TARGET)
clean:
	rm -rf ./$(BUILD_DIR)


# auto make build directories
$(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

.PHONY: clean all run debug leaks
