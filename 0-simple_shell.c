#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/stat.h>
#define MAX_ARGS 64
#define MAX_PATH_LENGTH 1024
/**
 * main - main function
 * Return: 0
 */
extern char **environ;
int main (int argc, char *argv)
{
	char *line = NULL;
	size_t size = 0;
	size_t chars_read;
	pid_t pid;
	char *args[MAX_ARGS];
	char *token;
	int token_count;
	int status;
	char error_message;
	char *path_copy;
	char *path;
	char full_path[MAX_PATH_LENGTH];
	char *token_path;
	int token_path_count;
	struct stat fileStat;
	bool command_executed = false;

	while (1)
	{
		printf("$ ");
		fflush(stdout);

		chars_read = getline(&line, &size, stdin);
		if (line[0] == '\n' || chars_read == '\0' || chars_read == EOF || (strncmp(line, "exit", 4) == 0))
			break;
		/*
		 * tokenize
		 */
		token = strtok(line, " \t\n");
		while (token != NULL && token_count < MAX_ARGS - 1)
		{
			args[token_count] = token;
			token_count++;
			token = strtok(NULL, " \t\n");
		}
		args[token_count] = NULL;
		/*
		 * execution
		 */
		if (token_count == 0)
			break;
		else
		{
			path = getenv("PATH");
			path_copy = strdup(path);
			token_path = strtok(path_copy, ":");

			while (token_path != NULL && !command_executed)
			{
				snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", token_path, args[0]);
				if (stat(full_path, &fileStat) == 0)
				{
					if (access(full_path, X_OK) == 0)
					{
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
							wait(&status);
							command_executed = true;
						}
					}
					else
						perror("access");
				}
				else if (stat(args[0], &fileStat) == 0)
				{
					if (access(args[0], X_OK) == 0)
					{
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
							wait(&status);
							command_executed = true;
						}
					}
					else
						perror("access");
				}
				token_path = strtok(NULL, ":");
			}
			if (command_executed == false)
				fprintf(stderr, "%s: command not found\n", args[0]);
		}
		token_count = 0;
		command_executed = false;
	}
	free(path_copy);
	free(line);
	return (0);
}
