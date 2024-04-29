#compilation vars-------------
CFLAGS=-Wall -Wextra -Werror -g3 -D COLORED -fsanitize=address

#names and folders------------
NAME=philo

SRCS=cleanup.c \
ft_utils.c \
get_set.c \
init_program.c \
main.c \
monitor_routine.c \
philo_routine.c \
time_utils.c \
validation.c \
write_functions.c

OBJS_DIR=objs/
OBJS=$(SRCS:%.c=$(OBJS_DIR)%.o)
HEADER=philo.h

#rules-----------------------
all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS_DIR)%.o: %.c $(HEADER) 
	@mkdir -p $(OBJS_DIR) 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean
	$(MAKE) all --no-print-directory

.PHONY: all clean fclean re
