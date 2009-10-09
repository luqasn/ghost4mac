//
//  ChatViewController.h
//  Genie
//
//  Created by Lucas on 15.07.09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "LRViewController.h"

@interface ChatViewController : NSViewController {
	NSMutableDictionary *commandTriggers;
	NSMutableArray *_messages;
	IBOutlet NSArrayController *listController;
	IBOutlet NSTableView *messageTable;
	BOOL hideCommands;
	BOOL autoScroll;
	NSPredicate *chatFilter;
	NSImage *favIcon;
}
@property(retain) NSMutableArray *messages;
@property BOOL hideCommands;
@property BOOL autoScroll;
@property(copy) NSPredicate* chatFilter;
- (void)parseConsoleOutput:(NSString*)line;
- (IBAction)inputCommand:(id)sender;
- (IBAction)copyLines:(id)sender;
@end