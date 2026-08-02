//
//  PlotView.h - MacPlot version
//
//  Created by Stephen Neely on 3/23/20.
//  Copyright © 2020 Stephen Neely. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface PlotView : NSView

+ (NSString *_Nonnull)getFileName;
+ (void)setFileName:(NSString *_Nullable)app_filename;
+ (NSSize)setPrintMode:(BOOL)mode;
- (BOOL)knowsPageRange:(NSRangePointer _Nullable )range;
- (NSRect)rectForPage:(NSInteger)page;

@end
