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
 * open.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

PRC_PUBLIC_API(prc_t*)
prcopen(const char* name, unsigned mode)
{
    const char* modes;
    FILE* fp;
    prc_t* prc;

    PRC_FILE_CREATOR_TYPE_PUSH();
    PRC_FILE_CREATOR_TYPE_SET(PRC_PRC_CREATOR, PRC_PRC_TYPE);

    PRC_CLEAR_ERRORNO();

    if ((mode & PRC_OPEN_CREATE) != 0) {
        modes = "wb";
    } else {
        modes = "rb";
    }
    
    fp = fopen(name, modes);
    
    if (fp == NULL) {
        PRC_SET_ERRORNO(PRC_ERROR_OPEN_FAILED);
        return NULL;
    }

    prc = prcfpopen(fp, mode);

    if (prc != NULL &&
        (mode & (PRC_OPEN_TOUCH|PRC_OPEN_UPDATE|PRC_OPEN_WRITE)) != 0) {
        prc_internal_t* iprc = (prc_internal_t*)prc;

        /* Now that we have read the old file, reopen for writing */
        fclose(fp);

        fp = fopen(name, "wb");

        if (fp == NULL){
            PRC_SET_ERRORNO(PRC_ERROR_OPEN_FAILED);
            return NULL;
        }
        iprc->fp = fp;
    }

    PRC_FILE_CREATOR_TYPE_POP();
    
    return prc;
}
