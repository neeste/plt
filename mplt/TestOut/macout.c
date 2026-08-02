/*
 File:	macout.c
*/

#include <CoreGraphics/CoreGraphics.h>

int	tstout(int, char **);
void	mac_context(CGContextRef *, CGRect *, CGColorSpaceRef);

//static CGColorSpaceRef	cs_rgb = NULL;

//----------------------------------------------------------------------------------------------
#ifdef NEVER
OSStatus
DrawHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void* inUserData)
{
    CGContextRef	ctx;
    HIRect		viewBounds;

    GetEventParameter(inEvent, kEventParamCGContextRef, typeCGContextRef, NULL, sizeof(CGContextRef), NULL, &ctx);
    CallNextEventHandler(inHandlerCallRef, inEvent);    // this erases the view
    HIViewRef   theView = (HIViewRef)inUserData; 
    HIViewGetBounds(theView, &viewBounds);
    if (viewBounds.size.width > viewBounds.size.height) {
	CGContextConcatCTM(ctx, CGAffineTransformMake(1, 0, 0, -1, 0, viewBounds.size.height));
	// origin now at bottom-left and the y axis goes upwards
    }

    mac_context(&ctx, (CGRect *) &viewBounds, cs_rgb);
    tstout(0, NULL);
    
    return noErr;
}
#endif
//----------------------------------------------------------------------------------------------
#ifdef NEVER
OSStatus
KeyHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void* inUserData)
{
    HIViewRef	contentView;
    OSStatus	err;
    UInt8	ch;
    WindowRef	w;

    err = GetEventParameter(inEvent, kEventParamKeyMacCharCodes, typeChar, NULL, 1, NULL, &ch);
    require_noerr(err, CantHandleKey);
    w = (WindowRef) inUserData;
    HIViewFindByID(HIViewGetRoot(w), kHIViewWindowContentID, &contentView);
    HIViewSetNeedsDisplay(contentView, true);

CantHandleKey:
    return err;
}
#endif
//----------------------------------------------------------------------------------------------
#ifdef NEVER
static  OSStatus
WindowCloseHandler(EventHandlerCallRef nextHandler, EventRef inEvent, void *userData)
{
    QuitApplicationEventLoop();
    return CallNextEventHandler(nextHandler, inEvent);
}
#endif
//----------------------------------------------------------------------------------------------
int 
macout(int argc, char* argv[])
{
//    EventTypeSpec   drawEvent	= {kEventClassControl, kEventControlDraw};
//    EventTypeSpec   keyEvent	= {kEventClassKeyboard, kEventRawKeyDown};
//    EventTypeSpec   closeEvent	= {kEventClassWindow, kEventWindowClose};
//    HIViewRef	    contentView;
//    IBNibRef	    nibRef;
//    OSStatus	    err;
//    WindowRef	    window;
	
//    err = CreateNibReference(CFSTR("main"), &nibRef);
//    require_noerr(err, CantGetNibRef);
//    err = SetMenuBarFromNib(nibRef, CFSTR("MenuBar"));
//    require_noerr(err, CantSetMenuBar);
//    err = CreateWindowFromNib(nibRef, CFSTR("MainWindow"), &window);
//    require_noerr(err, CantCreateWindow);
//    DisposeNibReference(nibRef);

    ///////////////////////////////////////////////////////////////////
    // Install DrawHandler on contentView
//    HIViewFindByID(HIViewGetRoot(window), kHIViewWindowContentID, &contentView);
//    err = InstallEventHandler(HIViewGetEventTarget(contentView), DrawHandler, 1, &drawEvent, contentView, NULL);
//    require_noerr(err, CantInstallHandler);
    // Install KeyHandler on the window
//    err = InstallEventHandler(GetWindowEventTarget(window), KeyHandler, 1, &keyEvent, window, NULL);
//    require_noerr(err, CantInstallHandler);
    // Install Window Close
//    err = InstallWindowEventHandler(window, NewEventHandlerUPP(WindowCloseHandler), 1, &closeEvent, window, NULL);
//    require_noerr(err, CantInstallHandler);
    ///////////////////////////////////////////////////////////////////

//    cs_rgb = CGColorSpaceCreateDeviceRGB();
//    SetWindowTitleWithCFString(window, CFSTR("TstOut"));
//    ShowWindow(window);
//    RunApplicationEventLoop();
	
CantInstallHandler:
CantCreateWindow:
CantSetMenuBar:
CantGetNibRef:
//    return err;
    return 0;
}
