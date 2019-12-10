#include "minishell.h"

void	dell_arr(char **arr)
{
	while(*arr != NULL)
	{
		ft_strdel(arr);
		arr++;
	}
}

void	working_line(t_shell *shell, char *line)
{
	char **com;
	char **path;
	char *name;

	path = shell->path;
	com = ft_strsplit(line, ' ');
	if (com == NULL)
		return ;
	while (*path != NULL)
	{
		name = ft_multi_strdup(3, *path, "/", com[0]);
		ft_printf("name = {%s}\n", name);
		if (access(name, F_OK | R_OK | X_OK) == 0)
			execve(name, com + 1, NULL);
		ft_strdel(&name);
		path++;
	}
	print_arr(com);
	dell_arr(com);
	free(com);
}

void	start_shell(t_shell *shell)
{
	char *line;

	ft_putstr("$>");
	while(get_next_line(0, &line))
	{
		ft_printf("line = {%s}\n", line);
		working_line(shell, line);
		ft_putstr("$>");
		ft_strdel(&line);
	}
}

void	print_arr(char **arr)
{
	if (arr == NULL)
		return ;
	ft_putstr("!@");
	while (*arr != NULL)
	{
		ft_printf("arr [%s]\n", *arr);
		arr++;
	}
}

void	print_shell(t_shell *shell)
{
	ft_printf("home = [%s]\n", shell->home);
	ft_printf("pwd = [%s]\n", shell->pwd);
	print_arr(shell->path);
}

void	working_env(t_shell *shell, char *env)
{
	if (!(ft_strncmp("HOME=", env, 5)))
		shell->home = ft_strdup(env + 5);
	else if (!(ft_strncmp("PWD=", env, 4)))
		shell->pwd = ft_strdup(env + 4);
	else if (!(ft_strncmp("PATH=", env, 5)))
		shell->path = ft_strsplit(env + 5, ':');
}

void	init(t_shell *shell, char **env)
{
	while(*env != NULL)
	{
		working_env(shell, *env);
		env++;
	}
	print_shell(shell);
}

void	print_error(char **av)
{
	char *err;

	err = "Too many arguments.\n\n\
   Use:\n\
   ./minishell\n";
	sys_err(err);
}

int		main(int ac, char **av, char **env)
{
	t_shell	shell;

	if (ac != 1)
		print_error(av);
	init(&shell, env);
	start_shell(&shell);
	return (0);
}
/*
	char *str[10];
   
	str[0] = "-l";
	str[1] = "-a";
	execve("/bin/ls", str, env);
	exit(0);
 */
