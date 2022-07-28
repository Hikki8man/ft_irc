SRC		= Bot.cpp main.cpp
OBJS	= ${SRC:.cpp=.o}
NAME	= Bot
CC		= c++
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror --std=c++98

%.o: %.cpp ${INCS}
	${CC} ${CFLAGS} -c $< -o $@

${NAME}: ${OBJS}
	${CC} -o ${NAME} ${OBJS} 
	
all: ${NAME}

bonus: all

clean:
	${RM} ${OBJS}
	${RM} a.out

fclean: clean
	${RM} ${NAME}

re: fclean all
	
.PHONY: all clean fclean re .c.o
