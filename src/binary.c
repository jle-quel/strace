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

	binary->argv = argv;

	return SUCCESS;
}

void binary_destructor(struct s_binary *binary)
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

void syscall_32(const struct s_binary *binary)
{
	const char *syscall = get_syscall_32(binary->regs.orig_rax);

	fprintf(stderr, "[ ] %s", syscall);
	fprintf(stderr, "(");
}

void syscall_64(const struct s_binary *binary)
{
	const char *syscall = get_syscall_64(binary->regs.orig_rax);

	fprintf(stderr, "[ ] %s", syscall);
	fprintf(stderr, "(");
}

void parameter_32(const struct s_binary *binary)
{
	int stack_value = 0;
	const int nparameters = get_nparameters_32(binary->regs.orig_rax);

	for (register int index = 0; index < nparameters; index += 1)
	{
		fprintf(stderr, "0x%llx", binary->regs.rsp + stack_value);

		if (index + 1 < nparameters)
			fprintf(stderr, ", ");

		stack_value += 4;
	}

	if (binary->regs.orig_rax == SYS_WRITE_32)
		fprintf(stderr, ") -> ");
	else
		fprintf(stderr, ")");
}

void parameter_64(const struct s_binary *binary)
{
	const typeof(binary->regs.orig_rax) registers[] =
	{
		binary->regs.rdi,
		binary->regs.rsi,
		binary->regs.rdx,
		binary->regs.r10,
		binary->regs.r8,
		binary->regs.r9,
	};
	const int nparameters = get_nparameters_64(binary->regs.orig_rax);

	for (register int index = 0; index < nparameters; index += 1)
	{
		fprintf(stderr, "0x%llx", registers[index]);

		if (index + 1 < nparameters)
			fprintf(stderr, ", ");
	}

	if (binary->regs.orig_rax == SYS_WRITE_64)
		fprintf(stderr, ") -> ");
	else
		fprintf(stderr, ")");
}

void tail(const struct s_binary *binary)
{
	printf("\n\n[+] Detaching from process %d\n", binary->pid);
}

void result(const struct s_binary *binary)
{
	if ((long long)binary->regs.rax < 0)
		fprintf(stderr, " = 0x%x\n", -1);
	else
		fprintf(stderr, " = 0x%llx\n", binary->regs.rax);
}
