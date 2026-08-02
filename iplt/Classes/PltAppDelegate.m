//
//  PltAppDelegate.m
//  Plt
//
//  Created by Stephen Neely on 3/19/11.
//  Copyright 2011 Boys Town National Research Hospital. All rights reserved.
//

#import "PltAppDelegate.h"
#import "TableViewController.h"
#import "PltViewController.h"

@implementation PltAppDelegate

@synthesize window, navController;

- (void) viewURL:(NSURL *)url {
    NSString *path = [url path];
    TableViewController *tvc = (TableViewController *)[navController visibleViewController];
    tvc.pltURL = url;
    CGRect frame = navController.view.frame;
    UIViewController *pvc = [[PltViewController alloc] initWithFrame: frame andText: path];
    [navController pushViewController:pvc animated:YES];
    [pvc release];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    window.backgroundColor = [UIColor blackColor];
    [window addSubview:navController.view];

    if (launchOptions && [launchOptions objectForKey:UIApplicationLaunchOptionsURLKey]) {
        NSURL *url = (NSURL*)[launchOptions objectForKey:UIApplicationLaunchOptionsURLKey];
        [self viewURL:url];
    }
    [window makeKeyAndVisible];

    return YES;
}

- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url sourceApplication:(NSString *)src annotation:(id)ann { 
    [self viewURL:url];

    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    [navController popToRootViewControllerAnimated: NO];
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    // Free up as much memory as possible by purging cached data objects that can be recreated
    // (or reloaded from disk) later.
}

- (void)dealloc {
    [navController release];
    [window release];
    
    [super dealloc];
}

@end
