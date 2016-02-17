/*
 * strtok_r.h
 *
 *  Created on: 2015年4月1日
 *      Author: Jevey
 */

#ifndef STRTOK_R_H_
#define STRTOK_R_H_


inline char* strtok_r(
    char *str,
    const char *delim,
    char **nextp)
{
    char *ret;

    if (str == NULL)
    {
        str = *nextp;
    }

	if (str == NULL) {
		return NULL;
	}

    str += strspn(str, delim);

    if (*str == '\0')
    {
        return NULL;
    }

    ret = str;

    str += strcspn(str, delim);

    if (*str)
    {
        *str++ = '\0';
    }

    *nextp = str;

    return ret;
}


#endif /* STRTOK_R_H_ */
