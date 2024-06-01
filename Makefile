NAME = philo
CC = gcc
FLAGS = -Wall -Wextra -Werror -pthread
RM = rm -rf

SRC = main.c thread.c philo.c utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re