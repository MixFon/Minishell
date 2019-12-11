#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"

typedef struct	s_shell
{
	char		*home;
	char		*pwd;
	char		**path;
	char		**env;
}				t_shell;

void	print_arr(char **arr);
#endif
