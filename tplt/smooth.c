/*
    Smooth the data, if necessary, using the ACM 3-point smoothing algorithm
 */

void
smooth3(int n, float *y)
{
    int     i;
    float   a, yp, yn;

    a = y[0];
    yp = y[n - 3];
    y[0] = (5 * y[0] + 2 * y[1] - y[2]) / 6;	    /* First point   */
    for (i = 2; i <= n - 1; i++) {		    /* Middle points */
	yn = y[i - 1];
	y[i - 1] = (a + yn + y[i]) / 3;
	a = yn;
    }
    y[n - 1] =  (5 * y[n - 1] + 2 * a - yp) / 6;    /* Last point    */
}
