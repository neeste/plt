/* output.h */

#include <stdio.h>
#include <stdlib.h>

#define XPAGE x_page		/* x page size in inches */
#define YPAGE y_page		/* y page size in inches */

#define TXPRDEF 0		/* Text precision is default */
#define TXPRSTR 1		/* Text precision is string */
#define TXPRCHR 2		/* Text precision is character */
#define TXPRSTK 3		/* Text precision is stroke */
#define TXPREXT 4		/* Text precision is extent */

#define STDFONT 0		/* standard text font */
#define ALTFONT 11		/* alternate text font */

typedef struct {
    float   xtx0;
    float   ytx0;
    float   xcat;
    float   ycat;
    float   xtxx[4];
    float   ytxx[4];
}       TXX;

/* ANSI C prototypes */

double  atan2deg(double, double);
double  cosdeg(double);
double  inqchh(void);
double  pythag(double, double);
double  qtxln(char *);
double  sindeg(double);
int     defdev(void);
int     inq_frame(double *, double *, int);
int     inqbci(void);
int     inqwci(void);
int     ldfont(char *, int);
int     opnout(int);
int     outfile(char *);
void    area(int, float *, float *);
void    clsout(void);
void    curve(float *, float *);
void    errout(char *);
void    inqchup(double *, double *);
void    line(int, float *, float *);
void    marker(int, float *, float *);
void    page(void);
void    orient(int);
void    qtxal(int *, int *);
void    registerfn(char *);
void    resetout(void);
void    set_creator(char *);
void    set_date(char *);
void    set_title(char *);
void    setbgci(int);
void    setchan(double);
void    setchh(double);
void    setchsp(double);
void    setchup(double, double);
void    setchxp(double);
void    setclp(double, double, double, double);
void    setfaci(int);
void    setfais(int);
void    setfalw(double);
void    setfasi(int);
void    setfgci(int);
void    setfnci(int);
void    setmkci(int);
void    setmkfi(int);
void    setmkfw(int);
void    setmks(double);
void    setmkt(int);
void    setmkup(double, double);
void    setnpn(int);
void    setpalette(int, float *, float *, float *);
void    setpci(int);
void    setpgb(int);
void    setpgp(int);
void    setplt(int, int);
void    setplw(double);
void    setpps(double);
void    settxal(int, int);
void    settxci(int);
void    settxft(int);
void    settxlw(double);
void    settxpr(int);
void    setvup(double, double, double, double);
void    setwidestroke(int);
void    setwin(double, double, double, double);
void    skfill(int, float *, float *, int, int);
void    skqtxx(double, double, char *, TXX *);
void    sktext(double, double, char *);
void    sound_enable(int);
void    sound_play(double, double);
void    sound_stop(void);
void    sound_wait(void);
void    text(double, double, char *);
void    updout(void);

extern float x_page, y_page;
extern short o_page;

/*lint -restore */
