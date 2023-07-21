#include "main.h"

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
    char current_dir[MAX_PATH_LENGTH], new_dir[MAX_PATH_LENGTH];

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
void handle_alias_case(char *args[], char *aliases[], int *num_aliases, int token_count)
{
	int i = 0, j = 0, alias_token_count;
	char alias_arg[MAX_PATH_LENGTH], *alias_name, *alias_value, *alias_token;
	char *alias_args[MAX_ARGS], alias[MAX_PATH_LENGTH];

	if (token_count == 1)
	{
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
			aliases[*num_aliases] = strdup(alias);
            (*num_aliases)++;
            aliases[*num_aliases] = NULL;
		}
		else
		{
			while (aliases[i] != NULL)
			{
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
}