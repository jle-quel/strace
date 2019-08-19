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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

////////////////////////////////////////////////////////////////////////////////
/// DEFINES
////////////////////////////////////////////////////////////////////////////////

#define SUCCESS 0
#define FAILURE 1

#define ELF_MAGIC_HASH 1179403647
#define ELF_MAGIC_SIZE 4
#define ELF_CLASS_SIZE 1

////////////////////////////////////////////////////////////////////////////////
/// ENUMS
////////////////////////////////////////////////////////////////////////////////

enum e_context
{
	ACCESS = 1,
	OPEN,
	READ,
	ELF,
	CLASS,
};

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

struct s_error
{
	enum e_context context;
	int (*handler)(void);
};

////////////////////////////////////////////////////////////////////////////////
/// DECLARATIONS
////////////////////////////////////////////////////////////////////////////////

int strace(const char *file);

int get_handler(void (**handler)(void), const char *file);

int error(const enum e_context context);

#endif
