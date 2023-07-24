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
/**
 * my_setenv - custom implementation of setenv
 * @name: name of the environment variable to set
 * @value: value of the environment variable
 * @overwrite: flag to indicate whether to overwrite existing value
 * Return: 0 on success, -1 on failure
 */
int my_setenv(const char *name, const char *value, int overwrite)
{
    size_t n_len, v_len, environ_size = 0, i;
    char *en_en, **env = environ, **new_environ;

    if (name == NULL || *name == '\0')
        return (-1);
    if (overwrite == 0 && my_getenv(name) != NULL)
        return (0);
    n_len = my_strlen(name);
    v_len = value ? my_strlen(value) : 0;
    *en_en = (char *)malloc(n_len + v_len + 2);
    if (en_en == NULL)
        return (-1);
    snprintf(en_en, n_len + v_len + 2, "%s=%s", name, value ? value : "");
    while (*env)
    {
        if (my_strncmp(*env, name, n_len) == 0 && (*env)[n_len] == '=')
        {
            if (overwrite)
            {
                free(*env);
                *env = en_en;
                return (0);
            }
            else
            {
                free(en_en);
                return (0);
            }
        }
        env++;
    }
    while (environ[environ_size] != NULL)
        environ_size++;
    **new_environ = (char **)malloc((environ_size + 2) * sizeof(char *));
    if (new_environ == NULL)
    {
        free(en_en);
        return (-1);
    }
    for (i = 0; i < environ_size; i++)
        new_environ[i] = environ[i];
    new_environ[environ_size] = en_en;
    new_environ[environ_size + 1] = NULL;
    environ = new_environ;
    return (0);
}
/**
 * my_unsetenv - custom implementation of unsetenv
 * @name: name of the environment variable to unset
 * Return: 0 on success, -1 on failure
 */
int my_unsetenv(const char *name)
{
    size_t name_len;
    char **env;

    if (name == NULL || *name == '\0')
        return (-1);
    name_len = my_strlen(name);
    **env = environ;
    while (*env)
    {
        if (my_strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
        {
            free(*env);
            while (*env)
            {
                *env = *(env + 1);
                env++;
            }
            return (0);
        }
        env++;
    }
    return (0);
}
