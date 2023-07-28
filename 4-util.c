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
 */
void check_exit(char *args[], int token_count)
{
	int exit_status;

	if (token_count > 1)
	{
		exit_status = my_atoi(args[1]);
		exit(exit_status);
	}
	else
		exit(0);
}
