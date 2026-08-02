/* winout.h - additional functions to support WIN32 interface */

#define COPY	"Copyright 1988-2005"
#define BTNRH	"Boys Town National Research Hospital"
#define RIGHTS	"Non-profit redistribution permitted."

#define REPLOT              1
#define PRINT_FILE          2
#define SET_SIZE            3
#define SET_ASPECT          4
#define MN_HELP_ABOUT       5
#define ENTER               6
#define EXIT                7
#define WINOUT              51
#define WINOUT_ICON         52
#define ABOUTBOX            53

#ifdef __cplusplus
extern "C" {
#endif

int     inq_frame(double *x, double *y, int m);
int     win_done();
int     win_file();
void    win_DC(HDC, int, int, int, int);
void    win_mesg(char *, char *);
void    win_quit();
void    win_page(void (*bp)(), void (*ep)());
void    win_title(char *);

#ifdef __cplusplus
}
#endif
