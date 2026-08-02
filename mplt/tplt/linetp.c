/*
 * linetp - draw a line between two points
 */
void
linetp(double x1, double y1, double x2, double y2)
{
    float   x[2], y[2];
    void    line(int, float *, float *);

    x[0] = (float) x1;
    y[0] = (float) y1;
    x[1] = (float) x2;
    y[1] = (float) y2;
    line(2, x, y);
}
