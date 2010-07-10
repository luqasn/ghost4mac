// 
//  Game.m
//  Genie
//
//  Created by Lucas on 11.01.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Game.h"

#import "Bot.h"

@implementation Game

@synthesize gameObject;

@dynamic status;
@dynamic id;
@dynamic name;
@dynamic gamemode;
@dynamic messages;
@dynamic bot;
@dynamic players;
@synthesize dateCreated;

-(void)awakeFromInsert
{
	self.dateCreated = [NSCalendarDate date];
}

@end
