/* 
 * dumpmsg - dump palm treo text-message database entries in convenient format
 *	Copyright 2008 Steve Tell
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <glib.h>

static char *prog_version = "dumpmsg 0.1";
static char *progname;

#define TREO_MAX_NTIMES 6
#define TREO_INCOMING 1
#define TREO_OUTGOING 2

typedef struct tmsgblock {
	unsigned char *ptr;
	int len;
} tmsgblock_t;

typedef struct treomsg {
	int ntimes;
	time_t time[TREO_MAX_NTIMES];
	char *mimetype;
	char tag[5];
	GPtrArray *msgblocks;
	int total_body_len;
	unsigned char *email;
	int email_len;
	unsigned char *filename;
	int filename_len;
	int inoutflag;
	char *phone;
	char *name;
} treomsg_t;

int g_forcetext = 0;
int g_verbose = 0;
int g_debug = 0;
int g_rawoutput = 0;
char *g_outfile = NULL;
int gmtoffset = 4*60*60; // TODO.  this is right for EDT
int g_mynumber_specified;
char *g_mynumber = "unknown";

int do_file(FILE *fp, char *fname);
treomsg_t *process_message(unsigned char *data, size_t datalen);
void output_raw_message(treomsg_t *mp, char *fname);
void format_message(treomsg_t *mp, char *fname);
void save_message_body(treomsg_t *mp, char *fname);

void usage()
{
	fprintf(stderr, "Usage: %s [options] <file> ... \n", progname);
	fprintf(stderr, " - Dump palm treo message-database entries into useful form\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\t-m N   assume N is sender's phone number for mailbox output\n");
	fprintf(stderr, "\t-o F   write message body to file F\n");
	fprintf(stderr, "\t-r     raw output instead of default mailbox format\n");
	fprintf(stderr, "\t-t     force message body to be treated as text\n");
	fprintf(stderr, "\t-x     tag-by-tag debug output\n");
	fprintf(stderr, "Version: %s\n", prog_version);
}

int
main(int argc, char **argv)
{
	extern int optind;
	extern char *optarg;
	int errflg = 0;
	int c;
	int rc;

	FILE *fin;

	progname = argv[0];
	while ((c = getopt (argc, argv, "m:o:rtvx")) != EOF) {
		switch(c) {
		case 'm':
			g_mynumber = optarg;
			g_mynumber_specified = 1;
			break;
		case 'o':
			g_outfile = optarg;
			break;
		case 'r':
			g_rawoutput = 1;
			break;
		case 't':
			g_forcetext = 1;
			break;
		case 'v':
			g_verbose = 1;
			break;
		case 'x':
			g_debug = 1;
			break;
		default:
			errflg = 1;
			break;
		}
	}

	if(errflg) {
		usage();
		exit(1);
	}

	if(optind >= argc)  {
		usage();
		exit(1);
	} else {
		for(; optind < argc; optind++) {
			fin = fopen(argv[optind], "r");
			if(fin) {
				rc = do_file(fin, argv[optind]);
			} else {
				perror(argv[optind]);
				exit(1);
			}
			fclose(fin);
		}
	}

	exit(rc);
}

/*
 * process an input file
 */
int
do_file(FILE *fp, char *fname)
{
	int c;
	struct stat statbuf;
	
	unsigned char* data;
        size_t datalen;
	treomsg_t *mp;

	if(fstat(fileno(fp), &statbuf) == -1) {
		perror("stat");
		return 1;
	}

        datalen = statbuf.st_size;

	data = (unsigned char*)malloc((size_t)datalen);

	if (fread(data, 1, (size_t)datalen, fp) == -1) {
		perror("read");
		free(data);
		return 1;
	}

	mp = process_message(data, datalen);
	if(mp) {
		if(g_rawoutput)
			output_raw_message(mp, fname);
		else
			format_message(mp, fname);
		if(g_outfile)
			save_message_body(mp, g_outfile);
		return 0;
	} else 
		return 2;

}


unsigned short get_word(unsigned char *p)
{
	return p[0]<<8 | p[1];
}
unsigned long get_dword(unsigned char *p)
{
	return (p[0] << 24) 
		| (p[1] << 16)
		| (p[2] << 8)
		| p[3];
}

void
cprint(char c)  /* print c if ascii, else print '.' */
{
        if( c >= ' ' && c < 0x7f )
                putchar(c);
        else
                putchar('.');
}

void 
sprint(unsigned char *buf, int len)
{
	unsigned char c;
	for(; len; len--, buf++) {
		c = *buf;
		switch(c) {
		case '\n':
			putchar(c);
			break;
		case 0:
			break;
		default:
            // cfoust: 06/13/20
            // I would rather have the original bytes.
			putchar(c);
			/*cprint(c);*/
			break;
		}
	}
}



//#define MIN(a,b)  ((a)<(b) ? (a) : (b))

/*
 * hex+ascii dump
 */
void
xdisp(unsigned char *buf, int len, int nohdr, char *prefix)
{
        int p = 0;
        int n;

        while(len > 0) {
		if(!nohdr)
			printf("%s%04x  ", prefix, p);		
                for(n = 0; n < 16; n++) {
                        if(n < len)
                                printf("%02x ", buf[n] & 0xff);
                        else    printf("   ");
                        if(n == 7)
                                printf(" ");
                }
                printf("  ");
                for(n = 0; n < MIN(len, 16); n++)
                        cprint(buf[n]);
		if(!nohdr)
			printf("\n");

                buf += 16;
                p += 16;
                len -= 16;
        }
}

void 
unknown_short_block(unsigned short w, unsigned char *buf, int len)
{
	if(g_debug) {
		printf("? ");
		xdisp(buf, len, 1, "");
	}
}

void 
unknown_long_block(unsigned short w, unsigned char *buf, int len)
{
	if(g_debug) {
		printf(" unknown[%d]:\n", len);
		xdisp(buf, len, 0, "     ");
	}
}

treomsg_t *process_message(unsigned char *data,
		size_t datalen)
{
	unsigned char *p, *cp;
	unsigned short w;
	unsigned long palmtime, adjtime;
	p = data;
	int len;
	int i;
	struct tm *tm;
	char timestring[128];
	treomsg_t *mp;
	tmsgblock_t *mblk;
	mp = g_new0(treomsg_t, 1); 
	mp->msgblocks = g_ptr_array_sized_new(16);

	while(p+1 < &data[datalen]) {
		w = get_word(p); p+= 2;
		if(g_debug) 
			printf("%04x", w);
		
		switch(w) {
		case 0x0000:
			unknown_short_block(w, p, 0);
//			p += 2;
			break;
		case 0x0001:
			unknown_short_block(w, p, 4);
			p += 4;
			break;
		case 0x0002:
			unknown_short_block(w, p, 4);
			p += 4;
			break;

		case 0x0003:
			unknown_short_block(w, p, 6);
			p += 6;
			break;
		case 0x0004:
		case 0x0008:
			unknown_short_block(w, p, 4);
			p += 4;
			break;

		case 0x100d:
		case 0x100e:
		case 0x100f:
		case 0x1012:
			unknown_short_block(w, p, 4);
			p += 4;
			break;

		case 0x0005:
		case 0x0006:
		case 0x0007:
		case 0x8000:
			palmtime = get_dword(p);
			p += 4;
            // cfoust: 04/11/20
            // It seems like the original line had some 1.5h buffer built in for
            // reasons I don't understand.
            // PalmOS uses HFS(+) timestamps.
            // http://fileformats.archiveteam.org/wiki/HFS/HFS%2B_timestamp
            // This page just says to stick with the 2082853800.
/*#define diff (2082853800+5400-8*60*60) //64 years (1970-1904) +1,5 hour */
#define diff (2082853800-5400)
			adjtime = palmtime - diff;
            if(g_debug) {
                printf(" palmtime %u adjtime %u\n", palmtime, adjtime);
            }
			if(mp->ntimes < TREO_MAX_NTIMES-1) {
				mp->time[mp->ntimes++] = adjtime;
			}
			if(g_debug) {
				tm = localtime(&adjtime);
				strftime(timestring, 128, "%Y-%b-%d %H:%M", tm);
				printf(" time %08x %d %s", palmtime, adjtime,
				       timestring);
			}
			break;

		case 0x000B:
		case 0x2008:
			len = get_word(p); p += 2;
			mp->mimetype = p;
			if(g_debug) 
				printf(" mimetype[%d]: \"%s\"", len, p);
			p += len;
			break;
		case 0x000C:
			len = get_word(p); p += 2;
			if(g_debug) 
				printf(" 000C [%d]: \"%s\"", len, p);
			p += len;
			break;

		case 0x000d:
			unknown_short_block(w, p, 4);
			p += 4;
			break;
		case 0x000e:
			unknown_short_block(w, p, 10);
			p += 10;
			break;

		case 0x000f:
			unknown_short_block(w, p, 4);
			p += 4;
			break;

		case 0x0012:
			unknown_short_block(w, p, 4);
			p += 4;
			break;

		case 0x0013:
			unknown_short_block(w, p, 10);
			p += 10;
			break;
		case 0x001b:
			if(g_debug)
				printf(" tag %c%c%c%c", p[0], p[1], p[2], p[3]); 
			for(i = 0; i < 4; i++) {
				mp->tag[i] = p[i];
				mp->tag[4] = 0;
			}
			p += 4;
			break;
		case 0x2002:
		case 0x2006:
		case 0x4007:
			len = get_word(p); p += 2;
			mblk = g_new0(tmsgblock_t, 1);
			mblk->len = len;
			mblk->ptr = p;
			g_ptr_array_add(mp->msgblocks, mblk);
			mp->total_body_len += len;
			if(g_debug) {
				printf(" messagebody[%d]:\n", len);
				xdisp(p, len, 0, "msg  ");
			}
			p += len;
			break;
		case 0x2005:
			len = get_word(p); p += 2;
			mp->email = p;
			mp->email_len = len;
			if(g_debug) 
				printf(" email[%d]: \"%s\"", len, p);
			p += len;
			break;

		case 0x2009:
			len = get_word(p); p += 2;
			mp->filename = p;
			mp->filename_len = len;
			if(g_debug) 
				printf(" fname[%d]: \"%s\"", len, p);
			p += len;
			break;

		case 0x2000:
		case 0x2001:
		case 0x3003:
		case 0x3005:
		case 0x3006:
			len = get_word(p); p += 2;
			if(g_debug) {
				printf(" unknownstring[%d]: \"%s\"", len, p);
			}
			p += len;
			break;

		case 0x2003:
			len = get_word(p); p += 2;
			unknown_long_block(w, p, len);
			p += len;
			break;

		case 0x4000:
			len = get_word(p); p += 2;
			unknown_long_block(w, p, len);
			p += len;
			break;

		case 0x4002:
		case 0x4003:
			len = get_word(p); p += 2;
			unknown_long_block(w, p, len);
			p += len;
			break;

		case 0x4004:
			unknown_short_block(w, p, 2);
			p += 2;
			break;

		case 0x4005:
		case 0x4006:
			len = get_word(p); p += 2;
			unknown_short_block(w, p, len);
			p += len;
			break;

		case 0x4009:
		case 0x400C:
			if(w == 0x4009) {
				if(g_debug) printf(" outgoing");
				mp->inoutflag = TREO_OUTGOING;
			} else {
				if(g_debug) printf(" incoming");
				mp->inoutflag = TREO_INCOMING;
			}
				   
			len = get_word(p); p += 2;

			for(cp = p; cp < &p[len] && *cp == 0; cp++)
				;
			if(cp < &p[len]) {
				if(g_debug) printf(" phone=\"%s\"", cp);
                // For some reason in my case I had to skip a byte when
                // the message was outgoing to have the correct phone number.
                // Not sure why this is.
                // - cfoust
				mp->phone = cp + (mp->inoutflag == TREO_OUTGOING ? 1 : 0);
				cp += strlen(cp);
				cp++;
				if(*cp) {
					mp->name = cp;
					if(g_debug) printf(" name=\"%s\"", cp);
				}
			}
			p += len;
			break;

		case 0x400d: 
		case 0x400e: 
		case 0x5002: 
			len = get_word(p); p += 2;
			unknown_long_block(w, p, len);
			p += len;
			break;
			

		case 0x8002:
		case 0x8003:
		case 0x8004:
		case 0x8005:
		case 0x8006:
		case 0x8007:
		case 0x8008:
			unknown_short_block(w, p, 4);
			p += 4;
			break;
		default:
			p -= 2;
			if(g_debug) {
				printf(" unknown block:\n");
				xdisp(p, &data[datalen] - p, 0, "     ");
			}
			else
				fprintf(stderr, "unknown block type %04x\n", w);
			g_free(mp);
			return(NULL);
			break;
		}
		if(g_debug)
			printf("\n");
	}
	return mp;
}

/*
 * dump the contents of the message structure in the simplest possible format
 */
void output_raw_message(treomsg_t *mp, char *fname)
{
	struct tm *tm;
	char timestring[128];
	int i;

	printf("InputFileName: %s\n", fname);
	printf("InOutFlag: %d", mp->inoutflag);
	switch(mp->inoutflag) {
	case TREO_INCOMING:
		printf(" incoming\n");
		break;
	case TREO_OUTGOING:
		printf(" outgoing\n");
		break;
	default:
		printf("\n");
	}

	if(mp->phone)
		printf("PhoneNumber: %s\n", mp->phone);
	if(mp->name)
		printf("PhoneName: %s\n", mp->name);
	if(mp->ntimes) {
		printf("NDates: %d\n");
		for(i = 0; i < mp->ntimes; i++) {
            printf("Date%d: %u\n", i, mp->time[i]);
		}
	}
	if(mp->mimetype) {
		printf("Content-Type: %s\n", mp->mimetype);
	}
	if(mp->filename) {
		printf("X-Filename: %s\n", mp->filename);
	}
	if(mp->tag[0]) {
		printf("X-TreoTag: %s\n", mp->tag);
	}
	printf("TotalBodyLength: %d\n", mp->total_body_len);
	printf("BodyBlocks: %d\n", mp->msgblocks->len);

	if(mp->total_body_len) {
		char c;
		int b, i;
		tmsgblock_t *mblk;

		for(b = 0; b < mp->msgblocks->len; b++) {
			mblk = (tmsgblock_t *)mp->msgblocks->pdata[b];
			printf("\nblock %d; length = %d bytes\n", 
			       b, mblk->len);
			if(mblk) 
				sprint(mblk->ptr, mblk->len);
		}
	}
	printf("\n\n");
}

/*
 * write the body of a message to a file
 */
void save_message_body(treomsg_t *mp, char *fname)
{
	FILE *fp;
	char c;
	int b, i;
	tmsgblock_t *mblk;

	fp = fopen(fname, "w");
	if(!fp) {
		perror(fname);
		return;
	}

	for(b = 0; b < mp->msgblocks->len; b++) {
		mblk = (tmsgblock_t *)mp->msgblocks->pdata[b];
		if(mblk) {
			fwrite(mblk->ptr, 1, mblk->len, fp);
		}
	}
	fclose(fp);
}

/*
 * format message into email mbox format.
 */
void format_message(treomsg_t *mp, char *fname)
{
	struct tm *tm;
	char timestring[128];

	if(mp->inoutflag == TREO_INCOMING && mp->phone)
		printf("From %s", mp->phone);
	else
		printf("From %s", g_mynumber);
	if(mp->ntimes > 0) {
		tm = localtime(&mp->time[0]);
		printf(" %s", asctime(tm));
	} else {
		
		printf(" Sat Dec 31 00:00:00 1969\n");
	}


	if(mp->inoutflag == TREO_INCOMING && mp->phone) {
		printf("From:");
		if(mp->name)
			printf(" \"%s\"", mp->name);
		printf(" <%s>\n", mp->phone);
	}
	if(mp->inoutflag == TREO_OUTGOING && mp->phone) {
		if(g_mynumber_specified)
			printf("From: \"me\" <%s>\n", g_mynumber);
		printf("To:");
		if(mp->name)
			printf(" \"%s\"", mp->name);
		printf(" <%s>\n", mp->phone);
	}
	if(mp->ntimes) {
		tm = localtime(&mp->time[0]);
		strftime(timestring, 128, "%a %b %e %H:%M:%S %Y", tm);
		printf("Date: %s\n", timestring);
		printf("Timestamp: %d\n", mp->time[0]);
	}
	/* TODO: additional date/time entries */

	if(mp->mimetype) {
		printf("Content-Type: %s\n", mp->mimetype);
	}
	if(mp->filename) {
		printf("X-Filename: %s\n", mp->filename);
	}
	if(mp->tag[0]) {
		printf("X-TreoTag: %s\n", mp->tag);
	}
	printf("X-InputFileName: %s\n", fname);
	printf("\n");

	if(mp->total_body_len) {
		if(g_forcetext 
		   || !mp->mimetype 
		   || !strcmp(mp->mimetype, "text/plain")) {
			int b;
			tmsgblock_t *mblk;
			
			for(b = 0; b < mp->msgblocks->len; b++) {
				mblk = (tmsgblock_t *)mp->msgblocks->pdata[b];
				if(mblk) 
					sprint(mblk->ptr, mblk->len);
			}
			printf("\n\n");
		} else {
			printf("(nontext message body of %d bytes)\n",
			       mp->total_body_len);
		}
	}
}
