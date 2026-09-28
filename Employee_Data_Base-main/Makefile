.RECIPEPREFIX = >
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c17 -O2 -g
SRC = src/main.c src/employee.c src/database.c src/file_ops.c src/menu.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = empitrack
TEST_SRC = tests/test_employee.c
TEST_TARGET = test_empitrack

all: $(TARGET)

$(TARGET): $(OBJ)
> $(CC) $(CFLAGS) -o $@ $^

%.o: %.c
> $(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC) $(filter-out src/main.o,$(OBJ))
> $(CC) $(CFLAGS) -o $@ $^

run: all
> ./$(TARGET)

clean:
> rm -f $(OBJ) $(TARGET) $(TEST_TARGET) src/*.o

.PHONY: all test run clean
