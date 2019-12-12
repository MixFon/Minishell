#include "minishell.h"

void	dell_arr(char ***arr)
{
	int i;

	i = -1;
	ft_putendl("ehllo");
	if (arr == NULL)
		return ;
	if (*arr == NULL)
		return ;
	if (**arr == NULL)
		return ;
	while(arr[0][++i] != NULL)
		ft_strdel(&arr[0][i]);
	free(*arr);
}

void	start_new_process(t_shell *shell, char **command,  char *name_bin)
{
	pid_t	child;
	int		*status;

	ft_printf("name no_pat\n");
	child = fork();
	if (child == 0)
		if (execve(name_bin, command, __environ) == -1)
			ft_printf("Error opening %s file\n", name_bin);
	if (wait(NULL) == -1)
		ft_putendl("Error wait");
}

void	is_no_path(t_shell *shell, char **command)
{
	ft_printf("name no_path = {%s}\n", *command);
	if (access(*command, F_OK | R_OK | X_OK) == 0)
		start_new_process(shell, command,  *command);
	else
		ft_printf("minishell: command not found: %s\n", command[0]);
}

void	is_a_path(t_shell *shell, char **command)
{
	char **path;
	char *name_bin;

	ft_putendl("is_a_path");
	path = shell->path;
	while (*path != NULL)
	{
		if (**command == '/')
			name_bin = ft_strdup(*command);
		else
			name_bin = ft_multi_strdup(3, *path, "/", *command);
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
	ft_strdel(&name_bin);
	ft_putendl("is_a_path<");
}

void	starting_bin(t_shell *shell, char *line)
{
	char **command;
	char **path;

	command = ft_strsplit(line, ' ');
	if (check_command(command))
		return ;
	ft_printf("shell->path:\n");
	print_arr(shell->path);
	if (shell->path == NULL)
		is_no_path(shell, command);
	else
		is_a_path(shell, command);
	//print_arr(shell->env);
	//print_arr(command);
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

	if (command[1] == NULL || command[1][0] == '~')
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

int		check_command(char **command)
{
	if (command == NULL)
		return (1);
	if (*command == NULL)
	{
		dell_arr(&command);
		return (1);
	}
	return (0);
}

void	command_env(t_shell *shell)
{
	print_arr(__environ);	
}

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

void	command_unsetenv(t_shell *shell, char **command)
{
	if (command[1] == NULL)
	{
		ft_putendl("unsetenv: wrong arguments.");
		return ;
	}
	unsetenv(command[1]);
	free_envir(shell);
	init(shell);
}

void	command_setenv(t_shell *shell, char **command)
{
	char **kv;

	if (command[1] == NULL)
		return ;
	if (is_char(command[1], '='))
	{
		kv = ft_strsplit(command[1], '=');
		ft_printf("kv[0] = [%s], kv[1] = {%s}\n", kv[0], kv[1]);
		if (setenv(kv[0], kv[1], 1) == -1)
			ft_putendl("Error setenv.");
		dell_arr(&kv);
	}
	else
		if (setenv(command[1], command[2], 1) == -1)
			ft_putendl("Error setenv.");
	free_envir(shell);
	init(shell);
}

int		starting_builtins(t_shell *shell, char *line)
{
	char **command;

	ft_putendl("builtins");
	command = ft_strsplit(line, ' ');
	if (check_command(command))
		return (1);
	if (!(ft_strcmp("exit", command[0])))
		exit(0);
	else if (!(ft_strcmp("env", command[0])))
	{
		command_env(shell);
		dell_arr(&command);
		return (1);
	}
	else if (!(ft_strcmp("setenv", command[0])))
	{
		command_setenv(shell, command);
		dell_arr(&command);
		return (1);
	}
	else if (!(ft_strcmp("unsetenv", command[0])))
	{
		command_unsetenv(shell, command);
		dell_arr(&command);
		return (1);
	}
	else if (!(ft_strcmp("cd", command[0])))
	{
		change_dir(shell, command);
		dell_arr(&command);
		return (1);
	}
	dell_arr(&command);
	ft_putendl("builtins<");
	return (0);
}

int not_print_c(char c)
{
	if (c > 0 && c < 32)
		return (1);
	return (0);
}

char	*parsing_line(char **line)
{
	char	*new_line;
	int		i;

	i = -1;
	new_line = ft_strnew(ft_strlen(*line));
	while ((*line)[++i] != '\0')
	{
		if (not_print_c((*line)[i]))
			new_line[i] = ' ';
		else
			new_line[i] = (*line)[i];
	}
	ft_strdel(line);
	ft_printf("new line = [%s]\n", new_line);
	return (new_line);
}

void	start_shell(t_shell *shell)
{
	char *line;

	ft_putstr("$>");
	while(get_next_line(0, &line))
	{
		ft_printf("line = {%s}\n", line);
		line = parsing_line(&line);
		if (!(starting_builtins(shell, line)))
			starting_bin(shell, line);
		ft_putstr("$>");
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	print_arr(char **arr)
{
	if (arr == NULL)
		return ;
	while (*arr != NULL)
	{
		ft_putendl(*arr);
		arr++;
	}
}

void	create_envir(t_shell *shell, char *temp)
{
	if (!(ft_strncmp("HOME", temp, 4)))
		shell->home = ft_strdup(temp + 5);
	else if (!(ft_strncmp("PATH", temp, 4)))
		shell->path = ft_strsplit(temp + 5, ':');
}

void	init(t_shell *shell)
{
	char **temp;
	
	temp = __environ;
	//dell_arr(&shell->path);
	//shell->env = __environ;
	shell->home = NULL;
	shell->path = NULL;
	shell->pwd  = NULL;
	while (*temp != NULL)
	{
		create_envir(shell, *temp);
		temp++;
	}
}

void	print_error(char **av)
{
	char *err;

	err = "Too many arguments.\n\n\
   Use:\n\
   ./minishell\n";
	sys_err(err);
}

void	free_envir(t_shell *shell)
{
	ft_strdel(&shell->home);
	ft_strdel(&shell->pwd);
	dell_arr(&shell->path);
}

int		main(int ac, char **av)
{
	t_shell	shell;

	if (ac != 1)
		print_error(av);
	init(&shell);
	start_shell(&shell);
	free_envir(&shell);
	return (0);
}
/*
	char *str[10];
   
	str[0] = "-l";
	str[1] = "-a";
	execve("/bin/ls", str, env);
	exit(0);
 */
