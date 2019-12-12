#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

//extern char **environ;

typedef struct	s_shell
{
	char		*home;
	char		*pwd;
	char		**path;
}				t_shell;

void	print_arr(char **arr);
int		check_command(char **command);
void	free_envir(t_shell *shell);
void	init(t_shell *shell);
#endif
