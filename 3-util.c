#include "main.h"
/**
 * my_strncmp - strncmp
 * @s1: string
 * @s2: string
 * @n: numi
 * Return: int
 */
int my_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n > 0 && *s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
/**
 * my_strdup - strdup
 * @s: string
 * Return: char
 */
char *my_strdup(const char *s)
{
	size_t len = my_strlen(s) + 1;
	char *copy = (char *)malloc(len);

	if (copy)
		my_memcpy(copy, s, len);
	return (copy);
}
/**
 * my_strlen - strlen
 * @s: string
 * Return: size_t
 */
size_t my_strlen(const char *s)
{
	const char *p = s;

	while (*p)
		p++;
	return (p - s);
}
/**
 * my_memcpy - memcpy
 * @dest: string
 * @src: string
 * @n: num
 * Return: void
 */
void *my_memcpy(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;

	while (n--)
		*d++ = *s++;
	return (dest);
}
/**
 * my_atoi - atoi
 * @str: str
 * Return: int
 */
int my_atoi(const char *str)
{
	int result = 0;
	int sign = 1;

	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
