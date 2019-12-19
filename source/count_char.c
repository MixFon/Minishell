/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 11:26:11 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 11:50:27 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_new_env(t_shell *shell, char *name, char *value)
{
	t_env *new;

	if (!(new = (t_env *)malloc(sizeof(t_env))))
		sys_err("Error malloc.\n");
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	new->next = shell->env;
	shell->env = new;
}

int		check_value(char *line, char *value)
{
	char	**arr;
	int		i;

	i = -1;
	arr = ft_strsplit(line, ':');
	while (arr[++i] != NULL)
		if (!(ft_strcmp(arr[i], value)))
		{
			dell_arr(&arr);
			return (1);
		}
	dell_arr(&arr);
	return (0);
}

void	setenv_list(t_shell *shell, char *name, char *value)
{
	t_env	*new;
	char	*temp;

	if (name == NULL || value == NULL)
		return ;
	if ((new = search_env_list(shell->env, name)) == NULL)
		add_new_env(shell, name, value);
	else
	{
		if (check_value(new->value, value))
			return ;
		temp = new->value;
		new->value = ft_multi_strdup(3, temp, ":", value);
		ft_strdel(&temp);
	}
}

void	command_setenv(t_shell *shell, char **command)
{
	char **kv;

	if (command[1] == NULL)
		return ;
	if (is_char(command[1], '='))
	{
		kv = ft_strsplit(command[1], '=');
		setenv_list(shell, kv[0], kv[1]);
		dell_arr(&kv);
	}
	else
		setenv_list(shell, command[1], command[2]);
	free_envir(shell);
	renew_env(shell);
}

int		count_char(char *line, char c)
{
	int count;

	count = 0;
	while (*line != '\0')
	{
		if (*line == c)
			count++;
		line++;
	}
	return (count);
}
