/*	GHostInterface.h
 *
 *	This file is part of Genie
 *	Copyright (C) 2009-2010 Lucas Romero
 *	Created 03.01.10
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

#import <Cocoa/Cocoa.h>
#ifdef __cplusplus
	class CGHost;
	class CGHostGenie;
	class CConfig;
	class MessageLogger;
	#define FAKE_CXX_TYPE(type) type
#else
	#define FAKE_CXX_TYPE(type) void *
#endif

@class BotLocal;

@protocol GHostDelegate
/* all messages are invoked on the mainThread */
- (void)ghostCreated:(NSValue*)ghost;
- (void)ghostTerminates:(NSValue*)ghost;
- (void)consoleOutputCallback:(NSString*)message;
- (void)chatMessageReceived:(NSDictionary*)data;
- (void)whisperReceived:(NSDictionary*)data;
- (void)emoteReceived:(NSDictionary*)data;
- (void)channelJoined:(NSDictionary*)data;
- (void)chatLeft:(NSDictionary*)data;
- (void)userJoinedChannel:(NSDictionary*)data;
- (void)userLeftChannel:(NSDictionary*)data;
- (void)incomingFriendInfo:(NSDictionary*)data;
- (void)incomingClanMemberInfo:(NSDictionary*)data;
- (void)gameCreated:(NSDictionary*)data;
- (void)gameDeleted:(NSDictionary*)data;
- (void)gameRefreshed:(NSDictionary*)data;
- (void)gameLoaded:(NSDictionary*)data;
@end


@interface GHostInterface : NSObject {
	NSThread* ghostThread;
	BOOL cancelled;
	BOOL running;
	NSMutableArray *cmdQueue;
	NSLock *cmdLock;
	NSLock *mainLock;
	NSObject <GHostDelegate> *delegate;
	NSNumber *useRemoteHasher;
	NSString *chatUsername;
	
	FAKE_CXX_TYPE(CGHostGenie *)instance;
	FAKE_CXX_TYPE(CConfig *)cfg;
	FAKE_CXX_TYPE(MessageLogger *)logger;
}
- (void)startBotWithConfig:(NSDictionary *)config;
- (void)stop;
- (NSNumber*)getHostPort;
- (NSValue*)ghostInstance;
- (void)execCommand:(NSDictionary *)cmd;
- (void)getLock;
- (void)releaseLock;
- (void)sendChat:(NSDictionary*)message;
@property (nonatomic, retain) NSString * chatUsername;
@property (nonatomic) BOOL running;
@property (nonatomic, readonly) NSNumber* useRemoteHasher;
@property (assign) NSObject <GHostDelegate> *delegate;
@end
