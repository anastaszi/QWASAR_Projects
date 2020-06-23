NAME = my_blockchain

 SRCFILES = main.c my_lib.c my_readline.c my_helper.c my_block.c my_node.c my_printing.c my_backup.c
 OBJFILES = $(SRCFILES:.c=.o)
 SRCFLDR = src
 OBJFLDR = obj
 HEADER = include
 FLAGS = -c -Wextra -Wall -Werror

all: $(NAME)

$(NAME):
	@gcc $(FLAGS) $(addprefix $(SRCFLDR)/,$(SRCFILES)) -I $(HEADER)/
	@gcc -o $(NAME) $(OBJFILES)
	@mkdir $(OBJFLDR)
	@mv $(OBJFILES) $(OBJFLDR)
	@echo "========================================================"
	@echo "|| \e[32mPROJECT\e[0m: $(NAME)                             ||"
	@echo "|| \e[32mAUTHORS\e[0m: Ksenia Anishenko & Anastasia Zimina       ||"
	@echo "|| \e[32mUSAGE\e[0m:   ./$(NAME) [backupfile]              ||"
	@echo "========================================================"
clean:
	@rm -rf $(OBJFLDR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
