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
 * tobuf.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

void
prcuint16tobuf(void* vbuf, prc_uint16_t i)
{
    prc_byte_t* buf = (prc_byte_t*)vbuf;

    buf[0] = (i >> 8) & 0xff;
    buf[1] = (i) & 0xff;
}

void
prcuint32tobuf(void* vbuf, prc_uint32_t i)
{
    prc_byte_t* buf = (prc_byte_t*)vbuf;

    buf[0] = (prc_byte_t)((i >> 24) & 0xff);
    buf[1] = (prc_byte_t)((i >> 16) & 0xff);
    buf[2] = (prc_byte_t)((i >> 8) & 0xff);
    buf[3] = (prc_byte_t)((i) & 0xff);
}

void
prcuint24tobuf(void* vbuf, prc_uint32_t i)
{
    prc_byte_t* buf = (prc_byte_t*)vbuf;

    buf[0] = (prc_byte_t)((i >> 16) & 0xff);
    buf[1] = (prc_byte_t)((i >> 8) & 0xff);
    buf[2] = (prc_byte_t)((i) & 0xff);
}
