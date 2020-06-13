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
 * rsrcget.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

PRC_PUBLIC_API(prc_resource_t*)
prcgetresource(prc_t* prc, prc_uint32_t type, prc_uint16_t id)
{
    prc_internal_t* iprc = (prc_internal_t*)prc;
    prc_internal_record_t* foo;
    
    PRC_CLEAR_ERRORNO();

    for (foo = iprc->records; foo != NULL; foo = foo->next) {
        if (foo->u_contents.resource.type == type &&
            foo->u_contents.resource.id == id)
            return &foo->u_contents.resource;
    }

    return NULL;
}
