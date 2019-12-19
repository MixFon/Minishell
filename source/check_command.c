/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 11:21:43 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 11:50:06 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	starting_bin(t_shell *shell, char *line)
{
	char **command;

	command = ft_strsplit(line, ' ');
	if (check_command(command))
		return ;
	if (shell->path == NULL)
		is_no_path(shell, command);
	else
		is_a_path(shell, command);
	dell_arr(&command);
}

char	*create_name_path(t_shell *shell, char **command)
{
	char *name_path;

	if (command[1][0] == '/')
		name_path = ft_strdup(command[1]);
	else if (command[1][0] == '~' && command[1][1] != '\0')
		name_path = ft_multi_strdup(3, shell->home, "/", command[1] + 2);
	else if (command[1][0] == '~' && command[1][1] == '\0')
		name_path = ft_strdup(shell->home);
	else if (command[1][0] == '-' && command[1][1] == '\0')
	{
		if (shell->pre_path == NULL)
			return (NULL);
		name_path = ft_strdup(shell->pre_path);
	}
	else
	{
		ft_strdel(&shell->pwd);
		shell->pwd = getcwd(NULL, 0);
		name_path = ft_multi_strdup(3, shell->pwd, "/", command[1]);
	}
	return (name_path);
}

void	new_dir(t_shell *shell, char *new_dir)
{
	ft_strdel(&shell->pre_path);
	shell->pre_path = getcwd(NULL, 0);
	chdir(new_dir);
	ft_strdel(&shell->pwd);
	shell->pwd = getcwd(NULL, 0);
}

void	command_cd(t_shell *shell, char **command)
{
	char *name_path;

	if (command[1] == NULL)
	{
		new_dir(shell, shell->home);
		return ;
	}
	else if (command[2] != NULL)
	{
		ft_putendl("cd: too many arguments.");
		return ;
	}
	if ((name_path = create_name_path(shell, command)) == NULL)
	{
		ft_putendl("There is no previous path.");
		return ;
	}
	if (access(name_path, F_OK) == 0)
	{
		if (!check_access(name_path))
			new_dir(shell, name_path);
	}
	else
		ft_printf("cd: no such file or directory: %s\n", command[1]);
	ft_strdel(&name_path);
}

int		check_command(char **command)
{
	if (command == NULL)
		return (1);
	if (*command == NULL)
	{
		dell_arr(&command);
		return (1);
	}
	return (0);
}
