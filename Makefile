# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: widraugr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/29 13:07:44 by widraugr          #+#    #+#              #
#    Updated: 2019/12/19 13:26:36 by widraugr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

NAMEBIN = libft

FILE_C = main.c\
		 add_tilda_dollar.c\
		 check_command.c\
		 count_char.c\
		 create_envir.c\
		 exit_env_setenv_cd.c\
		 init_environment.c\
		 is_a_path.c\
		 print_list_env.c\
		 search_env_list.c

FLAGS =  -Wall -Wextra -Werror -I libft -I include   

FLIB = -L ./libft -lftprintf 

DIRC = ./source/

VPATH = $(DIROBJ)

DIROBJ = ./obj/

OBJ = $(addprefix $(DIROBJ), $(FILE_C:.c=.o))

all : $(NAME)

$(NAME): $(DIROBJ) $(OBJ) 
	make -C libft
	gcc $(FLAGS) $(OBJ) $(FLIB) $(FMLXLIB) -o $(NAME)

$(DIROBJ)%.o : $(DIRC)%.c
	gcc  $(FLAGS) -c $< -o $@

$(DIROBJ):
	mkdir -p $(DIROBJ)

clean:
	/bin/rm -rf $(DIROBJ)
	make clean -C libft
	
fclean: clean
	/bin/rm -f $(NAME)
	make fclean -C libft
	
re: fclean all 
