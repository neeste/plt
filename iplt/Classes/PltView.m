//
//  pltView.m
//  iplt
//
//  Created by Stephen Neely on 3/19/11.
//  Copyright 2011 Boys Town National Research Hospital. All rights reserved.
//

#import "PltView.h"
#include <math.h>

#define MAXPATH         256
#define SWIPE_HORIZ_MIN	20
#define SWIPE_RATIO_MAX	0.3333333
#define MIDPOINT(p,q)	CGPointMake(((p.x)+(q.x))/2,((p.y)+(q.y))/2)
#define DISTANCE(p,q)	hypot((p.x)-(q.x),(p.y)-(q.y))
#define limit(x,y,z)    (((y)<(x))?x:(((y)>(z))?z:y))

int tplt(int, char **);
int tstout(int, char **);
int set_page(int);
int inq_page();
void mac_context(CGContextRef *, CGRect *, CGColorSpaceRef);
void mac_page(void (*bp)(int), void (*ep)(int, int));

static char appdir[MAXPATH];
static char docdir[MAXPATH];
static char filepath[MAXPATH];
static char *argv[2] = {"Plt", "/dev/null"};
static double maxjmp = 100;
static float zoom = 1;
static int argc = 2;
static int exit_flag = 0;
static int beg_page = 0;
static int end_page = 0;
static int max_page = 0;
static int lastTouchCount = 0;
static int swipe = 0;
static Boolean track = NO;
static CGColorSpaceRef	csp = NULL;
static CGPoint org0 = {0, 0};
static CGPoint orgn;
static CGRect vfrm = {{0, 0}, {320, 420}};

void
beg_pg(int pagecount)
{
    end_page = 0;
}

void
end_pg(int newpage, int pagecount)
{
    end_page = !exit_flag && !newpage;
    pagecount = inq_page();
    exit_flag = (pagecount == beg_page);
}

int
inq_break()
{
    return (exit_flag);
}

@implementation PltView

- (void)initSize {
    orgn = org0;
    zoom = 1;
}

- (id)initWithFrame:(CGRect)frame {
    // Initialize local variables
    vfrm = frame;
    *filepath = 0;
    orgn = org0;
    self = [super initWithFrame:vfrm];
    if (self) {
	csp = CGColorSpaceCreateDeviceRGB();
	[self setNeedsDisplay];

	// Enable multiple touches
	[self setMultipleTouchEnabled:YES];

	// Set current directory
        NSString *home = NSHomeDirectory();
	NSString *bndl = [[NSBundle mainBundle] bundlePath];
        sprintf(docdir, "%s/Documents",
		[home cStringUsingEncoding: NSASCIIStringEncoding]);
	strcpy(appdir, 
		[bndl cStringUsingEncoding: NSASCIIStringEncoding]);
	if (access(docdir, 0) == 0) {
	    chdir(docdir);
	}
    }
    mac_page(beg_pg, end_pg);

    return self;
}

- (void)pltPath:(NSString *)path {
    [self setHidden:NO];
    if (path) {
        strcpy(filepath, [path cStringUsingEncoding: NSASCIIStringEncoding]);
        if (access(filepath, 0) == 0) {
            beg_page = 1;
            max_page = 999;
            argv[1] = filepath;
        } else {
            *filepath = 0;
        }
    } else {
        *filepath = 0;
    }
    [self initSize];
    [self setNeedsDisplay];
}

- (void)drawRect:(CGRect)rect {
    CGContextRef ctx = UIGraphicsGetCurrentContext();
    CGRect bnd = [self bounds];
    CGContextClearRect(ctx, bnd);
    bnd.origin = orgn;
    bnd.size.width *= zoom;
    bnd.size.height *= zoom;
    csp = CGColorSpaceCreateDeviceRGB();
    mac_context(&ctx, &bnd, csp);
    if (*filepath) {
        exit_flag = 0;
        set_page(beg_page);
        tplt(argc, argv);
        if (end_page) {
            max_page = inq_page();
        }
    } else {
        tstout(0, NULL);
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touchSet[2];
    NSUInteger touchCount = 0;
    for (UITouch *touch in touches){	// enumerate touches
	touchSet[touchCount++] = touch;
	if (touchCount == 2) {
	    break;
	}
    }
    swipe = 0;
    track = NO;
    lastTouchCount = touchCount;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    CGPoint p1, p2, pp1, pp2;
    UITouch *touchSet[2];
    NSUInteger touchCount = 0;
    touchSet[0] = [touches anyObject];  // initize
    for (UITouch *touch in touches){	// enumerate touches
	touchSet[touchCount++] = touch;
	if (touchCount == 2) {
	    break;
	}
    }
    p1 = [touchSet[0] locationInView:self];
    pp1 = [touchSet[0] previousLocationInView:self];
    if (touchCount == 2) {
	p2 = [touchSet[1] locationInView:self];
	pp2 = [touchSet[1] previousLocationInView:self];
	if (lastTouchCount == 2) {
	    double dst = fmax(1,DISTANCE(p1,p2));
	    double d0 = fmax(1,DISTANCE(pp1,pp2));
	    CGFloat zmf = limit(0.5 / zoom, dst / d0, 2);    // zoom factor
	    CGPoint cen = MIDPOINT(p1,p2);
	    CGPoint c0 = MIDPOINT(pp1,pp2);
	    orgn.x += (cen.x - c0.x) - (cen.x - orgn.x) * (zmf - 1);
	    orgn.y += (cen.y - c0.y) - (cen.y - orgn.y) * (zmf - 1);
	    zoom *= zmf;
	    [self setNeedsDisplay];
	}
	swipe = 0;
	track = NO;
    } else if (touchCount == 1) {
	if (lastTouchCount == 2) {
	    return;
	}
	if (lastTouchCount == 1) {
	    CGFloat dx = p1.x - pp1.x;
	    CGFloat dy = p1.y - pp1.y;
	    if ((zoom == 1) 
                && (abs(dy) > SWIPE_HORIZ_MIN)
                && (fabs(dx / dy) < SWIPE_RATIO_MAX)) {
                if (dy > SWIPE_HORIZ_MIN) {
		    swipe = -1;
                } else if (dy < -SWIPE_HORIZ_MIN) {
                    swipe = 1;
		}
	    } else if (zoom != 1) {
		double ds = hypot(dx,dy);
		if (0 < ds && ds < maxjmp) {
		    orgn.x += dx;
		    orgn.y += dy;
		    [self setNeedsDisplay];
		}
		swipe = 0;
		track = YES;
	    }
	}
    }
    lastTouchCount = touchCount;
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [[event allTouches] anyObject];
    if (swipe) {
        if ((swipe < 0) && (beg_page > 1) ) {
            beg_page--;
        } else if ((swipe > 0) && (beg_page < max_page) ) {
            beg_page++;
        }
	swipe = 0;
        exit_flag = 0;
	[self setNeedsDisplay];
    } else if (touch.tapCount == 2) {
	// reset page size
	if (zoom != 1 || orgn.x != org0.x || orgn.y != org0.y) {
	    zoom = 1;
	    orgn = org0;
	} else {
	    zoom = 2;
	    if (orgn.x != org0.x || orgn.y != org0.y) {
		orgn = org0;
	    } else {
		CGPoint loc = [touch locationInView:self];
		orgn.x = org0.x + (org0.x - loc.x) * (zoom - 1);
		orgn.y = org0.y + (org0.y - loc.y) * (zoom - 1);
		orgn.x -= (loc.x - vfrm.size.width / 2) / 2;            // move toward window center
		orgn.y -= (loc.y - vfrm.size.height / 2) / 2;
	    }
	}
	[self setNeedsDisplay];
    } else if (track) {
	// finish tracking
	CGPoint p1 = [touch locationInView:self];
	CGPoint pp1 = [touch previousLocationInView:self];
	CGFloat dx = p1.x - pp1.x;
	CGFloat dy = p1.y - pp1.y;
	double ds = hypot(dx,dy);
	if (0 < ds && ds < maxjmp) {
	    orgn.x += dx;
	    orgn.y += dy;
	    [self setNeedsDisplay];
	}
	track = NO;
    }
    if (zoom < 1) {
	zoom = 1;
	orgn = org0;
	[self setNeedsDisplay];
    } else {
	CGFloat ox = limit(-vfrm.size.width * (zoom - 1), orgn.x, 0);
	CGFloat oy = limit(-vfrm.size.height * (zoom - 1), orgn.y, 0);
	if (orgn.x != ox || orgn.y != oy) {
	    orgn = CGPointMake(ox, oy);
	    [self setNeedsDisplay];
	}
    }
    lastTouchCount = 0;
}

- (void)dealloc {
    [super dealloc];
}

@end
