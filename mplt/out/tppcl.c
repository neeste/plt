/* tppcl.c */

#define MAXNCUT 64

void    otrans(double, double, float *, float *);
void    tpcl(double, double, double, double);

static int intersect(double, double, double, double, double, double, double, double,
                     float *, float *);
static int online(double, double, double, double, double, double);

/* tppcl - Two-point 'polygon-clipped' line
 */
void
tppcl(double x1, double y1, double x2, double y2, int n, float *px, float *py, int nc)
{
    int     i, j, k, on0, on1, on2, ni;
    float   xc[MAXNCUT], yc[MAXNCUT];
    float   px1, py1, px2, py2, ax, ay;

    /* 
     * Find an off-line vertex
     */
    px1 = 0;
    py1 = 0;
    on1 = 0;
    for (j = 0; j < n; j++) {
        px1 = px[j];
	py1 = py[j];
	on1 = online(x1, y1, x2, y2, px1, py1);
	if (on1 != 0)
	    break;
    }
    if (on1 == 0) // quit if no off-line vertices 
	return;
    on0 = on1;
    /* 
     * Find intersection points 
     */
    for (i = 0, ni = 0; i < n && ni < MAXNCUT; i++) {
	k = (i + j + 1) % n;
	px2 = px[k];
	py2 = py[k];
	if (px2 == px1 && py2 == py1)	// skip redundant vertices
	    continue;
	on2 = online(x1, y1, x2, y2, px2, py2);
	if (on2 != on1 && (on1 != 0 || on2 == on0)) {
	    ni += intersect(x1, y1, x2, y2, px1, py1, px2, py2, xc + ni, yc + ni);
	}
	px1 = px2;
	py1 = py2;
	on1 = on2;
	if (on1 != 0)
	    on0 = on1;
    }
    /*
     * Sort list of intersection points 
     */
    for (i = 0; i < ni; i++) {
	for (j = i + 1; j < ni; j++) {
	    if (xc[j] < xc[i] || (xc[j] == xc[i] && yc[j] < yc[i])) {
		ax = xc[i];
		ay = yc[i];
		xc[i] = xc[j];
		yc[i] = yc[j];
		xc[j] = ax;
		yc[j] = ay;
	    }
	}
    }
    /*
     * Convert to normalized coordinates, if needed
     */
    if (!nc) {
        for (i = 0; i < ni; i ++) {
            otrans(xc[i], yc[i], xc + i, yc + i);
	}
    }
    /*
     * Draw lines between pairs of intersection points
     */
    for (i = 1; i < ni; i += 2) {
	tpcl(xc[i - 1], yc[i - 1], xc[i], yc[i]);
    }
}

/* online - return 0 if the point (x3, y3) is on the line (x1, y1)-(x2, y1).
 *	    return -1, if the point is to the right.
 *	    return +1, if the point is to the left.
 *	    [Based on the sign of a cross-product]
 */
static int
online(x1, y1, x2, y2, x3, y3)
double  x1, y1, x2, y2, x3, y3;
{
    double cz;

    cz = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    if (cz > 0)
	return (1);
    if (cz < 0.)
	return (-1);
    return (0);
}

/* intersect - find the intersection of line A and segment B
 * Returns true if a valid intersection is found.
 * The intersection point is returned in (abx,aby).
 */
static int
intersect(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2, abx, aby)
double  ax1, ay1, ax2, ay2, bx1, by1, bx2, by2;
float  *abx, *aby;
{
    double   dax, day, dbx, dby, dx1, dy1, tb, det;

    dax = ax2 - ax1;
    day = ay2 - ay1;
    dbx = bx2 - bx1;
    dby = by2 - by1;
    det = dax * dby - day * dbx;
    if (det == 0.)
	return (0);		/* quit if lines are parallel */
    dx1 = bx1 - ax1;
    dy1 = by1 - ay1;
    tb = (day * dx1 - dax * dy1) / det;
    if (tb < 0 || 1 < tb)
	return (0);		/* intersection not within segment B */
    *abx = (float) (bx1 + tb * dbx);
    *aby = (float) (by1 + tb * dby);
    return (1);
}
