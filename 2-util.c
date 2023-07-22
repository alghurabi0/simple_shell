#include "main.h"
/**
 * helper - helper function
 * @sh: checks comments mode
 */
void helper(bool *sh)
{
	if (*sh)
		printf("# ");
	else
		printf("$ ");
	fflush(stdout);
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
void helper3(bool *command_executed, char *args[])
{
	if (*command_executed == false)
		fprintf(stderr, "%s: command not found\n", args[0]);
}
