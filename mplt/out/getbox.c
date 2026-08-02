/* getbox.c - Find smallest box which encloses area */

void
getbox(int n, float  *px, float  *py,
       float  *bx1, float  *by1, float  *bx2, float  *by2)
{
    int     i;

    *bx1 = px[0];
    *bx2 = px[0];
    *by1 = py[0];
    *by2 = py[0];
    if (n <= 0)
	return;
    for (i = 0; i < n; i++) {
	if (*bx1 > px[i])
	    *bx1 = px[i];
	if (*bx2 < px[i])
	    *bx2 = px[i];
	if (*by1 > py[i])
	    *by1 = py[i];
	if (*by2 < py[i])
	    *by2 = py[i];
    }
}
