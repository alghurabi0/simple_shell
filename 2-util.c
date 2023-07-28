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
 * helper4 - helper function
 * @command_executed: checks if command is executed
 * @last: last exit int
 * @status: status of operation
 * @args: args
 */
void helper4(bool *command_executed, char *args[], int *last, int *status)
{
	if (*command_executed == false)
		fprintf(stderr, "./hsh: %d: %s: not found\n", argv[0], 1, args[0]);
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
