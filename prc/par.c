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
 * Hallvard Trætteberg
 * Andrew Trevarrow
 */

/*
 * par(1) see usage.
 *
 * created: djw@djw.org, January 2, 1999.
 */

/*
 * Strictly speaking I guess prcsys.h is internal to the libprc library,
 * so it shouldn't be included by a user app (par). But it's so convenient
 * to deal with the (mac) porting issue that crop up here.
 */
#include "prcsys.h"
/*
 * prc.h is the public interface to libprc and should be all and user
 * program needs to use the library.
 */
#include "prc.h"

static char*
prcstrtime(char* buf, prc_time_t ptime)
{
    return prctimetostr(buf, ptime);
}

static int
list_header(prc_t* prc, FILE* fp)
{
    char buf[256];

    fprintf(fp,
            "name:       %s\n",
            prc->name);

    fprintf(fp,
            "type:       %s\n",
            prctypetostr(buf, prc->type));

    fprintf(fp,
            "cid:        %s\n",
            prctypetostr(buf, prc->cid));

    prcstrfattr(buf, prc->flags);
    if (buf[0] == '\0') /* no flags */
        strcpy(buf, "0x0000");
    fprintf(fp,
            "attributes: %s\n"
            "version:    %d\n",
            buf,
            prc->version);

    fprintf(fp,
            "ctime:      %s\n",
            prcstrtime(buf, prc->ctime));
    
    fprintf(fp,
            "mtime:      %s\n",
            prcstrtime(buf, prc->mtime));
    
    fprintf(fp,
            "btime:      %s\n",
            prcstrtime(buf, prc->btime));
    
    fprintf(fp,
            "modnum:     %ld\n"
            "szappinfo:  %ld\n"
            "szsortinfo: %ld\n"
            "nrecords:   %d\n"
            "",
            prc->modnum,
            prc->appinfoSize,
            prc->sortinfoSize,
            prc->nrecords);
    
    return 0;
}

static int
prcinfo(prc_t* prc)
{
    list_header(prc, stdout);

    return 0;
}

static char*
prcstrrexflags(char* buf, prc_byte_t flags)
{
    if ((flags & PRC_REC_FLAGS_DELETE) != 0)
        buf[0] = 'D';
    else
        buf[0] = '-';

    if ((flags & PRC_REC_FLAGS_DIRTY) != 0)
        buf[1] = 'd';
    else
        buf[1] = '-';

    if ((flags & PRC_REC_FLAGS_BUSY) != 0)
        buf[2] = 'b';
    else
        buf[2] = '-';

    if ((flags & PRC_REC_FLAGS_SECRET) != 0)
        buf[3] = 's';
    else
        buf[3] = '-';

    buf[4] = '\0';

    return buf;
}

static void
data_dump(FILE* fp, void* datav, unsigned datalen, unsigned maxlen)
{
    unsigned i;
    unsigned char* data = (unsigned char*)datav;

    if (datalen < maxlen)
        maxlen = datalen;

    for (i = 0; i < maxlen; i++) {
        int c = data[i];
        
        if (isprint(c)) {
            fputc(c, fp);
        } else {
            fputc('.', fp);
        }
    }
}

static int
list_record_mappee(prc_record_t* record, void* arg)
{
    FILE* fp = (FILE*)arg;
    char buf[256];

    prcstrrexflags(buf, record->flags);
    
    fprintf(fp, "%s %2d %4ld ",
            buf,
            PRC_REC_FLAGS_GET_CATEGORY(record->flags),
            record->datalen);
    
    data_dump(fp, record->data, record->datalen, 48);
    
    fputc('\n', fp);

    return 0; /* keep going */
}

static int
list_resource_mappee(prc_resource_t* resource, void* arg)
{
    FILE* fp = (FILE*)arg;
    char buf[256];

    fprintf(fp, "%s %04x %4ld ",
            prctypetostr(buf, resource->type),
            resource->id,
            resource->datalen);

    data_dump(fp, resource->data, resource->datalen, 48);

    fputc('\n', fp);

    return 0; /* keep going */
}

static int
prclist(prc_t* prc)
{
    if (PRC_IS_RESOURCE(prc))
        return prcmapresources(prc, list_resource_mappee, stdout);
    else
        return prcmaprecords(prc, list_record_mappee, stdout);
}

static int
data_to_file(void* data, prc_uint32_t datalen, const char* filename)
{
    FILE* fp;
    int   rv = 0;

    fp = fopen(filename, "wb");

    if (!fp) {
        fprintf(stderr, "could not open %s\n", filename);
        return -1;
    }

    if (fwrite(data, 1, (size_t)datalen, fp) != (size_t)datalen) {
        fprintf(stderr, "write to %s failed\n", filename);
        rv = -1;
    }

    fclose(fp);

    return rv;
}

static prc_byte_t*
file_to_data(const char* filename, prc_uint32_t* datalen_a)
{
    FILE* fp;
    struct stat statbuf;
    prc_byte_t* data;
	prc_uint32_t datalen;
 
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "cannot open %s\n", filename);
        return NULL;
    }

    if (fstat(fileno(fp), &statbuf) == -1) {
        fprintf(stderr, "could not stat!\n");
        fclose(fp);
        return NULL;
    }

	datalen = (prc_uint32_t)statbuf.st_size;

    data = (prc_byte_t*)malloc((size_t)datalen);

    if (fread(data, 1, (size_t)datalen, fp) == -1) {
        fprintf(stderr, "could not stat!\n");
        free(data);
        data = NULL;
    }

    fclose(fp);
    *datalen_a = datalen;

    return data;
}

static int
extract_one_record(prc_record_t* record, unsigned index, const char* filename)
{
    char buf[32];

    if (!filename) {
        sprintf(buf, "%03d.%x.%ld.pdr", index, record->flags, record->id);
        filename = buf;
    }

    return data_to_file(record->data, record->datalen, filename);
}

static int
extract_records(prc_t* prc, char** argv)
{
    int i;
    prc_uint16_t index;
    prc_record_t* record;

    for (i = 0; argv[i] != NULL; i++) {
        char* a = argv[i];
        char* f;
        
        f = NULL;
        if (strcmp(a, "-f") == 0) {
            f = argv[++i];
            a = argv[++i];
        }

        index = atoi(a);

        record = prcgetrecord(prc, index);

        if (!record) {
            fprintf(stderr, "index %d not found\n", index);
            return -1;
        }

        if (extract_one_record(record, index, f) == -1)
            return -1;
    }

    if (i == 0) { /* all */
        record = NULL;

        index = 0;
        while ((record = prcgetnextrecord(prc, record)) != NULL) {
            extract_one_record(record, index, NULL);
            index++;
        }
    }

    return 0;
}

static prc_uint16_t
prcstrtoid(const char* id_s)
{
    prc_uint16_t id;

    if (id_s[0] == '0' && (id_s[1] == 'x' || id_s[1] == 'X')) {
        id_s += 2;
        id = (prc_uint16_t)strtol(id_s, NULL, 16);
    } else {
        id = (prc_uint16_t)strtol(id_s, NULL, 10);
    }

    return id;
}

static int
extract_one_resource(prc_resource_t* resource, char* filename)
{
    char buf[16];

    if (!filename) {
        sprintf(buf, "%s%04x.bin",
                prctypetostr(buf, resource->type),
                resource->id);
        filename = buf;
    }

    return data_to_file(resource->data, resource->datalen, filename);
}

static int
extract_resources(prc_t* prc, char** argv)
{
    int i;
    prc_resource_t* resource;

    /* [-f filename] type id */
    for (i = 0; argv[i] != NULL;) {
        char* f;
        prc_uint16_t  id;
        prc_type_t type;
        char* type_s;
        char* id_s;

        type_s = argv[i];

        f = NULL;
        if (strcmp(type_s, "-f") == 0) {
            i++;
            f = argv[i++];
            type_s = argv[i];
        }
        
        i++;
        id_s = argv[i++];
        
        type = prcstrtotype(type_s);
        id = prcstrtoid(id_s);

        resource = prcgetresource(prc, type, id);
        if (!resource) {
            fprintf(stderr, "resource, type='%s', id=0x%x not found\n",
                    type_s, id);
            return -1;
        }
        
        if (extract_one_resource(resource, f) == -1)
            return -1;
    }
    
    if (i == 0) { /* all */
        resource = NULL;
        
        while ((resource = prcgetnextresource(prc, resource)) != NULL) {
            if (extract_one_resource(resource, NULL) == -1)
                return -1;
        }
    }

    return 0;
}

static unsigned
parse_info_args(char** argv,
                char** appname, unsigned* doAppinfo,
                char** sortname, unsigned* doSortinfo)
{
    unsigned nargs = 0;
    char*    arg;

    if (appname != NULL)
        *appname = NULL;
    if (sortname != NULL)
        *sortname = NULL;
    if (doAppinfo != NULL)
        *doAppinfo = 0;
    if (doSortinfo != NULL)
        *doSortinfo = 0;

    while ((arg = argv[nargs]) != NULL) {
        if (strcmp(arg, "-A") == 0) {
            if (doAppinfo != NULL)
                *doAppinfo = 1;
            nargs++;
            if (appname != NULL)
                *appname = argv[nargs++];
        } else if (strcmp(arg, "-S") == 0) {
            if (doSortinfo != NULL)
                *doSortinfo = 1;
            nargs++;
            if (sortname != NULL)
                *sortname = argv[nargs++];
        } else {
            break;
        }
    }

    return nargs;
}

static int
extract_info(prc_t* prc, char** argv)
{
    int n = 0;
    char* appInfo = NULL;
    char* sortInfo = NULL;

    n += parse_info_args(argv, &appInfo, 0, &sortInfo, 0);

    if (appInfo != NULL) { /* dump app info to file */
        data_to_file(prc->appinfoData, prc->appinfoSize, appInfo);
    }

    if (sortInfo != NULL) { /* dump sort info to file */
        data_to_file(prc->sortinfoData, prc->sortinfoSize, appInfo);
    }

    return n;
}

static int
extract_stream(prc_t* prc, char** argv)
{
    char* filename = argv[0];
    FILE* fp;
    int   rv = 0;

    if (!filename) {
        fprintf(stderr, "no stream output file specified\n");
        return -1;
    }

    fp = fopen(filename, "wb");

    if (!fp) {
        fprintf(stderr, "could not open %s\n", filename);
        return -1;
    }

    for (;;) {
        prc_byte_t   buf[4096];
        prc_uint32_t nread;

        nread = prcstreamread(prc, buf, sizeof(buf));

        if (fwrite(buf, 1, (size_t)nread, fp) != (size_t)nread) {
            fprintf(stderr, "write to %s failed\n", filename);
            rv = -1;
        }

        if (nread != sizeof(buf))
            break;
    }

    fclose(fp);

    return rv;
}

static int
prcextract(prc_t* prc, char** argv)
{
    if (PRC_IS_RESOURCE(prc)) {
        return extract_resources(prc, argv);
    } else if (PRC_IS_STREAM(prc)) {
        return extract_stream(prc, argv);
    } else {
        int n = extract_info(prc, argv);

        if (n == -1)
            return n;

        return extract_records(prc, &argv[n]);
    }
}

static int
delete_records(prc_t* prc, char** argv)
{
    int i;
    prc_uint16_t index;
    prc_record_t* record;
    unsigned      nRecords = prcgetnrecords(prc);
    prc_byte_t*   record_deletes;
    int           rv = 0;

    record_deletes = (prc_byte_t*)calloc(nRecords, sizeof(prc_byte_t));

    for (i = 0; argv[i] != NULL; i++) {
        char* a = argv[i];
        
        index = atoi(a);

        record_deletes[index] = 1;
    }

    index = nRecords;
    do {
        index--;

        if (!record_deletes[index])
            continue;
        
        record = prcgetrecord(prc, index);

        if (!record) {
            fprintf(stderr, "index %d not found\n", index);
            rv = -1;
            break;
        }

        /* remove the record from the prc, the destroy */
        record = prcremoverecord(prc, record);
        if (!record) {
            fprintf(stderr, "problem deleting record, inbdex=%d\n", index);
            rv = -1;
            break;
        } else {
            prcdestroyrecord(record);
        }
    } while (index > 0);

    free(record_deletes);

    return rv;
}

static int
delete_resources(prc_t* prc, char** argv)
{
    int i;
    prc_resource_t* resource;

    /* type id */
    for (i = 0; argv[i] != NULL;) {
        prc_uint16_t  id;
        prc_type_t type;
        char* type_s;
        char* id_s;

        type_s = argv[i++];
        id_s = argv[i++];
        
        type = prcstrtotype(type_s);
        id = prcstrtoid(id_s);

        resource = prcgetresource(prc, type, id);
        if (!resource) {
            fprintf(stderr, "resource, type='%s', id=0x%x not found\n",
                    type_s, id);
            return -1;
        }

        /* remove the record from the prc, the destroy */
        resource = prcremoveresource(prc, resource);
        if (!resource) {
            fprintf(stderr, "problem deleting resource, type='%s', id=0x%x\n",
                    type_s, id);
            return -1;
        } else {
            prcdestroyresource(resource);
        }
    }
    
    return 0;
}

static int
prcdelete(prc_t* prc, char** argv)
{
    if (PRC_IS_RESOURCE(prc)) {
        return delete_resources(prc, argv);
    } else {
        unsigned dAppInfo;
        unsigned dSortInfo;
        unsigned n = parse_info_args(argv, 0, &dAppInfo, 0, &dSortInfo);

        if (dAppInfo)
            prcsetappinfo(prc, NULL, 0);

        if (dSortInfo)
            prcsetsortinfo(prc, NULL, 0);

        return delete_records(prc, &argv[n]);
    }
}

const char*
prcbasename(const char* s)
{
    char* p = strrchr(s, '/');
    if (p != NULL) {
        return (const char*)(p + 1);
    } else {
        return s;
    }
}

const char*
prcsuffixname(const char* s)
{
    char* p = strrchr(s, '.');
    if (p != NULL) {
        return (const char*)(p + 1);
    } else {
        return s;
    }
}

const unsigned
prcfileisprc(const char* s)
{
    /* this is only a guess, need to open file */
    const char* suf = prcsuffixname(s);
    if (strcmp(suf, "prc") == 0 || strcmp(suf, "PRC") == 0 ||
        strcmp(suf, "pdb") == 0 || strcmp(suf, "PDB") == 0)
        return 1;
    else
        return 0;
}

static unsigned
parse_record_args(char** argv,
                  unsigned* category, unsigned* flags, prc_uint32_t* uid,
                  unsigned* isPrc_r)
{
    unsigned n;
    unsigned options = 0;
    char* f;

    *isPrc_r = 0;

    for (n = 0; (f = argv[n]) != NULL && f[0] == '-';) {
        if (strcmp(f, "-c") == 0) {
            *category = atoi(argv[n+1]);
            n += 2;
            options++;
        } else if (strcmp(f, "-u") == 0) {
            *uid = atoi(argv[n+1]);
            n += 2;
            options++;
        } else if (strcmp(f, "-a") == 0) {
            *flags = prcstrprecordattr(argv[n+1]);
            n += 2;
            options++;
        } else if (strcmp(f, "-p") == 0) {
            *isPrc_r = 1;
            return n;
        } else {
            break;
        }
    }

    if (options == 0) {
        unsigned flag;
        prc_uint32_t id;
        unsigned index;
        unsigned foo;

        if (prcfileisprc(argv[0])) { /* I think it's a prc */
            *isPrc_r = 1;
            return n;
        }

        foo = sscanf(f, "%d.%x.%ld", &index, &flag, &id);

        if (foo == 3) {
            *category = PRC_REC_FLAGS_GET_CATEGORY(flag);
            *flags = flag & 0xF0;
            *uid = id;
        }
    }
    
    return n;
}

static int
append_info(prc_t* prc, char** argv)
{
    int n = 0;
    char* appInfo = NULL;
    char* sortInfo = NULL;
    prc_byte_t*  data;
	prc_uint32_t datalen;

    n += parse_info_args(argv, &appInfo, 0, &sortInfo, 0);

    if (appInfo != NULL) { /* set app info from file */
        data = file_to_data(appInfo, &datalen);
        prcsetappinfo(prc, data, datalen);
        free(data);
    }

    if (sortInfo != NULL) { /* set sort info from file */
        data = file_to_data(sortInfo, &datalen);
        prcsetsortinfo(prc, data, datalen);
        free(data);
    }

    return n;
}

static int
append_one_record(prc_t* prc,
                  prc_uint32_t id,
                  prc_byte_t   flags,
                  prc_byte_t*  data,
                  prc_uint32_t datalen)
{
    prc_record_t* rec = prcnewrecord();

    prcrecordset(rec, data, datalen);

    rec->flags = flags;
    rec->id = id;
    
    rec = prcappendrecord(prc, rec);
    if (rec == NULL) {
        fprintf(stderr, "could not append record\n");
        return -1;
    }
    return 0;
}

static int
append_record_mappee(prc_record_t* m_record, void* arg)
{
    prc_t* prc = (prc_t*)arg;
    
    return append_one_record(prc,
                             m_record->id,
                             m_record->flags,
                             m_record->data,
                             m_record->datalen);
}

static int
append_records(prc_t* prc, char** argv)
{
    int n;

    for (n = 0; argv[n] != NULL; n++) {
        prc_byte_t*  data;
        prc_uint32_t datalen;
        unsigned     category = 0;
        unsigned     flags = 0;
        prc_uint32_t uid = 0;
        unsigned     isPrc;
        int x;

        n += parse_record_args(&argv[n], &category, &flags, &uid, &isPrc);

        if (isPrc) {
            prc_t* a_prc = prcopen(argv[n], PRC_OPEN_READ);

            if (a_prc == NULL) {
                fprintf(stderr, "could not open: %s\n", argv[n]);
                return -1;
            }

            if (PRC_IS_RESOURCE(a_prc)) {
                fprintf(stderr, "%s is a resource file\n", argv[n]);
                prcclose(a_prc);
                return -1;
            }

            x = prcmaprecords(a_prc, append_record_mappee, prc);

            prcclose(a_prc);

            if (x == -1)
                return -1;
        } else {

            prc_byte_t flags_b;

            data = file_to_data(argv[n], &datalen);

            if (!data)
                return -1;

            flags_b = (prc_byte_t)PRC_REC_FLAGS_MAKE(flags, category);

            x = append_one_record(prc,
                                  uid,
                                  flags_b,
                                  data,
                                  datalen);

            free(data);

            if (x != 0)
                return -1;
        }
    }

    return n;
}

static unsigned
filename_to_type_id(const char* s, prc_uint32_t* type_r, prc_uint16_t* id_r)
{
    if (!s || strlen(s) < 8)
        return 0;
    if (!(isxdigit((unsigned)s[4]) &&
          isxdigit((unsigned)s[5]) &&
          isxdigit((unsigned)s[6]) &&
          isxdigit((unsigned)s[7])))
        return 0;
    *type_r = prcstrtotype((char*)s);
    *id_r = (prc_uint16_t)strtol((char*)&s[4], NULL, 16);
    return 1;
}

static int
parse_resource_args(char** argv,
                    prc_uint32_t* type_r,
                    prc_uint16_t* id_r,
                    char** filename_r,
                    unsigned* isPrc_r)
{
    int n = 1;
    prc_uint32_t type;
    prc_uint16_t id;
    char* filename;
    unsigned isPrc = 0;

    /* [-T type id ] filename */
    if (strcmp("-T", argv[0]) == 0) {
        type = prcstrtotype(argv[n++]);
        id = prcstrtoid(argv[n++]);
        filename = argv[n++];
    } else if (strcmp("-p", argv[0]) == 0) {
        filename = argv[n++];
        isPrc = 1;
    } else { /* try to decode type & id from filename */
        if (prcfileisprc(argv[0])) { /* I think it's a prc */
            isPrc = 1;
        } else {
            /* TTTTiiii[.suffix] */
            const char* base = prcbasename(argv[0]);

            if (!filename_to_type_id(base, &type, &id)) {
                fprintf(stderr,
                        "Unable to determine the resource type and id from "
                        "filename: %s (expected form is TTTTxxxx[.suffix])\n",
                        argv[0]);
                return -1;
            }
        }
        filename = argv[0];
    }

    *type_r = type;
    *id_r = id;
    *filename_r = filename;
    *isPrc_r = isPrc;

    return n;
}

static int
append_one_resource(prc_t* prc,
                           prc_uint32_t type,
                           prc_uint16_t id,
                           prc_byte_t*  data,
                           prc_uint32_t datalen)
{
    prc_resource_t* resource;
    unsigned        append = 0;

    if ((resource = prcgetresource(prc, type, id)) == NULL) {
        resource = prcnewresource();
        append++;
    } else {
        fprintf(stderr,
                "replacing resource %s 0x%04x\n",
                prctypetostr(NULL, type), id);
    }
    
    prcresourceset(resource, type, id, data, datalen);
    
    if (append)
        resource = prcappendresource(prc, resource);
    
    if (resource == NULL) {
        fprintf(stderr,
                "could not append resource %s 0x%04x\n",
                prctypetostr(NULL, type), id);
        return -1;
    }

    return 0;
}
 
static int
append_mappee(prc_resource_t* m_resource, void* arg)
{
    prc_t* prc = (prc_t*)arg;
    
    return append_one_resource(prc,
                               m_resource->type,
                               m_resource->id,
                               m_resource->data,
                               m_resource->datalen);
}

static int
append_resources(prc_t* prc, char** argv)
{
    int n;

    for (n = 0; argv[n] != NULL;) {
        prc_byte_t*  data;
        prc_uint32_t datalen;
        prc_uint32_t type;
        prc_uint16_t id;
        char* filename;
        int x;
        unsigned isPrc;

        x = parse_resource_args(&argv[n], &type, &id, &filename, &isPrc);

        if (x == -1)
            return -1;

        n += x;

        if (isPrc) {
            prc_t* m_prc = prcopen(filename, PRC_OPEN_READ);

            if (m_prc == NULL) {
                fprintf(stderr, "could not open: %s\n", filename);
                return -1;
            }

            if (!PRC_IS_RESOURCE(m_prc)) {
                fprintf(stderr, "%s is not a resource file\n", filename);
                prcclose(m_prc);
                return -1;
            }

            x = prcmapresources(m_prc, append_mappee, prc);

            prcclose(m_prc);

            if (x == -1)
                return -1;
        } else {
            data = file_to_data(filename, &datalen);

            if (!data)
                return -1;

            x = append_one_resource(prc,
                                    type,
                                    id,
                                    data,
                                    datalen);
            
            free(data);

            if (x != 0)
                return -1;
        }
    }

    return n;
}

static int
append_stream(prc_t* prc, char** argv)
{
    int n;

    for (n = 0; argv[n] != NULL; n++) {
        prc_byte_t*  data;
		prc_uint32_t datalen;
        char*        filename = argv[n];

        data = file_to_data(filename, &datalen);

        if (!data)
            return -1;

        if (prcstreamwrite(prc, data, datalen) != datalen)
            return -1;

        free(data);
    }

    return n;
}

static int
prcappend(prc_t* prc, char** argv)
{
    if (PRC_IS_RESOURCE(prc)) {
        return append_resources(prc, argv);
    } else if (PRC_IS_STREAM(prc)) {
        return append_stream(prc, argv);
    } else {
        int n = append_info(prc, argv);

        if (n == -1)
            return n;

        return append_records(prc, &argv[n]);
    }
}

typedef struct type_id_t
{
    prc_uint32_t old_type;
    prc_uint16_t old_id;
    prc_uint32_t new_type;
    prc_uint16_t new_id;
} type_id_t;

static int
touch_resource_mappee(prc_resource_t* resource, void* arg)
{
    type_id_t* info = (type_id_t*)arg;

    if (resource->type == info->old_type && resource->id == info->old_id) {
        resource->type = info->new_type;
        resource->id = info->new_id;
        return 1;
    }
    
    return 0;
}

static int
touch_resources(prc_t* prc, char** argv)
{
    /* [-T type id] type id */
    int n;
    type_id_t info;

    info.new_type = 0;
    info.new_id = 0;

    for (n = 0; argv[n] != NULL;) {

        if (strcmp("-T", argv[n]) == 0) {
            n++;
            info.new_type = prcstrtotype(argv[n++]);
            info.new_id = prcstrtoid(argv[n++]);
            continue;
        }
    
        info.old_type = prcstrtotype(argv[n++]);
        info.old_id = prcstrtoid(argv[n++]);

        if (info.new_type != 0)
            prcmapresources(prc, touch_resource_mappee, &info);
    }

    return n;
}

static int
touch_records(prc_t* prc, char** argv)
{
    /* [-c category] [-a attr] [-i uid] index ... */
    int n;
    prc_uint32_t uid = 0;
    unsigned     flags = 0;
    unsigned     category = 0;

    for (n = 0; argv[n] != NULL; n++) {
        int delta;
        prc_record_t* record;
        prc_uint16_t index;
        unsigned isPrc;

        delta = parse_record_args(&argv[n], &category, &flags, &uid, &isPrc);

        if (!delta)
            continue;

        n += delta;
        index = atoi(argv[n]);

        record = prcgetrecord(prc, index);
        
        if (record->id == uid && record->flags == (flags | category))
            continue;

        record->flags = (flags | category);
        record->id = uid;
    }

    return n;
}

/*
 * records:   [-c catagory] [-a attrs] index
 * resources: [-T type id] type id
 */
static int
prctouch(prc_t* prc, char** argv)
{
    if (PRC_IS_RESOURCE(prc))
        return touch_resources(prc, argv);
    else
        return touch_records(prc, argv);
}

static void
usage()
{
    fprintf(stderr,
    "usage:\n"
    "par t dbfile                                                - list\n"
    "par h dbfile                                                - header\n"
    "par x dbfile [ioptions] [records ..]                        - extract\n"
    "par c [hoptions] dbfile name type cid [ioptions] [files ..] - create\n"
    "par u [hoptions] dbfile [ioptions] [updates ..]             - touch\n"
    "par a [hoptions] dbfile [ioptions] [files ..]               - append\n"
    "par d [hoptions] dbfile [doptions] [records ..]             - delete\n"
    "par version                                                 - version\n"
    "where dbfile is a pdb:\n"
    "ioptions = [-A appinfo] [-S sortinfo]\n"
    "doptions = [-A] [-S]\n"
    "file     = [-c category] [-a attrs] [-u uid] filename |\n"
    "           [-p databasefile] | resource.pdb\n"
    "record   = [-f filename] index\n"
    "update   = [-c category] [-a attrs] [-u uid] index\n"
    "where dbfile is a prc:\n"
    "file     = [-T type id] filename |\n"
    "           [-p resourcefile] | resource.prc\n"
    "record   = [-f filename] type id\n"
    "update   = [-T type id] type id\n"
    "hoptions:\n"
    "-n name  - database name\n"
    "-T type  - PRC creator type\n"
    "-I cid   - PRC creator id\n"
    "-a attrs - PRC attribute flags, bit-ORed symbolic or numeric:\n"
    "           resource|readonly|dirty|backup|newer|reset|copyprevent|\n"
    "           stream|hidden|launchable|recyclable|bundle|open|<numeric>\n"
    "-v vers  - 16 bit PRC version number, numeric (default is 1)\n"
    "-C ctime - creation time [[CC]YY]MMDDHHMM\n"
    "-m mtime - modification time\n"
    "-b btime - backup time\n"
    "");
}

#define HOPTIONS_NAME    (0x1<<0)
#define HOPTIONS_TYPE    (0x1<<1)
#define HOPTIONS_CID     (0x1<<2)
#define HOPTIONS_ATTR    (0x1<<3)
#define HOPTIONS_VERSION (0x1<<4)
#define HOPTIONS_CTIME   (0x1<<5)
#define HOPTIONS_MTIME   (0x1<<6)
#define HOPTIONS_BTIME   (0x1<<7)

static int
parse_hoptions(char** argv, prc_t* options, unsigned* options_set)
{
    int n = 0;
    prc_attr_t a = 0;
    prc_time_t t = 0;

    for (n = 0; argv[n] != NULL;) {
        if (strcmp(argv[n], "-n") == 0) {
            prcsetname(options, argv[n+1]);
            n += 2;
            *options_set |= HOPTIONS_NAME;
        } else if (strcmp(argv[n], "-T") == 0) {
            options->type = prcstrtotype(argv[n+1]);
            n += 2;
            *options_set |= HOPTIONS_TYPE;
        } else if (strcmp(argv[n], "-I") == 0) {
            options->cid = prcstrtotype(argv[n+1]);
            n += 2;
            *options_set |= HOPTIONS_CID;
        } else if (strcmp(argv[n], "-a") == 0) {
            a = prcstrpattr(argv[n+1]);
            options->flags = a;
            n += 2;
            *options_set |= HOPTIONS_ATTR;
        } else if (strcmp(argv[n], "-v") == 0) {
            options->version = atoi(argv[n+1]);
            n += 2;
            *options_set |= HOPTIONS_VERSION;
        } else if (strcmp(argv[n], "-C") == 0) {
            t = prcstrtotime(argv[n+1]);
            options->ctime = t;
            n += 2;
            *options_set |= HOPTIONS_CTIME;
        } else if (strcmp(argv[n], "-m") == 0) {
            t =  prcstrtotime(argv[n+1]);
            options->mtime = t;
            n += 2;
            *options_set |= HOPTIONS_MTIME;
        } else if (strcmp(argv[n], "-b") == 0) {
            t = prcstrtotime(argv[n+1]);
            options->btime = t;
            n += 2;
            *options_set |= HOPTIONS_BTIME;
        } else {
            break;
        }

        if (t == (prc_time_t)-1) {
            fprintf(stderr, "invalid time specification: %s\n", argv[n-1]);
            return -1;
        }

        if (a == (prc_attr_t)-1) {
            fprintf(stderr, "invalid attribute specification: %s\n",
                    argv[n-1]);
            return -1;
        }
    }

    return n;
}

static void
set_hoptions(prc_t* prc, prc_t* options, unsigned options_set)
{
    if (!prc || !options)
        return;

    if ((options_set & HOPTIONS_NAME) != 0)
        memcpy(prc->name, options->name, PRC_MAX_NAMESIZE); /* not so great */
    if ((options_set & HOPTIONS_TYPE) != 0)
        prc->type = options->type;
    if ((options_set & HOPTIONS_CID) != 0)
        prc->cid = options->cid;
    if ((options_set & HOPTIONS_ATTR) != 0)
        prc->flags = options->flags;
    if ((options_set & HOPTIONS_VERSION) != 0)
        prc->version = options->version;
    if ((options_set & HOPTIONS_CTIME) != 0)
        prc->ctime = options->ctime;
    if ((options_set & HOPTIONS_MTIME) != 0)
        prc->mtime = options->mtime;
    if ((options_set & HOPTIONS_BTIME) != 0)
        prc->btime = options->btime;
}

typedef enum {
    PAR_NONE = (unsigned)-1,
    PAR_LIST = 0,
    PAR_INFO,
    PAR_EXTRACT,
    PAR_CREATE,
    PAR_TOUCH,
    PAR_DELETE,
    PAR_APPEND,
    PAR_VERSION
} par_mode_t;

static par_mode_t
parse_mode(const char* s)
{
    if (strcmp(s, "-l") == 0 || strcmp(s, "l") == 0 ||
        strcmp(s, "-t") == 0 || strcmp(s, "t") == 0)
        return PAR_LIST;
    else if (strcmp(s, "-h") == 0 || strcmp(s, "h") == 0 ||
             strcmp(s, "-i") == 0 || strcmp(s, "i") == 0)
        return PAR_INFO;
    else if (strcmp(s, "-x") == 0 || strcmp(s, "x") == 0)
        return PAR_EXTRACT;
    else if (strcmp(s, "-c") == 0 || strcmp(s, "c") == 0)
        return PAR_CREATE;
    else if (strcmp(s, "-u") == 0 || strcmp(s, "u") == 0)
        return PAR_TOUCH;
    else if (strcmp(s, "-d") == 0 || strcmp(s, "d") == 0)
        return PAR_DELETE;
    else if (strcmp(s, "-a") == 0 || strcmp(s, "a") == 0)
        return PAR_APPEND;
    else if (strcmp(s, "-V") == 0 || strcmp(s, "V") == 0 ||
             strcmp(s, "-version") == 0 || strcmp(s, "version") == 0)
        return PAR_VERSION;
    else
        return PAR_NONE;
}

int
main(int argc, char** argv)
{
    char*  dbname;
    prc_t* prc;
    par_mode_t mode;
    int n = 1;
    prc_t optionsBuf;
    unsigned optionsSet;
    int rv = 0;
    char* mode_s;
    
    if (argc < 2) {
        fprintf(stderr, "not enough args!\n");
        usage();
        return 2;
    }

    /* init hoptions */
    memset(&optionsBuf, 0, sizeof(prc_t));
    optionsSet = 0;

    mode_s = argv[n++];
    
    if (strcmp(mode_s, "-r") == 0 || strcmp(mode_s, "r") == 0) { /* hack */
        optionsBuf.flags = PRC_FLAGS_RESOURCE;
        optionsSet |= HOPTIONS_ATTR;
        mode_s = "-c";
    }

    mode = parse_mode(mode_s);

    if (mode == PAR_NONE) {
        fprintf(stderr, "invalid mode\n");
        usage();
        return 2;
    }

    if (mode == PAR_VERSION) {
        printf("%02ld.%02ld", LIBPRC_VERSION_MAJOR, LIBPRC_VERSION_MINOR);
        if (LIBPRC_VERSION_MICRO != 0UL)
            printf(".%02ld", LIBPRC_VERSION_MICRO);
        printf("\n");
        return 0;
    }

    if (argc < 3) {
        fprintf(stderr, "not enough args!\n");
        usage();
        return 2;
    }

    if (mode == PAR_CREATE || mode == PAR_TOUCH ||
        mode == PAR_DELETE || mode == PAR_APPEND) {
        int rv;

        /* always touch the mod time for writable modes */
        optionsBuf.mtime = prctime(NULL); /* now */
        optionsSet |= HOPTIONS_MTIME;
        rv = parse_hoptions(&argv[n], &optionsBuf, &optionsSet);
        if (rv == -1) {
            usage();
            return 2;
        }
        n += rv;
    }
    
    dbname = argv[n++];

    switch (mode) {
    case PAR_CREATE: /* create */ {
        const char* name;
        prc_type_t  type;
        prc_type_t  cid;

        if (argc < n + 3) {
            fprintf(stderr, "not enough args!\n");
            usage();
            return 2;
        }
        
        if (strlen(argv[n+1]) != 4) {
            fprintf(stderr, "type must be 4 chars\n");
            usage();
            return 2;
        }

        if (strlen(argv[n+2]) != 4) {
            fprintf(stderr, "creator id must be 4 chars\n");
            usage();
            return 2;
        }

        name = argv[n++];
        type = prcstrtotype(argv[n++]);
        cid = prcstrtotype(argv[n++]);

        prc = prccreate(dbname, name, type, cid, optionsBuf.flags);

        if ((optionsSet & (HOPTIONS_NAME|HOPTIONS_TYPE|HOPTIONS_CID)) != 0)
            fprintf(stderr, "warning: -n,-T,-I ignored with -c mode\n");

        optionsSet &= ~(HOPTIONS_NAME|HOPTIONS_TYPE|HOPTIONS_CID);
    }
    break;
    case PAR_DELETE: /* update */
    case PAR_APPEND:
        prc = prcopen(dbname, PRC_OPEN_UPDATE);
        break;
    case PAR_TOUCH:  /* touch */
        prc = prcopen(dbname, PRC_OPEN_TOUCH);
        break;
    case PAR_LIST:   /* read only */
    case PAR_INFO:
    case PAR_EXTRACT:
        prc = prcopen(dbname, PRC_OPEN_READ);
        break;
    default:
        fprintf(stderr, "mode not implimented\n");
        return 3;
    }

    if (!prc) {
        prc_errno_t err = prcgeterrno();
        const char* e_string = prcstrerror(err);
        fprintf(stderr, "failed to open %s: %s\n", dbname, e_string);
        return 1;
    }

    if ((prc->flags & PRC_FLAGS_RESOURCE) == 0 &&
        (optionsBuf.flags & PRC_FLAGS_RESOURCE) != 0) {
        fprintf(stderr,
                "'resource' attribute can only be set at create time\n");
        return 2;
    }

    if (optionsSet != 0) {
        set_hoptions(prc, &optionsBuf, optionsSet);
    }

    switch (mode) {
    case PAR_CREATE: /* create become append */
    case PAR_APPEND:
        rv = prcappend(prc, &argv[n]);
        break;
    case PAR_TOUCH:  /* touch */
        rv = prctouch(prc, &argv[n]);
        break;
    case PAR_LIST:   /* read only */
        rv = prclist(prc);
        break;
    case PAR_INFO:
        rv = prcinfo(prc);
        break;
    case PAR_EXTRACT:
        rv = prcextract(prc, &argv[n]);
        break;
    case PAR_DELETE: /* delete */
        rv = prcdelete(prc, &argv[n]);
        break;
    default:
        fprintf(stderr, "mode not implimented\n");
        return 3;
    }

    prcclose(prc);

    if (rv == -1)
        return 1;
    else
        return 0;
}
