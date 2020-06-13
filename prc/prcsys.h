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
 * prcsys.h
 *
 * created: djw@djw.org, June 5, 2001.
 */
#ifndef _PRCSYS_H_
#define _PRCSYS_H_

/* Targets */
#if defined(macintosh)
#define PRC_TARGET_MACINTOSH
#elif defined(_WIN32)
#define PRC_TARGET_WIN32
#else
#define PRC_TARGET_GENERIC
#endif

/* Common system includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>

/* Target specific platform includes */
#if defined(PRC_TARGET_MACINTOSH)

#include <unix.h>

#else

#include <sys/types.h>
#include <sys/stat.h>

#endif

/* Target specific features */
#if defined(PRC_TARGET_MACINTOSH)

extern long _fcreator;
extern long _ftype;

#define PRC_PRC_CREATOR 'Gld1'
#define PRC_PRC_TYPE    'Gld0'

#define PRC_FILE_CREATOR_TYPE_PUSH() \
long prc_fcreator_save = _fcreator; \
long prc_ftype_save = _ftype; \

#define PRC_FILE_CREATOR_TYPE_SET(c,t) \
do { _fcreator = (c); _ftype = (t); } while (0)

#define PRC_FILE_CREATOR_TYPE_POP() \
PRC_FILE_CREATOR_TYPE_SET(prc_fcreator_save, prc_ftype_save);

#else

#define PRC_FILE_CREATOR_TYPE_PUSH()
#define PRC_FILE_CREATOR_TYPE_SET(c,t)
#define PRC_FILE_CREATOR_TYPE_POP()

#endif

/*
 * Compiler & platform specific hacks.
 */
#if defined(PRC_TARGET_WIN32)

#define PRC_STRNCASECMP(a,b,n) _strnicmp((a), (b), (n))

#elif defined(PRC_TARGET_MACINTOSH)

/* prc_strncasecmp defined in misc.c */
#define PRC_FEATURE_STRNCASECMP yes
extern int prc_strncasecmp(const char *str1, const char *str2, size_t n);
#define PRC_STRNCASECMP(a,b,n) prc_strncasecmp((a), (b), (n))

#else

#define PRC_STRNCASECMP(a,b,n) strncasecmp((a), (b), (n))

#endif

#endif /*_PRCSYS_H_*/

