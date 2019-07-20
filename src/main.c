#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static void usage(void)
{
	fprintf(stderr, "strace: usage: ./ft_strace <file>\n");
	exit(1);
}

////////////////////////////////////////////////////////////////////////////////
/// BOOTSTRAP FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	if (argc != 2)
		usage();

	strace(argv[1]);

	return 0;
}
