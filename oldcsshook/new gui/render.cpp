#include "render.hpp"
#include "../Main.h"
unsigned long render::main_font;
void render::setup_fonts() {
	main_font = g_pSurface->FontCreate();

	g_pSurface->SetFontGlyphSet(main_font, "Tahoma", 12, 500, 0, 0, font_flags::fontflag_antialias);
}

void render::line(int x, int y, int x2, int y2, color c) noexcept {
	g_pSurface->DrawSetColorRGBA(c.r, c.g, c.b, c.a);
	g_pSurface->DrawLine(x, y, x2, y2);
}

void render::rect(int x, int y, int x2, int y2, color c) noexcept {
	g_pSurface->DrawSetColorRGBA(c.r, c.g, c.b, c.a);
	g_pSurface->DrawOutlinedRect(x, y, x +x2, y + y2);
}

void render::filled_rect(int x, int y, int x2, int y2, color c) noexcept {
	g_pSurface->DrawSetColorRGBA(c.r, c.g, c.b, c.a);
	g_pSurface->DrawFilledRect(x, y, x + x2, y +y2);
}

void render::text(int x, int y, color color, int font, bool center, std::string str) noexcept {
	std::wstring text = std::wstring(str.begin(), str.end());
	const wchar_t* converted_text = text.c_str();

	int width, height;
	g_pSurface->GetTextSize(font, converted_text, width, height);

g_pSurface->DrawSetTextColorRGBA(color.r, color.g, color.b, color.a);
	g_pSurface->DrawSetTextFont(font);
	if (center)
	g_pSurface->DrawSetTextPos(x - (width / 2), y);
	else
		g_pSurface->DrawSetTextPos(x, y);

g_pSurface->DrawPrintText(converted_text, wcslen(converted_text), Valve::vgui::FontDrawType_t::FONT_DRAW_DEFAULT);
}

void render::get_text_size(unsigned long font, std::string str, int& width, int& height) noexcept {
	std::wstring text = std::wstring(str.begin(), str.end());
	const wchar_t* out = text.c_str();

	g_pSurface->GetTextSize(font, out, width, height);
}

float render::get_frametime() noexcept {
	return g_pGlobals->frametime;
}
