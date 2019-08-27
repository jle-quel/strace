#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

int binary_constructor(char **argv, struct s_binary *binary)
{
	int result;

	if ((result = get_filepath(argv[0], binary)) != SUCCESS)
		return result;

	if ((result = get_handler(binary)) != SUCCESS)
		return result;

	binary->parameters = argv;

	return SUCCESS;
}

void binary_deconstructor(struct s_binary *binary)
{
	free(binary->filepath);
}

void title_32(const struct s_binary *binary)
{
	printf("[+] Executing %s as process %d in 32 bit mode\n\n", binary->filepath, binary->pid);
}

void title_64(const struct s_binary *binary)
{
	printf("[+] Executing %s as process %d in 64 bit mode\n\n", binary->filepath, binary->pid);
}

void syscall_64(const struct s_binary *binary)
{
	const char *syscall = get_syscall_64(binary->regs.orig_rax);
	const int nparameters = get_nparameters_64(binary->regs.orig_rax);

	const size_t registers[] = 
	{
		binary->regs.rdi,
		binary->regs.rsi,
		binary->regs.rdx,
		binary->regs.r10,
		binary->regs.r8,
		binary->regs.r9,
	};

	fprintf(stderr, "%s", syscall); 
	fprintf(stderr, "(");

	for (register int index = 0; index < nparameters; index += 1)
	{
		fprintf(stderr, "0x%lx", registers[index]); 

		if (index + 1 < nparameters)
			fprintf(stderr, ", ");
	}

	if (binary->regs.orig_rax == SYS_write)
		fprintf(stderr, ") -> ");
	else
		fprintf(stderr, ")\n");

}

void syscall_32(const struct s_binary *binary)
{
	(void)binary;
}
