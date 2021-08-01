/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : u_final.cpp
 * PURPOSE     : Animation project.
 *               Units implementation module.
 *               Test unit module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * NOTE        : Module namespace 'units'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <string>

#include "../src/anim/anim.h"

#define NUM_OF_TARGETS 20

/* Project namespace */
namespace ivgl
{
  /* Units namespace */
  namespace units
  {
    /* First unit test */
    class final_unit : public unit  
    {
    public:
      //matr HandsRotate = matr::Translate(vec3(-0.5, -0.3, 0.1)) * matr::RotateY(90) ; // Rotate hands matrix
      //matr HandsPos = matr::Identity();                                               // Position hands matrix

      prim * Cube;
      prim * BigCube;
      primitives Gun;
      primitives Target[NUM_OF_TARGETS];
      primitives FirstTarget;
      prim * Sphere;
      prim * Boxes;
      FLT MouseX = 0;
      FLT MouseY = 0;
      FLT MouseZ = 0;
      vec3 SavedMax;
      vec3 SavedMin;
      BOOL IsAim = FALSE;
      BOOL IsShoot = FALSE;
      BOOL IsJump = FALSE;
      BOOL IsLocked = TRUE;
      INT IsKilled[NUM_OF_TARGETS];
      vec3 Tar_positions[NUM_OF_TARGETS];

      /* Constructor of test unit */
      final_unit( anim *Ani )
      {
        //for (INT i = 0; i < NUM_OF_TARGETS; i++)
        //  Tar_positions[i] = vec3(math::Rnd1F() * 100, 5, math::Rnd1F() * 100);

        //Ani->FlipFullScreen();
        //Ani->Camera.Resize(Ani->FrameW, Ani->FrameH);

        //Cube = Ani->primitive_manager::PrimCreateBox(vec3(5, 10, 5), vec3(-5, -10, -5));
        BigCube = Ani->primitive_manager::PrimCreateBox(vec3(320, 1, 320), vec3(-320, -1, -320));
        //BigCube->Mtl->Tex[0] = nullptr;
        Sphere = Ani->primitive_manager::PrimCreateSphere(vec3(0, 0, 0), 10, 20, 20);
        vec3 Ka(0.10588, 0.058824, 0.113725);
        vec3 Kd(0.427451, 0.470588, 0.541176);
        vec3 Ks(0.427451, 0.470588, 0.541176);
        FLT Ph = 9.84615;
        std::string name = "peweter";

        Gun.Load(&Gun, "bin/models/arms_new.g3dm");
        //FirstTarget.Load(&FirstTarget, "bin/models/target.g3dm");
        //for (INT i = 0; i < FirstTarget.NumOfPrims; i++)
        //{
        //  FirstTarget.Prims[i].Mtl->shd = Ani->shader_manager::ShaderCreate("TARGET");
        //  FirstTarget.Prims[i].Mtl->UpdateLoc();
          //Boxes = Ani->primitive_manager::PrimCreateBox(Target.Prims[0].Min, Target.Prims[0].Max);
        //}
        //for (INT i = 0; i < NUM_OF_TARGETS; i++)
        //  Target[i] = FirstTarget;

        for (INT i = 0; i < Gun.NumOfPrims; i++)
        {
          Gun.Prims[i].Mtl->shd = Ani->shader_manager::ShaderCreate("TARGET");
          Gun.Prims[i].Mtl->UpdateLoc();
        }
        BigCube->Mtl->Tex[0] = Ani->TexCreate("grass.g24");
        //SavedMax = Target.Prims[0].Max;
        //SavedMin = Target.Prims[0].Min;
        //Sphere->Mtl = Ani->material_manager::MtlCreate(Ka, Kd, Ks, Ph, (FLT)1.0, name);
        //Sphere->Mtl->Tex[0] = Ani->texture_manager::TexCreate("GIR.G32");
      } /* End of 'constructor' function */

      /* Destructor of test unit */
      ~final_unit( VOID )
      {
        //Cube->Free();
        Sphere->Free();
      } /* End of 'destructor' function */

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

        IsLocked = TRUE;

        MouseX = Ani->MouseX;
        MouseY = Ani->MouseY;
        MouseZ = Ani->MouseZ;
        if (Ani->Keys['W'] || Ani->Keys['A'] || Ani->Keys['S'] || Ani->Keys['D'])
              Ani->Camera.Move((((Ani->Camera.Dir * ((Ani->Keys['W'] - Ani->Keys['S'])) +
                             Ani->Camera.Right * ((Ani->Keys['D'] - Ani->Keys['A']))) * 30 * Ani->GlobalDeltaTime)));
        //if (Ani->Keys['W'] || Ani->Keys['A'] || Ani->Keys['S'] || Ani->Keys['D'])
        //      Ani->Camera.Move(((Ani->Camera.Dir * ((Ani->Keys['W'] - Ani->Keys['S'])) +
        //                     Ani->Camera.Right * ((Ani->Keys['D'] - Ani->Keys['A']))) * 30 * Ani->GlobalDeltaTime).GetVecInPlaneXZ());
        //if (Ani->Keys['Q'] || Ani->Keys['E'])
        //  Ani->Camera.Rotate(vec3(0, 1, 0), (-Ani->Keys['Q'] + Ani->Keys['E']) * -30 * Ani->GlobalDeltaTime);
        if (Ani->Keys[VK_CONTROL] || Ani->Keys[VK_SPACE])
          Ani->Camera.Move(vec3(0, 20 * Ani->GlobalDeltaTime, 0) * (Ani->Keys[VK_SPACE] - Ani->Keys[VK_CONTROL]));

        //if (Ani->Camera.Dir[1] < 0.95 && Ani->Camera.Dir[1] > -0.95)
          if (Ani->Keys[VK_LBUTTON] || Ani->Keys[VK_LEFT] || Ani->Keys[VK_RIGHT])
            Ani->Camera.Rotate(vec3(0, 1, 0), (-Ani->Keys[VK_LBUTTON] * Ani->MouseDX * 0.1 +
                                               (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime * 130);

        //if (Ani->Camera.Dir[1] < 0.95 && Ani->Camera.Dir[1] > -0.95)
          if (Ani->Keys[VK_LBUTTON] || Ani->Keys[VK_UP] || Ani->Keys[VK_DOWN])
            Ani->Camera.Rotate(Ani->Camera.Right, (-Ani->Keys[VK_LBUTTON] * Ani->MouseDY * 0.1 +
                                                   (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime * 130);

        {
          Ani->Camera.Rotate(math::vec3<FLT>(0, 1, 0), (15 * -1 * Ani->MouseDX +
                                                100 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
          //HandsRotate *= matr::Rotate(-math::vec3<FLT>(0, 1, 0), (100 * 1 * Ani->MouseDX +
          //                                      -30 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
        }
        if (Ani->Camera.Dir[1] < 0.95)
        {
          Ani->Camera.Rotate(Ani->Camera.Right, (15 * -1 * Ani->MouseDY +
                                                  100 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
        }
        //if (Ani->Keys[VK_LBUTTON] && Ani->Camera.Dir.Y < 0.95 && Ani->MouseDY <= 0)
        //if (Ani->Camera.Dir[1] < 0.95)
        //{
          //Ani->Camera.Rotate(Ani->Camera.Right, (100 * -1 * Ani->MouseDY +
          //                                        100 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
          //HandsRotate *= matr::Rotate(-Ani->Camera.Right, (100 * 1 * Ani->MouseDY +
          //                                        -30 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
        //}

        //if (Ani->Keys[VK_LBUTTON] && Ani->Camera.Dir[1] > -0.95 && Ani->MouseDY >= 0)
          //if (Ani->Camera.Dir[1] > -0.95)
          //{
            //Ani->Camera.Rotate(Ani->Camera.Right, (100 * -1 * Ani->MouseDY +
            //                                        100 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
            //HandsRotate *= matr::Rotate(-Ani->Camera.Right, (100 * 1 * Ani->MouseDY +
            //                                        -30 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
          //}
        //vec3 Pos = vec3(Ani->Camera.Loc[0], Ani->Camera.Loc[1], Ani->Camera.Loc[2]);
        //HandsPos = matr::Translate(Pos);
        if (Ani->Keys[VK_RBUTTON] || Ani->Keys[VK_RCONTROL])
        {
          if (!IsAim)
          {
            static FLT AimTime;
            AimTime += Ani->GlobalDeltaTime;
            Ani->Camera.SetProj(0.1, 0.1 + 0.3 * AimTime, 500);
            //HandsRotate = matr::Translate(vec3(6 * Ani->GlobalDeltaTime, 1 * Ani->GlobalDeltaTime, -10 * Ani->GlobalDeltaTime)) * HandsRotate;
            if (AimTime > 0.1)
            {
              IsAim = 1;
              AimTime = 0;
            }
          }
        }
        else if ((!Ani->Keys[VK_RBUTTON]) || (!Ani->Keys[VK_RCONTROL]))
        {
          if (IsAim)
          {
            static FLT AimTime;
            AimTime += Ani->GlobalDeltaTime;
            Ani->Camera.SetProj(0.1, 0.1 - 0.3 * AimTime, 500);
            //HandsRotate = matr::Translate(vec3(-6 * Ani->GlobalDeltaTime, -1 * Ani->GlobalDeltaTime, 10 * Ani->GlobalDeltaTime)) * HandsRotate;
            if (AimTime > 0.1)
            {
              IsAim = 0;
              AimTime = 0;
            }
          }
        }
        if (Ani->KeysClick['L'])
          IsLocked = !IsLocked;

      } /* End of 'Response' function */

     /* Unit render function.
      * ARGUMENTS: 
      *   - Pointer to animation system:
      *       anim *Ani;
      * RETURNS: None.
      */
      VOID Render( anim *Ani ) override
      {
        matr position 
        (
          Ani->Camera.Dir[0], Ani->Camera.Dir[1], Ani->Camera.Dir[2], 0,
          Ani->Camera.Up[0], Ani->Camera.Up[1], Ani->Camera.Up[2], 0,
          Ani->Camera.Right[0], Ani->Camera.Right[1], Ani->Camera.Right[2], 0,
          Ani->Camera.Loc[0], Ani->Camera.Loc[1], Ani->Camera.Loc[2], 1
        );
        //CHAR Buf[100];
 
        //sprintf(Buf, "%.3f\n", Ani->FPS);
        //OutputDebugStringA(Buf);

        //vec3 Weapon;
        //Weapon[0] = Ani->Camera.Loc[0];
        //Weapon[1] = Ani->Camera.Loc[1] - 0.3;
        //Weapon[2] = Ani->Camera.Loc[2] + 0.01;

        //INT Flag[5];
        //memset(Flag, 0, sizeof(INT));

        //Ani->Camera.SetRay(MouseX, MouseY);
        //vec3 difference = vec3(MouseX, MouseY, MouseZ) - Ani->Camera.Loc;
        //difference.Normalize();

        //FLT lookAngle = math::R2D(atan2(Ani->Camera.Dir[1], Ani->Camera.Dir[0]));
        //FLT lookAngle = math::R2D(atan2(difference[1], difference[0]));

        //if (Ani->KeysClick[VK_LBUTTON])
        //{
        //  for (INT i = 0; i < 5; i++)
        //  {
        //    //vec3 Target_pos = vec3(math::Rnd1F() * 10, 5, math::Rnd1F() * 10);
        //    //Target.Prims[0].Min = 
        //    if (Ani->Camera.Ray.BoxInter(
        //      Ani->Camera.Ray, vec3(Target[i].Prims[0].Min[0] + Tar_positions[i][0], 5, Target[i].Prims[0].Min[2] + Tar_positions[i][2]),
        //                       vec3(Target[i].Prims[0].Max[0] + Tar_positions[i][0], 5, Target[i].Prims[0].Max[2] + Tar_positions[i][2])))
        //    {
        //      //Ani->PrimitivesDraw(&Target[i], matr::Translate(vec3(Tar_positions[i][0], 0, Tar_positions[i][2])));
        //      IsKilled[i] = TRUE;
        //    }
        //  }
        //}
        //for (INT i = 0; i < NUM_OF_TARGETS; i++)
        //{
        //  if (IsKilled[i] != TRUE)
        //    Ani->PrimitivesDraw(&Target[i], matr::Translate(vec3(Tar_positions[i][0], 0, Tar_positions[i][2])));
        //  else
        //    Ani->PrimitivesDraw(&Target[i], matr::Translate(vec3(Tar_positions[i][0], 0, Tar_positions[i][2])) * matr::RotateX(90.f));

        //}

            //Flag[i] = 1;

          //if (Flag[i] && (!Ani->Camera.Ray.BoxInter(Ani->Camera.Ray, vec3(-5 + 30 * i, -10, -5), vec3(5 + 30 * i, 10, 5))))
          //  Ani->PrimDraw(Cube, matr::Translate(vec3(30 * i, 0, 0)));
          //Target.Prims[0].Max[0] += 30.0 * i;
          //Target.Prims[0].Min[0] += 30.0 * i;

          //if (Flag[i] && (!Ani->Camera.Ray.BoxInter(Ani->Camera.Ray, Target.Prims[0].Min, Target.Prims[0].Max)))
          //  Ani->PrimitivesDraw(&Target, matr::Translate(vec3(30 * i, 0, 0)));
          //Cube = Ani->primitive_manager::PrimCreateBox(Target.Prims[0].Min, Target.Prims[0].Max);
          //Ani->PrimDraw(Cube, matr::Identity());
          //if (Flag[i] && (!Ani->Camera.Ray.SphereInter(vec3(0 + 30 * i, 0, 0), 10)))
        //Target.Prims[0].Max = SavedMax;
        //Target.Prims[0].Min = SavedMin;
        //Ani->PrimitivesDraw(&Gun, matr::Scale(vec3(0.01)) * matr::RotateY(180) * matr::RotateZ(-45) * matr::Translate(Weapon));
        //for (INT i = 0; i < 1; i++)
        //{
        //  Ani->PrimDraw(Boxes, matr::Identity());
        //}
        //Ani->PrimitivesDraw(&Gun, matr::Identity());
        Ani->PrimDraw(BigCube, matr::Identity());
        //Ani->PrimitivesDraw(&Gun, /* matr::RotateY(210.0f) **/ matr::RotateX(90.0f) * matr::RotateZ(90.0f) * position);
        //Ani->PrimitivesDraw(&Gun, matr::RotateY(180.0f) * matr::RotateZ(-35.0f) *
        //                      matr::RotateX(-20.0f) * matr::RotateY(lookAngle - 20.0f) *
        //                     matr::Translate(Weapon));
        //Ani->PrimitivesDraw(&Gun, matr::Scale(vec3(0.1)) * HandsRotate * HandsPos);
        //if (IsLocked)
        Ani->PrimitivesDraw(&Gun, matr::RotateY(90.0f) * matr::Translate(vec3(0, -0.3, 0.3))
                             * matr::RotateZ(5.0f) * position);
        //else
        //Ani->PrimitivesDraw(&Gun, matr::Identity());

        //Ani->PrimDraw(Pr, matr().Identity() /* matr().RotateZ(sin(4 * Ani->Time) * 20) */ );
      } /* End of 'Render' function */
    }; /* End of 'final_unit' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

static ivgl::anim::unit_register<ivgl::units::final_unit> _("FinalUnit");

/* END OF 'u_final.cpp' FILE */

