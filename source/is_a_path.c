/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_a_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 11:19:17 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 12:04:22 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dell_arr(char ***arr)
{
	int i;

	i = -1;
	if (arr == NULL)
		return ;
	if (*arr == NULL)
		return ;
	if (**arr == NULL)
		return ;
	while (arr[0][++i] != NULL)
		ft_strdel(&arr[0][i]);
	free(*arr);
}

int		check_access(char *name_bin)
{
	if (access(name_bin, X_OK) == -1)
	{
		ft_printf("minishell: permission denied: %s\n", name_bin);
		return (1);
	}
	else if (access(name_bin, R_OK) == -1)
	{
		ft_printf("minishell: not read: %s\n", name_bin);
		return (1);
	}
	return (0);
}

void	start_new_process(t_shell *shell, char **command, char *name_bin)
{
	pid_t	child;

	if (check_access(name_bin))
		return ;
	child = fork();
	if (child == 0)
		if (execve(name_bin, command, shell->environment) == -1)
		{
			ft_printf("Error opening %s file\n", name_bin);
			exit(0);
		}
	if (wait(NULL) == -1)
		ft_putendl("Error wait");
}

void	is_no_path(t_shell *shell, char **command)
{
	if (access(*command, F_OK) == 0)
		start_new_process(shell, command, *command);
	else
		ft_printf("minishell: command not found: %s\n", command[0]);
}

void	is_a_path(t_shell *shell, char **command)
{
	char **path;
	char *name_bin;

	path = shell->path;
	while (*path != NULL)
	{
		if (**command == '/')
			name_bin = ft_strdup(*command);
		else
			name_bin = ft_multi_strdup(3, *path, "/", *command);
		if (access(name_bin, F_OK) == 0)
		{
			start_new_process(shell, command, name_bin);
			break ;
		}
		ft_strdel(&name_bin);
		path++;
	}
	if (*path == NULL)
		ft_printf("minishell: command not found: %s\n", command[0]);
	ft_strdel(&name_bin);
}
