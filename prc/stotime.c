/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is par & libprc, released May 13, 1999.
 * 
 * The Initial Developer of the Original Code is David Williams,
 * http://www.djw.org.  Portions created by David Williams are
 * Copyright (C) 1999 David Williams. All Rights Reserved.
 * Contributor(s):
 */

/*
 * stotime.c
 *
 * [[CC]YY]MMDDhhmm[.SS]
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

static int
twodigits(const char* buf)
{
    int rv = 0;

    if (isdigit(buf[0])) {
        rv = buf[0] - '0';
        if (isdigit(buf[1])) {
            rv = (10 * rv) + (buf[1] - '0');
        }
    }

    return rv;
}

PRC_PUBLIC_API(prc_time_t)
prcstrtotime(const char* buf)
{
    struct tm  tmBuf;
    prc_time_t prc_time;
    int        len;
    time_t     utime;

    int century = -1;
    int year = -1;
    int month;
    int day;
    int hour;
    int minute;
    const char* p;

    PRC_CLEAR_ERRORNO();

    if (!buf)
        return (prc_time_t)-1;
        
    if (strcmp(buf, "now") == 0)
        return prctime(NULL);
    else if (strcmp(buf, "never") == 0)
        return (prc_time_t)0L; /* beginning of time */
    
    len = strlen(buf);
    
    p = buf;
    
    /* [[CC]YY]MMDDhhmm */
    switch (len) {
    case 12: /* CCYYMMDDhhmm */
        century = twodigits(p);
        p += 2;
        /*FALLTHRU*/
    case 10: /* YYMMDDhhmm */
        year = twodigits(p);
        p += 2;
        /*FALLTHRU*/
    case  8: /* MMDDhhmm */
        month = twodigits(p);
        p += 2;
        day = twodigits(p);
        p += 2;
        hour = twodigits(p);
        p += 2;
        minute = twodigits(p);
        p += 2;
        break;
    default:
        return (prc_time_t)-1;
    }

    /* only do this when needed */
    if (century == -1 || year == -1) {
        int        current_year;
        time_t     current_time;
        struct tm* current_tm;
    
        time(&current_time); /* get the current time */
        current_tm = localtime(&current_time);
        current_year = current_tm->tm_year + 1900;
        
        if (century == -1)
            century = current_year / 100;

        if (year == -1)
            year = current_year % 100;
    }
    
    memset(&tmBuf, 0, sizeof(struct tm));

    tmBuf.tm_min = minute;
    tmBuf.tm_hour = hour;
    tmBuf.tm_mday = day;
    tmBuf.tm_mon = month - 1;
    tmBuf.tm_year = ((century * 100) + year) - 1900;
    tmBuf.tm_isdst = -1; /* guess if it is daylight savings */

    /* convert tm to Unix time_t */
    utime = mktime(&tmBuf);

    if (utime == (time_t)-1)
        return (prc_time_t)-1;

    prc_time = UNIX_TO_MAC_TIME(utime);

    return prc_time;
}
