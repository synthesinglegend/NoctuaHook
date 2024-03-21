/************************************************************************
*					Portable v34 SDK by InUrFace						*
*							15. 03. 2014								*
*					 Not for commercial purposes						*
*																		*
*																		*
************************************************************************/
#ifndef __COLOR_H__
#define __COLOR_H__

#include "Vector.h"
#define MENU_ALPHA 255
namespace Valve
{
	/*typedef struct
	{
		//byte r, g, b, a;
		std::uint8_t r, g, b, a;
	} color32;*/

	class Color
	{
	public:
		Color();
		Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);
		Color(const Color& color);

		Color& operator=(const Color& color);

		std::uint8_t& operator[ ](std::uint32_t i);
		std::uint8_t	operator[ ](std::uint32_t i) const;

	public:
		static Color	Empty;
		static Color	White;
		static Color	Black;
		static Color	Red;
		static Color	Green;
		static Color	Blue;
		static Color	Cyan;
		static Color	Yellow;
		static Color	LightBlue;
		static Color	Pink;
		static Color	FromHSB(float hue, float saturation, float brightness);

	public:
		std::uint8_t r, g, b, a;
	};
};

#endif // __COLOR_H__
