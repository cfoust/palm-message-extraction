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
 * Tomi Kankaanp‰‰
 * Steve Austin
 * Andrew Trevarrow
 */

/*
 * prcp.h
 *
 * created: djw@djw.org, January 2, 1999.
 */
#ifndef _PRCP_H_
#define _PRCP_H_

#define PRC_PUBLIC_API_IMPLEMENTATION
#include "prcsys.h"
#include "prc.h"

/* File header */
typedef struct prc_file_t {
    prc_byte_t name[32];
    prc_byte_t flags[2];
    prc_byte_t version[2];
    prc_byte_t ctime[4];
    prc_byte_t mtime[4];
    prc_byte_t btime[4];
    prc_byte_t modnum[4];
    prc_byte_t appinfo[4];
    prc_byte_t sortinfo[4];
    prc_byte_t type[4];
    prc_byte_t cid[4];
    prc_byte_t unique_id_seed[4];
    prc_byte_t next_record_list[4];
    prc_byte_t nrecords[2];
} prc_file_t;

/* File resource header */
typedef struct prc_file_resource_header_t {
    prc_byte_t type[4];
    prc_byte_t id[2];
    prc_byte_t offset[4];
} prc_file_resource_header_t;

/* File resource header */
typedef struct prc_resource_header_t {
    prc_uint32_t type;
    prc_uint32_t offset;
    prc_uint16_t id;
} prc_resource_header_t;

/* File record header */
typedef struct prc_file_record_header_t {
    prc_byte_t offset[4];
    prc_byte_t flags;
    prc_byte_t id[3];
} prc_file_record_header_t;

/* File record header */
typedef struct prc_record_header_t {
    prc_uint32_t offset;
    prc_uint32_t id;
    prc_byte_t   flags;
} prc_record_header_t;

/* Stream header */
#define PRC_STREAM_HEADER_SIZE      8
#define PRC_STREAM_BLOCKSIZE        4096UL
#define PRC_STREAM_SIGNATURE        0x44424C4BUL /* 'DBLK' */
#define PRC_STREAM_SIGNATURE_OFFSET 0
#define PRC_STREAM_BLOCKSIZE_OFFSET 4

/* CCYYMMDDhhmm */
#define PRC_TIME_DEFAULT_FORMAT_LEN 12
#define PRC_TIME_DEFAULT_FORMAT "%Y%m%d%H%M"

#if defined(PRC_TARGET_MACINTOSH)
/* As far as I can tell these should be the same on Unix / Mac */
#define MAC_MINUS_UNIX_TIME (-126144000L)
#else
#define MAC_MINUS_UNIX_TIME (2082844800L) /* from pilot */
#endif

/* 0x7c35b080 */

#define UNIX_TO_MAC_TIME(t) ((t)+MAC_MINUS_UNIX_TIME)
#define MAC_TO_UNIX_TIME(t) ((t)-MAC_MINUS_UNIX_TIME)

typedef struct prc_internal_record_t {
    union {
        prc_record_t   record;
        prc_resource_t resource;
    } u_contents;
    struct prc_internal_record_t* next;
} prc_internal_record_t;

#define PRC_RECORDTOINTERNAL(r) (prc_internal_record_t*)(r)
#define PRC_RESOURCETOINTERNAL(r) (prc_internal_record_t*)(r)

typedef struct prc_internal_t
{
    prc_t                  header;
    prc_internal_record_t* records;
    FILE*                  fp;
    unsigned               mode;
    prc_uint32_t           posn;
} prc_internal_t;

#define PRC_PRCTOINTERNAL(p) ((prc_internal_t*)(p))

prc_t* prcfpopen(FILE* fp, unsigned mode);
int prcfwriteheader(FILE* fp, prc_t*);
int prcfreadheader(FILE* fp, prc_t*, prc_uint32_t* appi, prc_uint32_t* sorti);
int prcfwriterecord(FILE* fp, prc_record_header_t*);
int prcfreadrecord(FILE* fp, prc_record_header_t*);
int prcfwriteresource(FILE* fp, prc_resource_header_t*);
int prcfreadresource(FILE* fp, prc_resource_header_t*);
int prcinitheader(prc_t* prc);
int prccommonset(prc_internal_record_t*, prc_byte_t* data, prc_uint32_t len);

/* misc buf to int, int to buf routines */
prc_uint16_t prcbuftouint16(const void*);
prc_uint32_t prcbuftouint32(const void*);
prc_uint32_t prcbuftouint24(const void*);
void         prcuint16tobuf(void*, prc_uint16_t);
void         prcuint32tobuf(void*, prc_uint32_t);
void         prcuint24tobuf(void*, prc_uint32_t);

/* error holder */
extern prc_errno_t prc_errno;

#define PRC_SET_ERRORNO(e) prc_errno = (e);
#define PRC_CLEAR_ERRORNO() prc_errno = PRC_ERROR_NONE;

/* debug */
#define PRC_DEBUGF(x)

#endif /*_PRCP_H_*/
