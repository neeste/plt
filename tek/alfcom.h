/* alfcom.h */

struct alfcom {
    int width, height, charsp, linesp, rotang, slnang;
    int tabsep, mrgsep, spacon, font, stdfnt, altfnt;
    float xratio, yratio, xscale, yscale, tr[6];
} alfcom_;

/* 
 * The following assumes ptr is a pointer to tr[6] which is set up
 * as a FORTRAN row major (2,3) matrix.
 */
#define TR(r,c) ptr[(((r)-1) + ((c)-1)*2)]
