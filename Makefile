NAME	= philo
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRC		= main.c utils.c utils2.c validate_input.c time.c simulation.c monitoring.c routines.c routines2.c init.c
OBJS	= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
