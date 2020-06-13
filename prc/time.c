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
 * time.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

PRC_PUBLIC_API(prc_time_t)
prctime(prc_time_t* buf)
{
    time_t     utime;
    prc_time_t ptime;

    PRC_CLEAR_ERRORNO();

    if (time(&utime) == (time_t)-1)
        return (prc_time_t)-1;

    ptime = UNIX_TO_MAC_TIME(utime);
    if (buf != NULL)
        *buf = ptime;

    return ptime;
}
