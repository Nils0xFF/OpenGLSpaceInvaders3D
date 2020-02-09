#include "color.h"
#include <assert.h>
#include <math.h>

Color::Color():R(0.0f),G(0.0f),B(0.0f)
{
  // TODO: add your code
}

Color::Color( float r, float g, float b):R(r),G(g),B(b)
{
	// TODO: add your code
}

Color Color::operator*(const Color& c) const
{
    float r = this->R * c.R;
    float g = this->G * c.G;
    float b = this->B * c.B;
    return Color(r,g,b);
}

Color Color::operator*(const float Factor) const
{
    float r = this->R * Factor;
    float g = this->G * Factor;
    float b = this->B * Factor;
	return Color(r,g,b);
}

Color Color::operator+(const Color& c) const
{
	return Color(this->R + c.R,this->G + c.G, this->B + c.B); // dummy (remove)
}

Color& Color::operator+=(const Color& c)
{
    this->R += c.R;
    this->G += c.G;
    this->B += c.B;
	return *this; // dummy (remove)
}

float Color::grayscale() const
{
    return (R+G+B)/3.0f;
}

Color Color::powColor(float exponent) const{
    return Color(pow(this->R, exponent), pow(this->G, exponent), pow(this->B, exponent));
}
