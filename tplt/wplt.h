/* wplt.h - header for WIN32 interface */

#define OPEN_FILE           1
#define REPLOT              2
#define ENTER               3
#define EDIT_FILE           4
#define PRINT_FILE          5
#define SAVE_FILE           6
#define SET_SIZE            7
#define SET_ASPECT          8
#define EXIT                9
#define MN_HELP_ABOUT       11
#define MN_HELP_MANUAL      12
#define VIEW_LOG            13
#define COPY_CLIP           14
#define BATCH_PRINT         15
#define UPDATE              16
#define WPLT                1
#define WPLT_ICON           2
#define ABOUTBOX            3

int     parse_cs(char **, int, char *, char *);
void    orient(int);
void    win_DC(HDC, int, int, int, int);
