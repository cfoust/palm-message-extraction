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
 * create.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

PRC_PUBLIC_API(prc_t*)
prccreate(const char* filename,
          const char* name,
          prc_type_t  type,
          prc_type_t  cid,
          prc_attr_t  attrs)
{
    prc_t* prc;

    PRC_CLEAR_ERRORNO();

    prc = prcopen(filename, PRC_OPEN_CREATE);
    if (!prc)
        return prc;

    prcsetname(prc, name);
    prc->cid = cid;
    prc->type = type;
    prc->ctime = prc->mtime = prctime(NULL);
    prc->btime = 0;
    prc->flags = attrs;

    return prc;
}

