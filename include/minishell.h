/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 13:03:50 by widraugr          #+#    #+#             */
/*   Updated: 2019/12/19 13:12:04 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdlib.h>

typedef struct		s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct		s_shell
{
	char			*home;
	char			*pwd;
	char			*pre_path;
	char			**path;
	char			**environment;
	t_env			*env;
}					t_shell;

void				print_arr(char **arr, char c);
int					check_command(char **command);
void				free_envir(t_shell *shell);
void				renew_env(t_shell *shell);
int					not_print_c(char c);
void				signal_work(int signal);
void				print_list_env(t_env *env);
/*
** File is_a_path.c
*/
void				dell_arr(char ***arr);
int					check_access(char *name_bin);
void				start_new_process(t_shell *shell, char **command,
		char *name_bin);
void				is_no_path(t_shell *shell, char **command);
void				is_a_path(t_shell *shell, char **command);
/*
** File check_command.c
*/
void				starting_bin(t_shell *shell, char *line);
char				*create_name_path(t_shell *shell, char **command);
void				new_dir(t_shell *shell, char *new_dir);
void				command_cd(t_shell *shell, char **command);
int					check_command(char **command);
/*
** File search_env_list.c
*/
int					is_char(char *line, char c);
void				del_one_lies(t_env **env);
void				unsetenv_list(t_shell *shell, char *name);
void				command_unsetenv(t_shell *shell, char **command);
t_env				*search_env_list(t_env *env, char *name);
/*
** File count_char.c
*/
void				add_new_env(t_shell *shell, char *name, char *value);
int					check_value(char *line, char *value);
void				setenv_list(t_shell *shell, char *name, char *value);
void				command_setenv(t_shell *shell, char **command);
int					count_char(char *line, char c);
/*
** File exit_env_setenv_cd.c
*/
void				getenv_list(t_shell *shell, char *name);
void				print_arr_echo(char **arr, char c);
void				working_echo(char *line);
void				command_echo(char **command, char *line);
void				exit_env_setenv_cd(t_shell *shell, char **command, int *bl);
/*
** File add_tilda_dollar.c
*/
int					starting_builtins(t_shell *shell, char *line);
int					not_print_c(char c);
size_t				lencstr(char *line);
char				*work_bollar(t_shell *shell, char *line,
		char *new_line, int i);
char				*add_tilda_dollar(t_shell *shell, char *line);
/*
** File create_envir.c
*/
char				*parsing_line(t_shell *shell, char **line);
void				print_greeting(char *pwd);
void				start_shell(t_shell *shell);
void				print_arr(char **arr, char c);
void				create_envir(t_shell *shell, char *name, char *value);
/*
** File print_list_env.c
*/
int					len_list(t_env *env);
void				renew_env(t_shell *shell);
void				print_error(char **av);
void				free_envir(t_shell *shell);
void				print_list_env(t_env *env);
/*
** File init_environment.c
*/
t_env				*chreate_env(char *line);
void				init_environment(t_shell *shell, char **env);
void				dell_strs(char **one, char **two);
void				delete_comm_line(t_shell *shell, char ***command,
		char **line);
/*
** File main.c
*/
void				init(t_shell *shell, char **env);
void				signal_work(int signal);
void				set_signal_handler(void);
void				delete_list_env(t_shell *shell);
int					main(int ac, char **av, char **env);
#endif
