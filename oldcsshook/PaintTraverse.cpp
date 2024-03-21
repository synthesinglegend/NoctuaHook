#include "Main.h"
#include "XorStr.h"
#include "menu.h"
float BOG_TO_GRD(float BOG) {
	return (180 / M_PI) * BOG;
}

float GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}

#include <cmath>
#include <vector>

// Функция преобразования градусов в радианы
#define GRD_TO_BOG(x) ((x) * M_PI / 180.0f)
// Функция преобразования радиан в градусы
#define BOG_TO_GRD(x) ((x) * 180.0f / M_PI)

// Структура цвета
struct Color1 {
	float r;
	float g;
	float b;
	float a;

	Color1(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) {}
};

// Функция рисования линии
void Line(int x1, int y1, int x2, int y2, Color1()) {
	// Здесь должен быть ваш код для рисования линии
}

// Функция рисования коловрата
void DrawKolovrat() {
	// Переменные для цветового эффекта
	static float x = 0, y = 0;
	static float r = 0, g = 0, b = 0;

	// Логика для цветового эффекта
	if (y >= 0.0f && y < 255.0f) {
		r = 255.0f;
		g = 0.0f;
		b = x;
	}
	else if (y >= 255.0f && y < 510.0f) {
		r = 255.0f - x;
		g = 0.0f;
		b = 255.0f;
	}
	else if (y >= 510.0f && y < 765.0f) {
		r = 0.0f;
		g = x;
		b = 255.0f;
	}
	else if (y >= 765.0f && y < 1020.0f) {
		r = 0.0f;
		g = 255.0f;
		b = 255.0f - x;
	}
	else if (y >= 1020.0f && y < 1275.0f) {
		r = x;
		g = 255.0f;
		b = 0.0f;
	}
	else if (y >= 1275.0f && y < 1530.0f) {
		r = 255.0f;
		g = 255.0f - x;
		b = 0.0f;
	}

	x += 0.75f;
	if (x >= 255.0f)
		x = 0.0f;

	y += 0.75f;
	if (y > 1530.0f)
		y = 0.0f;

	// Получение размеров экрана
	int screenWidth, screenHeight;
	g_pEngineClient->GetScreenSize(screenWidth, screenHeight);

	// Вычисление центра экрана
	POINT Middle;
	Middle.x = screenWidth / 2;
	Middle.y = screenHeight / 2;

	// Радиус коловрата
	int radius = screenHeight / 30;

	// Отрисовка коловрата
	for (int i = 0; i < 8; ++i) {
		int x1 = Middle.x + radius * cos(GRD_TO_BOG(i * 45));
		int y1 = Middle.y + radius * sin(GRD_TO_BOG(i * 45));
		int x2 = Middle.x + radius * cos(GRD_TO_BOG((i + 4) * 45));
		int y2 = Middle.y + radius * sin(GRD_TO_BOG((i + 4) * 45));
		g_Drawing.Line(x1, y1, x2, y2, Color(r, g, b, 255));
	}
}

void Crosshair()
{
	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	ConVar* crosshair = g_pCvar->FindVar( /*crosshair*/XorStr<0x6F, 10, 0xF1A82E3B>("\x0C\x02\x1E\x01\x00\x1C\x14\x1F\x05" + 0xF1A82E3B).s);

	int x = screen_x / 2;
	int y = screen_y / 2;

	int dy = screen_x / 90;
	int dx = screen_y / 90;

	Color crosshaircolor = Color(g_CVars.Colors.maincolor.r, g_CVars.Colors.maincolor.g, g_CVars.Colors.maincolor.b, 220);

	int iPunch = 0;

	if (g_CVars.Visuals.Crosshair.Dynamic)
	{
		if (g_CVars.Visuals.Crosshair.Type == 4) iPunch = (screen_y / 90) * LocalPlayer->GetPunchAngle().Length() * 2;
		else
		{
			x -= (dx * (LocalPlayer->GetPunchAngle().y));
			y += (dy * (LocalPlayer->GetPunchAngle().x));
		}
	}

	if (g_CVars.Visuals.Crosshair.Type == 1)
	{
		g_Drawing.FilledRect(x - 3, y - 1, 7, 3, Color(0, 0, 0, 128));
		g_Drawing.FilledRect(x - 1, y - 3, 3, 7, Color(0, 0, 0, 128));
		g_Drawing.FilledRect(x - 2, y, 5, 1, crosshaircolor);
		g_Drawing.FilledRect(x, y - 2, 1, 5, crosshaircolor);
	}
	else if (g_CVars.Visuals.Crosshair.Type == 2)
	{
		g_Drawing.FilledRect(x - 2, y - 2, 5, 5, crosshaircolor);
		g_Drawing.OutlinedRect(x - 2, y - 2, 5, 5, Color(0, 0, 0, 128));
	}
	if (g_CVars.Visuals.Crosshair.Type == 3)
	{
		/*
		g_Drawing.FilledRect( x - 2, y - 3, 5, 1, crosshaircolor );
		g_Drawing.FilledRect( x - 2, y + 3, 5, 1, crosshaircolor );
		g_Drawing.FilledRect( x - 3, y - 2, 1, 5, crosshaircolor );
		g_Drawing.FilledRect( x + 3, y - 2, 1, 5, crosshaircolor );
		*/

		g_Drawing.Circle(x + 1, y + 1, 4, 100, crosshaircolor);
	}
	if (g_CVars.Visuals.Crosshair.Type == 4)
	{
		// L33T RUSSKI RAINBOW OMG -------------------------------

		static float x = 0, y = 0;
		static float r = 0, g = 0, b = 0;

		if (y >= 0.0f && y < 255.0f) {
			r = 255.0f;
			g = 0.0f;
			b = x;
		}
		else if (y >= 255.0f && y < 510.0f) {
			r = 255.0f - x;
			g = 0.0f;
			b = 255.0f;
		}
		else if (y >= 510.0f && y < 765.0f) {
			r = 0.0f;
			g = x;
			b = 255.0f;
		}
		else if (y >= 765.0f && y < 1020.0f) {
			r = 0.0f;
			g = 255.0f;
			b = 255.0f - x;
		}
		else if (y >= 1020.0f && y < 1275.0f) {
			r = x;
			g = 255.0f;
			b = 0.0f;
		}
		else if (y >= 1275.0f && y < 1530.0f) {
			r = 255.0f;
			g = 255.0f - x;
			b = 0.0f;
		}

		x += 0.75f; //increase this value to switch colors faster
		if (x >= 255.0f)
			x = 0.0f;

		y += 0.75f; //increase this value to switch colors faster
		if (y > 1530.0f)
			y = 0.0f;


		// L33T RUSSKI RAINBOW OMG -------------------------------

		POINT Screen; int oodofdfo, jbjfdbdsf; g_pEngineClient->GetScreenSize((int)oodofdfo, (int)jbjfdbdsf); Screen.x = oodofdfo; Screen.y = jbjfdbdsf;
		//Middle POINT
		POINT Middle; Middle.x = (int)(Screen.x / 2); Middle.y = (int)(Screen.y / 2);
		int a = (int)(Screen.y / 2 / 30);
		float gamma = atan(a / a);
		static float faken_rot = 0.0f;
		faken_rot += 1.0f;
		if (faken_rot > 89.0f) faken_rot = 0.0f;
		int Drehungswinkel = faken_rot;

		int i = 0;
		while (i < 4)
		{
			std::vector <int> p;
			p.push_back(a * sin(GRD_TO_BOG(Drehungswinkel + (i * 90))));									//p[0]		p0_A.x
			p.push_back(a * cos(GRD_TO_BOG(Drehungswinkel + (i * 90))));									//p[1]		p0_A.y
			p.push_back((a / cos(gamma)) * sin(GRD_TO_BOG(Drehungswinkel + (i * 90) + BOG_TO_GRD(gamma))));	//p[2]		p0_B.x
			p.push_back((a / cos(gamma)) * cos(GRD_TO_BOG(Drehungswinkel + (i * 90) + BOG_TO_GRD(gamma))));	//p[3]		p0_B.y

			g_Drawing.Line(Middle.x, Middle.y, Middle.x + p[0], Middle.y - p[1], Color(r, g, b, 255));
			g_Drawing.Line(Middle.x + p[0], Middle.y - p[1], Middle.x + p[2], Middle.y - p[3], Color(r, g, b, 255));

			i++;
		}
	}
	if (g_CVars.Visuals.Crosshair.Type == 5)
	{
		DrawKolovrat();
	}
}
bool GetVisible(Vector& vStart2, Vector& vEnd2, BasePlayer* pEnt, BasePlayer* LocalPlayer)
{
	trace_t tr;
	Ray_t ray;
	Vector vStart, vEnd, vEndPos[3];
	vStart = vStart2;
	vEnd = vEnd2;
	TraceFilterSkipTwoEntities filter = TraceFilterSkipTwoEntities(LocalPlayer, pEnt);

	ray.Init(vStart, vEnd);
	g_pEngineTrace->TraceRay(ray, MASK_NPCWORLDSTATIC | CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX | CONTENTS_GRATE, &filter, &tr);

	if (tr.fraction == 1.f || tr.m_pEnt == pEnt) return true;

	vEndPos[0] = tr.endpos;

	ray.Init(vEnd, vStart);
	g_pEngineTrace->TraceRay(ray, MASK_NPCWORLDSTATIC | CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX | CONTENTS_GRATE, &filter, &tr);

	vEndPos[1] = tr.endpos;

	VectorSubtract(vEndPos[0], vEndPos[1], vEndPos[2]);

	float flLength = vEndPos[2].Length();

	if (tr.fraction != 0)
		if (flLength < 12)
			return true;

	return false;
}


void DrawHit(Vector pos, int y, Color color, int val)
{
	if (pos == Vector(0, 0, 0)) return;

	Vector screenSpace;
	g_Stuff.WorldToScreen(pos + Vector(0.f, 0.f, 72.f), screenSpace);
	g_Drawing.StringNormal(true, screenSpace.x, screenSpace.y + y, color, /*-%i*/XorStr<0x49, 4, 0x2884AAB7>("\x64\x6F\x22" + 0x2884AAB7).s, val);
}

void DrawHitmarker(Color color)
{
	int x = screen_x / 2;
	int y = screen_y / 2;

	for (int i = 4; i < 12; i++)
	{

		g_Drawing.FilledRect(x - i, y - i, 1, 1, color);
		g_Drawing.FilledRect(x - i, y + i, 1, 1, color);
		g_Drawing.FilledRect(x + i, y - i, 1, 1, color);
		g_Drawing.FilledRect(x + i, y + i, 1, 1, color);
	}
}

void HitESP()
{
	std::vector< hit_s >::iterator iter = hit.begin();

	for (int iHit = 0; iHit < (int)hit.size(); iHit++)
	{
		BasePlayer* Ent = (BasePlayer*)g_pClientEntityList->GetClientEntity(hit[iHit].idx);

		if (hit[iHit].time < g_pGlobals->curtime)
		{
			hit.erase(iter);
			continue;
		}

		if (hit[iHit].time >= g_pGlobals->curtime)
		{
			if (g_CVars.Visuals.ESP.Hit) DrawHit(hit[iHit].pos, -50 + (int)((hit[iHit].time - g_pGlobals->curtime)), Color(255, 0, 0, 255 * (TIME_TO_TICKS(hit[iHit].time - g_pGlobals->curtime)) / 85), hit[iHit].dmg);
		}

		iter++;
	}
}

void DrawHeader(int x, int y, int w, int r, int g, int b, int HealthBarWidth) //credits: GT3X/rifk
{
	Color colour;
	int i = 0;
	int xCoord = x;
	int yCoord = y;

	// Male Male :)
	for (i = 0; i < 5; i++)
	{
		g_Drawing.FilledRect(x - i, y + i, w, 1, Color(0, 0, 0, 255));
		g_Drawing.FilledRect(x - i, y + i, HealthBarWidth + 2, 1, Color(r, g, b, 255));
		g_Drawing.FilledRect(x - 3, y + 3, HealthBarWidth + 2, 1, Color((int)(r / 1.5f), (int)(g / 1.5f), (int)(b / 1.5f), 255));
		g_Drawing.FilledRect(x - 4, y + 4, HealthBarWidth + 2, 1, Color((int)(r / 1.5f), (int)(g / 1.5f), (int)(b / 1.5f), 255));
	}

	// Oben
	g_Drawing.FilledRect(x, y, w, 1, Color(255, 255, 255, 255));

	// Unten
	g_Drawing.FilledRect((x + 1) - 5, y + 5, w, 1, Color(255, 255, 255, 255));

	for (i = 0; i < 5; i++)
	{
		// Schr?g links
		g_Drawing.FilledRect(x, y, 1, 1, Color(255, 255, 255, 255));
		x--;
		y++;
	}

	x = xCoord;
	y = yCoord;

	for (i = 0; i < 5; i++)
	{
		// Schr?g rechts
		if (i != 0)
			g_Drawing.FilledRect(x + w, y, 1, 1, Color(255, 255, 255, 255));

		x--;
		y++;
	}
}

void DrawCornerBox(int x, int y, int w, int h, int borderPx, int r, int g, int b, int a)
{
	//Blackborder
	g_Drawing.FilledRect((x - (w / 2)) - 1, (y - h + borderPx) - 1, (w / 3) + 2, borderPx + 2, Color(0, 0, 0, 255)); //top
	g_Drawing.FilledRect((x - (w / 2) + w - w / 3) - 1, (y - h + borderPx) - 1, w / 3, borderPx + 2, Color(0, 0, 0, 255)); //top
	g_Drawing.FilledRect(x - (w / 2) - 1, (y - h + borderPx), borderPx + 2, (w / 3) + 1, Color(0, 0, 0, 255)); //left 
	g_Drawing.FilledRect(x - (w / 2) - 1, ((y - h + borderPx) + h - w / 3) - 1, borderPx + 2, (w / 3) + 2, Color(0, 0, 0, 255)); //left 
	g_Drawing.FilledRect(x - (w / 2), y - 1, (w / 3) + 1, borderPx + 2, Color(0, 0, 0, 255)); //bottom 
	g_Drawing.FilledRect(x - (w / 2) + w - (w / 3 + 1), y - 1, (w / 3) + 2, borderPx + 2, Color(0, 0, 0, 255)); //bottom 
	g_Drawing.FilledRect((x + w - borderPx) - (w / 2) - 1, (y - h + borderPx) - 1, borderPx + 2, w / 3 + 2, Color(0, 0, 0, 255));; //right 
	g_Drawing.FilledRect((x + w - borderPx) - (w / 2) - 1, ((y - h + borderPx) + h - w / 3) - 1, borderPx + 2, (w / 3) + 2, Color(0, 0, 0, 255)); //right 
	//Color
	g_Drawing.FilledRect(x - (w / 2), (y - h + borderPx), w / 3, borderPx, Color(r, g, b, a)); //top
	g_Drawing.FilledRect(x - (w / 2) + w - w / 3, (y - h + borderPx), w / 3, borderPx, Color(r, g, b, a)); //top
	g_Drawing.FilledRect(x - (w / 2), (y - h + borderPx), borderPx, w / 3, Color(r, g, b, a));//left 
	g_Drawing.FilledRect(x - (w / 2), (y - h + borderPx) + h - w / 3, borderPx, w / 3, Color(r, g, b, a)); //left 
	g_Drawing.FilledRect(x - (w / 2), y, w / 3, borderPx, Color(r, g, b, a)); //bottom 
	g_Drawing.FilledRect(x - (w / 2) + w - w / 3, y, w / 3, borderPx, Color(r, g, b, a)); //bottom 
	g_Drawing.FilledRect((x + w - borderPx) - (w / 2), (y - h + borderPx), borderPx, w / 3, Color(r, g, b, a)); //right 
	g_Drawing.FilledRect((x + w - borderPx) - (w / 2), (y - h + borderPx) + h - w / 3, borderPx, w / 3, Color(r, g, b, a)); //right 
}

static int iAlpha[64];

void BoundingBoxESP()
{
	Color visible, invisible;
	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	if (!LocalPlayer) return;

	if (!g_CVars.Visuals.ESP.Box
		&& !g_CVars.Visuals.ESP.Name
		&& !g_CVars.Visuals.ESP.Health
		&& !g_CVars.Visuals.ESP.Armor
		&& !g_CVars.Visuals.ESP.Weapon
		&& !g_CVars.Visuals.ESP.Bone
	//	&& !g_CVars.Visuals.ESP.HistoryTicks
		&& !g_CVars.Visuals.ESP.Hit
	//	&& !g_CVars.Visuals.ESP.Hitmarker
		&& !g_CVars.Visuals.ESP.AimSpot) return;

	player_info_t PlayerInfo;
	Color colour;

	for (int Index = 1; Index <= g_pGlobals->maxClients; Index++)
	{
		BasePlayer* Ent = (BasePlayer*)g_pClientEntityList->GetClientEntity(Index);
		if (!Ent || Ent == LocalPlayer) continue;

		float fps = 1 / g_pGlobals->frametime;
		int step = int(500 / fps);

		//	if (Ent->IsDormant() && iAlpha[Index] > 0) iAlpha[Index] -= step;
		//	else if (iAlpha[Index] < 130 && !Ent->IsDormant()) iAlpha[Index] += step;

		//	if (Ent->IsDormant()) continue;
		if (!(Ent->m_lifeState() == 0)) continue;
		if (g_CVars.Visuals.ESP.EnemyOnly)
		{
			if (Ent->m_iTeamNum() == LocalPlayer->m_iTeamNum()) continue;
		}

		Vector vPlayerFoot, vPlayerFootScreen, vPlayerHead, vPlayerHeadScreen;

		if (g_CVars.PlayerList.Friend[Index])
		{
			colour = Color(255, 255, 255, 255);
		}
		else
		{
			if (Ent->m_iTeamNum() == 2) colour = g_CVars.ColorSelector.ESP.TT; //Color::Red( );
			else if (Ent->m_iTeamNum() == 3) colour = g_CVars.ColorSelector.ESP.CT; //Color::LightBlue( );
		}

		vPlayerFoot = Ent->GetAbsOrigin();
		bool bDucking = Ent->m_fFlags() & FL_DUCKING;
		if (bDucking) vPlayerHead = vPlayerFoot + Vector(0.f, 0.f, 53.5f);
		else vPlayerHead = vPlayerFoot + Vector(0.f, 0.f, 72.f);

		if (!g_Stuff.WorldToScreen(vPlayerFoot, vPlayerFootScreen)) continue;
		if (!g_Stuff.WorldToScreen(vPlayerHead, vPlayerHeadScreen)) continue;
		if (g_CVars.Visuals.ESP.Active)
		{
			//int best_record = g_LagCompensation.bestrecord[Index];


		//	if (g_CVars.Visuals.ESP.HistoryTicks)
		//	{
		//		if (Ent->m_vecVelocity().Length2D() > 5)
		//		{
		//			g_Drawing.DrawBones(Ent, Color(255, 220, 80, clamp(iAlpha[Index], 0, 96)), 4);
		//			g_Drawing.DrawBones(Ent, Color(255, 220, 80, clamp(iAlpha[Index], 0, 112)), 3);
		//			g_Drawing.DrawBones(Ent, Color(255, 220, 80, clamp(iAlpha[Index], 0, 128)), 2);
		////			g_Drawing.DrawBones(Ent, Color(255, 220, 80, clamp(iAlpha[Index], 0, 144)), 1);
		//			g_Drawing.DrawBones(Ent, Color(255, 220, 80, clamp(iAlpha[Index], 0, 160)), best_record);
		//		}
		//	}
		//	if (g_CVars.Visuals.ESP.Bone)
		//	{
	//
		//		g_Drawing.DrawBones(Ent, Color(255, 255, 255, iAlpha[Index]), -1);
		//	}



			if (g_CVars.PlayerList.Friend[Index])
			{
				colour = Color(255, 255, 255, 255);
			}
			//	else
			//	{
			//		if (GetVisible(LocalPlayer->EyePosition(), Ent->EyePosition(), Ent, LocalPlayer))
			//		{
			//			if (Ent->m_iTeamNum() == 2)
			//			{
			//				colour = g_CVars.ColorSelector.ESP.TT; //Color::Red( );
			//			}
			//			else if (Ent->m_iTeamNum() == 3)
			///			{
			//				colour = g_CVars.ColorSelector.ESP.CT; //Color::Red( );
			//			}
		//			}
		//		}




			float Height = vPlayerFootScreen.y - vPlayerHeadScreen.y;
			float HalfWidth = Height * .225f;
			if (bDucking) HalfWidth *= 1.345794392523364f;

			Vector box = Vector((vPlayerHeadScreen.x - HalfWidth), vPlayerHeadScreen.y, 0.f);

			if (g_CVars.Visuals.ESP.Box == 1)
			{
				int WidthThird = HalfWidth / 2;
				int HeightThird = WidthThird;


				g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth - (1), vPlayerHeadScreen.y - (1), WidthThird + 2, 3, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b)); //h
				g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth - (1), vPlayerHeadScreen.y - (1), 3, HeightThird + 2, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b)); //h


				g_Drawing.FilledRect((vPlayerHeadScreen.x + HalfWidth) - WidthThird - (1), vPlayerHeadScreen.y - (1), WidthThird + 2, 3, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b)); //h
				g_Drawing.FilledRect(vPlayerHeadScreen.x + HalfWidth - (1), vPlayerHeadScreen.y - (1), 3, HeightThird + 2, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b)); //h


				g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth - (1), vPlayerHeadScreen.y + Height - 4, WidthThird + 2, 3, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b)); //h
				g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth - (1), (vPlayerHeadScreen.y + Height) - HeightThird - 4, 3, HeightThird + 2, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b)); //h


				g_Drawing.FilledRect((vPlayerHeadScreen.x + HalfWidth) - WidthThird - (1), vPlayerHeadScreen.y + Height - 4, WidthThird + 2, 3, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b)); //h
				g_Drawing.FilledRect(vPlayerHeadScreen.x + HalfWidth - (1), (vPlayerHeadScreen.y + Height) - HeightThird - 4, 3, HeightThird + 3, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b)); //h


				g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth, vPlayerHeadScreen.y, WidthThird, 1, colour); //h
				g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth, vPlayerHeadScreen.y, 1, HeightThird, colour); //v


				g_Drawing.FilledRect((vPlayerHeadScreen.x + HalfWidth) - WidthThird, vPlayerHeadScreen.y, WidthThird, 1, colour);
				g_Drawing.FilledRect(vPlayerHeadScreen.x + HalfWidth, vPlayerHeadScreen.y, 1, HeightThird, colour);


				g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth, vPlayerHeadScreen.y + Height - 3, WidthThird, 1, colour);
				g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth, (vPlayerHeadScreen.y + Height) - HeightThird - 3, 1, HeightThird, colour);


				g_Drawing.FilledRect((vPlayerHeadScreen.x + HalfWidth) - WidthThird, vPlayerHeadScreen.y + Height - 3, WidthThird, 1, colour);
				g_Drawing.FilledRect(vPlayerHeadScreen.x + HalfWidth, (vPlayerHeadScreen.y + Height) - HeightThird - 3, 1, HeightThird + 1, colour);
			}
			if (g_CVars.Visuals.ESP.Box == 2)
			{
				//Vector box = Vector((vPlayerHeadScreen.x - HalfWidth), vPlayerHeadScreen.y, 0.f);
				g_Drawing.OutlinedBox(box.x, box.y, (HalfWidth * 2), Height, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b, 160), Color(0, 0, 0, 128));
			}
			/*
					else if (g_CVars.Visuals.ESP.Box == 2)
					{
						int WidthThird = HalfWidth / 2;
						int HeightThird = WidthThird;


						g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth - (1), vPlayerHeadScreen.y - (1), WidthThird + 2, 3, Color::Black()); //h
						g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth - (1), vPlayerHeadScreen.y - (1), 3, HeightThird + 2, Color::Black()); //h


						g_Drawing.FilledRect((vPlayerHeadScreen.x + HalfWidth) - WidthThird - (1), vPlayerHeadScreen.y - (1), WidthThird + 2, 3, Color::Black()); //h
						g_Drawing.FilledRect(vPlayerHeadScreen.x + HalfWidth - (1), vPlayerHeadScreen.y - (1), 3, HeightThird + 2, Color::Black()); //h


						g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth - (1), vPlayerHeadScreen.y + Height - 4, WidthThird + 2, 3, Color::Black()); //h
						g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth - (1), (vPlayerHeadScreen.y + Height) - HeightThird - 4, 3, HeightThird + 2, Color::Black()); //h


						g_Drawing.FilledRect((vPlayerHeadScreen.x + HalfWidth) - WidthThird - (1), vPlayerHeadScreen.y + Height - 4, WidthThird + 2, 3, Color::Black()); //h
						g_Drawing.FilledRect(vPlayerHeadScreen.x + HalfWidth - (1), (vPlayerHeadScreen.y + Height) - HeightThird - 4, 3, HeightThird + 3, Color::Black()); //h


						g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth, vPlayerHeadScreen.y, WidthThird, 1, colour); //h
						g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth, vPlayerHeadScreen.y, 1, HeightThird, colour); //v


						g_Drawing.FilledRect((vPlayerHeadScreen.x + HalfWidth) - WidthThird, vPlayerHeadScreen.y, WidthThird, 1, colour);
						g_Drawing.FilledRect(vPlayerHeadScreen.x + HalfWidth, vPlayerHeadScreen.y, 1, HeightThird, colour);


						g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth, vPlayerHeadScreen.y + Height - 3, WidthThird, 1, colour);
						g_Drawing.FilledRect(vPlayerHeadScreen.x - HalfWidth, (vPlayerHeadScreen.y + Height) - HeightThird - 3, 1, HeightThird, colour);


						g_Drawing.FilledRect((vPlayerHeadScreen.x + HalfWidth) - WidthThird, vPlayerHeadScreen.y + Height - 3, WidthThird, 1, colour);
						g_Drawing.FilledRect(vPlayerHeadScreen.x + HalfWidth, (vPlayerHeadScreen.y + Height) - HeightThird - 3, 1, HeightThird + 1, colour);


					}
					*/


			

			if (g_CVars.Visuals.ESP.AimSpot) g_Drawing.DrawAimSpot(Ent, g_CVars.Aimbot.Hitbox, Color(255, 255, 255, iAlpha[Index]));

			g_pEngineClient->GetPlayerInfo(Index, &PlayerInfo);
			auto animstate = Ent->GetAnimstate();
			if (g_CVars.Visuals.ESP.Name)
			{
				//if( g_Stuff.Timeout[ Ent->entindex( ) ] ) g_Drawing.MenuStringNormal( true, false, box.x + HalfWidth, box.y - 26, Color( 255, 0, 0, 200 ), /*TIMED OUT!*/XorStr<0x80,11,0xDCDD6DA1>("\xD4\xC8\xCF\xC6\xC0\xA5\xC9\xD2\xDC\xA8"+0xDCDD6DA1).s );
				if (g_CVars.PlayerList.Friend[Index]) g_Drawing.MenuStringBold(true, false, box.x + HalfWidth, box.y - 13, Color(255, 255, 255, 255), /*Friend: %s*/XorStr<0xE1, 11, 0x9B2BE55F>("\xA7\x90\x8A\x81\x8B\x82\xDD\xC8\xCC\x99" + 0x9B2BE55F).s, PlayerInfo.name);
				else g_Drawing.MenuStringNormal(true, false, box.x + HalfWidth, box.y - 13, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b), xorstr(u8"%s"), PlayerInfo.name);
			}
			if (g_CVars.Visuals.ESP.Fillbox) g_Drawing.FilledRect(box.x, box.y, (HalfWidth * 2), Height, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b, 55));
			CTickRecord* current = &pPlayerHistory[Index][0];
			CTickRecord* previous = &pPlayerHistory[Index][1];
			//animstate->
			if (Ent->m_bIsDefusing())
			{
				g_Drawing.MenuStringNormal(true, false, box.x + HalfWidth, box.y - 16, Color(0, 255, 0, 255), "defusing");
			}
			if (Ent->m_iHealth() <= 100)
			{
				g_Drawing.MenuStringNormal(false, false, box.x - 26, box.y, Color(0, 255, 24, 255), "%i", Ent->m_iHealth());
			}
			if (Ent->m_ArmorValue() > 0)
			{
				g_Drawing.MenuStringNormal(false, false, box.x - 26, box.y + 12, Color(161, 248, 251, 255), "%i", Ent->m_ArmorValue());
			}
			if (g_CVars.Visuals.ESP.steamid)
			{
				//debug steamid fo ban lameass on BIG CEO MM HVH
				g_Drawing.MenuStringNormal(false, false, box.x + HalfWidth, box.y - 24, Color(0, 255, 0, 255), "%s", PlayerInfo.guid);
			}
			if (Ent->m_bHasDefuser())
			{
				g_Drawing.MenuStringNormal(false, false, box.x + (HalfWidth * 2) + 3, box.y + 8, Color(0, 255, 0, 255), "defuser");
			}
			g_Drawing.MenuStringNormal(false, false, box.x + (HalfWidth * 2) + 3, box.y, Color(0, 255, 0, 255), "$%i", Ent->m_iAccount()); //just box.y
			//g_Drawing.MenuStringNormal(false, false, box.x + (HalfWidth * 2) + 3, box.y + 8, Color(0, 255, 0, 255), "defuser", Ent->m_bHasDefuser()); //just box.y
			//g_Drawing.MenuStringNormal(false, false, box.x + (HalfWidth * 2) + 3, box.y, Color(251, 164, 161, 255), "FL:%i", (TIME_TO_TICKS(current->m_SimulationTime - previous->m_SimulationTime)));// box.y + 8

			//bool lcbreak = ((current->m_Origin - previous->m_Origin).LengthSqr() > 4096.f);
		//	if (lcbreak) g_Drawing.MenuStringNormal(false, false, box.x + (HalfWidth * 2) + 3, box.y, g_LagCompensation.CheckDelta(current) ? Color(255, 0, 0, 255) : Color(0, 255, 0, 255), "LC");

			//g_Drawing.MenuStringNormal(false, false, box.x + (HalfWidth * 2) + 3, box.y + 13, Color(255, 255, 255, iAlpha[Index]), "%i", best_record);

			int Health = Ent->m_iHealth();
			if (Health > 0)
			{
				int maxhp = 100;
				int Scale = Health * 2.55f;
				Color patch(0, 0, 0, iAlpha[Index]);
				if (Health > 100)
				{
					Color(0, 255, 255, 233);
					Health = 100;
				}

				if (g_CVars.Visuals.ESP.Health)
				{
					int left = 0;
					if (Ent->m_bHasHelmet())
					{
						//g_Drawing.MenuStringNormal(2, false, box.x - 10, box.y, Color(255, 255, 255, 255), "H");
						left += 13;
					}
					//	if(Ent->m_ArmorValue() > 0) g_Drawing.MenuStringNormal(2, false, box.x - 10, box.y + left, Color(255, 255, 255, 255), "K");



					box.y += Height + 3;




					//DrawHeader(vPlayerHeadScreen.x - HalfWidth + 2, vPlayerFootScreen.y + 3, HalfWidth * 2, 104, 130, 255, (Health / (100 / (HalfWidth * 2)) - 2));

					g_Drawing.OutlinedRect(box.x - 1, box.y, (HalfWidth * 2) + 2, 4, patch);
					g_Drawing.FilledRect(box.x, box.y, (HalfWidth * 2), 2, patch);
					g_Drawing.FilledRect(box.x, box.y, ((Health / (double)maxhp) * (HalfWidth * 2)), 2, Color(0, 255, 24, 255));


				}
			}
			static Vector duckOffset = Vector(0.f, 0.f, 53.5f);
			static Vector standOffset = Vector(0.f, 0.f, 72.f);
			Vector offset = (Ent->m_fFlags() & FL_DUCKING) ? duckOffset : standOffset;
			Vector max = (Ent->GetAbsOrigin() + offset);
			Vector top;

			if (!g_Stuff.WorldToScreen(max, top))
			{
				return;
			}

			if (g_CVars.Visuals.ESP.Armor)
			{
				float fArmor = min(Ent->m_ArmorValue(), 100);

				if (fArmor > 0)
				{
					int iScale = (fArmor * 2.55f);

					Color patch(85, 85, 85, 255);

					if (Ent->m_bHasHelmet())
					{
						Color(0, 0, 0, 255);
					}

					g_Drawing.OutlinedRect(((top.x - HalfWidth) - 6), (top.y - 1), 4, (Height + 2), patch);

					g_Drawing.FilledRect((top.x - HalfWidth) - 5, top.y, 2, Height, Color(161, 248, 251, 255));

					g_Drawing.FilledRect((top.x - HalfWidth) - 5, top.y, 2, ((100 - fArmor) * (Height / 100)) + 1, patch);
				}
			}
			/*
			if (g_CVars.Visuals.ESP.Line)
			{
				Vector vLineOrigin;
				int	iScreenWidth = 0;
				int	iScreenHeight = 0;

				g_pSurface->DrawSetColor(Color(255, 0, 255, 190));
				g_pSurface->DrawLine((int)vLineOrigin.x, (int)vLineOrigin.y, iScreenWidth / 2, iScreenHeight);
			}*/
		
			CSWeapon* Weapon = (CSWeapon*)Ent->GetActiveBaseCombatWeapon();
			if (Weapon)
			{
				if (g_CVars.Visuals.ESP.Health) box.y += 5;
				else box.y += Height + 5;

				const char* tmp = Weapon->GetWeaponAlias();

				char weaponName[0xFF];
				strcpy(weaponName, tmp);

				char* c = weaponName;
				while (*c)
				{
					*c = toupper(*c);
					++c;
				}

				if (g_CVars.Visuals.ESP.Weapon) g_Drawing.MenuStringNormal(true, false, box.x + HalfWidth, box.y, Color(g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b), /*%s*/XorStr<0x20, 3, 0x3EFE7DA2>("\x05\x52" + 0x3EFE7DA2).s, weaponName);
			}
		}
	}
}


void GroundESP( )
{
	BasePlayer* LocalPlayer = ( BasePlayer* ) g_pClientEntityList->GetClientEntity( g_pEngineClient->GetLocalPlayer( ) );
	if( !LocalPlayer ) return;

	for( int i = g_pClientEntityList->GetMaxEntities( ); i >= 0; --i )
	{
		if( i == g_pEngineClient->GetLocalPlayer( ) ) continue;

		BaseEntity* Ent = ( BaseEntity* ) g_pClientEntityList->GetClientEntity( i );
		if( !Ent ) continue;

		std::string name = Ent->GetClientClass( )->m_pNetworkName;
		if( name.empty( ) ) continue;

		const matrix3x4_t& trans = Ent->m_rgflCoordinateFrame( );

		Vector org = Ent->GetAbsOrigin( );
		Vector vMin = *( Vector* )( ( DWORD ) Ent + 0x168 );
		Vector vMax = *( Vector* )( ( DWORD ) Ent + 0x174 );

		Vector lbf, lbb, ltb, ltf, rtb, rbb, rbf, rtf, vBounds[ ] = {
			Vector( vMin.x, vMin.y, vMin.z ), // left bottom back corner
			Vector( vMin.x, vMax.y, vMin.z ), // left bottom front corner
			Vector( vMax.x, vMax.y, vMin.z ), // left top front corner
			Vector( vMax.x, vMin.y, vMin.z ), // left top back corner
			Vector( vMax.x, vMax.y, vMax.z ), // right top front corner
			Vector( vMin.x, vMax.y, vMax.z ), // right bottom front corner
			Vector( vMin.x, vMin.y, vMax.z ), // right bottom back corner
			Vector( vMax.x, vMin.y, vMax.z )  // right top back corner
		};

		Vector p[ 8 ];
		for( int i = 0; i < 8; i++ ) VectorTransform( vBounds[ i ], trans, p[ i ] );

		if ( !g_Stuff.WorldToScreen( p[ 0 ], lbb ) || !g_Stuff.WorldToScreen( p[ 1 ], lbf )
			|| !g_Stuff.WorldToScreen( p[ 2 ], ltf ) || !g_Stuff.WorldToScreen( p[ 3 ], ltb )
			|| !g_Stuff.WorldToScreen( p[ 4 ], rtf ) || !g_Stuff.WorldToScreen( p[ 5 ], rbf )
			|| !g_Stuff.WorldToScreen( p[ 6 ], rbb ) || !g_Stuff.WorldToScreen( p[ 7 ], rtb ) ) continue;

		Vector vBoundArray[] = { lbf, rtb, lbb, rtf, rbf, rbb, ltb, ltf };

		float left = ltf.x;
		float top = ltf.y;
		float right = ltf.x;
		float bottom = ltf.y;

		for( int i = 1; i < 8; i++ )
		{
			if( left > vBoundArray[ i ].x ) left = vBoundArray[ i ].x;
			if( top < vBoundArray[ i ].y ) top = vBoundArray[ i ].y;
			if( right < vBoundArray[ i ].x ) right = vBoundArray[ i ].x;
			if( bottom > vBoundArray[ i ].y ) bottom = vBoundArray[ i ].y;
		}

		BaseCombatWeapon* Weapon = ( BaseCombatWeapon* )Ent->GetBaseEntity( );
		IClientEntity* nigger = g_pClientEntityList->GetClientEntityFromHandle( Ent->m_hOwnerEntity( ) );

		Color colour = Color( g_CVars.ColorSelector.ESP.Wpn.r, g_CVars.ColorSelector.ESP.Wpn.g, g_CVars.ColorSelector.ESP.Wpn.b, 160 );

		if( !nigger )
		{
			if( name.find( /*CWeapon*/XorStr<0x96,8,0x0DB9BD3B>("\xD5\xC0\xFD\xF8\xEA\xF4\xF2"+0x0DB9BD3B).s ) != std::string::npos )
			{
				std::string huj = std::string( name.substr( 7 ) );
				std::transform( huj.begin( ), huj.end( ), huj.begin( ), ::toupper );

				g_Drawing.OutlinedBox( left, bottom, right - left, top - bottom, colour, Color( 0, 0, 0, 128 ) );
				g_Drawing.MenuStringNormal( true, false, left + ( ( right - left ) / 2 ), bottom - 13, Color( 255, 255, 255, 200 ), huj.c_str( ) );
			}
			else if( name.find( /*CAK47*/XorStr<0x09,6,0xC8536137>("\x4A\x4B\x40\x38\x3A"+0xC8536137).s ) != std::string::npos )
			{
				g_Drawing.OutlinedBox( left, bottom, right - left, top - bottom, colour, Color( 0, 0, 0, 128 ) );
				g_Drawing.MenuStringNormal( true, false, left + ( ( right - left ) / 2 ), bottom - 13, Color( 255, 255, 255, 200 ), /*AK47*/XorStr<0xAD,5,0x5BBD8D56>("\xEC\xE5\x9B\x87"+0x5BBD8D56).s );
			}
			else if( name.find( /*CDEagle*/XorStr<0xF0,8,0xCD0E0829>("\xB3\xB5\xB7\x92\x93\x99\x93"+0xCD0E0829).s ) != std::string::npos )
			{
				g_Drawing.OutlinedBox( left, bottom, right - left, top - bottom, colour, Color( 0, 0, 0, 128 ) );
				g_Drawing.MenuStringNormal( true, false, left + ( ( right - left ) / 2 ), bottom - 13, Color( 255, 255, 255, 200 ), /*DEAGLE*/XorStr<0x64,7,0x9B450D10>("\x20\x20\x27\x20\x24\x2C"+0x9B450D10).s );
			}
			else if( ( name.find( /*CC4*/XorStr<0x99,4,0x6F66FDB9>("\xDA\xD9\xAF"+0x6F66FDB9).s ) != std::string::npos ) || ( name.find( /*CPlantedC4*/XorStr<0x5D,11,0x97F93DB6>("\x1E\x0E\x33\x01\x0F\x16\x06\x00\x26\x52"+0x97F93DB6).s ) != std::string::npos ) )
			{
				g_Drawing.OutlinedBox( left, bottom, right - left, top - bottom, colour, Color( 0, 0, 0, 128 ) );
				g_Drawing.MenuStringNormal( true, false, left + ( ( right - left ) / 2 ), bottom - 13, Color( 255, 255, 255, 200 ), /*C4*/XorStr<0x7E,3,0x639BCC3C>("\x3D\x4B"+0x639BCC3C).s );
			}
			else if( name.find( /*CSmokeGrenade*/XorStr<0xC5,14,0x39C3D515>("\x86\x95\xAA\xA7\xA2\xAF\x8C\xBE\xA8\xA0\xAE\xB4\xB4"+0x39C3D515).s ) != std::string::npos )
			{
				g_Drawing.OutlinedBox( left, bottom, right - left, top - bottom, colour, Color( 0, 0, 0, 128 ) );
				g_Drawing.MenuStringNormal( true, false, left + ( ( right - left ) / 2 ), bottom - 13, Color( 255, 255, 255, 200 ), /*SMOKE*/XorStr<0xF6,6,0x37C461D1>("\xA5\xBA\xB7\xB2\xBF"+0x37C461D1).s );
			}
			else if( name.find( /*CFlashbang*/XorStr<0xF8,11,0x56B0E978>("\xBB\xBF\x96\x9A\x8F\x95\x9C\x9E\x6E\x66"+0x56B0E978).s ) != std::string::npos )
			{
				g_Drawing.OutlinedBox( left, bottom, right - left, top - bottom, colour, Color( 0, 0, 0, 128 ) );
				g_Drawing.MenuStringNormal( true, false, left + ( ( right - left ) / 2 ), bottom - 13, Color( 255, 255, 255, 200 ), /*FLASH*/XorStr<0xA7,6,0x8D4FE6F9>("\xE1\xE4\xE8\xF9\xE3"+0x8D4FE6F9).s );
			}
			else if( name.find( /*CHEGrenade*/XorStr<0xD3,11,0x75630ED6>("\x90\x9C\x90\x91\xA5\xBD\xB7\xBB\xBF\xB9"+0x75630ED6).s ) != std::string::npos )
			{
				g_Drawing.OutlinedBox( left, bottom, right - left, top - bottom, colour, Color( 0, 0, 0, 128 ) );
				g_Drawing.MenuStringNormal( true, false, left + ( ( right - left ) / 2 ), bottom - 13, Color( 255, 255, 255, 200 ), /*HEGREN*/XorStr<0xC5,7,0x589A89B2>("\x8D\x83\x80\x9A\x8C\x84"+0x589A89B2).s );
			}
		}
	}
}

void tab1337( int x, int y, int w, int h, Color color, Color color2 )
{
	int bright[ 4 ], fade[ 4 ], nigger, jew;

	bright[ 0 ] = x;
	bright[ 1 ] = y;
	bright[ 2 ] = w;
	bright[ 3 ] = h;

	fade[ 0 ] = x;
	fade[ 1 ] = y + 1;
	fade[ 2 ] = w;
	fade[ 3 ] = h - 2;

	g_Drawing.FilledRect( bright[ 0 ], bright[ 1 ], bright[ 2 ], 1, color );
	g_Drawing.FilledRect( bright[ 0 ], bright[ 1 ] + bright[ 3 ], bright[ 2 ], 1, color );

	g_Drawing.FilledRect( fade[ 0 ], fade[ 1 ], fade[ 2 ], 1, color2 );
	g_Drawing.FilledRect( fade[ 0 ], fade[ 1 ] + fade[ 3 ], fade[ 2 ], 1, color2 );

	if( h % 2 == 0 ) nigger = bright[ 3 ];
	else nigger = bright[ 3 ] + 1;

	if( h % 2 == 0 ) jew = fade[ 3 ];
	else jew = fade[ 3 ] + 1;

	for( int i = 0; i < ( nigger / 2 ); i++ )
	{
		g_Drawing.FilledRect( bright[ 0 ] - i, bright[ 1 ] + i, 1, 1, color );
		g_Drawing.FilledRect( bright[ 0 ] - i, bright[ 1 ] + bright[ 3 ] - i, 1, 1, color );
		g_Drawing.FilledRect( bright[ 0 ] + bright[ 2 ] + i, bright[ 1 ] + i, 1, 1, color );
		g_Drawing.FilledRect( bright[ 0 ] + bright[ 2 ] + i, bright[ 1 ] + bright[ 3 ] - i, 1, 1, color );
	}

	for( int i = 0; i < ( jew / 2 ); i++ )
	{
		g_Drawing.FilledRect( fade[ 0 ] - i, fade[ 1 ] + i, 1, 1, color2 );
		g_Drawing.FilledRect( fade[ 0 ] - i, fade[ 1 ] + fade[ 3 ] - i, 1, 1, color2 );
		g_Drawing.FilledRect( fade[ 0 ] + fade[ 2 ] + i, fade[ 1 ] + i, 1, 1, color2 );
		g_Drawing.FilledRect( fade[ 0 ] + fade[ 2 ] + i, fade[ 1 ] + fade[ 3 ] - i, 1, 1, color2 );
	}
}

const std::string ahTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%H:%M", &tstruct);

	return buf;
}
#include "uid1488.h"
void holopanel(BasePlayer* localplayer)
{
	float ping = g_pEngineClient->GetNetChannelInfo()->GetLatency(0) * 1000;
	float fps = 1 / g_pGlobals->frametime;
	int fakelag = ((TIME_TO_TICKS(LocalData[0].m_SimulationTime - LocalData[1].m_SimulationTime)) - 1);
	matrix3x4_t Matrix[128];

	if (!localplayer->SetupBones(Matrix, 128, 256, localplayer->m_flSimulationTime()))
		return;

	if (localplayer->m_lifeState() = 1)
		return;

	studiohdr_t* pStudioHeader = g_pModelInfo->GetStudiomodel(localplayer->GetModel());

	if (!pStudioHeader)
		return;

	mstudiobbox_t* pBox = pStudioHeader->pHitboxSet(0)->pHitbox(12);//можно выставить другой хитбокс

	if (!pBox)
		return;

	Vector vTraced = (pBox->bbmin + pBox->bbmax) * 0.5f;

	static Vector bone_pos;
	VectorTransform(vTraced, Matrix[pBox->bone], bone_pos);

	Vector angle;
	if (g_pInput->CAM_IsThirdPerson())
	{
		if (g_Stuff.WorldToScreen(bone_pos, angle))
		{
			g_Drawing.Line(angle.x, angle.y, angle.x + 100, angle.y - 150, Color(255, 255, 255));//линия от руки до квадрата

			g_Drawing.FilledRect(angle.x + 100, angle.y - 155, 150, 5, Color(111, 111, 111, 255));//вверхний квадрат

			g_Drawing.FilledRect(angle.x + 100, angle.y - 150, 150, 105, Color(0, 0, 0, 150));//задний фон

			//NOTE:каждая строчка это -20 у angle.y

			g_Drawing.MenuStringNormal(false, false, angle.x + 110, angle.y - 145, Color(255, 255, 255), "clawcore 2.0 [beta]");

			g_Drawing.MenuStringNormal(false, false, angle.x + 110, angle.y - 125, Color(255, 255, 255), "Fps : %2.f", fps);

			g_Drawing.MenuStringNormal(false, false, angle.x + 110, angle.y - 105, Color(255, 255, 255), "Ping : %2.f", ping);

			g_Drawing.MenuStringNormal(false, false, angle.x + 110, angle.y - 85, Color(255, 255, 255), "FL : %i", fakelag);

			if (fps < 60)
			{
				g_Drawing.MenuStringNormal(false, false, angle.x + 110, angle.y - 65, Color(219, 179, 15, 255), "WARNING! LOW FPS");
			}
		}
	}
}
typedef void( __thiscall* PaintTraverse_t )( void*, unsigned int, bool, bool );
void __fastcall Hooked_PaintTraverse( void* ptr, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	const char* pszPanelName = g_pPanel->GetName( vguiPanel );

	
	if (!strcmp("HudZoom", g_pPanel->GetName(vguiPanel)))
		return;
	static ConVar* zoom = g_pCvar->FindVar("zoom_sensitivity_ratio");
	zoom->GetFloat();
	zoom->SetValue(0);

	PaintTraverseVMT->Function< PaintTraverse_t >(40)(ptr, vguiPanel, forceRepaint, allowForce);

	g_GameEventManager.RegisterSelf( );

	bool bValid = false;
	if( pszPanelName && pszPanelName[ 0 ] == 'M' && pszPanelName[ 3 ] == 'S' && pszPanelName[ 9 ] == 'T' ) bValid = true;

	BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
	
	if( bValid )
	{
		DWORD dwDisconnectMsg = ( DWORD )GetModuleHandleA( /*engine.dll*/XorStr<0x8D,11,0xF704AB59>("\xE8\xE0\xE8\xF9\xFF\xF7\xBD\xF0\xF9\xFA"+0xF704AB59).s ) + 0x2E15C8;
        DWORD dwOld;
        //VirtualProtect( ( LPVOID ) dwDisconnectMsg, 4, PAGE_READWRITE, &dwOld );
        //char msg[ 19 ];
        //strcpy( msg, "you suck" );
        //WriteProcessMemory( GetCurrentProcess( ), ( LPVOID ) dwDisconnectMsg, &msg, sizeof( msg ), 0 );
        //VirtualProtect( ( LPVOID ) dwDisconnectMsg, 4, dwOld, &dwOld );

		g_Drawing.FontInit( );
		g_Whitelist.Init( );

		// leet ruski antiban ))))) xaxaxax
		static ConVar* cvar_xbox_autothrottle = g_pCvar->FindVar( /*xbox_autothrottle*/XorStr<0x1C,18,0xFE51A435>("\x64\x7F\x71\x67\x7F\x40\x57\x57\x4B\x51\x4E\x55\x47\x5D\x5E\x47\x49"+0xFE51A435).s );
		static ConVar* cvar_xbox_throttlebias = g_pCvar->FindVar( /*xbox_throttlebias*/XorStr<0xB6,18,0x1151B3C8>("\xCE\xD5\xD7\xC1\xE5\xCF\xD4\xCF\xD1\xCB\xB4\xAD\xA7\xA1\xAD\xA4\xB5"+0x1151B3C8).s );
		static ConVar* cvar_xbox_throttlespoof = g_pCvar->FindVar( /*xbox_throttlespoof*/XorStr<0x90,19,0x3676A912>("\xE8\xF3\xFD\xEB\xCB\xE1\xFE\xE5\xF7\xED\xEE\xF7\xF9\xEE\xEE\xF0\xCF\xC7"+0x3676A912).s );

		if( cvar_xbox_autothrottle->GetInt( ) != 1 ) cvar_xbox_autothrottle->SetValue( 1 );
		if( cvar_xbox_throttlebias->GetInt( ) != 100 ) cvar_xbox_throttlebias->SetValue( 100 );
		if( cvar_xbox_throttlespoof->GetInt( ) != 200 ) cvar_xbox_throttlespoof->SetValue( 200 );



		if( g_pEngineClient->IsInGame( ) )
		{
			if (g_CVars.Visuals.ESP.trails)
			{

				BasePlayer* player = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
				//if (!player)
				///	return;
				//if (player->() == 0) return;

				if (g_pEngineClient->IsInGame())
				{ 
					m_pEngineEffects->Ricochet( player->GetAbsOrigin( ), Vector( 0, 0, 0 ) ); 
					//m_pEngineEffects->EnergySplash(player->GetAbsOrigin(), Vector(0, 0, 0), true);		
				}
			}

			if (g_CVars.Visuals.ESP.Fullbright)
			{
				g_pEngineClient->ExecuteClientCmd("mat_fullbright 1");
			}
			else if (!g_CVars.Visuals.ESP.Fullbright)
			{
				g_pEngineClient->ExecuteClientCmd("mat_fullbright 0");
			}

			if( MovementRecorder.DrawPath[ 0 ] != Vector( 0, 0, 0 ) )
			{
				Vector StartPos;
				Vector Origin;

				if( g_Stuff.WorldToScreen( MovementRecorder.DrawPath[ 0 ], StartPos ) )
				{
					if( g_Stuff.WorldToScreen( MovementRecorder.DrawPath[ 0 ] + Vector( 0.f, 0.f, 72.f ), Origin ) )
					{
						int h = StartPos.y - Origin.y;
						g_Drawing.OutlinedBox( StartPos.x, StartPos.y - h, 10, h, Color( 255, 255, 255, 160 ), Color( 0, 0, 0, 128 ) );
						g_Drawing.MenuStringNormal( true, false, StartPos.x + 5, StartPos.y - h - 13, Color( 255, 255, 255, 200 ), /*Start Position*/XorStr<0xD3,15,0xCD77542F>("\x80\xA0\xB4\xA4\xA3\xF8\x89\xB5\xA8\xB5\xA9\xB7\xB0\x8E"+0xCD77542F).s );

						for( int i = 1; i <= MovementRecorder.TickEnd; i++ )
						{
							Vector Tmp;
							if( !g_Stuff.WorldToScreen( MovementRecorder.DrawPath[ i ], Tmp ) ) continue;
							g_Drawing.OutlinedRect( Tmp.x - 1, Tmp.y - 1, 3, 3, Color( 0, 0, 0, 128 ) );
							g_Drawing.FilledRect( Tmp.x, Tmp.y, 1, 1, Color( 255, 255, 255, 160 ) );
						}
					}
				}
			}

			HitESP( );
			if( g_CVars.Visuals.ESP.Ground ) GroundESP( );
			BoundingBoxESP( );
			Crosshair( );
		} 
	//	g_GUI.Draw( );
		n_menu gn_menu;
		//gn_menu.render( );
		if( g_pEngineClient->IsInGame( ) )
		{
			int pos = 3;
			BasePlayer* localplayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
			float ping = g_pEngineClient->GetNetChannelInfo()->GetLatency(0) * 1000;
			float fps = 1 / g_pGlobals->frametime;
			float tickrate = 1 / g_pGlobals->interval_per_tick;
			int fakelag = ((TIME_TO_TICKS(LocalData[0].m_SimulationTime - LocalData[1].m_SimulationTime)) - 1);
			
			//ss_watermark2 << "uid: " << std::to_string(my_uid);
			if ((GetAsyncKeyState(g_CVars.Aimbot.BodyAimValue)))
			{
				g_Drawing.MenuStringNormal(false, false, 5, 400, Color::White, "force body");
			}
			if (GetAsyncKeyState(g_CVars.Aimbot.mindmgpressedvalue))
			{
			g_Drawing.MenuStringNormal(false, false, 5, 445, Color::White, "override dmg");
			}
			
			bool bIsScoped = LocalPlayer->m_iFOV() >= LocalPlayer->m_iDefaultFOV();

			CSWeapon* Weapon = (CSWeapon*)LocalPlayer->GetActiveBaseCombatWeapon();

			BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
			CTickRecord LocalData[8];
			int w = screen_x,
				h = screen_y,
				x = w / 2,
				y = h / 2,
				size = 1;

		//	Vector vel = LocalPlayer->GetVelocity();
		//	vel.z = 0.0f;
		//	g_Drawing.MenuStringNormal5(false, false, 3, pos + 350, Color(128, 128, 255), "VEL:%i", (int)(ceilf(vel.Length())));

	//		bool lcbreak = ((LocalData[0].m_Origin - LocalData[1].m_Origin).LengthSqr() > 4096.f);
		//	g_Drawing.MenuStringNormal5(false, false, 3, pos + 300, lcbreak ? Color::Green : Color::Red, "LC");


			if (LocalPlayer && Weapon && Weapon->IsWeapon())
			{
				if (Weapon->GetWeaponID() == WEAPON_SCOUT || Weapon->GetWeaponID() == WEAPON_G3SG1 || Weapon->GetWeaponID() == WEAPON_SG550 || Weapon->GetWeaponID() == WEAPON_AWP)
				{
					if (bIsScoped)
					{
						g_Drawing.FilledRect(0, y, w, size, Color::Black);
						g_Drawing.FilledRect(x, 0, size, h, Color::Black);
					}
				}
			}

			if( g_CVars.Miscellaneous.AirStuck )
			{
				if( g_CVars.Miscellaneous.AirStuckPress ) g_Drawing.MenuStringNormal( true, false, screen_x / 2, pos, Color::White, /*IN AIRSTUCK!*/XorStr<0xA3,13,0xC37AE085>("\xEA\xEA\x85\xE7\xEE\xFA\xFA\xFE\xFE\xEF\xE6\x8F"+0xC37AE085).s );
			}

			if (g_CVars.MovementRecorder.Active)
			{


				std::string situation;
				if (MovementRecorder.Situation == NOTHING) situation = /*Stopped*/XorStr<0x66, 8, 0xEB60D5AB>("\x35\x13\x07\x19\x1A\x0E\x08" + 0xEB60D5AB).s;
				else if (MovementRecorder.Situation == RECORDING) situation = /*Recording*/XorStr<0x98, 10, 0x60B56B8C>("\xCA\xFC\xF9\xF4\xEE\xF9\xF7\xF1\xC7" + 0x60B56B8C).s;
				else if (MovementRecorder.Situation == PLAYING) situation = /*Playing*/XorStr<0x90, 8, 0x42040230>("\xC0\xFD\xF3\xEA\xFD\xFB\xF1" + 0x42040230).s;
				else if (MovementRecorder.Situation == STARTPOS) situation = /*Finding Start Position*/XorStr<0x8E, 23, 0x387C1F1C>("\xC8\xE6\xFE\xF5\xFB\xFD\xF3\xB5\xC5\xE3\xF9\xEB\xEE\xBB\xCC\xF2\xED\xF6\xD4\xC8\xCD\xCD" + 0x387C1F1C).s;

				pos += 50;
				g_Drawing.MenuStringNormal(false, false, 5, pos, Color::White, /*Movement Recorder: %s*/XorStr<0x21, 22, 0x754575AC>("\x6C\x4D\x55\x41\x48\x43\x49\x5C\x09\x78\x4E\x4F\x42\x5C\x4B\x55\x43\x08\x13\x11\x46" + 0x754575AC).s, situation.c_str());
				g_Drawing.MenuStringNormal(false, false, 5, pos + 13, Color::White, /*F6 - Record*/XorStr<0x17, 12, 0x7361CE9C>("\x51\x2E\x39\x37\x3B\x4E\x78\x7D\x70\x52\x45" + 0x7361CE9C).s);
				g_Drawing.MenuStringNormal(false, false, 5, pos + 13 * 2, Color::White, /*F7 - Save*/XorStr<0xF5, 10, 0x2143EC6D>("\xB3\xC1\xD7\xD5\xD9\xA9\x9A\x8A\x98" + 0x2143EC6D).s);
				g_Drawing.MenuStringNormal(false, false, 5, pos + 13 * 3, Color::White, /*F8 - Load*/XorStr<0x9A, 10, 0xD4AE142D>("\xDC\xA3\xBC\xB0\xBE\xD3\xCF\xC0\xC6" + 0xD4AE142D).s);
				g_Drawing.MenuStringNormal(false, false, 5, pos + 13 * 4, Color::White, /*F9 - Play*/XorStr<0xFC, 10, 0xEF2C55A5>("\xBA\xC4\xDE\xD2\x20\x51\x6E\x62\x7D" + 0xEF2C55A5).s);
				g_Drawing.MenuStringNormal(false, false, 5, pos + 13 * 5, Color::White, /*F10 - Stop Playing/Recording*/XorStr<0x02, 29, 0xEEAFE36F>("\x44\x32\x34\x25\x2B\x27\x5B\x7D\x65\x7B\x2C\x5D\x62\x6E\x69\x78\x7C\x74\x3B\x47\x73\x74\x77\x6B\x7E\x72\x72\x7A" + 0xEEAFE36F).s);
				g_Drawing.MenuStringNormal(false, false, 5, pos + 13 * 6, Color::White, /*F11 - Find StartPosition*/XorStr<0x73, 25, 0x5C2156BC>("\x35\x45\x44\x56\x5A\x58\x3F\x13\x15\x18\x5D\x2D\x0B\xE1\xF3\xF6\xD3\xEB\xF6\xEF\xF3\xE1\xE6\xE4" + 0x5C2156BC).s);
			}
		}
	}

}
