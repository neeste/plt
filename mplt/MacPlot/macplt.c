/* macplt.c - Mac version of PLT program */

#ifdef NEVER

#include <Carbon/Carbon.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXPATH	    256
#define MAXARGS	    256
#define limit(x,y,z)	(((y)<(x))?x:(((y)>(z))?z:y))

char	*inq_logfn();
int	tplt(int, char **);
int	errcnt();
void	info_line(CGContextRef, float, float, float, float, int);
void	mac_context(CGContextRef *, CGRect *, CGColorSpaceRef);
void	mac_page(void (*)(), void (*)(int, int));
void	newext(char *, char *);
void	outfile(char *);
OSErr   openAppEventHandler(AppleEvent *,AppleEvent *,SInt32);
OSErr   reopenAppEventHandler(AppleEvent *,AppleEvent *,SInt32);
OSErr   openDocsEventHandler(AppleEvent *,AppleEvent *,SInt32);
OSErr   printDocsEventHandler(AppleEvent *,AppleEvent *,SInt32);

static char	    file_flag = 0;
static char	    exit_flag = 0;
static char	    *argv0;
static char	    filename[MAXPATH];
static char	    *edit = "/Applications/TextEdit.app/Contents/MacOS/TextEdit";
static char	    *lgfn = "plt.log";
static char	    *av[MAXARGS];
static char	    defsav[5] = ".eps";
static float	    vx = 0;
static float	    vy = 0;
static float	    zfac = 1.4142136;
static float	    zoom = 1;
static int	    curpage = 0;
static int	    setpage = 0;
static int	    ac = 0;
static int	    bmw = 0;
static int	    bmh = 0;
static int	    file_time = 0;
static void	    *bm_data;
static CGContextRef	bm_ctx;
static CGContextRef	pr_ctx;
static CGContextRef	vu_ctx;
static CGColorSpaceRef	cs_rgb = NULL;
static CGPoint		orgn;
static PMPrintSession	pses;
static PMPrintSettings	pset;
static PMPageFormat	pfmt;
static PMRect		prec;
static WindowRef	window;

//----------------------------------------------------------------------------------------------
static OSStatus 
DrawHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void* inUserData)
{
    static HIRect   viewBounds;

    GetEventParameter( inEvent, kEventParamCGContextRef, typeCGContextRef, NULL, sizeof(CGContextRef), NULL, &vu_ctx );
    HIViewGetBounds((HIViewRef)inUserData, &viewBounds);
    vx = viewBounds.size.width; 
    vy = viewBounds.size.height; 
    if (vx > vy) {
	CGContextConcatCTM(vu_ctx, CGAffineTransformMake(1, 0, 0, -1, 0, vy));
	// origin now at bottom-left and the y axis goes upwards
    }
    CGContextDrawImage(vu_ctx, (CGRect) viewBounds, CGBitmapContextCreateImage(bm_ctx));

    return noErr;
}

//--------------------------------------------------------------------------
static void
view_beg_page(int pagcnt)
{
}

//--------------------------------------------------------------------------
static void
view_end_page(int newpag, int pagcnt)
{
    HIViewRef	contentView;

    if (newpag) {
	curpage--;
    }
    if (pagcnt == (1 + curpage - setpage)) {
	HIViewFindByID(HIViewGetRoot(window), kHIViewWindowContentID, &contentView);
	HIViewSetNeedsDisplay(contentView, true);
    }
}

//----------------------------------------------------------------------------------------------
static void 
ViewPlot(Boolean init)
{
    char	str[20];
    struct stat sb;
    CGRect	bnd;

    if (init) {
	zoom = 1;
	orgn.x = 0;
	orgn.y = 0;
    }	
    // process pltfile for viewing
    mac_page(view_beg_page, view_end_page);
    bnd.size.width = bmw * zoom;
    bnd.size.height = bmh * zoom;
    bnd.origin = orgn;
    mac_context(&bm_ctx, &bnd, cs_rgb);
    setpage = curpage;
    sprintf(str, "-o%d", setpage);
    av[1] = str;
    tplt(ac, av);
    if (stat(filename, &sb) == 0) {
	file_time = sb.st_mtime;
    }
}

//--------------------------------------------------------------------------
static int 
DoMenu(void)
{
    OSStatus            err;
    IBNibRef            nibRef;

    err = CreateNibReference(CFSTR("main"), &nibRef);
    require_noerr( err, CantGetNibRef );
    err = SetMenuBarFromNib(nibRef, CFSTR("MenuBar"));
    require_noerr( err, CantSetMenuBar );
    DisposeNibReference(nibRef);

CantSetMenuBar:
CantGetNibRef:
    return err;
}

// *********************************************************************** 
static void 
EditFile(char *fn)
{
    FSRef		app, file;
    LSLaunchFSRefSpec	rs;

    FSPathMakeRef((UInt8 *) edit, &app, NULL);
    FSPathMakeRef((UInt8 *) fn, &file, NULL);
    rs.appRef = &app;
    rs.numDocs = 1;
    rs.itemRefs = &file;
    rs.passThruParams = NULL;
    rs.launchFlags = kLSLaunchDefaults;
    rs.asyncRefCon = NULL;
    LSOpenFromRefSpec(&rs, NULL);
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
static void
chk_errs()
{
    int num_errs = errcnt();
    if (num_errs > 0) {
	StandardAlert(kAlertPlainAlert, "\pMessages written to PLT log file.", 
	    "\pCommand-L to view log.", NULL, NULL);
    }
}

//--------------------------------------------------------------------------
static void
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

//--------------------------------------------------------------------------
static int
SaveEvent(NavReplyRecord *reply)
{
    char	    *dot;
    char	    spn[MAXPATH];
    char	    sfn[MAXPATH];
    int		    i;
    AEDesc	    dsc;
    FSRef	    ref;
    OSStatus        err;
    static char	    *ext[] = {".eps", ".cgm", ".emf", ".svg"};
    static char	    *opt[] = {"-m5 ", "-m6 ", "-m7 ", "-m8 "};
    static int	    nt = sizeof(ext) / sizeof(ext[0]);

    err = AECoerceDesc(&reply->selection, typeFSRef, &dsc);
    require_noerr(err, CantSave);
    err = AEGetDescData(&dsc, &ref, sizeof(ref));
    require_noerr(err, CantSave);
    FSRefMakePath(&ref,  (UInt8 *) spn, MAXPATH);
    CFStringGetCString(reply->saveFileName, sfn, MAXPATH, 0);
    strcat(spn, "/");
    strcat(spn, sfn);
    if (reply->replacing) {
	remove(spn);
    }
    // process pltfile for export
    dot = strchr(spn, '.');
    for (i = 0; i < nt; i++) {
	if (strncmp(dot, ext[i], 4) == 0) {
	    strcpy(defsav, ext[i]);
	    break;
	}
    }
    if (i < nt) {
	outfile(spn);
	av[1] = opt[i];
	tplt(ac, av);
	if (i == 0) {
	    epsfix(spn);
	}
	ViewPlot(0);
    }
    
CantSave:
    return err;
}

// *********************************************************************** 
static void 
SaveDialogHandler(NavEventCallbackMessage Selector, NavCBRecPtr Parms, void *UD)
{
    OSErr	    err = noErr;
    NavReplyRecord  reply;
    NavUserAction   ua;
    NavDialogRef    dlg;
    
    switch(Selector) {
	case kNavCBUserAction:
	    dlg = Parms->context;
	    err = NavDialogGetReply(dlg, &reply);
	    require_noerr(err, CantNavEvnt);
	    ua = NavDialogGetUserAction(dlg);
	    switch (ua) {
		case kNavUserActionSaveAs:
		    SaveEvent(&reply);
		    break;
	    }
	    NavDisposeReply(&reply);
	    break;
	case kNavCBTerminate:
	    break;	    
    }

CantNavEvnt:
    return;
}

// *********************************************************************** 
static void 
extract_name(char *name, char *path)
{
    char *s;
    
    for (s = path; *s; s++)
	continue;
    while (s > path && s[-1] != '/')
	s--;
    while (*s)
	*name++ = *s++;
    *name = '\0';
}

// *********************************************************************** 
static void 
SaveDialog(void)
{
    char		    sfn[MAXPATH];
    OSErr		    err = noErr;
    NavDialogRef	    ref;
    NavDialogCreationOptions opt;
    NavEventUPP		    upp;
    OSType		    doc = 'plt ';
    OSType		    app = 'Plt ';

    extract_name(sfn, filename);
    newext(sfn, defsav);
    err = NavGetDefaultDialogCreationOptions(&opt);
    require_noerr(err, CantNavGet);
    opt.parentWindow = window;
    opt.modality = kWindowModalityWindowModal;
    opt.saveFileName = CFStringCreateWithCString(NULL, sfn, 0);
    opt.message = CFSTR("Export to eps, emf, cgm, or svg file.");
    upp = NewNavEventUPP(SaveDialogHandler), 
    err = NavCreatePutFileDialog(&opt, doc, app, upp, window, &ref);
    require_noerr(err, CantNavGet);
    err = NavDialogRun(ref);
    require_noerr(err, CantNavGet);
    NavDialogDispose(ref);
    DisposeNavEventUPP(upp);
    
CantNavGet:
    return;
}

//--------------------------------------------------------------------------
static void
prnt_beg_page(int pagcnt)
{
    CGRect	    bnd;

    PMSessionBeginPage(pses, pfmt, NULL);
    PMSessionGetCGGraphicsContext(pses, &pr_ctx);
    PMGetAdjustedPageRect(pfmt, &prec);
    bnd.size.width = prec.right - prec.left;
    bnd.size.height = prec.bottom - prec.top;
    bnd.origin.x = 12;
    bnd.origin.y = -48;
    CGContextConcatCTM(pr_ctx, CGAffineTransformMake(1, 0, 0, -1, 0, bnd.size.height));
    mac_context(&pr_ctx, &bnd, cs_rgb);
    CGContextBeginPage (pr_ctx, (CGRect *) &prec);
}

//--------------------------------------------------------------------------
static void
prnt_end_page(int newpag, int pagcnt)
{
    CGContextEndPage (pr_ctx);
     PMSessionEndPage(pses);
}

// *********************************************************************** 
static void 
PrintDialog(void)
{
    Boolean	    acc;
    PMPrinter	    pr;
    OSStatus        err;

    err = PMCreateSession(&pses);
    require_noerr(err, CantPrintSes);
    err = PMCreatePrintSettings(&pset);
    require_noerr(err, CantPrintSet);
    err = PMSessionDefaultPrintSettings(pses, pset);
    require_noerr(err, CantPrintFmt);
    err = PMSetPageRange(pset, 1, 2);	    // get page range ???
    require_noerr(err, CantPrintFmt);
    err = PMCreatePageFormat(&pfmt);
    require_noerr(err, CantPrintFmt);
    err = PMSessionDefaultPageFormat(pses, pfmt);
    require_noerr(err, CantPrint);
    err = PMSessionPrintDialog(pses, pset, pfmt, &acc);
    require_noerr(err, CantPrint);
    // process pltfile for printing
    PMSessionGetCurrentPrinter(pses, &pr);
    PMSessionBeginCGDocument(pses, pset, pfmt);
    mac_page(prnt_beg_page, prnt_end_page);
    av[1] = "-o1";
    tplt(ac, av);
    PMSessionEndDocument(pses);
    ViewPlot(0);

CantPrint:
    PMRelease(pfmt);
CantPrintFmt:
    PMRelease(pset);
CantPrintSet:
    PMRelease(pses);
CantPrintSes:
    return;
}

// *********************************************************************** 
static  OSStatus  
WindowCmdHandler(EventHandlerCallRef nextHandler, EventRef inEvent, void *userData)
{
    OSStatus        err = noErr;
    HICommand	    command;
    
    GetEventParameter( inEvent, kEventParamDirectObject, typeHICommand, NULL, sizeof(HICommand), NULL, &command );
    switch ( command.commandID ) {
	case 'edit':
	    EditFile(filename);
	    break;
	case kHICommandSave:
	    SaveDialog();
	    break;
	case kHICommandPrint:
	    PrintDialog();
	    break;
	case 'redo':
	    ViewPlot(1);
	    break;
	default:
	    err = CallNextEventHandler(nextHandler, inEvent);
    }
    
    return(err);
}

// *********************************************************************** 
static  OSStatus  
WindowCloseHandler(EventHandlerCallRef nextHandler, EventRef inEvent, void *userData)
{
    OSStatus        err = noErr;
    
    file_flag = 0;
    file_time = 0;
    if (bm_ctx) {
	CGContextRelease(bm_ctx);
	bm_ctx = NULL;
    }
    if (bm_data) {
	free(bm_data);
	bm_data = NULL;
    }
    err = CallNextEventHandler(nextHandler, inEvent);
    
    return(err);
}

// *********************************************************************** 
static  OSStatus  
WindowResizeHandler(EventHandlerCallRef nextHandler, EventRef inEvent, void *userData)
{
    OSStatus        err = noErr;
    HIViewRef	    contentView;

    HIViewFindByID(HIViewGetRoot(window), kHIViewWindowContentID, &contentView);
    HIViewSetNeedsDisplay(contentView, true);

    return(err);
}

//----------------------------------------------------------------------------------------------
static OSStatus 
KeyHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void* inUserData)
{
    int page;
    UInt8 ch;
    OSStatus err; 
    
    err = GetEventParameter(inEvent, kEventParamKeyMacCharCodes, typeChar, NULL,
			    sizeof(char), NULL, &ch );
    require_noerr( err, CantGetKey);
    page = curpage;
    switch (ch) {
	case 11:	    // PageUp
	    page--;
	    break;
	case '\r':	    // Enter
	case 12:	    // PageDn
	    page++;
	    break;
    }
    if (curpage != page) {
	curpage = (page < 1) ? 1 : page;
	ViewPlot(1);
    }
CantGetKey:
    CallNextEventHandler(inHandlerCallRef, inEvent); 
    return err;
}

//----------------------------------------------------------------------------------------------
static OSStatus 
MouseHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void* inUserData)
{
    float	zf;
    int		inf_mode;
    HIViewRef	contentView;
    UInt16	mb;
    UInt32	km;
    UInt32	cc;
    OSStatus	err;
    CGPoint	loc;
    static float    status_bar_height = 20;
    
    err = GetEventParameter(inEvent, kEventParamWindowMouseLocation, typeHIPoint, NULL,
			    sizeof(loc), NULL, &loc );
    require_noerr( err, CantGetMouse);
    err = GetEventParameter(inEvent, kEventParamMouseButton, typeMouseButton, NULL,
			    sizeof(mb), NULL, &mb );
    require_noerr( err, CantGetMouse);
    err = GetEventParameter(inEvent, kEventParamClickCount, typeUInt32, NULL,
			    sizeof(cc), NULL, &cc );
    require_noerr( err, CantGetMouse);
    err = GetEventParameter(inEvent, kEventParamKeyModifiers, typeUInt32, NULL,
			    sizeof(km), NULL, &km );
    require_noerr( err, CantGetMouse);
    loc.y = bmh - (loc.y - status_bar_height);
    if ((mb == 2)			// right-button
	|| (mb == 1 && (km & 256))) {	// alt-left-button
	zf = (km & 512) ? 1 / zfac : zfac;
	if (zf < sqrt(zfac) / zoom) {
	    zf = 1 / zoom;
	}
	orgn.x -= (loc.x - orgn.x) * (zf - 1);	    // zoom in on mouse location
	orgn.y -= (loc.y - orgn.y) * (zf - 1);
	zoom *= zf;
	orgn.x -= (loc.x - bmw / 2) / 2;	    // move toward window center
	orgn.y -= (loc.y - bmh / 2) / 2;
	orgn.x = limit(-bmw * (zoom - 1), orgn.x, 0);
	orgn.y = limit(-bmh * (zoom - 1), orgn.y, 0);
	ViewPlot(0);
    } else if (mb == 1) {
	if (loc.y > vy) {
	    ViewPlot(0);
	} else {
	    inf_mode = (km & 4096) ? 2 : (km & 512) ? 1 : 0;
	    info_line(bm_ctx, loc.x - orgn.x, loc.y - orgn.y, vx, vy, inf_mode);
	    HIViewFindByID(HIViewGetRoot(window), kHIViewWindowContentID, &contentView);
	    HIViewSetNeedsDisplay(contentView, true);
	}
        if (cc == 2) {
	    if (zoom == 1) {
		orgn.x -= loc.x - orgn.x;	    // zoom in on mouse location
		orgn.y -= loc.y - orgn.y;
		zoom = 2;
	    } else {
		orgn.x = 0;
		orgn.y = 0;
		zoom = 1;
	    }
	    ViewPlot(0);
	}
    }

CantGetMouse:
    CallNextEventHandler(inHandlerCallRef, inEvent); 
    return err;
}

//--------------------------------------------------------------------------
static int
OpenPlot(AEDescList docList)
{
    long	    index, count;
    FSRef	    theFSRef;
    EventTypeSpec   closeEvent	= {kEventClassWindow, kEventWindowClose};
    EventTypeSpec   resizeEvent = {kEventClassWindow, kEventWindowBoundsChanging};
    EventTypeSpec   cmdEvent	= {kEventClassCommand, kEventCommandProcess};
    EventTypeSpec   drawEvent	= {kEventClassControl, kEventControlDraw};
    EventTypeSpec   keyEvent	= {kEventClassKeyboard, kEventRawKeyDown};
    EventTypeSpec   mouseEvent	= {kEventClassMouse, kEventMouseDown};
    HIViewRef       contentView;
    IBNibRef        nibRef;
    OSStatus        err = noErr;
    Rect	    win_rect;

    curpage = 1;
    ac = 0;
    av[ac++] = argv0;
    av[ac++] = "-o1";
    err = AECountItems(&docList, &count);
    require_noerr(err, CantOpenPlot);
    for (index = 1; index <= count; index++) {
        err = AEGetNthPtr(&docList,index,typeFSRef,NULL,NULL,&theFSRef,
	  sizeof(theFSRef),NULL);
	require_noerr(err, CantOpenPlot); 
	FSRefMakePath(&theFSRef,  (UInt8 *) filename, MAXPATH);
	av[ac++] = filename;
    }
    err = CreateNibReference(CFSTR("main"), &nibRef);
    require_noerr(err, CantGetNibRef);
    err = CreateWindowFromNib(nibRef, CFSTR("MainWindow"), &window);
    require_noerr(err, CantCreateWindow);
    DisposeNibReference(nibRef);
    GetWindowBounds(window, kWindowGlobalPortRgn, &win_rect);
    bmw = win_rect.right - win_rect.left;
    bmh = win_rect.bottom - win_rect.top;
    bm_data = (void *) calloc(bmw * bmh, 2);
    bm_ctx = CGBitmapContextCreate(bm_data, bmw, bmh, 5, bmw * 2, cs_rgb, kCGImageAlphaNoneSkipFirst);
    // Install DrawHandler on contentView
    HIViewFindByID(HIViewGetRoot(window), kHIViewWindowContentID, &contentView);
    err = InstallEventHandler(HIViewGetEventTarget(contentView), DrawHandler, 1, &drawEvent, contentView, NULL);
    require_noerr(err, CantInstallHandler);
    // Install Window Event handlers
    err = InstallWindowEventHandler(window, NewEventHandlerUPP(WindowCmdHandler), 1, &cmdEvent, window, NULL);
    require_noerr( err, CantInstallHandler);
    err = InstallWindowEventHandler(window, NewEventHandlerUPP(WindowCloseHandler), 1, &closeEvent, window, NULL);
    require_noerr( err, CantInstallHandler);
    err = InstallWindowEventHandler(window, NewEventHandlerUPP(WindowResizeHandler), 1, &resizeEvent, window, NULL);
    require_noerr( err, CantInstallHandler);
    err = InstallEventHandler(GetWindowEventTarget(window), KeyHandler, 1, &keyEvent, window, NULL);
    require_noerr(err, CantInstallHandler);
    err = InstallEventHandler(GetWindowEventTarget(window), MouseHandler, 1, &mouseEvent, window, NULL);
    require_noerr(err, CantInstallHandler);
    SetWindowTitleWithCFString(window, CFStringCreateWithCString(NULL, filename, kCFStringEncodingASCII));
    
    file_flag = 1;
    ViewPlot(1);
    ShowWindow(window);

CantInstallHandler:
CantCreateWindow:
CantGetNibRef:
CantOpenPlot:
    return(err);
}

// *********************************************************************** 
static OSErr 
OpenApplicationAE(AppleEvent *appEvent,AppleEvent *reply,SInt32 handlerRefcon)
{
    return noErr;
}

// *********************************************************************** 
static OSErr 
ReopenApplicationAE(AppleEvent *appEvent,AppleEvent *reply,SInt32 handlerRefcon)
{
    return noErr;
}

// *********************************************************************** 
static OSErr 
QuitApplicationAE(AppleEvent *appEvent,AppleEvent *reply,SInt32 handlerRefcon)
{
    file_flag = 0;
    exit_flag = 1;
    QuitApplicationEventLoop();
    return noErr;
}

// *********************************************************************** 
static OSErr 
OpenDocumentsAE(AppleEvent *appEvent,AppleEvent *reply,SInt32 handlerRefcon)
{
    AEDescList	docList;
    OSErr	err;

    err = AEGetParamDesc(appEvent,keyDirectObject,typeAEList,&docList);
    require_noerr(err, CantOpenDocuments);
    OpenPlot(docList);

CantOpenDocuments:
    return err;
}

// *********************************************************************** 
static OSErr 
OpenContentsAE(AppleEvent *appEvent,AppleEvent *reply,SInt32 handlerRefcon){
    AEDescList	docList;
    OSErr	err;
    
    err = AEGetParamDesc(appEvent,keyDirectObject,typeAEList,&docList);
    require_noerr(err, CantOpenContents);
    OpenPlot(docList);

CantOpenContents:
    return err;
}

// *********************************************************************** 
static OSErr 
PrintDocumentsAE(AppleEvent *appEvent,AppleEvent *reply,SInt32 handlerRefcon)
{
    return noErr;
}

// *********************************************************************** 
static void 
NewFile(void)
{
    FSRef		app;
    LSLaunchFSRefSpec	rs;
    
    FSPathMakeRef((UInt8 *) edit, &app, NULL);
    rs.appRef = &app;
    rs.numDocs = 0;
    rs.itemRefs = NULL;
    rs.passThruParams = NULL;
    rs.launchFlags = kLSLaunchDefaults;
    rs.asyncRefCon = NULL;
    LSOpenFromRefSpec(&rs, NULL);
}

// *********************************************************************** 
static void 
OpenDialogHandler(NavEventCallbackMessage Selector, NavCBRecPtr Parms, void *UD)
{
    OSErr err = noErr;
    NavReplyRecord reply;
    NavUserAction ua;
    NavDialogRef dlg;

    switch(Selector) {
	case kNavCBUserAction:
	    dlg = Parms->context;
	    err = NavDialogGetReply(dlg, &reply);
	    require_noerr(err, CantNavEvnt);
	    ua = NavDialogGetUserAction(dlg);
	    switch (ua) {
		case kNavUserActionOpen:
		    OpenPlot(reply.selection);
		    break;
	    }
	    NavDisposeReply(&reply);
	    break;
	case kNavCBTerminate:
	    break;	    
    }
    
CantNavEvnt:
    return;
}

// *********************************************************************** 
static void 
OpenDialog(void)
{
    NavDialogRef		ref;
    NavDialogCreationOptions	opt;
    NavEventUPP			upp;
    OSErr			err = noErr;
    
    err = NavGetDefaultDialogCreationOptions(&opt);
    require_noerr(err, CantNavGet);  
    upp = NewNavEventUPP(OpenDialogHandler), 
    err = NavCreateGetFileDialog(&opt, NULL, upp, NULL, NULL, NULL, &ref);
    require_noerr(err, CantNavGet);
    err = NavDialogRun(ref);
    require_noerr(err, CantNavGet);
    NavDialogDispose(ref);
    DisposeNavEventUPP(upp);
    chk_errs();
    
CantNavGet:
    return;
}

// *********************************************************************** 
static  OSStatus  
CommandProcessHandler( EventHandlerCallRef nextHandler, EventRef inEvent, void *userData )
{
    OSErr       err = noErr;
    HICommand   command;
    
    GetEventParameter( inEvent, kEventParamDirectObject, typeHICommand, NULL, sizeof(HICommand), NULL, &command );
    switch ( command.commandID ) {
	case kHICommandNew:
	    NewFile();
	    break;
	case kHICommandOpen:
	    OpenDialog();
	    break;
	case kHICommandQuit:
	    file_flag = 0;
	    exit_flag = 1;
	    QuitApplicationEventLoop();
	    break;
	case 'logf':
	    lgfn = inq_logfn();
	    if (access(lgfn, 0) == 0) {
		EditFile(lgfn);
	    }
	    break;
	default:
	    err = CallNextEventHandler(nextHandler, inEvent);
    }

    return(err);
}

// *********************************************************************** 
static  OSStatus  
AppActivateHandler( EventHandlerCallRef nextHandler, EventRef inEvent, void *userData )
{
    OSErr       err = noErr;
    struct stat sb;

    if (file_flag && file_time) {
	if (stat(filename, &sb) == 0) {
	    if (sb.st_mtime != file_time) {
		ViewPlot(0);
		chk_errs();
	    }
	}
    }

    return(err);
}

// *********************************************************************** 
static OSErr 
InstallEventHandlers(void)
{
    OSErr		err;
    const EventTypeSpec cmd[] = { { kEventClassCommand, kEventCommandProcess } };
    const EventTypeSpec app[] = { { kEventClassApplication, kEventAppActivated } };

    // Install Apple events
    err = AEInstallEventHandler(kCoreEventClass, kAEOpenApplication,
        NewAEEventHandlerUPP((AEEventHandlerProcPtr) OpenApplicationAE), 0, false);
    require_noerr(err, CantInstallAppleEventHandler);   
    err = AEInstallEventHandler(kCoreEventClass, kAEReopenApplication,
	NewAEEventHandlerUPP((AEEventHandlerProcPtr) ReopenApplicationAE), 0, false);
    require_noerr(err, CantInstallAppleEventHandler);    
    err = AEInstallEventHandler(kCoreEventClass, kAEQuitApplication,
	NewAEEventHandlerUPP((AEEventHandlerProcPtr) QuitApplicationAE), 0, false);
    require_noerr(err, CantInstallAppleEventHandler);    
    err = AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments,
        NewAEEventHandlerUPP((AEEventHandlerProcPtr) OpenDocumentsAE), 0, false);
    require_noerr(err, CantInstallAppleEventHandler);
    err = AEInstallEventHandler(kCoreEventClass, kAEOpenContents,
        NewAEEventHandlerUPP((AEEventHandlerProcPtr) OpenContentsAE), 0, false);
    require_noerr(err, CantInstallAppleEventHandler);
    err = AEInstallEventHandler(kCoreEventClass, kAEPrintDocuments,
        NewAEEventHandlerUPP((AEEventHandlerProcPtr) PrintDocumentsAE), 0, false);
    require_noerr(err, CantInstallAppleEventHandler);

    // Install Application events
    InstallApplicationEventHandler(NewEventHandlerUPP(CommandProcessHandler), 
	GetEventTypeCount(cmd), cmd, NULL, NULL );
    InstallApplicationEventHandler(NewEventHandlerUPP(AppActivateHandler), 
	GetEventTypeCount(app), app, NULL, NULL );

CantInstallAppleEventHandler: 
    return(err);
}

//--------------------------------------------------------------------------
static void
RegisterHelp()
{
    CFBundleRef	    bnd	;
    CFURLRef	    url;
    FSRef	    ref;
    OSStatus	    err;

    bnd = CFBundleGetMainBundle();
    if (bnd == NULL) goto CantRegisterHelp;
    url = CFBundleCopyBundleURL(bnd);
    if (url == NULL) goto CantRegisterHelp;
    if (!CFURLGetFSRef(url, &ref)) goto CantRegisterHelp; 
    err = AHRegisterHelpBook(&ref);
    require_noerr(err, CantRegisterHelp);

CantRegisterHelp:
    if (url) CFRelease(url);
}

//--------------------------------------------------------------------------
int 
main(int argc, char* argv[])
{
    // save pointer to first arg
    argv0 = argv[0];
    // initialialize app
    cs_rgb = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    InstallEventHandlers();    
    DoMenu();
    RegisterHelp();
    // start event loop
    RunApplicationEventLoop();

    return noErr;
}

#endif // NEVER
