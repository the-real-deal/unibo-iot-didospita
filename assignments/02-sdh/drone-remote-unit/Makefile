# Makefile for compiling Java projects (Linux/MacOS/Windows)

SRC_DIR = src
BUILD_DIR = build
MAIN_CLASS = Main

ifeq ($(OS),Windows_NT)
	RM = rmdir /s /q
	MKDIR_BUILD = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	FIND_SOURCES = $(shell dir /s /B $(SRC_DIR)\*.java)
	PATH_SEP = ;
else
	RM = rm -rf
	MKDIR_BUILD = mkdir -p $(BUILD_DIR)
	FIND_SOURCES = $(shell find $(SRC_DIR) -name "*.java")
	PATH_SEP = :
endif

SOURCES = $(FIND_SOURCES)

.PHONY: all
all: build

.PHONY: build
build:
	$(MKDIR_BUILD)
	javac -d $(BUILD_DIR) -sourcepath $(SRC_DIR) $(SOURCES)

.PHONY: run
run: build
	java -cp $(BUILD_DIR) $(MAIN_CLASS)

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)
