#include "main.h"

void process_mode(int argc, char *argv[], bool *comments_mode, bool *file_mode, FILE **input_file)
{
	if (argc == 2)
	{
		if (strcmp(argv[1], "sh") == 0)
			*comments_mode = true;
		else
		{
			*file_mode = true;
			*input_file = fopen(argv[1], "r");
			if (*input_file == NULL)
			{
				perror("Error opening file");
				exit(EXIT_FAILURE);
			}
		}
	}
}
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
				execute_command(full_path, args, command_executed, status);
			else if (access(args[0], X_OK) == 0)
				execute_full_command(args, command_executed, status);
			else
				perror("access");
		}
		token_path = strtok(NULL, ":");
	}
	free(path_copy);
}
void cleanup(char *aliases[], int num_aliases, char *line, FILE *input_file)
{
	int i;

	for (i = 0; i < num_aliases; i++)
        free(aliases[i]);
    free(line);
    if (input_file)
        fclose(input_file);
}
void handle_variable_replacement(char *args[], int *token_count, int last_exit_status)
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
            snprintf(exit_status_str, sizeof(exit_status_str), "%d", last_exit_status);
            args[i] = strdup(exit_status_str);
        }
    }
}