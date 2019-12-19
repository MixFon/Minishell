/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tilda_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 11:30:44 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 11:49:12 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		starting_builtins(t_shell *shell, char *line)
{
	char	**command;
	int		bl;

	bl = 0;
	command = ft_strsplit(line, ' ');
	if (check_command(command))
	{
		free(command);
		return (1);
	}
	exit_env_setenv_cd(shell, command, &bl);
	if (!(ft_strcmp("unsetenv", command[0])))
	{
		command_unsetenv(shell, command);
		bl = 1;
	}
	else if (!(ft_strcmp("echo", command[0])))
	{
		command_echo(command, line);
		bl = 1;
	}
	dell_arr(&command);
	return (bl);
}

int		not_print_c(char c)
{
	if (c > 0 && c < 32)
		return (1);
	return (0);
}

size_t	lencstr(char *line)
{
	size_t i;

	i = 0;
	while (line[i] > 32 && line[i] != '"')
		i++;
	return (i);
}

char	*work_bollar(t_shell *shell, char *line, char *new_line, int i)
{
	char	*rez;
	size_t	len;
	t_env	*env;
	char	*temp;

	len = lencstr(line + i);
	temp = ft_strnew(len);
	temp = ft_strncpy(temp, line + i + 1, len - 1);
	if ((env = search_env_list(shell->env, temp)) == NULL)
		rez = ft_multi_strdup(2, new_line, line + i + len);
	else
		rez = ft_multi_strdup(3, new_line, env->value, line + i + len);
	ft_strdel(&temp);
	return (rez);
}

char	*add_tilda_dollar(t_shell *shell, char *line)
{
	char	*new_line;
	char	*temp;
	int		i;

	i = -1;
	new_line = ft_strnew(ft_strlen(line));
	while (line[++i] != '\0')
	{
		if (line[i] == '~' && line[i + 1] == '/')
		{
			temp = ft_multi_strdup(3, new_line, shell->home, line + i + 1);
			dell_strs(&line, &new_line);
			return (add_tilda_dollar(shell, temp));
		}
		else if (line[i] == '$')
		{
			temp = work_bollar(shell, line, new_line, i);
			dell_strs(&line, &new_line);
			return (add_tilda_dollar(shell, temp));
		}
		new_line[i] = line[i];
	}
	ft_strdel(&line);
	return (new_line);
}
