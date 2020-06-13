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
 * strmwrit.c
 *
 * created: djw@djw.org, June 16, 2001.
 */
#include "prcp.h"

static prc_internal_record_t*
newrecord()
{
    prc_internal_record_t* foo;
    prc_uint32_t           data_size;
    prc_byte_t*            data;
    
    foo = (prc_internal_record_t*)malloc(sizeof(prc_internal_record_t));
    if (!foo) {
        PRC_SET_ERRORNO(PRC_ERROR_ALLOC_FAILED);
        return NULL;
    }
    memset(foo, 0, sizeof(prc_internal_record_t));
    
    data_size = PRC_STREAM_HEADER_SIZE + PRC_STREAM_BLOCKSIZE;
    data = (prc_byte_t*)malloc(data_size);
    if (!data) {
        free(foo);
        PRC_SET_ERRORNO(PRC_ERROR_ALLOC_FAILED);
        return NULL;
    }
    prcuint32tobuf(data + PRC_STREAM_SIGNATURE_OFFSET, PRC_STREAM_SIGNATURE);
    prcuint32tobuf(data + PRC_STREAM_BLOCKSIZE_OFFSET, PRC_STREAM_BLOCKSIZE);

    foo->u_contents.record.data = data;
    foo->u_contents.record.datalen = data_size;

    return foo;
}

PRC_PUBLIC_API(prc_uint32_t)
prcstreamwrite(prc_t*       prc,
               prc_byte_t*  data,
               prc_uint32_t size)
{
    prc_internal_t* iprc = PRC_PRCTOINTERNAL(prc);
    prc_internal_record_t* prev;
    prc_internal_record_t* foo;
    prc_uint32_t           record_index;
    prc_uint32_t           record_offset;
    prc_uint32_t           bytes_written;
    prc_uint32_t           bytes_size;
    prc_uint32_t           index;
    
    PRC_CLEAR_ERRORNO();

    if (!(prc->flags & PRC_FLAGS_STREAM)) { /* not a stream */
        PRC_SET_ERRORNO(PRC_ERROR_NOT_STREAM);
        return 0;
    }

    record_index = iprc->posn / PRC_STREAM_BLOCKSIZE;
    record_offset = iprc->posn % PRC_STREAM_BLOCKSIZE;

    /* seek */
    prev = NULL;
    foo = iprc->records;
    for (index = 0; index < record_index; index++) {
        if (!foo) { /* make a new record */
            foo = newrecord();
            if (!foo)
                return 0;
            iprc->header.nrecords++;
            if (prev != NULL) {
                prev->next = foo;
            } else {
                iprc->records = foo;
            }
        }
        prev = foo;
        foo = foo->next;
    }

    /* write */
    bytes_written = 0;
    bytes_size = size;

    while (bytes_written < bytes_size) {
        prc_uint32_t block_bytes = bytes_size - bytes_written;
        prc_byte_t*  stream_data;

        if (block_bytes > (PRC_STREAM_BLOCKSIZE - record_offset))
            block_bytes = (PRC_STREAM_BLOCKSIZE - record_offset);

        if (!foo) { /* make a new record */
            foo = newrecord();
            if (!foo)
                break;
            iprc->header.nrecords++;
            if (prev != NULL) {
                prev->next = foo;
            } else {
                iprc->records = foo;
            }
        }

        stream_data = foo->u_contents.record.data + PRC_STREAM_HEADER_SIZE;
        memcpy(stream_data + record_offset, data + bytes_written, block_bytes);
        prcuint32tobuf(foo->u_contents.record.data +
                       PRC_STREAM_BLOCKSIZE_OFFSET,
                       (record_offset + block_bytes));

        bytes_written += block_bytes;
        prev = foo;
        foo = foo->next;
        record_offset = 0;
    }

    iprc->posn += bytes_written;

    return bytes_written;
}
