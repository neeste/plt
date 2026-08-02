/*
 * C macros to replace some of the FORTRAN intrisic functions
 */

#define min0(a,b)  (((a) < (b)) ? (a) : (b))
#define amin1(a,b) min0(a,b)
#define max0(a,b)  (((a) < (b)) ? (b) : (a))
#define amax1(a,b) max0(a,b)
#define nint(a)    ((int) (((a) < 0.0) ? (a)-0.5 : (a)+0.5))
#define amod(a,b) ((a) - floor((a)/(b))*(b))

#define TRUE 1
#define FALSE 0
