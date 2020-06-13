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
 * errno.c
 *
 * created: djw@djw.org, June 16, 2001.
 */
#include "prcp.h"

PRC_PUBLIC_API(const char*)
prcstrerror(prc_errno_t e)
{
    const char* rv;

    if (e > PRC_ERROR_NERRORS) {
        e = PRC_ERROR_NERRORS;
    }

#define E_DEF(e, s) case (e): rv = (s); break
    switch(e) {
        E_DEF(PRC_ERROR_NONE,              "no error");
        E_DEF(PRC_ERROR_RECORD_ZEROLENGTH, "zero length record");
        E_DEF(PRC_ERROR_RECORD_TOOBIG,     "record too big (>64K)");
        E_DEF(PRC_ERROR_SEEK_FAILED,       "seek failed");
        E_DEF(PRC_ERROR_READ_FAILED,       "read failed");
        E_DEF(PRC_ERROR_ALLOC_FAILED,      "memory allocation failed");
        E_DEF(PRC_ERROR_STAT_FAILED,       "stat failed");
        E_DEF(PRC_ERROR_OPEN_FAILED,       "open failed");
        E_DEF(PRC_ERROR_BADARG,            "bad argument");
        E_DEF(PRC_ERROR_NERRORS,           "unexpected error");
        E_DEF(PRC_ERROR_NOT_STREAM,        "stream operation not on stream");
        E_DEF(PRC_ERROR_EOF,               "end of file");
    }
#undef E_DEF

    return rv;
}
