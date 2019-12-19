/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_setenv_cd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 11:28:01 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 12:00:50 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	getenv_list(t_shell *shell, char *name)
{
	t_env *env;

	if ((env = search_env_list(shell->env, name)) == NULL)
		return ;
	ft_putstr(env->value);
}

void	print_arr_echo(char **arr, char c)
{
	if (arr == NULL)
		return ;
	ft_putstr(*arr);
	arr++;
	while (*arr != NULL)
	{
		ft_putchar(c);
		ft_putstr(*arr);
		arr++;
	}
	ft_putchar('\n');
}

void	working_echo(char *line)
{
	int		bl;

	bl = 1;
	line = ft_strstr(line, "echo") + 4;
	while (*(line + 1) == ' ')
		line++;
	while (*(++line) != '\0')
	{
		if (*line == '"')
		{
			bl = !bl;
			continue;
		}
		if (bl && *line == ' ' && *(line + 1) == ' ')
			continue;
		else
			ft_putchar(*line);
	}
	ft_putchar('\n');
}

void	command_echo(char **command, char *line)
{
	int		count;

	count = count_char(line, '"');
	if (command[1] == NULL)
		return ;
	if (count % 2 != 0)
	{
		ft_putendl("echo: error double quotes.");
		return ;
	}
	else if (count == 0)
		print_arr_echo(command + 1, ' ');
	else
		working_echo(line);
}

void	exit_env_setenv_cd(t_shell *shell, char **command, int *bl)
{
	if (!(ft_strcmp("exit", command[0])))
		exit(0);
	else if (!(ft_strcmp("env", command[0])))
	{
		print_list_env(shell->env);
		*bl = 1;
	}
	else if (!(ft_strcmp("setenv", command[0])))
	{
		command_setenv(shell, command);
		*bl = 1;
	}
	else if (!(ft_strcmp("cd", command[0])))
	{
		command_cd(shell, command);
		*bl = 1;
	}
}
