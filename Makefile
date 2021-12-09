# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/26 11:13:50 by nneronin          #+#    #+#              #
#    Updated: 2021/12/09 12:20:54 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL_NAME	:= $(shell uname -s)
name = dontcare

all: 
ifeq ($(SHELL_NAME), Darwin)
	@make -f Makefile-mac
else
	@echo "OS: $(SHELL_NAME)"
	@echo "Making windows version."
	@mingw32-make -f Makefile-win
endif

clean:
ifeq ($(SHELL_NAME), Darwin)
	@make clean -f Makefile-mac
else
	@mingw32-make clean -f Makefile-win 
endif
	
fclean:
ifeq ($(SHELL_NAME), Darwin)
	@make fclean -f Makefile-mac
else
	@mingw32-make fclean -f Makefile-win 
endif

re: fclean all

.PHONY: clean, all, re, fclean, framework, framework_del, frameworks_re
