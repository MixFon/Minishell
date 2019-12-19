/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_list_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 11:35:20 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 12:05:35 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		len_list(t_env *env)
{
	int i;

	i = 0;
	while (env != NULL)
	{
		i++;
		env = env->next;
	}
	return (i);
}

void	renew_env(t_shell *shell)
{
	t_env	*env;
	int		len;
	int		i;

	i = 0;
	len = len_list(shell->env);
	if (!(shell->environment = (char **)malloc(sizeof(char *) * (len + 1))))
		sys_err("Error malloc\n");
	env = shell->env;
	shell->environment[len] = NULL;
	shell->home = NULL;
	shell->path = NULL;
	shell->pwd = getcwd(NULL, 0);
	while (env != NULL)
	{
		create_envir(shell, env->name, env->value);
		shell->environment[i] = ft_multi_strdup(3, env->name, "=", env->value);
		env = env->next;
		i++;
	}
}

void	print_error(char **av)
{
	char *err;

	av = NULL;
	err = "Too many arguments.\n\n\
	Use:\n\
	./minishell\n";
	sys_err(err);
}

void	free_envir(t_shell *shell)
{
	ft_strdel(&shell->pwd);
	ft_strdel(&shell->home);
	ft_strdel(&shell->pre_path);
	dell_arr(&shell->path);
	dell_arr(&shell->environment);
}

void	print_list_env(t_env *env)
{
	while (env != NULL)
	{
		ft_printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
}
