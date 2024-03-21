#include "Main.h"

Color Color::Empty(0, 0, 0, 0);
Color Color::White(255, 255, 255, 255);
Color Color::Black(0, 0, 0, 255);
Color Color::Red(255, 0, 0, 255);
Color Color::Green(0, 255, 0, 255);
Color Color::Blue(0, 0, 255, 255);
Color Color::Cyan(0, 128, 255, 255);
Color Color::Yellow(255, 255, 0, 255);
Color Color::LightBlue(160, 200, 255);
Color Color::Pink(255, 0, 255, 255);

Color::Color()
	: r(0),
	g(0),
	b(0),
	a(0)
{

}

Color::Color(std::uint8_t R, std::uint8_t G, std::uint8_t B, std::uint8_t A)
	: r(R),
	g(G),
	b(B),
	a(A)
{

}

Color::Color(const Color& color)
	: r(color.r),
	g(color.g),
	b(color.b),
	a(color.a)
{

}

Color& Color::operator=(const Color& color)
{
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;

	return *this;
}

std::uint8_t& Color::operator[ ](std::uint32_t i)
{
	return ((std::uint8_t*)this)[i];
}


std::uint8_t Color::operator[ ](std::uint32_t i) const
{
	return ((std::uint8_t*)this)[i];
}

Color Color::FromHSB(float hue, float saturation, float brightness)
{
	float h = hue == 1.0f ? 0 : hue * 6.0f;
	float f = h - (int)h;
	float p = brightness * (1.0f - saturation);
	float q = brightness * (1.0f - saturation * f);
	float t = brightness * (1.0f - (saturation * (1.0f - f)));

	if (h < 1)
	{
		return Color(
			(unsigned char)(brightness * 255),
			(unsigned char)(t * 255),
			(unsigned char)(p * 255)
		);
	}
	else if (h < 2)
	{
		return Color(
			(unsigned char)(q * 255),
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255)
		);
	}
	else if (h < 3)
	{
		return Color(
			(unsigned char)(p * 255),
			(unsigned char)(brightness * 255),
			(unsigned char)(t * 255)
		);
	}
	else if (h < 4)
	{
		return Color(
			(unsigned char)(p * 255),
			(unsigned char)(q * 255),
			(unsigned char)(brightness * 255)
		);
	}
	else if (h < 5)
	{
		return Color(
			(unsigned char)(t * 255),
			(unsigned char)(p * 255),
			(unsigned char)(brightness * 255)
		);
	}
	else
	{
		return Color(
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255),
			(unsigned char)(q * 255)
		);
	}
}