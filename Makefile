NAME	= run
LIBFT	= ./libft
HEADERS	= -I ./includes -I ${LIBFT}
LIBS	= ${LIBFT}/libft.a
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRC = main.c close_program.c parse_input.c error_handling.c init_data.c status_handling.c
OBJS = $(SRC:.c=.o)

all: $(NAME)

$(LIBS):
	$(MAKE) -C $(LIBFT)

$(OBJS): $(LIBS)

$(NAME): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re
