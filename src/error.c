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

int fork_error(void)
{
	perror("strace: fork");

	return errno;
}

int readlink_error(void)
{
	perror("strace: readlink");

	return errno;
}

int getenv_error(void)
{
	perror("strace: getenv");

	return errno;
}

int strtok_error(void)
{
	perror("strace: strtok");

	return errno;
}

int malloc_error(void)
{
	perror("strace: malloc");

	return errno;
}

int strdup_error(void)
{
	perror("strace: strdup");

	return errno;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int error(const enum e_context context)
{
	struct s_error error[] = 
	{
		{ACCESS, access_error},
		{OPEN, open_error},
		{READ, read_error},
		{ELF, elf_error},
		{CLASS, class_error},
		{FORK, fork_error},
		{READLINK, readlink_error},
		{GETENV, getenv_error},
		{STRTOK, strtok_error},
		{MALLOC, malloc_error},
		{STRDUP, strdup_error},
	};
	const int error_size = sizeof(error) / sizeof(*error);

	for (register int index = 0; index < error_size; index += 1)
	{
		if (context == error[index].context)
			error[index].handler();
	}

	return context;
}
