#include "Main.h"

CConfig g_Config;

float GetPrivateProfileFloat(LPCSTR lpAppName, LPCSTR lpKeyName, FLOAT flDefault, LPCSTR lpFileName)
{
	char szData[32];
	GetPrivateProfileStringA(lpAppName, lpKeyName, std::to_string(flDefault).c_str(), szData, 32, lpFileName);
	return (float)atof(szData);
}

void WritePrivateProfileFloat(LPCSTR lpAppName, LPCSTR lpKeyName, FLOAT flValue, LPCSTR lpFileName)
{
	WritePrivateProfileStringA(lpAppName, lpKeyName, std::to_string((float)flValue).c_str(), lpFileName);
}

// =======================================================================================================================

int GetPrivateProfileInteger(LPCSTR lpAppName, LPCSTR lpKeyName, INT flDefault, LPCSTR lpFileName)
{
	char szData[32];
	GetPrivateProfileStringA(lpAppName, lpKeyName, std::to_string(flDefault).c_str(), szData, 32, lpFileName);
	return atoi(szData);
}

void WritePrivateProfileInteger(LPCSTR lpAppName, LPCSTR lpKeyName, INT flValue, LPCSTR lpFileName)
{
	WritePrivateProfileStringA(lpAppName, lpKeyName, std::to_string((int)flValue).c_str(), lpFileName);
}

// =======================================================================================================================

void GetPrivateProfileColor(LPCSTR lpAppName, LPCSTR lpKeyName, Color& cvar, LPCSTR lpFileName)
{
	char szData[32];
	char* red, * green, * blue, * alpha;
	GetPrivateProfileStringA(lpAppName, lpKeyName, "r0,g0,b0,a0", szData, 32, lpFileName);

	int len = strlen(szData);
	for (int i = 0; i < len; i++)
	{
		if (szData[i] == 'r' && szData[i + 2] == ',') red = &szData[i + 1];
		else if (szData[i] == 'r' && szData[i + 3] == ',') red = &szData[i + 1];
		else if (szData[i] == 'r' && szData[i + 4] == ',') red = &szData[i + 1]; 

		if (szData[i] == 'g' && szData[i + 2] == ',') green = &szData[i + 1];
		else if (szData[i] == 'g' && szData[i + 3] == ',') green = &szData[i + 1];
		else if (szData[i] == 'g' && szData[i + 4] == ',') green = &szData[i + 1];

		if (szData[i] == 'b') blue = &szData[i + 1];

		if (szData[i] == 'a') alpha = &szData[i + 1];
	}

	len = strlen(red);

	for (int i = 0; i < len; i++)
	{
		if (red[i] == ',' && red[i + 1] == 'g') red[i] = 0;
	}

	len = strlen(green);

	for (int i = 0; i < len; i++)
	{
		if (green[i] == ',' && green[i + 1] == 'b') green[i] = 0;
	}

	std::string r, g, b;
	r = red;
	g = green;
	b = blue;

	cvar = Color(atoi(r.c_str()), atoi(g.c_str()), atoi(b.c_str()), 255);
}

void WritePrivateProfileColor(LPCSTR lpAppName, LPCSTR lpKeyName, Color flValue, LPCSTR lpFileName)
{
	std::string colorstring;
	colorstring += "r";
	colorstring += std::to_string(flValue.r);
	colorstring += ",g";
	colorstring += std::to_string(flValue.g);
	colorstring += ",b";
	colorstring += std::to_string(flValue.b);
	colorstring += ",a";
	colorstring += std::to_string(flValue.a);
	WritePrivateProfileStringA(lpAppName, lpKeyName, colorstring.c_str(), lpFileName);
}


// =======================================================================================================================

void CConfig::SetModule(HMODULE hModule)
{
	m_hModule = hModule;
}


void CConfig::Load(std::string path)
{
	if (path.find(g_CVars.Miscellaneous.ConfigName) == std::string::npos)
		path.append(g_CVars.Miscellaneous.ConfigName);

	g_CVars.Aimbot.Active = GetPrivateProfileInteger("Aimbot", "Active", 0, path.c_str());
	g_CVars.Aimbot.AutoShoot = GetPrivateProfileInteger("Aimbot", "AutoShoot", 0, path.c_str());
	g_CVars.Aimbot.baimafterhp = GetPrivateProfileInteger("Aimbot", "baimafterhp", 0, path.c_str());
	g_CVars.Aimbot.baimafterhpslider = GetPrivateProfileInteger("Aimbot", "baimafterhpslider", 0, path.c_str());
	g_CVars.Aimbot.AutoWall = GetPrivateProfileInteger("Aimbot", "AutoWall", 0, path.c_str());
	g_CVars.Aimbot.MultiSpot = GetPrivateProfileInteger("Aimbot", "MultiSpot", 0, path.c_str());
	g_CVars.Aimbot.HitboxOwns = GetPrivateProfileInteger("Aimbot", "HitboxOwns", 0, path.c_str());
	g_CVars.Aimbot.HitScan = GetPrivateProfileInteger("Aimbot", "HitScan", 0, path.c_str());
	g_CVars.Aimbot.FriendlyFire = GetPrivateProfileInteger("Aimbot", "FriendlyFire", 0, path.c_str());
	g_CVars.Aimbot.TargetSelection = GetPrivateProfileInteger("Aimbot", "TargetSelection", 0, path.c_str());
	g_CVars.Aimbot.Hitbox = GetPrivateProfileInteger("Aimbot", "Hitbox", 0, path.c_str());
	g_CVars.Aimbot.HitboxMode = GetPrivateProfileInteger("Aimbot", "HitboxMode", 0, path.c_str());
	g_CVars.Aimbot.Silent = GetPrivateProfileInteger("Aimbot", "Silent", 0, path.c_str());
	g_CVars.Aimbot.PerfectSilent = GetPrivateProfileInteger("Aimbot", "PerfectSilent", 0, path.c_str());
	g_CVars.Aimbot.baiminair = GetPrivateProfileInteger("Aimbot", "baiminair", 0, path.c_str());
	g_CVars.Aimbot.baimincrouch = GetPrivateProfileInteger("Aimbot", "baimincrouch", 0, path.c_str());
	g_CVars.Aimbot.AntiSMAC = GetPrivateProfileInteger("Aimbot", "AntiSMAC", 0, path.c_str());
	g_CVars.Aimbot.BodyAWP = GetPrivateProfileInteger("Aimbot", "BodyAWP", 0, path.c_str());
	g_CVars.Aimbot.PointScale = GetPrivateProfileFloat("Aimbot", "PointScale", 0, path.c_str());
	g_CVars.Aimbot.SnapLimiter = GetPrivateProfileInteger("Aimbot", "SnapLimiter", 0, path.c_str());
	g_CVars.Aimbot.AngleLimit = GetPrivateProfileInteger("Aimbot", "AngleLimit", 0, path.c_str());
	g_CVars.Aimbot.AngleLimitTens = GetPrivateProfileFloat("Aimbot", "AngleLimitTens", 0, path.c_str());
	g_CVars.Aimbot.MinDamage = GetPrivateProfileInteger("Aimbot", "MinDamage", 0, path.c_str());
	g_CVars.Aimbot.Key = GetPrivateProfileInteger("Aimbot", "Key", 0, path.c_str());

	g_CVars.Aimbot.BodyAim = GetPrivateProfileInteger("Aimbot", "BodyAim", 0, path.c_str());

	g_CVars.Aimbot.BodyAimValue = GetPrivateProfileInteger("Aimbot", "BodyAimValue", 0, path.c_str());

	g_CVars.Aimbot.backtrackticks = GetPrivateProfileInteger("Aimbot", "backtrackticks", 0, path.c_str());
	g_CVars.Aimbot.forwardtrack = GetPrivateProfileFloat("Aimbot", "forwardtrack", 0, path.c_str());

	//g_CVars.Aimbot.Interpolation.LagPrediction = GetPrivateProfileInteger("Aimbot", "LagPrediction", 0, path.c_str());
//	g_CVars.Aimbot.Interpolation.PingReducer = GetPrivateProfileInteger("Aimbot", "PingReducer", 0, path.c_str());

//	g_CVars.Aimbot.Resolver.Active = GetPrivateProfileInteger("Resolver", "Active", 0, path.c_str());
//	g_CVars.Aimbot.Resolver.Smart = GetPrivateProfileInteger("Resolver", "Smart", 0, path.c_str());
//	g_CVars.Aimbot.Resolver.Mode = GetPrivateProfileInteger("Resolver", "Mode", 0, path.c_str());
//	g_CVars.Aimbot.Resolver.Type = GetPrivateProfileInteger("Resolver", "Type", 0, path.c_str());

	g_CVars.Triggerbot.Active = GetPrivateProfileInteger("Triggerbot", "Active", 0, path.c_str());
	g_CVars.Triggerbot.Seed = GetPrivateProfileInteger("Triggerbot", "Seed", 0, path.c_str());
	g_CVars.Triggerbot.Strength = GetPrivateProfileInteger("Triggerbot", "Strength", 0, path.c_str());
	g_CVars.Triggerbot.Key = GetPrivateProfileInteger("Triggerbot", "Key", 0, path.c_str());
	g_CVars.Triggerbot.Hitbox = GetPrivateProfileInteger("Triggerbot", "Hitbox", 0, path.c_str());
	g_CVars.Triggerbot.Spread = GetPrivateProfileInteger("Triggerbot", "Spread", 0, path.c_str());
	g_CVars.Triggerbot.Recoil = GetPrivateProfileInteger("Triggerbot", "Recoil", 0, path.c_str());

	g_CVars.Accuracy.ForceSeed = GetPrivateProfileInteger("Accuracy", "ForceSeed", 0, path.c_str());
	g_CVars.Accuracy.PerfectAccuracy = GetPrivateProfileInteger("Accuracy", "PerfectAccuracy", 0, path.c_str());
	g_CVars.Accuracy.NoSpread = GetPrivateProfileInteger("Accuracy", "NoSpreadMode", 1, path.c_str());

	g_CVars.Visuals.ESP.Box = GetPrivateProfileInteger("ESP", "Box", 0, path.c_str());
	g_CVars.Visuals.ESP.Fillbox = GetPrivateProfileInteger("ESP", "Fillbox", 0, path.c_str());
	g_CVars.Visuals.ESP.Name = GetPrivateProfileInteger("ESP", "Name", 0, path.c_str());
	g_CVars.Visuals.ESP.Health = GetPrivateProfileInteger("ESP", "Health", 0, path.c_str());
	g_CVars.Visuals.ESP.Weapon = GetPrivateProfileInteger("ESP", "Weapon", 0, path.c_str());
	g_CVars.Visuals.ESP.Bone = GetPrivateProfileInteger("ESP", "Bone", 0, path.c_str());
	g_CVars.Visuals.ESP.Armor = GetPrivateProfileInteger("ESP", "Armor", 0, path.c_str());
	g_CVars.Visuals.ESP.AimSpot = GetPrivateProfileInteger("ESP", "AimSpot", 0, path.c_str());
	g_CVars.Visuals.ESP.Hit = GetPrivateProfileInteger("ESP", "Hit", 0, path.c_str());
	g_CVars.Visuals.ESP.trails = GetPrivateProfileInteger("ESP", "trails", 0, path.c_str());
	g_CVars.Visuals.ESP.Ground = GetPrivateProfileInteger("ESP", "Ground", 0, path.c_str());
	g_CVars.Visuals.ESP.EnemyOnly = GetPrivateProfileInteger("ESP", "EnemyOnly", 0, path.c_str());
	g_CVars.Visuals.ESP.HitSound = GetPrivateProfileInteger("ESP", "HitSound", 0, path.c_str());
	g_CVars.Visuals.ESP.KillSound = GetPrivateProfileInteger("ESP", "KillSound", 0, path.c_str());

	g_CVars.Visuals.Chams.Active = GetPrivateProfileInteger("Chams", "Active", 0, path.c_str());
	g_CVars.Visuals.Chams.Weapons = GetPrivateProfileInteger("Chams", "Weapons", 0, path.c_str());
	g_CVars.Visuals.Chams.Shadows = GetPrivateProfileInteger("Chams", "Shadows", 0, path.c_str());
	g_CVars.Visuals.Chams.Outline = GetPrivateProfileInteger("Chams", "Outline", 0, path.c_str());
	g_CVars.Visuals.Chams.HandsOutline = GetPrivateProfileInteger("Chams", "HandsOutline", 0, path.c_str());
	g_CVars.Visuals.Chams.EnemyOnly = GetPrivateProfileInteger("Chams", "EnemyOnly", 0, path.c_str());

	g_CVars.Visuals.Crosshair.Type = GetPrivateProfileInteger("Crosshair", "Type", 0, path.c_str());
	g_CVars.Visuals.Crosshair.Dynamic = GetPrivateProfileInteger("Crosshair", "Dynamic", 0, path.c_str());

	g_CVars.Visuals.ASUS = GetPrivateProfileFloat("Visuals", "ASUS", 0, path.c_str());
	g_CVars.Visuals.NightMode = GetPrivateProfileFloat("Visuals", "NightMode", 0, path.c_str());
	g_CVars.Visuals.Radar = GetPrivateProfileInteger("Visuals", "Radar", 0, path.c_str());
	g_CVars.Visuals.NoSky = GetPrivateProfileInteger("Visuals", "NoSky", 0, path.c_str());
	g_CVars.Visuals.NoHands = GetPrivateProfileInteger("Visuals", "NoHands", 0, path.c_str());
	g_CVars.Visuals.NoSmoke = GetPrivateProfileInteger("Visuals", "NoSmoke", 0, path.c_str());
	g_CVars.Visuals.NoFlash = GetPrivateProfileInteger("Visuals", "NoFlash", 0, path.c_str());
	g_CVars.Visuals.NoVisualRecoil = GetPrivateProfileInteger("Visuals", "NoVisualRecoil", 0, path.c_str());
	g_CVars.Visuals.BulletTracer = GetPrivateProfileInteger("Visuals", "BulletTracer", 0, path.c_str());
	g_CVars.Visuals.ColorFog = GetPrivateProfileInteger("Visuals", "ColorFog", 0, path.c_str());
	g_CVars.Visuals.ViewModelFOV = GetPrivateProfileInteger("Visuals", "ViewModelFOV", 0, path.c_str());
	g_CVars.Visuals.Start = GetPrivateProfileInteger("Visuals", "Start", 0, path.c_str());
	g_CVars.Visuals.End = GetPrivateProfileInteger("Visuals", "End", 0, path.c_str());
	g_CVars.Visuals.Impact = GetPrivateProfileInteger("Visuals", "BulletImpact", 0, path.c_str());
	g_CVars.Visuals.Chams.LocalChams = GetPrivateProfileInteger("Visuals", "LocalChams", 0, path.c_str());
	g_CVars.Visuals.Chams.EnemyChams = GetPrivateProfileInteger("Visuals", "EnemyChams", 0, path.c_str());
	g_CVars.Visuals.Chams.HandChams = GetPrivateProfileInteger("Visuals", "HandChams", 0, path.c_str());
	g_CVars.Visuals.Chams.WeaponChams = GetPrivateProfileInteger("Visuals", "WeaponChams", 0, path.c_str());
	g_CVars.Visuals.Chams.LocalMaterialType = GetPrivateProfileInteger("Visuals", "LocalMaterialType", 0, path.c_str());
	g_CVars.Visuals.Chams.EnemyMaterialType = GetPrivateProfileInteger("Visuals", "EnemyMaterialType", 0, path.c_str());
	g_CVars.Visuals.Chams.HandMaterialType = GetPrivateProfileInteger("Visuals", "HandMaterialType", 0, path.c_str());
	g_CVars.Visuals.Chams.WeaponMaterialType = GetPrivateProfileInteger("Visuals", "WeaponMaterialType", 0, path.c_str());
	g_CVars.Visuals.Chams.ChamsChanger = GetPrivateProfileInteger("Visuals", "ChamsChanger", 0, path.c_str());
	g_CVars.Visuals.ESP.Active = GetPrivateProfileInteger("Visuals", "ESPActive", 0, path.c_str());
	g_CVars.Visuals.ESP.Fullbright = GetPrivateProfileInteger("Visuals", "FullBright", 0, path.c_str());

	g_CVars.Miscellaneous.AntiAim.Active = GetPrivateProfileInteger("AntiAim", "Active", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.Pitch = GetPrivateProfileInteger("AntiAim", "Pitch", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.Yaw = GetPrivateProfileInteger("AntiAim", "Yaw", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.lbybreaker = GetPrivateProfileInteger("AntiAim", "lby", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.Variation = GetPrivateProfileInteger("AntiAim", "Variation", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.WallDetectionMode = GetPrivateProfileInteger("AntiAim", "WallDetectionMode", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.Static = GetPrivateProfileInteger("AntiAim", "Static", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.WallDetection = GetPrivateProfileInteger("AntiAim", "WallDetection", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.AtTargets = GetPrivateProfileInteger("AntiAim", "AtTargets", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.opposite = GetPrivateProfileInteger("AntiAim", "opposite", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.lowdelta = GetPrivateProfileInteger("AntiAim", "lowdelta", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.DuckInAir = GetPrivateProfileInteger("AntiAim", "DuckInAir", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.RealValue = GetPrivateProfileFloat("AntiAim", "RealValue", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.RealJit = GetPrivateProfileFloat("AntiAim", "RealJit", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.RealJit2 = GetPrivateProfileFloat("AntiAim", "RealJit2", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.FakeJit = GetPrivateProfileFloat("AntiAim", "FakeJit", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.FakeJit2 = GetPrivateProfileFloat("AntiAim", "FakeJit2", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.PitchValue = GetPrivateProfileFloat("AntiAim", "PitchValue", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.FakeValue = GetPrivateProfileFloat("AntiAim", "FakeValue", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.TurnOff = GetPrivateProfileFloat("AntiAim", "EnemyCheck", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.RightManualValue = GetPrivateProfileFloat("AntiAim", "RightManual", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.inverterkey = GetPrivateProfileFloat("AntiAim", "inverterkey", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.LeftManualValue = GetPrivateProfileFloat("AntiAim", "LeftManual", 0, path.c_str());

	g_CVars.Miscellaneous.AntiAim.way = GetPrivateProfileFloat("AntiAim", "way", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.JitterReal = GetPrivateProfileFloat("AntiAim", "JitterReal", 0, path.c_str());
	g_CVars.Miscellaneous.AntiAim.JitterFake = GetPrivateProfileFloat("AntiAim", "JitterFake", 0, path.c_str());

	g_CVars.Miscellaneous.Fakelag.Active = GetPrivateProfileInteger("Fakelag", "Active", 0, path.c_str());
	g_CVars.Miscellaneous.Fakelag.Mode = GetPrivateProfileInteger("Fakelag", "Mode", 0, path.c_str());
	g_CVars.Miscellaneous.Fakelag.Value = GetPrivateProfileInteger("Fakelag", "Value", 0, path.c_str());
	g_CVars.Miscellaneous.Fakelag.InAttack = GetPrivateProfileInteger("Fakelag", "InAttack", 0, path.c_str());
	g_CVars.Miscellaneous.Fakelag.AirOnly = GetPrivateProfileInteger("Fakelag", "AirOnly", 0, path.c_str());

	g_CVars.Miscellaneous.BunnyHop = GetPrivateProfileInteger("Miscellaneous", "BunnyHop", 0, path.c_str());
	g_CVars.Miscellaneous.FastThrow = GetPrivateProfileInteger("Miscellaneous", "FastThrow", 0, path.c_str());
	g_CVars.Miscellaneous.AutoStrafe = GetPrivateProfileInteger("Miscellaneous", "AutoStrafe", 0, path.c_str());
	g_CVars.Miscellaneous.RotationValue = GetPrivateProfileInteger("Miscellaneous", "Rotate", 0, path.c_str());
	g_CVars.Miscellaneous.TickValue = GetPrivateProfileInteger("Miscellaneous", "Ticks", 0, path.c_str());
	g_CVars.Miscellaneous.CircleStrafe = GetPrivateProfileInteger("Miscellaneous", "CircleStrafe", 0, path.c_str());
	g_CVars.Miscellaneous.slowwalkvalue = GetPrivateProfileInteger("Miscellaneous", "slowwalkvalue", 0, path.c_str());
	g_CVars.Miscellaneous.slowwalkspeed = GetPrivateProfileInteger("Miscellaneous", "slowwalkspeed", 0, path.c_str());
	g_CVars.Miscellaneous.slowwalk = GetPrivateProfileInteger("Miscellaneous", "slowwalk", 0, path.c_str());
	g_CVars.Miscellaneous.EdgeJump = GetPrivateProfileInteger("Miscellaneous", "EdgeJump", 0, path.c_str());
	g_CVars.Miscellaneous.Speedhack = GetPrivateProfileInteger("Miscellaneous", "Speedhack", 0, path.c_str());
	g_CVars.Miscellaneous.SpeedhackValue = GetPrivateProfileInteger("Miscellaneous", "SpeedhackValue", 0, path.c_str());
	g_CVars.Miscellaneous.AutoKnife = GetPrivateProfileInteger("Miscellaneous", "AutoKnife", 0, path.c_str());
	g_CVars.Miscellaneous.RoundSay = GetPrivateProfileInteger("Miscellaneous", "RoundSay", 0, path.c_str());
	g_CVars.Miscellaneous.KillSay = GetPrivateProfileInteger("Miscellaneous", "KillSay", 0, path.c_str());
	g_CVars.Miscellaneous.CheatsBypass = GetPrivateProfileInteger("Miscellaneous", "CheatsBypass", 0, path.c_str());
	g_CVars.Miscellaneous.AirStuck = GetPrivateProfileInteger("Miscellaneous", "AirStuck", 0, path.c_str());
	g_CVars.Miscellaneous.FakeDuck = GetPrivateProfileInteger("Miscellaneous", "FakeDuck", 0, path.c_str());
	g_CVars.Miscellaneous.FakeDuckValue = GetPrivateProfileInteger("Miscellaneous", "FakeDuckKey", 0, path.c_str());
	g_CVars.Miscellaneous.CircleStrafeValue = GetPrivateProfileInteger("Miscellaneous", "CircleStrafeKey", 0, path.c_str());
	g_CVars.Miscellaneous.CircleStrafe = GetPrivateProfileInteger("Miscellaneous", "CircleStrafe", 0, path.c_str());
	g_CVars.Miscellaneous.RotationValue = GetPrivateProfileInteger("Miscellaneous", "RotationDivision", 0, path.c_str());
	g_CVars.Miscellaneous.TickValue = GetPrivateProfileInteger("Miscellaneous", "SimulationTick", 0, path.c_str());
	g_CVars.Miscellaneous.LagExploit = GetPrivateProfileInteger("Miscellaneous", "LagExploit", 0, path.c_str());
	g_CVars.Miscellaneous.LagExploitValue = GetPrivateProfileInteger("Miscellaneous", "LagExploitValue", 0, path.c_str());
	g_CVars.Miscellaneous.FakeWalkValue = GetPrivateProfileInteger("Miscellaneous", "FakeWalk", 0, path.c_str());


	g_CVars.Menu.x = GetPrivateProfileInteger("Miscellaneous", "Menu.x", 0, path.c_str());
	g_CVars.Menu.y = GetPrivateProfileInteger("Miscellaneous", "Menu.y", 0, path.c_str());

	g_CVars.Radar.x = GetPrivateProfileInteger("Miscellaneous", "Radar.x", 0, path.c_str());
	g_CVars.Radar.y = GetPrivateProfileInteger("Miscellaneous", "Radar.y", 0, path.c_str());

	GetPrivateProfileColor("Colors", "ESP.TT", g_CVars.ColorSelector.ESP.TT, path.c_str());
	GetPrivateProfileColor("Colors", "ESP.CT", g_CVars.ColorSelector.ESP.CT, path.c_str());
	GetPrivateProfileColor("Colors", "ESP.Wpn", g_CVars.ColorSelector.ESP.Wpn, path.c_str());



	g_CVars.Visuals.Chams.EnemyMaterialType = GetPrivateProfileInteger("Miscellaneous", "EnemyMaterialType", 0, path.c_str());

	g_CVars.Visuals.Chams.HandMaterialType = GetPrivateProfileInteger("Miscellaneous", "HandMaterialType", 0, path.c_str());

	g_CVars.Visuals.Chams.LocalMaterialType = GetPrivateProfileInteger("Miscellaneous", "LocalMaterialType", 0, path.c_str());

	g_CVars.Visuals.Chams.WeaponMaterialType = GetPrivateProfileInteger("Miscellaneous", "WeaponMaterialType", 0, path.c_str());


	GetPrivateProfileColor("Colors", "EnemyChamsColor", g_CVars.Visuals.Chams.EnemyChamsColor, path.c_str());
	GetPrivateProfileColor("Colors", "WeaponChamsColor", g_CVars.Visuals.Chams.WeaponChamsColor, path.c_str());
	GetPrivateProfileColor("Colors", "LocalChamsColor", g_CVars.Visuals.Chams.LocalChamsColor, path.c_str());
	GetPrivateProfileColor("Colors", "HandChamsColor", g_CVars.Visuals.Chams.HandChamsColor, path.c_str());

	GetPrivateProfileColor("Colors", "Chams.CTInvis", g_CVars.ColorSelector.Chams.CTInvis, path.c_str());
	GetPrivateProfileColor("Colors", "Chams.CTVis", g_CVars.ColorSelector.Chams.CTVis, path.c_str());
	GetPrivateProfileColor("Colors", "Chams.CTOutline", g_CVars.ColorSelector.Chams.CTOutline, path.c_str());

	GetPrivateProfileColor("Colors", "Chams.TTInvis", g_CVars.ColorSelector.Chams.TTInvis, path.c_str());
	GetPrivateProfileColor("Colors", "Chams.TTVis", g_CVars.ColorSelector.Chams.TTVis, path.c_str());
	GetPrivateProfileColor("Colors", "Chams.TTOutline", g_CVars.ColorSelector.Chams.TTOutline, path.c_str());

	GetPrivateProfileColor("Colors", "Chams.WpnInvis", g_CVars.ColorSelector.Chams.WpnInvis, path.c_str());
	GetPrivateProfileColor("Colors", "Chams.WpnVis", g_CVars.ColorSelector.Chams.WpnVis, path.c_str());
	GetPrivateProfileColor("Colors", "Chams.WpnOutline", g_CVars.ColorSelector.Chams.WpnOutline, path.c_str());
	GetPrivateProfileColor("Colors", "BulletColor", g_CVars.Visuals.BulletColor, path.c_str());
	GetPrivateProfileColor("Colors", "ImpactColor", g_CVars.Visuals.ImpactColor, path.c_str());
	GetPrivateProfileColor("Colors", "FogColor", g_CVars.Visuals.FogColor, path.c_str());
}

void CConfig::Save(std::string path)
{
	if (path.find(g_CVars.Miscellaneous.ConfigName) == std::string::npos)
		path.append(g_CVars.Miscellaneous.ConfigName);

	WritePrivateProfileInteger("Aimbot", "Active", g_CVars.Aimbot.Active, path.c_str());
	WritePrivateProfileInteger("Aimbot", "AutoShoot", g_CVars.Aimbot.AutoShoot, path.c_str());
	WritePrivateProfileInteger("Aimbot", "AutoWall", g_CVars.Aimbot.AutoWall, path.c_str());
	WritePrivateProfileInteger("Aimbot", "baimafterhpslider", g_CVars.Aimbot.baimafterhpslider, path.c_str());
	WritePrivateProfileInteger("Aimbot", "baimafterhp", g_CVars.Aimbot.baimafterhp, path.c_str());
	WritePrivateProfileInteger("Aimbot", "HitboxOwns", g_CVars.Aimbot.HitboxOwns, path.c_str());
	WritePrivateProfileInteger("Aimbot", "MultiSpot", g_CVars.Aimbot.MultiSpot, path.c_str());
	WritePrivateProfileInteger("Aimbot", "baiminair", g_CVars.Aimbot.baiminair, path.c_str());
	WritePrivateProfileInteger("Aimbot", "baimincrouch", g_CVars.Aimbot.baimincrouch, path.c_str());
	WritePrivateProfileInteger("Aimbot", "HitScan", g_CVars.Aimbot.HitScan, path.c_str());
	WritePrivateProfileInteger("Aimbot", "FriendlyFire", g_CVars.Aimbot.FriendlyFire, path.c_str());
	WritePrivateProfileInteger("Aimbot", "TargetSelection", g_CVars.Aimbot.TargetSelection, path.c_str());
	WritePrivateProfileInteger("Aimbot", "Hitbox", g_CVars.Aimbot.Hitbox, path.c_str());
	WritePrivateProfileInteger("Aimbot", "HitboxMode", g_CVars.Aimbot.HitboxMode, path.c_str());
	WritePrivateProfileInteger("Aimbot", "Silent", g_CVars.Aimbot.Silent, path.c_str());
	WritePrivateProfileInteger("Aimbot", "PerfectSilent", g_CVars.Aimbot.PerfectSilent, path.c_str());
	WritePrivateProfileInteger("Aimbot", "AntiSMAC", g_CVars.Aimbot.AntiSMAC, path.c_str());
	WritePrivateProfileInteger("Aimbot", "BodyAWP", g_CVars.Aimbot.BodyAWP, path.c_str());
	WritePrivateProfileFloat("Aimbot", "PointScale", g_CVars.Aimbot.PointScale, path.c_str());
	WritePrivateProfileInteger("Aimbot", "SnapLimiter", g_CVars.Aimbot.SnapLimiter, path.c_str());
	WritePrivateProfileInteger("Aimbot", "AngleLimit", g_CVars.Aimbot.AngleLimit, path.c_str());
	WritePrivateProfileInteger("Aimbot", "AngleLimitTens", g_CVars.Aimbot.AngleLimitTens, path.c_str());
	WritePrivateProfileInteger("Aimbot", "MinDamage", g_CVars.Aimbot.MinDamage, path.c_str());
	WritePrivateProfileInteger("Aimbot", "Key", g_CVars.Aimbot.Key, path.c_str());

//	WritePrivateProfileInteger("Aimbot", "LagPrediction", g_CVars.Aimbot.Interpolation.LagPrediction, path.c_str());

	//WritePrivateProfileInteger("Aimbot", "PingReducer", g_CVars.Aimbot.Interpolation.PingReducer, path.c_str());

	WritePrivateProfileInteger("Aimbot", "backtrackticks", g_CVars.Aimbot.backtrackticks, path.c_str());
	WritePrivateProfileFloat("Aimbot", "forwardtrack", g_CVars.Aimbot.forwardtrack, path.c_str());
	WritePrivateProfileInteger("Aimbot", "BodyAim", g_CVars.Aimbot.BodyAim, path.c_str());

	WritePrivateProfileInteger("Aimbot", "BodyAimValue", g_CVars.Aimbot.BodyAimValue, path.c_str());

//	WritePrivateProfileInteger("Resolver", "Active", g_CVars.Aimbot.Resolver.Active, path.c_str());
//	WritePrivateProfileInteger("Resolver", "Smart", g_CVars.Aimbot.Resolver.Smart, path.c_str());
//	WritePrivateProfileInteger("Resolver", "Mode", g_CVars.Aimbot.Resolver.Mode, path.c_str());
//	WritePrivateProfileInteger("Resolver", "Type", g_CVars.Aimbot.Resolver.Type, path.c_str());

	WritePrivateProfileInteger("Triggerbot", "Active", g_CVars.Triggerbot.Active, path.c_str());
	WritePrivateProfileInteger("Triggerbot", "Seed", g_CVars.Triggerbot.Seed, path.c_str());
	WritePrivateProfileInteger("Triggerbot", "Strength", g_CVars.Triggerbot.Strength, path.c_str());
	WritePrivateProfileInteger("Triggerbot", "Key", g_CVars.Triggerbot.Key, path.c_str());
	WritePrivateProfileInteger("Triggerbot", "Hitbox", g_CVars.Triggerbot.Hitbox, path.c_str());
	WritePrivateProfileInteger("Triggerbot", "Recoil", g_CVars.Triggerbot.Recoil, path.c_str());
	WritePrivateProfileInteger("Triggerbot", "Spread", g_CVars.Triggerbot.Spread, path.c_str());

	WritePrivateProfileInteger("Accuracy", "ForceSeed", g_CVars.Accuracy.ForceSeed, path.c_str());
	WritePrivateProfileInteger("Accuracy", "PerfectAccuracy", g_CVars.Accuracy.PerfectAccuracy, path.c_str());
	WritePrivateProfileInteger("Accuracy", "NoSpread", g_CVars.Accuracy.NoSpread, path.c_str());

	WritePrivateProfileInteger("ESP", "Box", g_CVars.Visuals.ESP.Box, path.c_str());
	WritePrivateProfileInteger("ESP", "Fillbox", g_CVars.Visuals.ESP.Fillbox, path.c_str());
	WritePrivateProfileInteger("ESP", "Name", g_CVars.Visuals.ESP.Name, path.c_str());
	WritePrivateProfileInteger("ESP", "Health", g_CVars.Visuals.ESP.Health, path.c_str());
	WritePrivateProfileInteger("ESP", "Weapon", g_CVars.Visuals.ESP.Weapon, path.c_str());
	WritePrivateProfileInteger("ESP", "Bone", g_CVars.Visuals.ESP.Bone, path.c_str());
	WritePrivateProfileInteger("ESP", "Armor", g_CVars.Visuals.ESP.Armor, path.c_str());
	WritePrivateProfileInteger("ESP", "AimSpot", g_CVars.Visuals.ESP.AimSpot, path.c_str());
	WritePrivateProfileInteger("ESP", "Hit", g_CVars.Visuals.ESP.Hit, path.c_str());
	WritePrivateProfileInteger("ESP", "trails", g_CVars.Visuals.ESP.trails, path.c_str());
	WritePrivateProfileInteger("ESP", "Ground", g_CVars.Visuals.ESP.Ground, path.c_str());
	WritePrivateProfileInteger("ESP", "EnemyOnly", g_CVars.Visuals.ESP.EnemyOnly, path.c_str());
	WritePrivateProfileInteger("ESP", "HitSound", g_CVars.Visuals.ESP.HitSound, path.c_str());
	WritePrivateProfileInteger("ESP", "KillSound", g_CVars.Visuals.ESP.KillSound, path.c_str());

	WritePrivateProfileInteger("Chams", "Active", g_CVars.Visuals.Chams.Active, path.c_str());
	WritePrivateProfileInteger("Chams", "Weapons", g_CVars.Visuals.Chams.Weapons, path.c_str());
	WritePrivateProfileInteger("Chams", "Shadows", g_CVars.Visuals.Chams.Shadows, path.c_str());
	WritePrivateProfileInteger("Chams", "Outline", g_CVars.Visuals.Chams.Outline, path.c_str());
	WritePrivateProfileInteger("Chams", "HandsOutline", g_CVars.Visuals.Chams.HandsOutline, path.c_str());
	WritePrivateProfileInteger("Chams", "EnemyOnly", g_CVars.Visuals.Chams.EnemyOnly, path.c_str());

	WritePrivateProfileInteger("Chams", "LocalMaterialType", g_CVars.Visuals.Chams.LocalMaterialType, path.c_str());
	WritePrivateProfileInteger("Chams", "EnemyMaterialType", g_CVars.Visuals.Chams.EnemyMaterialType, path.c_str());
	WritePrivateProfileInteger("Chams", "HandMaterialType", g_CVars.Visuals.Chams.HandMaterialType, path.c_str());
	WritePrivateProfileInteger("Chams", "WeaponMaterialType", g_CVars.Visuals.Chams.WeaponMaterialType, path.c_str());



	WritePrivateProfileColor("Colors", "EnemyChamsColor", g_CVars.Visuals.Chams.EnemyChamsColor, path.c_str());
	WritePrivateProfileColor("Colors", "LocalChamsColor", g_CVars.Visuals.Chams.LocalChamsColor, path.c_str());
	WritePrivateProfileColor("Colors", "HandChamsColor", g_CVars.Visuals.Chams.HandChamsColor, path.c_str());
	WritePrivateProfileColor("Colors", "WeaponChamsColor", g_CVars.Visuals.Chams.WeaponChamsColor, path.c_str());



	WritePrivateProfileInteger("Chams", "Shadows", g_CVars.Visuals.Chams.Shadows, path.c_str());
	WritePrivateProfileInteger("Chams", "Shadows", g_CVars.Visuals.Chams.Shadows, path.c_str());

	WritePrivateProfileInteger("Crosshair", "Type", g_CVars.Visuals.Crosshair.Type, path.c_str());
	WritePrivateProfileInteger("Crosshair", "Dynamic", g_CVars.Visuals.Crosshair.Dynamic, path.c_str());

	WritePrivateProfileFloat("Visuals", "ASUS", g_CVars.Visuals.ASUS, path.c_str());
	WritePrivateProfileFloat("Visuals", "NightMode", g_CVars.Visuals.NightMode, path.c_str());
	WritePrivateProfileInteger("Visuals", "Radar", g_CVars.Visuals.Radar, path.c_str());
	WritePrivateProfileInteger("Visuals", "NoSky", g_CVars.Visuals.NoSky, path.c_str());
	WritePrivateProfileInteger("Visuals", "NoHands", g_CVars.Visuals.NoHands, path.c_str());
	WritePrivateProfileInteger("Visuals", "NoSmoke", g_CVars.Visuals.NoSmoke, path.c_str());
	WritePrivateProfileInteger("Visuals", "NoFlash", g_CVars.Visuals.NoFlash, path.c_str());
	WritePrivateProfileInteger("Visuals", "NoVisualRecoil", g_CVars.Visuals.NoVisualRecoil, path.c_str());
	WritePrivateProfileInteger("Visuals", "BulletTracer", g_CVars.Visuals.BulletTracer, path.c_str());
	WritePrivateProfileInteger("Visuals", "ColorFog", g_CVars.Visuals.ColorFog, path.c_str());
	WritePrivateProfileInteger("Visuals", "ViewModelFOV", g_CVars.Visuals.ViewModelFOV, path.c_str());
	WritePrivateProfileInteger("Visuals", "Start", g_CVars.Visuals.Start, path.c_str());
	WritePrivateProfileInteger("Visuals", "End", g_CVars.Visuals.End, path.c_str());
	WritePrivateProfileInteger("Visuals", "BulletImpact", g_CVars.Visuals.Impact, path.c_str());
	WritePrivateProfileInteger("Visuals", "LocalChams", g_CVars.Visuals.Chams.LocalChams, path.c_str());
	WritePrivateProfileInteger("Visuals", "EnemyChams", g_CVars.Visuals.Chams.EnemyChams, path.c_str());
	WritePrivateProfileInteger("Visuals", "HandChams", g_CVars.Visuals.Chams.HandChams, path.c_str());
	WritePrivateProfileInteger("Visuals", "WeaponChams", g_CVars.Visuals.Chams.WeaponChams, path.c_str());
	WritePrivateProfileInteger("Visuals", "LocalMaterialType", g_CVars.Visuals.Chams.LocalChams, path.c_str());
	WritePrivateProfileInteger("Visuals", "EnemyMaterialType", g_CVars.Visuals.Chams.EnemyMaterialType, path.c_str());
	WritePrivateProfileInteger("Visuals", "HandMaterialType", g_CVars.Visuals.Chams.HandMaterialType, path.c_str());
	WritePrivateProfileInteger("Visuals", "WeaponMaterialType", g_CVars.Visuals.Chams.WeaponMaterialType, path.c_str());
	WritePrivateProfileInteger("Visuals", "ChamsChanger", g_CVars.Visuals.Chams.ChamsChanger, path.c_str());
	WritePrivateProfileInteger("Visuals", "ESPActive", g_CVars.Visuals.ESP.Active, path.c_str());
	WritePrivateProfileInteger("Visuals", "FullBright", g_CVars.Visuals.ESP.Fullbright, path.c_str());

	WritePrivateProfileInteger("AntiAim", "Active", g_CVars.Miscellaneous.AntiAim.Active, path.c_str());
	WritePrivateProfileInteger("AntiAim", "Pitch", g_CVars.Miscellaneous.AntiAim.Pitch, path.c_str());
	WritePrivateProfileInteger("AntiAim", "Yaw", g_CVars.Miscellaneous.AntiAim.Yaw, path.c_str());
	WritePrivateProfileInteger("AntiAim", "Variation", g_CVars.Miscellaneous.AntiAim.Variation, path.c_str());
	WritePrivateProfileInteger("AntiAim", "Static", g_CVars.Miscellaneous.AntiAim.Static, path.c_str());
	WritePrivateProfileInteger("AntiAim", "lby", g_CVars.Miscellaneous.AntiAim.lbybreaker, path.c_str());
	WritePrivateProfileInteger("AntiAim", "WallDetection", g_CVars.Miscellaneous.AntiAim.WallDetection, path.c_str());
	WritePrivateProfileInteger("AntiAim", "WallDetectionMode", g_CVars.Miscellaneous.AntiAim.WallDetectionMode, path.c_str());
	WritePrivateProfileInteger("AntiAim", "AtTargets", g_CVars.Miscellaneous.AntiAim.AtTargets, path.c_str());
	WritePrivateProfileInteger("AntiAim", "lowdelta", g_CVars.Miscellaneous.AntiAim.lowdelta, path.c_str());
	WritePrivateProfileInteger("AntiAim", "opposite", g_CVars.Miscellaneous.AntiAim.opposite, path.c_str());
	WritePrivateProfileInteger("AntiAim", "DuckInAir", g_CVars.Miscellaneous.AntiAim.DuckInAir, path.c_str());
	WritePrivateProfileFloat("AntiAim", "RealValue", g_CVars.Miscellaneous.AntiAim.RealValue, path.c_str());
	WritePrivateProfileFloat("AntiAim", "PitchValue", g_CVars.Miscellaneous.AntiAim.PitchValue, path.c_str());
	WritePrivateProfileFloat("AntiAim", "RealJit", g_CVars.Miscellaneous.AntiAim.RealJit, path.c_str());
	WritePrivateProfileFloat("AntiAim", "RealJit2", g_CVars.Miscellaneous.AntiAim.RealJit2, path.c_str());
	WritePrivateProfileFloat("AntiAim", "FakeJit2", g_CVars.Miscellaneous.AntiAim.FakeJit2, path.c_str());
	WritePrivateProfileFloat("AntiAim", "FakeJit", g_CVars.Miscellaneous.AntiAim.FakeJit, path.c_str());
	WritePrivateProfileFloat("AntiAim", "FakeValue", g_CVars.Miscellaneous.AntiAim.FakeValue, path.c_str());
	WritePrivateProfileFloat("AntiAim", "EnemyCheck", g_CVars.Miscellaneous.AntiAim.TurnOff, path.c_str());
	WritePrivateProfileFloat("AntiAim", "RightManual", g_CVars.Miscellaneous.AntiAim.RightManualValue, path.c_str());
	WritePrivateProfileFloat("AntiAim", "inverterkey", g_CVars.Miscellaneous.AntiAim.inverterkey, path.c_str());
	WritePrivateProfileFloat("Visuals", "ViewModelFOV", g_CVars.Visuals.ViewModelFOV, path.c_str());
	WritePrivateProfileFloat("AntiAim", "LeftManual", g_CVars.Miscellaneous.AntiAim.LeftManualValue, path.c_str());

	WritePrivateProfileFloat("AntiAim", "way", g_CVars.Miscellaneous.AntiAim.way, path.c_str());
	WritePrivateProfileFloat("AntiAim", "JitterReal", g_CVars.Miscellaneous.AntiAim.JitterReal, path.c_str());
	WritePrivateProfileFloat("AntiAim", "JitterFake", g_CVars.Miscellaneous.AntiAim.JitterFake, path.c_str());

	WritePrivateProfileInteger("FakeLag", "Active", g_CVars.Miscellaneous.Fakelag.Active, path.c_str());
	WritePrivateProfileInteger("FakeLag", "Mode", g_CVars.Miscellaneous.Fakelag.Mode, path.c_str());
	WritePrivateProfileInteger("FakeLag", "Value", g_CVars.Miscellaneous.Fakelag.Value, path.c_str());
	WritePrivateProfileInteger("FakeLag", "InAttack", g_CVars.Miscellaneous.Fakelag.InAttack, path.c_str());
	WritePrivateProfileInteger("FakeLag", "AirOnly", g_CVars.Miscellaneous.Fakelag.AirOnly, path.c_str());

	WritePrivateProfileInteger("Miscellaneous", "BunnyHop", g_CVars.Miscellaneous.BunnyHop, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "FastThrow", g_CVars.Miscellaneous.FastThrow, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "AutoStrafe", g_CVars.Miscellaneous.AutoStrafe, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "Rotate", g_CVars.Miscellaneous.RotationValue, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "Ticks", g_CVars.Miscellaneous.TickValue, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "CircleStrafe", g_CVars.Miscellaneous.CircleStrafe, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "EdgeJump", g_CVars.Miscellaneous.EdgeJump, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "Speedhack", g_CVars.Miscellaneous.Speedhack, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "SpeedhackValue", g_CVars.Miscellaneous.SpeedhackValue, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "AutoKnife", g_CVars.Miscellaneous.AutoKnife, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "RoundSay", g_CVars.Miscellaneous.RoundSay, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "KillSay", g_CVars.Miscellaneous.KillSay, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "CheatsBypass", g_CVars.Miscellaneous.CheatsBypass, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "FakeDuck", g_CVars.Miscellaneous.FakeDuck, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "LagExploit", g_CVars.Miscellaneous.LagExploit, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "LagExploitValue", g_CVars.Miscellaneous.LagExploitValue, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "slowwalkspeed", g_CVars.Miscellaneous.slowwalkspeed, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "slowwalkvalue", g_CVars.Miscellaneous.slowwalkvalue, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "slowwalk", g_CVars.Miscellaneous.slowwalk, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "FakeDuckKey", g_CVars.Miscellaneous.FakeDuckValue, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "CircleStrafeKey", g_CVars.Miscellaneous.CircleStrafeValue, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "CircleStrafe", g_CVars.Miscellaneous.CircleStrafe, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "RotationDivision", g_CVars.Miscellaneous.RotationValue, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "SimulationTick", g_CVars.Miscellaneous.TickValue, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "AirStuck", g_CVars.Miscellaneous.AirStuck, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "FakeWalk", g_CVars.Miscellaneous.FakeWalkValue, path.c_str());

	WritePrivateProfileInteger("Miscellaneous", "Menu.x", g_CVars.Menu.x, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "Menu.y", g_CVars.Menu.y, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "Radar.x", g_CVars.Radar.x, path.c_str());
	WritePrivateProfileInteger("Miscellaneous", "Radar.y", g_CVars.Radar.y, path.c_str());

	WritePrivateProfileColor("Colors", "ESP.TT", g_CVars.ColorSelector.ESP.TT, path.c_str());
	WritePrivateProfileColor("Colors", "ESP.CT", g_CVars.ColorSelector.ESP.CT, path.c_str());
	WritePrivateProfileColor("Colors", "ESP.Wpn", g_CVars.ColorSelector.ESP.Wpn, path.c_str());

	WritePrivateProfileColor("Colors", "Chams.CTInvis", g_CVars.ColorSelector.Chams.CTInvis, path.c_str());
	WritePrivateProfileColor("Colors", "Chams.CTVis", g_CVars.ColorSelector.Chams.CTVis, path.c_str());
	WritePrivateProfileColor("Colors", "Chams.CTOutline", g_CVars.ColorSelector.Chams.CTOutline, path.c_str());

	WritePrivateProfileColor("Colors", "Chams.TTInvis", g_CVars.ColorSelector.Chams.TTInvis, path.c_str());
	WritePrivateProfileColor("Colors", "Chams.TTVis", g_CVars.ColorSelector.Chams.TTVis, path.c_str());
	WritePrivateProfileColor("Colors", "Chams.TTOutline", g_CVars.ColorSelector.Chams.TTOutline, path.c_str());

	WritePrivateProfileColor("Colors", "Chams.WpnInvis", g_CVars.ColorSelector.Chams.WpnInvis, path.c_str());
	WritePrivateProfileColor("Colors", "Chams.WpnVis", g_CVars.ColorSelector.Chams.WpnVis, path.c_str());
	WritePrivateProfileColor("Colors", "Chams.WpnOutline", g_CVars.ColorSelector.Chams.WpnOutline, path.c_str());
	WritePrivateProfileColor("Colors", "BulletColor", g_CVars.Visuals.BulletColor, path.c_str());
	WritePrivateProfileColor("Colors", "ImpactColor", g_CVars.Visuals.ImpactColor, path.c_str());
	WritePrivateProfileColor("Colors", "FogColor", g_CVars.Visuals.FogColor, path.c_str());
}