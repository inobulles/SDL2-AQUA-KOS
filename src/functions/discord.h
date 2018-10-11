
#ifndef __AQUA__SDL2_KOS_FUNCTIONS_DISCORD_H
	#define __AQUA__SDL2_KOS_FUNCTIONS_DISCORD_H
	
	// following the Discord RPC documentation (https://discordapp.com/developers/docs/rich-presence/how-to)
	// and the "send presence" example         (https://github.com/discordapp/discord-rpc/blob/master/examples/send-presence/send-presence.c)
	
	#include "../discord/discord_rpc.h"
	
	static const char*          kos_discord_rpc_application_id = "499960380949397507";
	static int64_t              kos_discord_rpc_start_time;
	static DiscordEventHandlers kos_discord_rpc_handlers;
	
	static void discord_rpc_generic_handler() {
		printf("============================================================================================================= TODO\n");
		
	}
	
	static void discord_rpc_ready_handler(const DiscordUser* user) {
		printf("============================================================================================================= INFO Discord RPC connected to user %s#%s - %s\n", user->username, user->discriminator, user->userId);
		
	}
	
	#include <time.h>
	
	void update_discord_rpc(void) {
		DiscordRichPresence discord_presence;
		memset(&discord_presence, 0, sizeof(DiscordRichPresence));
		
		discord_presence.state          = "Using AQUA";
		discord_presence.details        = "Navigating AQUA desktop";
		discord_presence.startTimestamp = kos_discord_rpc_start_time;
		discord_presence.endTimestamp   = time(0);
		discord_presence.instance       = 0;
		
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
		update_discord_rpc();
		
	}
	
	void dispose_discord_rpc(void) {
		Discord_Shutdown();
		
	}
	
#endif
