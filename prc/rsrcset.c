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
 * rsrcset.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

int
prccommonset(prc_internal_record_t* common,
             prc_byte_t*   data,
             prc_uint32_t  datalen)
{
    prc_resource_t* rsrc = (prc_resource_t*)common;
    void* cdata;

    if (data == NULL || datalen == 0) {
        datalen = 0;
        cdata = NULL;
    } else {
        cdata = (void*)malloc(datalen);
        memcpy(cdata, data, datalen);
    }

    if (rsrc->data != NULL)
        free(rsrc->data);

    rsrc->data = cdata;
    rsrc->datalen = datalen;

    return 0;
}

PRC_PUBLIC_API(int)
prcresourceset(prc_resource_t* rsrc,
               prc_uint32_t  type,
               prc_uint16_t  id,
               prc_byte_t*   data,
               prc_uint32_t  datalen)
{
    int rv;

    PRC_CLEAR_ERRORNO();

    if (datalen > PRC_MAX_CHUNK_SIZE) {
        PRC_SET_ERRORNO(PRC_ERROR_BADARG);
        return -1;
    }

    rv = prccommonset((prc_internal_record_t*)rsrc, data, datalen);

    if (rv != 0)
        return rv;

    rsrc->type = type;
    rsrc->id = id;

    return 0;
}
