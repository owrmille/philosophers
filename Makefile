CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = philosophers
SRC = philosophers.c \
	process_input.c \
	input_utils.c
OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re