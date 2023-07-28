#include "main.h"
/**
 * empty_or_not - checks for empty lines
 * @line: line input
 * Return: boolean
 */
bool empty_or_not(const char *line)
{
	while (*line)
	{
		if (!isspace(*line))
			return (false);
		line++;
	}
	return (true);
}
/**
 * check_exit - executes exit command
 * @token_count: args count
 * @args: args
 * @line: line
 * @input: input
 */
void check_exit(char *args[], int token_count, char *line, FILE *input)
{
	int exit_st;

	if (token_count > 1)
	{
		exit_st = my_atoi(args[1]);
		if (exit_st < 0)
		{
			fprintf(stderr, "./hsh: 1: exit: Illegal number: %d\n", exit_st);
			exit(2);
		}
		cleanup(line, input);
		exit(exit_st);
	}
	else
	{
		cleanup(line, input);
		exit(0);
	}
}
/**
 * cd_home - handles "cd" command with no arguments (cd to $HOME)
 * Return: 0 on success, 1 on failure
 */
int cd_home(void)
{
	const char *home = my_getenv("HOME");

	if (home == NULL)
		return (1);
	return (chdir_and_update_env(home));
}
/**
 * chdir_and_update_env - Handles chdir and updates PWD and OLDPWD
 * @directory: The directory to change to
 * Return: 0 on success, 1 on failure
 */
int chdir_and_update_env(const char *directory)
{
	char current_dir[MAX_PATH_LENGTH], new_dir[MAX_PATH_LENGTH];

	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(directory) != 0)
	{
		fprintf(stderr, "./hsh: 1: cd: can't cd to %s\n", directory);
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
	if (setenv("PWD", new_dir, 1) != 0)
	{
		perror("setenv");
		return (1);
	}
	return (0);
}
