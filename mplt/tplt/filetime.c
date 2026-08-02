/* filetime.c */

#include <string.h>
#include <time.h>
#include "plt.h"
#include "pltblk.h"
#include "cincl.h"
#include "fort.h"

#ifndef _MSC_VER
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#define _fstat  fstat
#define _stat   stat
#define _fileno fileno
#else
#ifdef MSDOS
#include <sys\types.h>
#include <sys\stat.h>
#endif 
#ifdef WIN32
#include <sys\stat.h>
#endif
#endif

/* filetime - return pointer to ascii string with file modification time */

char   *
filetime(void)
{
#ifndef WIN32
    struct _stat statb;

    statb.st_mtime = 0;		/* will change if file and not pipe */
    if (_fstat(_fileno(misc_.lunin), &statb) == 0 && statb.st_mtime != 0) {
	return (ctime(&statb.st_mtime));	/* file modification time */
    }
#endif
    return ("");		/* failed to get file time */
}

/* clocktime - return pointer to ascii string with current time of day */

char   *
clocktime(void)
{
    time_t  t;
    static char s[26];

    (void) time(&t);
    strcpy(s, asctime(localtime(&t)));	/* current time of day */
    s[24] = '\0';		/* remove '\n' from end */
    return (s);
}
