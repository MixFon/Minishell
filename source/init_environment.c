/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 11:36:34 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 12:24:37 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*chreate_env(char *line)
{
	t_env	*new;
	size_t	len;

	if (!(new = (t_env *)malloc(sizeof(t_env))))
		sys_err("Error malloc\n");
	len = ft_strcl(line, '=');
	new->name = ft_strnew(len);
	new->name = ft_strncpy(new->name, line, len);
	new->value = ft_strdup(line + len + 1);
	new->next = NULL;
	return (new);
}

void	init_environment(t_shell *shell, char **env)
{
	t_env	*new_env;

	shell->env = NULL;
	new_env = NULL;
	while (*env != NULL)
	{
		if (shell->env == NULL)
			shell->env = chreate_env(*env);
		else
		{
			new_env = chreate_env(*env);
			new_env->next = shell->env;
			shell->env = new_env;
		}
		env++;
	}
}

void	dell_strs(char **one, char **two)
{
	ft_strdel(one);
	ft_strdel(two);
}

void	delete_comm_line(t_shell *shell, char ***command, char **line)
{
	free(*command);
	ft_strdel(line);
	print_greeting(shell->pwd);
}
