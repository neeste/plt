/* outlib.c */

#include "output.h"

double (*_double_function[])() = {
    inqchh, qtxln, pythag, atan2deg, sindeg, cosdeg,
};
int (*_int_function[])() = {
    defdev, inqbci, inqwci, ldfont, opnout,
};
void (*_void_function[])() = {
    area, clsout, errout, inqchup, line, marker, page, qtxal, registerfn,
    resetout, setbgci, setchan, setchh, setchsp, setchup, setchxp, setclp,
    setfaci, setfais, setfalw, setfasi, setfgci, setfnci, setmkci, setmkfi,
    setmkfw, setmks, setmkt, setmkup, setnpn, setpci, setpgb, setpgp, 
    setplt, setplw, setpps, settxal, settxci, settxft, settxlw, settxpr, 
    setvup, setwidestroke, setwin, set_creator, set_date, set_title, 
    text, updout, orient,
};

/* this function is the entry point for the DLL */

int _stdcall LibMain(void *hinstDll,unsigned long dwReason,void *reserved)
{
	return(1);
}
