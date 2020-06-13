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
 * sortiset.c
 *
 * created: djw@djw.org, May 15, 1999.
 */
#include "prcp.h"

PRC_PUBLIC_API(int)
prcsetsortinfo(prc_t* prc, prc_byte_t* data, prc_uint32_t datalen)
{
    void* cdata;

    PRC_CLEAR_ERRORNO();

    if (datalen > PRC_MAX_CHUNK_SIZE) {
        PRC_SET_ERRORNO(PRC_ERROR_BADARG);
        return -1;
    }

    if (data == NULL || datalen == 0) {
        datalen = 0;
        cdata = NULL;
    } else {
        cdata = (void*)malloc(datalen);
        memcpy(cdata, data, datalen);
    }

    if (prc->sortinfoData != NULL)
        free(prc->sortinfoData);

    prc->sortinfoData = cdata;
    prc->sortinfoSize = datalen;

    return 0;
}
