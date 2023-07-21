#include "main.h"
/**
 * main - simple shell for Unix based systems
 * @argc: arguments count
 * @argv: arguments of main
 * @line: buffer stores input from stdin
 * @size: size of line buffer
 * @chars_read: reads input from stdin
 * @pid: forks a new process
 * @args: stores arguments after tokenizing
 * @token: tokenize stdin
 * @token_count: keeps track of tokens number
 * @status: status of the forked process
 * @path_copy: copies env path
 * @path: gets env path
 * @full_path: rearrange env path and arguments in a string
 * @token_path: tokenize env path
 * @stat: struct to store paths
 * @command_executed: command execution status
 * @exit_status: exit status
 * @directory: directory
 * @current_dir: current directory
 * @new_dir: new directory
 * @i: int for multiple purposes
 * @j: int used for printing alias args
 * @alias_arg: string to copy alias arg
 * @alias_name: stores alias name
 * @alias_value: stores alias value
 * @alias_args: stores alias args
 * @alias_token_count: keeps track of alias tokens
 * @alias_token: tokenize alias args
 * @alias: string to print alias name and value
 * @aliases: stores aliases set
 * @num_aliases: number of aliases set
 * @comments_mode: detects comment mode in the shell
 * @file_mode: detects file mode input
 * @input_file: input file passed to shell
 * @cd_result: checks the cd status
 * @is_builtin_command: checks for builtins
 * Return: 0
 */
int main(int argc, char **argv)
{
	size_t size = 0;
	ssize_t chars_read;
	int token_count = 0, status, num_aliases = 0, cd_result, is_builtin_command;
	int last_exit_status = 0;
	char *line = NULL, *args[MAX_ARGS], *aliases[MAX_ALIASES];
	bool command_executed = false, comments_mode = false, file_mode = false;
	FILE *input_file = NULL;

	process_mode(argc, argv, &comments_mode, &file_mode, &input_file);
	while (1)
	{
		if (file_mode)
		{
			chars_read = getline(&line, &size, input_file);
			if (chars_read == -1)
				break;
		}
		else
		{
			if (comments_mode)
				printf("# ");
			else
				printf("$ ");
			fflush(stdout);
			chars_read = getline(&line, &size, stdin);
			if (comments_mode || line[0] == '#')
				continue;
		}
		if (line[0] == '\n' || chars_read == '0' || chars_read == (ssize_t)EOF)
			break;
		tokenize(line, args, &token_count, MAX_ARGS);
		if (token_count > 0)
		{
			handle_variable_replacement(args, &token_count, last_exit_status);
			is_builtin_command = execute_builtin_command(args, token_count);
        	if (is_builtin_command)
            	continue;
			else if (strcmp(args[0], "cd") == 0)
			{
				cd_result = change_directory(args);
            	if (cd_result != 0)
					fprintf(stderr, "Invalid usage of cd command\n");
				continue;
			}
			else if (strcmp(args[0], "alias") == 0)
			{
				handle_alias_case(args, aliases, &num_aliases, token_count);
				continue;
			}
			else
				path(args, &command_executed, &status);
			if (command_executed == false)
				fprintf(stderr, "%s: command not found\n", args[0]);
		}
		command_executed = false;
		last_exit_status = status;
	}
	cleanup(aliases, num_aliases, line, input_file);
	return (0);
}