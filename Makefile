NAME = mini_paint

CC = gcc
CFLAG = -Wall -Wextra -Werror

RM = rm -f

SRC = mini_paint.CC
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAG) $(OBJ) -lm -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all