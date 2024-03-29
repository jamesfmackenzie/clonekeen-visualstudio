
#include <windows.h>
#include <stdint.h>
#include <ctype.h>
#include <SDL.h>

#define MAX_FILES_TO_RETRIEVE		500
static struct
{
	char *fname;
	uint32_t attributes;
} dirlist[MAX_FILES_TO_RETRIEVE];

/*
void c------------------------------() {}
*/

int main(int argc, char* argv[])
{
	return KeenMain(argc, argv);
}

void platform_msgbox(const char *message)
{
	MessageBoxA(NULL, message, "CloneKeen", MB_OK | MB_ICONSTOP);
}

/*
void c------------------------------() {}
*/

// load a directory listing of *path (can be relative to current dir)
// into memory, and return the number of files and subdirectories in the directory.
int LoadDirectoryListing(char *path)
{
	char searchstr[MAX_PATH];
	HANDLE SHandle;
	WIN32_FIND_DATA	wfd;
	int curfile = 0;

	strcpy(searchstr, path);
	strcat(searchstr, "*");
	lprintf("GetDirListing: '%s'\n", searchstr);

	FreeDirectoryListing();

	if ((SHandle = FindFirstFile(searchstr, &wfd)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (wfd.cFileName[0] != '.')
			{
				dirlist[curfile].fname = _strdup(wfd.cFileName);
				if (!dirlist[curfile].fname) { FreeDirectoryListing(); return 0; }

				dirlist[curfile].attributes = wfd.dwFileAttributes;
				curfile++;
			}

		} while (FindNextFile(SHandle, &wfd) && curfile < MAX_FILES_TO_RETRIEVE);
	}

	return curfile;
}

// retrieve a pointer to the filename of file #fno, out of GetDirListing's list.
// this pointer does not have to be valid over multiple calls to GetFileName.
// e.g. calling GetFileName(0) must return a valid pointer, but that pointer
// need not necessarily remain valid if you then call GetFileName(1). This is
// to allow use of a global buffer which this func could copy filenames into
// and then return a pointer to, which may be useful in some implementations.
const char *GetFileAtIndex(int fno)
{
	return dirlist[fno].fname;
}

// returns nonzero if index "fno" is a directory
char GetIsDirectory(int fno)
{
	return (dirlist[fno].attributes & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
}

// free the memory used by LoadDirListing
void FreeDirectoryListing(void)
{
	int i;
	for (i = 0; i<MAX_FILES_TO_RETRIEVE; i++)
	{
		if (dirlist[i].fname)
		{
			free(dirlist[i].fname);
			dirlist[i].fname = NULL;
		}
	}
}

/*
void c------------------------------() {}
*/


