#ifndef HOOK_UTIL_H_
#define HOOK_UTIL_H_
static int find_name(pid_t pid, const char *name,const  char *libn, unsigned long *addr);
static int find_libbase(pid_t pid, char *libn, unsigned long *addr);
 
#endif
