/*
 This file is part of Genie.
 Copyright 2009 Lucas Romero
 
 Genie is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Genie is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Genie.  If not, see <http://www.gnu.org/licenses/>.
 */

#import <Cocoa/Cocoa.h>
#import "MBPreferencesController.h"
#import "GHostConfigFile.h"
#import "UKKQueue.h"



@interface ConfigController : NSViewController <MBPreferencesModule> {
	NSMutableArray *cfgfiles;
	IBOutlet NSPopUpButton *configSelector;
	IBOutlet NSPanel *newConfigPanel;
	IBOutlet NSTextField *newConfigName;
	IBOutlet NSTextView *textEdit;
	UKKQueue *fileWatcher;
	IBOutlet NSArrayController *cfgArrayController;
	IBOutlet NSArrayController *secondController;
	BOOL initDone;
}
@property(retain) NSMutableArray *cfgfiles;
- (void)reloadConfigList;
- (IBAction)editConfig:(id)sender;
- (IBAction)newConfig:(id)sender;
- (IBAction)revertConfig:(id)sender;
- (IBAction)saveConfig:(id)sender;
- (IBAction)newConfigAccept:(id)sender;
- (IBAction)newConfigCancel:(id)sender;
- (GHostConfigFile *)selectedConfig;
@end
