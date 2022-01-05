NAME = pipex
NAME_B =
CC = gcc
CFLAGS = -Wall -Werror -Wextra
PATH_SRC = ./srcs/
PATH_INC = ./inc/
PATH_BONUS = ./bonus/
SRC =
SRC_B =
OBJ = $(SRC:.c=.o)
OBJ_B = $(SRC_B:.c=.o)



FSANITIZE = -fsanitize=address

G3 = -g3

all : $(NAME)

%.o : %.c
		$(CC) $(CFLAGS) -I$(PATH_INC) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -I$(PATH_INC) $(OBJ) -o $(NAME)

bonus : $(NAME_B)

$(NAME_B): $(OBJ_B)
	$(CC) $(CFLAGS) -I$(PATH_INC) $(OBJ_B) -o $(NAME_B)

clean :
		rm -f $(OBJ) $(OBJ_B)
fclean : clean
		rm -f $(NAME) $(NAME_B)

sanitize : fclean $(NAME)
	echo "PROUT\n"
	$(CC) $(CFLAGS) $(G3) -I$(PATH_INC) $(OBJ) -o $(NAME)
	echo "PROUT PROUT"

re : fclean all

.PHONY : all clean fclean re
