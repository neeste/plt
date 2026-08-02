/* pyhtag.c - Computes pyhtagorian distance given x,y components */

double
pythag(double a, double b)
{
    double  p, q, r;

    if (a < 0.0)
	a = -a;
    if (b < 0.0)
	b = -b;
    if (a > b) {
	p = a;
	q = b;
    } else {
	p = b;
	q = a;
    }
    if (p == 0.0)
	return (0.0);

    r = q / p;
    r *= r;
    r /= 4.0 + r;
    p += 2.0 * p * r;
    q *= r;
    r = q / p;
    r *= r;
    r /= 4.0 + r;
    p += 2.0 * p * r;
    q *= r;
    r = q / p;
    r *= r;
    r /= 4.0 + r;
    return (p + 2.0 * p * r);
}
