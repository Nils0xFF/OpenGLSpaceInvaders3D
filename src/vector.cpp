#include "vector.h"
#include <assert.h>
#include <math.h>
#include <sstream>

Vector::Vector( float x, float y, float z):Y(y),X(x),Z(z)
{
	// TODO: add your code
}

Vector::Vector()
{
	// TODO: add your code
}

float Vector::dot(const Vector& v) const
{
	return this->X * v.X + this->Y * v.Y + this->Z * v.Z;
}

Vector Vector::cross(const Vector& v) const
{
	return Vector(
                  this->Y * v.Z - this->Z * v.Y,
                  this->Z * v.X - this->X * v.Z,
                  this->X * v.Y - this->Y * v.X
    );
}



Vector Vector::operator+(const Vector& v) const
{
	return Vector(
        this->X + v.X,
        this->Y + v.Y,
        this->Z + v.Z
    );
}

Vector Vector::operator-(const Vector& v) const
{
    return Vector(
          this->X - v.X,
          this->Y - v.Y,
          this->Z - v.Z
    );
}

Vector Vector::operator*(float c) const
{
	return Vector(
				  this->X*c,
				  this->Y*c,
				  this->Z*c
    );
}

Vector Vector::operator-() const
{
	return Vector(
        -(this->X),
        -(this->Y),
        -(this->Z)
    );
}

Vector& Vector::operator+=(const Vector& v)
{
    this->X += v.X;
    this->Y += v.Y;
    this->Z += v.Z;
	return *this;
}



Vector& Vector::normalize()
{
    float length = this->length();
    this->X /= length;
    this->Y /= length;
    this->Z /= length;
	return *this;
}

float Vector::length() const
{
	return sqrt(this->lengthSquared());
}

float Vector::lengthSquared() const
{
    return this->X * this->X + this->Y * this->Y + this->Z * this->Z;
}

Vector Vector::reflection( const Vector& normal) const
{
	Vector n = normal;
	n.normalize();
	
	return *this - (n * this->dot(n)) * 2.0f;
}

//helper function for float comparisson
bool cmpf(float A, float B, float epsilon = 0.000005f)
{
    return (fabs(A - B) <= epsilon);
}

bool Vector::triangleIntersection( const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
	
	// compute the normal of the plane
	Vector normal = (b-a).cross((c-a));
	
	// catch special cases
	if(normal.length() == 0){
		return false;
	}
	
	normal.normalize();
	
	// check if paralell
    if(cmpf(normal.dot(d), 0.0f)){
        return false;
    }
	
	// distance from plane to origin
	float distance = normal.dot(a);
	
	// calculate distance to intersection
	s = (distance - normal.dot(*this)) / normal.dot(d);
	
	// check sight connection to point of intersection
    if((s < 0 && !cmpf(s, 0.0f)))
        return false;
	
	// calculate point of intersection
	Vector hit = (*this) + d * s;

	// calculate the areas
	float areaABC = (b-a).cross(c-a).length() * 0.5f;
	float areaABP = (b-a).cross(hit-a).length() * 0.5f;
	float areaACP = (c-a).cross(hit-a).length() * 0.5f;
	float areaBCP = (c-b).cross(hit-b).length() * 0.5f;
	
	// compare partial areas to big area
	return areaABC >= areaABP + areaACP + areaBCP || cmpf(areaABC, (areaABP + areaACP + areaBCP));
}

bool Vector::yPlaneIntersection(const Vector &d, const Vector &n, float &s) const
{
    
    Vector normal = n;
        
    normal.normalize();
    
    // check if paralell
    if(cmpf(normal.dot(d), 0.0f)){
        return false;
    }
    
    // calculate distance to intersection
    s = -normal.dot(*this) / normal.dot(d);
    
//    // check sight connection to point of intersection
//    if((s < 0 && !cmpf(s, 0.0f)))
//        return false;
    
    return true;
}

std::string Vector::toString() const{
    std::stringstream ss;
    ss << "X: " << X << " ,Y: " << Y << ",Z: " << Z;
    return ss.str();
}
