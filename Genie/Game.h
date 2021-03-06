//
//  Game.h
//  Genie
//
//  Created by Lucas on 11.01.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

typedef enum GameStatus {
	GameStatusLobby,
	GameStatusClosed,
	GameStatusRunning
} GameStatus;

@class Bot;

@interface Game :  NSManagedObject  
{
	NSValue *gameObject;
	NSCalendarDate *dateCreated;
}

@property (nonatomic, retain) NSValue * gameObject;

@property (nonatomic, retain) NSNumber * status;
@property (nonatomic, retain) NSNumber * id;
@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NSString * gamemode;
@property (nonatomic, retain) NSSet* messages;
@property (nonatomic, retain) Bot * bot;
@property (nonatomic, retain) NSSet* players;
@property (nonatomic, retain) NSCalendarDate* dateCreated;

@end


@interface Game (CoreDataGeneratedAccessors)
- (void)addMessagesObject:(NSManagedObject *)value;
- (void)removeMessagesObject:(NSManagedObject *)value;
- (void)addMessages:(NSSet *)value;
- (void)removeMessages:(NSSet *)value;

- (void)addPlayersObject:(NSManagedObject *)value;
- (void)removePlayersObject:(NSManagedObject *)value;
- (void)addPlayers:(NSSet *)value;
- (void)removePlayers:(NSSet *)value;

@end

