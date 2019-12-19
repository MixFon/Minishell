/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_envir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 11:33:23 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 12:24:39 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parsing_line(t_shell *shell, char **line)
{
	char	*new_line;
	int		i;
	int		bl;

	i = -1;
	bl = 1;
	*line = add_tilda_dollar(shell, *line);
	new_line = ft_strnew(ft_strlen(*line));
	while ((*line)[++i] != '\0')
	{
		if ((*line)[i] == '"')
			bl = !bl;
		if (not_print_c((*line)[i]) && bl)
			new_line[i] = ' ';
		else
			new_line[i] = (*line)[i];
	}
	ft_strdel(line);
	return (new_line);
}

void	print_greeting(char *pwd)
{
	char *line;

	line = pwd;
	while (*line != '\0')
		line++;
	while (*line != '/')
		line--;
	if (*(line + 1) == '\0')
		ft_putstr("/ $>");
	else
		ft_printf("%s $>", line + 1);
}

void	start_shell(t_shell *shell)
{
	char	*line;
	char	**command;
	int		i;

	print_greeting(shell->pwd);
	while (get_next_line(0, &line))
	{
		i = -1;
		command = ft_strsplit(line, ';');
		if (check_command(command))
		{
			delete_comm_line(shell, &command, &line);
			continue;
		}
		while (command[++i] != NULL)
		{
			command[i] = parsing_line(shell, &command[i]);
			if (!(starting_builtins(shell, command[i])))
				starting_bin(shell, command[i]);
		}
		dell_arr(&command);
		print_greeting(shell->pwd);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	print_arr(char **arr, char c)
{
	if (arr == NULL)
		return ;
	ft_printf("%s", *arr);
	arr++;
	while (*arr != NULL)
	{
		ft_printf("%c%s", c, *arr);
		arr++;
	}
	ft_putchar('\n');
}

void	create_envir(t_shell *shell, char *name, char *value)
{
	if (!(ft_strcmp("HOME", name)))
		shell->home = ft_strdup(value);
	else if (!(ft_strcmp("PATH", name)))
		shell->path = ft_strsplit(value, ':');
}
