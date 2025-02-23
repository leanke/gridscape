all: clean build test

LIBRARIES := -lncurses
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
COMPILER ?= linux_clang
CFLAGS := -I$(INCLUDE_DIR) -Wall -Wextra -Werror -fsanitize=address,undefined,bounds,pointer-overflow,leak
LDFLAGS := -fsanitize=address,undefined,bounds,pointer-overflow,leak

ifeq ($(COMPILER), linux_gcc)
	CC := gcc
endif
ifeq ($(COMPILER), linux_clang)
	CC := clang
endif
ifeq ($(COMPILER), windows)
	CC := x86_64-w64-mingw32-g++
endif


DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))


build: $(BUILD_DIR)/gridscape

$(BUILD_DIR)/gridscape: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

test:
	$(BUILD_DIR)/gridscape

.PHONY: all build clean
