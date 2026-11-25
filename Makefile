NAME = codexion

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = srcs/main.c srcs/parsing.c srcs/init.c srcs/utils.c srcs/cleanup.c srcs/coder.c srcs/logger.c srcs/dongle.c srcs/heap.c srcs/heap_remove.c srcs/arbiter.c srcs/monitor.c
OBJS = $(SRCS:.c=.o)

INCLUDES = -Iincludes

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re