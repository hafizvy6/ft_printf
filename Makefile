NAME	= libftprintf.a
CC		= cc
CFLAGS	= -Wall -Wextra -Werror
HEAD	= ft_printf.h
SRCS	= ft_printf.c ft_parse_format.c ft_print_text.c \
		  ft_print_number.c ft_print_pointer.c ft_render_digits.c \
		  ft_emit_padding.c ft_write_text.c
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c $(HEAD)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

bonus: all

re: fclean all

rebonus: fclean bonus

.PHONY: all bonus clean fclean re rebonus
