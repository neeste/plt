//
//  PltViewController.h
//  Plt
//
//  Created by Stephen Neely on 3/19/11.
//  Copyright 2011 Boys Town National Research Hospital. All rights reserved.
//

#import <UIKit/UIKit.h>

@class PltView;

@interface PltViewController : UIViewController {
    UILabel *label;
    NSString *text;
    PltView *pltView;
}

- (id)initWithFrame:(CGRect)frame andText:(NSString *)text;

@property (retain) IBOutlet UILabel *label;
@property (retain) NSString *text;
@property (nonatomic, retain) PltView *pltView;

@end
