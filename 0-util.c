#include "main.h"
/**
 * mode - checks if the shell mode is in comments or file mode
 * @argc: args count
 * @argv: arguments
 * @sh: bool to check for comments mode
 * @file_mode: bool to check for file mode
 * @input: file passed to shell
 */
void mode(int argc, char *argv[], bool *sh, bool *file_mode, FILE **input)
{
	if (argc == 2)
	{
		if (strcmp(argv[1], "sh") == 0)
			*sh = true;
		else
		{
			*file_mode = true;
			*input = fopen(argv[1], "r");
			if (*input == NULL)
			{
				perror("Error opening file");
				exit(EXIT_FAILURE);
			}
		}
	}
}
/**
 * tokenize - tokenize args
 * @line: line buffer stores stdin
 * @args: args
 * @token_count: token count
 * @max_args: max args
 */
void tokenize(char *line, char *args[], int *token_count, int max_args)
{
	char *token;

	token = strtok(line, " \t\n");
	*token_count = 0;
	while (token != NULL && *token_count < max_args - 1)
	{
		args[*token_count] = token;
		(*token_count)++;
		token = strtok(NULL, " \t\n");
	}
	args[*token_count] = NULL;
}
/**
 * path - search for executables
 * @args: args
 * @command_executed: checks if the command passed has been executed
 * @status: status of operation
 */
void path(char *args[], bool *command_executed, int *status)
{
	char *path, *path_copy, *token_path, full_path[MAX_PATH_LENGTH];
	struct stat fileStat;

	path = getenv("PATH");
	path_copy = strdup(path);
	token_path = strtok(path_copy, ":");

	while (token_path != NULL && !(*command_executed))
	{
		snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", token_path, args[0]);
		if (stat(full_path, &fileStat) == 0 || stat(args[0], &fileStat) == 0)
		{
			if (access(full_path, X_OK) == 0)
				exe(full_path, args, command_executed, status);
			else if (access(args[0], X_OK) == 0)
				execute_full_command(args, command_executed, status);
			else
				perror("access");
		}
		token_path = strtok(NULL, ":");
	}
	free(path_copy);
}
/**
 * cleanup - handles freeing memory
 * @aliases: pinter to arrays where aliases are stored
 * @num_aliases: int to track how many aliases are within the aliases array
 * @line: buffer stores stdin
 * @input: file passed to the shell
 */
void cleanup(char *aliases[], int num_aliases, char *line, FILE *input)
{
	int i;

	for (i = 0; i < num_aliases; i++)
		free(aliases[i]);
	free(line);
	if (input)
		fclose(input);
}
/**
 * handle_variable - handles variable replacements
 * @args: args
 * @token_count: token count
 * @last: stores exit status
 */
void handle_variable(char *args[], int *token_count, int last)
{
	int i;
	char pid_str[16];
	char exit_status_str[16];

	for (i = 0; i < *token_count; i++)
	{
		if (strcmp(args[i], "$$") == 0)
		{
			snprintf(pid_str, sizeof(pid_str), "%d", getpid());
			args[i] = strdup(pid_str);
		}
		else if (strcmp(args[i], "$?") == 0)
		{
			snprintf(exit_status_str, sizeof(exit_status_str), "%d", last);
			args[i] = strdup(exit_status_str);
		}
	}
}
