/*
 * opnout - open output
 */
#include <signal.h>
#include "output.h"
#include "copa.h"
#include "device.h"

#ifdef _MSC_VER
#include <io.h>
#include <fcntl.h>
#include <string.h>
#define bopen(fn)	_open(fn, (int) (_O_RDONLY|_O_BINARY))
#else /* _MSC_VER */
#include <unistd.h>
#include <sys/file.h>
#define _read   read
#define _swab   swab
#define _close  close
#ifndef O_BINARY
#define O_BINARY 0
#endif
#ifdef linux
void swab();
#endif
#define bopen(fn)	open(fn, O_RDONLY | O_BINARY)
#endif /* _MSC_VER */

int     binddev(int);
int     chkdev(void);
void    onintr(int);
void    onfpe(int);
void    reset_colors(void);
void    reset_fonts(void);

FILE *fpout = NULL;

int
opnout(int device)
{
    if (!(copa_.creator && *copa_.creator))
	copa_.creator = "BTNRH";    /* set creator prior to device open */
#ifdef WIN32
    if (fpout == NULL) {
        if (device == 6) {
	    fpout = fopen("out.cgm", "wb");
	} else if (device == 7) {
	    fpout = fopen("out.emf", "wb");
	}
    }
#else /* WIN32 */
    if (fpout == NULL) {
        fpout = fdopen(fileno(stdout),"wb");
    }
    (void) signal(SIGINT, onintr);  /* catch interrupts */
    (void) signal(SIGFPE, onfpe);   /* catch floating-point errors */
#endif /* WIN32 */
    if (binddev(device) == 0)
	return (0);
    if (DEV_OPEN()== 0)
	return (0);
    resetout();
    copa_.nerrs = 0;		    /* number of messages to 'errout' */
    copa_.logfile = NULL;	    /* indicates presence of a log file */
    return (dev_.isopen = 1);	    /* successful open */
}

/*
 * clsout - close output
 */
void
clsout()
{
    if (dev_.isopen == 0)
	return;
    DEV_CLOSE();
    reset_colors();
    reset_fonts();
    fflush(fpout);
#ifdef WIN32
    if (fpout) {  
	fclose(fpout);
	fpout = NULL;
    }
    if (copa_.nerrs > 0) {
	if (copa_.logfile != NULL && copa_.logfile != stderr) {
	    fclose(copa_.logfile);
	}
    }
#else /* WIN32 */
    if (copa_.nerrs > 0) {
	if (copa_.logfile == NULL) {
	    fprintf(stderr, "\n *** %d message line%s NOT written ***\n\n",
		copa_.nerrs, (copa_.nerrs == 1) ? "" : "s");
	} else if (copa_.logfile != stderr) {
	    fclose(copa_.logfile);
	    fprintf(stderr, "\n *** %d message line%s written to '%s' ***\n\n",
		copa_.nerrs, (copa_.nerrs == 1) ? "" : "s", copa_.lfname);
	}
    }
#endif /* WIN32 */
    dev_.isopen = 0;
}

/*
 * page - go to a new page
 */
void
page()
{
    DEV_PAGE();
    dev_.posknwn = 0;
}

#include "font.h"

int
ldfont(fn, ft)
char   *fn;
int     ft;
{
    int     fnttyp, mxg, mxn;
    int     ifd, swpbyt = 0;
    unsigned int header_size = 48;
    FONT   *f_;

    if (ft < 0 || ft > 2)
	return (4);
    if (curfont[ft] != deffont[ft]) { // current font reset to default
	free((void *) curfont[ft]->xlft);
	free((void *) curfont[ft]->xadv);
	free((void *) curfont[ft]->khar);
	free((void *) curfont[ft]->node);
	free((void *) curfont[ft]);
        curfont[ft] = deffont[ft];
        dev_.txpr = TXPRSTK;
    }
    if (*fn == '\0') {
	return (0);
    }
    // initialize new font
    f_ = (FONT *) calloc(1, sizeof(FONT)); // sizeof(FONT) >= header_size
    if (f_ == NULL)
	return (2);

    if ((ifd = bopen(fn)) < 0) {
	free(f_);
	return (1);
    }
    _read(ifd, (void *) f_, header_size);
    if (f_->nkhar <= 0 || f_->nkhar > MAXNCHR) {
	_swab((char *) &f_->nkhar, (char *) &f_->nkhar, sizeof(short) * 2);
	if (f_->nkhar < 0 || f_->nkhar > MAXNCHR) {
	    free(f_);
	    _close(ifd);
	    return (3);
	}
	swpbyt = 1;
    }
    mxg = f_->nkhar;
    mxn = f_->fnnd;
    fnttyp = f_->ftyp;
    if (fnttyp < 0 || fnttyp > 2) {
	free(f_);
	_close(ifd);
	return (3);
    }
    if (fnttyp != ft) {
	if ((ft == 0 && fnttyp == 1) || (ft == 1 && fnttyp == 0)) {
	    ;
	} else {
	    free(f_);
	    _close(ifd);
	    return (3);
	}
    }
    f_->xlft = (void *) calloc(mxg, sizeof(char));
    f_->xadv = (void *) calloc(mxg, sizeof(char));
    f_->khar = (short int *) calloc(mxg + 1, sizeof(short));
    f_->node = (short int *) calloc(mxn, sizeof(short));
    if (f_->node == NULL) {
	if (f_->xlft != NULL)
	    free(f_->xlft);
	if (f_->xadv != NULL)
	    free(f_->xadv);
	if (f_->khar != NULL)
	    free(f_->khar);
	free(f_);
	_close(ifd);
	return (2);
    }
    _read(ifd, (void *) f_->xlft, sizeof(char) * mxg);
    _read(ifd, (void *) f_->xadv, sizeof(char) * mxg);
    _read(ifd, (void *) f_->khar, sizeof(short) * (mxg + 1));
    _read(ifd, (void *) f_->node, sizeof(short) * mxn);
    _close(ifd);
    if (swpbyt) {
	_swab((char *) f_->khar, (char *) f_->khar, sizeof(short) * (mxg + 1));
	_swab((char *) f_->node, (char *) f_->node, sizeof(short) * mxn);
    }
    // new font becomes current font
    curfont[ft] = f_;
    dev_.txpr = TXPRSTK;
    return (0);
}

/* defdev - return device number for default device */

int
defdev()
{
    return (chkdev());
}

int
outfile(char *fn)
{
    fpout = fopen(fn, "wb");
    return (fpout != NULL);
}

#ifndef WIN32

void
onintr(int i)
{
    (void) signal(SIGINT, SIG_IGN);	/* ignore interrupts */
    dev_.prompt = 0;
    clsout();
    fprintf(stderr, "\007");
    exit(1);
}

void
onfpe(int i)
{
    (void) signal(SIGINT, SIG_IGN);	/* ignore interrupts */
    dev_.prompt = 0;
    clsout();
    fprintf(stderr, "**** floating-point error!\n");
    exit(1);
}

#endif /* WIN32 */
