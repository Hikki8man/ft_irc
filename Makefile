#===============================#
#          PROJECT NAME         #
#===============================#

NAME		=	ircserv

#===============================#
#             VPATH             #
#===============================#

VPATH		=	srcs:includes:srcs/commands:srcs/commands/impl:includes/commands:includes/commands/impl

#===============================#
#             DIRS              #
#===============================#

OBJS_DIR	=	objs
INCS_DIR	=  	includes
LIBFT_DIR	=	./libft/
RL_DIR		=	/Users/$$USER/.brew/opt/readline/lib
RL_DIR_I	=	/Users/$$USER/.brew/opt/readline/include

#===============================#
#            SOURCES            #
#===============================#

#             MAIN              #

SRCS		= 	main.cpp

#             CORE              #

SRCS		+=	Server.cpp \
				Client.cpp \
				Channel.cpp \
				utils.cpp \
				Reply.cpp \
				ErrReply.cpp \
				Irc.cpp

#             CMDS              #

SRCS 		+=	Command.cpp \
				CommandManager.cpp \
				NickCommand.cpp \
				UserCommand.cpp \
				JoinCommand.cpp \
				PartCommand.cpp \
				PrivmsgCommand.cpp \
				PingCommand.cpp \
				PassCommand.cpp \
				NoticeCommand.cpp \
				ModeCommand.cpp \
				QuitCommand.cpp \
				NamesCommand.cpp \
				WhoisCommand.cpp \
				MotdCommand.cpp \
				TimeCommand.cpp \

#===============================#
#            HEADERS            #
#===============================#

HEADERS		=	Irc.hpp \
				Server.hpp \
				Client.hpp \
				Channel.hpp \
				common.hpp \
				Command.hpp \
				CommandManager.hpp \
				CommandExecutor.hpp \
				JoinCommand.hpp \
				ModeCommand.hpp \
				NamesCommand.hpp \
				NickCommand.hpp \
				NoticeCommand.hpp \
				PartCommand.hpp \
				PassCommand.hpp \
				PingCommand.hpp \
				PrivmsgCommand.hpp \
				UserCommand.hpp \
				WhoisCommand.hpp \
				MotdCommand.hpp \
				TimeCommand.hpp \

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
CFLAGS		=	-g3#-Wall -Wextra -Werror -std=c++98

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
	@${CC} ${CFLAGS} -c $< -o $@ -I${INCS_DIR}
	@echo "[${NAME}] Compiling ${YELLOW}$<${WHITE}"

${NAME}: ${OBJS} ${LIBFT_SRCS}
	@echo "[${NAME}] Creating executable..."
	@${CC} -o ${NAME} ${OBJS}
	@echo "[${NAME}] ${GREEN}Compilation successful!${WHITE}"


#            .PHONY             #

.PHONY: all clean fclean re .c.o
