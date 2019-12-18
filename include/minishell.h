#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

//extern char **environ;

typedef struct		s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct	s_shell
{
	char		*home;
	char		*pwd;
	char		*pre_path;
	char		**path;
	char		**environment;
	t_env		*env;
}				t_shell;


void	print_arr(char **arr, char c);
int		check_command(char **command);
void	free_envir(t_shell *shell);
void	renew_env(t_shell *shell);
int		not_print_c(char c);
void	signal_work(int signal);
void	print_list_env(t_env *env);
#endif
