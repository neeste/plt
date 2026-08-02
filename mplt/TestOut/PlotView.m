//
//  PlotView.m - TestOut version
//
//  Created by Stephen Neely on 3/23/20.
//  Copyright © 2020 Stephen Neely. All rights reserved.
//

#import "PlotView.h"

int    tstout(int, char **);
void    mac_context(CGContextRef *, CGRect *, CGColorSpaceRef);

@implementation PlotView

- (void)viewDidMoveToWindow{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowDidResize:) name:NSWindowDidResizeNotification object:[self window]];
}
- (void)windowDidResize:(NSNotification*) notification{
    NSRect winrect = [[self window] frame];
    NSRect contentpane = [[self window] contentRectForFrameRect:winrect];
    NSRect view_rect = NSMakeRect(0, 0, contentpane.size.width, contentpane.size.height);
    [self setFrame:view_rect];
    [self setNeedsDisplay:true];
}
- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    // establish graphics context
    CGRect viewBounds = CGRectMake(dirtyRect.origin.x, dirtyRect.origin.x, dirtyRect.size.width, dirtyRect.size.height);
    NSGraphicsContext *currentGrpahicsContext = [NSGraphicsContext currentContext];
    CGContextRef ctx = (CGContextRef)[currentGrpahicsContext graphicsPort];
    CGColorSpaceRef cs_rgb = CGColorSpaceCreateDeviceRGB();
    mac_context(&ctx, (CGRect *) &viewBounds, cs_rgb);
    // draw plot
    tstout(0, NULL);
}

@end
