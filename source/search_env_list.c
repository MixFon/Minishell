/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_env_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 11:24:57 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 12:04:37 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_char(char *line, char c)
{
	while (*line != '\0')
	{
		if (*line == c)
			return (1);
		line++;
	}
	return (0);
}

void	del_one_lies(t_env **env)
{
	ft_strdel(&(*env)->name);
	ft_strdel(&(*env)->value);
	free(*env);
}

void	unsetenv_list(t_shell *shell, char *name)
{
	t_env *env;
	t_env *pre;

	env = shell->env;
	if (!ft_strcmp(env->name, name))
	{
		shell->env = shell->env->next;
		del_one_lies(&env);
		return ;
	}
	while (env != NULL)
	{
		if (!ft_strcmp(env->name, name))
		{
			pre->next = env->next;
			del_one_lies(&env);
			return ;
		}
		pre = env;
		env = env->next;
	}
}

void	command_unsetenv(t_shell *shell, char **command)
{
	if (command[1] == NULL)
	{
		ft_putendl("unsetenv: wrong arguments.");
		return ;
	}
	unsetenv_list(shell, command[1]);
	free_envir(shell);
	renew_env(shell);
}

t_env	*search_env_list(t_env *env, char *name)
{
	while (env != NULL)
	{
		if (!ft_strcmp(env->name, name))
			return (env);
		env = env->next;
	}
	return (NULL);
}
