#include "Main.h"

Drawing g_Drawing;

void Drawing::FontInit()
{
	static bool Init = false;
	if (!Init)
	{
		Init = true;
		g_Font = g_pSurface->FontCreate();
		g_MenuFontNormal = g_pSurface->FontCreate();
		g_MenuFontBold = g_pSurface->FontCreate();
		g_OldFont = g_pSurface->FontCreate();
		g_WelcomeFont = g_pSurface->FontCreate();
		g_MenuFontTabs1 = g_pSurface->FontCreate();
		g_MenuFontTabs2 = g_pSurface->FontCreate();
		g_MenuFontTabs3 = g_pSurface->FontCreate();
		g_MenuFontTabs4 = g_pSurface->FontCreate();
		g_MenuFontTabs5 = g_pSurface->FontCreate();

		g_pSurface->SetFontGlyphSet(g_MenuFontTabs1, "Fifaks 1.0 dev1", 18, 400, 0, 0, Valve::vgui::EFontFlags::FONTFLAG_ANTIALIAS);
		g_pSurface->SetFontGlyphSet(g_MenuFontTabs2, "Fifaks 1.0 dev1", 17, 500, 0, 0, Valve::vgui::EFontFlags::FONTFLAG_ANTIALIAS);
		g_pSurface->SetFontGlyphSet(g_MenuFontTabs3, "Fifaks 1.0 dev1", 15, 100, 0, 0, Valve::vgui::EFontFlags::FONTFLAG_ANTIALIAS);
		g_pSurface->SetFontGlyphSet(g_MenuFontTabs4, "Fifaks 1.0 dev1", 13, 0, 0, 0, Valve::vgui::EFontFlags::FONTFLAG_ANTIALIAS);
		g_pSurface->SetFontGlyphSet(g_MenuFontTabs5, "Fifaks 1.0 dev1", 92, 0, 0, 0, Valve::vgui::EFontFlags::FONTFLAG_ANTIALIAS);
		g_pSurface->SetFontGlyphSet(g_Font, "Fifaks 1.0 dev1", 12, 400, 0, 0, Valve::vgui::EFontFlags::FONTFLAG_NONE);
		g_pSurface->SetFontGlyphSet(g_MenuFontNormal, "Arial", 12, 400, 0, 0, Valve::vgui::EFontFlags::FONTFLAG_OUTLINE);
		g_pSurface->SetFontGlyphSet(g_MenuFontBold, "Fifaks 1.0 dev1", 14, 900, 0, 0, Valve::vgui::EFontFlags::FONTFLAG_ANTIALIAS);
		g_pSurface->SetFontGlyphSet(g_OldFont, "Fifaks 1.0 dev1", 12, 400, 0, 0, Valve::vgui::EFontFlags::FONTFLAG_OUTLINE);

		g_pSurface->SetFontGlyphSet(g_WelcomeFont, "Fifaks 1.0 dev1", 92, 400, 0, 0, Valve::vgui::EFontFlags::FONTFLAG_ANTIALIAS);
	}
}

void Drawing::DrawPolygon(int count, Valve::vgui::Vertex_t* Vertexs, Color color)
{
	static int Texture = g_pSurface->CreateNewTextureID(true);
	unsigned char buffer[4] = { 255, 255, 255, 255 };

	g_pSurface->DrawSetTextureRGBA(Texture, buffer, 1, 1);
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawSetTexture(Texture);

	g_pSurface->DrawTexturedPolygon(count, Vertexs);
}

void Drawing::DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float& flRainbow)
{
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		FilledRect(x + i, y, 1, height, colRainbow);
	}
}

void Drawing::DrawCircle(int x, int y, int radius, Color color)
{
	int a = radius;
	int b = 0;
	int err = 0;

	while (a >= b)
	{
		FilledRect(x + a, y + b, 1, 1, color);
		FilledRect(x + a, y + b, 1, 1, color);
		FilledRect(x - a, y + b, 1, 1, color);
		FilledRect(x - a, y + b, 1, 1, color);
		FilledRect(x - a, y - b, 1, 1, color);
		FilledRect(x - a, y - b, 1, 1, color);
		FilledRect(x + a, y - b, 1, 1, color);
		FilledRect(x + a, y - b, 1, 1, color);


		if (err > 0)
		{
			a -= 1;
			err -= a * 2 + 1;
		}

		if (err <= 0)
		{
			b += 1;
			err += b * 2 + 1;
		}
	}
}

void Drawing::MenuStringNormal(bool centerx, bool centery, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_MenuFontNormal);

	int wide, tall;
	g_pSurface->GetTextSize(g_MenuFontNormal, wbuf, wide, tall);
	g_flMenuFontNormalSize = wide + 2;
	if (centerx) x -= wide / 2;
	if (centery) y -= tall / 2;

	//g_pSurface->DrawSetTextColor( Color( 0, 0, 0, color.a ) );
	//g_pSurface->DrawSetTextPos( x + 1, y + 1 );
	//g_pSurface->DrawPrintText( wbuf, wcslen( wbuf ), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT );

	g_pSurface->DrawSetTextColor(color);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void Drawing::MenuStringNormal1(bool centerx, bool centery, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_MenuFontTabs1);

	int wide, tall;
	g_pSurface->GetTextSize(g_MenuFontTabs1, wbuf, wide, tall);
	g_flMenuFontNormalSize = wide + 2;
	if (centerx) x -= wide / 2;
	if (centery) y -= tall / 2;

	g_pSurface->DrawSetTextColor(color);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void Drawing::MenuStringNormal2(bool centerx, bool centery, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_MenuFontTabs2);

	int wide, tall;
	g_pSurface->GetTextSize(g_MenuFontTabs2, wbuf, wide, tall);
	g_flMenuFontNormalSize = wide + 2;
	if (centerx) x -= wide / 2;
	if (centery) y -= tall / 2;

	g_pSurface->DrawSetTextColor(color);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void Drawing::MenuStringNormal5(bool centerx, bool centery, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_MenuFontTabs5);

	int wide, tall;
	g_pSurface->GetTextSize(g_MenuFontTabs5, wbuf, wide, tall);
	g_flMenuFontNormalSize = wide + 2;
	if (centerx) x -= wide / 2;
	if (centery) y -= tall / 2;

	g_pSurface->DrawSetTextColor(color);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void Drawing::MenuStringNormal3(bool centerx, bool centery, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_MenuFontTabs3);

	int wide, tall;
	g_pSurface->GetTextSize(g_MenuFontTabs3, wbuf, wide, tall);
	g_flMenuFontNormalSize = wide + 2;
	if (centerx) x -= wide / 2;
	if (centery) y -= tall / 2;

	g_pSurface->DrawSetTextColor(color);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void Drawing::MenuStringNormal4(bool centerx, bool centery, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_MenuFontTabs4);

	int wide, tall;
	g_pSurface->GetTextSize(g_MenuFontTabs4, wbuf, wide, tall);
	g_flMenuFontNormalSize = wide + 2;
	if (centerx) x -= wide / 2;
	if (centery) y -= tall / 2;

	g_pSurface->DrawSetTextColor(color);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void Drawing::MenuStringBold(bool centerx, bool centery, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_MenuFontBold);

	int wide, tall;
	g_pSurface->GetTextSize(g_MenuFontBold, wbuf, wide, tall);
	g_flMenuFontBoldSize = wide + 2;
	if (centerx) x -= wide / 2;
	if (centery) y -= tall / 2;

	g_pSurface->DrawSetTextColor(Color(0, 0, 0, color.a));
	g_pSurface->DrawSetTextPos(x + 1, y + 1);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);

	g_pSurface->DrawSetTextColor(color);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void Drawing::MenuString2(bool centerx, bool centery, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_Font);
	g_pSurface->DrawSetTextColor(color);

	int wide, tall;
	g_pSurface->GetTextSize(g_Font, wbuf, wide, tall);
	if (centerx) x -= wide / 2;
	if (centery) y -= tall / 2;

	g_pSurface->DrawSetTextPos(x, y + 1);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void Drawing::String(bool center, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_Font);

	if (center)
	{
		int wide, tall;
		g_pSurface->GetTextSize(g_Font, wbuf, wide, tall);
		x -= wide / 2;
	}

	// ==================================

	g_pSurface->DrawSetTextColor(color);
	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void Drawing::StringNormal(bool center, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_Font);
	g_pSurface->DrawSetTextColor(color);

	if (center)
	{
		int wide, tall;
		g_pSurface->GetTextSize(g_Font, wbuf, wide, tall);
		x -= wide / 2;
	}

	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void Drawing::BigString(bool center, int x, int y, Color color, const char* input, ...)
{
	va_list va_alist;
	char buf[1024];
	wchar_t wbuf[1024];

	va_start(va_alist, input);
	vsprintf(buf, input, va_alist);
	va_end(va_alist);

	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_pSurface->DrawSetTextFont(g_WelcomeFont);
	g_pSurface->DrawSetTextColor(color);

	if (center)
	{
		int wide, tall;
		g_pSurface->GetTextSize(g_WelcomeFont, wbuf, wide, tall);
		x -= wide / 2;
		y -= tall / 2;
	}

	g_pSurface->DrawSetTextPos(x, y);
	g_pSurface->DrawPrintText(wbuf, wcslen(wbuf), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

int Drawing::Pulse(int min, int max, float speed)
{
	return min + abs(sin(DEG2RAD(g_pGlobals->curtime * 180 * speed)) * ((float)max - (float)min));
}

void Drawing::Line(int x0, int y0, int x1, int y1, Color color)
{
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawLine(x0, y0, x1, y1);
}

void Drawing::FilledRect(int x, int y, int w, int h, Color color)
{
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawFilledRect(x, y, x + w, y + h);
}

void Drawing::OutlinedRect(int x, int y, int w, int h, Color color)
{
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Drawing::Circle(int x, int y, int radius, int segments, Color color)
{
	g_pSurface->DrawSetColor(color);
	g_pSurface->DrawOutlinedCircle(x, y, radius, segments);
}

void Drawing::OutlinedBox(int x, int y, int w, int h, Color color, Color outline)
{
	OutlinedRect(x - 1, y - 1, w + 2, h + 2, outline);
	OutlinedRect(x + 1, y + 1, w - 2, h - 2, outline);
	OutlinedRect(x, y, w, h, color);
}

void Drawing::GradientHorizontal(int x, int y, int w, int h, Color from, Color to, int alpha)
{
	float diff_r, diff_g, diff_b;
	float val_r, val_g, val_b;
	Color color;

	diff_r = from.r - to.r;
	diff_g = from.g - to.g;
	diff_b = from.b - to.b;

	if (from.r > to.r && diff_r < 0) diff_r = -diff_r;
	if (from.g > to.g && diff_g < 0) diff_g = -diff_g;
	if (from.b > to.b && diff_b < 0) diff_b = -diff_b;

	val_r = diff_r / h;
	val_g = diff_g / h;
	val_b = diff_b / h;

	for (int i = 0; i < h; i++)
	{
		color = Color(
			from.r - (val_r * i),
			from.g - (val_g * i),
			from.b - (val_b * i),
			alpha);
		//Line( x, y + i, x + w, y + i, color );
		FilledRect(x, y + i, w, h, color);
	}
}

void Drawing::GradientVertical(int x, int y, int w, int h, Color from, Color to, int alpha)
{
	int diff_r, diff_g, diff_b;
	int val_r, val_g, val_b;
	Color color;

	diff_r = from.r - to.r;
	diff_g = from.g - to.g;
	diff_b = from.b - to.b;

	if (from.r > to.r && diff_r < 0) diff_r = -diff_r;
	if (from.g > to.g && diff_g < 0) diff_g = -diff_g;
	if (from.b > to.b && diff_b < 0) diff_b = -diff_b;

	val_r = diff_r / w;
	val_g = diff_g / w;
	val_b = diff_b / w;

	for (int i = 0; i < w; i++)
	{
		color = Color(
			from.r - (val_r * i),
			from.g - (val_g * i),
			from.b - (val_b * i),
			alpha);
		//Line( x + i, y, x + i, y + h, color );
		FilledRect(x + i, y, 1, h, color);
	}
}

void Drawing::ColoredFilledRect(int x, int y, int w, int h, Color color)
{
	int r = color.r;
	int g = color.g;
	int b = color.b;

	if (r < h) r = h;
	if (g < h) g = h;
	if (b < h) b = h;

	Color fixed = Color(r, g, b);

	Color darker = Color(int(fixed.r * 0.65), int(fixed.g * 0.65), int(fixed.b * 0.65), 255);
	GradientHorizontal(x, y, w, h, darker, fixed, 255);
	OutlinedRect(x, y, w, h, fixed);
}

void Drawing::Box3D(Vector* points, Color color)
{
	Vector startPos, endPos, centerPos;

	g_pSurface->DrawSetColor(color);

	for (int i = 0; i < 3; i++)
	{
		if (g_Stuff.WorldToScreen(points[i], startPos) && g_Stuff.WorldToScreen(points[i + 1], endPos)) g_pSurface->DrawLine(startPos.x, startPos.y, endPos.x, endPos.y);
	}

	if (g_Stuff.WorldToScreen(points[0], startPos) && g_Stuff.WorldToScreen(points[3], endPos)) g_pSurface->DrawLine(startPos.x, startPos.y, endPos.x, endPos.y);

	for (int i = 4; i < 7; i++)
	{
		if (g_Stuff.WorldToScreen(points[i], startPos) && g_Stuff.WorldToScreen(points[i + 1], endPos)) g_pSurface->DrawLine(startPos.x, startPos.y, endPos.x, endPos.y);
	}

	if (g_Stuff.WorldToScreen(points[4], startPos) && g_Stuff.WorldToScreen(points[7], endPos)) g_pSurface->DrawLine(startPos.x, startPos.y, endPos.x, endPos.y);
	if (g_Stuff.WorldToScreen(points[0], startPos) && g_Stuff.WorldToScreen(points[6], endPos)) g_pSurface->DrawLine(startPos.x, startPos.y, endPos.x, endPos.y);
	if (g_Stuff.WorldToScreen(points[1], startPos) && g_Stuff.WorldToScreen(points[5], endPos)) g_pSurface->DrawLine(startPos.x, startPos.y, endPos.x, endPos.y);
	if (g_Stuff.WorldToScreen(points[2], startPos) && g_Stuff.WorldToScreen(points[4], endPos)) g_pSurface->DrawLine(startPos.x, startPos.y, endPos.x, endPos.y);
	if (g_Stuff.WorldToScreen(points[3], startPos) && g_Stuff.WorldToScreen(points[7], endPos)) g_pSurface->DrawLine(startPos.x, startPos.y, endPos.x, endPos.y);
	if (g_Stuff.WorldToScreen(points[8], centerPos)) g_Drawing.FilledRect(centerPos.x, centerPos.y, 1, 1, color);
}

void Drawing::DrawBones(BasePlayer* Ent, Color color)
{
	Vector points[19];
	Vector pointstransformed[19];
	Vector pointsw2s[19];

	matrix3x4_t matrix[128];
	if (!(Ent->SetupBones(matrix, 128, 0x100, Ent->m_flSimulationTime()))) return;
	void* pModel = Ent->GetModel();
	if (!pModel) return;
	studiohdr_t* studiohdr = g_pModelInfo->GetStudiomodel(pModel);
	mstudiohitboxset_t* studiohitboxset = studiohdr->pHitboxSet(Ent->m_nHitboxSet());
	if (!studiohitboxset) return;

	for (int i = 0; i <= 18; i++)
	{
		mstudiobbox_t* studiobbox = studiohitboxset->pHitbox(i);
		if (!studiobbox) return;
		points[i] = (studiobbox->bbmax + studiobbox->bbmin) * .5f;
		VectorTransform(points[i], matrix[studiobbox->bone], pointstransformed[i]);

		if (!g_Stuff.WorldToScreen(pointstransformed[i], pointsw2s[i])) return;
	}

	// left leg
	Line((int)pointsw2s[4].x, (int)pointsw2s[4].y, (int)pointsw2s[3].x, (int)pointsw2s[3].y, color);
	Line((int)pointsw2s[3].x, (int)pointsw2s[3].y, (int)pointsw2s[2].x, (int)pointsw2s[2].y, color);
	Line((int)pointsw2s[2].x, (int)pointsw2s[2].y, (int)pointsw2s[1].x, (int)pointsw2s[1].y, color);
	Line((int)pointsw2s[1].x, (int)pointsw2s[1].y, (int)pointsw2s[0].x, (int)pointsw2s[0].y, color);

	// right leg
	Line((int)pointsw2s[8].x, (int)pointsw2s[8].y, (int)pointsw2s[7].x, (int)pointsw2s[7].y, color);
	Line((int)pointsw2s[7].x, (int)pointsw2s[7].y, (int)pointsw2s[6].x, (int)pointsw2s[6].y, color);
	Line((int)pointsw2s[6].x, (int)pointsw2s[6].y, (int)pointsw2s[5].x, (int)pointsw2s[5].y, color);
	Line((int)pointsw2s[5].x, (int)pointsw2s[5].y, (int)pointsw2s[0].x, (int)pointsw2s[0].y, color);

	// penis to head
	Line((int)pointsw2s[0].x, (int)pointsw2s[0].y, (int)pointsw2s[9].x, (int)pointsw2s[9].y, color);
	Line((int)pointsw2s[9].x, (int)pointsw2s[9].y, (int)pointsw2s[10].x, (int)pointsw2s[10].y, color);
	Line((int)pointsw2s[10].x, (int)pointsw2s[10].y, (int)pointsw2s[11].x, (int)pointsw2s[11].y, color);
	Line((int)pointsw2s[11].x, (int)pointsw2s[11].y, (int)pointsw2s[12].x, (int)pointsw2s[12].y, color);

	// right hand
	Line((int)pointsw2s[11].x, (int)pointsw2s[11].y, (int)pointsw2s[16].x, (int)pointsw2s[16].y, color);
	Line((int)pointsw2s[16].x, (int)pointsw2s[16].y, (int)pointsw2s[17].x, (int)pointsw2s[17].y, color);
	Line((int)pointsw2s[17].x, (int)pointsw2s[17].y, (int)pointsw2s[18].x, (int)pointsw2s[18].y, color);

	// left hand
	Line((int)pointsw2s[11].x, (int)pointsw2s[11].y, (int)pointsw2s[13].x, (int)pointsw2s[13].y, color);
	Line((int)pointsw2s[13].x, (int)pointsw2s[13].y, (int)pointsw2s[14].x, (int)pointsw2s[14].y, color);
	Line((int)pointsw2s[14].x, (int)pointsw2s[14].y, (int)pointsw2s[15].x, (int)pointsw2s[15].y, color);

}

void Drawing::DrawHitbox(BasePlayer* Ent, int iHitbox, Color color)
{
	matrix3x4_t matrix[128];
	if (!(Ent->SetupBones(matrix, 128, 0x100, Ent->m_flSimulationTime()))) return;
	void* pModel = Ent->GetModel();
	if (!pModel) return;
	studiohdr_t* studiohdr = g_pModelInfo->GetStudiomodel(pModel);
	mstudiohitboxset_t* studiohitboxset = studiohdr->pHitboxSet(Ent->m_nHitboxSet());
	if (!studiohitboxset) return;
	mstudiobbox_t* studiobbox = studiohitboxset->pHitbox(iHitbox);
	if (!studiobbox) return;

	Vector points[9];

	float scalecenter = g_pGlobals->interval_per_tick;
	points[8] = (studiobbox->bbmax + studiobbox->bbmin) * 0.5f;
	points[0] = studiobbox->bbmin + points[0] * scalecenter;

	points[1].x = studiobbox->bbmin.x + points[0].x * scalecenter;
	points[1].y = studiobbox->bbmax.y - points[0].y * scalecenter;
	points[1].z = studiobbox->bbmin.z + points[0].z * scalecenter;

	points[2].x = studiobbox->bbmax.x - points[0].x * scalecenter;
	points[2].y = studiobbox->bbmax.y - points[0].y * scalecenter;
	points[2].z = studiobbox->bbmin.z + points[0].z * scalecenter;

	points[3].x = studiobbox->bbmax.x - points[0].x * scalecenter;
	points[3].y = studiobbox->bbmin.y + points[0].y * scalecenter;
	points[3].z = studiobbox->bbmin.z + points[0].z * scalecenter;

	points[4] = studiobbox->bbmax - points[0] * scalecenter;

	points[5].x = studiobbox->bbmin.x + points[0].x * scalecenter;
	points[5].y = studiobbox->bbmax.y - points[0].y * scalecenter;
	points[5].z = studiobbox->bbmax.z - points[0].z * scalecenter;

	points[6].x = studiobbox->bbmin.x + points[0].x * scalecenter;
	points[6].y = studiobbox->bbmin.y + points[0].y * scalecenter;
	points[6].z = studiobbox->bbmax.z - points[0].z * scalecenter;

	points[7].x = studiobbox->bbmax.x - points[0].x * scalecenter;
	points[7].y = studiobbox->bbmin.y + points[0].y * scalecenter;
	points[7].z = studiobbox->bbmax.z - points[0].z * scalecenter;

	Vector pointsTransformed[9];
	for (int i = 0; i <= 8; i++) VectorTransform(points[i], matrix[studiobbox->bone], pointsTransformed[i]);

	Box3D(pointsTransformed, color);
}

void Drawing::DrawAimSpot(BasePlayer* Ent, int iHitbox, Color color)
{
	matrix3x4_t matrix[128];
	if (!(Ent->SetupBones(matrix, 128, 0x100, Ent->m_flSimulationTime()))) return;
	void* pModel = Ent->GetModel();
	if (!pModel) return;
	studiohdr_t* studiohdr = g_pModelInfo->GetStudiomodel(pModel);
	mstudiohitboxset_t* studiohitboxset = studiohdr->pHitboxSet(Ent->m_nHitboxSet());
	if (!studiohitboxset) return;
	mstudiobbox_t* studiobbox = studiohitboxset->pHitbox(iHitbox);
	if (!studiobbox) return;

	float scalecenter = g_pGlobals->interval_per_tick;

	Vector points[] = { ((studiobbox->bbmin + studiobbox->bbmax) * .5f),
		Vector(studiobbox->bbmin.x + studiobbox->bbmax.x, studiobbox->bbmin.y + (studiobbox->bbmax.y), studiobbox->bbmin.z + (studiobbox->bbmax.z)),
	  Vector(studiobbox->bbmin.x + studiobbox->bbmax.x, studiobbox->bbmax.y - (studiobbox->bbmax.y), studiobbox->bbmin.z + (studiobbox->bbmax.z)),
	  Vector(studiobbox->bbmax.x - studiobbox->bbmax.x, studiobbox->bbmax.y - (studiobbox->bbmax.y), studiobbox->bbmin.z + (studiobbox->bbmax.z)),
	  Vector(studiobbox->bbmax.x - studiobbox->bbmax.x, studiobbox->bbmin.y + (studiobbox->bbmax.y), studiobbox->bbmin.z + (studiobbox->bbmax.z)),
	  Vector(studiobbox->bbmax.x - studiobbox->bbmax.x, studiobbox->bbmax.y - (studiobbox->bbmax.y), studiobbox->bbmax.z - (studiobbox->bbmax.z)),
	  Vector(studiobbox->bbmin.x + studiobbox->bbmax.x, studiobbox->bbmax.y - (studiobbox->bbmax.y), studiobbox->bbmax.z - (studiobbox->bbmax.z)),
	  Vector(studiobbox->bbmin.x + studiobbox->bbmax.x, studiobbox->bbmin.y + (studiobbox->bbmax.y), studiobbox->bbmax.z - (studiobbox->bbmax.z)),
	  Vector(studiobbox->bbmax.x - studiobbox->bbmax.x, studiobbox->bbmin.y + (studiobbox->bbmax.y), studiobbox->bbmax.z - (studiobbox->bbmax.z)) };

	Vector pointsTransformed[9], pts[9];
	VectorTransform(points[0], matrix[studiobbox->bone], pointsTransformed[0]);

	if (g_Stuff.WorldToScreen(pointsTransformed[0], pts[0]))
	{
		g_Drawing.FilledRect(pts[0].x - 2, pts[0].y - 2, 5, 5, color);
		g_Drawing.OutlinedRect(pts[0].x - 2, pts[0].y - 2, 5, 5, Color(0, 0, 0, 160));
	}
}