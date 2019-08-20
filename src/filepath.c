#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static char *create_filepath(const char *filename)
{
	char *filepath;

	if ((filepath = strdup(filename)) == NULL)
		error(STRDUP);

	return filepath;
}

static char *create_absolute_filepath(const char *token, const char *filename)
{
	char *filepath = NULL;
	const size_t length = strlen(token) + strlen("/") + strlen(filename) + 1;

	if ((filepath = malloc(sizeof(char) * length)) == NULL)
	{
		error(MALLOC);
		return NULL;
	}

	bzero(filepath, length);
	strcpy(filepath, token);
	strcat(filepath, "/");
	strcat(filepath, filename);

	return filepath;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

char *get_filepath(const char *filename)
{
	char *filepath;
	char *path;
	char *token;

	if (filename[0] != '/')
	{
		if ((path = getenv("PATH")) != NULL)
		{
			token = strtok(path, ":");

			while (token)
			{
				if ((filepath = create_absolute_filepath(token, filename)) == NULL)
					return NULL;

				if (access(filepath, F_OK) == 0)
					return filepath;

				token = strtok(NULL, ":");
				free(filepath);
			}
		}
	}

	return create_filepath(filename);
}
