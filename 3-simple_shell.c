#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>
#define MAX_ARGS 64
#define MAX_PATH_LENGTH 1024
/**
 * main - main function
 * Return: 0
 */
extern char **environ;
void print_alias(char *alias_name)
{
        char *alias = getenv(alias_name);

        if (alias != NULL)
                printf("%s\n", alias);
}
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
	char *path_copy;
	char *path;
	char full_path[MAX_PATH_LENGTH];
	char *token_path;
	struct stat fileStat;
	bool command_executed = false;
	int exit_status;
	const char *directory;
	char current_dir[MAX_PATH_LENGTH];
	char new_dir[MAX_PATH_LENGTH];
	int i;
	int j;
	int h;
	int k;
	char alias_arg[MAX_PATH_LENGTH];
	char *alias_name;
	char *alias_value;
	char *alias_args[MAX_ARGS];
	int alias_token_count;
	char *alias_token;
	char alias[MAX_PATH_LENGTH];

	while (1)
	{
		printf("$ ");
		fflush(stdout);

		chars_read = getline(&line, &size, stdin);
		if (line[0] == '\n' || chars_read == '\0' || chars_read == EOF)
			break;
		/*
		 * tokenize
		 */
		token = strtok(line, " \t\n");
		token_count = 0;
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
		if (token_count > 0)
		{
			if (strncmp(args[0], "exit", 4) ==0)
			{
				if (token_count > 1)
				{
					exit_status = atoi(args[1]);
					exit(exit_status);
				}
				else
					break;
			}
			else if (strcmp(args[0], "setenv") == 0)
			{
				if (token_count != 3)
					fprintf(stderr, "Invalid usage of setenv command\n");
				else
					if (setenv(args[1], args[2], 1) != 0)
						perror("setenv");
				continue;
			}
			else if (strcmp(args[0], "unsetenv") == 0)
			{
				if (token_count != 2)
					fprintf(stderr, "Invalid usage of unsetenv command\n");
				else
					if (unsetenv(args[1]) != 0)
						perror("unsetenv");
				continue;
			}
			else if (strcmp(args[0], "cd") == 0)
			{
				if (token_count > 2)
					fprintf(stderr, "Invalid usage of cd command\n");
				else
				{
					directory = token_count == 1 ? getenv("HOME") : args[1];
					if (strcmp(directory, "-") == 0)
					{
						directory = getenv("OLDPWD");
						if (directory == NULL)
						{
							fprintf(stderr, "OLDPWD not set\n");
							continue;
						}
						printf("%s\n", directory);
					}
					if (getcwd(current_dir, sizeof(current_dir)) == NULL)
					{
						perror("getcwd");
						continue;
					}
					if (chdir(directory) != 0)
					{
						perror("chdir");
						continue;
					}
					if (setenv("OLDPWD", current_dir, 1) != 0)
						perror("setenv");
					if (getcwd(new_dir, sizeof(new_dir)) == NULL)
					{
						perror("getcwd");
						continue;
					}
					if (setenv("PWD", new_dir, 1) != 0)
						perror("setenv");
				}
				continue;
			}
			else if (strcmp(args[0], "alias") == 0)
			{
				printf("alias command detected\n");
				if (token_count == 1)
				{
					i = 0;
					while (environ[i] != NULL)
					{
						if (strncmp(environ[i], "alias", 5) == 0)
							printf("%s\n", environ[i]);
						i++;
					}
					if (environ == NULL)
						printf("the env is null\n");
				}
				else
				{
					if (strchr(args[1], '=') != NULL)
					{
						printf("equal sign detected\n");
						strcpy(alias_arg, args[1]);
						printf("second arg is copied, %s\n", alias_arg);
						alias_token = strtok(alias_arg, "=");
						alias_token_count = 0;
						while (alias_token != NULL)
						{
							alias_args[alias_token_count] = alias_token;
							alias_token_count++;
							alias_token = strtok(NULL, "=");
						}
						alias_args[alias_token_count] = NULL;
						printf("first alias arg, %s\n", alias_args[0]);
						printf("second alias arg, %s\n", alias_args[1]);
						alias_name = alias_args[0];
						printf("alias name set, %s\n", alias_name);
						alias_value = alias_args[1];
						printf("alias value set, %s\n", alias_value);
						snprintf(alias, MAX_PATH_LENGTH, "alias %s='%s'", alias_name, alias_value);
						printf("alias path is written, %s\n", alias);
						setenv(alias_name, alias_value, 1);
						printf("setenv is done\n");
						h = 0;
						while (environ[h] != NULL)
						{
								// if (strncmp(environ[h], "alias", 5) == 0 && strncmp(environ[h], alias, strlen(alias)) == 0)
								// {
								// 	environ[h] = alias;
								// 	return;
								// }
							h++;
						}
						if (environ == NULL)
						{
							printf("environ is null\n");
						}
						printf("environ while loop is completed with %d\n", h);
						environ[h] = alias;
						printf("setting environ[h] to alias\n");
						environ[h + 1] = NULL;
						printf("setting last environ to null\n");
					}
					else
					{
						j = 0;
						k = 1;
						while (environ[j] != NULL)
						{
							if (strncmp(environ[j], "alias", 5) == 0)
							{
								while (args[k] != NULL)
								{
									if (strstr(environ[j], args[k]) != NULL)
										printf("%s\n", environ[j]);
									k++;
								}
							}
							j++;
						}
						if (environ == NULL)
							printf("the env is null\n");
					}
				}
				continue;
			}
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
		}
			if (command_executed == false)
				fprintf(stderr, "%s: command not found\n", args[0]);
		}
		token_count = 0;
		command_executed = false;
	}
	free(path_copy);
	free(line);
	free(alias_arg);
	free(alias_args);
	return (0);
}
