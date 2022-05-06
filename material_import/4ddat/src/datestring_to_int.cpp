//
//  4ddat_to_admf.cpp
//  4ddat_to_admf
//
//  Created by yushihang on 2021/3/26.
//

#include "datestring_to_int.hpp"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#ifdef _WIN32
#include "bson-macros.h"
#endif
namespace admfExport {
    
#define BSON_ERROR_JSON 1
#define BSON_ERROR_READER 2
#define BSON_ERROR_INVALID 3
    
    
#ifndef WRONG
#define WRONG (-1)
#endif /* !defined WRONG */

    
    /* avoid system-dependent struct tm definitions */
    struct bson_tm {
        int64_t tm_sec;    /* seconds after the minute [0-60] */
        int64_t tm_min;    /* minutes after the hour [0-59] */
        int64_t tm_hour;   /* hours since midnight [0-23] */
        int64_t tm_mday;   /* day of the month [1-31] */
        int64_t tm_mon;    /* months since January [0-11] */
        int64_t tm_year;   /* years since 1900 */
        int64_t tm_wday;   /* days since Sunday [0-6] */
        int64_t tm_yday;   /* days since January 1 [0-365] */
        int64_t tm_isdst;  /* Daylight Savings Time flag */
        int64_t tm_gmtoff; /* offset from CUT in seconds */
        char *tm_zone;     /* timezone abbreviation */
    };
    
    typedef enum {
        BSON_JSON_ERROR_READ_CORRUPT_JS = 1,
        BSON_JSON_ERROR_READ_INVALID_PARAM,
        BSON_JSON_ERROR_READ_CB_FAILURE,
    } bson_json_error_code_t;
    
    
    static int64_t
    increment_overflow32 (int_fast32_t *const lp, int64_t const m)
    {
        register int_fast32_t const l = *lp;
        
        if ((l >= 0) ? (m > INT_FAST32_MAX - l) : (m < INT_FAST32_MIN - l))
            return true;
        *lp += m;
        return false;
    }

    static int64_t
    increment_overflow (int64_t *const ip, int64_t j)
    {
        register int64_t const i = *ip;
        
        /*
         ** If i >= 0 there can only be overflow if i + j > INT_MAX
         ** or if j > INT_MAX - i; given i >= 0, INT_MAX - i cannot overflow.
         ** If i < 0 there can only be overflow if i + j < INT_MIN
         ** or if j < INT_MIN - i; given i < 0, INT_MIN - i cannot overflow.
         */
        if ((i >= 0) ? (j > INT_MAX - i) : (j < INT_MIN - i))
            return true;
        *ip += j;
        return false;
    }
    
    
#ifndef TZ_MAX_TIMES
#define TZ_MAX_TIMES 2000
#endif /* !defined TZ_MAX_TIMES */
    
#ifndef TZ_MAX_TYPES
    /* This must be at least 17 for Europe/Samara and Europe/Vilnius.  */
#define TZ_MAX_TYPES 256 /* Limited by what (unsigned char)'s can hold */
#endif                   /* !defined TZ_MAX_TYPES */

#ifndef TZ_MAX_CHARS
#define TZ_MAX_CHARS 50 /* Maximum number of abbreviation characters */
    /* (limited by what unsigned chars can hold) */
#endif                  /* !defined TZ_MAX_CHARS */
    
#ifndef TZ_MAX_LEAPS
#define TZ_MAX_LEAPS 50 /* Maximum number of leap second corrections */
#endif


    struct ttinfo {            /* time type information */
        int_fast32_t tt_gmtoff; /* UT offset in seconds */
        int tt_isdst;           /* used to set tm_isdst */
        int tt_abbrind;         /* abbreviation list index */
        int tt_ttisstd;         /* true if transition is std time */
        int tt_ttisgmt;         /* true if transition is UT */
    };
    
    struct lsinfo {          /* leap second information */
        int64_t ls_trans;      /* transition time */
        int_fast64_t ls_corr; /* correction to apply */
    };
    
#define BIGGEST(a, b) (((a) > (b)) ? (a) : (b))
    
#ifdef TZNAME_MAX
#define MY_TZNAME_MAX TZNAME_MAX
#endif /* defined TZNAME_MAX */
#ifndef TZNAME_MAX
#define MY_TZNAME_MAX 255
#endif /* !defined TZNAME_MAX */
    
    struct state {
        int leapcnt;
        int timecnt;
        int typecnt;
        int charcnt;
        int goback;
        int goahead;
        int64_t ats[TZ_MAX_TIMES];
        unsigned char types[TZ_MAX_TIMES];
        struct ttinfo ttis[TZ_MAX_TYPES];
        char chars[BIGGEST (TZ_MAX_CHARS + 1, (2 * (MY_TZNAME_MAX + 1)))];
        struct lsinfo lsis[TZ_MAX_LEAPS];
        int defaulttype; /* for early times or if no transitions */
    };
    
    static int gmt_is_set;
    static struct state gmtmem;
#define gmtptr (&gmtmem)
    
    
#define SECSPERMIN 60
#define MINSPERHOUR 60
#define HOURSPERDAY 24
#define DAYSPERWEEK 7
#define DAYSPERNYEAR 365
#define DAYSPERLYEAR 366
#define SECSPERHOUR (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY ((int_fast32_t) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR 12
    
#define TM_SUNDAY 0
#define TM_MONDAY 1
#define TM_TUESDAY 2
#define TM_WEDNESDAY 3
#define TM_THURSDAY 4
#define TM_FRIDAY 5
#define TM_SATURDAY 6
    
#define TM_JANUARY 0
#define TM_FEBRUARY 1
#define TM_MARCH 2
#define TM_APRIL 3
#define TM_MAY 4
#define TM_JUNE 5
#define TM_JULY 6
#define TM_AUGUST 7
#define TM_SEPTEMBER 8
#define TM_OCTOBER 9
#define TM_NOVEMBER 10
#define TM_DECEMBER 11
    
#define TM_YEAR_BASE 1900
    
#define EPOCH_YEAR 1970
#define EPOCH_WDAY TM_THURSDAY
    
#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))
    
    static const int mon_lengths[2][MONSPERYEAR] = {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
    
    static const int year_lengths[2] = {DAYSPERNYEAR, DAYSPERLYEAR};
    
    static void
    gmtload (struct state *const sp)
    {
        memset (sp, 0, sizeof (struct state));
        sp->typecnt = 1;
        sp->charcnt = 4;
        sp->chars[0] = 'G';
        sp->chars[1] = 'M';
        sp->chars[2] = 'T';
    }
    
    

    static int64_t
    leaps_thru_end_of (register const int64_t y)
    {
        return (y >= 0) ? (y / 4 - y / 100 + y / 400)
        : -(leaps_thru_end_of (-(y + 1)) + 1);
    }
    
    static struct bson_tm *
    timesub (const int64_t *const timep,
             const int_fast32_t offset,
             register const struct state *const sp,
             register struct bson_tm *const tmp)
    {
        register const struct lsinfo *lp;
        register int64_t tdays;
        register int64_t idays; /* unsigned would be so 2003 */
        register int_fast64_t rem;
        int64_t y;
        register const int *ip;
        register int_fast64_t corr;
        register int64_t hit;
        register int64_t i;
        
        corr = 0;
        hit = 0;
        i = (sp == NULL) ? 0 : sp->leapcnt;
        while (--i >= 0) {
            lp = &sp->lsis[i];
            if (*timep >= lp->ls_trans) {
                if (*timep == lp->ls_trans) {
                    hit = ((i == 0 && lp->ls_corr > 0) ||
                           lp->ls_corr > sp->lsis[i - 1].ls_corr);
                    if (hit)
                        while (i > 0 &&
                               sp->lsis[i].ls_trans == sp->lsis[i - 1].ls_trans + 1 &&
                               sp->lsis[i].ls_corr == sp->lsis[i - 1].ls_corr + 1) {
                            ++hit;
                            --i;
                        }
                }
                corr = lp->ls_corr;
                break;
            }
        }
        y = EPOCH_YEAR;
        tdays = *timep / SECSPERDAY;
        rem = *timep - tdays * SECSPERDAY;
        while (tdays < 0 || tdays >= year_lengths[isleap (y)]) {
            int64_t newy;
            register int64_t tdelta;
            register int64_t idelta;
            register int64_t leapdays;
            
            tdelta = tdays / DAYSPERLYEAR;
            idelta = tdelta;
            if (idelta == 0)
                idelta = (tdays < 0) ? -1 : 1;
            newy = y;
            if (increment_overflow (&newy, idelta))
                return NULL;
            leapdays = leaps_thru_end_of (newy - 1) - leaps_thru_end_of (y - 1);
            tdays -= ((int64_t) newy - y) * DAYSPERNYEAR;
            tdays -= leapdays;
            y = newy;
        }
        {
            register int_fast32_t seconds;
            
            seconds = (int_fast32_t) (tdays * SECSPERDAY);
            tdays = seconds / SECSPERDAY;
            rem += seconds - tdays * SECSPERDAY;
        }
        /*
         ** Given the range, we can now fearlessly cast...
         */
        idays = (int64_t) tdays;
        rem += offset - corr;
        while (rem < 0) {
            rem += SECSPERDAY;
            --idays;
        }
        while (rem >= SECSPERDAY) {
            rem -= SECSPERDAY;
            ++idays;
        }
        while (idays < 0) {
            if (increment_overflow (&y, -1))
                return NULL;
            idays += year_lengths[isleap (y)];
        }
        while (idays >= year_lengths[isleap (y)]) {
            idays -= year_lengths[isleap (y)];
            if (increment_overflow (&y, 1))
                return NULL;
        }
        tmp->tm_year = y;
        if (increment_overflow (&tmp->tm_year, -TM_YEAR_BASE))
            return NULL;
        tmp->tm_yday = idays;
        /*
         ** The "extra" mods below avoid overflow problems.
         */
        tmp->tm_wday =
        EPOCH_WDAY +
        ((y - EPOCH_YEAR) % DAYSPERWEEK) * (DAYSPERNYEAR % DAYSPERWEEK) +
        leaps_thru_end_of (y - 1) - leaps_thru_end_of (EPOCH_YEAR - 1) + idays;
        tmp->tm_wday %= DAYSPERWEEK;
        if (tmp->tm_wday < 0)
            tmp->tm_wday += DAYSPERWEEK;
        tmp->tm_hour = (int64_t) (rem / SECSPERHOUR);
        rem %= SECSPERHOUR;
        tmp->tm_min = (int64_t) (rem / SECSPERMIN);
        /*
         ** A positive leap second requires a special
         ** representation. This uses "... ??:59:60" et seq.
         */
        tmp->tm_sec = (int64_t) (rem % SECSPERMIN) + hit;
        ip = mon_lengths[isleap (y)];
        for (tmp->tm_mon = 0; idays >= ip[tmp->tm_mon]; ++(tmp->tm_mon))
            idays -= ip[tmp->tm_mon];
        tmp->tm_mday = (int64_t) (idays + 1);
        tmp->tm_isdst = 0;
#ifdef TM_GMTOFF
        tmp->TM_GMTOFF = offset;
#endif /* defined TM_GMTOFF */
        return tmp;
    }
    
    
    static struct bson_tm *
    gmtsub (const int64_t *const timep,
            const int_fast32_t offset,
            struct bson_tm *const tmp)
    {
        register struct bson_tm *result;
        
        if (!gmt_is_set) {
            gmt_is_set = true;
            gmtload (gmtptr);
        }
        result = timesub (timep, offset, gmtptr, tmp);
#ifdef TM_ZONE
        /*
         ** Could get fancy here and deliver something such as
         ** "UT+xxxx" or "UT-xxxx" if offset is non-zero,
         ** but this is no time for a treasure hunt.
         */
        tmp->TM_ZONE = offset ? wildabbr : gmtptr ? gmtptr->chars : gmt;
#endif /* defined TM_ZONE */
        return result;
    }
    
    
    static int64_t
    normalize_overflow (int64_t *const tensptr, int64_t *const unitsptr, const int64_t base)
    {
        register int64_t tensdelta;
        
        tensdelta =
        (*unitsptr >= 0) ? (*unitsptr / base) : (-1 - (-1 - *unitsptr) / base);
        *unitsptr -= tensdelta * base;
        return increment_overflow (tensptr, tensdelta);
    }
    
    static int64_t
    normalize_overflow32 (int_fast32_t *const tensptr,
                          int64_t *const unitsptr,
                          const int64_t base)
    {
        register int64_t tensdelta;
        
        tensdelta =
        (*unitsptr >= 0) ? (*unitsptr / base) : (-1 - (-1 - *unitsptr) / base);
        *unitsptr -= tensdelta * base;
        return increment_overflow32 (tensptr, tensdelta);
    }
    
    static int64_t
    tmcomp (register const struct bson_tm *const atmp,
            register const struct bson_tm *const btmp)
    {
        register int64_t result;
        
        if (atmp->tm_year != btmp->tm_year)
            return atmp->tm_year < btmp->tm_year ? -1 : 1;
        if ((result = (atmp->tm_mon - btmp->tm_mon)) == 0 &&
            (result = (atmp->tm_mday - btmp->tm_mday)) == 0 &&
            (result = (atmp->tm_hour - btmp->tm_hour)) == 0 &&
            (result = (atmp->tm_min - btmp->tm_min)) == 0)
            result = atmp->tm_sec - btmp->tm_sec;
        return result;
    }
    
    
    /* The minimum and maximum finite time values.  */
    static int64_t const time_t_min = INT64_MIN;
    static int64_t const time_t_max = INT64_MAX;
    
    static int64_t
    time2sub (struct bson_tm *const tmp,
              struct bson_tm *(*const funcp) (const int64_t *, int_fast32_t, struct bson_tm *),
              const int_fast32_t offset,
              int64_t *const okayp,
              const int64_t do_norm_secs)
    {
        register const struct state *sp;
        register int64_t dir;
        register int64_t i, j;
        register int64_t saved_seconds;
        register int_fast32_t li;
        register int64_t lo;
        register int64_t hi;
        int_fast32_t y;
        int64_t newt;
        int64_t t;
        struct bson_tm yourtm, mytm;
        
        *okayp = false;
        yourtm = *tmp;
        if (do_norm_secs) {
            if (normalize_overflow (&yourtm.tm_min, &yourtm.tm_sec, SECSPERMIN))
                return WRONG;
        }
        if (normalize_overflow (&yourtm.tm_hour, &yourtm.tm_min, MINSPERHOUR))
            return WRONG;
        if (normalize_overflow (&yourtm.tm_mday, &yourtm.tm_hour, HOURSPERDAY))
            return WRONG;
        y = (int_fast32_t) yourtm.tm_year;
        if (normalize_overflow32 (&y, &yourtm.tm_mon, MONSPERYEAR))
            return WRONG;
        /*
         ** Turn y into an actual year number for now.
         ** It is converted back to an offset from TM_YEAR_BASE later.
         */
        if (increment_overflow32 (&y, TM_YEAR_BASE))
            return WRONG;
        while (yourtm.tm_mday <= 0) {
            if (increment_overflow32 (&y, -1))
                return WRONG;
            li = y + (1 < yourtm.tm_mon);
            yourtm.tm_mday += year_lengths[isleap (li)];
        }
        while (yourtm.tm_mday > DAYSPERLYEAR) {
            li = y + (1 < yourtm.tm_mon);
            yourtm.tm_mday -= year_lengths[isleap (li)];
            if (increment_overflow32 (&y, 1))
                return WRONG;
        }
        for (;;) {
            i = mon_lengths[isleap (y)][yourtm.tm_mon];
            if (yourtm.tm_mday <= i)
                break;
            yourtm.tm_mday -= i;
            if (++yourtm.tm_mon >= MONSPERYEAR) {
                yourtm.tm_mon = 0;
                if (increment_overflow32 (&y, 1))
                    return WRONG;
            }
        }
        if (increment_overflow32 (&y, -TM_YEAR_BASE))
            return WRONG;
        yourtm.tm_year = y;
        if (yourtm.tm_year != y)
            return WRONG;
        if (yourtm.tm_sec >= 0 && yourtm.tm_sec < SECSPERMIN)
            saved_seconds = 0;
        else if (y + TM_YEAR_BASE < EPOCH_YEAR) {
            /*
             ** We can't set tm_sec to 0, because that might push the
             ** time below the minimum representable time.
             ** Set tm_sec to 59 instead.
             ** This assumes that the minimum representable time is
             ** not in the same minute that a leap second was deleted from,
             ** which is a safer assumption than using 58 would be.
             */
            if (increment_overflow (&yourtm.tm_sec, 1 - SECSPERMIN))
                return WRONG;
            saved_seconds = yourtm.tm_sec;
            yourtm.tm_sec = SECSPERMIN - 1;
        } else {
            saved_seconds = yourtm.tm_sec;
            yourtm.tm_sec = 0;
        }
        /*
         ** Do a binary search.
         */
        lo = INT64_MIN;
        hi = INT64_MAX;
        
        for (;;) {
            t = lo / 2 + hi / 2;
            if (t < lo)
                t = lo;
            else if (t > hi)
                t = hi;
            if ((*funcp) (&t, offset, &mytm) == NULL) {
                /*
                 ** Assume that t is too extreme to be represented in
                 ** a struct bson_tm; arrange things so that it is less
                 ** extreme on the next pass.
                 */
                dir = (t > 0) ? 1 : -1;
            } else
                dir = tmcomp (&mytm, &yourtm);
            if (dir != 0) {
                if (t == lo) {
                    if (t == time_t_max)
                        return WRONG;
                    ++t;
                    ++lo;
                } else if (t == hi) {
                    if (t == time_t_min)
                        return WRONG;
                    --t;
                    --hi;
                }
                if (lo > hi)
                    return WRONG;
                if (dir > 0)
                    hi = t;
                else
                    lo = t;
                continue;
            }
            if (yourtm.tm_isdst < 0 || mytm.tm_isdst == yourtm.tm_isdst)
                break;
            /*
             ** Right time, wrong type.
             ** Hunt for right time, right type.
             ** It's okay to guess wrong since the guess
             ** gets checked.
             */
            sp = (const struct state *) gmtptr;
            if (sp == NULL)
                return WRONG;
            for (i = sp->typecnt - 1; i >= 0; --i) {
                if (sp->ttis[i].tt_isdst != yourtm.tm_isdst)
                    continue;
                for (j = sp->typecnt - 1; j >= 0; --j) {
                    if (sp->ttis[j].tt_isdst == yourtm.tm_isdst)
                        continue;
                    newt = t + sp->ttis[j].tt_gmtoff - sp->ttis[i].tt_gmtoff;
                    if ((*funcp) (&newt, offset, &mytm) == NULL)
                        continue;
                    if (tmcomp (&mytm, &yourtm) != 0)
                        continue;
                    if (mytm.tm_isdst != yourtm.tm_isdst)
                        continue;
                    /*
                     ** We have a match.
                     */
                    t = newt;
                    goto label;
                }
            }
            return WRONG;
        }
    label:
        newt = t + saved_seconds;
        if ((newt < t) != (saved_seconds < 0))
            return WRONG;
        t = newt;
        if ((*funcp) (&t, offset, tmp))
            *okayp = true;
        return t;
    }

    
    static int64_t
    time2 (struct bson_tm *const tmp,
           struct bson_tm *(*const funcp) (const int64_t *, int_fast32_t, struct bson_tm *),
           const int_fast32_t offset,
           int64_t *const okayp)
    {
        int64_t t;
        
        /*
         ** First try without normalization of seconds
         ** (in case tm_sec contains a value associated with a leap second).
         ** If that fails, try with normalization of seconds.
         */
        t = time2sub (tmp, funcp, offset, okayp, false);
        return *okayp ? t : time2sub (tmp, funcp, offset, okayp, true);
    }
    
    static int64_t
    time1 (struct bson_tm *const tmp,
           struct bson_tm *(*const funcp) (const int64_t *, int_fast32_t, struct bson_tm *),
           const int_fast32_t offset)
    {
        register int64_t t;
        register const struct state *sp;
        register int64_t samei, otheri;
        register int64_t sameind, otherind;
        register int64_t i;
        register int64_t nseen;
        int64_t seen[TZ_MAX_TYPES];
        int64_t types[TZ_MAX_TYPES];
        int64_t okay;
        
        if (tmp == NULL) {
            errno = EINVAL;
            return WRONG;
        }
        if (tmp->tm_isdst > 1)
            tmp->tm_isdst = 1;
        t = time2 (tmp, funcp, offset, &okay);
        if (okay)
            return t;
        if (tmp->tm_isdst < 0)
#ifdef PCTS
        /*
         ** POSIX Conformance Test Suite code courtesy Grant Sullivan.
         */
            tmp->tm_isdst = 0; /* reset to std and try again */
#else
        return t;
#endif /* !defined PCTS */
        /*
         ** We're supposed to assume that somebody took a time of one type
         ** and did some math on it that yielded a "struct tm" that's bad.
         ** We try to divine the type they started from and adjust to the
         ** type they need.
         */
        sp = (const struct state *) gmtptr;
        if (sp == NULL)
            return WRONG;
        for (i = 0; i < sp->typecnt; ++i)
            seen[i] = false;
        nseen = 0;
        for (i = sp->timecnt - 1; i >= 0; --i)
            if (!seen[sp->types[i]]) {
                seen[sp->types[i]] = true;
                types[nseen++] = sp->types[i];
            }
        for (sameind = 0; sameind < nseen; ++sameind) {
            samei = types[sameind];
            if (sp->ttis[samei].tt_isdst != tmp->tm_isdst)
                continue;
            for (otherind = 0; otherind < nseen; ++otherind) {
                otheri = types[otherind];
                if (sp->ttis[otheri].tt_isdst == tmp->tm_isdst)
                    continue;
                tmp->tm_sec += sp->ttis[otheri].tt_gmtoff - sp->ttis[samei].tt_gmtoff;
                tmp->tm_isdst = !tmp->tm_isdst;
                t = time2 (tmp, funcp, offset, &okay);
                if (okay)
                    return t;
                tmp->tm_sec -= sp->ttis[otheri].tt_gmtoff - sp->ttis[samei].tt_gmtoff;
                tmp->tm_isdst = !tmp->tm_isdst;
            }
        }
        return WRONG;
    }

    
    int64_t
    _bson_timegm (struct bson_tm *const tmp)
    {
        if (tmp != NULL)
            tmp->tm_isdst = 0;
        return time1 (tmp, gmtsub, 0L);
    }

    
    int
    bson_vsnprintf (char *str,          /* IN */
                    size_t size,        /* IN */
                    const char *format, /* IN */
                    va_list ap)         /* IN */
    {
#ifdef _MSC_VER
        int r = -1;
        
        BSON_ASSERT (str);
        
        if (size != 0) {
            r = _vsnprintf_s (str, size, _TRUNCATE, format, ap);
        }
        
        if (r == -1) {
            r = _vscprintf (format, ap);
        }
        
        str[size - 1] = '\0';
        
        return r;
#else
        int r;
        
        r = vsnprintf (str, size, format, ap);
        str[size - 1] = '\0';
        return r;
#endif
    }
    
    void
    bson_set_error (bson_error_t *error, /* OUT */
                    uint32_t domain,     /* IN */
                    uint32_t code,       /* IN */
                    const char *format,  /* IN */
                    ...)                 /* IN */
    {
        va_list args;
        
        if (error) {
            error->domain = domain;
            error->code = code;
            
            va_start (args, format);
            bson_vsnprintf (error->message, sizeof error->message, format, args);
            va_end (args);
            
            error->message[sizeof error->message - 1] = '\0';
        }
    }
    
    static bool
    get_tok (const char *terminals,
             const char **ptr,
             int32_t *remaining,
             const char **out,
             int32_t *out_len)
    {
        const char *terminal;
        bool found_terminal = false;
        
        if (!*remaining) {
            *out = "";
            *out_len = 0;
        }
        
        *out = *ptr;
        *out_len = -1;
        
        for (; *remaining && !found_terminal;
             (*ptr)++, (*remaining)--, (*out_len)++) {
            for (terminal = terminals; *terminal; terminal++) {
                if (**ptr == *terminal) {
                    found_terminal = true;
                    break;
                }
            }
        }
        
        if (!found_terminal) {
            (*out_len)++;
        }
        
        return found_terminal;
    }
    
    
    static bool
    digits_only (const char *str, int32_t len)
    {
        int i;
        
        for (i = 0; i < len; i++) {
            if (!isdigit (str[i])) {
                return false;
            }
        }
        
        return true;
    }

    
    static bool
    parse_num (const char *str,
               int32_t len,
               int32_t digits,
               int32_t min,
               int32_t max,
               int32_t *out)
    {
        int i;
        int magnitude = 1;
        int32_t value = 0;
        
        if ((digits >= 0 && len != digits) || !digits_only (str, len)) {
            return false;
        }
        
        for (i = 1; i <= len; i++, magnitude *= 10) {
            value += (str[len - i] - '0') * magnitude;
        }
        
        if (value < min || value > max) {
            return false;
        }
        
        *out = value;
        
        return true;
    }
    
    bool
    _bson_iso8601_date_parse (const char *str,
                              int32_t len,
                              int64_t *out,
                              bson_error_t *error)
    {
        const char *ptr = NULL;
        int32_t remaining = len;
        
        const char *year_ptr = NULL;
        const char *month_ptr = NULL;
        const char *day_ptr = NULL;
        const char *hour_ptr = NULL;
        const char *min_ptr = NULL;
        const char *sec_ptr = NULL;
        const char *millis_ptr = NULL;
        const char *tz_ptr = NULL;
        
        int32_t year_len = 0;
        int32_t month_len = 0;
        int32_t day_len = 0;
        int32_t hour_len = 0;
        int32_t min_len = 0;
        int32_t sec_len = 0;
        int32_t millis_len = 0;
        int32_t tz_len = 0;
        
        int32_t year;
        int32_t month;
        int32_t day;
        int32_t hour;
        int32_t min;
        int32_t sec = 0;
        int64_t millis = 0;
        int32_t tz_adjustment = 0;
        
        struct bson_tm posix_date = {0};
        
#define DATE_PARSE_ERR(msg)                                \
bson_set_error (error,                                  \
BSON_ERROR_JSON,                        \
BSON_JSON_ERROR_READ_INVALID_PARAM,     \
"Could not parse \"%s\" as date: " msg, \
str);                                   \
return false
        
#define DEFAULT_DATE_PARSE_ERR                                                 \
DATE_PARSE_ERR ("use ISO8601 format yyyy-mm-ddThh:mm plus timezone, either" \
" \"Z\" or like \"+0500\"")
        
        ptr = str;
        
        /* we have to match at least yyyy-mm-ddThh:mm */
        if (!(get_tok ("-", &ptr, &remaining, &year_ptr, &year_len) &&
              get_tok ("-", &ptr, &remaining, &month_ptr, &month_len) &&
              get_tok ("T", &ptr, &remaining, &day_ptr, &day_len) &&
              get_tok (":", &ptr, &remaining, &hour_ptr, &hour_len) &&
              get_tok (":+-Z", &ptr, &remaining, &min_ptr, &min_len))) {
            DEFAULT_DATE_PARSE_ERR;
        }
        
        /* if the minute has a ':' at the end look for seconds */
        if (min_ptr[min_len] == ':') {
            if (remaining < 2) {
                DATE_PARSE_ERR ("reached end of date while looking for seconds");
            }
            
            get_tok (".+-Z", &ptr, &remaining, &sec_ptr, &sec_len);
            
            if (!sec_len) {
                DATE_PARSE_ERR ("minute ends in \":\" seconds is required");
            }
        }
        
        /* if we had a second and it is followed by a '.' look for milliseconds */
        if (sec_len && sec_ptr[sec_len] == '.') {
            if (remaining < 2) {
                DATE_PARSE_ERR ("reached end of date while looking for milliseconds");
            }
            
            get_tok ("+-Z", &ptr, &remaining, &millis_ptr, &millis_len);
            
            if (!millis_len) {
                DATE_PARSE_ERR ("seconds ends in \".\", milliseconds is required");
            }
        }
        
        /* backtrack by 1 to put ptr on the timezone */
        ptr--;
        remaining++;
        
        get_tok ("", &ptr, &remaining, &tz_ptr, &tz_len);
        
        if (!parse_num (year_ptr, year_len, 4, -9999, 9999, &year)) {
            DATE_PARSE_ERR ("year must be an integer");
        }
        
        /* values are as in struct tm */
        year -= 1900;
        
        if (!parse_num (month_ptr, month_len, 2, 1, 12, &month)) {
            DATE_PARSE_ERR ("month must be an integer");
        }
        
        /* values are as in struct tm */
        month -= 1;
        
        if (!parse_num (day_ptr, day_len, 2, 1, 31, &day)) {
            DATE_PARSE_ERR ("day must be an integer");
        }
        
        if (!parse_num (hour_ptr, hour_len, 2, 0, 23, &hour)) {
            DATE_PARSE_ERR ("hour must be an integer");
        }
        
        if (!parse_num (min_ptr, min_len, 2, 0, 59, &min)) {
            DATE_PARSE_ERR ("minute must be an integer");
        }
        
        if (sec_len && !parse_num (sec_ptr, sec_len, 2, 0, 60, &sec)) {
            DATE_PARSE_ERR ("seconds must be an integer");
        }
        
        if (tz_len > 0) {
            if (tz_ptr[0] == 'Z' && tz_len == 1) {
                /* valid */
            } else if (tz_ptr[0] == '+' || tz_ptr[0] == '-') {
                int32_t tz_hour;
                int32_t tz_min;
                
                if (tz_len != 5 || !digits_only (tz_ptr + 1, 4)) {
                    DATE_PARSE_ERR ("could not parse timezone");
                }
                
                if (!parse_num (tz_ptr + 1, 2, -1, -23, 23, &tz_hour)) {
                    DATE_PARSE_ERR ("timezone hour must be at most 23");
                }
                
                if (!parse_num (tz_ptr + 3, 2, -1, 0, 59, &tz_min)) {
                    DATE_PARSE_ERR ("timezone minute must be at most 59");
                }
                
                /* we inflect the meaning of a 'positive' timezone.  Those are hours
                 * we have to substract, and vice versa */
                tz_adjustment =
                (tz_ptr[0] == '-' ? 1 : -1) * ((tz_min * 60) + (tz_hour * 60 * 60));
                
                if (!(tz_adjustment > -86400 && tz_adjustment < 86400)) {
                    DATE_PARSE_ERR ("timezone offset must be less than 24 hours");
                }
            } else {
                DATE_PARSE_ERR ("timezone is required");
            }
        }
        
        if (millis_len > 0) {
            int i;
            int magnitude;
            millis = 0;
            
            if (millis_len > 3 || !digits_only (millis_ptr, millis_len)) {
                DATE_PARSE_ERR ("milliseconds must be an integer");
            }
            
            for (i = 1, magnitude = 1; i <= millis_len; i++, magnitude *= 10) {
                millis += (millis_ptr[millis_len - i] - '0') * magnitude;
            }
            
            if (millis_len == 1) {
                millis *= 100;
            } else if (millis_len == 2) {
                millis *= 10;
            }
            
            if (millis < 0 || millis > 1000) {
                DATE_PARSE_ERR ("milliseconds must be at least 0 and less than 1000");
            }
        }
        
        posix_date.tm_sec = sec;
        posix_date.tm_min = min;
        posix_date.tm_hour = hour;
        posix_date.tm_mday = day;
        posix_date.tm_mon = month;
        posix_date.tm_year = year;
        posix_date.tm_wday = 0;
        posix_date.tm_yday = 0;
        
        millis = 1000 * _bson_timegm (&posix_date) + millis;
        millis += tz_adjustment * 1000;
        *out = millis;
        
        return true;
    }

}
