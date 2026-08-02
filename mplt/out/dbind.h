/* dbind.h */

/* To add a new output device:
 *
 * 	(1) increment NDEV
 * 	(2) declare d_bind function
 * 	(3) add d_bind function to d_bind array
 */

#define NDEV 10

void    d0bind(void);
void    d1bind(void);
void    d2bind(void);
void    d3bind(void);
void    d4bind(void);
void    d5bind(void);
void    d6bind(void);
void    d7bind(void);
void    d8bind(void);
void    d9bind(void);

void (*d_bind[NDEV])(void) = { 
    d0bind,
    d1bind,
    d2bind,
    d3bind,
    d4bind,
    d5bind,
    d6bind,
    d7bind,
    d8bind,
    d9bind,
};
