#include <strace.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

static inline int _wait(const pid_t pid)
{
	int status;
	static sigset_t set = {0};

	sigemptyset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);

	waitpid(pid, &status, WUNTRACED);

	sigaddset(&set, SIGHUP);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SIGPIPE);
	sigaddset(&set, SIGTERM);
	sigprocmask(SIG_BLOCK, &set, NULL);

	return status;
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

int tracer(const struct s_binary *binary)
{
	int status;
	int sig = 0;
	size_t id = 0;

	binary->title(binary);

	ptrace(PTRACE_SEIZE, binary->pid, NULL, NULL);
	status = _wait(binary->pid);
	ptrace(PTRACE_SETOPTIONS, binary->pid, 0, PTRACE_O_TRACESYSGOOD);

	while (true)
	{
		ptrace(PTRACE_SYSCALL, binary->pid, NULL, sig); 
		status = _wait(binary->pid);

		if (WIFSIGNALED(status) == true)
		{
			printf("\n[!] process exited with signal %d\n", WTERMSIG(status));
			break ;
		}
		else if (WIFEXITED(status) == true)
		{
			printf("\n[!] process exited with exit %d\n", WEXITSTATUS(status));
			break ;
		}
		else if (WIFSTOPPED(status) == true)
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

				sig = 0;
				id += 1;
			}
			else
			{
				sig = WSTOPSIG(status);
				printf("[!] process receive signal %d\n", WSTOPSIG(status));
			}
		}
	}

	return SUCCESS;
}
