/*
 * hook2.h
 *
 *  Created on: May 26, 2015
 *      Author: azhi
 */

#ifndef LIBHOOK_THOOK_H_
#define LIBHOOK_THOOK_H_

#define HOOK_FAILED -1
#define HOOK_SUCCESS 0

#ifdef __cplusplus
extern "C" {
#endif
int elfHook(const char *soname, const char *symbol, void *replace_func, void **old_func);
#ifdef __cplusplus
}
#endif
#endif /* LIBHOOK_HOOK2_H_ */
