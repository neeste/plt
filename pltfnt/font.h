/* font.h - FONT structure definition */

#define MAXNFNT 3
#define MAXNCHR 128
#define MAXNSYM 32
#define MAXNCND 1024
#define MAXNSND 512

typedef struct {
    short int nkhar;
    short int fnnd;
    char    ftyp, fpsp;
    char    fchh, fchw, flnw, fsln;
    char    fnx0, fny0;
    char    fhal[4];
    char    fval[6];
    char    frsv[10];		/* reserve bytes up to 32 byte boundary */
    char   *xlft;
    char   *xadv;
    short int *khar;
    short int *node;
}       FONT;

typedef struct {
    float   txx, txy, tyx, tyy, txo, tyo;
}       TRANSF;

extern FONT *curfont[MAXNFNT];
extern FONT *deffont[MAXNFNT];
