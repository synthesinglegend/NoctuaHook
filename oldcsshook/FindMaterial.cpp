#include "Main.h"

bool bInGame = false;

std::vector< IMaterial* > vecSkyTextures;
std::vector< IMaterial* > vecPlayerTextures;
std::vector< IMaterial* > vecWeaponTextures;
std::vector< IMaterial* > vecMapTextures;

void SetAsusWalls(float flAlpha)
{
	for (unsigned int i = 0; i < vecMapTextures.size(); i++)
	{
		vecMapTextures[i]->ColorModulate(g_CVars.Visuals.NightMode, g_CVars.Visuals.NightMode, g_CVars.Visuals.NightMode);
		vecMapTextures[i]->AlphaModulate(flAlpha);
	}
}

void SetSkyChams(bool bDraw)
{
	for (unsigned int i = 0; i < vecSkyTextures.size(); i++)
	{
		if (bDraw) vecSkyTextures[i]->ColorModulate(0.01f, 0.01f, 0.01f);
	}
}

void SetPlayerChams(bool bWallHack, bool bWireFrame)
{
	for (unsigned int i = 0; i < vecPlayerTextures.size(); i++)
	{
		vecPlayerTextures[i]->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, bWallHack);
		vecPlayerTextures[i]->SetMaterialVarFlag(MATERIAL_VAR_ZNEARER, bWallHack);
		vecPlayerTextures[i]->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, bWireFrame);
	}
}

void SetWeaponChams(bool bWireFrame, bool bNoHand)
{
	for (unsigned int i = 0; i < vecWeaponTextures.size(); i++)
	{
		if (strstr(vecWeaponTextures[i]->GetName(), /*hand*/XorStr<0x10, 5, 0x3D1540CD>("\x78\x70\x7C\x77" + 0x3D1540CD).s))
		{
			vecWeaponTextures[i]->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, bNoHand);
			vecWeaponTextures[i]->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, bWireFrame);
		}
	}
}


void Visuals()
{
	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	if (!g_pEngineClient->IsInGame() || !g_pEngineClient->IsConnected() || !LocalPlayer) return;

	for (auto i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
	{
		IMaterial* material = g_pMaterialSystem->GetMaterial(i);

		if (!material) continue;

		if (strstr(material->GetTextureGroupName(), "World"))
		{
			material->SetMaterialVarFlag(MATERIAL_VAR_TRANSLUCENT, (g_CVars.Visuals.ASUS < 1.f) ? true : false);
			material->ColorModulate(g_CVars.Visuals.ASUS, g_CVars.Visuals.ASUS, g_CVars.Visuals.ASUS);
			material->AlphaModulate(g_CVars.Visuals.ASUS);
		}

		if (strstr(material->GetTextureGroupName(), "SkyBox textures"))
		{
			if (g_CVars.Visuals.NoSky) material->ColorModulate(0.01f, 0.01f, 0.01f);
			else material->ColorModulate(1.f, 1.f, 1.f);
		}

		if (strstr(material->GetName(), "hand"))
		{
			material->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_CVars.Visuals.NoHands);
		}
	}
}

typedef IMaterial* (__thiscall* FindMaterial_t)(void*, const char*, const char*, bool, const char*);
IMaterial* __fastcall Hooked_FindMaterial(void* ecx, void* edx, const char* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix)
{
	IMaterial* pTemp = MaterialSystemVMT->Function< FindMaterial_t >(27)(ecx, pMaterialName, pTextureGroupName, complain, pComplainPrefix);

	if (pMaterialName && pTextureGroupName)
	{
		if (g_pEngineClient->IsInGame())
		{
			if (g_CVars.Visuals.ASUS > 0.8) g_CVars.Visuals.ASUS = 1.0;
			SetAsusWalls(g_CVars.Visuals.ASUS);
			if (g_CVars.Visuals.ASUS < 0.9) g_CVars.Visuals.NoSky = true;
			SetSkyChams(g_CVars.Visuals.NoSky);
			SetWeaponChams(false, g_CVars.Visuals.NoHands);
			bInGame = true;
		}
		else
		{
			if (bInGame)
			{
				vecSkyTextures.clear();
				SetAsusWalls(1.0f);
				SetSkyChams(false);
				SetPlayerChams(false, false);
				SetWeaponChams(false, false);
				vecMapTextures.clear();
				bInGame = false;
			}

			if (strstr(pTextureGroupName, "SkyBox textures")) vecSkyTextures.push_back(pTemp);
			if (strstr(pTextureGroupName, "World") && !strstr(pMaterialName, "models\\player")) vecMapTextures.push_back(pTemp);
			if (strstr(pTextureGroupName, "Model texture") && strstr(pMaterialName, "models\\player")) vecPlayerTextures.push_back(pTemp);
			if (strstr(pTextureGroupName, "Model textures") && strstr(pMaterialName, "models\\weapons")) vecWeaponTextures.push_back(pTemp);
		}
	}

	return pTemp;
}