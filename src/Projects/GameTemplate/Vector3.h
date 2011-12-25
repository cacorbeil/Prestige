#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3
{
public:
   Vector3(){};
   Vector3(float X, float Y, float Z) {mX=X;mY=Y;mZ=Z;}
 
   inline const Vector3& operator+=( const Vector3& rhs );
   float mX,mY,mZ;
};
 
inline const Vector3 &Vector3::operator+=( const Vector3& rhs )
{
   mX += rhs.mX; mY += rhs.mY; mZ += rhs.mZ;
   return *this;
}
 
inline Vector3 operator+( const Vector3& lhs, const Vector3& rhs )
{
   return Vector3( lhs.mX + rhs.mX, lhs.mY + rhs.mY, lhs.mZ + rhs.mZ );
}
 
inline Vector3 operator*( const Vector3& lhs, const Vector3& rhs )
{
   return Vector3( lhs.mX * rhs.mX, lhs.mY * rhs.mY, lhs.mZ * rhs.mZ );
}
 
inline Vector3 operator*( const Vector3& lhs, float rhs )
{
   return Vector3( lhs.mX * rhs, lhs.mY * rhs, lhs.mZ * rhs);
}

#endif VECTOR3_H