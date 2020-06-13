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
 * recapp.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

PRC_PUBLIC_API(prc_record_t*)
prcappendrecord(prc_t* prc, prc_record_t* rec)
{
    prc_internal_t* iprc = PRC_PRCTOINTERNAL(prc);
    prc_internal_record_t* prev;
    prc_internal_record_t* foo;
    prc_internal_record_t* newGuy = PRC_RECORDTOINTERNAL(rec);
    
    PRC_CLEAR_ERRORNO();

    prev = NULL;
    for (foo = iprc->records; foo != NULL; prev = foo, foo = foo->next)
        ;
    
    if (prev != NULL) {
        prev->next = newGuy;
    } else {
        iprc->records = newGuy;
    }

    iprc->header.nrecords++;

    return &newGuy->u_contents.record;
}
