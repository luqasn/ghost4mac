//
//  Channel.h
//  Genie
//
//  Created by Lucas on 11.01.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class Server;
@class User;
@class ChatMessage;

@interface Channel : NSObject
{
}

@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NSSet* messages;
@property (nonatomic, retain) NSSet* users;
@property (nonatomic, retain) Server * server;

@end


@interface Channel (CoreDataGeneratedAccessors)
- (void)addMessagesObject:(ChatMessage *)value;
- (void)removeMessagesObject:(ChatMessage *)value;
- (void)addMessages:(NSSet *)value;
- (void)removeMessages:(NSSet *)value;

- (void)addUsersObject:(User *)value;
- (void)removeUsersObject:(User *)value;
- (void)addUsers:(NSSet *)value;
- (void)removeUsers:(NSSet *)value;

@end

