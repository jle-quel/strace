#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static char *create_filepath(const char *filename)
{
	char *filepath;

	if ((filepath = strdup(filename)) == NULL)
		return NULL;

	return filepath;
}

static char *create_absolute_filepath(const char *token, const char *filename)
{
	char *filepath = NULL;
	const size_t length = strlen(token) + strlen("/") + strlen(filename) + 1;

	if ((filepath = malloc(sizeof(char) * length)) == NULL)
		return NULL;

	bzero(filepath, length);
	strcpy(filepath, token);
	strcat(filepath, "/");
	strcat(filepath, filename);

	return filepath;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int get_filepath(const char *filename, struct s_binary *binary)
{
	char *filepath;
	char *path;
	char *token;

	if (*filename != '/')
	{
		if ((path = getenv("PATH")) != NULL)
		{
			token = strtok(path, ":");

			while (token)
			{
				if ((filepath = create_absolute_filepath(token, filename)) == NULL)
					return MALLOC;
				if (access(filepath, F_OK) == 0)
					break;

				token = strtok(NULL, ":");
				free(filepath);
				filepath = NULL;
			}
		}
	}

	if (filepath)
		binary->filepath = filepath;
	else
		if ((binary->filepath = create_filepath(filename)) == NULL)
			return STRDUP;

	return SUCCESS;
}
