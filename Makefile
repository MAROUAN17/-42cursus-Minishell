CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
CC = cc
NAME = main
SRC = main.c ft_split.c ft_strcmp.c ft_strjoin.c fill_token.c ft_strtrim.c \
builtins.c create_ast.c ft_strncmp.c ft_strdup.c
OBJS = ${SRC:.c=.o}

all : ${NAME}

${NAME} : ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -lreadline -o ${NAME}

%.o: %.c parse_header.h
	${CC} ${CFLAGS} -c -o $@ $<

clean : 
	rm -rf ${OBJS} ${BOBJS}

fclean : clean
	rm -rf ${NAME}

re : fclean all

.PHONY : all clean fclean re bonus