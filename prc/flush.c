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
 * flush.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

static prc_uint32_t
flush_app_info(prc_internal_t* iprc)
{
    size_t n;
    n = fwrite(iprc->header.appinfoData, 1,
               (size_t)iprc->header.appinfoSize, iprc->fp);
    if (n != (size_t)iprc->header.appinfoSize)
        return (prc_uint32_t)-1;
    return (prc_uint32_t)n;
}

static prc_uint32_t
flush_sort_info(prc_internal_t* iprc)
{
    size_t n;
    n = fwrite(iprc->header.sortinfoData, 1,
               (size_t)iprc->header.sortinfoSize, iprc->fp);
    if (n != (size_t)iprc->header.sortinfoSize)
        return (prc_uint32_t)-1;
    return (prc_uint32_t)n;
}

static int
flush_records(prc_internal_t* iprc, prc_uint32_t offset, FILE* fp)
{
    prc_internal_record_t* records = iprc->records;
    unsigned isResource = PRC_IS_RESOURCE(&iprc->header);
    unsigned isStream = PRC_IS_STREAM(&iprc->header);
    prc_internal_record_t* foo;

    for (foo = records; foo != NULL; foo = foo->next) {
        union {
            prc_record_header_t   record;
            prc_resource_header_t resource;
        } uBuf;
        int rv;

        if (isResource) {
            uBuf.resource.offset = offset;
            uBuf.resource.id = foo->u_contents.resource.id;
            uBuf.resource.type = foo->u_contents.resource.type;
            rv = prcfwriteresource(fp, &uBuf.resource);
        } else {
            uBuf.record.offset = offset;
            uBuf.record.flags = foo->u_contents.record.flags;
            uBuf.record.id = foo->u_contents.record.id;
            rv = prcfwriterecord(fp, &uBuf.record);
        }

        if (rv == -1)
            return -1;
            
        offset += foo->u_contents.record.datalen;
    }
    
    fputc(0, fp);
    fputc(0, fp);

    if (iprc->header.appinfoData != NULL) {
        prc_uint32_t n = flush_app_info(iprc);

        if (n == (prc_uint32_t)-1)
            return -1;
    }

    if (iprc->header.sortinfoData != NULL) {
        prc_uint32_t n = flush_sort_info(iprc);

        if (n == (prc_uint32_t)-1)
            return -1;
    }

    for (foo = records; foo != NULL; foo = foo->next) {
        prc_byte_t*  data = foo->u_contents.record.data;
        prc_uint32_t datalen = foo->u_contents.record.datalen;
        if (data != NULL) {
            size_t n;
            if (isStream && !foo->next) { /* last record, do a fixup */
                prc_uint32_t blocksize;
                blocksize = prcbuftouint32(data + PRC_STREAM_BLOCKSIZE_OFFSET);
                datalen = PRC_STREAM_HEADER_SIZE + blocksize;
            }
            n = fwrite(data, 1, (size_t)datalen, fp);
            if (n != (size_t)datalen)
                return -1;
        }
    }

    return 0;
}

PRC_PUBLIC_API(int)
prcflush(prc_t* prc)
{
    prc_internal_t* iprc = (prc_internal_t*)prc;
    prc_uint32_t offset;
    unsigned is_resource = (prc->flags & PRC_FLAGS_RESOURCE) != 0;

    PRC_CLEAR_ERRORNO();

    if ((iprc->mode & (PRC_OPEN_CREATE|PRC_OPEN_UPDATE|PRC_OPEN_WRITE|PRC_OPEN_TOUCH)) == 0) {
        return -1;
    }

    fseek(iprc->fp, 0L, SEEK_SET);

    if (prcfwriteheader(iprc->fp, prc) != 0)
        return -1;

    if (is_resource) {
        offset = sizeof(prc_file_t) +
            (sizeof(prc_file_resource_header_t) * prc->nrecords) +
            2;

    } else {
        offset = sizeof(prc_file_t) +
            (sizeof(prc_file_record_header_t) * prc->nrecords) +
            2;

        if (iprc->header.appinfoData != NULL)
            offset += iprc->header.appinfoSize;

        if (iprc->header.sortinfoData != NULL)
            offset += iprc->header.sortinfoSize;
    }

    return flush_records(iprc, offset, iprc->fp);
}
