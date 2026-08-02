/* cincl.h */

struct cincl {
    int     inclev;
    FILE   *inc_file[MAXINCL];
};

extern struct cincl cincl_;
