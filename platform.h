#if defined __linux__
#define M_TIME_FUNCTION M_TIME_CLOCK_GETTIME
#endif


#define M_TIME_CLOCK_GETTIME 1
#define M_TIME_GETTICKCOUNT 2

#if M_TIME_FUNCTION == M_TIME_CLOCK_GETTIME

#include <time.h>

typedef struct timespec m_time;

static inline void m_time_get(m_time *t) {
	clock_gettime(CLOCK_MONOTONIC, t);
}

static inline void m_time_diff(m_time *r, m_time *t1, m_time *t2) {
	if (t2->tv_nsec >= t1->tv_nsec) {
		r->tv_nsec = t2->tv_nsec - t1->tv_nsec;
		r->tv_sec = t2->tv_sec - t1->tv_sec;
	} else {
		r->tv_nsec = 1000000000 + t2->tv_nsec - t1->tv_nsec;
		r->tv_sec = t2->tv_sec - t1->tv_sec - 1;
	}
}

#define M_TIME_SEC_MSEC_FMT "%lu.%03lu"
#define M_TIME_SEC_MSEC_ARGS(t) ((t).tv_sec), ((t).tv_nsec/1000000)
#define M_TIME_IN_USEC_DBL(t) ((t).tv_sec*1000000.0 + (t).tv_nsec/1000.0)

#elif M_TIME_FUNCTION == M_TIME_GETTICKCOUNT

#include <Windows.h>

typedef DWORD m_time;

static inline void m_time_get(m_time *t) {
	*t = GetTickCount();
}

static inline void m_time_diff(m_time *r, m_time *t1, m_time *t2) {
	*r = *t2 - *t1;
}

#define M_TIME_SEC_MSEC_FMT "%u.%03u"
#define M_TIME_SEC_MSEC_ARGS(t) (t)/1000, (t)%1000
#define M_TIME_IN_USEC_DBL(t) ((t)*1000.0)

#else

#error M_TIME_FUNCTION is unknown or not defined

#endif
