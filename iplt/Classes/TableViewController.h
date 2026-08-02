//
//  TableViewController.h
//  Plt
//
//  Created by Stephen Neely on 3/19/11.
//  Copyright 2011 Boys Town National Research Hospital. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "DirectoryWatcher.h"

@interface TableViewController : UITableViewController <DirectoryWatcherDelegate,
                                                          UIDocumentInteractionControllerDelegate>
{
    DirectoryWatcher *docWatcher;
    NSMutableArray *documentURLs;
    UIDocumentInteractionController *docInteractionController;
    NSURL *pltURL;
}

@property (nonatomic, retain) DirectoryWatcher *docWatcher;
@property (nonatomic, retain) NSMutableArray *documentURLs;
@property (nonatomic, retain) UIDocumentInteractionController *docInteractionController;
@property (retain) NSURL *pltURL;

@end
