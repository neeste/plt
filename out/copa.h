struct copa {

/* fill area attributes */
    int     curfasi;
    int     curfais;
    int     curfaci;
    float   curfalw;
    float   curxrf;
    float   curyrf;

/* line attributes */
    int     curplt;
    int     curdot;
    int     curpci;
    float   curplw;
    float   curpps;

/* text attributes */
    int     curtxpr;
    int     curtxft;
    int     curtxalh;
    int     curtxalv;
    int     curtxci;
    float   curtxlw;
    float   curchh;
    float   curchux;
    float   curchuy;
    float   curchsx;
    float   curchsy;
    float   curchxp;
    float   curchsp;

/* marker attributes */
    int     curmkt;
    int     curmkci;
    int     curmkfi;
    int     curmkfw;
    float   curmks;
    float   curmkux;
    float   curmkuy;

/* device attributes */
    int     outdev;
    int     curfgci;

/* normalization transformation */
    int     rot;
    float   oxx;
    float   oxy;
    float   oxo;
    float   oyx;
    float   oyy;
    float   oyo;
    float   ixx;
    float   ixy;
    float   ixo;
    float   iyx;
    float   iyy;
    float   iyo;

/* error logging */
    int     nerrs;
    char   *lfname;
    FILE   *logfile;

/* job information */
    char   *title;		/* file name */
    char   *creator;		/* program name */
    char   *date;		/* date and time */

};

extern struct copa copa_;

void otrans(double, double, float *, float *);

