/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/18 10:34:17 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 12:24:40 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init(t_shell *shell, char **env)
{
	init_environment(shell, env);
	shell->environment = env;
	shell->home = NULL;
	shell->path = NULL;
	shell->pwd = NULL;
	shell->pre_path = NULL;
}

void	signal_work(int signal)
{
	char *pwd;

	signal = 0;
	ft_putchar('\n');
	pwd = getcwd(NULL, 0);
	print_greeting(pwd);
	ft_strdel(&pwd);
}

void	set_signal_handler(void)
{
	signal(SIGINT, signal_work);
	signal(SIGQUIT, signal_work);
	signal(SIGTSTP, signal_work);
}

void	delete_list_env(t_shell *shell)
{
	t_env *temp;

	while (shell->env != NULL)
	{
		temp = shell->env;
		shell->env = shell->env->next;
		del_one_lies(&temp);
	}
}

int		main(int ac, char **av, char **env)
{
	t_shell	shell;

	if (ac != 1)
		print_error(av);
	set_signal_handler();
	init(&shell, env);
	renew_env(&shell);
	start_shell(&shell);
	free_envir(&shell);
	delete_list_env(&shell);
	return (0);
}
