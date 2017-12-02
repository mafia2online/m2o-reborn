#include <discord-rpc.h>

static const char* APPLICATION_ID = "385521976564908032";
static int FrustrationLevel = 0;
static int64_t StartTime;

static void updateDiscordPresence()
{
    //char buffer[256];
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Developing";
    discordPresence.details = "Details";
    discordPresence.startTimestamp = StartTime;
    discordPresence.endTimestamp = time(0) + 5 * 60;
    //discordPresence.largeImageKey = "canary-large";
    discordPresence.smallImageKey = "m2o-logo";
    discordPresence.smallImageText = "Hello";
    discordPresence.partyId = "party1234";
    discordPresence.partySize = 1;
    discordPresence.partyMax = 6;
    discordPresence.matchSecret = "xyzzy";
    discordPresence.joinSecret = "join";
    discordPresence.spectateSecret = "look";
    discordPresence.instance = 0;
    Discord_UpdatePresence(&discordPresence);
}

static void handleDiscordReady()
{
    printf("\nDiscord: ready\n");
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
    printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
    printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

static void handleDiscordJoin(const char* secret)
{
    printf("\nDiscord: join (%s)\n", secret);
}

static void handleDiscordSpectate(const char* secret)
{
    printf("\nDiscord: spectate (%s)\n", secret);
}

static void discordInit()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;
    handlers.joinGame = handleDiscordJoin;
    handlers.spectateGame = handleDiscordSpectate;
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

