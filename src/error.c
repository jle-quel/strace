#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

int access_error(void)
{
	perror("strace: access");

	return errno;
}

int open_error(void)
{
	perror("strace: open");

	return errno;
}

int read_error(void)
{
	perror("strace: read");

	return errno;
}

int elf_error(void)
{
	fprintf(stderr, "strace: file given is not an ELF\n");

	return -1;
}

int class_error(void)
{
	fprintf(stderr, "strace: file given is neither 32 or 64\n");

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void error(const enum e_context context)
{
	int (*handler[])(void) =
	{
		access_error,
		open_error,
		read_error,
		elf_error,
		class_error,
	};

	exit(handler[context]());
}
