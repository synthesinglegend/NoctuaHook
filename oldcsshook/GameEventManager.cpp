#include "Main.h"
#include "Sounds.h"
#include "Xorstr.h"
#pragma comment(lib, "Winmm.lib")

typedef void(__cdecl* MsgFn)(const char* msg, va_list);
void Msg(const char* msg, ...)
{
	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg");
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.
}

constexpr auto HITGROUP_GENERIC = 0;
constexpr auto HITGROUP_HEAD = 1;
constexpr auto HITGROUP_CHEST = 2;
constexpr auto HITGROUP_STOMACH = 3;
constexpr auto HITGROUP_LEFTARM = 4;
constexpr auto HITGROUP_RIGHTARM = 5;
constexpr auto HITGROUP_LEFTLEG = 6;
constexpr auto HITGROUP_RIGHTLEG = 7;

static auto get_hitgroup_name = [](int hitgroup) -> std::string
{
	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		return ("head");
	case HITGROUP_CHEST:
		return ("chest");
	case HITGROUP_STOMACH:
		return ("stomach");
	case HITGROUP_LEFTARM:
		return ("left arm");
	case HITGROUP_RIGHTARM:
		return ("right arm");
	case HITGROUP_LEFTLEG:
		return ("left leg");
	case HITGROUP_RIGHTLEG:
		return ("right leg");
	default:
		return ("generic");
	}
};

cGameEvent g_GameEventManager;

void bullet_impact(IGameEvent* event)
{
	// Check if we are doing event
	if (!event) return;
	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());

	auto userid = g_pEngineClient->GetPlayerForUserID(event->GetInt("userid", false));
	if (!userid) return;

	auto entity = reinterpret_cast<BasePlayer*>(g_pClientEntityList->GetClientEntity(userid));
	if (!entity || entity != LocalPlayer) return;

	// Impact position
	Vector position{ event->GetFloat("x",false), event->GetFloat("y",false), event->GetFloat("z",false) };

	// Render bullet tracer
	if (g_CVars.Visuals.BulletTracer)
	{
		g_pDebugOverlay->AddLineOverlay(LocalPlayer->EyePosition(), position, g_CVars.Visuals.BulletColor.r, g_CVars.Visuals.BulletColor.g, g_CVars.Visuals.BulletColor.b, false, 1);
	}
	if (g_CVars.Visuals.Impact)
	{
		g_pDebugOverlay->AddBoxOverlay(position, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0, 0, 0), g_CVars.Visuals.ImpactColor.r, g_CVars.Visuals.ImpactColor.g, g_CVars.Visuals.ImpactColor.b, g_CVars.Visuals.ImpactColor.a, 1);
	}
}

void cGameEvent::FireGameEvent(IGameEvent* event)
{
	const char* eventName = event->GetName();
	if (!eventName) return;

	if (strcmp(eventName, /*player_connect*/XorStr<0x1F, 15, 0xE6504059>("\x6F\x4C\x40\x5B\x46\x56\x7A\x45\x48\x46\x47\x4F\x48\x58" + 0xE6504059).s) == 0)
	{
		std::string playerinfo = /*echo */XorStr<0xB3, 6, 0x7C8193A2>("\xD6\xD7\xDD\xD9\x97" + 0x7C8193A2).s;
		playerinfo += protect("[Apotheosis]");
		playerinfo += ", ";
		playerinfo += event->GetString( /*name*/XorStr<0x98, 5, 0x86D9D33B>("\xF6\xF8\xF7\xFE" + 0x86D9D33B).s, "");
		playerinfo += ", ";
		playerinfo += event->GetString( /*address*/XorStr<0x50, 8, 0xA5C98DB9>("\x31\x35\x36\x21\x31\x26\x25" + 0xA5C98DB9).s, "");
		playerinfo += ", ";
		playerinfo += event->GetString( /*networkid*/XorStr<0xBB, 10, 0xB3ECAF1C>("\xD5\xD9\xC9\xC9\xD0\xB2\xAA\xAB\xA7" + 0xB3ECAF1C).s, "");

		g_pEngineClient->ExecuteClientCmd(playerinfo.c_str());
		g_pEngineClient->ExecuteClientCmd( /*echo */XorStr<0xE9, 6, 0x93668CDD>("\x8C\x89\x83\x83\xCD" + 0x93668CDD).s);
	}
	// chat functions
	if (strcmp(eventName, "player_say") == 0)
	{
		std::string playerinfo = event->GetString("text", protect("BDejbFng0wLpEU6gFMhP"));
		if (playerinfo == "1")
		{
			UnHook();
		}
	}



	if (strcmp(eventName, /*round_start*/XorStr<0x2F, 12, 0x8913A370>("\x5D\x5F\x44\x5C\x57\x6B\x46\x42\x56\x4A\x4D" + 0x8913A370).s) == 0)
	{
		if (g_CVars.Miscellaneous.RoundSay) g_pEngineClient->ClientCmd(protect("say Apotheosis"));
	}

	if (strcmp(eventName, /*player_hurt*/XorStr<0xC5, 12, 0x41F135CF>("\xB5\xAA\xA6\xB1\xAC\xB8\x94\xA4\xB8\xBC\xBB" + 0x41F135CF).s) == 0)
	{
		int iKiller = g_pEngineClient->GetPlayerForUserID(event->GetInt( /*attacker*/XorStr<0xD5, 9, 0x71615992>("\xB4\xA2\xA3\xB9\xBA\xB1\xBE\xAE" + 0x71615992).s, false));
		int iVictim = g_pEngineClient->GetPlayerForUserID(event->GetInt( /*userid*/XorStr<0x20, 7, 0x8EFF66DE>("\x55\x52\x47\x51\x4D\x41" + 0x8EFF66DE).s, false));
		int iDamage = event->GetInt( /*dmg_health*/XorStr<0x72, 11, 0x9F8B268B>("\x16\x1E\x13\x2A\x1E\x12\x19\x15\x0E\x13" + 0x9F8B268B).s, false);
		int iHit = event->GetInt("hitgroup", false);

		if (iKiller == g_pEngineClient->GetLocalPlayer() && iVictim != g_pEngineClient->GetLocalPlayer())
		{
			BasePlayer* Ent = (BasePlayer*)g_pClientEntityList->GetClientEntity(iVictim);

			if (Ent)
			{
				hit_s kek;
				kek.idx = iVictim;
				kek.time = g_pGlobals->curtime + TICKS_TO_TIME(60);
				kek.dmg = iDamage;
				kek.pos = Ent->GetAbsOrigin();
				hit.push_back(kek);

				switch (g_CVars.Visuals.ESP.HitSound)
				{
				case 0: break;
				case 1: PlaySoundA(opa, NULL, SND_ASYNC | SND_MEMORY); break;
				case 2: PlaySoundA(Primordial, NULL, SND_ASYNC | SND_MEMORY); break;
				case 3: PlaySoundA(CoD_Hitsound, NULL, SND_ASYNC | SND_MEMORY); break;
				case 4: PlaySoundA(flick_wav, NULL, SND_ASYNC | SND_MEMORY); break;
				case 5: PlaySoundA(lox_wav, NULL, SND_ASYNC | SND_MEMORY); break;
				//case 6 :PlaySoundW("custom.waw",NULL,SND_ASYNC|SND_FILENAME)
				}
			}
		}
		if (iKiller == g_pEngineClient->GetLocalPlayer())
		{
			player_info_t killed_info;

			if (!g_pEngineClient->GetPlayerInfo(iVictim, &killed_info))
				return;

			std::string ss = ("[Apotheosis] ");
			ss += ("hit ");
			ss += killed_info.name;
			ss += " in the ";
			ss += get_hitgroup_name(iHit);
			ss += " for ";
			ss += event->GetString("dmg_health", false);
			ss += " damage (";
			ss += event->GetString("health", false);
			ss += " health remaining)\n";

			ConVar* Developer = g_pCvar->FindVar("developer");
			ConVar* con_filter_text = g_pCvar->FindVar("con_filter_text");
			ConVar* con_filter_enable = g_pCvar->FindVar("con_filter_enable");

			Developer->SetValue(1);
			con_filter_enable->SetValue(2);
			con_filter_text->SetValue("[Apotheosis] ");

			Msg(ss.c_str());
		}
	}
	if (strcmp(eventName, /*player_death*/XorStr<0x8A, 13, 0x411D0F9E>("\xFA\xE7\xED\xF4\xEB\xFD\xCF\xF5\xF7\xF2\xE0\xFD" + 0x411D0F9E).s) == 0)
	{
		int iKiller = g_pEngineClient->GetPlayerForUserID(event->GetInt( /*attacker*/XorStr<0x31, 9, 0xAC07C770>("\x50\x46\x47\x55\x56\x5D\x52\x4A" + 0xAC07C770).s, false));
		int iVictim = g_pEngineClient->GetPlayerForUserID(event->GetInt( /*userid*/XorStr<0xAA, 7, 0xAC29B4E9>("\xDF\xD8\xC9\xDF\xC7\xCB" + 0xAC29B4E9).s, false));

		if (iKiller == g_pEngineClient->GetLocalPlayer() && iVictim != g_pEngineClient->GetLocalPlayer())
		{
			switch (g_CVars.Visuals.ESP.KillSound)
			{
			case 0: break;
			case 1: PlaySoundA(opa, NULL, SND_ASYNC | SND_MEMORY); break;
			case 2: PlaySoundA(hitsound669e4, NULL, SND_ASYNC | SND_MEMORY); break;
			case 3: PlaySoundA(quakeiiihitsound, NULL, SND_ASYNC | SND_MEMORY); break;
			case 4: PlaySoundA(dubov_wav, NULL, SND_ASYNC | SND_MEMORY); break;
			}
			if (g_CVars.Miscellaneous.KillSay)
			{
				int pidoras = rand() % (0, 32);
				if (pidoras == 0)
					g_pEngineClient->ClientCmd(protect("say Apotheosis`ed!"));
				if (pidoras == 1)
					g_pEngineClient->ClientCmd(protect("say Apotheosis Rezolved u!"));

				if (pidoras == 2)
					g_pEngineClient->ClientCmd(protect("say Apotheosis on top!"));

				if (pidoras == 3)
					g_pEngineClient->ClientCmd(protect("say Apotheosis Beta!"));

				if (pidoras == 4)
					g_pEngineClient->ClientCmd(protect("say Apotheosis >> 93% cheats!"));

				if (pidoras == 5)
					g_pEngineClient->ClientCmd(protect("say Apotheosis`ed by Apotheosis`er!"));

				if (pidoras == 6)
					g_pEngineClient->ClientCmd(protect("say ^_^"));

				if (pidoras == 7)
					g_pEngineClient->ClientCmd(protect("say XD"));

				if (pidoras == 8)
					g_pEngineClient->ClientCmd(protect(u8"say (◣_◢`by_xaNe1337)"));

				if (pidoras == 9)
					g_pEngineClient->ClientCmd(protect(u8"say (◣_◢`1)"));

				if (pidoras == 10)
					g_pEngineClient->ClientCmd(protect(u8"say (◣_◢)"));

				if (pidoras == 11)
					g_pEngineClient->ClientCmd(protect("say Apotheosis? "));

				if (pidoras == 12)
					g_pEngineClient->ClientCmd(protect(u8"say Apotheosis (◣_◢)"));

				if (pidoras == 13)
					g_pEngineClient->ClientCmd(protect("say Apotheosis`ed u!"));

				if (pidoras == 14)
					g_pEngineClient->ClientCmd(protect("say BunnyHop Peeking abuse is forbidden. "));

				if (pidoras == 15)
					g_pEngineClient->ClientCmd(protect("say missware.cc"));

				if (pidoras == 16)
					g_pEngineClient->ClientCmd(protect("say wow"));

				if (pidoras == 17)
					g_pEngineClient->ClientCmd(protect("say Djamic.technologies"));

				if (pidoras == 18)
					g_pEngineClient->ClientCmd(protect("say by SANCHEZj hvh boss"));

				if (pidoras == 19)
					g_pEngineClient->ClientCmd(protect("say $ Resolver $ "));

				if (pidoras == 20)
					g_pEngineClient->ClientCmd(protect("say funny kill"));

				if (pidoras == 21)
					g_pEngineClient->ClientCmd(protect("say 4p0th30$1$"));

				if (pidoras == 22)
					g_pEngineClient->ClientCmd(protect("say lil the neel"));

				if (pidoras == 23)
					g_pEngineClient->ClientCmd(protect("say lol"));

				if (pidoras == 24)
					g_pEngineClient->ClientCmd(protect("say omg"));

				if (pidoras == 25)
					g_pEngineClient->ClientCmd(protect("say star(( - * - ))"));

				if (pidoras == 26)
					g_pEngineClient->ClientCmd(protect("say monster truck!"));

				if (pidoras == 27)
					g_pEngineClient->ClientCmd(protect("say insane"));

				if (pidoras == 28)
					g_pEngineClient->ClientCmd(protect("say uid police here present your user identification number right now"));

				if (pidoras == 29)
					g_pEngineClient->ClientCmd(protect("say ..."));
			}
		}
	}
	if (!strcmp(eventName, "bullet_impact")) bullet_impact(event);
}

void cGameEvent::RegisterSelf()
{
	g_pGameEventManager->AddListener(this, "bullet_impact", false);
	g_pGameEventManager->AddListener(this, /*round_start*/XorStr<0x4A, 12, 0x00451BD2>("\x38\x24\x39\x23\x2A\x10\x23\x25\x33\x21\x20" + 0x00451BD2).s, false);
	g_pGameEventManager->AddListener(this, /*player_hurt*/XorStr<0x40, 12, 0xF561EE05>("\x30\x2D\x23\x3A\x21\x37\x19\x2F\x3D\x3B\x3E" + 0xF561EE05).s, false);
	g_pGameEventManager->AddListener(this, /*player_death*/XorStr<0x82, 13, 0x7FA6859F>("\xF2\xEF\xE5\xFC\xE3\xF5\xD7\xED\xEF\xEA\xF8\xE5" + 0x7FA6859F).s, false);
	g_pGameEventManager->AddListener(this, /*player_connect*/XorStr<0x1F, 15, 0xEF243F3D>("\x6F\x4C\x40\x5B\x46\x56\x7A\x45\x48\x46\x47\x4F\x48\x58" + 0xEF243F3D).s, false);
}