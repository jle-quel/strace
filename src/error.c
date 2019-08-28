#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void _usage(void)
{
	fprintf(stderr, "strace: usage: ./ft_strace <binary> <argv>\n");
}

static void _access(void)
{
	perror("strace: access");
}

static void _open(void)
{
	perror("strace: open");
}

static void _read(void)
{
	perror("strace: read");
}

static void _elf(void)
{
	fprintf(stderr, "strace: not an ELF file format\n");
}

static void _class(void)
{
	fprintf(stderr, "strace: not an 64 or 32 bit mode\n");
}

static void _fork(void)
{
	perror("strace: fork");
}

static void _getenv(void)
{
	perror("strace: getenv");
}

static void _malloc(void)
{
	perror("strace: malloc");
}

static void _strdup(void)
{
	perror("strace: strdup");
}

static void _execve(void)
{
	perror("strace: execve");
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int error(const enum e_context context)
{
	const struct s_error error[] = 
	{
		{USAGE, _usage},
		{ACCESS, _access},
		{OPEN, _open},
		{READ, _read},
		{ELF, _elf},
		{CLASS, _class},
		{FORK, _fork},
		{GETENV, _getenv},
		{MALLOC, _malloc},
		{STRDUP, _strdup},
		{EXECVE, _execve},
	};
	const int error_size = sizeof(error) / sizeof(*error);

	for (register int index = 0; index < error_size; index += 1)
	{
		if (context == error[index].context)
			error[index].handler();
	}

	return context;
}
