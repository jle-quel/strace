#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

bool is_killing_signal(const int signum)
{
	const int sig[] =
	{
		SIGHUP,
		SIGINT,
		SIGQUIT,
		SIGSEGV,
		SIGABRT,
		SIGTERM,
		SIGSTOP,
		SIGTSTP,
	};
	const int size = sizeof(sig) / sizeof(*sig);

	for (register int index = 0; index < size; index += 1)
	{
		if (signum == sig[index])
			return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

int tracee(const struct s_binary *binary)
{
	extern char **environ;

	kill(binary->pid, SIGSTOP);
	execve(binary->filepath, binary->argv, environ);
	exit(error(EXECVE));
}

// Handle all errors
int tracer(const struct s_binary *binary)
{
	int status;
	int id = 0;
	static sigset_t	set = {0};

	binary->title(binary);

	ptrace(PTRACE_SEIZE, binary->pid, NULL, NULL);
	waitpid(binary->pid, &status, 0);
	ptrace(PTRACE_SETOPTIONS, binary->pid, 0, PTRACE_O_TRACESYSGOOD);

	while (true)
	{
		sigemptyset(&set);
		sigprocmask(SIG_SETMASK, &set, NULL);

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
					binary->parameter(binary);
				}
				else
					binary->result(binary);

				id += 1;
			}
			else
			{
				printf("\n[+] process receive signal %d\n\n", WSTOPSIG(status));

	//			if (is_killing_signal(WSTOPSIG(status)) == true)
			//		break;
			}
		}

		if (WIFEXITED(status) == true)
		{
			printf("\n\n[+] process exited with %d\n", WEXITSTATUS(status));
			break;
		}

		sigaddset(&set, SIGHUP);
		sigaddset(&set, SIGINT);
		sigaddset(&set, SIGQUIT);
		sigaddset(&set, SIGPIPE);
		sigaddset(&set, SIGTERM);
		sigprocmask(SIG_BLOCK, &set, NULL);
	}

	return SUCCESS;
}