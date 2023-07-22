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
