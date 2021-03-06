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
#import "ghost4mac/TaskWrapper.h"

@interface GHostController : NSObject <TaskWrapperController> {
	TaskWrapper *ghost;
	BOOL running;
	NSString *ghostDir;
	NSMutableString *consoleBuffer;
}
extern NSString * const GHConsoleOutput;
extern NSString * const GHProcessStarted;
extern NSString * const GHProcessStopped;
@property BOOL running;
@property(copy) NSString *ghostDir;
+ (GHostController*)sharedController;
- (void)startWithConfig:(NSString*)cfg;
- (void)stop;
@end
