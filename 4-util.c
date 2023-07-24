#include "main.h"
/**
 * my_getenv - custom implementation of getenv
 * @name: name of the environment variable to retrieve
 * Return: Pointer to the value of the environment variable, or NULL if not found
 */
char* my_getenv(const char *name)
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
