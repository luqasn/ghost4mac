/*	Bot.m
 *
 *	This file is part of Genie
 *	Copyright (C) 2009-2010 Lucas Romero
 *	Created 06.01.10
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

#import "Bot.h"
#import "Game.h"

@implementation Bot 

@dynamic autoStart;
@dynamic name;
@dynamic hostPort;
@dynamic adminCount;
@dynamic version;
@dynamic comment;
@synthesize running;
@synthesize botStatus;
@dynamic messages;
@dynamic games;
@dynamic servers;

static NSPersistentStore* tempStore = nil;

- (NSPersistentStore*)getTempStore
{
	if (tempStore) {
		return tempStore;
	}
	NSPersistentStoreCoordinator *psc = [self.managedObjectContext persistentStoreCoordinator];
	tempStore = [psc addPersistentStoreWithType:NSInMemoryStoreType configuration:nil URL:nil options:nil error:nil];
	return tempStore;
}

- (NSPersistentStore*)tempStore
{
	NSArray *stores = [[self.managedObjectContext persistentStoreCoordinator] persistentStores];
	NSEnumerator *e = [stores objectEnumerator];
	NSPersistentStore *store;
	while (store = [e nextObject]) {
		if ([[store type] isEqualToString:NSInMemoryStoreType]) {
			return store;
		}
	}
	return nil;
}

- (id)AddTemporaryEntity:(NSString*)entity
{
	id obj = [NSEntityDescription insertNewObjectForEntityForName:entity inManagedObjectContext:self.managedObjectContext];
	//[self.managedObjectContext assignObject:obj toPersistentStore:[self getTempStore]];
	
	//NSPersistentStore *store = [[self.managedObjectContext persistentStoreCoordinator] persistentStoreForURL:[NSURL URLWithString:@"/dev/mem"]];
	//NSLog(@"TEMPORARY STORE: %@", store);
	[self.managedObjectContext assignObject:obj toPersistentStore:[self tempStore]];
	return obj;
}

- (void)awakeFromFetch
{
	[self willChangeValueForKey:@"botStatus"];
	botStatus = BotStatusStopped;
	[self didChangeValueForKey:@"botStatus"];
	[self willChangeValueForKey:@"running"];
	running = [NSNumber numberWithBool:NO];
	[self didChangeValueForKey:@"running"];
	
	if ([self.autoStart boolValue]) {
		[self start];
	}
}

-(void)start
{
}

-(void)stop
{
}

-(void)startStop
{
}

- (void)sendCommand:(NSDictionary *)cmd
{
}

/*- (void) setStartIndicator:(NSNumber*)run
{
	if ([run boolValue]) {
		[self stop];
	}
	else {
		[self start];
	}

}
- (NSNumber*)startIndicator
{
	return self.running;
}*/

@end
