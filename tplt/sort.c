/*
 *  sotr.c - Sort the data into increasing X values
 */

void
sort(int n, float *px, float *py, float *pz)
{
    int     i, ii, j;
    float   a;

    for (i = 0; i < n; i++) {
	ii = i;
	for (j = i + 1; j < n; j++) {
	    if (px[j] < px[ii]) {
		ii = j;
	    }
	}
	if (ii != i) {
	    a = px[i];
	    px[i] = px[ii];
	    px[ii] = a;
	    a = py[i];
	    py[i] = py[ii];
	    py[ii] = a;
	    a = pz[i];
	    pz[i] = pz[ii];
	    pz[ii] = a;
	}
    }
}
