CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread
NAME = philosophers
SRC = main.c \
	eat.c \
	init.c \
	input_utils.c \
	monitor.c \
	process_input.c \
	routine.c \
	run_simulation.c \
	state.c \
	time_utils.c \
	utils.c
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