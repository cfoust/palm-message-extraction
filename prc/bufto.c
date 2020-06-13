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
 * bufto.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

prc_uint16_t
prcbuftouint16(const void* vbuf)
{
    prc_byte_t*  buf = (prc_byte_t*)vbuf;
    prc_uint16_t rv;

    rv = 
	((prc_uint16_t)buf[0] << 8) +
	((prc_uint16_t)buf[1] << 0);

    return rv;
}

prc_uint32_t
prcbuftouint32(const void* vbuf)
{
    prc_byte_t*  buf = (prc_byte_t*)vbuf;
    prc_uint32_t rv;

    rv = 
	((prc_uint32_t)buf[0] << 24) +
	((prc_uint32_t)buf[1] << 16) +
	((prc_uint32_t)buf[2] << 8) +
	((prc_uint32_t)buf[3] << 0);

    return rv;
}

prc_uint32_t
prcbuftouint24(const void* vbuf)
{
    prc_byte_t*  buf = (prc_byte_t*)vbuf;
    prc_uint32_t rv;

    rv = 
	((prc_uint32_t)buf[0] << 16) +
	((prc_uint32_t)buf[1] << 8) +
	((prc_uint32_t)buf[2] << 0);

    return rv;
}
