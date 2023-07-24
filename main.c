#include "main.h"
/**
 * main - simple shell for Unix based systems
 * @argc: arguments count
 * @argv: arguments of main
 * Return: 0
 */
int main(int argc, char **argv)
{
	size_t size = 0;
	ssize_t chars_read;
	int token_count = 0, status, cd_result, builtin, last = 0;
	char *line = NULL, *args[MAX_ARGS];
	bool command_executed = false, sh = false, file_mode = false;
	FILE *input = NULL;

	mode(argc, argv, &sh, &file_mode, &input);
	while (1)
	{
		if (file_mode)
			chars_read = getline(&line, &size, input);
		else
		{
			chars_read = getline(&line, &size, stdin);
			if (sh || line[0] == '#')
				continue;
		}
		if (line[0] == '\n' || chars_read == '0' || chars_read == (ssize_t)EOF)
			break;
		tokenize(line, args, &token_count, MAX_ARGS);
		handle_variable(args, &token_count, last);
		builtin = execute_builtin_command(args, token_count);
		if (builtin)
			continue;
		else if (my_strcmp(args[0], "cd") == 0)
		{
			cd_result = change_directory(args);
			helper2(cd_result);
			continue;
		}
		else
			path(args, &command_executed, &status);
		helper3(&command_executed, args);
		helper4(&command_executed, &last, &status);
	}
	cleanup(line, input);
	return (0);
}