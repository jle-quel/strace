#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void tracee(const char *filename)
{
	const char *argv[2] = {filename, NULL};
	extern char **environ;

	kill(getpid(), SIGSTOP);
	execve(argv[0], (char * const *)argv, environ);
}

void tracer(const pid_t child)
{
	int status;
	int id = 0;
	struct user_regs_struct regs = {0};

	printf("[+] Seizing process %d in 64 bit mode\n", child);
	ptrace(PTRACE_SEIZE, child, NULL, NULL);
	waitpid(child, &status, 0);

	if (WIFSTOPPED(status) == true)
		printf("[+] process receive signal %d\n\n", WSTOPSIG(status));

	ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

	while (true)
	{
		ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		waitpid(child, &status, 0);

		if (WIFSTOPPED(status) == true)
		{
			if (WSTOPSIG(status) & 0x80)
			{
				ptrace(PTRACE_GETREGS, child, NULL, &regs);

				if (id % 2 == 0)
					fprintf(stderr, "syscall = %lld\t|\t", regs.orig_rax);
				else
					fprintf(stderr, "return = %lld\n", regs.rax);

				id += 1;
			}
			else
			{
				printf("[+] process receive signal %d\n\n", WSTOPSIG(status));
				kill(child, WSTOPSIG(status));
				break; // need to handle signal differentl
			}
		}
		else if (WIFEXITED(status) == true)
		{
			printf("return = ?\n\n[+] process exited with %d\n", WEXITSTATUS(status));
			return;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int handler_64(const char *filename)
{
	pid_t child;

	if ((child = fork()) == -1)
		return FORK;

	if (child == 0)
		tracee(filename);
	else
		tracer(child);

	return SUCCESS;
}
