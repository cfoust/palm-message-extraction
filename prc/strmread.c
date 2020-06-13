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
 * strmread.c
 *
 * created: djw@djw.org, June 16, 2001.
 */
#include "prcp.h"

PRC_PUBLIC_API(prc_uint32_t)
prcstreamread(prc_t*   prc,
              prc_byte_t*  data,
              prc_uint32_t size)
{
    prc_internal_t* iprc = PRC_PRCTOINTERNAL(prc);
    prc_internal_record_t* prev;
    prc_internal_record_t* foo;
    prc_uint32_t           record_index;
    prc_uint32_t           record_offset;
    prc_uint32_t           bytes_read;
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
        prc_uint32_t signature;

        if (!foo) { /* goto next record */
            PRC_SET_ERRORNO(PRC_ERROR_EOF);
            return 0;
        }
        /* check signature on block */
        signature = prcbuftouint32(foo->u_contents.record.data +
                                   PRC_STREAM_SIGNATURE_OFFSET);
        if (signature != PRC_STREAM_SIGNATURE) {
            PRC_SET_ERRORNO(PRC_ERROR_NOT_STREAM);
            return 0;
        }
        prev = foo;
        foo = foo->next;
    }

    /* read */
    bytes_read = 0;
    bytes_size = size;

    while (bytes_read < bytes_size) {
        prc_uint32_t block_bytes = bytes_size - bytes_read;
        prc_byte_t*  stream_data;
        prc_uint32_t block_size;

        if (!foo) { /* process next record */
            PRC_SET_ERRORNO(PRC_ERROR_EOF);
            break;
        }

        block_size = prcbuftouint32(foo->u_contents.record.data +
                                    PRC_STREAM_BLOCKSIZE_OFFSET);

        if (block_bytes > (block_size - record_offset))
            block_bytes = (block_size - record_offset);

        stream_data = foo->u_contents.record.data + PRC_STREAM_HEADER_SIZE;
        memcpy(data + bytes_read, stream_data + record_offset, block_bytes);
        bytes_read += block_bytes;
        prev = foo;
        foo = foo->next;
        record_offset = 0;
    }

    iprc->posn += bytes_read;

    return bytes_read;
}
