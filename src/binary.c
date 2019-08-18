#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static void _close(int *fd)
{
	close(*fd);
}

static bool is_elf(const int fd)
{
	char buf[4] = {0};

	if (read(fd, buf, ELF_MAGIC_SIZE) == -1)
		error(READ);

	return *(int *)buf == ELF_MAGIC_HASH;
}

void handler32(void)
{
	printf("32\n");
}

void handler64(void)
{
	printf("64\n");
}

static void *get_handler(const int fd)
{
	char type;

	if (read(fd, &type, ELF_CLASS_SIZE) == -1)
		error(READ);

	if (type == ELFCLASS32)
		return handler32;
	if (type == ELFCLASS64)
		return handler64;

	error(CLASS);

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

struct s_binary get_binary(const char *file)
{
	__attribute__((cleanup(_close))) int fd;
	struct s_binary binary = {0};

	if (access(file, F_OK) == -1)
		error(ACCESS);
	if ((fd = open(file, O_RDONLY)) == -1)
		error(OPEN);
	if (is_elf(fd) == false)
		error(ELF);

	binary.handler = get_handler(fd);

	return binary;
}
