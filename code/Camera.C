
#include "Camera.h"
using namespace gRays;

Camera::Camera()
{
   setEyeViewUp( Vector(0,0,1), Vector(0,0,-1), Vector(0,1,0) );
   setFov( 60.0 );
   setAspectRatio( 16.0/9.0 );
   setNearPlane(0.0);
   setFarPlane(1.0e6);
}

void Camera::setEyeViewUp( const Vector& eye, const Vector& view, const Vector& up )
{
   position = eye;
   axis_view = view.unitvector();
   axis_up = ( up - (up*axis_view) * axis_view ).unitvector();
   axis_right = (axis_view^axis_up).unitvector();
}

const Vector Camera::view( const double x, const double y ) const
{
   double xx = (2.0*x-1.0)*htanfov;
   double yy = (2.0*y-1.0)*vtanfov;
   return (axis_up * yy + axis_right * xx + axis_view).unitvector();
}

void Camera::setFov( const double fov )
{
   FOV = fov;
   htanfov = tan( FOV*0.5*M_PI/180.0 );
   vtanfov = htanfov/aspect_ratio;
}

void Camera::setAspectRatio( const double ar )
{
   aspect_ratio = ar;
   vtanfov = htanfov/aspect_ratio;
}


bool Camera::viewPlaneFromSpace( const Vector& P, double* x, double* y ) const
{
    Vector dP = P - position;
    double ndP = dP * axis_view;


    if (ndP <= 0.0 ){ return false; }
    Vector X = dP/ndP - axis_view;
   
    *x= (( X*axis_right/htanfov ) + 1.0)/2.0; 
    
    *y = (( X*axis_up/vtanfov ) + 1.0)/2.0;
     
    return ( *x>=0 && *x<=1 && *y>=0 && *y<=1 );
}
