#===============================#
#          PROJECT NAME         #
#===============================#

NAME		=	ircserv

#===============================#
#             VPATH             #
#===============================#

VPATH		=	srcs:includes

#===============================#
#             DIRS              #
#===============================#

OBJS_DIR	=	objs
LIBFT_DIR	=	./libft/
RL_DIR		=	/Users/$$USER/.brew/opt/readline/lib
RL_DIR_I	=	/Users/$$USER/.brew/opt/readline/include

#===============================#
#            SOURCES            #
#===============================#

#             MAIN              #

SRCS		= 	main.cpp

#===============================#
#            HEADERS            #
#===============================#

HEADERS		=	ft_irc.hpp

#===============================#
#            OBJECTS            #
#===============================#

OBJS		=	$(addprefix objs/, ${SRCS:.cpp=.o})

#===============================#
#             FILES             #
#===============================#

MAKEFILE 	=	Makefile

#===============================#
#            COMMANDS           #
#===============================#

RM			=	rm -f

#===============================#
#            COMPILER           #
#===============================#

CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -std=c++98

#===============================#
#             COLORS            #
#===============================#

BLACK		=	\x1b[30m
RED			=	\x1b[31m
GREEN		=	\x1b[32m
YELLOW		=	\x1b[33m
BLUE		=	\x1b[34m
PURPLE		=	\x1b[35m
CYAN		=	\x1b[36m
WHITE		=	\x1b[37m

#===============================#
#             RULES             #
#===============================#

#          MAIN RULES           #

all: ${NAME} 

bonus: all

re: fclean all

#        CLEANING RULES         #

clean:
	${RM} ${OBJS}
	${RM} a.out

fclean: clean
	${RM} ${NAME}

#       COMPILATION RULES       #

${OBJS_DIR}/%.o: %.cpp ${MAKEFILE} #${HEADERS}
	@mkdir -p ${OBJS_DIR}
	@${CC} ${CFLAGS} -c $< -o $@ -I ${INCS_DIR}
	@echo "[${NAME}] Compiling ${YELLOW}$<${WHITE}"

${NAME}: ${OBJS} ${LIBFT_SRCS}
	@echo "[${NAME}] Creating executable..."
	@${CC} -o ${NAME} ${OBJS}
	@echo "[${NAME}] ${GREEN}Compilation successful!${WHITE}"


#            .PHONY             #

.PHONY: all clean fclean re .c.o
