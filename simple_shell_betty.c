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
#define MAX_ALIASES 64
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
 * Return: 0
 */
extern char **environ;
int execute_command(char *full_path, char *args[], bool *command_executed, int *status)
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
int change_directory(char *args[])
{
    const char *directory;
    char current_dir[MAX_PATH_LENGTH];
    char new_dir[MAX_PATH_LENGTH];

	if (args[1] == NULL)
        directory = "/root";
    else if (strcmp(args[1], "-") == 0)
	{
        directory = getenv("OLDPWD");
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
    if (setenv("PWD", new_dir, 1) != 0)
	{
        perror("setenv");
        return (1);
    }
    return (0);
}
int execute_builtin_command(char *args[], int token_count)
{
    int exit_status;

    if (strncmp(args[0], "exit", 4) == 0)
	{
        if (token_count > 1)
		{
            exit_status = atoi(args[1]);
            exit(exit_status);
        }
		else
            exit(0);
    }
	else if (strcmp(args[0], "setenv") == 0)
	{
        if (token_count != 3)
            fprintf(stderr, "Invalid usage of setenv command\n");
        else
            if (setenv(args[1], args[2], 1) != 0)
                perror("setenv");
    }
	else if (strcmp(args[0], "unsetenv") == 0)
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
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t size = 0;
	ssize_t chars_read;
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
	int i;
	int j;
	char alias_arg[MAX_PATH_LENGTH];
	char *alias_name;
	char *alias_value;
	char *alias_args[MAX_ARGS];
	int alias_token_count;
	char *alias_token;
	char alias[MAX_PATH_LENGTH];
	char *aliases[MAX_ALIASES];
    int num_aliases = 0;
	char *dollar_path;
	bool comments_mode = false;
	bool file_mode = false;
	FILE *input_file = NULL;
	int cd_result;
	int is_builtin_command;

	if (argc == 2)
	{
		if (strcmp(argv[1], "sh") == 0)
			comments_mode = true;
		else
		{
			file_mode = true;
			input_file = fopen(argv[1], "r");
			if (input_file == NULL)
			{
				perror("Error opening file");
				return (1);
			}
		}
	}

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
				if (token_count == 1)
				{
					i = 0;
					while (aliases[i] != NULL)
					{
						if (strncmp(aliases[i], "alias", 5) == 0)
							printf("%s\n", aliases[i]);
						i++;
					}
				}
				else
				{
					if (strchr(args[1], '=') != NULL)
					{
						strcpy(alias_arg, args[1]);
						alias_token = strtok(alias_arg, "=");
						alias_token_count = 0;
						while (alias_token != NULL)
						{
							alias_args[alias_token_count] = alias_token;
							alias_token_count++;
							alias_token = strtok(NULL, "=");
						}
						alias_args[alias_token_count] = NULL;
						alias_name = alias_args[0];
						alias_value = alias_args[1];
						snprintf(alias, MAX_PATH_LENGTH, "alias %s='%s'", alias_name, alias_value);
						setenv(alias_name, alias_value, 1);
						/* h = 0;
						while (environ[h] != NULL)
						{
								// if (strncmp(environ[h], "alias", 5) == 0 && strncmp(environ[h], alias, strlen(alias)) == 0)
								// {
								// 	environ[h] = alias;
								// 	return;
								// }
							h++;
						}
						*/
						aliases[num_aliases] = strdup(alias);
                    				num_aliases++;
                    				aliases[num_aliases] = NULL;
					}
					else
					{
						i = 0;
						while (aliases[i] != NULL)
						{
							j = 1;
							while (args[j] != NULL)
							{
								if (strstr(aliases[i], args[j]) != NULL)
									printf("%s\n", aliases[i]);
								j++;
							}
						i++;
						}
					}
				}
				continue;
			}
			else if (strncmp(args[0], "echo", 4) == 0 && strncmp(args[1], "$?", 2) == 0)
			{
				printf("%d\n", status);
				continue;
			}
			else if (strncmp(args[0], "echo", 4) == 0 && strncmp(args[1], "$$", 2) == 0)
			{
				printf("%d\n", getpid());
				continue;
			}
			else if (strncmp(args[0], "echo", 4) == 0 && strncmp(args[1], "$PATH", 5) == 0)
			{
				dollar_path = getenv("PATH");
				if (path != NULL)
					printf("%s\n", dollar_path);
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
							execute_command(full_path, args, &command_executed, &status);
						}
						else
							perror("access");
					}
					else if (stat(args[0], &fileStat) == 0)
					{
						if (access(args[0], X_OK) == 0)
						{
							execute_full_command(args, &command_executed, &status);
						}
						else
							perror("access");
					}
					/* else if (aliases[0] != NULL)
					{
						printf("aliases 0 is not null, %s\n", aliases[0]);
						for (j = 0; aliases[j] != NULL; j++)
						{
							if (strstr(aliases[j], args[0]) != NULL)
							{
								alias_token = strtok(aliases[j], "=");
								alias_token_count = 0;
								while (alias_token != NULL)
								{
									alias_args[alias_token_count] = alias_token;
									alias_token_count++;
									alias_token = strtok(NULL, "=");
								}
								alias_args[alias_token_count] = NULL;
								printf("alias value being executed is, %s\n", alias_args[1]);
								while (alias_args[1])
								{
									alias
								}
								pid = fork();
								if (pid == -1)
								{
									perror("fork");
									exit(EXIT_FAILURE);
								}
								else if (pid == 0)
								{
									execve(alias_args[1], alias_args, environ);
									exit(EXIT_SUCCESS);
								}
								else
								{
									wait(&status);
									command_executed = true;
								}
							}
						}
					}
					*/
					token_path = strtok(NULL, ":");
				}
			}
			if (command_executed == false)
				fprintf(stderr, "%s: command not found\n", args[0]);
		}
		command_executed = false;
	}
	for (i = 0; i < num_aliases; i++)
        	free(aliases[i]);
	free(path_copy);
	free(line);
	if (file_mode)
		fclose(input_file);
	return (0);
}
