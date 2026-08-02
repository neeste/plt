//
//  AppDelegate.m
//  MacPlot
//
//  Created by Stephen Neely on 3/23/20.
//  Copyright © 2020 Stephen Neely. All rights reserved.
//

#import "AppDelegate.h"

#define MAXPATH        1024

int     outfile(char *);
int     errcnt(void);
void    epsfix(char *);
void    mac_page(void(*)(int), void(*)(int, int));
void    ViewPlot(char *, char *);

static char         ppn[MAXPATH] = "";
static char         spn[MAXPATH] = "";
static char         *ext[] = {".eps", ".cgm", ".emf", ".svg"};
static char         *opt[] = {"-m5 ", "-m6 ", "-m7 ", "-m8 "};
static int          nt = sizeof(ext) / sizeof(ext[0]);
static NSString     *def_ext = @"eps";
static NSString     *editApp = @"TextEdit";

@implementation AppDelegate

PlotView *plot;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
}
- (void)application:(NSApplication *)sender
          openFiles:(NSArray<NSString *> *)filenames {
    NSLog(@"application:openFiles");
    for(NSString *file in filenames) {
        [self openPlot:file];
    }
}
- (IBAction)chooseFile:(id)sender {
    NSLog(@"chooseFile");
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg beginWithCompletionHandler:^(NSInteger result){
        NSArray<NSURL *> *urls = [openDlg URLs];
        for(NSURL *url in urls) {
            [self openPlot:url.path];
        }
    }];
}
- (void)openPlot:(NSString *)filename {
    NSLog(@"openPlot %@",filename);
    [PlotView setFileName:filename];
    if (!self->_appController) {
        self->_appController = [[AppController alloc] initWithWindowNibName:@"AppController"];
    }
    [[self->_appController window] display];
    [self->_appController showWindow:self];
    NSDocumentController *dc = [NSDocumentController sharedDocumentController];
    [dc noteNewRecentDocumentURL:[NSURL fileURLWithPath:filename]];
}
- (IBAction)exportFile:(id)sender {
    NSString *pltFile = [PlotView getFileName];
    NSArray<NSString *> *extensions = @[@"eps", @"cgm", @"emf", @"svg"];
    // New name for the file using the current name and EPS extension
    NSString* folder = [pltFile stringByDeletingLastPathComponent];
    NSString* filename = pltFile.lastPathComponent;
    NSString* newFile = [[filename stringByDeletingPathExtension] stringByAppendingPathExtension:def_ext];
    NSURL *folderURL = [NSURL URLWithString:folder];
    strncpy(ppn, [pltFile UTF8String], MAXPATH);

    // Set the default name for the file and show the panel.
    NSSavePanel*    panel = [NSSavePanel savePanel];
    [panel setTitle:@"Export"];
    [panel setNameFieldStringValue:newFile];
    [panel setDirectoryURL:folderURL];
#if (MAC_OS_X_VERSION > MAX_OS_X_VERSION_10_9)
    [panel setShowsTagField:false];
#endif
    [panel setAllowedFileTypes:extensions];
    [panel beginWithCompletionHandler:^(NSInteger result){
        if (result == NSFileHandlingPanelOKButton) {
            NSURL*  theURL = [panel URL];
            NSString *thePath = [theURL path];
            NSLog(@"Export %@", thePath);
            strncpy(spn, [thePath UTF8String], MAXPATH);
            // process pltfile for export
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
                def_ext = extensions[select ];
            }
        }
    }];
}
- (IBAction)editFile:(id)sender {
    NSString *pltFile = [PlotView getFileName];
    NSLog(@"Edit %@", pltFile);
    [[NSWorkspace sharedWorkspace] openFile:pltFile withApplication:editApp];
}
- (IBAction)printFile:(id _Nonnull )sender {
    PlotView *pltView = [[PlotView alloc] init];
    NSRect rect = NSMakeRect(0,0,10.5*72,8*72);
    NSSize size = NSMakeSize(8*72,10.5*72);
    [PlotView setPrintMode:YES];
    NSPrintInfo *printInfo = [NSPrintInfo sharedPrintInfo];
    [printInfo setOrientation:1]; // Landscape
    [printInfo setVerticallyCentered:YES];
    [printInfo setHorizontallyCentered:YES];
    [printInfo setPaperSize:size];
    [pltView drawRect:rect];
    NSPrintOperation *op = [NSPrintOperation printOperationWithView:pltView printInfo:printInfo];
    [op setShowsPrintPanel:YES];
    [op runOperation];
    [PlotView setPrintMode:NO];
}
- (IBAction)errorLog:(id _Nonnull )sender {
    NSString *pltFile = [PlotView getFileName];
    NSString* folder = [pltFile stringByDeletingLastPathComponent];
    NSString *logFile = [folder stringByAppendingPathComponent:@"plt.log"];
    [[NSWorkspace sharedWorkspace] openFile:logFile withApplication:editApp];
}
- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end

