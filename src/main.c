#include <strace.h>

struct s_binary binary = {0};

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static int execution(struct s_binary *binary)
{
	int result;

	if ((binary->pid = fork()) == -1)
		return FORK;

	if (binary->pid == 0)
	{
		binary->pid = getpid();
		tracee(binary);
	}
	else
		if ((result = tracer(binary)) != SUCCESS)
			return result;

	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int strace(char **argv)
{
	int result;

	if ((result = binary_constructor(argv, &binary)) != SUCCESS)
		return error(result);

	if ((result = execution(&binary)) != SUCCESS)
		return error(result);

	binary_destructor(&binary);

	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// BOOTSTRAP FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	if (argc < 2)
		return error(USAGE);

	return strace(argv + 1);
}
