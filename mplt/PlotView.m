//
//  PlotView.m - MacPlot version
//
//  Created by Stephen Neely on 3/23/20.
//  Copyright © 2020 Stephen Neely. All rights reserved.
//

#import "PlotView.h"
#import "AppDelegate.h"

#define MAXPATH        1024


char    *inq_logfn(void);
int     tplt(int, char **);
int     errcnt(void);
void    info_line(CGContextRef, float, float, float, float, int);
void    mac_context(CGContextRef *, CGRect *, CGColorSpaceRef);
void    mac_page(void(*)(int), void(*)(int, int));
void    newext(char *, char *);
void    outfile(char *);
void    ViewPlot(char *, char *);

static char            filename[MAXPATH] = "";
static char            option[20] = "";
static float           zoom = 1;
static int             first_time = 1;
static int             needs_display = 0;
static int             curpage = 0;
static char            file_flag = 0;
static int             bmw = 525;
static int             bmh = 400;
static int             setpage = 0;
static BOOL            print_mode = 0;
static CGContextRef    vu_ctx;
static CGColorSpaceRef cs_rgb = NULL;
static CGPoint         orgn;
static CGRect          viewBounds;
static NSDate         *file_time = nil;
static NSDate         *elog_time = nil;
static NSString       *file_name = @"";

@implementation PlotView

+ (NSString *_Nonnull)getFileName {
    return [NSString stringWithUTF8String:filename];
}
+ (void)setFileName:(NSString *_Nullable)nameFile {
    file_name = nameFile;
    strncpy(filename, [nameFile UTF8String], MAXPATH);
    if (strlen(filename)) {
        curpage = setpage = 1;
    } else {
        setpage = 0;
    }
    elog_time = nil;
}
+ (NSSize)setPrintMode:(BOOL)mode {
    print_mode = mode;
    if (mode) {
        bmw = 72 * 10.5;
        bmh = 72 * 8;
    } else {
        bmw = 525;
        bmh = 400;
    }
    viewBounds = CGRectMake(0, 0, bmw, bmh);
    return NSMakeSize(bmw, bmh);
}
- (void)viewDidMoveToWindow{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowDidResize:)name:NSWindowDidResizeNotification object:[self window]];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowUpdate:)name:NSWindowDidUpdateNotification object:[self window]];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowUpdate:)name:NSWindowDidBecomeKeyNotification object:[self window]];
    if (first_time) {
        first_time = 0;
        zoom = 1;
        orgn.x = 0;
        orgn.y = 0;
        cs_rgb = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    }
}
- (void)windowDidResize:(NSNotification*) notification{
    NSRect winrect = [[self window] frame];
    NSRect contentpane = [[self window] contentRectForFrameRect:winrect];
    NSRect view_rect = NSMakeRect(0, 0, contentpane.size.width, contentpane.size.height);
    [self setFrame:view_rect];
    if (!print_mode) [self setNeedsDisplay:true];
}
- (void)windowUpdate:(NSNotification*) notification{
    if (self) {
        if (!print_mode) [self setNeedsDisplay:true];
    }
}
- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    // establish graphics context
    if (!print_mode) {
        bmw = dirtyRect.size.width;
        bmh = dirtyRect.size.height;
    } else {
        bmw = 72 * 10.5;
        bmh = 72 * 8;
    }
    viewBounds = CGRectMake(0, 0, bmw, bmh);
    [self setFrame:viewBounds];
#if (MAC_OS_X_VERSION >= MAC_OS_X_VERSION_10_15)
    vu_ctx = (__bridge CGContextRef)([NSGraphicsContext CGContext]);
#else  // graphicsPort deprecated in MAC_OS_X_VERSION_10_15
    NSGraphicsContext *currentGrpahicsContext = [NSGraphicsContext currentContext];
    vu_ctx = (CGContextRef)[currentGrpahicsContext graphicsPort];
#endif
    if (vu_ctx && setpage) {
        [[self window] setTitle:file_name];
        // draw plot
        mac_context(&vu_ctx, &viewBounds, cs_rgb);
        file_flag = 1;
        sprintf(option, "-o%d", setpage);
        ViewPlot(option, filename);
        if (!print_mode) {
            [self checkErrorLog];
            if (needs_display) {
                [self setNeedsDisplay:true];
            }
        }
    }
}
- (void)checkErrorLog {
     // check whether pltfile has changed
     NSString *path = [NSString stringWithCString:filename encoding:NSASCIIStringEncoding];
     NSDictionary *attributes = [[NSFileManager defaultManager] attributesOfItemAtPath: path error:nil];
     file_time = [attributes objectForKey: NSFileModificationDate];
     if (![file_time isEqualToDate:elog_time]){
        // check for errors
        int num_errs = errcnt();
        if (num_errs > 0){
            char *plural = (num_errs > 1) ? "s" : "";
            NSString *msgFrmt = @" %d message line%s written to log file";
            NSString *alertMsg = [NSString stringWithFormat:msgFrmt, num_errs, plural];
            NSAlert *alert = [[NSAlert alloc] init];
            [alert addButtonWithTitle:@"View Log"];
            [alert addButtonWithTitle:@"Cancel"];
            [alert setMessageText:@"Alert"];
            [alert setInformativeText:alertMsg];
            [alert setAlertStyle:NSAlertStyleWarning];
            [alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:@selector(alertDidEnd:returnCode:contextInfo:) contextInfo:nil];
        } else {
            [self deleteErrorLog];
        }
        elog_time = file_time;
    }
}
- (void)deleteErrorLog {
    NSString *pltFile = [PlotView getFileName];
    NSString* folder = [pltFile stringByDeletingLastPathComponent];
    NSString *logFile = [folder stringByAppendingPathComponent:@"plt.log"];
    remove([logFile UTF8String]);
}
- (void)alertDidEnd:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo
{
    if (returnCode == NSAlertFirstButtonReturn) {
        NSLog(@"if (returnCode == NSAlertFirstButtonReturn)");
        NSString *pltFile = [PlotView getFileName];
        NSString* folder = [pltFile stringByDeletingLastPathComponent];
        NSString *logFile = [folder stringByAppendingPathComponent:@"plt.log"];
        [[NSWorkspace sharedWorkspace] openFile:logFile withApplication:@"TextEdit"];
    } else if (returnCode == NSAlertSecondButtonReturn) {
        NSLog(@"else if (returnCode == NSAlertSecondButtonReturn)");
    }
}
- (BOOL)knowsPageRange:(NSRangePointer _Nullable )range {
    return YES;
}
- (NSRect)rectForPage:(NSInteger)page {
    CGRect rect= CGRectMake(0, 0, bmw, bmh);
    return (page == 1) ? rect : NSZeroRect;
}

#define limit(x,y,z) (((y)<(x))?x:(((y)>(z))?z:y))

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)keyDown:(NSEvent *)event {
    int page = curpage;
    unsigned short ch = [event keyCode];
    // 116 = page up, 126 = arrow up
    if (ch == 116 || ch == 126) {
        page--;
    } else if (ch == 121 || ch == 125 || ch == 36) { // 121 = page down, 125 = arrow down, 36 = enter
        page++;
    }
    if (curpage != page) {
        curpage = (page < 1) ? 1 : page;
        [self setNeedsDisplay:YES];
    }
}

- (void)mouseDown:(NSEvent *)event {
    NSPoint loc = [self convertPoint:[event locationInWindow] fromView:nil];
    NSInteger clicks = [event clickCount];
    loc.y = bmh - loc.y; // Flip Y coordinate to match internal representation
    
    if (clicks == 2) {
        if (zoom == 1) {
            orgn.x -= loc.x - orgn.x;
            orgn.y -= loc.y - orgn.y;
            zoom = 2;
        } else {
            orgn.x = 0;
            orgn.y = 0;
            zoom = 1;
        }
        [self setNeedsDisplay:YES];
    } else {
        int inf_mode = ([event modifierFlags] & NSEventModifierFlagOption) ? 1 : 0;
        info_line(vu_ctx, loc.x - orgn.x, loc.y - orgn.y, bmw, bmh, inf_mode);
        [self setNeedsDisplay:YES];
    }
}

- (void)rightMouseDown:(NSEvent *)event {
    NSPoint loc = [self convertPoint:[event locationInWindow] fromView:nil];
    loc.y = bmh - loc.y;
    
    float zf = 1.4142136;
    if ([event modifierFlags] & NSEventModifierFlagOption) {
        zf = 1 / zf;
    }
    if (zf < sqrt(1.4142136) / zoom) {
        zf = 1 / zoom;
    }
    
    orgn.x -= (loc.x - orgn.x) * (zf - 1);
    orgn.y -= (loc.y - orgn.y) * (zf - 1);
    zoom *= zf;
    orgn.x -= (loc.x - bmw / 2) / 2;
    orgn.y -= (loc.y - bmh / 2) / 2;
    
    orgn.x = limit(-bmw * (zoom - 1), orgn.x, 0);
    orgn.y = limit(-bmh * (zoom - 1), orgn.y, 0);
    [self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent *)event {
    orgn.x += [event deltaX];
    orgn.y += [event deltaY]; // deltaY is naturally matching because delta Y is inverted in Cocoa events already (down is positive)
    
    orgn.x = limit(-bmw * (zoom - 1), orgn.x, 0);
    orgn.y = limit(-bmh * (zoom - 1), orgn.y, 0);
    [self setNeedsDisplay:YES];
}

@end

//--------------------------------------------------------------------------
static void
view_beg_page(int pagcnt)
{
}

//--------------------------------------------------------------------------
static void
view_end_page(int newpag, int pagcnt)
{
 //   HIViewRef    contentView;

    if (newpag) {
        curpage--;
    }
    if (pagcnt == (1 + curpage - setpage)) {
        if (!print_mode) {
            needs_display++;
        }
    }
}

//----------------------------------------------------------------------------------------------
void
ViewPlot(char *option, char *pltfile)
{
    char    *av[8];
    int     ac = 3;
    CGRect    bnd;

    // process pltfile for viewing
    mac_page(view_beg_page, view_end_page);
    bnd.size.width = bmw * zoom;
    bnd.size.height = bmh * zoom;
    bnd.origin = orgn;
    mac_context(&vu_ctx, &bnd, cs_rgb);
    av[0] = "Plt";
    av[1] = option;
    av[2] = pltfile;
    tplt(ac, (char **)av);
}

//--------------------------------------------------------------------------
static int
get_nums(char *s, int *n, int m)
{
    int i = 0;
    
    while (*s && i < m) {
    while (*s == ' ')
        s++;
        n[i++] = atoi(s);
    while (*s && *s != ' ')
        s++;
    }
    
    return (i);
}

//--------------------------------------------------------------------------
void
epsfix(char *sfn)
{
    FILE *fp1, *fp2;
    char  tfn[MAXPATH];
    char  line[256];
    int    n1 = 14, n2 = 8, nb = 0, np = 0, b[4], p[1];
    static char *s1 = "%%BoundingBox:";
    static char *s2 = "%%Pages:";
    static char *f1 = "%%%%BoundingBox: %d %d %d %d\n";
    static char *f2 = "%%%%Pages: %d\n";
    
    fp1 = fopen(sfn, "rt");
    while (fgets(line, 256, fp1)) {
        if (strncmp(line, s1, n1) == 0) {
            nb = get_nums(line + n1, b, 4);
        } else if (strncmp(line, s2, n2) == 0) {
            np = get_nums(line + n2, p, 1);
        }
    }
    fclose(fp1);
    if (nb == 4 && np == 1) {
    strcpy(tfn, sfn);
    newext(tfn, ".~~~");
        fp1 = fopen(sfn, "rt");
        fp2 = fopen(tfn, "wt");
        while (fgets(line, 256, fp1)) {
            if (strncmp(line, s1, n1) == 0) {
                fprintf(fp2, f1, b[0], b[1], b[2], b[3]);
            } else if (strncmp(line, s2, n2) == 0) {
                fprintf(fp2, f2, p[0]);
            } else {
                fputs(line, fp2);
            }
        }
        fclose(fp1);
        fclose(fp2);
        remove(sfn);
        rename(tfn, sfn);
    }
}

