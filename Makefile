# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchabot <bchabot@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/25 20:18:45 by rlaforge          #+#    #+#              #
#    Updated: 2023/09/07 19:02:12 by bchabot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS =	src/main.cpp \
		src/server/Server.cpp \
		src/server/cmds/cmdAuth.cpp \
		src/server/cmds/cmdInvite.cpp \
		src/server/cmds/cmdJoin.cpp \
		src/server/cmds/cmdKick.cpp \
		src/server/cmds/cmdPart.cpp \
		src/server/cmds/cmdPrivmsg.cpp \
		src/server/cmds/cmdTopic.cpp \
		src/server/cmds/cmdMode.cpp \
		src/channel/Channel.cpp \
		src/client/Client.cpp


OBJS = $(SRCS:.cpp=.o)

CFLAGS = -Wall -Werror -Wextra -std=c++98 -g

RM = rm -f

CC = c++

all: logo ${NAME}

logo :
	@echo " \e[1;34m __    __   ______   __  __   ______    \e[0m"
	@echo " \e[1;34m/\ \"-./  \ /\  __ \ /\ \/ /  /\  ___\   \e[0m"
	@echo " \e[1;34m\ \ \-./\ \\\\\\ \  __ \\\\\\ \  _\"-.\ \  __\   \e[0m"
	@echo " \e[1;34m \ \_\ \ \_\\\\\\ \_\ \_\\\\\\ \_\ \_\\\\\\ \_____\ \e[0m"
	@echo " \e[1;34m  \/_/  \/_/ \/_/\/_/ \/_/\/_/ \/_____/ \e[0m\n\n"

${NAME} :	${OBJS}
		@${CC} ${OBJS} -o ${NAME}
		@echo "\e[1A\e[2K\e[1;32m📦 $(NAME) compiled!\n\e[0m"

%.o	: %.cpp
		@${CC} ${CFLAGS} $< -c -o $@
		@echo "\e[1A\e[2K\e[1m🗜️ Compiling $@\e[0m"

clean:
		@echo "\e[1;91m🧹 Cleaning binary files\e[0m"
		${RM} ${OBJS}

fclean:	clean
		@echo "\e[1;91m🧹 Cleaning executable\e[0m"
		${RM} ${NAME}

start: fclean re
		clear && ./${NAME}

re: fclean all

.PHONY: all clean fclean logo re



