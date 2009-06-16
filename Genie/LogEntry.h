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

@interface LogEntry : NSObject {
	NSImage *image;
	NSString *sender;
	NSDate *date;
	NSString *text;
}
@property(copy) NSImage* image;
@property(copy) NSString* sender;
@property(copy) NSString* text;
@property(copy) NSDate*	date;

//- (id)initWithText:(NSString*)newText sender:(NSString*)newSender date:(NSDate*)newDate image:(NSString*)newImage;
+ (id)logEntryWithLine:(NSString*)line;
+ (id)logEntryWithText:(NSString*)text sender:(NSString*)sender date:(NSDate*)date image:(NSImage*)image;
@end