#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void _close(int *fd)
{
	close(*fd);
}

static int is_elf(const int fd)
{
	char buf[4] = {0};

	if (read(fd, buf, ELF_MAGIC_SIZE) == -1)
		return READ;

	if (*(int *)buf == ELF_MAGIC_HASH)
		return SUCCESS;

	return ELF;
}

static int set_handler(int (**handler)(const char *), const int fd)
{
	char type;

	if (read(fd, &type, ELF_CLASS_SIZE) == -1)
		return READ;

	switch (type)
	{
		case ELFCLASS32:
			*handler = handler_32;
			return SUCCESS;
		case ELFCLASS64:
			*handler = handler_64;
			return SUCCESS;
	}

	return CLASS;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int get_handler(int (**handler)(const char *), const char *filename)
{
	int result;
	__attribute__((cleanup(_close))) int fd;

	if (access(filename, F_OK) == -1)
		return ACCESS;
	if ((fd = open(filename, O_RDONLY)) == -1)
		return OPEN;

	if ((result = is_elf(fd)) != SUCCESS)
		return result;
	if ((result = set_handler(handler, fd)) != SUCCESS)
		return result;

	return SUCCESS;
}
