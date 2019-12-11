#include "minishell.h"

void	dell_arr(char ***arr)
{
	int i;

	i = -1;
	while(arr[0][++i] != NULL)
		ft_strdel(&arr[0][i]);
	free(arr[0]);
}

void	start_new_process(t_shell *shell,char **command,  char *name_bin)
{
	pid_t	child;
	int		*status;

	child = fork();
	if (child == 0)
		if (execve(name_bin, command, NULL) == -1)
			ft_printf("Error opening %s file\n", name_bin);
	if (wait(NULL) == -1)
		ft_putendl("Error wait");
	ft_printf("Status = [%d}\n", *status);
}

void	starting_bin(t_shell *shell, char *line)
{
	char **command;
	char **path;
	char *name_bin;

	path = shell->path;
	command = ft_strsplit(line, ' ');
	if (command == NULL)
		return ;
	while (*path != NULL)
	{
		name_bin = ft_multi_strdup(3, *path, "/", command[0]);
		ft_printf("name = {%s}\n", name_bin);
		if (access(name_bin, F_OK | R_OK | X_OK) == 0)
		{
			start_new_process(shell, command,  name_bin);
			break ;
		}
		ft_strdel(&name_bin);
		path++;
	}
	if (*path == NULL)
		ft_printf("minishell: command not found: %s\n", command[0]);
	//print_arr(shell->env);
	//print_arr(command);
	ft_strdel(&name_bin);
	dell_arr(&command);
}

char	*create_name_path(t_shell *shell, char **command)
{
	char *name_path;
	
	if (command[1][0] == '/')
		name_path = ft_strdup(command[1]);
	else
	{
		ft_strdel(&shell->pwd);
		shell->pwd = getcwd(NULL, 0);
		name_path =  ft_multi_strdup(3, shell->pwd, "/", command[1]);
	}
	ft_printf("name_path = {%s}\n", name_path);
	return (name_path);
}

void	change_dir(t_shell *shell, char **command)
{
	char *name_path;

	if (command[1] == NULL)
	{
		chdir(shell->home);
		return ;
	}
	else if (command[2] != NULL)
		ft_printf("cd: too many arguments\n"); 
	name_path = create_name_path(shell, command);
	if (access(name_path, F_OK | R_OK | X_OK) == 0)
		chdir(name_path);
	else
		ft_printf("cd: no such file or directory: %s\n", command[1]);
	ft_strdel(&name_path);
}

int		starting_builtins(t_shell *shell, char *line)
{
	char **command;

	command = ft_strsplit(line, ' ');
	if (!(ft_strcmp("exit", command[0])))
		exit(0);
	if (!(ft_strcmp("cd", command[0])))
	{
		change_dir(shell, command);
		dell_arr(&command);
		return (1);
	}
	dell_arr(&command);
	return (0);
}

void	start_shell(t_shell *shell)
{
	char *line;

	ft_putstr("$>");
	while(get_next_line(0, &line))
	{
		ft_printf("line = {%s}\n", line);
		if (!(starting_builtins(shell, line)))
			starting_bin(shell, line);
		ft_putstr("$>");
		ft_strdel(&line);
	}
}

void	print_arr(char **arr)
{
	ft_putstr("Print arr\n");
	if (arr == NULL)
		return ;
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
	shell->env = env;
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
