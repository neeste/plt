//
//  AppDelegate.h
//  MacPlot
//
//  Created by Stephen Neely on 3/23/20.
//  Copyright © 2020 Stephen Neely. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AppController.h"
#import "PlotView.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
    PlotView *plot;
}

@property (assign) IBOutlet NSWindow * _Nonnull window;
@property AppController * _Nonnull appController;

- (IBAction)chooseFile:(id _Nonnull )sender;
- (IBAction)exportFile:(id _Nonnull )sender;
- (IBAction)editFile:(id _Nonnull )sender;
- (IBAction)printFile:(id _Nonnull )sender;
- (IBAction)errorLog:(id _Nonnull )sender;
- (void)application:(NSApplication *_Nullable)sender openFiles:(NSArray<NSString *> *_Nullable)filenames;
- (void)openPlot:(NSString *_Nullable)filename;

@end

