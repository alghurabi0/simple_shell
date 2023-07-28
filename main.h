#ifndef MY_HEADER
#define MY_HEADER

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include <fcntl.h>
#define MAX_ARGS 64
#define MAX_PATH_LENGTH 1024

extern char **environ;

int exe(char *full_path, char *args[], bool *command_executed, int *status);
int execute_full_command(char *args[], bool *command_executed, int *status);
int change_directory(char *args[]);
int execute_builtin_command(char *args[], int token_count);
void mode(int argc, char *argv[], bool *sh, bool *file_mode, FILE **input);
void tokenize(char *line, char *args[], int *token_count, int max_args);
void path(char *args[], bool *command_executed, int *status);
void cleanup(char *line, FILE *input);
void handle_variable(char *args[], int *token_count, int last);
void helper2(int cd_result);
void helper3(bool *command_executed, char *args[], char *argv[]);
void helper4(bool *command_executed, int *last, int *status);
int my_strcmp(const char *s1, const char *s2);
int my_strncmp(const char *s1, const char *s2, size_t n);
size_t my_strlen(const char *s);
void *my_memcpy(void *dest, const void *src, size_t n);
char *my_strdup(const char *s);
int my_atoi(const char *str);
char *my_getenv(const char *name);
bool empty_or_not(const char *line);
void check_exit(char *args[], int token_count);
int cd_home();
int chdir_and_update_env(const char *directory);
#endif
