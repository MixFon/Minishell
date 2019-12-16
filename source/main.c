#include "minishell.h"

void	dell_arr(char ***arr)
{
	int i;

	i = -1;
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
		if (execve(name_bin, command, environ) == -1)
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
	print_arr(shell->path, '\n');
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
	else if (command[1][0] == '~' && command[1][1] != '\0')
		name_path = ft_multi_strdup(3, shell->home, "/", command[1] + 2);
	else if (command[1][0] == '~' && command[1][1] == '\0')
		name_path = ft_strdup(shell->home);
	else if (command[1][0] == '-' && command[1][1] == '\0')
	{
		if (shell->pre_path == NULL)
			return (NULL);
		name_path = ft_strdup(shell->pre_path);
	}
	else
	{
		ft_strdel(&shell->pwd);
		shell->pwd = getcwd(NULL, 0);
		name_path = ft_multi_strdup(3, shell->pwd, "/", command[1]);
	}
	ft_printf("name_path = {%s}\n", name_path);
	return (name_path);
}

void	new_dir(t_shell *shell, char *new_dir)
{
	ft_strdel(&shell->pre_path);
	shell->pre_path = getcwd(NULL, 0);	
	chdir(new_dir);
	ft_strdel(&shell->pwd);
	shell->pwd = getcwd(NULL, 0);
}

void	command_cd(t_shell *shell, char **command)
{
	char *name_path;

	if (command[1] == NULL)
	{
		new_dir(shell, shell->home);
		return ;
	}
	else if (command[2] != NULL)
	{
		ft_printf("cd: too many arguments\n"); 
		return ;
	}
	if((name_path = create_name_path(shell, command)) == NULL)
	{
		ft_putendl("There is no previous path.");
		return ;
	}
	if (access(name_path, F_OK | R_OK | X_OK) == 0)
		new_dir(shell, name_path);
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
	print_arr(environ, '\n');	
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
	renew_env(shell);
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

char	*dollar(char *line)
{
	char *temp;
	char *new_str;

	temp = line;
	while (*temp != '\0' && *temp != ' ' && !not_print_c(*temp) &&
			*temp != '"')
		temp++;
	new_str = ft_strnew(temp - line);
	new_str = ft_strncat(new_str, line, temp - line);
	line = getenv(new_str + 1);
	ft_putstr(line);
	ft_strdel(&new_str);
	return (temp);
}
void	print_arr_echo(char **arr, char c)
{
	if (arr == NULL)
		return ;
	if (**arr == '$')
		dollar(*arr);
	else
		ft_printf("%s", *arr);
	arr++;
	while (*arr != NULL)
	{
		ft_putchar(c);
		if (**arr == '$')
			dollar(*arr);
		else
			ft_putstr(*arr);
		arr++;
	}
	ft_putchar('\n');
}

void	working_echo(char *line)
{
	int		bl;

	bl = 1;
	line = ft_strstr(line, "echo") + 4;
	while (*(line + 1) == ' ')
		line++;
	while(*(++line) != '\0')
	{
		if (*line == '$')
			line = dollar(line);
		if (*line == '"')
		{
			bl = !bl;
			continue;
		}
		if (bl && *line == ' ' && *(line + 1) == ' ')
			continue;
		else
			ft_putchar(*line);
	}
	ft_putchar('\n');
}

void	command_echo(t_shell *shell, char **command, char *line)
{
	char	**arr;
	int		count;

	count = count_char(line, '"');
	ft_printf("count = {%d}\n", count);
	if (command[1] == NULL)
		return ;
	if (count % 2 != 0)
	{
		ft_putendl("echo: error double quotes.");
		return ;
	}
	else if (count == 0)
		print_arr_echo(command + 1, ' ');
	else
		working_echo(line);
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
		command_cd(shell, command);
		dell_arr(&command);
		return (1);
	}
	else if (!(ft_strcmp("echo", command[0])))
	{
		command_echo(shell, command, line);
		dell_arr(&command);
		return (1);
	}
	dell_arr(&command);
	ft_putendl("builtins<");
	return (0);
}

int		not_print_c(char c)
{
	if (c > 0 && c < 32)
		return (1);
	return (0);
}

char	*add_tilda_path(t_shell *shell, char *line)
{
	char	*new_line;
	char	*temp;
	int		i;

	i = -1;
	//ft_printf("line tilda = [%s]\n", line);
	//ft_printf("shell->home = [%s]\n", shell->home);
	new_line = ft_strnew(ft_strlen(line));
	while (line[++i] != '\0')
	{
		if (line[i] == '~' && line[i + 1] == '/')
		{
			temp = ft_multi_strdup(3, new_line, shell->home, line + i + 1); 
			ft_strdel(&line);
			ft_strdel(&new_line);
			return (add_tilda_path(shell, temp));
		}
		new_line[i] = line[i];
	}
	ft_strdel(&line);
	//ft_printf("new_line tilda = [%s]\n", new_line);
	return (new_line);
}

char	*parsing_line(t_shell *shell, char **line)
{
	char	*new_line;
	int		i;
	int		bl;

	i = -1;
	bl = 1;
	*line = add_tilda_path(shell, *line);
	new_line =ft_strnew(ft_strlen(*line));
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
	ft_printf("new line = [%s]\n", new_line);
	return (new_line);
}

void	print_greeting(t_shell *shell)
{
	char *line;
	
	line = shell->pwd;
	while (*line != '\0')
		line ++;
	while (*line != '/')
		line--;
	if (*(line + 1) == '\0')
		ft_putstr("/ $>");
	else
		ft_printf("%s $>", line + 1);
}

void	start_shell(t_shell *shell)
{
	char *line;
	char **command;
	int i;

	print_greeting(shell);
	while(get_next_line(0, &line))
	{
		i = -1;
		ft_printf("line = {%s}\n", line);
		command = ft_strsplit(line, ';');
		if (check_command(command))
		{
			print_greeting(shell);
			ft_strdel(&line);
			free(command);
			continue;
		}
		while (command[++i] != NULL)
		{
			command[i] = parsing_line(shell, &command[i]);
			if (!(starting_builtins(shell, command[i])))
				starting_bin(shell, command[i]);
		}
		dell_arr(&command);
		print_greeting(shell);
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

void	create_envir(t_shell *shell, char *temp)
{
	if (!(ft_strncmp("HOME", temp, 4)))
		shell->home = ft_strdup(temp + 5);
	else if (!(ft_strncmp("PATH", temp, 4)))
		shell->path = ft_strsplit(temp + 5, ':');
}

void	renew_env(t_shell *shell)
{
	char **temp;
	
	temp = environ;
	//dell_arr(&shell->path);
	//shell->env = __environ;
	shell->home = NULL;
	shell->path = NULL;
	shell->pwd = getcwd(NULL, 0);
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
	ft_strdel(&shell->pre_path);
	dell_arr(&shell->path);
}

void	init(t_shell *shell)
{
	shell->home = NULL;
	shell->path = NULL;
	shell->pwd = NULL;
	shell->pre_path = NULL;
}

int		main(int ac, char **av)
{
	t_shell	shell;

	if (ac != 1)
		print_error(av);
	init(&shell);
	renew_env(&shell);
	start_shell(&shell);
	free_envir(&shell);
	return (0);
}
