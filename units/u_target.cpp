/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : u_target.cpp
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
#include "../utils/player.h"
//#include "../bin/music/"

#define NumOfTargets 30
#define NumOfBullets 250

/* Project namespace */
namespace ivgl
{
  /* Units namespace */
  namespace units
  {
    /* First unit test */
    class common_target : public unit  
    {
    public:
      ivgl::player P;

    private:
      FLT MouseX;
      FLT MouseY;
      FLT ScreenX, ScreenY;
      BOOL IsDebug;
      BOOL IsShot;
      
      /* Bullet struct array */
      //struct tagBULLET
      //{
      //  primitives Pr; /* Bullet as primitive */
      //  vec3 Dir;     /* Direction */
      //  vec3 Pos;     /* Bullet position */
      //} Bullets[256];

      /* Target coordinates class */
      class target
      {
      public:
        vec3 StartPosition;
        FLT RandomCoef;
        vec3 Position;
        vec3 Norm;
        matr Trans;
        BOOL IsCross = 0;
        BOOL IsShoot = 0;
        prim *Sphere; 

        target( VOID ) : Position(vec3()), Trans(matr()), RandomCoef(rand() / (FLT)RAND_MAX)
        {
          //StartPosition = vec3(490.9, -50.5, -495.867);
        }

       /* Evaluate normals function.
        * ARGUEMNTS:
        *   - target position:
        *       vec3 Pos;
        *   - rotate angle:
        *       FLT AngleInDegree;
        * RETURNS: None.
        */
       VOID EvalNormal( vec3 Pos, FLT AngleInDegree )
       {
         Trans = matr::RotateY(AngleInDegree);
         Trans = Trans * matr::Translate(Pos);

         Norm = vec3(sin(math::D2R(AngleInDegree)), 0, cos(math::D2R(AngleInDegree)));
         //Pos += StartPosition; 
         Position = Pos /* + StartPosition */;
         Pos[1] += 7;
         Sphere = ivgl::anim::GetPtr()->primitive_manager::PrimCreateSphere(Pos, 4.7, 20, 20);
       } /* End of 'EvalNormal' function */
      }; /* End of 'target_coords' class */
    target *Targets;
    primitives Prs;

    public:
      /* Constructor of test unit */
      common_target( anim *Ani )
      {
        P << "//bin//music//DeathFlash.wav";
        IsShot = FALSE;
        IsDebug = FALSE;
        ScreenX = Ani->FrameW / 2;
        ScreenY = Ani->FrameH / 2;

        Targets = new target[NumOfTargets];
        Prs.Load(&Prs, "bin/models/new_target.g3dm");

        for (INT i = 0; i < Prs.NumOfPrims; i++)
        {
          Prs.Prims[i].Mtl->shd = Ani->shader_manager::ShaderCreate("TARGET");
          Prs.Prims[i].Mtl->UpdateLoc();
        }

        for (INT i = 0; i < NumOfTargets; i++)
        {
          vec3 v(math::Rnd0F() * 300);

          //v = v * 200;
          if (i % 2 == 0)
            v = vec3(v[0], 0.9, v[2]);
          else
            v = vec3(-v[0], 0.9, v[2]);
          Targets[i].EvalNormal(v, 0);
        }
      } /* End of 'constructor' function */
      
      /* Destructor of test unit */
      ~common_target( VOID )
      {
        delete [] Targets;
      } /* End of 'destructor' function */
      
     /* Unit response function.
      * ARGUMENTS: 
      *   - Positioner to animation system:
      *       anim *Ani;
      * RETURNS: None.
      */
      VOID Response( anim *Ani ) override
      {
        static INT Cnt = 0;

        ScreenX = Ani->FrameW / 2;
        ScreenY = Ani->FrameH / 2;
        //SetCursorPos(ScreenX, ScreenY);
        MouseX = Ani->MouseX;
        MouseY = Ani->MouseY;

        std::string Buf;
        if (Ani->KeysClick['G'])
          IsDebug = !IsDebug;
 
        if (Ani->KeysClick['R'])
          for (INT i = 0; i < NumOfTargets; i++)
          {
            Targets[i].IsShoot = 0;
            Targets[i].EvalNormal(vec3(math::Rnd0F() * 300, 0.9, math::Rnd0F() * 300), 0);
          }

        if (Ani->KeysClick[VK_LBUTTON])
        {
          //P.SetLooping(ivgl::player::REPEAT_ALL);
          for (INT i = 0; i < NumOfTargets; i++)
            if (!Targets[i].IsShoot)
            {
              //vec3 P = Targets[i].Position;
              //vec3 N = Targets[i].Norm;
              //FLT D = -(P & N);
              Ani->Camera.SetRay(MouseX, MouseY);
              //math::ray<FLT> Ray = Ani->Camera.Ray;
              //FLT t = - (N[0] * Ray.Org[0] + N[1] * Ray.Org[1] + N[2] * Ray.Org[2] + D) / 
              //          (N[0] * Ray.Dir[0] + N[1] * Ray.Dir[1] + N[2] * Ray.Dir[2]);

              //vec3 V = Ray.Org + Ray.Dir * t;

              Targets[i].IsCross = FALSE;
              //if (V < (Targets[i].Position + 4.0) && V > (Targets[i].Position - 4.0))
              //  Targets[i].IsCross = TRUE;
              if (Ani->Camera.Ray.SphereInter(Targets[i].Position, 9))
                Targets[i].IsCross = TRUE;
              if (Targets[i].IsCross)
                Targets[i].IsShoot = TRUE;
              P.Play(0);

            }
          }
      } /* End of 'Response' function */

     /* Unit render function.
      * ARGUMENTS: 
      *   - Positioner to animation system:
      *       anim *Ani;
      * RETURNS: None.
      */
      VOID Render( anim *Ani ) override
      {
        for (INT i = 0; i < NumOfTargets; i++)
        {
          //Targets[i].Position[1] += 360.0f / i + Ani->Time * 40;
          //Targets[i].Sphere = Ani->primitive_manager::PrimCreateSphere(Targets[i].Position, 4.7, 20, 20);
          if (IsDebug)
            Ani->PrimDraw(Targets[i].Sphere, matr::Identity());
          if (!Targets[i].IsShoot)
          {
            FLT angle;
            if (Ani->Camera.Dir[2] < 0)
              angle = math::R2D(atan(Ani->Camera.Dir[0] / Ani->Camera.Dir[2]));
            else
              angle = 180 + math::R2D(atan(Ani->Camera.Dir[0] / Ani->Camera.Dir[2]));
            //Targets[i].EvalNormal(Targets[i].Position + vec3(sin(Ani->Time) * Targets[i].RandomCoef / 3 + Targets[i].RandomCoef, 0, sin(Ani->Time) * Targets[i].RandomCoef / 3 + Targets[i].RandomCoef) / 10 * Targets[i].RandomCoef, angle);
            Ani->PrimitivesDraw(&Prs, matr::Scale(vec3(0.6, 0.6, 0.1)) * Targets[i].Trans/* *
              matr::RotateY(360.0f / i + Ani->Time * 40) */);
          }
          else
            Ani->PrimitivesDraw(&Prs, matr::Scale(vec3(0.6, 0.6, 0.1)) * matr::RotateX(-90.0f) * Targets[i].Trans * matr::Translate(vec3(0, 1, -0)));
        }
      } /* End of 'Render' function */
    }; /* End of 'common_target' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

static ivgl::anim::unit_register<ivgl::units::common_target> _("TargetUnit");

/* END OF 'u_targets.cpp' FILE */

