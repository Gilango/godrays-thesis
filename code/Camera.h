
#ifndef __CAMERA_H__
#define __CAMERA_H__


#include "Vector.h"

namespace gRays
{

//! Very basic camera class
class Camera
{

  public:

    //! Default constructor positions & orients camera with default settings
    /*!
     *  The default settings are: <br>
     *     eye          = (0,0,1) <br>
     *     view         = (0,0,-1) <br>
     *     up           = (0,1,0) <br>
     *     fov          = 60 degrees <br>
     *     aspect ratio = 16/9 <br>
     *     near plane   = 0 <br>
     *     far plane    = 10^6
     */
    Camera();
   ~Camera(){}

    //! Set the position and orientation of the camera.
    /*!
     *  The direction vectors view and up do not have to be unit <br> 
     *  vectors.  If view and up are not orthogonal, the view vector <br>
     *  is left intact (but normalized), and the up vector is  <br>
     *  made perpendicular to the view vector, then normalized. 
					*
					*  @param eye New camera position in 3D space
					*  @param view New camera direction (through center of image)
					*  @param up New "up" direction for camera, indicating which direction is up (for camera roll)
					*
     */
    void setEyeViewUp( const Vector& eye, const Vector& view, const Vector& up );

    //! Retrieved the camera position
    const Vector& eye() const  { return position; }

    //! Retrieves the direction of the center of the camera view
    const Vector& view() const { return axis_view; }

    //! Retrieves the direction that is right in the image plane
    const Vector& right() const { return axis_right; }

    //! Retrieves the direction that is up in the image plane
    const Vector& up() const   { return axis_up; }

    //! Generates a unit vector goiing through the indicated position of the image plane
    const Vector view( const double x, const double y ) const;

    //! Set the horizontal field of view, in degrees
    void setFov( const double fov );

    //! Retrieve the horizontal field of view, in degrees
    const double& fov() const { return FOV; }

    //! Set the aspect ratio for the image plane 
    void setAspectRatio( const double ar );

    //! Retrieve the aspect ratio
    const double& aspectRatio() const { return aspect_ratio; }

    //! Set the near plane distance
    void setNearPlane( const double n ){ near = n; }

    //! Retrieve the near plane distance
    const double& nearPlane() const { return near; }

    //! Set the far plane distance
    void setFarPlane( const double n ){ far = n; }

    //! Retrieve the far plane distance
    const double& farPlane() const { return far; }

    //! Returns the position on the view plane corresponding to the input position in 3D space
    bool viewPlaneFromSpace( const Vector& P, double* x, double* y ) const;


  private:
    
    double FOV, aspect_ratio;
    double htanfov, vtanfov;
    double near, far;

    Vector position;
    Vector axis_right, axis_up, axis_view;
};


}

#endif


/*! 
    \page commits Commits
    \section Camera Camera.h
    $Id: Camera.h 509 2012-12-04 22:37:12Z pkilgo $
    $Revision: 509 $
    $Date: 2012-12-04 17:37:12 -0500 (Tue, 04 Dec 2012) $
    $Author: pkilgo $
    $HeadURL: https://gilango@svn.cs.clemson.edu/repos/ash/include/Camera.h $
 */

