TARGET = my_bc
CC = gcc
CFLAGS = -c -Wall -Wextra -Werror -fsanitize=address -g
OFLAGS = -g -fsanitize=address -o
OBJFLDR = obj
SRCFLDR = src
HDRFLDR = include
SOURCES = my_bc.c my_stack.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = my_bc

all: $(TARGET)

$(TARGET):
	@$(CC) $(CFLAGS) $(addprefix $(SRCFLDR)/,$(SOURCES)) -I $(HDRFLDR)/
	@$(CC) $(OFLAGS) $(TARGET) $(OBJECTS)
	@mkdir $(OBJFLDR)
	@mv $(OBJECTS) $(OBJFLDR)
	@echo "========================================================"
	@echo "|| PROJECT: $(TARGET)                                    ||"
	@echo "|| AUTHORS: Anastasia Zimina                          ||"
	@echo "|| USAGE:   ./$(TARGET) [expression]                  ||"
	@echo "========================================================"

clean:
	@rm -rf $(OBJFLDR)

fclean: clean
	@rm -f $(TARGET)

re: fclean all
