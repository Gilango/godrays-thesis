//*******************************************************************
//
//   Vector.h
//
// 3D vector class in the namespace ash
//
//
//
//*******************************************************************

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>
#include <iostream>
using std::cout;
using std::endl;

using std::ostream;

namespace gRays
{

//! Vector is a 3D vector class
class Vector
{
  public:
   //! Name of class to be used in Lua
   //static const char *className;

   //! Default constructor initializes components to 0
   Vector() { xyz[0] = xyz[1] = xyz[2] = 0; /*className = "Vector";*/ }

   //! Copy constructor
   Vector(const Vector& v)
   { 
      xyz[0] = v.xyz[0];
      xyz[1] = v.xyz[1];
      xyz[2] = v.xyz[2]; 
      //className = v.className;
   }
   
   //! Initialize all three components explicitly
   Vector(const double a, const double b, const double c)
   {
      xyz[0] = a;
      xyz[1] = b;
      xyz[2] = c; 
      //className = "Vector";
   }

   ~Vector(){}

   //!  Set all three components
   void set( const double vx, const double vy, const double vz )
   {
      xyz[0] = vx;
      xyz[1] = vy;
      xyz[2] = vz;
   }
   

   friend ostream& operator<< (ostream& output, const Vector& v)
   {
	   output<<v[0] <<" "<<v[1] <<" "<<v[2] <<" "<<endl;
	   return output;
   }

   //! Add two vectors together
   const Vector operator+        (const Vector& v) const 
   { 
      return Vector(xyz[0]+v.xyz[0], xyz[1]+v.xyz[1], xyz[2]+v.xyz[2]); 
   }
  
   //! Subtract one vector from another
   const Vector operator-        (const Vector& v) const
   { 
      return Vector(xyz[0]-v.xyz[0], xyz[1]-v.xyz[1], xyz[2]-v.xyz[2]); 
   }

   //! Unary minus
   friend const Vector operator- (const Vector& v)
   { return Vector(-v.xyz[0],-v.xyz[1],-v.xyz[2]); }

   //! Multiplication of a constant with a vector
   friend const Vector operator* (const double w, const Vector& v)
   { return v*w; }
	  
   //! Multiplication of a vector with a constant
   const Vector operator*        (const double v) const
   { return Vector(xyz[0]*v, xyz[1]*v, xyz[2]*v); }

   //! Division of a vector by a constant
   const Vector operator/        (const double v) const
   { return Vector(xyz[0]/v, xyz[1]/v, xyz[2]/v); }

   //! Inner product
   const double operator*        (const Vector& v) const  
   { return (xyz[0]*v.xyz[0] + xyz[1]*v.xyz[1] + xyz[2]*v.xyz[2]); }
  
   //! Cross product
   const Vector operator^        (const Vector& v) const 
   { return Vector(xyz[1]*v.xyz[2] - xyz[2]*v.xyz[1], 
		   xyz[2]*v.xyz[0] - xyz[0]*v.xyz[2], 
		   xyz[0]*v.xyz[1] - xyz[1]*v.xyz[0]); }

   //! Assign vector value from another vector's values
   Vector& operator=       (const Vector& v)
   { xyz[0] = v.xyz[0]; xyz[1] = v.xyz[1]; xyz[2] = v.xyz[2]; return *this; }
  
   //! In place component-wise addition
   Vector& operator+=      (const Vector& v)
   { xyz[0] += v.xyz[0]; xyz[1] += v.xyz[1]; xyz[2] += v.xyz[2]; return *this; }
  
   //! In place component-wise subtraction
   Vector& operator-=      (const Vector& v)
   { xyz[0] -= v.xyz[0]; xyz[1] -= v.xyz[1]; xyz[2] -= v.xyz[2]; return *this; }
  
   //! In place component-wise multiplication by a constant
   Vector& operator*=      (const double v)
   { xyz[0] *= v; xyz[1] *= v; xyz[2] *= v; return *this; }
  
   //! In place component-wise division by a constant
   Vector& operator/=      (const double v)
   { xyz[0] /= v; xyz[1] /= v; xyz[2] /= v; return *this; }
  
   //! boolean equal
   bool operator==         (const Vector& v)
   { return ((xyz[0] == v.xyz[0]) && (xyz[1] == v.xyz[1]) && (xyz[2] == v.xyz[2])); }
   
   //! boolean not equal
   bool operator!=         (const Vector& v)
   { return ((xyz[0] != v.xyz[0]) || (xyz[1] != v.xyz[1]) || (xyz[2] != v.xyz[2])); }

   //! Read-only access to components by index
   const double& operator[] (const int v) const { return xyz[v]; }

   //! Read-write access to components by index
         double& operator[] (const int v)       { return xyz[v]; }

   //! Read-only access to components by index with ()
   const double& operator() (const int v) const { return xyz[v]; }

   //! Read-only access to X component
   const double X() const { return xyz[0]; }

   //! Read-only access to Y component
   const double Y() const { return xyz[1]; }

   //! Read-only access to Z component
   const double Z() const { return xyz[2]; }

   //! Calculate and return the magnitude of this vector
   const double magnitude() const 
   { return std::sqrt( xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2] ); }
  
   //! Calculate and return a unit length vector in the direction of this vector
   const Vector unitvector() const { return *this/magnitude(); }

   //! Normalize this vector in place
   void normalize() 
   {
       double mag = magnitude(); xyz[0] /= mag; xyz[1] /= mag; xyz[2] /= mag; 
   }

//  Comparisons

   //! Equality test of this vector with another, component-wise
   const bool operator==         (const Vector& v) const
       { return ( xyz[0]==v.xyz[0] && xyz[1]==v.xyz[1] && xyz[2]==v.xyz[2] ); }
  
   //! Inequality test of this vector with another, component-wise
   const bool operator!=         (const Vector& v) const
       { return ( xyz[0]!=v.xyz[0] || xyz[1]!=v.xyz[1] || xyz[2]!=v.xyz[2] ); }
  
   //! Compare the length of this vector with another
   const bool operator<          (const Vector& v) const
       { return ( magnitude() < v.magnitude() ); }
  
   //! Compare the length of this vector with another
   const bool operator<=         (const Vector& v) const
       { return ( magnitude() <= v.magnitude() ); }
  
   //! Compare the length of this vector with another
   const bool operator>          (const Vector& v) const
       { return ( magnitude() > v.magnitude() ); }
  
   //! Compare the relative length of this vector with another
   const bool operator>=         (const Vector& v) const
       { return ( magnitude() >= v.magnitude() ); }

   //! Determine whether two vectors are parallel
   const bool operator||         (const Vector& v) const
       { return (  std::fabs((*this)*v) == v.magnitude()*((*this).magnitude()) ); }
   
  private:
  double xyz[3];
};

class Vector4{
public:        
    Vector4(){
        xyzw[0] = xyzw[1] = xyzw[2] = xyzw[3] = 0; 
    }
    
    //! Copy constructor
   Vector4(const Vector4& v)
   { 
      xyzw[0] = v.xyzw[0];
      xyzw[1] = v.xyzw[1];
      xyzw[2] = v.xyzw[2]; 
      xyzw[3] = v.xyzw[3]; 
   }
        
   // cast 3D vector to 4D vector
   Vector4(const Vector& v)
   { 
      xyzw[0] = v.X();
      xyzw[1] = v.Y();
      xyzw[2] = v.Z();
      xyzw[3] = 1;
   }
   
   //! Initialize all four components explicitly
   Vector4(const double a, const double b, const double c, const double d)
   {
      xyzw[0] = a;
      xyzw[1] = b;
      xyzw[2] = c; 
      xyzw[2] = d; 
   }
   
      //! Read-only access to components by index
   const double& operator[] (const int v) const { return xyzw[v]; }

   //! Read-write access to components by index
   double& operator[] (const int v)       { return xyzw[v]; }
         
   
      friend ostream& operator<< (ostream& output, const Vector4& v)
   {
	   output<<v[0] <<" "<<v[1] <<" "<<v[2] <<" "<< v[3] <<endl;
	   return output;
   }
   
private:
    double xyzw[4];
      
};

/*
ostream& operator<< (ostream& output, const Vector& v)
{
	   output<<(float)v[0] <<" "<<(float)v[1] <<" "<<(float)v[2] <<" "<<endl;
	   return output;
}*/
class Vector2 {
  public:
    Vector2() { xy[0] = xy[1] = 0; }
    
    Vector2(const double a, const double b) 
    {
      xy[0] = a;
      xy[1] = b;
    }

    Vector2(const Vector2& v) 
    {
      xy[0] = v.xy[0];
      xy[1] = v.xy[1];
    }

   void set( const double a, const double b )
   {
     xy[0] = a;
     xy[1] = b;
   }

   //! Read-only access to components by index
   const double& operator[] (const int v) const { return xy[v]; }

   //! Read-write access to components by index
         double& operator[] (const int v)       { return xy[v]; }

   const Vector2 operator+        (const Vector2& v) const 
   { 
      return Vector2(xy[0]+v.xy[0], xy[1]+v.xy[1]); 
   }
  

   //! Multiplication of a constant with a vector
   friend const Vector2 operator* (const double w, const Vector2& v)
   { return v*w; }
	  
   //! Multiplication of a vector with a constant
   const Vector2 operator*        (const double v) const
   { return Vector2(xy[0]*v, xy[1]*v); }

   //! Read-only access to X component
   const double X() const { return xy[0]; }

   //! Read-only access to Y component
   const double Y() const { return xy[1]; }

   friend ostream& operator<< (ostream& output, const Vector2& v)
   {
	   output<<v[0] <<" "<<v[1] <<" "<<endl;
	   return output;
   }

  private:
    double xy[2];
};


}



#endif


/*! 
    \page commits Commits
    \section Vector Vector.h
    $Id: Vector.h 490 2012-06-14 21:04:31Z jtessen $
    $Revision: 490 $
    $Date: 2012-06-14 17:04:31 -0400 (Thu, 14 Jun 2012) $
    $Author: jtessen $
    $HeadURL: https://gilango@svn.cs.clemson.edu/repos/ash/include/Vector.h $
 */

