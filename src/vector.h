#ifndef __SimpleRayTracer__vector__
#define __SimpleRayTracer__vector__

#include <iostream>

class Vector
{
public:

	static const Vector right() { return Vector(1, 0, 0); }
	static const Vector up() { return Vector(0, 1, 0); }
	static const Vector forward() { return Vector(0, 0, 1); }
	static const Vector zero() { return Vector(0, 0, 0); }

    float X;
    float Y;
    float Z;
    
    Vector( float x, float y, float z);
    Vector();
    
    float dot(const Vector& v) const;
    Vector cross(const Vector& v) const;
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector& operator+=(const Vector& v);
    Vector operator*(float c) const;
	Vector operator*(const Vector& v) const;
    Vector operator-() const;
    Vector& normalize();
    float length() const;
    float lengthSquared() const;
    Vector reflection( const Vector& normal) const;
    bool triangleIntersection( const Vector& d, const Vector& a, const Vector& b,
                              const Vector& c, float& s) const;
    bool yPlaneIntersection( const Vector& d, const Vector& n, float& s) const;
    std::string toString() const;
 };

std::ostream& operator<<(std::ostream& os, const Vector& v);
bool cmpf(float A, float B, float epsilon);
#endif /* defined(__SimpleRayTracer__vector__) */
