//
//  TableViewController.m
//  Plt
//
//  Created by Stephen Neely on 3/19/11.
//  Copyright 2011 Boys Town National Research Hospital. All rights reserved.
//

#import "TableViewController.h"
#import "PltViewController.h"

static PltViewController *pltViewController;
static UIAlertView *alertSave;
static UIAlertView *alertView;
static BOOL editMode = NO;

@interface TableViewController (private)
@end

static NSString* examples[] = {
    @"logo.plt",
    @"nhc.plt",
    @"stc.plt"
};

#define NUM_DOCS (sizeof(examples)/sizeof(examples[0]))
#define kRowHeight 58.0

@implementation TableViewController

@synthesize docWatcher, documentURLs, docInteractionController, pltURL;

- (void)promptSave:(NSString *)pltpath {
    NSString *hdr = @"Save file?";
    NSString *msg = [pltpath lastPathComponent];
    NSString *btn1 = @"Yes";
    NSString *btn2 = @"No";
    alertSave = [[UIAlertView alloc] 
                 initWithTitle:hdr message:msg delegate:self cancelButtonTitle:nil 
                 otherButtonTitles:btn1, btn2, nil];
    [alertSave show];
    [alertSave release];
}

- (void)promptView:(NSString *)pltpath {
    NSString *hdr = @"Plt file:";
    NSString *msg = [pltpath lastPathComponent];
    NSString *btn1 = @"View";
    NSString *btn2 = @"Delete";
    alertView = [[UIAlertView alloc] 
                 initWithTitle:hdr message:msg delegate:self cancelButtonTitle:@"Cancel"
                 otherButtonTitles:btn1, btn2, nil];
    [alertView show];
    [alertView release];
}

- (void) fileCopy:(NSString *)pltpath {
    NSLog(@"fileCopy: %@.", pltpath);
    NSString *docdir = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
    NSString *docpath = [docdir stringByAppendingPathComponent:[pltpath lastPathComponent]];
    NSURL *plt_url = [NSURL fileURLWithPath:pltpath];
    NSURL *doc_url = [NSURL fileURLWithPath:docpath];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager copyItemAtURL:plt_url toURL:doc_url error:nil]) {
        // Handle the error.
        NSLog(@"*** Errror copying from %@ to %@.", pltpath, docpath);
    }    
}

- (void) fileDelete:(NSString *)pltpath {
    NSLog(@"fileDelete: %@.", pltpath);
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager removeItemAtPath:pltpath error:nil]) {
        // Handle the error.
        NSLog(@"*** Errror deleting file %@.", pltpath);
    }    
    // scan for existing documents
    [self directoryDidChange:self.docWatcher];
}

- (void) filePlot:(NSString *)pltpath {
    pltpath = [self.pltURL path];
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:YES];
    CGRect frame = self.view.frame;
    frame.origin.y = 20;
    pltViewController = [[PltViewController alloc] initWithFrame: frame andText: pltpath];
    [self.navigationController pushViewController:pltViewController animated:YES];
    [pltViewController release];    
}

- (void) fileView:(NSString *)pltpath {
    [self filePlot:pltpath];
}

- (void)alertView:(UIAlertView *)av didDismissWithButtonIndex:(NSInteger)bi {
    if (self.pltURL) {
        NSString *pltpath = [self.pltURL path];
        if (av == alertSave) {
            if (bi == 0) {
                [self fileCopy:pltpath];
            }      
        } else if (av == alertView) {
            if (bi == 1) {
                [self fileView:pltpath];
            } else if (bi == 2) {
                [self fileDelete:pltpath];
            }
        }
        self.pltURL = nil;
    }
}

- (void)setupDocumentControllerWithURL:(NSURL *)url {
    if (self.docInteractionController == nil) {
        self.docInteractionController = [UIDocumentInteractionController interactionControllerWithURL:url];
        self.docInteractionController.delegate = self;
    } else {
        self.docInteractionController.URL = url;
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    NSString * version = [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleShortVersionString"];
    NSString *title = [NSString stringWithFormat: @"Plt %@", version];
    self.title = title;
    self.navigationItem.rightBarButtonItem = [[[UIBarButtonItem alloc]
                                               initWithTitle:@"Edit"
                                               style:UIBarButtonItemStylePlain 
                                               target:self action:@selector(goEdit)] autorelease];
    editMode = NO;

    // Move examples to Documents directory
    NSString *appdir = [NSHomeDirectory() stringByAppendingPathComponent:@"Plt.app"];    
    for (int i = 0; i < NUM_DOCS; i++) {
        NSString *file = [appdir stringByAppendingPathComponent: examples[i]];
        [self fileCopy:file];
        [self fileDelete:file];
    }
    
    // start monitoring the document directory…
    NSString *documentsDirectoryPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];    
    self.docWatcher = [DirectoryWatcher watchFolderWithPath:documentsDirectoryPath delegate:self];
    self.documentURLs = [NSMutableArray array];
    
    // scan for existing documents
    [self directoryDidChange:self.docWatcher];
}

- (void)goEdit {
    if (editMode) {
        self.navigationItem.rightBarButtonItem.title = @"Edit";
        self.navigationItem.rightBarButtonItem.style = UIBarButtonItemStylePlain;
        editMode = NO;
    } else {
        self.navigationItem.rightBarButtonItem.title = @"Done";
        self.navigationItem.rightBarButtonItem.style = UIBarButtonItemStyleDone;
        editMode = YES;
    }
}

- (void)viewWillAppear:(BOOL)animated {
    if (self.pltURL) {
        NSString *pltpath = [self.pltURL path];
        NSString *docdir = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
        NSString *pltdir = [pltpath stringByDeletingLastPathComponent];
        if (![docdir isEqualToString: pltdir]) {
            [self promptSave:pltpath];
        }
    }
    [[UIApplication sharedApplication] setStatusBarHidden:NO withAnimation:YES];
}

- (void)viewDidUnload {
    self.documentURLs = nil;
    self.docWatcher = nil;
}

- (void)dealloc {
    [documentURLs release];
    [docWatcher release];
    
    [super dealloc];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return YES;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    if (self.documentURLs.count > 0) {
        return self.documentURLs.count;
    }
    return 1;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
    NSString *title = nil;

    title = @"Plt Documents";
    
    return title;
}

- (NSString *)formattedFileSize:(unsigned long long)size {
    NSString *formattedStr = nil;
    if (size == 0) {
		formattedStr = @"Empty";
    } else if (size > 0 && size < 1024) {
            formattedStr = [NSString stringWithFormat:@"%qu bytes", size];
    } else {
        if (size >= 1024 && size < pow(1024, 2)) {
            formattedStr = [NSString stringWithFormat:@"%.1f KB", (size / 1024.)];
        } else {
            if (size >= pow(1024, 2) && size < pow(1024, 3)) {
                formattedStr = [NSString stringWithFormat:@"%.2f MB", (size / pow(1024, 2))];
            } else {
                if (size >= pow(1024, 3)) {
                    formattedStr = [NSString stringWithFormat:@"%.3f GB", (size / pow(1024, 3))];
                }
            }
        }
    }

    return formattedStr;
}

- (UITableViewCell *)tableView:(UITableView*)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *cellIdentifier = @"cellID";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    
    if (!cell) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellIdentifier] autorelease];
    }
    if (self.documentURLs.count > 0) {
        NSURL *fileURL;
        // second section is the contents of the Documents folder
        fileURL = [self.documentURLs objectAtIndex:indexPath.row];
        [self setupDocumentControllerWithURL:fileURL];
	
        // layout the cell
        cell.textLabel.text = [[fileURL path] lastPathComponent];
        NSError *error;
        NSString *fileURLString = [self.docInteractionController.URL path];
        NSDictionary *fileAttributes = [[NSFileManager defaultManager] attributesOfItemAtPath:fileURLString error:&error];
        NSInteger fileSize = [[fileAttributes objectForKey:NSFileSize] intValue];
        NSString *fileDate = [[[fileAttributes objectForKey:NSFileModificationDate] description] substringToIndex:16];
        cell.detailTextLabel.text = [NSString stringWithFormat:@"%@ - %@",
                                     fileDate, [self formattedFileSize:fileSize]];
    } else {
        cell.textLabel.text = @"< No files >";
        cell.detailTextLabel.text = @"";
    }
    
    return cell;
}

- (CGFloat)tableView:(UITableView*)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return kRowHeight;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    NSURL *fileURL;
    fileURL = [self.documentURLs objectAtIndex:indexPath.row];
    self.pltURL = fileURL;
    if (editMode) {
        [self promptView:[fileURL path]];
    } else {
        [self fileView:[fileURL path]];
    }
}

- (UIViewController *)documentInteractionControllerViewControllerForPreview:(UIDocumentInteractionController *)interactionController {
    return self;
}

- (void)directoryDidChange:(DirectoryWatcher *)folderWatcher
{
    [self.documentURLs removeAllObjects];    // clear out the old docs and start over
	
    NSString *documentsDirectoryPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents"];
    NSArray *documentsDirectoryContents = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:documentsDirectoryPath error:NULL];
    
    for (NSString* curFileName in [documentsDirectoryContents objectEnumerator]) {
        NSString *filePath = [documentsDirectoryPath stringByAppendingPathComponent:curFileName];
        NSURL *fileURL = [NSURL fileURLWithPath:filePath];

        BOOL isDirectory;
        [[NSFileManager defaultManager] fileExistsAtPath:filePath isDirectory:&isDirectory];
		
        // proceed to add the document URL to our list (ignore the "Inbox" folder)
        if (!(isDirectory && [curFileName isEqualToString: @"Inbox"])) {
            [self.documentURLs addObject:fileURL];
        }
    }
	
    [self.tableView reloadData];
}

@end
