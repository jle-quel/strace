#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int strace(char **argv)
{
	int result;
	__attribute__((cleanup(binary_deconstructor))) struct s_binary binary = {0};

	if ((result = binary_constructor(argv, &binary)) != SUCCESS)
		return error(result);

	if ((result = execution(&binary)) != SUCCESS)
		return error(result);

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
