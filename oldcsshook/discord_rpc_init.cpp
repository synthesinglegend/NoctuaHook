#include "discord_rpc_init.h"
#include <ctime>
#include "Main.h"

void Discord::Initialize()
{
    DiscordEventHandlers Handle;
    memset(&Handle, 0, sizeof(Handle));
    Discord_Initialize("1206307931788677180", &Handle, 1, NULL);
}

void Discord::Update()
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Counter-Strike: Source";
    discordPresence.details = "NoctuaHook";
    discordPresence.largeImageText = "NoctuaHook";
    discordPresence.smallImageText = "Beta";
    discordPresence.largeImageKey = "https://i.imgur.com/5eRMpEk.png";
    discordPresence.smallImageKey = "https://i.imgur.com/F6tm3jJ.png";
    Discord_UpdatePresence(&discordPresence);
}




/*
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    static auto elapsed = std::time(nullptr);

    discordPresence.largeImageText = "";
    discordPresence.state = "";
    discordPresence.largeImageKey = "";
    discordPresence.startTimestamp = elapsed;
    discordPresence.smallImageKey = "";
    discordPresence.smallImageText = "";
    discordPresence.button1_label = ""; // да да я добавил кнопочки =)
    discordPresence.button1_url = "";
    discordPresence.button2_label = "";
    discordPresence.button2_url = "";
    Discord_UpdatePresence(&discordPresence);
}
*/