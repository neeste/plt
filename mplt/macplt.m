//
//  macplt.m
//  Single-file programmatic Cocoa App for PLT
//

#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CoreGraphics.h>

#define MAXPATH 1024

// --- Global variables from out ---core
char    *inq_logfn(void);
int     tplt(int, char **);
int     errcnt(void);
void    info_line(CGContextRef, float, float, float, float, int);
void    mac_context(CGContextRef *, CGRect *, CGColorSpaceRef);
void    mac_page(void(*)(int), void(*)(int, int));
void    newext(char *, char *);
void    outfile(char *);
void    ViewPlot(char *, char *);
void    epsfix(char *);

// Global State
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
static NSDate         *elog_time = nil;
static NSString       *file_name = @"";

static char         ppn[MAXPATH] = "";
static char         spn[MAXPATH] = "";
static char         *ext[] = {".eps", ".cgm", ".emf", ".svg"};
static char         *opt[] = {"-m5 ", "-m6 ", "-m7 ", "-m8 "};
static int          nt = sizeof(ext) / sizeof(ext[0]);
static NSString     *def_ext = @"eps";
static NSString     *editApp = @"TextEdit";

// --- PlotView Interface ---
@interface PlotView : NSView
+ (NSString *_Nonnull)getFileName;
+ (void)setFileName:(NSString *_Nullable)nameFile;
+ (NSSize)setPrintMode:(BOOL)mode;
- (BOOL)knowsPageRange:(NSRangePointer _Nullable )range;
- (NSRect)rectForPage:(NSInteger)page;
@end

// --- AppDelegate Interface ---
@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@property (strong) NSWindow *window;
@property (strong) PlotView *plotView;
- (void)openPlot:(NSString *)filename;
- (IBAction)chooseFile:(id)sender;
- (IBAction)editFile:(id)sender;
- (IBAction)showHelp:(id)sender;
- (IBAction)exportFile:(id)sender;
- (IBAction)editFile:(id)sender;
- (IBAction)printFile:(id)sender;
- (IBAction)errorLog:(id)sender;
@end


// --- PlotView Implementation ---
@implementation PlotView

+ (NSString *_Nonnull)getFileName {
    return [NSString stringWithUTF8String:filename];
}
+ (void)setFileName:(NSString *_Nullable)nameFile {
    if (file_name != nameFile) {
        [file_name release];
        file_name = [nameFile retain];
    }
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
    if (!print_mode) {
        bmw = dirtyRect.size.width;
        bmh = dirtyRect.size.height;
    } else {
        bmw = 72 * 10.5;
        bmh = 72 * 8;
    }
    viewBounds = CGRectMake(0, 0, bmw, bmh);
    [self setFrame:viewBounds];
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= 101500)
    vu_ctx = (__bridge CGContextRef)([[NSGraphicsContext currentContext] CGContext]);
#else
    NSGraphicsContext *currentGrpahicsContext = [NSGraphicsContext currentContext];
    vu_ctx = (CGContextRef)[currentGrpahicsContext graphicsPort];
#endif
    if (vu_ctx && setpage) {
        [[self window] setTitle:file_name];
        mac_context(&vu_ctx, &viewBounds, cs_rgb);
        file_flag = 1;
        sprintf(option, "-o%d", setpage);
        ViewPlot(option, filename);
        if (!print_mode) {
            [self checkErrorLog];
            if (needs_display) {
                needs_display = 0;
                [self setNeedsDisplay:true];
            }
        }
    }
}
- (void)checkErrorLog {
     NSString *path = [NSString stringWithCString:filename encoding:NSASCIIStringEncoding];
     NSDictionary *attributes = [[NSFileManager defaultManager] attributesOfItemAtPath: path error:nil];
     NSDate *current_file_time = [attributes objectForKey: NSFileModificationDate];
     if (![current_file_time isEqualToDate:elog_time]){
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
            [alert beginSheetModalForWindow:[self window] completionHandler:^(NSModalResponse returnCode) {
                if (returnCode == NSAlertFirstButtonReturn) {
                    NSString *pltFile = [PlotView getFileName];
                    NSString* folder = [pltFile stringByDeletingLastPathComponent];
                    NSString *logFile = [folder stringByAppendingPathComponent:@"plt.log"];
                    [[NSWorkspace sharedWorkspace] openFile:logFile withApplication:@"TextEdit"];
                }
            }];
        } else {
            [self deleteErrorLog];
        }
        if (elog_time != current_file_time) {
            [elog_time release];
            elog_time = [current_file_time retain];
        }
    }
}
- (void)deleteErrorLog {
    NSString *pltFile = [PlotView getFileName];
    NSString* folder = [pltFile stringByDeletingLastPathComponent];
    NSString *logFile = [folder stringByAppendingPathComponent:@"plt.log"];
    remove([logFile UTF8String]);
}
- (BOOL)knowsPageRange:(NSRangePointer _Nullable )range { return YES; }
- (NSRect)rectForPage:(NSInteger)page {
    CGRect rect= CGRectMake(0, 0, bmw, bmh);
    return (page == 1) ? rect : NSZeroRect;
}

#define limit(x,y,z) (((y)<(x))?x:(((y)>(z))?z:y))

- (BOOL)acceptsFirstResponder { return YES; }

- (void)keyDown:(NSEvent *)event {
    int page = curpage;
    unsigned short ch = [event keyCode];
    if (ch == 116 || ch == 126) {
        page--;
    } else if (ch == 121 || ch == 125 || ch == 36) {
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
    loc.y = bmh - loc.y;
    
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
    orgn.y += [event deltaY];
    orgn.x = limit(-bmw * (zoom - 1), orgn.x, 0);
    orgn.y = limit(-bmh * (zoom - 1), orgn.y, 0);
    [self setNeedsDisplay:YES];
}
@end

// --- C Helper Functions ---
static void view_beg_page(int pagcnt) {}
static void view_end_page(int newpag, int pagcnt) {
    if (newpag) curpage--;
    if (pagcnt == (1 + curpage - setpage)) {
        if (!print_mode) needs_display++;
    }
}
void ViewPlot(char *option, char *pltfile) {
    if (option == NULL || pltfile == NULL) {
        return;
    }
    char    *av[8];
    int     ac = 3;
    CGRect    bnd;
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

static int get_nums(char *s, int *n, int m) {
    int i = 0;
    while (*s && i < m) {
        while (*s == ' ') s++;
        n[i++] = atoi(s);
        while (*s && *s != ' ') s++;
    }
    return (i);
}

void epsfix(char *sfn) {
    FILE *fp1, *fp2;
    char  tfn[MAXPATH];
    char  line[256];
    int    n1 = 14, n2 = 8, nb = 0, np = 0, b[4], p[1];
    static char *s1 = "%%BoundingBox:";
    static char *s2 = "%%Pages:";
    static char *f1 = "%%%%BoundingBox: %d %d %d %d\n";
    static char *f2 = "%%%%Pages: %d\n";
    
    fp1 = fopen(sfn, "rt");
    if (!fp1) return;
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

// --- AppDelegate Implementation ---
@implementation AppDelegate

- (void)openDocument:(id)sender {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setCanChooseFiles:YES];
    [panel setCanChooseDirectories:NO];
    [panel setAllowsMultipleSelection:NO];
    [panel setAllowedFileTypes:@[@"plt"]];
    
    if ([panel runModal] == NSModalResponseOK) {
        NSURL *url = [[panel URLs] firstObject];
        if (url) {
            NSString *path = [url path];
            [self openPlot:path];
        }
    }
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSArray *args = [[NSProcessInfo processInfo] arguments];
    
    // Check if a file was passed as argument.
    // If the app is launched from Finder, the second argument might be -psn_...
    // We should ignore -psn arguments.
    NSString *fileToOpen = @"";
    if (args.count > 1) {
        NSString *arg = args[1];
        if (![arg hasPrefix:@"-psn"]) {
            fileToOpen = arg;
        }
    }
    
    if (fileToOpen.length > 0) {
        [self openPlot:fileToOpen];
    } else if ([[PlotView getFileName] length] == 0) {
        [self openPlot:@""];
    }
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}
- (void)application:(NSApplication *)sender openFiles:(NSArray<NSString *> *)filenames {
    for(NSString *file in filenames) {
        [self openPlot:file];
    }
}
- (BOOL)application:(NSApplication *)sender openFile:(NSString *)filename {
    [self openPlot:filename];
    return YES;
}
- (IBAction)chooseFile:(id)sender {
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg beginWithCompletionHandler:^(NSInteger result){
        if (result == NSModalResponseOK) {
            NSArray<NSURL *> *urls = [openDlg URLs];
            for(NSURL *url in urls) {
                [self openPlot:url.path];
            }
        }
    }];
}
- (void)openPlot:(NSString *)filename {
    [PlotView setFileName:filename];
    if (!self.window) {
        NSRect frame = NSMakeRect(0, 0, 1050, 800);
        self.window = [[NSWindow alloc] initWithContentRect:frame
                                                  styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable
                                                    backing:NSBackingStoreBuffered
                                                      defer:NO];
        self.window.title = @"PLT";
        self.plotView = [[PlotView alloc] initWithFrame:frame];
        self.window.contentView = self.plotView;
        [self.window setDelegate:self];
        [self.window setAcceptsMouseMovedEvents:YES];
    }
    [self.window makeKeyAndOrderFront:nil];
    [self.plotView setNeedsDisplay:YES];
    NSDocumentController *dc = [NSDocumentController sharedDocumentController];
    [dc noteNewRecentDocumentURL:[NSURL fileURLWithPath:filename]];
}
- (IBAction)exportFile:(id)sender {
    NSString *pltFile = [PlotView getFileName];
    if (pltFile.length == 0) return;
    NSArray<NSString *> *extensions = @[@"eps", @"cgm", @"emf", @"svg"];
    NSString* folder = [pltFile stringByDeletingLastPathComponent];
    NSString* filename = pltFile.lastPathComponent;
    NSString* newFile = [[filename stringByDeletingPathExtension] stringByAppendingPathExtension:def_ext];
    NSURL *folderURL = [NSURL fileURLWithPath:folder];
    strncpy(ppn, [pltFile UTF8String], MAXPATH);

    NSSavePanel* panel = [NSSavePanel savePanel];
    [panel setTitle:@"Export"];
    [panel setNameFieldStringValue:newFile];
    [panel setDirectoryURL:folderURL];
    [panel setShowsTagField:false];
    [panel setAllowedFileTypes:extensions];
    [panel beginWithCompletionHandler:^(NSInteger result){
        if (result == NSModalResponseOK) {
            NSURL*  theURL = [panel URL];
            NSString *thePath = [theURL path];
            strncpy(spn, [thePath UTF8String], MAXPATH);
            char *dot = strchr(spn, '.');
            int select;
            for (select = 0; select < nt; select++) {
                if (strncmp(dot, ext[select], 4) == 0) {
                    break;
                }
            }
            if (select < nt) {
                outfile(spn);
                mac_page(NULL, NULL);
                ViewPlot(opt[select], ppn);
                if (select == 0) epsfix(spn);
                def_ext = extensions[select];
            }
        }
    }];
}
- (IBAction)editFile:(id)sender {
    NSString *pltFile = [PlotView getFileName];
    if (pltFile.length > 0) {
        [[NSWorkspace sharedWorkspace] openFile:pltFile withApplication:editApp];
    }
}
- (IBAction)showHelp:(id)sender {
    NSString *helpPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"Plt.help/Contents/Resources/PltHelp.html"];
    if ([[NSFileManager defaultManager] fileExistsAtPath:helpPath]) {
        [[NSWorkspace sharedWorkspace] openFile:helpPath];
    }
}
- (IBAction)printFile:(id)sender {
    if (!self.plotView) return;
    PlotView *pltView = [[PlotView alloc] init];
    NSRect rect = NSMakeRect(0,0,10.5*72,8*72);
    NSSize size = NSMakeSize(8*72,10.5*72);
    [PlotView setPrintMode:YES];
    NSPrintInfo *printInfo = [NSPrintInfo sharedPrintInfo];
    [printInfo setOrientation:NSPaperOrientationLandscape];
    [printInfo setVerticallyCentered:YES];
    [printInfo setHorizontallyCentered:YES];
    [printInfo setPaperSize:size];
    [pltView drawRect:rect];
    NSPrintOperation *op = [NSPrintOperation printOperationWithView:pltView printInfo:printInfo];
    [op setShowsPrintPanel:YES];
    [op runOperation];
    [PlotView setPrintMode:NO];
}
- (IBAction)errorLog:(id)sender {
    NSString *pltFile = [PlotView getFileName];
    if (pltFile.length > 0) {
        NSString* folder = [pltFile stringByDeletingLastPathComponent];
        NSString *logFile = [folder stringByAppendingPathComponent:@"plt.log"];
        [[NSWorkspace sharedWorkspace] openFile:logFile withApplication:editApp];
    }
}
@end


// --- Programmatic Main ---
int main(int argc, const char * argv[]) {
    static AppDelegate *strongDelegate = nil;
    @autoreleasepool {
        NSApplication *app = [NSApplication sharedApplication];
        [app setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        AppDelegate *delegate = [[AppDelegate alloc] init];
        strongDelegate = delegate;
        app.delegate = delegate;
        
        // Build the Menu
        NSMenu *menubar = [[NSMenu alloc] init];
        [app setMainMenu:menubar];
        
        // 1. App Menu
        NSMenuItem *appMenuItem = [[NSMenuItem alloc] init];
        [menubar addItem:appMenuItem];
        NSMenu *appMenu = [[NSMenu alloc] init];
        [appMenuItem setSubmenu:appMenu];
        
        NSMenuItem *aboutMenuItem = [[NSMenuItem alloc] initWithTitle:@"About Plt" action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];
        [appMenu addItem:aboutMenuItem];
        
        NSMenuItem *helpMenuItem = [[NSMenuItem alloc] initWithTitle:@"Plt Help" action:@selector(showHelp:) keyEquivalent:@"?"];
        [helpMenuItem setTarget:delegate];
        [appMenu addItem:helpMenuItem];
        
        [appMenu addItem:[NSMenuItem separatorItem]];
        NSMenuItem *quitMenuItem = [[NSMenuItem alloc] initWithTitle:@"Quit Plt" action:@selector(terminate:) keyEquivalent:@"q"];
        [appMenu addItem:quitMenuItem];
        
        // 2. File Menu
        NSMenuItem *fileMenuItem = [[NSMenuItem alloc] init];
        [menubar addItem:fileMenuItem];
        NSMenu *fileMenu = [[NSMenu alloc] initWithTitle:@"File"];
        [fileMenuItem setSubmenu:fileMenu];
        
        NSMenuItem *openMenuItem = [[NSMenuItem alloc] initWithTitle:@"Open..." action:@selector(chooseFile:) keyEquivalent:@"o"];
        [openMenuItem setTarget:delegate];
        [fileMenu addItem:openMenuItem];
        
        NSMenuItem *editMenuItem = [[NSMenuItem alloc] initWithTitle:@"Edit" action:@selector(editFile:) keyEquivalent:@"e"];
        [editMenuItem setTarget:delegate];
        [fileMenu addItem:editMenuItem];
        
        [fileMenu addItem:[NSMenuItem separatorItem]];
        NSMenuItem *printMenuItem = [[NSMenuItem alloc] initWithTitle:@"Print..." action:@selector(print:) keyEquivalent:@"p"];
        [fileMenu addItem:printMenuItem];
        
        // 3. Edit Menu
        NSMenuItem *editMenuMenuItem = [[NSMenuItem alloc] init];
        [menubar addItem:editMenuMenuItem];
        NSMenu *editMenu = [[NSMenu alloc] initWithTitle:@"Edit"];
        [editMenuMenuItem setSubmenu:editMenu];
        
        [editMenu addItem:[[NSMenuItem alloc] initWithTitle:@"Undo" action:@selector(undo:) keyEquivalent:@"z"]];
        [editMenu addItem:[[NSMenuItem alloc] initWithTitle:@"Redo" action:@selector(redo:) keyEquivalent:@"Z"]];
        [editMenu addItem:[NSMenuItem separatorItem]];
        [editMenu addItem:[[NSMenuItem alloc] initWithTitle:@"Cut" action:@selector(cut:) keyEquivalent:@"x"]];
        [editMenu addItem:[[NSMenuItem alloc] initWithTitle:@"Copy" action:@selector(copy:) keyEquivalent:@"c"]];
        [editMenu addItem:[[NSMenuItem alloc] initWithTitle:@"Paste" action:@selector(paste:) keyEquivalent:@"v"]];
        [editMenu addItem:[[NSMenuItem alloc] initWithTitle:@"Delete" action:@selector(delete:) keyEquivalent:@""]];
        [editMenu addItem:[[NSMenuItem alloc] initWithTitle:@"Select All" action:@selector(selectAll:) keyEquivalent:@"a"]];
        
        NSLog(@"starting [app run]");
        [app run];
        NSLog(@"finished [app run]");
    }
    return 0;
}
