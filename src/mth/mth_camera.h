/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME    : mth_camera.h
 * PURPOSE      : Animation project.
 *                Mathematics library.
 *                Space camera handle module.
 * PROGRAMMER   : CGSG-SummerCamp'2021.
 *                ID3.
 * LAST UPDATE  : 21.07.2021.
 * NOTE         : Module namespace 'mth'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_camera_h_
#define __mth_camera_h_

#include "mth_vec3.h"
#include "mth_matr.h"
#include "mth_ray.h"

/* Math library namespace */
namespace math
{
  //class matr;

  /* Space camera handle class */
  template<class type>
    class camera
    {
    private:
      /* Update camera project parameters function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UpdateProj( VOID )
      {
        type ratio_x = Size / 2, ratio_y = Size / 2;

        if (FrameW >= FrameH)
          ratio_x *= (type)FrameW / FrameH;
        else
          ratio_y *= (type)FrameH / FrameW;
        Proj = matr<type>::Frustum(-ratio_x, ratio_x, -ratio_y, ratio_y, ProjDist, FarClip);
      } /* End of 'UpdateProj' function */

      /* Update camera view parameters function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID UpdateView( VOID )
      {
        View = matr<type>::View(Loc, At, Up);
        /*
        CamRight = VecSet(View.M[0][0],
                          View.M[1][0],
                          View.M[2][0]);

        CamUp = VecSet(View.M[0][1],
                            View.M[1][1],
                            View.M[2][1]);

        CamDir = VecSet(-View.M[0][2],
                                -View.M[1][2],
                                -View.M[2][2]);
        Dir = vec3<type>(-) 
                */

      } /* End of 'UpdateView' function */

    public:
      vec3<type>
        Loc,            // camera location
        Dir, Up, Right, // basis camera directions
        At;             // camera pivot point
      type
        ProjDist,       // near project plane distance
        FarClip,        // far project plane distance
        Size;           // inner project plane rectangle size
      INT
        FrameW, FrameH; // Camera frame size
      matr<type>
        View,           // view matrix
        Proj,           // projection matrix
        VP;             // View and Proj madtrix production
      math::ray<FLT> Ray;

      /* Default constructor */
      camera( VOID ) :
        /*Loc(490.9, -50.5, -495.867)*/Loc(0, 10, 15), Dir(0, 0, -1), Up(0, 1, 0), Right(1, 0, 0), At(0, 0, 0),
        ProjDist(0.1), FarClip(800), Size(0.1),
        FrameW(30), FrameH(30)
      {
        UpdateProj();
        UpdateView();
        VP = View * Proj;
      } /* End of 'matr' function */

      /* Set project camera parameters function.
       * ARGUMENTS:
       *   - new near project plane size:
       *       type NewSize;
       *   - new project distance:
       *       type NewProjDist;
       *   - new far clip plane distance:
       *       type FarC;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & SetProj( type NewSize, type NewProjDist, type NewFarClip )
      {
        // Store new projection parameters
        ProjDist = NewProjDist;
        FarClip = NewFarClip;
        Size = NewSize;

        UpdateProj();
        VP = View * Proj;
        return *this;
      } /* End of 'SetProj' function */

      /* Set camera project pixel size function.
       * ARGUMENTS:
       *   - new frame size in pixels:
       *       type NewFrameW, NewFrameH;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & Resize( type NewFrameW, type NewFrameH )
      {
        FrameW = NewFrameW;
        FrameH = NewFrameH;

        UpdateProj();
        VP = View * Proj;
        return *this;
      } /* End of 'Resize' function */

      /* Set camera location and orientation function.
       * ARGUMENTS:
       *   - new camera location:
       *       const vec3<type> &L;
       *   - new camera pivot point:
       *       const vec3<type> &A;
       *   - new camera approx up direction:
       *       const vec3<type> &U;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & SetLocAtUp( const vec3<type> &L, const vec3<type> &A, const vec3<type> &U = vec3<type>(0, 1, 0) )
      {
        Loc = L;
        At = A;
        Up = U;

        Dir = (At - Loc).Normalizing();
        Right = (Dir % Up).Normalizing();

        UpdateView();

        VP = View * Proj;

        return *this;
      } /* End of 'SetLocAtUp' function */
 
      /* Camera rotate function.
       * ARGUMENTS:
       *   - rotate axis:
       *       const vec3<type> &Axis;
       *   - rotation angle (in degree):
       *       type AngleInDegree;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & Rotate( const vec3<type> &Axis, type AngleInDegree )
      {
        matr<type> m = matr<type>::Translate(-At) * matr<type>::Rotate(Axis, AngleInDegree) * matr<type>::Translate(At);

        Loc = m.TransformPoint(Loc);
        Up = m.TransformVector(Up);
        SetLocAtUp(Loc, At, Up);
        return *this;
      } /* End of 'Rotate' function */

      /* Camera movement function.
       * ARGUMENTS:
       *   - movement directions:
       *       const vec3<type> &Direction;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & MoveY0( vec3<type> Direction )
      {
        Loc += Direction;
        At += Direction;
        Loc[1] = 5;
        SetLocAtUp(Loc, At, Up);
        return *this;
      } /* End of 'Move' function */
      /* Camera movement function.
       * ARGUMENTS:
       *   - movement directions:
       *       const vec3<type> &Direction;
       * RETURNS:
       *   (camera &) self reference.
       */
      camera & Move( const vec3<type> &Direction )
      {
        Loc += Direction;
        At += Direction;
        SetLocAtUp(Loc, At, Up);
        return *this;
      } /* End of 'Move' function */

      /* Obtain ray from camera and projection plane function.
       * ARGUMENTS:
       *   - frame size in pixels:
       *       INT FrameW, FrameH;
       *   - frame pixel coordinates:
       *       Type FrameX, FrameY;
       * RETURNS: NONE.
       */
      VOID SetRay( FLT Sx, FLT Sy )
      {
        DBL Wp, Hp;

        Wp = Hp = Size;

       if (FrameW > FrameH)
          Wp *= (DBL)FrameW / FrameH;
        else
          Hp *= (DBL)FrameH / FrameW;

        vec3<type> A = Dir * ProjDist;
        vec3<type> B = Right * (Sx - FrameW / 2.0) * Wp / FrameW;
        vec3<type> C = Up * (-Sy + FrameH / 2.0) * Hp / FrameH;
        vec3<type> X = (A + B) + C;
        Ray.Org = Loc + X;
        Ray.Dir = X.Normalizing();
      } /* End of 'SetRay' function */

    }; /* End of 'camera' class */
} /* end of 'mth' namespace */

#endif /* __mth_camera_h_ */

/* END OF 'mth_camera.h' FILE */