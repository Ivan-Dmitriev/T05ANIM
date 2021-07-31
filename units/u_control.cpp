/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : u_control.cpp
 * PURPOSE     : Animation project.
 *               Units implementation module.
 *               Camera unit module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * NOTE        : Module namespace 'units'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "../src/anim/anim.h"

/* Project namespace */
namespace ivgl
{
  /* Units namespace */
  namespace units
  {
    /* First unit test */
    class control_unit : public unit  
    {
    public:
      /* Constructor of test unit */
      control_unit( anim *Ani )
      {
      } /* End of 'constructor' function */
      
     /* Unit response function.
      * ARGUMENTS: 
      *   - Pointer to animation system:
      *       anim *Ani;
      * RETURNS: None.
      */
      VOID Response( anim *Ani ) override
      {
        if (Ani->KeysClick['F'] && Ani->Keys[VK_MENU])
          Ani->FlipFullScreen();
        /* pause toggle */
        if (Ani->KeysClick['P'] && Ani->Keys[VK_MENU])
          Ani->IsPause = !Ani->IsPause;
        /* wireframe toggle */
        if (Ani->KeysClick['W'] && Ani->Keys[VK_MENU])
        {
          INT mode[2];

          glGetIntegerv(GL_POLYGON_MODE, mode);
          if (mode[0] == GL_FILL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
          else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (Ani->Keys['W'] || Ani->Keys['A'] || Ani->Keys['D'] || Ani->Keys['S'])
            Ani->Camera.Move((Ani->Camera.Dir * ((Ani->Keys['W'] - Ani->Keys['S'])) + 
                              Ani->Camera.Right * ((Ani->Keys['D'] - Ani->Keys['A']))) * 30 * Ani->GlobalDeltaTime);

        if (Ani->Keys[VK_CONTROL] || Ani->Keys[VK_SPACE]) 
          Ani->Camera.Move(vec3(0, 20 * Ani->GlobalDeltaTime, 0) * (Ani->Keys[VK_SPACE] - Ani->Keys[VK_CONTROL]));

        FLT Dist, Wp, Hp, sx, sy, sinT, cosT, cosP, sinP, plen, Azimuth, Elevator,
          ProjDist = Ani->Camera.ProjDist;
        vec3 dv, CamAt = Ani->Camera.At, CamLoc = Ani->Camera.Loc, 
          CamRight = Ani->Camera.Right, CamUp = Ani->Camera.Up, CamDir = Ani->Camera.Dir;
        /* Translate camera */
        if (Ani->Keys[VK_RBUTTON])
        {
          Wp = Ani->Camera.Size;
          Hp = Ani->Camera.Size;

          if (Ani->FrameW > Ani->FrameH)
            Wp *= (FLT)Ani->FrameW / Ani->FrameH;
          else
            Hp *= (FLT)Ani->FrameH / Ani->FrameW;

          Dist = !(CamAt - CamLoc);

          sx = -Ani->MouseDX * Wp / Ani->FrameW * Dist / ProjDist;
          sy = Ani->MouseDY * Hp / Ani->FrameH * Dist / ProjDist;

          dv = (CamRight * sx) + (CamUp * sy),
          CamAt = CamAt + dv;
          CamLoc = CamLoc + dv;
        }

        if (Ani->Keys['W'] || Ani->Keys['S'])
        {
          vec3 d = CamDir * (30 * Ani->GlobalDeltaTime * (Ani->Keys['W'] - Ani->Keys['S']));
          CamLoc = CamLoc + d;
          CamAt =  CamAt + d;
        }
        if (Ani->Keys['A'] || Ani->Keys['D'])
        {
          vec3 d = CamRight * (30 * Ani->GlobalDeltaTime * (Ani->Keys['D'] - Ani->Keys['A']));
          CamLoc = CamLoc + d;
          CamAt =  CamAt + d;
        }
        /* rotate camera */
        Dist = !(CamAt - CamLoc);

        cosT = (CamLoc[1] - CamAt[1]) / Dist;
        sinT = sqrt(1 - cosT * cosT);

        plen = Dist * sinT;
        cosP = (CamLoc[2] - CamAt[2]) / plen;
        sinP = (CamLoc[0] - CamAt[0]) / plen;

        Azimuth = math::R2D(atan2(sinP, cosP));
        Elevator = math::R2D(atan2(sinT, cosT));

        Azimuth += Ani->GlobalDeltaTime * 
          (-70.0 * Ani->Keys[VK_LBUTTON] * Ani->MouseDX +
            30.0 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));

        Elevator += Ani->GlobalDeltaTime * 
          (-70.0 * Ani->Keys[VK_LBUTTON] * Ani->MouseDY +
            30.0 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));

        Dist += Ani->GlobalDeltaTime * 
          (30.0 * Ani->MouseDZ +
            8.0 * (1 + Ani->Keys[VK_SHIFT] * 27) *
            (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]));

        if (Elevator < 1)
          Elevator = 1;
        else if (Elevator > 179)
          Elevator = 179;
        /* set camera */
        matr ResMatr = matr::RotateX(Elevator) * matr::RotateY(Azimuth);
        ResMatr = ResMatr * matr::Translate(CamAt);
        Ani->Camera.Loc[1] = 1;
                                          
        Ani->Camera.SetLocAtUp(ResMatr.TransformPoint(vec3(0, Dist, 0)), 
          CamAt, vec3(0, 1, 0));
      } /* End of 'Response' function */
       
     /* Unit render function.
      * ARGUMENTS: 
      *   - Pointer to animation system:
      *       anim *Ani;
      * RETURNS: None.
      */
      VOID Render( anim *Ani ) override
      {
      } /* End of 'Render' function */
    }; /* End of 'control_unit' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

static ivgl::anim::unit_register<ivgl::units::control_unit> _("ControlUnit");

/* END OF 'u_control.cpp' FILE */
