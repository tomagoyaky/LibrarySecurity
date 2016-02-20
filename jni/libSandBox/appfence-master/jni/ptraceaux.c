#include "config.h"

/* Common part */
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include "ptraceaux.h"

long ptrace_attach(pid_t pid) {
	return ptrace(PTRACE_ATTACH, pid, NULL, NULL);
}

long ptrace_detach(pid_t pid) {
	return ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

long ptrace_setopt(pid_t pid, int opt) {
	return ptrace(PTRACE_SETOPTIONS, pid, NULL, (void *) opt);
}

long ptrace_cont(pid_t pid) {
	return ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
}

#ifdef SANDBOX_ARCH_ARM
#include "arm/ptraceaux_more.c"
#endif /* ARM */

#ifdef SANDBOX_ARCH_X86
#include "x86/ptraceaux_more.c"
#endif /* x86 */

