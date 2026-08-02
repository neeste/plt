/* device.h */

struct dev {
    float adupi;
    float curxnc;
    float curync;
    float chhnc;
    float chuxnc;
    float chuync;
    float chsxnc;
    float chsync;
    float chsp;
    float chxp;
    short txpr;
    short posknwn;
    short maxpen;
    short prompt;
    short border;
    short fgci;
    short bgci;
    short fnci;
    short bci;
    short wci;
    short nci;
    short gray;
    short fillwhite;
    short isopen;
    short widestroke;
};

extern struct dev dev_;

extern int (*d_open) (void);
extern void (*d_close) (void);
extern void (*d_trns) (double, double, int);
extern void (*d_curv) (float *, float *);
extern void (*d_page) (void);
extern void (*d_pnwd) (double);
extern void (*d_text) (double, double, char *);
extern void (*d_qtxx) (double, double, char *, TXX *);
extern void (*d_fill) (int, float *, float *, int, int);
extern void (*d_setpal) (void);
extern void (*d_clip) (void);

#define DEV			copa_.outdev
#define DEV_OPEN()		((*d_open)())
#define DEV_CLOSE()		((*d_close)())
#define DEV_TRNS(x,y,d)		((*d_trns)(x,y,d))
#define DEV_CURV(x,y)		((*d_curv)(x,y))
#define DEV_PAGE()		((*d_page)())
#define DEV_PNWD(w)		((*d_pnwd)(w))
#define DEV_TEXT(x,y,s)		((*d_text)(x,y,s))
#define	DEV_QTXX(x,y,s,t)	((*d_qtxx)(x,y,s,t))
#define DEV_FILL(n,x,y,f,c)	((*d_fill)(n,x,y,f,c))
#define DEV_SETPAL()    	((*d_setpal)())
#define DEV_CLIP()    	        ((*d_clip)())

#define limit(a,b,c) (((b)<(a))?(a):((b)>(c))?(c):(b))
#define put_ch(c)    putc(c,fpout)


typedef struct {int l, t, r, b;} BB;	// bounding box

/* Color Table */

#define ct_scale(c) ((c) / (float) ct_max)

typedef struct {
    short int r, g, b;
}       RGB_COLOR;

extern RGB_COLOR *ct;
extern int ct_min, ct_max;
extern FILE *fpout;
