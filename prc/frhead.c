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
 * frhead.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

int
prcfreadheader(FILE* fp,
               prc_t* prc,
               prc_uint32_t* appInfoOffset_r,
               prc_uint32_t* sortInfoOffset_r)
{
    prc_file_t fbuf;
    prc_uint32_t appInfoOffset;
    prc_uint32_t sortInfoOffset;

    if (fread(&fbuf, sizeof(prc_file_t), 1, fp) != 1)
        return -1;

    memset(prc, 0, sizeof(prc_t));
    strncpy(&prc->name[0], fbuf.name, 32);

    prc->flags = prcbuftouint16(&fbuf.flags);
    prc->version = prcbuftouint16(&fbuf.version);
    prc->ctime = prcbuftouint32(&fbuf.ctime);
    prc->mtime = prcbuftouint32(&fbuf.mtime);
    prc->btime = prcbuftouint32(&fbuf.btime);
    prc->modnum = prcbuftouint32(&fbuf.modnum);
    appInfoOffset = prcbuftouint32(&fbuf.appinfo);
    sortInfoOffset = prcbuftouint32(&fbuf.sortinfo);
    prc->type = prcbuftouint32(&fbuf.type);
    prc->cid = prcbuftouint32(&fbuf.cid);
    /* seedID */
    /* nextRecord */
    prc->nrecords = prcbuftouint16(&fbuf.nrecords);

    /* if sortInfo != 0, use it to determine appInfo size */
    *appInfoOffset_r = appInfoOffset;
    *sortInfoOffset_r = sortInfoOffset;

    return 0;
}
