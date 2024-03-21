#ifndef __GUI_H_
#define __GUI_H_


const std::string key_array[124] =
{
    "",
    "Mouse 1",
    "Mouse 2",
    "",
    "Mouse 3",
    "Mouse 4",
    "Mouse 5",
    "",
    "Backspace",
    "Tab",
    "",
    "",
    "",
    "Enter",
    "",
    "",
    "Shift",
    "Ctrl",
    "Alt",
    "Pause",
    "Caps Lock",
    "",
    "",
    "",
    "",
    "",
    "",
    "Escape",
    "",
    "",
    "",
    "",
    "Space",
    "Page Up",
    "Page Down",
    "End",
    "Home",
    "Left",
    "Up",
    "Right",
    "Down",
    "",
    "",
    "",
    "Print",
    "Insert",
    "Delete",
    "",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "",
    "",
    "",
    "",
    "",
    "Num0",
    "Num1",
    "Num2",
    "Num3",
    "Num4",
    "Num5",
    "Num6",
    "Num7",
    "Num8",
    "Num9",
    "Multi",
    "Add",
    "",
    "Subtract",
    "Decimal",
    "Divide",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12"
};

const std::string& GetNameFromCode(uint32_t code)
{
    if (code > 124)
        return key_array[0];

    return key_array[code];
}




typedef struct DropDownFlag_s
{
    int x, y, maxListCount;

    int* currentIndex;

    bool* isActive;

    const char** list;
}DropDownFlag_t;

typedef struct CheckboxColorFlag_s
{
    int x, y, h;

    int* colordata;

    bool isAlpha, isActive;

    Color* color;
}CheckboxColorFlag_t;

typedef struct KeybindFlag_s
{
    bool isActive;
}KeybindFlag_t;


class GUISection {
public:
    void RenderAllDropdowns();
    void RenderAllColors();

    void DisableAllDropdowns();
    void DisableAllColors();

    void Dropdown(int, int, int, int&, const char*, const char**, bool&);
    void SliderColor(int, bool, int, int, int, int, int, int&, int, Color&);
    void CheckBoxColor(int, bool&, bool&, const char*, Color&, int*, bool);
    void Keybind(int, int*, bool&, const char*);



    void ClearVecDropdownsData() { vectorOfDropdowns.clear(); }

    void ClearVecColorsData() { vectorOfColors.clear(); }



    void SetActiveDropdown(bool value) { isRandomDropdownActive = value; }

    bool AnyDropdownAreActive() { return isRandomDropdownActive; }


    void SetActiveColor(bool value) { isRandomColorActive = value; }

    bool AnyColorAreActive() { return isRandomColorActive; }


    void SetActiveKeybind(bool value) { isRandomKeybindActive = value; }

    bool AnyKeybindAreActive() { return isRandomKeybindActive; }


    void SetFocusedWindow(int index) { focusedWindowIndex = index; }
private:
    std::vector<DropDownFlag_t> vectorOfDropdowns;
    std::vector<CheckboxColorFlag_t> vectorOfColors;

    bool isRandomKeybindActive, isRandomDropdownActive, isRandomColorActive;

    bool disableAllDropdowns, disableAllColors;

    int focusedWindowIndex;

    bool AddNewDropdown(DropDownFlag_t dropdownData)
    {
        vectorOfDropdowns.push_back(dropdownData);
        return true;
    }

    bool AddNewColor(CheckboxColorFlag_t checkboxColorData)
    {
        vectorOfColors.push_back(checkboxColorData);
        return true;
    }

    bool IsItThisFocusedWindow(int index) { return (focusedWindowIndex == index); }
};


class TabGUISection
{
public:

    void AddTabUp(int, int, int, int, int, const char*, Color, Color);
    void AddTabDown(int, int, int, int, int&, int, const char*, Color, Color);

    inline void clear()
    {
        max_size = 0;
        max_tabup_size = 0;
    }

public:

    int activetabup = 1;

private:
    int max_size = 0;
    int max_tabup_size = 0;

};


static int itemscount = 0;


class GUI
{
public:
    struct tabs
    {
        int x, y, w, h, half, secondhalf;
    }tabs[6];

    struct menu
    {
        int x, y, w, h;
    }menu;

    int activetabup, activetabRage, activetabVisual, activetabMisc, activetabOther, activetabConfig;
    int columns[2];
    int itemheight[2];

    bool ShouldDisableInput();
    void SliderInt(int, bool, int, int, int, int&, const char*);
    void Slider(int, bool, int, float, float, float&, const char*);
    void CheckBox(int, bool&, const char*);
    void ButtonBox(int, bool&, const char*);
    void Text(int, const char*, Color);
    void Section(bool, int, int, const char*);
    void DrawingTabUp(int, int, int, int, int, const char*, Color, Color);
    void DrawingTabDown(int, int, int, int, int&, int, const char*, Color, Color);
    void Menu(int, int, int, int);
    void Radar(int, int, int, int);
    void tabsInit();
    void menuRender(int, int, int, int);
    void tabsRender();
    void ColorSchemes();
    void Draw();
};
extern GUI g_GUI;

#endif // __GUI_H_