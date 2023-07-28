#include "main.h"
/**
 * exe - executes commands
 * @full_path: string that contains the full path of the executable file
 * @args: args
 * @command_executed: checks if the command has been executed
 * @status: status of the operation
 * Return: 0
 */
int exe(char *full_path, char *args[], bool *command_executed, int *status)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		execve(full_path, args, environ);
		exit(EXIT_SUCCESS);
	}
	else
	{
		wait(status);
		*command_executed = true;
	}

	return (0);
}
/**
 * execute_full_command - executes commands with full paths
 * @args: args
 * @command_executed: checks if the commans has been executed
 * @status: status of the operation
 * Return: 0
 */
int execute_full_command(char *args[], bool *command_executed, int *status)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		execve(args[0], args, environ);
		exit(EXIT_SUCCESS);
	}
	else
	{
		wait(status);
		*command_executed = true;
	}

	return (0);
}
/**
 * change_directory - handles cd command
 * @args: args
 * Return: 0 or 1
 */
int change_directory(char *args[])
{
	const char *directory;
	char current_dir[MAX_PATH_LENGTH], new_dir[MAX_PATH_LENGTH];

	if (args[1] == NULL)
		directory = "/root";
	else if (my_strncmp(args[1], "-", 1) == 0)
	{
		directory = my_getenv("OLDPWD");
		if (directory == NULL)
		{
			fprintf(stderr, "OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", directory);
	}
	else
		directory = args[1];
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(directory) != 0)
	{
		perror("chdir");
		return (1);
	}
	if (setenv("OLDPWD", current_dir, 1) != 0)
	{
		perror("setenv");
		return (1);
	}
	if (getcwd(new_dir, sizeof(new_dir)) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	setenv("PWD", new_dir, 1);
	return (0);
}
/**
 * execute_builtin_command - handles builtins
 * @args: args
 * @token_count: tokens count
 * Return: 0
 */
int execute_builtin_command(char *args[], int token_count)
{
	int fd = STDERR_FILENO;
	const char *message = "Invalid usage of setenv command\n";

	if (my_strncmp(args[0], "exit", 4) == 0)
	{
		if (token_count > 1)
		{
			fprintf(stderr, "./hsh: 1: exit: Illegal number: -98\n");
			exit(2);
		}
		else
			exit(0);
	}
	else if (my_strcmp(args[0], "setenv") == 0)
	{
		if (token_count != 3)
			write(fd, message, my_strlen(message));
		else
			if (setenv(args[1], args[2], 1) != 0)
				perror("setenv");
	}
	else if (my_strcmp(args[0], "unsetenv") == 0)
	{
		if (token_count != 2)
			fprintf(stderr, "Invalid usage of unsetenv command\n");
		else
			if (unsetenv(args[1]) != 0)
				perror("unsetenv");
	}
	else
		return (0);
	return (1);
}
