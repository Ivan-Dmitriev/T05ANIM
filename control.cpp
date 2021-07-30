 /*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : control.cpp
 * PURPOSE     : Animation project.
 *               Units collection.
 *               Control interface scene.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Daniil Smirnov.
 * LAST UPDATE : 26.07.2021.
 * NOTE        : Module namespace 'bogl::unit'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "../../animation/animation.h"
#include "../../bogl.h"

namespace bogl
{ 
  class cam_control : public unit
  {
  public:
    cam_control( animation *Ani )
    {
    }
    VOID Response( animation *Ani ) override
    {
      if (Ani->Keys['W'] || Ani->Keys['A'] || Ani->Keys['S'] || Ani->Keys['D'])
        Ani->Camera.Move((vec3(Ani->Camera.Dir[0], 0, Ani->Camera.Dir[2]) * ((Ani->Keys['W'] - Ani->Keys['S'])) + 
                          vec3(Ani->Camera.Right[0], 0, Ani->Camera.Right[2]) * ((Ani->Keys['D'] - Ani->Keys['A']))) * 5 * Ani->GlobalDeltaTime);

      if (Ani->Keys[VK_CONTROL] || Ani->Keys[VK_SPACE])
        Ani->Camera.Move(vec3(0, 5 * Ani->GlobalDeltaTime, 0) * (Ani->Keys[VK_SPACE] - Ani->Keys[VK_CONTROL]));

      //if (Ani->Keys[VK_RBUTTON])
      //{
      //  FLT Hp, Wp = Hp = Ani->Camera.ProjDist;
      //
      //  if (Ani->Camera.FrameW > Ani->Camera.FrameH)
      //    Wp *= static_cast<FLT>(Ani->Camera.FrameW) / Ani->Camera.FrameH;
      //  else
      //    Hp *= static_cast<FLT>(Ani->Camera.FrameH) / Ani->Camera.FrameW;
      //
      //  FLT Dist = !(Ani->Camera.At - Ani->Camera.Loc);
      //
      //  FLT sx = -Ani->MouseDx * Wp / Ani->Camera.FrameW * Dist / Ani->Camera.ProjDist;
      //  FLT sy = Ani->MouseDy * Hp / Ani->Camera.FrameH * Dist / Ani->Camera.ProjDist;
      //
      //  Ani->Camera.Move(Ani->Camera.Right * sx +
      //                    Ani->Camera.Up * sy);
      //}


      if (Ani->Keys[VK_LBUTTON] || Ani->Keys[VK_LEFT] || Ani->Keys[VK_RIGHT])
        Ani->Camera.Rotate(vec3(0, 1, 0), (-Ani->Keys[VK_LBUTTON] * Ani->MouseDx +
                                          (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime * 130);

      if (Ani->Keys[VK_LBUTTON] || Ani->Keys[VK_UP] || Ani->Keys[VK_DOWN])
        Ani->Camera.Rotate(Ani->Camera.Right, (-Ani->Keys[VK_LBUTTON] * Ani->MouseDy +
                                               (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime * 130);
    }
  };
  static bogl::animation::unit_register<cam_control> _("Camera");
  class control : public scene
  {
  public:
    control( animation *Ani )
    {
      *this << "Camera";
    }
  };
  static bogl::animation::unit_register<control> __("Ctrl");
}

/* END OF 'control.cpp' FILE */