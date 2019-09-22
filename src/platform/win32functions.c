
#include <windows.h>
#include <stdint.h>
#include <ctype.h>

#include "win32functions.h"

int strcasecmp(const char *a, const char *b)
{
return stricmp(a, b);
}

// implementation of strcasestr(), as mingw doesn't have it.
// this version only returns whether or not the needle is present,
// it does not return the position.
char *strcasestr(const char *haystack, const char *needle)
{
	int i;
	int matchamt = 0;

	for (i = 0; i<haystack[i]; i++)
	{
		if (tolower(haystack[i]) != tolower(needle[matchamt]))
		{
			matchamt = 0;
		}
		if (tolower(haystack[i]) == tolower(needle[matchamt]))
		{
			matchamt++;
			if (needle[matchamt] == 0) return (char *)1;
		}
	}

	return 0;
}