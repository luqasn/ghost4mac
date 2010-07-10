/*	GameStatusImageValueTransformer.m
 *
 *	This file is part of Genie
 *	Copyright (C) 2009-2010 Lucas Romero
 *	Created 01.01.10
 *
 *	Genie is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Genie is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 * 	You should have received a copy of the GNU General Public License
 * 	along with Genie.  If not, see <http://www.gnu.org/licenses/>.
 */

#import "GameStatusImageValueTransformer.h"
#import "Game.h"


@implementation GameStatusImageValueTransformer
+ (Class)transformedValueClass { return [NSImage class]; }
+ (BOOL)allowsReverseTransformation { return NO; }
- (id)transformedValue:(id)value {
	
	NSNumber *val = value;
	
	switch ([val intValue]) {
		case GameStatusLobby:
			return [NSImage imageNamed:@"YellowDot.png"];
		case GameStatusClosed:
			return [NSImage imageNamed:@"RedDot.png"];
		case GameStatusRunning:
			return [NSImage imageNamed:@"GreenDot.png"];
	}
	return nil;
}
@end
