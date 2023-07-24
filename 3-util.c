#include "main.h"
int my_strncmp(const char *s1, const char *s2, size_t n)
{
    while (n > 0 && *s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
        n--;
    }
    if (n == 0)
        return 0;
    return (*(unsigned char*)s1 - *(unsigned char*)s2);
}
char *my_strdup(const char *s)
{
    size_t len = my_strlen(s) + 1;
    char *copy = (char*)malloc(len);

    if (copy)
        my_memcpy(copy, s, len);
    return (copy);
}
size_t my_strlen(const char *s)
{
    const char *p = s;

    while (*p)
        p++;
    return (p - s);
}
void *my_memcpy(void *dest, const void *src, size_t n)
{
    char *d = dest;
    const char *s = src;

    while (n--)
        *d++ = *s++;
    return (dest);
}
