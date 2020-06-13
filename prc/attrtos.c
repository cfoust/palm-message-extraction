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
 * attrtos.c
 *
 * created: djw@djw.org, January 2, 1999.
 */
#include "prcp.h"

typedef struct {
	char*        string;
	unsigned int bits;
} attrs_data_t;

static const attrs_data_t header_attrs_data[] = {
	{ "resource",    PRC_FLAGS_RESOURCE    },
	{ "readonly",    PRC_FLAGS_READONLY    },
	{ "dirty",       PRC_FLAGS_DIRTY       },
	{ "backup",      PRC_FLAGS_BACKUP      },
	{ "newer",       PRC_FLAGS_NEWER       }, /* PalmOS2 */
	{ "reset",       PRC_FLAGS_RESET       }, /* PalmOS2 */

	{ "copyprevent", PRC_FLAGS_COPYPREVENT }, /* PalmOS3 */
	{ "stream",      PRC_FLAGS_STREAM      }, /* PalmOS3 */
	{ "hidden",      PRC_FLAGS_HIDDEN      }, /* PalmOS3 */
	{ "launchable",  PRC_FLAGS_LAUNCHABLE  }, /* PalmOS3.2 */
	{ "recyclable",  PRC_FLAGS_RECYCLABLE  }, /* PalmOS4.0 */
	{ "bundle",      PRC_FLAGS_BUNDLE      }, /* PalmOS4.0 */

	{ "open",        PRC_FLAGS_OPEN        }, /* probably a bad idea */
	{ 0 }
};

static char static_buf[11*16];

static char*
strattrs(const attrs_data_t* attrs_data, char* buf, prc_attr_t attrs)
{
    int i;
    unsigned first = 1;
    char* p;

    if (buf == NULL)
        buf = static_buf;

    buf[0] = '\0';

    for (p = buf, i = 0; attrs_data[i].string != NULL; i++) {
        if ((attrs & attrs_data[i].bits) != 0) {
            char* q;
            if (!first)
                *p++ = '|';
            for (q = attrs_data[i].string; *q;) {
                *p++ = *q++;
            }
            first = 0;
        }
    }

    *p = '\0';

    return buf;
}

PRC_PUBLIC_API(char*)
prcstrfattr(char* buf, prc_attr_t attrs)
{
    PRC_CLEAR_ERRORNO();

    return strattrs(header_attrs_data, buf, attrs);
}

static unsigned int
parse_attrs_alpha(const attrs_data_t* attrs_data, char* s, char** q)
{
    int i;
    char* p;
    int   slen = 0;

    for (p = s; isalpha((unsigned)*p); p++)
        slen++;
	
    for (i = 0; attrs_data[i].string != NULL; i++) {
        int dlen = strlen(attrs_data[i].string);
        if (slen == dlen
            &&
            PRC_STRNCASECMP(attrs_data[i].string, s, dlen) == 0) {
            if (q != NULL)
                *q = s + slen;
            return attrs_data[i].bits;
        }
    }
	
    return -1;
}

static unsigned int
parse_number(char* s, char** q)
{
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        return (unsigned int)strtol(&s[2], q, 16);
    } else if (s[0] == '0') {
        return (unsigned int)strtol(&s[1], q, 8);
    } else {
        return (unsigned int)strtol(s, q, 10);
    }
}

static unsigned int
parse_attrs(const attrs_data_t* attrs_data, char* s)
{
    unsigned int n_result = 0;
    char* p;
    for (p = s; *p != '\0';) {
        if (*p == '|') { /* allow, but skip */
            p++;
        } else if (isalpha((unsigned)*p)) { /* symbolic defn */
            int foo = parse_attrs_alpha(attrs_data, p, &p);
            if (foo == -1)
                return -1;
            n_result |= foo;
        } else if (isdigit((unsigned)*p)) {
            n_result |= parse_number(p, &p);
        } else {
            return -1;
        }
    }

    return n_result;
}

PRC_PUBLIC_API(prc_attr_t)
prcstrpattr(const char* s)
{
    return (prc_attr_t)parse_attrs(header_attrs_data, (char*)s);
}

static const attrs_data_t record_attrs_data[] = {
	{ "delete",    PRC_REC_FLAGS_DELETE    },
	{ "dirty",     PRC_REC_FLAGS_DIRTY     },
	{ "busy",      PRC_REC_FLAGS_BUSY      },
	{ "secret",    PRC_REC_FLAGS_SECRET    },
	{ 0 }
};

PRC_PUBLIC_API(prc_attr_t)
prcstrprecordattr(const char* s)
{
    prc_attr_t attrs;
    attrs = parse_attrs(record_attrs_data, (char*)s);
    return (attrs & 0xFF); /* force to one byte */
}

