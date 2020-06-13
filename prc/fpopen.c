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
 * Tomi Kankaanpaa
 * Andrew Trevarrow
 */

/*
 * fpopen.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

static int
read_records(prc_internal_t* iprc,
             FILE* fp,
             prc_uint32_t filesize,
             prc_uint32_t* firstRecordOffset_r)
{
    prc_uint16_t           i;
    prc_internal_record_t* foo;
    prc_internal_record_t* prev = NULL;
    int n = 0;
    unsigned isResource = PRC_IS_RESOURCE(&iprc->header);

    iprc->records = NULL;

    for (i = 0; i < iprc->header.nrecords; i++) {
        union {
            prc_record_header_t   record;
            prc_resource_header_t resource;
        } uBuf;
        int rv;

        if (isResource) {
            rv = prcfreadresource(fp, &uBuf.resource);
        } else {
            rv = prcfreadrecord(fp, &uBuf.record);
        }

        if (rv == -1)
            return rv;

#if 0
        PRC_DEBUGF(("%02d: %02d %5d\n",
                    i, uBuf.record.id, uBuf.record.offset));
#endif

        foo = (prc_internal_record_t*)prcnewrecord();
        if (foo == NULL)
            return -1;

        if (isResource) {
            foo->u_contents.record.data = (prc_byte_t*)uBuf.resource.offset;
            foo->u_contents.resource.id = uBuf.resource.id;
            foo->u_contents.resource.type = uBuf.resource.type;
        } else {
            foo->u_contents.record.data = (prc_byte_t*)uBuf.record.offset;
            foo->u_contents.record.id = uBuf.record.id;
            foo->u_contents.record.flags = uBuf.record.flags;
        }
        foo->next = NULL;
        
        if (prev != NULL) {
            prev->next = foo;
        } else {
            iprc->records = foo;

            if (!isResource)
                *firstRecordOffset_r = uBuf.record.offset;
        }
        
        prev = foo;
    }
    
    /* seek to read data */
    for (foo = iprc->records; foo != NULL; foo = foo->next) {
        void*        data;
        size_t       nread;
        long         posn;
        long         next_posn;
        size_t       size;

        posn = (long)foo->u_contents.record.data;
        if (foo->next != NULL)
            next_posn = (long)foo->next->u_contents.record.data;
        else
            next_posn = filesize;

        size = next_posn - posn;

        if (size >= (64 * 1024)) {
            PRC_SET_ERRORNO(PRC_ERROR_RECORD_TOOBIG);
            PRC_DEBUGF((">= 64k length record (%d)", n));
            return -1;
        } else if (size == 0) {
            /* this is not really an error I guess */
            PRC_SET_ERRORNO(PRC_ERROR_RECORD_ZEROLENGTH);
            PRC_DEBUGF(("zero length record (%d)", n));
            data = NULL;
        } else {
            data = (void*)malloc(size);
            
            if (data == NULL)
                return -1;
            
            if (fseek(fp, posn, SEEK_SET) == -1) {
                PRC_SET_ERRORNO(PRC_ERROR_SEEK_FAILED);
                PRC_DEBUGF(("seek failed for record %d", n));
                return -1;
            }
            nread = fread(data, 1, size, fp);
            
            if (size != nread) {
                PRC_SET_ERRORNO(PRC_ERROR_READ_FAILED);
                PRC_DEBUGF(("expected %ld, read %ld",
                            (prc_uint32_t)size, (prc_uint32_t)nread));
                return -1;
            }
        }
            
        foo->u_contents.record.data = data;
        foo->u_contents.record.datalen = size;

        n++;
    }
    return n;
}

static void*
read_info(prc_internal_t* iprc, FILE* fp, const char* name,
          prc_uint32_t offset, prc_uint32_t size)
{
    size_t nread;
    void* data = (void*)malloc(size);

    if (!data) {
        PRC_SET_ERRORNO(PRC_ERROR_ALLOC_FAILED);
        PRC_DEBUGF(("memory allocation failed for %s", name));
        return NULL;
    }

    if (fseek(fp, (size_t)offset, SEEK_SET) == -1) {
        PRC_SET_ERRORNO(PRC_ERROR_SEEK_FAILED);
        PRC_DEBUGF(("seek failed for %s", name));
        free(data);
        return NULL;
    }

    nread = fread(data, 1, (size_t)size, fp);
            
    if (size != (prc_uint32_t)nread) {
        PRC_SET_ERRORNO(PRC_ERROR_READ_FAILED);
        PRC_DEBUGF(("fread failed for %s expected %ld, read %ld",
                   name, size, (prc_uint32_t)nread));
        free(data);
        return NULL;
    }

    return data;
}

static int
read_app_info(prc_internal_t* iprc, FILE* fp,
              prc_uint32_t offset, prc_uint32_t size)
{
    void* data;

    data = read_info(iprc, fp, "appInfo", offset, size);
    if (data == NULL)
        return -1;

    iprc->header.appinfoData = data;
    iprc->header.appinfoSize = size;

    return 0;
}

static int
read_sort_info(prc_internal_t* iprc, FILE* fp,
              prc_uint32_t offset, prc_uint32_t size)
{
    void* data;

    data = read_info(iprc, fp, "sortInfo", offset, size);
    if (data == NULL)
        return -1;

    iprc->header.sortinfoData = data;
    iprc->header.sortinfoSize = size;

    return 0;
}

PRC_PUBLIC_API(prc_t*)
prcfpopen(FILE* fp, unsigned mode)
{
    prc_t      buf;
    prc_internal_t* iprc = NULL;
    prc_uint32_t appInfoOffset = 0;
    prc_uint32_t sortInfoOffset = 0;

    PRC_CLEAR_ERRORNO();

    if ((mode & PRC_OPEN_CREATE) != 0) {
        prcinitheader(&buf);
    } else {
        fseek(fp, 0, SEEK_SET);
        
        if (prcfreadheader(fp, &buf, &appInfoOffset, &sortInfoOffset) != 0)
            return NULL;
    }
    
    iprc = (prc_internal_t*)malloc(sizeof(prc_internal_t));

    if (iprc == NULL)
        return NULL;

    memcpy(&iprc->header, &buf, sizeof(prc_t));
    iprc->records = NULL;
    iprc->fp = fp;
    iprc->mode = mode;
    iprc->posn = 0;

    if ((mode & PRC_OPEN_CREATE) == 0) {
        struct stat statbuf;
        int    rv;
        prc_uint32_t firstRecordOffset = 0;

        if (fstat(fileno(fp), &statbuf) == -1) {
            PRC_SET_ERRORNO(PRC_ERROR_STAT_FAILED);
            PRC_DEBUGF(("could not stat!"));
            return NULL;
        }

        rv = read_records(iprc, fp, statbuf.st_size, &firstRecordOffset);

        if (!PRC_IS_RESOURCE(&iprc->header)) {
            if (appInfoOffset != 0) {
                prc_uint32_t appInfoSize;

                if (sortInfoOffset != 0)
                    appInfoSize = sortInfoOffset - appInfoOffset;
                else if (firstRecordOffset != 0)
                    appInfoSize = firstRecordOffset - appInfoOffset;
                else
                    appInfoSize = statbuf.st_size - appInfoOffset;

                if (read_app_info(iprc, fp, appInfoOffset, appInfoSize) == -1)
                    return NULL;
            }

            if (sortInfoOffset != 0) {
                prc_uint32_t sortInfoSize;

                if (firstRecordOffset != 0)
                    sortInfoSize = firstRecordOffset - sortInfoOffset;
                else
                    sortInfoSize = statbuf.st_size - sortInfoOffset;

                if (read_sort_info(iprc, fp,
                                   sortInfoOffset, sortInfoSize) == -1)
                    return NULL;
            }
        }

        if (rv == -1)
            return NULL;
    }

    return &iprc->header;
}
