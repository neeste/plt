/*
 * indx - find an occurrence of character c in str
 */
int
indx(char *str, int c)
{
    register char *ps;

    for (ps = str; *ps != '\0';) {
	if (*ps++ == c)
	    return (int)(ps - str);
    }
    return (0);
}
