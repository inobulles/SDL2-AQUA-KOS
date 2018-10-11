
#ifndef __AQUA__SDL2_KOS_FUNCTIONS_DISCORD_H
	#define __AQUA__SDL2_KOS_FUNCTIONS_DISCORD_H
	
	// following the Discord RPC documentation (https://discordapp.com/developers/docs/rich-presence/how-to)
	// and the "send presence" example         (https://github.com/discordapp/discord-rpc/blob/master/examples/send-presence/send-presence.c)
	
	#include "../discord/discord_rpc.h"
	
	typedef struct {
		unsigned long long state;
		unsigned long long details;
		
		unsigned long long large_image_text;
		unsigned long long small_image_text;
		
		unsigned long long large_image;
		unsigned long long small_image;
		
		unsigned long long party;
		
		unsigned long long party_size;
		unsigned long long party_max;
		
	} kos_discord_rpc_t;
	
	static const char*          kos_discord_rpc_application_id = "499960380949397507";
	static int64_t              kos_discord_rpc_start_time;
	static DiscordEventHandlers kos_discord_rpc_handlers;
	
	static void discord_rpc_generic_handler() {
		printf("TODO\n");
		
	}
	
	static void discord_rpc_ready_handler(const DiscordUser* user) {
		printf("INFO Discord RPC connected to user %s#%s - %s\n", user->username, user->discriminator, user->userId);
		
	}
	
	#include <time.h>
	
	void update_discord_rpc(kos_discord_rpc_t* this) {
		DiscordRichPresence discord_presence;
		memset(&discord_presence, 0, sizeof(DiscordRichPresence));
		
		discord_presence.startTimestamp = kos_discord_rpc_start_time;
		discord_presence.endTimestamp   = time(0);
		
		discord_presence.state          = (const char*) this->state;
		discord_presence.details        = (const char*) this->details;
		
		discord_presence.largeImageText = (const char*) this->large_image_text;
		discord_presence.smallImageText = (const char*) this->small_image_text;
		
		discord_presence.largeImageKey  = (const char*) this->large_image;
		discord_presence.smallImageKey  = (const char*) this->small_image;
		
		discord_presence.partyId        = (const char*) this->party;
		discord_presence.partySize      = (int)         this->party_size;
		discord_presence.partyMax       = (int)         this->party_max;
		
		discord_presence.instance = 0;
		Discord_UpdatePresence(&discord_presence);
		
	}
	
	void loop_discord_rpc(void) {
		#ifdef DISCORD_DISABLE_IO_THREAD
			Discord_UpdateConnection();
		#endif
		
		Discord_RunCallbacks();
		
	}
	
	void init_discord_rpc(void) {
		kos_discord_rpc_start_time = time(0);
		memset(&kos_discord_rpc_handlers, 0, sizeof(DiscordEventHandlers));
		
		kos_discord_rpc_handlers.ready        = discord_rpc_ready_handler;
		kos_discord_rpc_handlers.disconnected = discord_rpc_generic_handler;
		kos_discord_rpc_handlers.errored      = discord_rpc_generic_handler;
		kos_discord_rpc_handlers.joinGame     = discord_rpc_generic_handler;
		kos_discord_rpc_handlers.spectateGame = discord_rpc_generic_handler;
		kos_discord_rpc_handlers.joinRequest  = discord_rpc_generic_handler;
		
		Discord_Initialize(kos_discord_rpc_application_id, &kos_discord_rpc_handlers, 1, NULL);
		loop_discord_rpc();
		
	}
	
	void dispose_discord_rpc(void) {
		Discord_Shutdown();
		
	}
	
#endif
