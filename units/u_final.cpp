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
      prim * Cube;
      primitives Prs;
      primitives Target;
      prim * Sphere;
      prim * Boxes;
      /* Constructor of test unit */
      final_unit( anim *Ani )
      {
        Cube = Ani->primitive_manager::PrimCreateBox(vec3(5, 10, 5), vec3(-5, -10, -5));
        Sphere = Ani->primitive_manager::PrimCreateSphere(vec3(0, 0, 0), 10, 20, 20);
        vec3 Ka(0.10588, 0.058824, 0.113725);
        vec3 Kd(0.427451, 0.470588, 0.541176);
        vec3 Ks(0.427451, 0.470588, 0.541176);
        FLT Ph = 9.84615;
        std::string name = "peweter";

        Prs.Load(&Prs, "bin/models/m4a1.g3dm");
        Target.Load(&Target, "bin/models/target.g3dm");
        for (INT i = 0; i < Target.NumOfPrims; i++)
        {
          Target.Prims[i].Mtl->shd = Ani->shader_manager::ShaderCreate("TARGET");
          Target.Prims[i].Mtl->UpdateLoc();
          Boxes = Ani->primitive_manager::PrimCreateBox(Target.Prims[0].Min, Target.Prims[0].Max);
        }

        //Sphere->Mtl = Ani->material_manager::MtlCreate(Ka, Kd, Ks, Ph, (FLT)1.0, name);
        //Sphere->Mtl->Tex[0] = Ani->texture_manager::TexCreate("GIR.G32");
      } /* End of 'constructor' function */

      /* Destructor of test unit */
      ~final_unit( VOID )
      {
        Cube->Free();
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
      } /* End of 'Response' function */

     /* Unit render function.
      * ARGUMENTS: 
      *   - Pointer to animation system:
      *       anim *Ani;
      * RETURNS: None.
      */
      VOID Render( anim *Ani ) override
      {
        CHAR Buf[100];
 
        sprintf(Buf, "%.3f\n", Ani->FPS);
        OutputDebugStringA(Buf);
        //matr WeaponMatr = matr::Translate(vec3(Ani->Camera.Loc[0] + 0.25, Ani->Camera.Loc[1] - 0.65, 
        //                                        Ani->Camera.Loc[2] - 1));
	//WeaponMatr *= matr::Scale(vec3(0.1f, 0.15f, 0.15f));
	//WeaponMatr *= matr::Rotate(vec3(0.0f, 1.0f, 0.0f), 1.5708f);
        vec3 Weapon;
        Weapon[0] = Ani->Camera.Loc[0] + 10;
        Weapon[1] = Ani->Camera.Loc[1];
        Weapon[2] = Ani->Camera.Loc[2] + 10;

        //SetWindowTextA(Ani->win::hWnd, Buf);
        //Ani->PrimDraw(&Pr, matr().Identity() /* matr().RotateZ(sin(4 * Ani->Time) * 20) */ );
        //Ani->PrimitivesDraw(&Prs, WeaponMatr/* matr().RotateZ(sin(4 * Ani->Time) * 20) */ );
        //Ani->PrimDraw(Cube, matr::Identity());
        //Ani->PrimDraw(Sphere, matr::Identity());
        
        //math::ray<FLT> Ray;
        //Ray = Ani->Camera.Apply(Ani->FrameW, Ani->FrameH, MouseX, MouseY);
        //math::ray<FLT> Ray(Ani->Camera.Loc, vec3(MouseX, MouseY, Ani->Camera.At[2]));
        FLT Flag[5];

        for (INT i = 0; i < 5; i++)
        {
          if (Ani->KeysClick[VK_LBUTTON])
          {
            FLT MouseX = Ani->MouseX;
            FLT MouseY = Ani->MouseY;
            Ani->Camera.SetRay(MouseX, MouseY);
            Flag[i] = 1;
          }

          if (Flag[i] && (!Ani->Camera.Ray.BoxInter(Ani->Camera.Ray, vec3(-5 + 30 * i, -10, -5), vec3(5 + 30 * i, 10, 5))))
            Ani->PrimDraw(Cube, matr::Translate(vec3(30 * i, 0, 0)));
          //if (Flag[i] && (!Ani->Camera.Ray.SphereInter(vec3(0 + 30 * i, 0, 0), 10)))

        }
        //Ani->PrimitivesDraw(&Prs, matr::RotateX(180) * matr::Scale(vec3(0.2)) * matr::Translate(Weapon));
        //for (INT i = 0; i < 1; i++)
        //{
        //  Ani->PrimDraw(Boxes, matr::Identity());
        //}
        //Ani->PrimitivesDraw(&Target, matr::Identity());

        //Ani->PrimDraw(Pr, matr().Identity() /* matr().RotateZ(sin(4 * Ani->Time) * 20) */ );
      } /* End of 'Render' function */
    }; /* End of 'final_unit' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

static ivgl::anim::unit_register<ivgl::units::final_unit> _("FinalUnit");

/* END OF 'u_final.cpp' FILE */

