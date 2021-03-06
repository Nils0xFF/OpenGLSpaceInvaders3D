#ifndef __SimpleRayTracer__color__
#define __SimpleRayTracer__color__

#include <iostream>

class Color
{
public:
    float R = 0;
    float G = 0;
    float B = 0;
    
    Color();
    Color( float r, float g, float b);
    float grayscale() const;
    Color operator*(const Color& c) const;
    Color operator*(const float Factor) const;
    Color operator+(const Color& c) const;
    Color& operator+=(const Color& c);
    Color powColor(float exponent) const;
    std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const Color& c);


#endif /* defined(__SimpleRayTracer__color__) */
