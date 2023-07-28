#include "main.h"
/**
 * empty_or_not - checks for empty lines
 * @line: line input
 * Return: boolean
 */
bool empty_or_not(const char *line)
{
	while (*line)
	{
		if (!isspace(*line))
			return (false);
		line++;
	}
	return (true);
}
