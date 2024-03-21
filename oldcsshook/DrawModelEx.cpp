#include "Main.h"

Color visible, invisible, outlinecolour;

int __stdcall Hooked_DrawModelEx(ModelRenderInfo_t& pInfo)
{
	ModelRenderVMT->SetHookEnabled(false);

	bool players = false;
	bool weapons = false;
	bool hands = false;
	bool arms = false;
	bool localplayerchams = false;

	static IMaterial* MatType[] =
	{
	mat_vertex_in = g_Stuff.CreateMaterial("VertexLitGeneric",
		{
			R"(
				"VertexLitGeneric"
                {
					"$basetexture" "vgui/white_additive"
					"$model" "1"
					"$ignorez" "1"
					"$nodecal" "1"
					"$selfillum" "1"
                    "$halflambert" "1"
				}"
			)"
		}),
	mat_vertex_out = g_Stuff.CreateMaterial("VertexLitGeneric",
		{
			R"(
            "VertexLitGeneric"
                {
					"$basetexture" "vgui/white_additive"
					"$model" "1"
					"$ignorez" "0"
					"$nodecal" "1"
					"$selfillum" "1"
                    "$halflambert" "1"
                }"
            )"
		}),
	mat_flat = g_Stuff.CreateMaterial("UnlitGeneric",
		{
			R"#(
            "UnlitGeneric"
                {
                    "$basetexture" "vgui/white_additive"
                    "$model" "1"
                    "$flat" "1"
                    "$nocull" "0"
                    "$selfillum" "1"
                    "$halflambert" "1"
                    "$nofog" "1"
                    "$ignorez" "1"
                }"
            )#"
		}),
	mat_outline = g_Stuff.CreateMaterial("UnlitGeneric",
		{
			R"#(
            "UnlitGeneric"
                {
                    "$basetexture" "vgui/white_additive"
                    "$model" "1"
                    "$flat" "1"
                    "$nocull" "0"
                    "$selfillum" "1"
                    "$halflambert" "1"
                    "$nofog" "1"
                    "$ignorez" "1"
                }"
            )#"
		})
	};

	auto LocalMaterial = MatType[g_CVars.Visuals.Chams.LocalMaterialType];
	auto EnemyMaterial = MatType[g_CVars.Visuals.Chams.EnemyMaterialType];
	auto WeaponMaterial = MatType[g_CVars.Visuals.Chams.WeaponMaterialType];
	auto HandMaterial = MatType[g_CVars.Visuals.Chams.HandMaterialType];

	std::string pszModelName = g_pModelInfo->GetModelName((model_t*)pInfo.pModel);

	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	BasePlayer* pModelEntity = (BasePlayer*)g_pClientEntityList->GetClientEntity(pInfo.entity_index);

	if (pModelEntity)
	{
		if (pModelEntity->m_iTeamNum() != LocalPlayer->m_iTeamNum())
		{
			if (pszModelName.find( /*models/player*/XorStr<0x62, 14, 0xE7AF524E>("\x0F\x0C\x00\x00\x0A\x14\x47\x19\x06\x0A\x15\x08\x1C" + 0xE7AF524E).s) != std::string::npos)
			{
				if (g_CVars.Visuals.Chams.EnemyChams)
				{
					g_Stuff.ForceMaterial((float)g_CVars.Visuals.Chams.EnemyChamsColor.r / 255, (float)g_CVars.Visuals.Chams.EnemyChamsColor.g / 255, (float)g_CVars.Visuals.Chams.EnemyChamsColor.b / 255, (float)g_CVars.Visuals.Chams.EnemyChamsColor.a / 255, EnemyMaterial, true, true);
					g_pModelRender->DrawModelEx(pInfo);
					players = true;
				}
			}
		}
	}

	if (pszModelName.find( /*models/weapons/w_*/XorStr<0xF0, 18, 0x6594C4FA>("\x9D\x9E\x96\x96\x98\x86\xD9\x80\x9D\x98\x8A\x94\x92\x8E\xD1\x88\x5F" + 0x6594C4FA).s) != std::string::npos)
	{
		if (g_CVars.Visuals.Chams.WeaponChams)
		{
			visible = g_CVars.ColorSelector.Chams.WpnVis;
			invisible = g_CVars.ColorSelector.Chams.WpnInvis;
			outlinecolour = g_CVars.ColorSelector.Chams.WpnOutline;

			if (g_CVars.Visuals.Chams.Outline)
			{
				mat_outline->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				g_Stuff.ForceMaterial((float)outlinecolour.r / 255, (float)outlinecolour.g / 255, (float)outlinecolour.b / 255, 1.f, mat_outline, true, true);
				g_pModelRender->DrawModelEx(pInfo);
				mat_outline->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			}

			g_Stuff.ForceMaterial((float)invisible.r / 255, (float)invisible.g / 255, (float)invisible.b / 255, 1.f, mat_flat, true, true);
			g_pModelRender->DrawModelEx(pInfo);
			g_Stuff.ForceMaterial((float)g_CVars.Visuals.Chams.WeaponChamsColor.r / 255, (float)g_CVars.Visuals.Chams.WeaponChamsColor.g / 255, (float)g_CVars.Visuals.Chams.WeaponChamsColor.b / 255, (float)g_CVars.Visuals.Chams.WeaponChamsColor.a / 255, WeaponMaterial, true, true);
			weapons = true;
		}
	}

	if (LocalPlayer)
	{
		if (LocalPlayer == pModelEntity)
		{
			if (pszModelName.find( /*models/player*/XorStr<0x62, 14, 0xE7AF524E>("\x0F\x0C\x00\x00\x0A\x14\x47\x19\x06\x0A\x15\x08\x1C" + 0xE7AF524E).s) != std::string::npos)
			{
				if (g_CVars.Visuals.Chams.LocalChams)
				{
					g_Stuff.ForceMaterial((float)g_CVars.Visuals.Chams.LocalChamsColor.r / 255, (float)g_CVars.Visuals.Chams.LocalChamsColor.g / 255, (float)g_CVars.Visuals.Chams.LocalChamsColor.b / 255, (float)g_CVars.Visuals.Chams.LocalChamsColor.a / 255, LocalMaterial, true, true);
					g_pModelRender->DrawModelEx(pInfo);
					localplayerchams = true;
				}
			}
		}
	}

	/*if (g_CVars.Visuals.Chams.HandsOutline)
	{
		if (pszModelName.find("models/weapons/v_") != std::string::npos)
		{
			invisible = (LocalPlayer->m_iTeamNum() == 2) ? g_CVars.ColorSelector.Chams.TTInvis : g_CVars.ColorSelector.Chams.CTInvis;

			if (g_CVars.Visuals.Chams.Outline)
			{
				outline->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				g_Stuff.ForceMaterial((float)invisible.r / 255, (float)invisible.g / 255, (float)invisible.b / 255, 1.f, outline, true, true);
				g_pModelRender->DrawModelEx(pInfo);
				outline->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			}

			g_Stuff.ForceMaterial(1.0f, 1.f, 1.f, 1.f, 0, true, true);
			g_pModelRender->DrawModelEx(pInfo);
			hands = true;
		}
	}*/

	if (g_CVars.Visuals.Chams.HandChams)
	{
		if (pszModelName.find("models/weapons/v_") != std::string::npos)
		{
			g_Stuff.ForceMaterial((float)g_CVars.Visuals.Chams.HandChamsColor.r / 255, (float)g_CVars.Visuals.Chams.HandChamsColor.g / 255, (float)g_CVars.Visuals.Chams.HandChamsColor.b / 255, (float)g_CVars.Visuals.Chams.HandChamsColor.a / 255, HandMaterial, true, true);
			g_pModelRender->DrawModelEx(pInfo);
			arms = true;
		}
	}

	int bRet = g_pModelRender->DrawModelEx(pInfo);

	if (players || weapons || hands || arms || localplayerchams) g_Stuff.ForceMaterial(1.f, 1.f, 1.f, 1.f, 0, true, true);

	ModelRenderVMT->SetHookEnabled(true);
	return bRet;
}