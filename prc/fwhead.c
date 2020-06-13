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
 * fwhead.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

int
prcfwriteheader(FILE* fp, prc_t* prc)
{
    prc_file_t fbuf;
    prc_uint32_t appInfoOffset = 0;
    prc_uint32_t sortInfoOffset = 0;
    
    if (prc->appinfoData != NULL) {
        appInfoOffset = sizeof(prc_file_t) +
            (sizeof(prc_file_record_header_t) * prc->nrecords) +
            2;
    }
    
    if (prc->sortinfoData != NULL) {
        sortInfoOffset = sizeof(prc_file_t) + appInfoOffset +
            (sizeof(prc_file_record_header_t) * prc->nrecords) +
            2;
    }
    
    memset(&fbuf, 0, sizeof(prc_file_t));
    strncpy(&fbuf.name[0], prc->name, 32);

    prcuint16tobuf(&fbuf.flags, prc->flags);
    prcuint16tobuf(&fbuf.version, prc->version);
    prcuint32tobuf(&fbuf.ctime, prc->ctime);
    prcuint32tobuf(&fbuf.mtime, prc->mtime);
    prcuint32tobuf(&fbuf.btime, prc->btime);
    prcuint32tobuf(&fbuf.modnum, prc->modnum);
    prcuint32tobuf(&fbuf.cid, prc->cid);
    prcuint32tobuf(&fbuf.type, prc->type);
    prcuint16tobuf(&fbuf.nrecords, prc->nrecords);
    prcuint32tobuf(&fbuf.appinfo, appInfoOffset);
    prcuint32tobuf(&fbuf.sortinfo, sortInfoOffset);

    if (fwrite(&fbuf, sizeof(prc_file_t), 1, fp) != 1)
        return -1;

    return 0;
}
