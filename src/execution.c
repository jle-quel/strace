#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

int tracee(struct s_binary *binary)
{
	extern char **environ;

	binary->pid = getpid();

	kill(binary->pid, SIGSTOP);
	execve(binary->filepath, binary->parameters, environ);
	exit(error(EXECVE));
}

int tracer(struct s_binary *binary)
{
	int status;
	int id = 0;
	static sigset_t	set = {0};

	binary->title(binary);

	sigemptyset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);

	ptrace(PTRACE_SEIZE, binary->pid, NULL, NULL);
	waitpid(binary->pid, &status, 0);
	ptrace(PTRACE_SETOPTIONS, binary->pid, 0, PTRACE_O_TRACESYSGOOD);

	while (true)
	{
		ptrace(PTRACE_SYSCALL, binary->pid, NULL, NULL);
		waitpid(binary->pid, &status, 0);

		if (WIFSTOPPED(status) == true)
		{
			if (WSTOPSIG(status) & 0x80)
			{
				ptrace(PTRACE_GETREGS, binary->pid, NULL, &binary->regs);

				if (id % 2 == 0)
				{
					binary->syscall(binary);
				}

				id += 1;
			}
			else
				printf("\n[+] process receive signal %d\n\n", WSTOPSIG(status));
		}

		if (WIFEXITED(status) == true)
		{
			printf("\n[+] process exited with %d\n", WEXITSTATUS(status));
			break;
		}
	}

	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int execution(struct s_binary *binary)
{
	int result;

	if ((binary->pid = fork()) == -1)
		return FORK;

	if (binary->pid == 0)
		tracee(binary);
	else
		if ((result = tracer(binary)) != SUCCESS)
			return result;

	return SUCCESS;
}
