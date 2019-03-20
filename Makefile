RM := rm -rf
ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CXXARGS := -std=c++17 -I "$(ROOT_DIR)/include/" $(LDLIBS)
EXEC_DIR := bin
EXEC_NAME := pom
build: main.cpp
	@mkdir -p "$(EXEC_DIR)"
	$(CXX) main.cpp -o "$(EXEC_DIR)/$(EXEC_NAME)" $(CXXARGS)
clean: $(EXEC_DIR)
	$(RM) "$(ROOT_DIR)/$(EXEC_DIR)"
all:
	build
