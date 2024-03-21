#include "Main.h"

Whitelist g_Whitelist;

void Whitelist::Init()
{

}

bool Whitelist::List(int index)
{
	if (index == g_pEngineClient->GetLocalPlayer()) return false;

	player_info_t pInfo;
	g_pEngineClient->GetPlayerInfo(index, &pInfo);

	std::string guid = pInfo.guid;

	if (strcmp(pInfo.name, protect("FeeD1337")) == 0) return true;
	else if (strcmp(pInfo.name, protect("vezdehod2115")) == 0) return true;
	return false;
}