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
#define MAX_ARGS 64
#define MAX_PATH_LENGTH 1024
#define MAX_ALIASES 64

extern char **environ;

int execute_command(char *full_path, char *args[], bool *command_executed, int *status);
int execute_full_command(char *args[], bool *command_executed, int *status);
int change_directory(char *args[]);
int execute_builtin_command(char *args[], int token_count);
void handle_alias_case(char *args[], char *aliases[], int *num_aliases, int token_count);
void process_mode(int argc, char *argv[], bool *comments_mode, bool *file_mode, FILE **input_file);
void tokenize(char *line, char *args[], int *token_count, int max_args);
void path(char *args[], bool *command_executed, int *status);
void cleanup(char *aliases[], int num_aliases, char *line, FILE *input_file);
void handle_variable_replacement(char *args[], int *token_count, int last_exit_status);

#endif
