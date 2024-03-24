#include "Main.h"
#include "XorStr.h"
#include <filesystem>
#include <fstream>
GUI g_GUI;


std::string GetExtension(const std::string& target)
{
	auto found = target.find_last_of(".");

	std::string extension("");

	if (found != std::string::npos)
		extension = target.substr(found + 1, target.length());

	return extension;
}
BOOL DirectoryExists( LPCTSTR szPath )
{
	DWORD dwAttrib = GetFileAttributes( szPath );

	return ( dwAttrib != INVALID_FILE_ATTRIBUTES &&
					 ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
}
bool GUI::ShouldDisableInput()
{
	if (bMouse)
	{
		if (GetAsyncKeyState(VK_LBUTTON)) return true;
		else if (GetAsyncKeyState(VK_RBUTTON)) return true;
		else return false;
	}
	else return false;
}

void GUI::SliderInt(int column, bool active, int width, int min, int max, int& value, const char* name)
{
	itemheight[column] += 27;

	int y = itemheight[column];

	int x = columns[column] + (column == 0 ? (18) : ((tabs[3].w / 2) - 13));

	g_Drawing.MenuStringNormal3(false, true, x, y + 15, Color(255, 255, 255), "%s", name);
	g_Drawing.MenuStringNormal2(false, true, x + 220, y + 27, Color(255, 255, 255), "%i", value);

	width += 10;

	if (value > max) value = max;
	if (value < min) value = min;

	if (g_Stuff.Mouse.ExternalHold(x - 1, y + 24, width + 6, 11, itemscount + 1)) {
		value = (mouse_x - x) * (max - min) / width;
		if (value > max) value = max;
		if (value < min) value = min;
	}

	int valueX = x + ((value - min) * width / (max - min));
	int val = valueX - x;

	g_Drawing.FilledRect(x - 1, y + 23, width + 2, 9, Color(0, 0, 0));
	g_Drawing.FilledRect(x, y + 24, width, 7, Color(33, 33, 33));

	if (val != 0)
		g_Drawing.FilledRect(x - 1, y + 23, val + 2, 9, Color(0, 0, 0));

	g_Drawing.FilledRect(x, y + 24, val, 7, Color(237, 87, 87));

	itemscount++;
}

void GUI::Slider(int column, bool active, int width, float min, float max, float& value, const char* name)
{
	itemheight[column] += 27;

	int y = itemheight[column];

	int x = columns[column] + (column == 0 ? (18) : ((tabs[3].w / 2) - 13));

	g_Drawing.MenuStringNormal3(false, true, x, y + 15, Color(255, 255, 255), "%s", name);
	g_Drawing.MenuStringNormal2(false, true, x + 220, y + 27, Color(255, 255, 255), "%.1f", value);

	width += 10;

	if (value > max) value = max;
	if (value < min) value = min;

	if (g_Stuff.Mouse.ExternalHold(x - 1, y + 24, width + 6, 11, itemscount + 1)) {
		value = (mouse_x - x) * (max - min) / width;
		if (value > max) value = max;
		if (value < min) value = min;
	}

	int valueX = x + ((value - min) * width / (max - min));
	int val = valueX - x;

	g_Drawing.FilledRect(x - 1, y + 23, width + 2, 9, Color(0, 0, 0));
	g_Drawing.FilledRect(x, y + 24, width, 7, Color(33, 33, 33));

	if (val != 0)
		g_Drawing.FilledRect(x - 1, y + 23, val + 2, 9, Color(0, 0, 0));

	g_Drawing.FilledRect(x, y + 24, val, 7, Color(237, 87, 87));

	itemscount++;
}

void GUISection::SliderColor(int column, bool active, int x, int y, int width, int min, int max, int& value, int colorVal, Color& color)
{
	g_Drawing.MenuStringNormal3(false, true, x + 102, y + 4, Color(255, 255, 255), "%i", value);

	width += 10;

	if (value > max) value = max;
	if (value < min) value = min;

	if (g_Stuff.Mouse.ExternalHold(x - 1, y - 1, width + 6, 11, itemscount + 1)) {
		value = (mouse_x - x) * (max - min) / width;
		if (value > max) value = max;
		if (value < min) value = min;
	}

	color[colorVal] = value;

	int valueX = x + ((value - min) * width / (max - min));
	int val = valueX - x;

	g_Drawing.FilledRect(x - 1, y - 1, width + 2, 11, Color(0, 0, 0));
	g_Drawing.FilledRect(x, y, width, 9, Color(33, 33, 33));
	if (val != 0)
		g_Drawing.FilledRect(x - 1, y - 1, val + 2, 11, Color(0, 0, 0));
	g_Drawing.FilledRect(x, y, val, 9, Color(237, 87, 87));

	itemscount++;
}

void GUI::CheckBox(int column, bool& var, const char* name)
{
	itemheight[column] += 27;

	if (column == 0)
	{
		g_Drawing.FilledRect(columns[column] + 16, itemheight[column] + 22, 9, 9, Color(0, 0, 0));
		g_Drawing.FilledRect(columns[column] + 17, itemheight[column] + 23, 7, 7, Color(33, 33, 33));

		if (g_Stuff.Mouse.Click(columns[column] + 16, itemheight[column] + 22, 9, 9)) var = !var;
		if (var) g_Drawing.FilledRect(columns[column] + 17, itemheight[column] + 23, 7, 7, Color(237, 87, 87));

		g_Drawing.MenuStringNormal2(false, true, columns[column] + 33, itemheight[column] + 27, Color(255, 255, 255), name);
	}
	else if (column == 1)
	{
		g_Drawing.FilledRect(columns[column] + 12, itemheight[column] + 22, 9, 9, Color(0, 0, 0));
		g_Drawing.FilledRect(columns[column] + 13, itemheight[column] + 23, 7, 7, Color(33, 33, 33));

		if (g_Stuff.Mouse.Click(columns[column] + 12, itemheight[column] + 22, 9, 9)) var = !var;
		if (var) g_Drawing.FilledRect(columns[column] + 13, itemheight[column] + 23, 7, 7, Color(237, 87, 87));

		g_Drawing.MenuStringNormal2(false, true, columns[column] + 29, itemheight[column] + 27, Color(255, 255, 255), name);
	}

	itemscount++;
}

void GUI::ButtonBox(int column, bool& var, const char* name)
{
	itemheight[column] += 27;

	int column_x_positive = (column == 0) ? 4 : 0;
	int column_x_negative = (column == 1) ? 4 : 0;

	g_Drawing.FilledRect(columns[column] + 14 + column_x_negative, itemheight[column] + 22 - 7, 123 + 129, 22, Color(0, 0, 0));

	Color main = Color(33, 33, 33);

	if (g_Stuff.Mouse.Click(columns[column] + 14 + column_x_negative, itemheight[column] + 22 - 7, 123 + 129, 22)) {
		var = !var;
		main = Color(25, 25, 25);
	}
	else if (g_Stuff.Mouse.IsInBox(columns[column] + 14 + column_x_negative, itemheight[column] + 22 - 7, 123 + 129, 22)) {
		main = Color(237, 87, 87);
	}


	g_Drawing.FilledRect(columns[column] + 15 + column_x_negative, itemheight[column] + 23 - 7, 123 + 127, 20, main);

	//if (g_Stuff.Mouse.Click(columns[column] + 12 + column_x_negative, itemheight[column] + 22, 9, 9)) var = !var;
	//if (var) g_Drawing.FilledRect(columns[column] + 13 + column_x_negative, itemheight[column] + 23, 7, 7, Color(237, 87, 87));

	g_Drawing.MenuStringNormal2(true, true, columns[column] + 15 + column_x_negative + (123 + 129) / 2, itemheight[column] + 27 + 11 / 2 + 1 - 7, Color(255, 255, 255), name);

	itemscount++;
}

void GUI::Text(int column, const char* name, Color col)
{
	itemheight[column] += 27;

	if (column == 0)
	{
		g_Drawing.MenuStringNormal2(false, true, columns[column] + 33 - 17, itemheight[column] + 27, col, name);
	}
	else if (column == 1)
	{
		g_Drawing.MenuStringNormal2(false, true, columns[column] + 29 - 13, itemheight[column] + 27, col, name);
	}

	itemscount++;
}

void GUISection::CheckBoxColor(int column, bool& var, bool& opened, const char* name, Color& color, int* colordata, bool alpha = false)
{
	g_GUI.itemheight[column] += 27;

	int x, y;
	x = g_GUI.columns[column] + 16;
	y = g_GUI.itemheight[column];

	if (column == 1) //# 1 == RIGHT
		x = x - 4;

	int totalcount = alpha ? 4 : 3;

	g_Drawing.FilledRect(x, y + 22, 9, 9, Color(0, 0, 0));
	g_Drawing.FilledRect(x + 1, y + 23, 7, 7, Color(33, 33, 33));

	if (g_Stuff.Mouse.Click(x, y + 22, 9, 9)) var = !var;
	if (var) g_Drawing.FilledRect(x + 1, y + 23, 7, 7, Color(237, 87, 87));

	g_Drawing.MenuStringNormal2(false, true, x + 17, y + 27, Color(255, 255, 255), name);

	if (g_Stuff.Mouse.Click(x + 227, y + 21, 18, 10) && !AnyKeybindAreActive() && !AnyDropdownAreActive()) {
		opened = !opened;
	}

	g_Drawing.FilledRect(x + 227, y + 22, 18, 10, Color(0, 0, 0));
	g_Drawing.FilledRect(x + 228, y + 23, 16, 8, color);

	int autoheight = (totalcount + 1) * 10 + (totalcount * 5); //Максимальное кол-во по высоте + дистанция между ними

	CheckboxColorFlag_t checkboxColorDataNew;

	checkboxColorDataNew.x = x + 227 + 10;
	checkboxColorDataNew.y = y + 23 + 18;
	checkboxColorDataNew.h = autoheight;
	checkboxColorDataNew.isAlpha = alpha;
	checkboxColorDataNew.isActive = opened;
	checkboxColorDataNew.color = &color;
	checkboxColorDataNew.colordata = colordata;

	AddNewColor(checkboxColorDataNew);

	itemscount++;
}

void GUI::Section(bool first, int column, int items, const char* name)
{
	itemscount = 0;
	if (!first) itemheight[column] += 46;

	int x, y, w, h;

	if (!column) {
		x = columns[column] + 8;
		y = itemheight[column] + 8;
		w = 267;
		h = 27 * (items + 1) + 10;

		g_Drawing.FilledRect(x, y, w, h, Color(0, 0, 0));
		g_Drawing.FilledRect(x + 1, y + 1, w - 2, h - 2, Color(27, 27, 27));
		g_Drawing.FilledRect(x, y, w, 27, Color(0, 0, 0));
		g_Drawing.FilledRect(x + 1, y + 1, w - 2, 25, Color(26, 26, 26));

		g_Drawing.MenuStringNormal1(false, true, columns[column] + tabs[1].w - 40, itemheight[column] + 20, Color(255, 255, 255), name);
	}
	else if (column) {
		x = columns[column] + 4;
		y = itemheight[column] + 8;
		w = 274;
		h = 27 * (items + 1) + 10;

		g_Drawing.FilledRect(x, y, w, h, Color(0, 0, 0));
		g_Drawing.FilledRect(x + 1, y + 1, w - 2, h - 2, Color(27, 27, 27));
		g_Drawing.FilledRect(x, y, w, 27, Color(0, 0, 0));
		g_Drawing.FilledRect(x + 1, y + 1, w - 2, 25, Color(26, 26, 26));

		g_Drawing.MenuStringNormal1(false, true, columns[column] + tabs[1].w - 40, itemheight[column] + 20, Color(255, 255, 255), name);
	}
}

void GUISection::Dropdown(int column, int min, int max, int& value, const char* name, const char** list, bool& active)
{
	g_GUI.itemheight[column] += 27;

	int x = g_GUI.columns[column];
	int y = g_GUI.itemheight[column];

	g_Drawing.MenuStringNormal3(false, true, x + 15, y + 23, Color(255, 255, 255), name);

	if (g_Stuff.Mouse.Click(x + 140, y + 15 - 3, 129, 17 + 6) && !AnyKeybindAreActive()) {
		if (AnyDropdownAreActive()) { //# Какой то все таки лист активен и нам нужно проверить если это и есть этот же лист
			if (IsItThisFocusedWindow(itemscount + 1)) {
				//# Это и есть этот лист

				SetActiveDropdown(false);
				active = false;
			}
			/*else {
				//# Игрок пытается активировать другой лист когда открыт какой то еще другой лист

				SetActiveDropdown(false);
				active = false;

				DisableAllDropdowns(); //# Выключаем все листы так как мы не может определить какой был другой лист
			}*/
		}
		else {
			SetActiveDropdown(true);

			SetFocusedWindow(itemscount + 1);
			//# +1 потому что весь код идет пока что для старого числа а в конце кода мы его обновляем

			active = true;//!active;
		}
	}

	g_Drawing.FilledRect(x + 140, y + 15 - 3, 129, 17 + 6, Color(0, 0, 0));
	g_Drawing.FilledRect(x + 141, y + 16 - 3, 127, 15 + 6, Color(27, 27, 27));

	//g_Drawing.FilledRect(x + 140, y + 15, 129, 17, Color(0, 0, 0));
	//g_Drawing.FilledRect(x + 141, y + 16, 127, 15, Color(27, 27, 27));

	DropDownFlag_t dropdownDataNew;
	dropdownDataNew.x = x + 140;
	dropdownDataNew.y = y + 35;//y + 31;
	dropdownDataNew.maxListCount = max;
	dropdownDataNew.list = list;
	dropdownDataNew.isActive = &active;
	dropdownDataNew.currentIndex = &value;

	AddNewDropdown(dropdownDataNew);

	itemscount++;
}

#include <winnt.h>

void GUISection::Keybind(int column, int* key, bool& value, const char* name)
{
	g_GUI.itemheight[column] += 27;

	int x = g_GUI.columns[column];
	int y = g_GUI.itemheight[column];

	g_Drawing.MenuStringNormal3(false, true, x + 15, y + 23, Color(255, 255, 255), name);

	if (g_Stuff.Mouse.Click(x + 140 + 19, y + 15 - 3, 129, 17) && !AnyKeybindAreActive() && !AnyDropdownAreActive()) {
		SetActiveKeybind(true);
		value = true;//!value;
	}

	g_Drawing.FilledRect(x + 140 + 19 + 5, y + 15 - 3, 105, 17 + 6, Color(0, 0, 0));
	g_Drawing.FilledRect(x + 141 + 19 + 5, y + 16 - 3, 103, 15 + 6, Color(27, 27, 27));

	char* active_text = nullptr;

	if (value == true) {
		active_text = "...";
		for (int i = 0; i < ARRAYSIZE(key_array); ++i) {
			if ((char*)GetNameFromCode(i).c_str() == "")
				continue;

			if (GetAsyncKeyState(i) & 1) {
				*key = i;
				value = false;
				SetActiveKeybind(false);
				break;
			}
		}
	}
	else {
		active_text = (char*)GetNameFromCode(*key).c_str();
	}

	g_Drawing.MenuStringNormal3(true, false, x + 159 + 5 + 105 / 2, y + 31 - 14, Color(255, 255, 255), "%s", active_text);

	//g_Drawing.FilledRect(x + 140, y + 15, 129, 17, Color(0, 0, 0));
	//g_Drawing.FilledRect(x + 141, y + 16, 127, 15, Color(27, 27, 27));

	itemscount++;
}

void GUISection::DisableAllDropdowns()
{
	int totalDropdownsCount = vectorOfDropdowns.size();

	//# Какая то часть меню подала сигнал о выключении всех выпадающих листов, что значит
	//# Что открылся CheckboxColor или Keybind либо что то новое

	for (int i = 0; i <= totalDropdownsCount; ++i) {
		bool* isActive = vectorOfDropdowns[i].isActive;

		if (*isActive) {
			*isActive = false;
			SetActiveDropdown(false);
		}
	}

	vectorOfDropdowns.clear();
}

void GUISection::RenderAllDropdowns()
{
	int totalDropdownsCount = vectorOfDropdowns.size();

	for (int i = totalDropdownsCount - 1; i >= 0; --i) {
		int x = vectorOfDropdowns[i].x;
		int y = vectorOfDropdowns[i].y;
		int maxlist = vectorOfDropdowns[i].maxListCount;

		int* value = vectorOfDropdowns[i].currentIndex;

		bool* isActive = vectorOfDropdowns[i].isActive;

		const char** list = vectorOfDropdowns[i].list;

		//dropdownDataNew.x = x + 140;
		//dropdownDataNew.y = y + 31;

		//if (*isActive)
		//	g_Stuff.Mouse.addmenuflag(FL_MENUMOUSECLICKSDISABLE);
		//else
		//	g_Stuff.Mouse.removemenuflag(FL_MENUMOUSECLICKSDISABLE);

		if (*isActive == true) {
			for (auto i = 0; i < maxlist; ++i) {
				if (i == maxlist - 1) g_Drawing.FilledRect(x, y + 1 + i * 22, 129, 27, Color(0, 0, 0));
				else g_Drawing.FilledRect(x, y + i * 22, 129, 25, Color(0, 0, 0));
				g_Drawing.FilledRect(x + 1, y + 1 + i * 22 - 1, 127, 27, Color(25, 25, 25));

				if (!g_Stuff.Mouse.IsInBox(x, y + 1 + i * 22 - 1, 129, 27) && i != *value)
					g_Drawing.MenuStringNormal3(false, false, x + 9, y + 1 + i * 22 + 4, Color(255, 255, 255), list[i]);
				else if (g_Stuff.Mouse.IsInBox(x, y + 1 + i * 22 - 1, 129, 27) && i != *value)
					g_Drawing.MenuStringNormal3(false, false, x + 9, y + 1 + i * 22 + 4, Color(237, 94, 94), list[i]);

				if (i == *value) {
					g_Drawing.MenuStringNormal3(false, false, x + 9, y - 18, Color(255, 255, 255), list[i]);
					g_Drawing.MenuStringNormal3(false, false, x + 9, y + 1 + i * 22 + 4, Color(237, 87, 87), list[i]);
				}

				if (g_Stuff.Mouse.Click(x, y + 1 + i * 22 - 1, 129, 27)) {
					if (i != *value) {
						g_Drawing.MenuStringNormal3(false, false, x + 9, y - 18, Color(255, 255, 255), list[i]);
						*value = i;
						*isActive = false;
						SetActiveDropdown(false);
					}
				}
			}
		}
		else
			g_Drawing.MenuStringNormal3(false, false, x + 9, y - 18, Color(255, 255, 255), list[*value]);
	}
}

static const char* RGBAvalues[] = {
	"Red",
	"Green",
	"Blue",
	"Alpha"
};

void GUISection::RenderAllColors()
{
	int totalColorsCount = vectorOfColors.size();

	for (int i = totalColorsCount - 1; i >= 0; --i) {
		int x = vectorOfColors[i].x;
		int y = vectorOfColors[i].y;
		int h = vectorOfColors[i].h;

		int* colordata = vectorOfColors[i].colordata;

		bool isAlpha = vectorOfColors[i].isAlpha;  // vectorOfColors[i].isAlpha;
		bool isActive = vectorOfColors[i].isActive;

		Color* color = vectorOfColors[i].color;

		if (isActive) {
			int totalcount = isAlpha ? 4 : 3;

			int autoheight = (totalcount + 1) * 10 + ((totalcount) * 5);

			g_Drawing.FilledRect(x, y - 9, 167 + 20, autoheight + 2, Color(0, 0, 0));
			g_Drawing.FilledRect(x + 1, y - 8, 165 + 20, autoheight, Color(28, 28, 28));

			for (int i = 0; i <= totalcount - 1; ++i) {
				g_Drawing.MenuStringNormal3(false, false, x + 10, y + (15 * i), Color(255, 255, 255), RGBAvalues[i]);

				SliderColor(0, true, x + 10 + 38 + 10, y + 2 + (15 * i), 86, 0, 255, colordata[i], i, *color);
			}
		}
	}
}

#define LEFT 0
#define RIGHT 1

struct list_s
{
	int index;
}list_s[64];

void GUI::DrawingTabUp(int x, int y, int w, int h, int index, const char* text, Color first, Color second)
{
	if (g_Stuff.Mouse.Click(x, y, w, h))
		activetabup = index;

	if (activetabup == index)
		g_Drawing.MenuStringNormal1(false, false, x + w * .1f, y, first, text);
	else if (activetabup != index)
		g_Drawing.MenuStringNormal1(false, false, x + w * .1f, y, second, text);
}

void GUI::DrawingTabDown(int x, int y, int w, int h, int& currentTab, int index, const char* text, Color first, Color second)
{
	if (g_Stuff.Mouse.Click(x, y, w, h))
		currentTab = index;

	if (currentTab == index)
		g_Drawing.MenuStringNormal2(false, false, x + w * .1f, y, first, text);
	else if (currentTab != index)
		g_Drawing.MenuStringNormal2(false, false, x + w * .1f, y, second, text);
}

void GUI::tabsInit()
{
	int tabheight = 18;

	tabs[1].x = menu.x + 20;
	tabs[1].y = menu.y + 15;
	tabs[1].w = (menu.w * .1f);
	tabs[1].h = tabheight;
	tabs[1].half = tabs[1].h / 2;
	tabs[1].secondhalf = tabs[1].h - tabs[1].half;

	tabs[2].x = menu.x + 70;
	tabs[2].y = menu.y + 15;
	tabs[2].w = (menu.w * .1f);
	tabs[2].h = tabheight;
	tabs[2].half = tabs[2].h / 2;
	tabs[2].secondhalf = tabs[2].h - tabs[2].half;

	tabs[3].x = menu.x + 137;
	tabs[3].y = menu.y + 15;
	tabs[3].w = (menu.w * .1f);
	tabs[3].h = tabheight;
	tabs[3].half = tabs[3].h / 2;
	tabs[3].secondhalf = tabs[3].h - tabs[3].half;

	tabs[4].x = menu.x + 185;
	tabs[4].y = menu.y + 15;
	tabs[4].w = (menu.w * .1f);
	tabs[4].h = tabheight;
	tabs[4].half = tabs[4].h / 2;
	tabs[4].secondhalf = tabs[4].h - tabs[4].half;

//	tabs[5].x = menu.x + 240;
	//tabs[5].y = menu.y + 15;
	//tabs[5].w = (menu.w * .1f);
	//tabs[5].h = tabheight;
	//tabs[5].half = tabs[5].h / 2;
	//tabs[5].secondhalf = tabs[5].h - tabs[5].half;
}

void GUI::menuRender(int x, int y, int w, int h)
{
	static bool Init = false;
	if (Init == false)
	{
		Init = true;
		g_CVars.Menu.w = 562;
		g_CVars.Menu.h = 421;

		activetabup = 1;

		activetabRage = 1;
		activetabVisual = 1;
		activetabMisc = 1;
		activetabOther = 1;
		activetabConfig = 1;
	}

	g_Stuff.Mouse.DragMenu(x, y, w, 50);

	menu.x = x + 5;
	menu.y = y + 8 + (((h - 8) - (h - 8)));
	menu.w = w;
	menu.h = h;

	g_Drawing.FilledRect(menu.x - 1, menu.y - 1, menu.w + 2, menu.h + 2, Color(0, 0, 0));
	g_Drawing.FilledRect(menu.x, menu.y, menu.w, menu.h, Color(31, 31, 31));

	g_Drawing.FilledRect(menu.x + 4, menu.y + 4, menu.w - 8, menu.h - 8, Color(0, 0, 0));
	g_Drawing.FilledRect(menu.x + 5, menu.y + 5, menu.w - 10, menu.h - 10, Color(24, 24, 24));

	g_Drawing.FilledRect(menu.x + 4, menu.y + 7, menu.w - 8, 37, Color(0, 0, 0));
	g_Drawing.FilledRect(menu.x + 5, menu.y + 8, menu.w - 10, 35, Color(28, 28, 28));

	g_Drawing.FilledRect(menu.x + 4, menu.y + 4, menu.w - 8, 4, Color(0, 0, 0));
	g_Drawing.GradientVertical(menu.x + 5, menu.y + 5, menu.w - 10, 2, Color(237, 87, 87), Color(234, 237, 87), 255);

	g_Drawing.FilledRect(menu.x + 9, menu.y + 47, menu.w - 18, 39, Color(0, 0, 0));
	g_Drawing.FilledRect(menu.x + 10, menu.y + 48, menu.w - 20, 37, Color(28, 28, 28));

	columns[0] = menu.x;
	columns[1] = menu.x + 275;
}


void TabGUISection::AddTabUp(int x, int y, int w, int h, int index, const char* text, Color first, Color second)
{
	if (g_Stuff.Mouse.Click(x + max_tabup_size, y, w, h))
		activetabup = index;

	if (activetabup == index)
		g_Drawing.MenuStringNormal2(false, false, x + w * .1f + max_tabup_size, y, first, text);
	else if (activetabup != index)
		g_Drawing.MenuStringNormal2(false, false, x + w * .1f + max_tabup_size, y, second, text);


	//max_size += g_pSurface->GetTextSize();

	{
		va_list va_alist;
		char buf[1024];
		wchar_t wbuf[1024];

		va_start(va_alist, text);
		vsprintf(buf, text, va_alist);
		va_end(va_alist);

		MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

		int wide, tall;
		g_pSurface->GetTextSize(g_MenuFontTabs2, wbuf, wide, tall);


		max_tabup_size += wide + 15;
	}
}

void TabGUISection::AddTabDown(int x, int y, int w, int h, int& currentTab, int index, const char* text, Color first, Color second)
{
	int wide, tall;

	{
		va_list va_alist;
		char buf[1024];
		wchar_t wbuf[1024];

		va_start(va_alist, text);
		vsprintf(buf, text, va_alist);
		va_end(va_alist);

		MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

		g_pSurface->GetTextSize(g_MenuFontTabs2, wbuf, wide, tall);
	}


	if (g_Stuff.Mouse.Click(x + max_size, y, w + wide / 2, h))
		currentTab = index;

	if (currentTab == index)
		g_Drawing.MenuStringNormal2(false, false, x + w * .1f + max_size, y, first, text);
	else if (currentTab != index)
		g_Drawing.MenuStringNormal2(false, false, x + w * .1f + max_size, y, second, text);


	max_size += wide + 15;
}

static TabGUISection tab_guisection;

static TabGUISection tab_down_ragebot;
static TabGUISection tab_down_visual;


void GUI::tabsRender()
{
	tab_guisection.clear();
	tab_down_ragebot.clear();
	tab_down_visual.clear();

	tab_guisection.AddTabUp(tabs[1].x, menu.y + 15, tabs[1].w, tabs[1].h, 1, xorstr("Rage"), Color(237, 87, 87), Color(255, 255, 255));
	tab_guisection.AddTabUp(tabs[1].x, menu.y + 15, tabs[1].w, tabs[1].h, 2, xorstr("Visual"), Color(237, 87, 87), Color(255, 255, 255));
	tab_guisection.AddTabUp(tabs[1].x, menu.y + 15, tabs[1].w, tabs[1].h, 3, xorstr("Misc"), Color(237, 87, 87), Color(255, 255, 255));
	tab_guisection.AddTabUp(tabs[1].x, menu.y + 15, tabs[1].w, tabs[1].h, 4, xorstr("Config"), Color(237, 87, 87), Color(255, 255, 255));

	g_Drawing.MenuStringNormal1(false, false, menu.x + 460, menu.y + 15, Color(255, 255, 255), xorstr("Noctua"));
	g_Drawing.MenuStringNormal1(false, false, menu.x + 515, menu.y + 15, Color(237, 87, 87), xorstr("Hook"));


	if (tab_guisection.activetabup == 1) { //# Rage Tab
		tab_down_ragebot.AddTabDown(tabs[1].x, menu.y + 58, tabs[1].w, tabs[1].h, activetabRage, 1, xorstr("Aimbot"), Color(237, 87, 87), Color(255, 255, 255));
		tab_down_ragebot.AddTabDown(tabs[1].x, menu.y + 58, tabs[1].w, tabs[1].h, activetabRage, 2, xorstr("Accuracy"), Color(237, 87, 87), Color(255, 255, 255));
		tab_down_ragebot.AddTabDown(tabs[1].x, menu.y + 58, tabs[1].w, tabs[1].h, activetabRage, 3, xorstr("HvH"), Color(237, 87, 87), Color(255, 255, 255));

	}
	else if (tab_guisection.activetabup == 2) { //# Visual Tab
		tab_down_visual.AddTabDown(tabs[1].x, menu.y + 58, tabs[1].w, tabs[1].h, activetabVisual, 1, xorstr("ESP"), Color(237, 87, 87), Color(255, 255, 255));
		tab_down_visual.AddTabDown(tabs[1].x, menu.y + 58, tabs[1].w, tabs[1].h, activetabVisual, 2, xorstr("World"), Color(237, 87, 87), Color(255, 255, 255));
		tab_down_visual.AddTabDown(tabs[1].x, menu.y + 58, tabs[1].w, tabs[1].h, activetabVisual, 3, xorstr("Fog"), Color(237, 87, 87), Color(255, 255, 255));
	}
	else if (tab_guisection.activetabup == 3) { //# Misc Tab
		tab_down_visual.AddTabDown(tabs[1].x, menu.y + 58, tabs[1].w, tabs[1].h, activetabMisc, 1, xorstr("Other"), Color(237, 87, 87), Color(255, 255, 255));
		tab_down_visual.AddTabDown(tabs[1].x, menu.y + 58, tabs[1].w, tabs[1].h, activetabMisc, 2, xorstr("Exploits"), Color(237, 87, 87), Color(255, 255, 255));
	}
	else if (tab_guisection.activetabup == 4) { //# Config Tab
		tab_down_visual.AddTabDown(tabs[1].x, menu.y + 58, tabs[1].w, tabs[1].h, activetabConfig, 1, xorstr("Config"), Color(237, 87, 87), Color(255, 255, 255));
	}
}

static bool AntiAimPitchActive = false;
const char* AntiAimPitch[] =
{
	"Off",
	"Normal",
	"Inverse Normal",
	"Down",
	"Up",
	"Random",
	"Untrusted",
	"LBY",
	"Custom"
};

static bool LagCompActive = false;
const char* LagComp[] =
{
	"Off",
	"On",
	"Extrapolation"
};

static bool HitscanActive = false;
const char* Hitscan[] =
{
	"Off",
	"Low",
	"Medium",
	"High"
};
static bool MultiSpotAcitve = false;
const char* MultiSpot[] =
{
	"Off",
	"Main Spot",
	"All Hitboxes"
};
static bool HitBoxModifiedActive = false;
const char* HitBoxModified[] =
{
	"Off",
	"Limbs",
	"Body",
	"All Boxes"
};
static int aimbotkeyselect = false;
const char* AimbotKey[] =
{
	"Auto",
	"Mouse 1",
	"Mouse 2",
	"Mouse 3",
	"Mouse 4",
	"Mouse 5"
};

static bool AntiAimYawActive = false;
const char* AntiAimYaw[] =
{
	"Forwards",
	"Backwards",
	"Sideways",
	"Custom",
	"Jitter"
};

static bool LegitYawActive = false;
const char* LegitYaw[] =
{
	"Off",
	"Right",
	"Left"
};

static bool yawvariationactive = false;
const char** yawvariation;
const char* yawvariation_1[] =
{
	"Normal",
	"Fake Side 1",
	"Fake Side 2",
	"Random"
};
const char* yawvariation_2[] =
{
	"Additional",
	"Adaptive",
	"Static",
	"Update"
};
const char* yawvariation_3[] =
{
	"Reality",
	"Nocitter static",
	"3-Way"
};

static bool AtTargetsActive = false;
const char* AtTargets[] =
{
	"Off",
	"Distance",
	"Average"
};

static bool EdgeActive = false;
const char* Edge[] =
{
	"Off",
	"Normal",
	"Fake Peek",
	"Jitter",
};

static bool FakeLagActive = false;
const char* FakeLag[] =
{
	"Constant",
	"Aimware",
	"Random",
	"Adaptive",
	"Standart",
	"Alternative"
};

static bool NoSpreadActive = false;
const char* NoSpread[] =
{
	"Off",
	"Classic",
	"Iterative",
	"Rotation",
};

static bool ResolverActive = false;
const char* Resolver[] =
{
	"Bullet Spin",
	"Backward",
	"HFH",
	"Animstate",
	"Bruteforce",
	"crash state"
};

static bool CrosshairActive = false;
const char* Crosshair[] =
{
	"Off",
	"Cross",
	"Dot",
	"Round",
	"Swastika",
};

static bool EntityActive = false;
const char* Entity[] =
{
	"CT",
	"T",
	"Weapons"
};
static bool ElementActive = false;
const char* Element[] =
{
	"ESP",
	"Chams"
};
static bool EspBoxActive = false;
const char* EspBox[] =
{
	"Off",
	"InterWebz",
	"Insomnia"
};
static bool ColorSelectorActive = false;
const char* ColorSelector[] =
{
	"Visible",
	"Through Walls",
	"Outline"
};
static bool HitsoundActive = false;
const char* Hitsound[] =
{
	"Off",
	"Opa",
	"Primordial",
	"COD",
	"Neverlose",
	"Bloom"
};

static bool KillSoundActive = false;
const char* KillSound[] =
{
	"Off",
	"Opa",
	"Minecraft",
	"Quake",
	"Dubov"
};
static bool ChamsOpened = false;
const char* ChamsName[] =
{
	"Off",
	"LocalPlayer",
	"Enemy",
	"Weapons",
	"Hands"
};

static bool MaterialOpen = false;
const char* MaterialType[] =
{
	"mat_vertex_in",
	"mat_vertex_out",
	"mat_unlit_in",
	"mat_unlit_out"
};

static bool LocalChamsOpen = false;
static int LocalChamsData[4] = { g_CVars.Visuals.Chams.LocalChamsColor.r, g_CVars.Visuals.Chams.LocalChamsColor.g, g_CVars.Visuals.Chams.LocalChamsColor.b, g_CVars.Visuals.Chams.LocalChamsColor.a };
static bool EnemyChamsOpen = false;
static int EnemyChamsData[4] = { g_CVars.Visuals.Chams.LocalChamsColor.r, g_CVars.Visuals.Chams.LocalChamsColor.g, g_CVars.Visuals.Chams.LocalChamsColor.b, g_CVars.Visuals.Chams.LocalChamsColor.a };
static bool WeaponChamsOpen = false;
static int WeaponChamsData[4] = { g_CVars.Visuals.Chams.LocalChamsColor.r, g_CVars.Visuals.Chams.LocalChamsColor.g, g_CVars.Visuals.Chams.LocalChamsColor.b, g_CVars.Visuals.Chams.LocalChamsColor.a };
static bool HandChamsOpen = false;
static int HandChamsData[4] = { g_CVars.Visuals.Chams.LocalChamsColor.r, g_CVars.Visuals.Chams.LocalChamsColor.g, g_CVars.Visuals.Chams.LocalChamsColor.b, g_CVars.Visuals.Chams.LocalChamsColor.a };
static bool ESPOpen = false;
static int ESPData[4] = { g_CVars.Visuals.Chams.ESPColor.r, g_CVars.Visuals.Chams.ESPColor.g, g_CVars.Visuals.Chams.ESPColor.b, g_CVars.Visuals.Chams.ESPColor.a };
#include "mainshit.h"
void GUI::Menu(int x, int y, int w, int h)
{
	menuRender(x, y, w, h);
	tabsInit();
	tabsRender();

	static GUISection Rage_Aimbot;
	static GUISection Rage_AntiAim;
	static GUISection Rage_FakeLag;
	static GUISection Visuals;
	static GUISection Misc;
	static GUISection Config;
	if (tab_guisection.activetabup == 1) {

		itemheight[0] = menu.y + tabs[1].h + 65;
		itemheight[1] = menu.y + tabs[1].h + 65;
	

		if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 54;
		else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 54;
	}
	if (tab_guisection.activetabup == 1) {
		if (activetabRage == 1) {
			static int key = 40;
			static bool value = false;
			static bool aimbothitboxes = false;
			static bool aimbotkey = false;

			static bool aimbottargetselection = false;
			static bool aimbotheightmode = false;

			itemheight[0] = menu.y + tabs[1].h + 65;
			itemheight[1] = menu.y + tabs[1].h + 65;

			Section(true, RIGHT, 4, "Additional");

			Rage_Aimbot.Dropdown(RIGHT, 0, 6, g_CVars.Aimbot.Key, "Aimbot Key", AimbotKey, aimbotkey);

			static int hitboxselect = 0;
			const char* AimbotHitboxes[] = { "Head", "Neck", "Chest", "Stomach" };
			Rage_Aimbot.Dropdown(RIGHT, 0, 4, hitboxselect, "Hitbox", AimbotHitboxes, aimbothitboxes);

			if (hitboxselect == 0) g_CVars.Aimbot.Hitbox = HITBOX_HEAD;
			else if (hitboxselect == 1) g_CVars.Aimbot.Hitbox = HITBOX_NECK;
			else if (hitboxselect == 2) g_CVars.Aimbot.Hitbox = HITBOX_CHEST;
			else if (hitboxselect == 3) g_CVars.Aimbot.Hitbox = HITBOX_STOMACH;

		//	Slider(RIGHT, true, 200, 0, 1, g_CVars.Aimbot.PointScale, "PointScale");

			const char* AimbotHeightmode[] = { "Auto", "Origin", "Center", "Center Fixed", "Highest" };
			Rage_Aimbot.Dropdown(RIGHT, 0, 5, g_CVars.Aimbot.HitboxMode, "Height Mode", AimbotHeightmode, aimbotheightmode);

			const char* AimbotTargetselection[] = { "Distance", "Health", "Next Shot", "Random" };
			Rage_Aimbot.Dropdown(RIGHT, 0, 4, g_CVars.Aimbot.TargetSelection, "Target Aimbot", AimbotTargetselection, aimbottargetselection);
			Section(false, RIGHT, 2, "Occlusion");
			SliderInt(RIGHT, true, 200, 0, 100, g_CVars.Aimbot.MinDamage, "AimBot Min Damage");
			Slider(RIGHT, true, 200, 0, 1, g_CVars.Aimbot.PointScale, "Hitbox Scale");


			Section(true, LEFT, 8, "Aimbot");
			CheckBox(LEFT, g_CVars.Aimbot.Active, protect("Active"));
			CheckBox(LEFT, g_CVars.Aimbot.AutoShoot, protect("Auto Fire"));
			//CheckBox(LEFT, g_CVars.Aimbot.Resolver.defaultAct, "Fake Correction");
			CheckBox(LEFT, g_CVars.Miscellaneous.OriginCorrection, "BackTrack Correction");
			CheckBox(LEFT, g_CVars.Aimbot.Silent, "Silent");
			CheckBox(LEFT, g_CVars.Aimbot.PerfectSilent, "Perfect Silent");
			CheckBox(LEFT, g_CVars.Aimbot.BodyAWP, "Body AWP");
			CheckBox(LEFT, g_CVars.Aimbot.FriendlyFire, "Friendly Fire");

			Rage_Aimbot.RenderAllDropdowns();
			Rage_Aimbot.RenderAllColors();

			Rage_Aimbot.ClearVecDropdownsData();
			Rage_Aimbot.ClearVecColorsData();

			if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 54;
			else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 54;
		}
		else if (activetabRage == 2)
		{ 

			itemheight[0] = menu.y + tabs[1].h + 65;
			itemheight[1] = menu.y + tabs[1].h + 65;
			Section(true, LEFT, 3, "Accuracy");

			CheckBox(LEFT, g_CVars.Accuracy.PerfectAccuracy, "Accuracy Boost");
			CheckBox(LEFT, g_CVars.Accuracy.NoSpread, "No Spread");
			CheckBox(LEFT, g_CVars.Accuracy.ForceSeed, "Force Seed");


			static int key = 40;
			static bool value = false;

			Section(false, LEFT, 6, "Extra");
			CheckBox(LEFT, g_CVars.Aimbot.BodyAim, "BodyAim");
			CheckBox(LEFT, g_CVars.Aimbot.baiminair, "BodyAim In Air");
			CheckBox(LEFT, g_CVars.Aimbot.baimincrouch, "BodyAim In Crouch");
			Rage_Aimbot.Keybind(LEFT, &g_CVars.Aimbot.BodyAimValue, value, "BodyAim");
			CheckBox(LEFT, g_CVars.Aimbot.baimafterhp, "Body After HP");
			SliderInt(LEFT, true, 200, 0, 100, g_CVars.Aimbot.baimafterhpslider, "HP");


			static bool xxx = false;
			static Color col = Color(147, 125, 199, 255);
			static int colordata[4] = { 147, 125, 199, 255 };


			//Rage_Aimbot.Dropdown(LEFT, 0, 3, g_CVars.Aimbot.Interpolation.LagPrediction, "LagCompensation", LagComp, LagCompActive);

		//	Rage_Aimbot.Dropdown(LEFT, 0, 5, g_CVars.Aimbot.Resolver.Type, "Resolver Type", Resolver, ResolverActive);




			static bool xxxx = false;
			static Color colx = Color(147, 125, 199);
			static int colordatax[4] = { 147, 125, 199, 255 };

			Section(true, RIGHT, 3, "Additional");
		//	CheckBox(RIGHT, g_CVars.Aimbot.AutoWall, "Auto Wall");
			Rage_Aimbot.Dropdown(RIGHT, 0, 4, g_CVars.Aimbot.HitScan, "HitScan", Hitscan, HitscanActive);
			Rage_Aimbot.Dropdown(RIGHT, 0, 3, g_CVars.Aimbot.MultiSpot, "MultiSpot", MultiSpot, MultiSpotAcitve);
			Rage_Aimbot.Dropdown(RIGHT, 0, 4, g_CVars.Aimbot.HitboxOwns, "HitBox Modified", HitBoxModified, HitBoxModifiedActive);
		//	CheckBox(RIGHT, g_CVars.Aimbot.MultiSpot, "Multi Spot");
		//	CheckBox(RIGHT, g_CVars.Aimbot.HitScan, "Hitscan");



			Rage_Aimbot.RenderAllDropdowns();
			Rage_Aimbot.RenderAllColors();

			Rage_Aimbot.ClearVecDropdownsData();
			Rage_Aimbot.ClearVecColorsData();
			if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 54;
			else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 54;
		}
		else if (activetabRage == 3) {
			itemheight[0] = menu.y + tabs[1].h + 65;
			itemheight[1] = menu.y + tabs[1].h + 65;
			static bool pitchtype = false;

			Section(true, LEFT,7, "Anti-Aim");

			CheckBox(LEFT, g_CVars.Miscellaneous.AntiAim.Active, "Active");

			Rage_AntiAim.Dropdown(LEFT, 0, 9, g_CVars.Miscellaneous.AntiAim.Pitch, "Pitch", AntiAimPitch, AntiAimPitchActive);
			Rage_AntiAim.Dropdown(LEFT, 0, ARRAYSIZE(AntiAimYaw), g_CVars.Miscellaneous.AntiAim.Yaw, "Yaw", AntiAimYaw, AntiAimYawActive);

			switch (g_CVars.Miscellaneous.AntiAim.Yaw)
			{
			case 0: yawvariation = yawvariation_1; break;
			case 1: yawvariation = yawvariation_1; break;
			case 2: yawvariation = yawvariation_1; break;
			case 3: yawvariation = yawvariation_2; break;
			case 4: yawvariation = yawvariation_3; break;
			}
			Rage_AntiAim.Dropdown(LEFT, 0, (g_CVars.Miscellaneous.AntiAim.Yaw == 9) ? 1 : 3, g_CVars.Miscellaneous.AntiAim.Variation, "Yaw Mode", yawvariation, yawvariationactive);

			Slider(LEFT, true, 200, 0, 360, g_CVars.Miscellaneous.AntiAim.PitchValue, "Custom Pitch");
			if (g_CVars.Miscellaneous.AntiAim.Yaw == 4)
			{
				Slider(LEFT, true, 200, 0, 360, g_CVars.Miscellaneous.AntiAim.JitterFake, "Jitter Real");
				Slider(LEFT, true, 200, 0, 360, g_CVars.Miscellaneous.AntiAim.JitterReal, "Jitter Fake");
			}
			else
			{
				Slider(LEFT, true, 200, 0, 360, g_CVars.Miscellaneous.AntiAim.RealValue, "Custom Real");
				Slider(LEFT, true, 200, 0, 360, g_CVars.Miscellaneous.AntiAim.FakeValue, "Custom Fake");
			}

			static int key = 40;
			static bool value = false;
			static bool value1 = false;
			static bool value32 = false;
			
			Section(true, RIGHT, 5, "FakeLag");

			CheckBox(RIGHT, g_CVars.Miscellaneous.Fakelag.Active, "Active");

			SliderInt(RIGHT, true, 200, 0, 17, g_CVars.Miscellaneous.Fakelag.Value, "Choked Packets");

			CheckBox(RIGHT, g_CVars.Miscellaneous.Fakelag.InAttack, "In Attack");

			CheckBox(RIGHT, g_CVars.Miscellaneous.Fakelag.AirOnly, "Air Only");


			Rage_FakeLag.Dropdown(RIGHT, 0, ARRAYSIZE(FakeLag), g_CVars.Miscellaneous.Fakelag.Mode, "FakeLag", FakeLag, FakeLagActive);
			
			if (g_CVars.Miscellaneous.AntiAim.Active)
			{
				Section(false, LEFT, 10, "Anti-Aim Adjusment");
				CheckBox(LEFT, g_CVars.Miscellaneous.AntiAim.Static, "In Attack Pitch");
				CheckBox(LEFT, g_CVars.Miscellaneous.AntiAim.opposite, "Jitter Opposite");
				CheckBox(LEFT, g_CVars.Miscellaneous.AntiAim.lowdelta, "Jitter Low Delta");
				CheckBox(LEFT, g_CVars.Miscellaneous.AntiAim.lbybreaker, "LBY Breaker");
				CheckBox(LEFT, g_CVars.Miscellaneous.AntiAim.manuals, "Manuals");
				CheckBox(LEFT, g_CVars.Miscellaneous.AntiAim.TurnOff, "Enemy Check");
				Rage_AntiAim.Dropdown(LEFT, 0, 3, g_CVars.Miscellaneous.AntiAim.AtTargets, "At Targets", AtTargets, AtTargetsActive);
				Misc.Keybind(LEFT, &g_CVars.Miscellaneous.AntiAim.RightManualValue, value, "Right Manual");
				Misc.Keybind(LEFT, &g_CVars.Miscellaneous.AntiAim.LeftManualValue, value1, "Left Manual");
				Misc.Keybind(LEFT, &g_CVars.Miscellaneous.AntiAim.inverterkey, value32, "Inverter");

				Section(false, RIGHT, 2, "Freestand");
				CheckBox(RIGHT, g_CVars.Miscellaneous.AntiAim.WallDetection, "Freestand");
				Rage_AntiAim.Dropdown(RIGHT, 0, 4, g_CVars.Miscellaneous.AntiAim.WallDetectionMode, "Freestand Mode", Edge, EdgeActive);

			}
			if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 65;
			else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 65;
			//	Section(false, LEFT, 2, "Test Section Left x2");
			//	CheckBox(LEFT, g_CVars.Miscellaneous.AntiAim.Active, "q1e q6qe4EW");
			//	CheckBox(LEFT, g_CVars.Miscellaneous.AntiAim.Active, "B1F56");
			 


			
			Rage_AntiAim.RenderAllDropdowns();
			Rage_AntiAim.RenderAllColors();

			Rage_AntiAim.ClearVecDropdownsData();
			Rage_AntiAim.ClearVecColorsData();

			Rage_FakeLag.RenderAllDropdowns();
			Rage_FakeLag.RenderAllColors();

			Rage_FakeLag.ClearVecDropdownsData();
			Rage_FakeLag.ClearVecColorsData();

			//if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 65;
			//else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 65;
		}
	}



else if (tab_guisection.activetabup == 2) {
	if (activetabVisual == 1) {
		itemheight[0] = menu.y + tabs[1].h + 65;
		itemheight[1] = menu.y + tabs[1].h + 65;

		Section(true, LEFT, 10, "ESP");
		Visuals.CheckBoxColor(LEFT, g_CVars.Visuals.ESP.Active, ESPOpen, "Active", g_CVars.Visuals.Chams.ESPColor, ESPData);
		//CheckBox(LEFT, g_CVars.Visuals.ESP.Box, "Box");
		Visuals.Dropdown(LEFT, 0, 3, g_CVars.Visuals.ESP.Box, "Box", EspBox, EspBoxActive);
		CheckBox(LEFT, g_CVars.Visuals.ESP.Fillbox, "Fill Box");
		CheckBox(LEFT, g_CVars.Visuals.ESP.Name, "Name");
		CheckBox(LEFT, g_CVars.Visuals.ESP.Health, "Health");
		CheckBox(LEFT, g_CVars.Visuals.ESP.Armor, "Armor");
		CheckBox(LEFT, g_CVars.Visuals.ESP.Weapon, "Weapon");
		CheckBox(LEFT, g_CVars.Visuals.ESP.Ground, "Ground");
		CheckBox(LEFT, g_CVars.Visuals.ESP.Hit, "Hitmarker");
		CheckBox(LEFT, g_CVars.Visuals.ESP.EnemyOnly, "Enemy Only");


		Section(true, RIGHT, 3, "Chams Editor");

		Visuals.Dropdown(RIGHT, 0, ARRAYSIZE(ChamsName), g_CVars.Visuals.Chams.ChamsChanger, "Type", ChamsName, ChamsOpened);
		switch (g_CVars.Visuals.Chams.ChamsChanger)
		{
		case 1:	Visuals.CheckBoxColor(RIGHT, g_CVars.Visuals.Chams.LocalChams, LocalChamsOpen, "Local Chams", g_CVars.Visuals.Chams.LocalChamsColor, LocalChamsData, true); Visuals.Dropdown(RIGHT, 0, ARRAYSIZE(MaterialType), g_CVars.Visuals.Chams.LocalMaterialType, "Material Type", MaterialType, MaterialOpen); break;
		case 2:	Visuals.CheckBoxColor(RIGHT, g_CVars.Visuals.Chams.EnemyChams, EnemyChamsOpen, "Enemy Chams", g_CVars.Visuals.Chams.EnemyChamsColor, EnemyChamsData, true); Visuals.Dropdown(RIGHT, 0, ARRAYSIZE(MaterialType), g_CVars.Visuals.Chams.EnemyMaterialType, "Enemy Type", MaterialType, MaterialOpen); break;
		case 3:	Visuals.CheckBoxColor(RIGHT, g_CVars.Visuals.Chams.WeaponChams, WeaponChamsOpen, "Weapon Chams", g_CVars.Visuals.Chams.WeaponChamsColor, WeaponChamsData, true); Visuals.Dropdown(RIGHT, 0, ARRAYSIZE(MaterialType), g_CVars.Visuals.Chams.WeaponMaterialType, "Weapon Type", MaterialType, MaterialOpen); break;
		case 4:	Visuals.CheckBoxColor(RIGHT, g_CVars.Visuals.Chams.HandChams, HandChamsOpen, "Hand Chams", g_CVars.Visuals.Chams.HandChamsColor, HandChamsData, true); Visuals.Dropdown(RIGHT, 0, ARRAYSIZE(MaterialType), g_CVars.Visuals.Chams.HandMaterialType, "Weapon Type", MaterialType, MaterialOpen); break;
		}

		Section(false, RIGHT, 1, "Debug");
		CheckBox(RIGHT, g_CVars.Visuals.ESP.steamid, "Steamid Debug");

		Visuals.RenderAllDropdowns();
		Visuals.RenderAllColors();

		Visuals.ClearVecDropdownsData();
		Visuals.ClearVecColorsData();

	}
	if (activetabVisual == 2) {
		itemheight[0] = menu.y + tabs[1].h + 65;
		itemheight[1] = menu.y + tabs[1].h + 65;


		Section(true, RIGHT, 5, "Misc");


		CheckBox(RIGHT, g_CVars.Visuals.ESP.Fullbright, "FullBright");
		CheckBox(RIGHT, g_CVars.Visuals.ESP.trails, "Energy Splash Effect");


		Slider(RIGHT, true, 200, 0, 1, g_CVars.Visuals.ASUS, "Asus Walls");
		Slider(RIGHT, true, 200, 0, 1, g_CVars.Visuals.NightMode, "NightMode");
		//Slider(RIGHT, true, 200, 0, 90, g_CVars.Visuals.ViewModelFOV, "ViewModel Fov");



		Section(false, RIGHT, 1, "Radar");
		CheckBox(RIGHT, g_CVars.Visuals.Radar, "Radar");

		//Section(false, RIGHT, 1, "Fov");
		//Slider(RIGHT, true, 200, 10, 200, g_CVars.Visuals.fo, "Asus Walls");

		Section(true, LEFT, 5, "Local Screen");

		CheckBox(LEFT, g_CVars.Visuals.NoSky, "Sky Deleting");
		CheckBox(LEFT, g_CVars.Visuals.NoSmoke, "Anti Smoke");
		CheckBox(LEFT, g_CVars.Visuals.NoFlash, "Anti Flash");
		CheckBox(LEFT, g_CVars.Visuals.NoHands, "Hands Deleting");
		CheckBox(LEFT, g_CVars.Visuals.NoVisualRecoil, "No Visual Recoil");

		//	static bool xxx1 = false;
		//	static int colordata1[4] = { g_CVars.Visuals.WallColor.r, g_CVars.Visuals.WallColor.g, g_CVars.Visuals.WallColor.b };
		//	Visuals.CheckBoxColor(LEFT, g_CVars.Visuals.ColorWalls, xxx1, "Night Mode", g_CVars.Visuals.WallColor, colordata1);

	
		Section(false, LEFT, 1, "Crosshair");

		Visuals.Dropdown(LEFT, 0, 5, g_CVars.Visuals.Crosshair.Type, "Crosshair", Crosshair, CrosshairActive);


		Visuals.RenderAllDropdowns();
		Visuals.RenderAllColors();

		Visuals.ClearVecDropdownsData();
		Visuals.ClearVecColorsData();

		if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 65;
		else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 65;

	}
	if (activetabVisual == 3)
	{
		itemheight[0] = menu.y + tabs[1].h + 65;
		itemheight[1] = menu.y + tabs[1].h + 65;

		Section(true, LEFT, 3, "Custom Fog");
		static bool xx = false;
		static int colordataxx[4] = { g_CVars.Visuals.FogColor.r, g_CVars.Visuals.FogColor.g, g_CVars.Visuals.FogColor.b, g_CVars.Visuals.FogColor.a };
		Visuals.CheckBoxColor(LEFT, g_CVars.Visuals.ColorFog, xx, "Fog Changer", g_CVars.Visuals.FogColor, colordataxx);
		SliderInt(LEFT, true, 200, 0, 8000, g_CVars.Visuals.Start, "Start");
		SliderInt(LEFT, true, 200, 0, 8000, g_CVars.Visuals.End, "End");


		Visuals.RenderAllDropdowns();
		Visuals.RenderAllColors();

		Visuals.ClearVecDropdownsData();
		Visuals.ClearVecColorsData();

		if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 65;
		else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 65;
	}
	}
	else if (tab_guisection.activetabup == 3) {
		if (activetabMisc == 1) {
			itemheight[0] = menu.y + tabs[1].h + 65;
			itemheight[1] = menu.y + tabs[1].h + 65;
			static int key = 40;
			static bool value = false;
			static bool value21 = false;
			static bool value1 = false;			
			static bool value5 = false;
			static bool value2 = false;
			static bool value4 = false;
			Section(true, LEFT, 11, "Other");

			static bool xxxx = false;
			static int colordatax[3] = { g_CVars.Visuals.BulletColor.r, g_CVars.Visuals.BulletColor.g, g_CVars.Visuals.BulletColor.b };
			Misc.CheckBoxColor(LEFT, g_CVars.Visuals.BulletTracer, xxxx, "Bullet Tracer", g_CVars.Visuals.BulletColor, colordatax);
			static bool xxx = false;
			static int colordata[4] = { g_CVars.Visuals.ImpactColor.r, g_CVars.Visuals.ImpactColor.g, g_CVars.Visuals.ImpactColor.b, g_CVars.Visuals.ImpactColor.a };
			Misc.CheckBoxColor(LEFT, g_CVars.Visuals.Impact, xxx, "Bullet Impact", g_CVars.Visuals.ImpactColor, colordata, 150);

			CheckBox(LEFT, g_CVars.Miscellaneous.AutoKnife, "KnifeBot");
			CheckBox(LEFT, g_CVars.Miscellaneous.FastThrow, "Fast Grenade Throw");
			CheckBox(LEFT, g_CVars.Miscellaneous.RoundSay, "RoundSay");
			CheckBox(LEFT, g_CVars.Miscellaneous.KillSay, "KillSay");

			CheckBox(LEFT, g_CVars.Miscellaneous.FakeDuck, "FakeDuck");

			CheckBox(LEFT, g_CVars.Miscellaneous.AntiAim.DuckInAir, "DuckInAir");

			Misc.Keybind(LEFT, &g_CVars.Miscellaneous.FakeDuckValue, value, "FakeDuck");


			Section(true, RIGHT, 8, "Movement");

			CheckBox(RIGHT, g_CVars.Miscellaneous.CircleStrafe, "Circle Strafe");

			Misc.Keybind(RIGHT, &g_CVars.Miscellaneous.CircleStrafeValue, value2, "Circle Strafe");

			CheckBox(RIGHT, g_CVars.Miscellaneous.slowwalk, "Slow Walk");

			Misc.Keybind(RIGHT, &g_CVars.Miscellaneous.slowwalkvalue, value5, "SlowWalk");

			SliderInt(RIGHT, true, 200, 0, 100, g_CVars.Miscellaneous.slowwalkspeed, "SlowWalk Speed");

			Misc.Keybind(RIGHT, &g_CVars.Miscellaneous.FakeWalkValue, value1, "FakeWalk");


			CheckBox(RIGHT, g_CVars.Miscellaneous.BunnyHop, "BunnyHop");
			CheckBox(RIGHT, g_CVars.Miscellaneous.AutoStrafe, "AutoStrafer");

	


			Section(false, RIGHT, 2, "Sound");

			Misc.Dropdown(RIGHT, 0, 6, g_CVars.Visuals.ESP.HitSound, "HitSound", Hitsound, HitsoundActive);
			Misc.Dropdown(RIGHT, 0, 5, g_CVars.Visuals.ESP.KillSound, "KillSound", KillSound, KillSoundActive);

			Misc.RenderAllDropdowns();
			Misc.RenderAllColors();

			Misc.ClearVecDropdownsData();
			Misc.ClearVecColorsData();

			if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 65;
			else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 65;
		}
		if (activetabMisc == 2)
		{
			itemheight[0] = menu.y + tabs[1].h + 65;
			itemheight[1] = menu.y + tabs[1].h + 65;
			static int key = 40;
			static bool value = false;
			static bool value1 = false;
			static bool value2 = false;
			static bool value3 = false;
			Section(true, LEFT, 3, "Exploits");
			CheckBox(LEFT, g_CVars.Miscellaneous.AirStuck, "AirStuck (F)");
			CheckBox(LEFT, g_CVars.Miscellaneous.LagExploit, "Lag exploit");
			Misc.Keybind(LEFT, &g_CVars.Miscellaneous.LagExploitValue, value, "Lag Exploit");


		//	Section(true, RIGHT, 2, "Exploits");
		//	Misc.Keybind(RIGHT, &g_CVars.Miscellaneous.DefensiveValue, value2, "Teleport On Shot");
		//	Misc.Keybind(RIGHT, &g_CVars.Miscellaneous.DefensiveValue1, value3, "Teleport On Shot2 ");

			if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 65;
			else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 65;
		}
	}
	

	else if (tab_guisection.activetabup == 4) {
		if (activetabConfig == 1) {
			itemheight[0] = menu.y + tabs[1].h + 65;
			itemheight[1] = menu.y + tabs[1].h + 65;

				WIN32_FIND_DATAA ffd;
				LARGE_INTEGER filesize;
				CHAR szDir[MAX_PATH];
				HANDLE hFind = INVALID_HANDLE_VALUE;


				char szPath[MAX_PATH];

				GetModuleFileNameA(g_Config.m_hModule, szPath, MAX_PATH);
				if ( !DirectoryExists( "C:\\NoctuaHook\\" ) )
				{
					std::filesystem::create_directory( "C:\\NoctuaHook\\" );
					std::ofstream output( "C:\\NoctuaHook\\default.cfg" );
				}
				std::string strPath("C:\\NoctuaHook\\");


				strncpy_s(szDir, strPath.c_str(), MAX_PATH);
				strncat_s(szDir, "*", MAX_PATH);

				hFind = FindFirstFileA(szDir, &ffd);

				std::vector<std::string> files;

				static int m_iConfig = 0;
				int index = 0;

				int size_section = 0;

				if (hFind != INVALID_HANDLE_VALUE) {
					do
					{
						auto ext = GetExtension(ffd.cFileName);

						if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && ext.compare("cfg") == 0) {
							filesize.LowPart = ffd.nFileSizeLow;
							filesize.HighPart = ffd.nFileSizeHigh;

							index++;
							files.push_back(ffd.cFileName);

							size_section++;
						}
					} while (FindNextFileA(hFind, &ffd) != 0);

					FindClose(hFind);
				}

				Section(true, LEFT, size_section + 3, "Config List");



				static int value = 0;

				std::vector<std::string> list = files; //vectorOfDropdowns[i].list;


				//g_GUI.itemheight[LEFT] += 27;

				int x = g_GUI.columns[LEFT] + 12;
				int y = g_GUI.itemheight[LEFT] + 38;

				int w = 123;

				g_Drawing.FilledRect(x, y - 1, w + 129, 1, Color(0, 0, 0));

				for (auto i = 0; i < size_section; ++i) {
					if (i == size_section - 1) g_Drawing.FilledRect(x, y + 1 - 1 + i * 27, w + 129, 27 + 1, Color(0, 0, 0));
					else g_Drawing.FilledRect(x, y + i * 27, w + 129, 25 + 2, Color(0, 0, 0));
					g_Drawing.FilledRect(x + 1, y + 1 + i * 27 - 1, w + 127, 27, Color(25, 25, 25));

					if (!g_Stuff.Mouse.IsInBox(x, y + 1 + i * 27 - 1, w + 129, 27) && i != value)
						g_Drawing.MenuStringNormal3(false, false, x + 9, y + 1 + i * 27 + 4, Color(255, 255, 255), list.at(i).c_str());
					else if (g_Stuff.Mouse.IsInBox(x, y + 1 + i * 27 - 1, w + 129, 27) && i != value)
						g_Drawing.MenuStringNormal3(false, false, x + 9, y + 1 + i * 27 + 4, Color(237, 94, 94), list.at(i).c_str());

					if (i == value) {
						//g_Drawing.MenuStringNormal3(false, false, x + 9, y - 18, Color(255, 255, 255), list.at(i).c_str());
						g_Drawing.MenuStringNormal3(false, false, x + 9, y + 1 + i * 27 + 4, Color(237, 87, 87), list.at(i).c_str());
					}

					if (g_Stuff.Mouse.Click(x, y + 1 + i * 27 - 1, w + 129, 27)) {
						if (i != value) {
							//g_Drawing.MenuStringNormal3(false, false, x + 9, y - 18, Color(255, 255, 255), list.at(i).c_str());
							value = i;
							g_CVars.Miscellaneous.ConfigName = strPath + list.at(i);
							std::cout << g_CVars.Miscellaneous.ConfigName << std::endl;
						}
					}
				}

				//g_Drawing.FilledRect(x + 1, y -1, w + 127, size_section * 22, Color(255, 0, 0, 128));

				g_GUI.itemheight[LEFT] += 27 * size_section;



				ButtonBox(LEFT, g_CVars.Miscellaneous.Load, "Load");
				ButtonBox(LEFT, g_CVars.Miscellaneous.Save, "Save");
				ButtonBox( LEFT, g_CVars.Miscellaneous.Open, "Open Configs Folder" );


				if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 65;
				else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 65;
			}
		}
		else {
			itemheight[0] = menu.y + tabs[1].h + 65;
			itemheight[1] = menu.y + tabs[1].h + 65;


			g_CVars.Menu.h = itemheight[0] - menu.y + 65;

			if (itemheight[0] > itemheight[1]) g_CVars.Menu.h = itemheight[0] - menu.y + 65;
			else if (itemheight[1] > itemheight[0]) g_CVars.Menu.h = itemheight[1] - menu.y + 65;
		}

		if (g_CVars.Menu.h < 421)
			g_CVars.Menu.h = 421;


		itemscount = 0;
	}

	void GUI::Radar(int x, int y, int w, int h)
	{
		int titlebar = 5;

		g_Stuff.Mouse.DragRadar(x, y, w, h);

		g_Drawing.FilledRect(x, y, w, h, g_CVars.Colors.radar_fill);
		g_Drawing.OutlinedRect(x, y, w, h, g_CVars.Colors.radar_outline);
		g_Drawing.OutlinedRect(x - 1, y - 1, w + 2, h + 2, g_CVars.Colors.radar_outline2);

		int radar_x = x + 5;
		int radar_y = y + 5;
		int radar_w = w - 10;
		int radar_h = h - 10;

		g_Drawing.FilledRect(radar_x, radar_y, radar_w, radar_h, g_CVars.Colors.radar_inside_fill);

		for (int i = 0; i < 12; i++)
		{
			g_pSurface->DrawSetColor(Color(70, 70, 70, 72));
			g_pSurface->DrawLine(radar_x, radar_y + 20 * i, radar_x + radar_w, radar_y + 20 * i);
		}

		for (int i = 0; i < 12; i++)
		{
			g_pSurface->DrawSetColor(Color(70, 70, 70, 72));
			g_pSurface->DrawLine(radar_x + 20 * i, radar_y, radar_x + 20 * i, radar_y + radar_h);
		}

		g_Drawing.FilledRect(radar_x, radar_y + (radar_h / 2), radar_w, 1, g_CVars.Colors.radar_separator);
		g_Drawing.FilledRect(radar_x + (radar_w / 2), radar_y, 1, radar_h, g_CVars.Colors.radar_separator);
		g_Drawing.OutlinedRect(radar_x, radar_y, radar_w, radar_h, g_CVars.Colors.radar_inside_outline);

		BasePlayer* LocalPlayer = (BasePlayer*)g_pClientEntityList->GetClientEntity(g_pEngineClient->GetLocalPlayer());
		if (!LocalPlayer) return;

		player_info_t PlayerInfo{};
		Color colour;

		for (int Index = 1; Index <= g_pGlobals->maxClients; Index++)
		{
			BasePlayer* Ent = (BasePlayer*)g_pClientEntityList->GetClientEntity(Index);
			if (!Ent || Ent == LocalPlayer) continue;
			if (Ent->IsDormant()) continue;
			if (!(Ent->m_lifeState() == 0)) continue;

			if (g_CVars.PlayerList.Friend[Index]) colour = Color::White;
			else
			{
				if (Ent->m_iTeamNum() == 2) colour = Color::Red;
				else if (Ent->m_iTeamNum() == 3) colour = Color::LightBlue;
			}

			static int x1, yx1;
			float dx = Ent->GetAbsOrigin().x - LocalPlayer->GetAbsOrigin().x;
			float dy = Ent->GetAbsOrigin().y - LocalPlayer->GetAbsOrigin().y;
			float flAngle = g_Stuff.radarangles.y;
			float yaw = (flAngle) * (M_PI / 180.0);
			float mainViewAngles_CosYaw = cos(yaw);
			float mainViewAngles_SinYaw = sin(yaw);
			float ax = dy * (-mainViewAngles_CosYaw) + dx * mainViewAngles_SinYaw;
			float ay = dx * (-mainViewAngles_CosYaw) - dy * mainViewAngles_SinYaw;
			float range = 100 * 30;

			if (fabs(ax) > range || fabs(ay) > range)
			{
				if (ay > ax)
				{
					if (ay > -ax)
					{
						ax = range * ax / ay;
						ay = range;
					}
					else
					{
						ay = -range * ay / ax;
						ax = -range;
					}
				}
				else
				{
					if (ay > -ax)
					{
						ay = range * ay / ax;
						ax = range;
					}
					else
					{
						ax = -range * ax / ay;
						ay = -range;
					}
				}
			}

			int ScreenX = radar_x + (radar_w / 2) + int(ax / range * float(125));
			int ScreenY = radar_y + (radar_h / 2) + int(ay / range * float(125));
			int size = g_CVars.Radar.w;
			size -= 10;

			if (ScreenX < radar_x - size) ScreenX = radar_x - size + 5;
			else if (ScreenX > radar_x + size) ScreenX = radar_x + size - 5;

			if (ScreenY < radar_y - size) ScreenY = radar_y - size + 5;
			else if (ScreenY > radar_y + size) ScreenY = radar_y + size - 5;

			g_Drawing.FilledRect(ScreenX - 3, ScreenY - 1, 7, 3, Color::Black);
			g_Drawing.FilledRect(ScreenX - 1, ScreenY - 3, 3, 7, Color::Black);

			g_Drawing.FilledRect(ScreenX - 2, ScreenY, 5, 1, colour);
			g_Drawing.FilledRect(ScreenX, ScreenY - 2, 1, 5, colour);
		}
	}

	void GUI::ColorSchemes()
	{
		if (g_CVars.Colors.Scheme == 1)
		{
			g_CVars.Colors.gui_fill = Color(40, 40, 40, 245);
			g_CVars.Colors.gui_fill2 = Color(100, 100, 100, 220);
			g_CVars.Colors.gui_outline = Color(60, 60, 60, 255);
			g_CVars.Colors.gui_outline2 = Color::Black;
			g_CVars.Colors.gui_sections = Color(90, 90, 90, 128);
			g_CVars.Colors.inside_fill = Color(40, 40, 40, 255);
			g_CVars.Colors.inside_outline = Color(60, 60, 60, 255);

			g_CVars.Colors.tabs_fill = Color(110, 110, 110, 200);
			g_CVars.Colors.tabs_outline = Color(30, 30, 30, 200);

			g_CVars.Colors.radar_fill = Color(40, 40, 40, 245);
			g_CVars.Colors.radar_fill2 = Color(100, 100, 100, 220);
			g_CVars.Colors.radar_outline = Color(60, 60, 60, 255);
			g_CVars.Colors.radar_outline2 = Color::Black;
			g_CVars.Colors.radar_inside_fill = Color(50, 50, 50, 192);
			g_CVars.Colors.radar_inside_outline = Color(60, 60, 60, 255);
			g_CVars.Colors.radar_separator = Color(70, 70, 70, 192);

			g_CVars.Colors.maincolor = Color(241, 24, 121, 255);
			g_CVars.Colors.maincolorfade = Color(241, 24, 121, 255);
			g_CVars.Colors.mouseoutline = Color::Black;
		}
		else if (g_CVars.Colors.Scheme == 2)
		{
			g_CVars.Colors.gui_fill = Color(215, 215, 215, 220);
			g_CVars.Colors.gui_fill2 = Color(150, 150, 150, 220);
			g_CVars.Colors.gui_outline = Color(195, 195, 195, 255);
			g_CVars.Colors.gui_outline2 = Color(60, 60, 60, 200);
			g_CVars.Colors.gui_sections = Color(90, 90, 90, 128);
			g_CVars.Colors.inside_fill = Color(170, 170, 170, 255);
			g_CVars.Colors.inside_outline = Color(60, 60, 60, 255);

			g_CVars.Colors.tabs_fill = Color(125, 125, 125, 200);
			g_CVars.Colors.tabs_outline = Color(90, 90, 90, 200);

			g_CVars.Colors.radar_fill = Color(215, 215, 215, 220);
			g_CVars.Colors.radar_fill2 = Color(150, 150, 150, 220);
			g_CVars.Colors.radar_outline = Color(195, 195, 195, 255);
			g_CVars.Colors.radar_outline2 = Color(60, 60, 60, 200);
			g_CVars.Colors.radar_inside_fill = Color(170, 170, 170, 200);
			g_CVars.Colors.radar_inside_outline = Color(60, 60, 60, 255);
			g_CVars.Colors.radar_separator = Color(70, 70, 70, 192);

			g_CVars.Colors.maincolor = Color(241, 24, 121, 255);
			g_CVars.Colors.maincolorfade = Color(241, 24, 121, 255);
			g_CVars.Colors.mouseoutline = Color(60, 60, 60, 200);
		}
	}

	void GUI::Draw()
	{
		ColorSchemes();

		if (g_pEngineClient->IsInGame() && g_CVars.Visuals.Radar) Radar(g_CVars.Radar.x, g_CVars.Radar.y, g_CVars.Radar.w, g_CVars.Radar.h);

		if (GetAsyncKeyState(VK_INSERT) & 1) bMouse = !bMouse;
		if (!bMouse) return;

		g_Stuff.Mouse.Wrapper();

		if (g_CVars.Miscellaneous.Load)
		{
			g_Config.Load(g_CVars.Miscellaneous.ConfigName);
			g_CVars.Miscellaneous.Load = false;
		}

		if (g_CVars.Miscellaneous.Save)
		{
			g_Config.Save(g_CVars.Miscellaneous.ConfigName);
			g_CVars.Miscellaneous.Save = false;
		}
		if ( g_CVars.Miscellaneous.Open )
		{
			ShellExecute( NULL, NULL, "C:\\NoctuaHook", NULL, NULL, SW_SHOWNORMAL );
			g_CVars.Miscellaneous.Open = false;
		}
		Menu(g_CVars.Menu.x, g_CVars.Menu.y, g_CVars.Menu.w, g_CVars.Menu.h);
		g_Stuff.Mouse.Draw(g_CVars.Colors.maincolor, g_CVars.Colors.mouseoutline);

		if (g_CVars.Radar.x < 0) g_CVars.Radar.x = 0;
		if (g_CVars.Radar.y < 0) g_CVars.Radar.y = 0;
		int radarx = screen_x - g_CVars.Radar.h;
		int radary = screen_y - g_CVars.Radar.w;
		if (g_CVars.Radar.x > radarx) g_CVars.Radar.x = radarx;
		if (g_CVars.Radar.y > radary) g_CVars.Radar.y = radary;

		if (g_CVars.Menu.x < 0) g_CVars.Menu.x = 0;
		if (g_CVars.Menu.y < 0) g_CVars.Menu.y = 0;
		int menux = screen_x - g_CVars.Menu.w;
		int menuy = screen_y - g_CVars.Menu.h;
		if (g_CVars.Menu.x > menux) g_CVars.Menu.x = menux;
		if (g_CVars.Menu.y > menuy) g_CVars.Menu.y = menuy;
	}