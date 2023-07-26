#include "main.h"
/**
 * my_getenv - custom implementation of getenv
 * @name: name of the environment variable to retrieve
 * Return: Pointer to the value of the environment variable, or NULL if
 */
char *my_getenv(const char *name)
{
	size_t name_len;
	char **env = environ;

	if (name == NULL || *name == '\0')
		return (NULL);
	name_len = my_strlen(name);
	while (*env)
	{
		if (my_strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
			return (*env + name_len + 1);
		env++;
	}
	return (NULL);
}
/**
 * helper2 - helper function
 * @cd_result: change directory function return flag
 */
void helper2(int cd_result)
{
	if (cd_result != 0)
		fprintf(stderr, "Invalid usage of cd command\n");
}
/**
 * helper3 - helper function
 * @command_executed: check if command is executed
 * @args: args
 */
void helper3(bool *command_executed, char *args[], char *argv[])
{
	if (*command_executed == false)
		fprintf(stderr, "%s: %d: %s: not found\n", argv[0], getpid(), args[0]);
}
/**
 * helper4 - helper function
 * @command_executed: checks if command is executed
 * @last: last exit int
 * @status: status of operation
 */
void helper4(bool *command_executed, int *last, int *status)
{
	*command_executed = false;
	*last = *status;
}
/**
 * my_strcmp - strcmp
 * @s1: s1
 * @s2: string
 * Return: int
 */
int my_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
