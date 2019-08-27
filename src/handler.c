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

static int set_handler(struct s_binary *binary, const int fd)
{
	char type;

	if (read(fd, &type, ELF_CLASS_SIZE) == -1)
		return READ;


	switch (type)
	{
		case ELFCLASS32:
			binary->title = title_32;
			binary->syscall = syscall_32;
			binary->parameter = parameter_32;
			binary->result = result_32;

			return SUCCESS;
		case ELFCLASS64:
			binary->title = title_64;
			binary->syscall = syscall_64;
			binary->parameter = parameter_64;
			binary->result = result_64;

			return SUCCESS;
	}

	return CLASS;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int get_handler(struct s_binary *binary)
{
	int result;
	__attribute__((cleanup(_close))) int fd;

	if (access(binary->filepath, F_OK) == -1)
		return ACCESS;
	if ((fd = open(binary->filepath, O_RDONLY)) == -1)
		return OPEN;

	if ((result = is_elf(fd)) != SUCCESS)
		return result;
	if ((result = set_handler(binary, fd)) != SUCCESS)
		return result;

	return SUCCESS;
}
