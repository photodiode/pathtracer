
TARGET = real
LIBS = -lm -lpthread
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wshadow -Wunreachable-code -pedantic -O2

SRC_DIR = src
BUILD_DIR = build

SRC = $(foreach sdir ,$(SRC_DIR), $(wildcard $(sdir)/*.c))

$(BUILD_DIR)/$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $@
