#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int strace(const char *filename)
{
//	int (*handler)(const char *);
	char *filepath;

	if ((filepath = get_filepath(filename)) == NULL)
		return FAILURE;

	filepath ? puts(filepath) : puts("invalid\n");

	free(filepath);

//	if ((result = get_handler(&handler, filename)) != SUCCESS)
//		return error(result);
//
//	handler(filename);

	return SUCCESS;
}
