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
 * fwrec.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

int
prcfwriterecord(FILE* fp, prc_record_header_t* record)
{
    prc_file_record_header_t fbuf;

    prcuint32tobuf(&fbuf.offset, record->offset);
    prcuint24tobuf(&fbuf.id, record->id);
    fbuf.flags = record->flags;

    if (fwrite(&fbuf, sizeof(prc_file_record_header_t), 1, fp) != 1)
        return -1;

    return 0;
}
