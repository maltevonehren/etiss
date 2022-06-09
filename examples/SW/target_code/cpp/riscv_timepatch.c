#ifdef __riscv__

#include <sys/time.h>
#include <sys/timeb.h>

extern int _ftime(struct timeb *tp);

// get seconds since epoch using semihosting call from libgloss
int _gettimeofday(struct timeval *tv, void *_)
{
    struct timeb tp;
    _ftime(&tp);

    tv->tv_sec = tp.time;
    tv->tv_usec = 0;
    return 0;
}

#endif
