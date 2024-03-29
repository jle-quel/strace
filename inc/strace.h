#ifndef STRACE_H
#define STRACE_H

////////////////////////////////////////////////////////////////////////////////
/// INCLUDES
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <elf.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include <sys/reg.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/stat.h>

////////////////////////////////////////////////////////////////////////////////
/// DEFINES
////////////////////////////////////////////////////////////////////////////////

#define SUCCESS 0
#define FAILURE -1

#define ELF_MAGIC_HASH 1179403647
#define ELF_MAGIC_SIZE 4
#define ELF_CLASS_SIZE 1

#define SYS_WRITE_64 1
#define SYS_WRITE_32 4

////////////////////////////////////////////////////////////////////////////////
/// ENUMS
////////////////////////////////////////////////////////////////////////////////

enum e_context
{
	USAGE = 1,
	ACCESS,
	OPEN,
	READ,
	ELF,
	CLASS,
	FORK,
	READLINK,
	GETENV,
	MALLOC,
	STRDUP,
	EXECVE,
};

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

struct s_binary
{
	char *filepath;
	char **argv;
	pid_t pid;
	struct user_regs_struct regs;

	void (*title)(const struct s_binary *binary);
	void (*tail)(const struct s_binary *binary);
	void (*syscall)(const struct s_binary *binary);
	void (*parameter)(const struct s_binary *binary);
	void (*result)(const struct s_binary *binary);
};

struct s_error
{
	enum e_context context;
	void (*handler)(void);
};

struct s_systable
{
	int id;
	char *name;
	int nparameters;
};

////////////////////////////////////////////////////////////////////////////////
/// GLOBAL VARIABLES
////////////////////////////////////////////////////////////////////////////////

struct s_binary binary;

////////////////////////////////////////////////////////////////////////////////
/// DECLARATIONS
////////////////////////////////////////////////////////////////////////////////

int strace(char **argv);

void binary_destructor(struct s_binary *binary);
void title_32(const struct s_binary *binary);
void title_64(const struct s_binary *binary);
void tail(const struct s_binary *binary);
void syscall_32(const struct s_binary *binary);
void syscall_64(const struct s_binary *binary);
void parameter_32(const struct s_binary *binary);
void parameter_64(const struct s_binary *binary);
void result(const struct s_binary *binary);

int tracer(const struct s_binary *binary);
int tracee(const struct s_binary *binary);

int binary_constructor(char **argv, struct s_binary *binary);

const char *get_syscall_32(const int id);
int get_nparameters_32(const int id);
const char *get_syscall_64(const int id);
int get_nparameters_64(const int id);

int get_filepath(const char *filename, struct s_binary *binary);
int get_handler(struct s_binary *binary);

int error(const enum e_context context);

#endif
