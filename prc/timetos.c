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
 * timetos.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

PRC_PUBLIC_API(char*)
prctimetostr(char* buf, prc_time_t prc_time)
{
    time_t      utime;
    struct tm*  tm_p;
    size_t      rv = 0;
    const char* result_string = "error";

    PRC_CLEAR_ERRORNO();

    if (prc_time == 0) {
        /*
         * Win32 localtime() can't deal with negative utime, so
         * handle the ones we care about here:
         */
        result_string = "190401010000";
    } else {
        /*
         * Of course all other early dates will fail, but we tried...
         */
        utime = MAC_TO_UNIX_TIME(prc_time);
        tm_p  = localtime(&utime); /* should this be gmtime()? */

        if (tm_p != NULL) {

            /* CCYYMMDDhhmm */
            rv = sprintf(buf,
                         "%04d%02d%02d%02d%02d",
                         (tm_p->tm_year + 1900),
                         (tm_p->tm_mon + 1),
                         tm_p->tm_mday,
                         tm_p->tm_hour,
                         tm_p->tm_min);
            
            if (rv == PRC_TIME_DEFAULT_FORMAT_LEN)
                result_string = buf;
        }
    }

    if (result_string != buf)
        strcpy(buf, result_string);
    
    return buf;
}
