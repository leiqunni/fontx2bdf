/*
 *  fontx2bdf -- fontx2 -> bdf font file convertor
 *	fontx2 �����Υե���Ȥ� bdf �����ˤ��롣
 *
 *  ����ѥ������ˡ:
 *	gcc -o fontx2bdf fontx2bdf.c
 *
 *  �Ȥ���:
 *	fontx2bdf < $fontx2�����Υե����� | bdftopcf > �ʤ�Ȥ�.pcf
 *
 *  �Х�:
 *      bdf�ե��������Ƭ�������ꤵ����������˥����ø��Ǥ� (^^;)��
 *	���Υץ����ϡ�80x86 (FreeBSD, MS-DOS, etc.) �ʳ��δĶ��Ǥ�
 *	�տޤ����Ȥ���ˤ�ư���ʤ����⤷��ޤ���
 *
 *  Copyright (c) 1995 by Dai Ishijima
 *
 */

/*
 *  ����2��: Oct. 14, 1997 by Dai ISHIJIMA
 *      �ѹ���
 *          ��80x86 �ʳ��Ǥ� (���֤�) �Ȥ���褦�ˤʤä�
 *          ������ѥ������ -Wall ���ץ�����Ĥ��ơ������å��򸷤�������
 */

#ifdef __TURBOC__
#define MSDOS
#endif
#ifdef _MSC_VER
#define MSDOS
#endif
#ifdef LSI_C
#define MSDOS
#define HAVE_FSETBIN
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#ifdef MSDOS
#include <fcntl.h>
#include <io.h>
#endif
#ifndef HAVE_FSETBIN
#define fsetbin(fp) setmode(fileno(fp), O_BINARY)
#endif

#define SBCS 0
#define DBCS 1
#define ID_LEN 6
#define NAM_LEN 8

#define NPROP 18
#define DESCENT 2

#define CHARS_SBCS 256


/* $fontx2 �ե�����Υإå����� */
typedef struct {
    char id[ID_LEN];
    char name[NAM_LEN];
    unsigned char width;
    unsigned char height;
    unsigned char type;
} fontx_h;


/* ��Ǽ���Ƥ���ʸ����ɽ */
typedef struct {
    unsigned short start;
    unsigned short end;
} fontx_tbl;


/* BDF�ե��������Ƭ�� */
void bdfheader(int width, int height, int startcode, char *name, int type)
{
    int point_size;
    int pixel_size;
    int av_width;

    pixel_size = height;		/* �⤵ [�ɥå�ñ��] */
    point_size = (height - 1) * 10;	/* �⤵ [�ݥ���� x 10] */
    av_width = width * 10;		/* ʿ���� [�ݥ���� x10] */
    /* */
    printf("STARTFONT 2.1\n");
    printf("COMMENT\n");
    printf("FONT -%s-Fixed-Medium-R-Normal-", name);
    printf("-%d-%d",  pixel_size, point_size);
    if (type == DBCS) {
	printf("-75-75-C-%d-JISX0208.1983-0\n", av_width);
    }
    else {
	printf("-75-75-C-%d-JISX0201.1976-0\n", av_width);
    }
    printf("SIZE %d 75 75\n", width);
    printf("FONTBOUNDINGBOX %d %d 0 %d\n", width, height, -DESCENT);
    printf("STARTPROPERTIES %d\n", NPROP);
    printf("FONTNAME_REGISTRY \"\"\n");
    printf("FOUNDRY \"%s\"\n", name);
    printf("FAMILY_NAME \"Fixed\"\n");
    printf("WEIGHT_NAME \"Medium\"\n");
    printf("SLANT \"R\"\n");
    printf("SETWIDTH_NAME \"Normal\"\n");
    printf("ADD_STYLE_NAME \"\"\n");
    printf("PIXEL_SIZE %d\n", pixel_size);
    printf("POINT_SIZE %d\n", point_size);
    printf("RESOLUTION_X 75\n");
    printf("RESOLUTION_Y 75\n");
    printf("SPACING \"C\"\n");
    printf("AVERAGE_WIDTH %d\n", av_width);
    if (type == DBCS) {
	printf("CHARSET_REGISTRY \"JISX0208.1983\"\n");
    }
    else {
	printf("CHARSET_REGISTRY \"JISX0201.1976\"\n");
    }
    printf("CHARSET_ENCODING \"0\"\n");
    printf("DEFAULT_CHAR %d\n", startcode);
    printf("FONT_DESCENT %d\n", DESCENT);
    printf("FONT_ASCENT %d\n", height - DESCENT);
/*  printf("COPYRIGHT \"?\"\n");*/
    printf("ENDPROPERTIES\n");
}


/* $fontx2�إå�������ɤߤȤ� */
int readheader(FILE *fp, fontx_h *header)
{
    fread(header->id, ID_LEN, 1, fp);
    if (strncmp(header->id, "FONTX2", ID_LEN) != 0) {
	return(1);
    }
    fread(header->name, NAM_LEN, 1, fp);
    header->width = (unsigned char)getc(fp);
    header->height = (unsigned char)getc(fp);
    header->type = (unsigned char)getc(fp);
    return(0);
}


/* i386 �� short int ���ɤ� */
unsigned short getshort(FILE *fp)
{
    int i, j;

    i = (unsigned char)getc(fp);
    j = (unsigned char)getc(fp);
    return(i + j * 256);
}


/* ��Ǽ���Ƥ���ʸ����ɽ���ɤ� */
void readtbl(fontx_tbl *table, int size, FILE *fp)
{
    while (size > 0) {
	table->start = getshort(fp);
	table->end = getshort(fp);
	++table;
	--size;
    }
}


/* �ե����̾�� $fontx2 ���� BDF �� */
void copyname(char *bdf_name, char *fontx_name)
{
    int i, j;

    j = i = 0;
    while (i < NAM_LEN) {
	bdf_name[j] = tolower(fontx_name[i]);
	if (bdf_name[j] == 0) {
	    break;
	}
	if ((bdf_name[j] != '-') && (bdf_name[j] != ' ')) {
	    ++j;	/* '-' �� ' ' �ʤ饹���å� */
	}
	++i;
    }
    bdf_name[j] = 0;
    bdf_name[0] = toupper(bdf_name[0]);
}


/* ��ʸ���Υӥåȥޥåפ�� */
void tobdf(int code, int width, int height)
{
    int x, y;

    printf("STARTCHAR %04x\n", code);
    printf("ENCODING %d\n", code);
    printf("SWIDTH %d 0\n", width * 72);
    printf("DWIDTH %d 0\n", width);
    printf("BBX %d %d 0 %d\n", width, height, -DESCENT);
    printf("BITMAP\n");
    for (y = 0; y < height; y++) {
	for (x = 0; x < (width + 7) / 8; x++) {
	    printf("%02x", getchar());
	}
	printf("\n");
    }
    printf("ENDCHAR\n");
}


/* �ե���ȥե�����˲�ʸ���ξ��󤬤��뤫 */
unsigned int nchars(int n_tbl, fontx_tbl *table)
{
    int i;
    int n;

    n = 0;
    for (i = 0; i < n_tbl; i++) {
	n += table[i].end - table[i].start + 1;
    }
    return(n);
}


/* ���ե�JIS -> JIS �Ѵ� */
int stoj(int code)
{
    int x, y;
    int hi, lo;

    hi = (unsigned short)code / 256;
    lo = (unsigned short)code % 256;
    if (hi >= 0xe0) {
	hi -= (0xe0 - 0xa0);
    }
    if (lo > 0x9e) {
	x = (hi - 0x80) * 2 + 0x20;
	y = lo + 0x20 - 0x9e;
    }
    else {
	x = (hi - 0x80) * 2 + 0x20 - 1;
	if (lo > 0x7f) {
	    --lo;
	}
	y = lo - 0x1f;
    }
    return(x * 256 + y);
}


void main()
{
    fontx_h h;
    unsigned char size;
    fontx_tbl *table;
    int i;
    unsigned int code;
    char name[10];
    
#ifdef __MSDOS__
    fsetbin(stdin);
#endif
    if (readheader(stdin, &h) != 0) {
	fprintf(stderr, "not in FONTX2 format\n");
	exit(1);
    }
    copyname(name, h.name);
    if (h.type == DBCS) {
        size = getc(stdin);
	table = (fontx_tbl *)calloc(size, sizeof(fontx_tbl));
/*	fread(table, size, sizeof(fontx_tbl), stdin);*/
	readtbl(table, size, stdin);
	bdfheader(h.width, h.height, stoj(table[0].start), name, DBCS);
	printf("CHARS %u\n", nchars(size, table));
	for (i = 0; i < size; i++) {
	    for (code = table[i].start; code <= table[i].end; code++) {
		tobdf(stoj(code), h.width, h.height);
	    }
	}
    }
    else {
	bdfheader(h.width, h.height, 0, name, h.type);
	printf("CHARS %u\n", 256);
	for (i = 0; i < 256; i++) {
	    tobdf(i, h.width, h.height);
	}
    }
    printf("ENDFONT\n");
    exit(0);
}

/* Local Variables: */
/* compile-command:"gcc -o fontx2bdf fontx2bdf.c" */
/* End: */
