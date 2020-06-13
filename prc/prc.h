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
 * Steve Austin
 * Andrew Trevarrow
 */

/*
 * prc.h
 *
 * Defines the libprc library interface.
 *
 * created: djw@djw.org, January 2, 1999.
 *
 *------------------------------------
 *
 * Prc Format.
 * ==========
 * PrcHeader 78 bytes
 * ----------
 * SectionHeader0 10 bytes
 *  char name[4];
 *  short id;
 *  short offseth;
 *  short offsetl;
 * ----------
 * SectionHeader1 10 bytes
 * ----------
 * SectionHeaderN 10 bytes
 * ----------
 * SectionHeaderPadding 2 bytes
 * ----------
 * Data0
 * ----------
 * Data1
 * ----------
 * DataN
 * ----------
 *
 * Pdb Format.
 * ==========
 * PrcHeader 78 bytes
 * ----------
 * SectionHeader0 8 bytes
 *  short offseth;
 *  short offsetl;
 *  long  flags;
 * ----------
 * SectionHeader1 8 bytes
 * ----------
 * SectionHeaderN 8 bytes
 * ----------
 * SectionHeaderPadding 2 bytes
 * ----------
 * [AppInfo]
 * ----------
 * [SortInfo]
 * ----------
 * Data0
 * ----------
 * Data1
 * ----------
 * DataN
 * ----------
 */
#ifndef _PRC_H_
#define _PRC_H_

#include <stdio.h>

#define LIBPRC_VERSION       0x00000501L /* 00MMmmuu = MM.mm[.uu] */

/*
 * Version history:
 * 00000501 Fixes a bad Windows .exe (no setargv.obj in project) in 500.
 * 00000500 32 bit APIs. Bug fixes to stream support.
 * 00000400 MPL copyright. PalmOS4.0 header flags. Mac port integration.
 *          Win32 DLL integration. Cleaned up some error messages in
 *          par input parsing. Man page cleanups and examples.
 *          Added rudimentary FileStream support. Fixed long filename bug.
 * 00000300 Bug fixes: delete multiple records, leaks in prcclose(),
 *          correct truncation in delete mode, record set bugs.
 *          New: support for new 3.X database attributes, db append.
 *          Win32 wildcard support added. Killed strptime()/strftime()
 * 00000200 Delete, app, sort info support, par touch options, Win32.
 * 00000100 First versioned version.
 */
#define LIBPRC_VERSION_MAJOR ((LIBPRC_VERSION>>16)&0xff)
#define LIBPRC_VERSION_MINOR ((LIBPRC_VERSION>>8)&0xff)
#define LIBPRC_VERSION_MICRO ((LIBPRC_VERSION)&0xff)

#if defined(_WIN32) && !defined(PRC_WIN32_STATIC_BUILD)
  #ifdef  PRC_PUBLIC_API_IMPLEMENTATION
  #define PRC_PUBLIC_API(_t) __declspec(dllexport) _t
  #else
  #define PRC_PUBLIC_API(_t) __declspec(dllimport) _t
  #endif
#else /* sane */
  #define PRC_PUBLIC_API(_t) _t
#endif

/* types, these should be ported to any new host */
typedef unsigned char  prc_byte_t;
typedef short          prc_int16_t;
typedef unsigned short prc_uint16_t;
typedef long           prc_int32_t;
typedef unsigned long  prc_uint32_t;
typedef unsigned long  prc_time_t;
typedef unsigned long  prc_type_t;
typedef prc_uint16_t   prc_attr_t;

#define PRC_MAX_NAMESIZE 32

/* File header */
typedef struct prc_t {
    prc_byte_t   name[PRC_MAX_NAMESIZE];
    prc_attr_t   flags;
    prc_uint16_t version;
    prc_time_t   ctime;
    prc_time_t   mtime;
    prc_time_t   btime;
    prc_uint32_t modnum;
    prc_uint32_t type;
    prc_uint32_t cid;
    prc_uint16_t nrecords;
    void*        appinfoData;
    prc_uint32_t appinfoSize;
    void*        sortinfoData;
    prc_uint32_t sortinfoSize;
} prc_t;

/* File attributes */
#define PRC_FLAGS_RESOURCE    (0x1<<0)
#define PRC_FLAGS_READONLY    (0x1<<1)
#define PRC_FLAGS_DIRTY       (0x1<<2)
#define PRC_FLAGS_BACKUP      (0x1<<3)
#define PRC_FLAGS_NEWER       (0x1<<4)  /* PalmOS2 */
#define PRC_FLAGS_RESET       (0x1<<5)  /* PalmOS2 */
#define PRC_FLAGS_COPYPREVENT (0x1<<6)  /* PalmOS3 */
#define PRC_FLAGS_STREAM      (0x1<<7)  /* PalmOS3 */
#define PRC_FLAGS_HIDDEN      (0x1<<8)  /* PalmOS3.2 */
#define PRC_FLAGS_LAUNCHABLE  (0x1<<9)  /* PalmOS3.2 */
#define PRC_FLAGS_RECYCLABLE  (0x1<<10) /* PalmOS4.0 */
#define PRC_FLAGS_BUNDLE      (0x1<<11) /* PalmOS4.0 */
#define PRC_FLAGS_OPEN        (0x1<<15)

#define PRC_IS_RESOURCE(p) (((p)->flags & PRC_FLAGS_RESOURCE) != 0)
#define PRC_IS_STREAM(p)   (((p)->flags & PRC_FLAGS_STREAM) != 0)

/* Record Attributes */
#define	PRC_REC_FLAGS_DELETE 0x80
#define	PRC_REC_FLAGS_DIRTY  0x40
#define	PRC_REC_FLAGS_BUSY   0x20
#define	PRC_REC_FLAGS_SECRET 0x10
#define PRC_REC_FLAGS_GET_CATEGORY(x) ((x) & 0xf)
#define PRC_REC_FLAGS_MAKE(f,c) (((c) & 0xf)|((f) & 0xf0))

/* Record header */
typedef struct prc_record_t {
    prc_byte_t*  data;
    prc_uint32_t datalen; /* needs to be 32 bit even though device chokes */
    prc_uint32_t id;
    prc_byte_t   flags;
} prc_record_t;

/* Resource header */
typedef struct prc_resource_t {
    prc_byte_t*  data;
    prc_uint32_t datalen;
    prc_uint32_t type;
    prc_uint16_t id;
} prc_resource_t;

/* memMaxChunkAllocSize in Palm headers */
#define	PRC_MAX_CHUNK_SIZE 0xFFE1UL

/* Errno */
typedef enum
{
    PRC_ERROR_NONE,
    PRC_ERROR_RECORD_ZEROLENGTH,
    PRC_ERROR_RECORD_TOOBIG,
    PRC_ERROR_SEEK_FAILED,
    PRC_ERROR_READ_FAILED,
    PRC_ERROR_ALLOC_FAILED,
    PRC_ERROR_STAT_FAILED,
    PRC_ERROR_OPEN_FAILED,
    PRC_ERROR_BADARG,
    PRC_ERROR_NOT_STREAM,
    PRC_ERROR_EOF,

    PRC_ERROR_NERRORS
} prc_errno_t;

#define PRC_OPEN_READ   (0)
#define PRC_OPEN_WRITE  (0x1<<0)
#define PRC_OPEN_CREATE (0x1<<1)
#define PRC_OPEN_TOUCH  (0x1<<2) /* will not flush records */
#define PRC_OPEN_UPDATE (0x1<<3)

/* database manipulation */
PRC_PUBLIC_API(prc_t*)          prccreate(const char* fname,
                                          const char* name,
                                          prc_type_t  type,
                                          prc_type_t  cid,
                                          prc_attr_t  attrs);
/* convenience */
#define prccreateprc(f,n,t,c) prccreate((f),(n),(t),(c),PRC_FLAGS_RESOURCE);
#define prccreatepdb(f,n,t,c) prccreate((f),(n),(t),(c),0);
#define prccreatestream(f,n,t,c) prccreate((f),(n),(t),(c),PRC_FLAGS_STREAM);

PRC_PUBLIC_API(prc_t*)          prcopen(const char* name, unsigned mode);
PRC_PUBLIC_API(prc_t*)          prcfpopen(FILE* fp, unsigned mode);
PRC_PUBLIC_API(int)             prcclose(prc_t*);
PRC_PUBLIC_API(int)             prcflush(prc_t*);
PRC_PUBLIC_API(int)             prcdestroy(prc_t* prc);
PRC_PUBLIC_API(int)             prcsetappinfo(prc_t*,
                                              prc_byte_t*  data,
                                              prc_uint32_t datalen);
PRC_PUBLIC_API(int)             prcsetsortinfo(prc_t*,
                                               prc_byte_t*  data,
                                               prc_uint32_t datalen);

/* record interface */
PRC_PUBLIC_API(unsigned)        prcgetnrecords(prc_t*);
PRC_PUBLIC_API(prc_record_t*)   prcgetrecord(prc_t*, prc_uint16_t index);
PRC_PUBLIC_API(prc_record_t*)   prcnewrecord();
PRC_PUBLIC_API(int)             prcdestroyrecord(prc_record_t* rec);
PRC_PUBLIC_API(prc_record_t*)   prcremoverecord(prc_t* prc, prc_record_t* rec);
PRC_PUBLIC_API(int)             prcrecordset(prc_record_t*,
                                             prc_byte_t*  data,
                                             prc_uint32_t datalen);
PRC_PUBLIC_API(prc_record_t*)   prcappendrecord(prc_t*, prc_record_t*);
PRC_PUBLIC_API(prc_record_t*)   prcgetnextrecord(prc_t* prc,
                                                 prc_record_t* last);
typedef int (*prc_record_mappee_t)(prc_record_t* record, void* arg);
PRC_PUBLIC_API(int)             prcmaprecords(prc_t* prc,
                                              prc_record_mappee_t,
                                              void* arg);

/* resource interface */
PRC_PUBLIC_API(prc_resource_t*) prcgetresource(prc_t*,
                                               prc_uint32_t type,
                                               prc_uint16_t id);
PRC_PUBLIC_API(prc_resource_t*) prcnewresource();
PRC_PUBLIC_API(int)             prcdestroyresource(prc_resource_t* resource);
PRC_PUBLIC_API(prc_resource_t*) prcremoveresource(prc_t* prc, prc_resource_t*);
PRC_PUBLIC_API(int)             prcresourceset(prc_resource_t*,
                                               prc_uint32_t type,
                                               prc_uint16_t id,
                                               prc_byte_t*  data,
                                               prc_uint32_t datalen);
PRC_PUBLIC_API(prc_resource_t*) prcappendresource(prc_t*, prc_resource_t*);
PRC_PUBLIC_API(prc_resource_t*) prcgetnextresource(prc_t* prc,
                                                   prc_resource_t* last);
typedef int (*prc_resource_mappee_t)(prc_resource_t* resource, void* arg);
PRC_PUBLIC_API(int)             prcmapresources(prc_t* prc,
                                                prc_resource_mappee_t,
                                                void* arg);

/* stream interface */
PRC_PUBLIC_API(prc_uint32_t)    prcstreamwrite(prc_t*,
                                               prc_byte_t*  data,
                                               prc_uint32_t datalen);
PRC_PUBLIC_API(prc_uint32_t)    prcstreamread(prc_t*,
                                              prc_byte_t*  data,
                                              prc_uint32_t datalen);

/* type/cid util */
PRC_PUBLIC_API(char*)           prctypetostr(char* buf, prc_type_t type);
PRC_PUBLIC_API(prc_type_t)      prcstrtotype(const char* buf);

/* time util */
PRC_PUBLIC_API(prc_time_t)      prctime(prc_time_t* buf); /* now */
PRC_PUBLIC_API(char*)           prctimetostr(char* buf, prc_time_t prc_time);
PRC_PUBLIC_API(prc_time_t)      prcstrtotime(const char*);

/* file attribute util */
PRC_PUBLIC_API(char*)           prcstrfattr(char* buf, prc_attr_t flags);
PRC_PUBLIC_API(prc_attr_t)      prcstrpattr(const char* buf);
PRC_PUBLIC_API(prc_attr_t)      prcstrprecordattr(const char* s);

/* misc useful utils */
PRC_PUBLIC_API(void)            prcsetname(prc_t* prc, const char* name);

/* error */
PRC_PUBLIC_API(prc_errno_t)     prcgeterrno();
PRC_PUBLIC_API(const char*)     prcstrerror(prc_errno_t e);

#endif /*_PRC_H_*/
