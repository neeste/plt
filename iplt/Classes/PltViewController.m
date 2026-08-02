//
//  PltViewController.m
//  PushAndPop
//

#import "PltViewController.h"
#import "PltView.h"

static CGRect vc_frame;

@implementation PltViewController

@synthesize label, text, pltView;

- (id)initWithFrame: (CGRect) frame andText:(NSString *)someText
{
    // Custom initialization
    self.title = @"PLT View";
    self.text = someText;
    vc_frame = frame;
    float w = vc_frame.size.width;
    float h = vc_frame.size.height;
    if ((w / h) > (8 / 10.5)) {
        float v = floor(h * 8 / 10.5);
        vc_frame.origin.x = floor((w - v) / 2); 
        vc_frame.size.width = v; 
    }

    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    self.label.text = [self.text lastPathComponent];
    self.title = [self.text lastPathComponent];	
    pltView = [[PltView alloc] initWithFrame:vc_frame];
    [pltView pltPath: self.text];
    [self.view addSubview:pltView];
}

- (void)dealloc {
    [pltView release];
    [label release];
    [text release];
    
    [super dealloc];
}


@end
