/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : u_landscape.cpp
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
    class land_unit : public unit  
    {
      primitives Land;

    public:
      /* Constructor of test unit */
      land_unit( anim *Ani )
      {
        /*
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        system("chcp 1251");
        */
        Land.Load(&Land, "bin/models/graccy_mountains.g3dm");
        for (INT i = 0; i < Land.NumOfPrims; i++)
        {
          Land.Prims[i].Mtl->shd = Ani->shader_manager::ShaderCreate("TARGET");
          Land.Prims[i].Mtl->UpdateLoc();
        }

      } /* End of 'constructor' function */
      /* Destructor of test unit */
      ~land_unit( VOID )
      {
        Land.Free();
        //FreeConsole();

      } /* End of 'destructor' function */
      
     /* Unit response function.
      * ARGUMENTS: 
      *   - Pointer to animation system:
      *       anim *Ani;
      * RETURNS: None.
      */
      VOID Response( anim *Ani ) override
      {
        static DBL ReloadTime = 0;

        ReloadTime += Ani->GlobalDeltaTime;

        /* Update shafer */
        //if (ReloadTime > 3)
        //{
        //  printf("%f %f %f\n", Ani->Camera.Loc[0], Ani->Camera.Loc[1], Ani->Camera.Loc[2]);
        //  ReloadTime = 0;
        //}

        CHAR Buf[100];

        sprintf(Buf, "%f\n", Ani->FPS);

        OutputDebugStringA(Buf);
      } /* End of 'Response' function */

     /* Unit render function.
      * ARGUMENTS: 
      *   - Pointer to animation system:
      *       anim *Ani;
      * RETURNS: None.
      */
      VOID Render( anim *Ani ) override
      {
        Ani->PrimitivesDraw(&Land, matr::Scale(vec3(100.0f)));
      } /* End of 'Render' function */
    }; /* End of 'land_unit' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

static ivgl::anim::unit_register<ivgl::units::land_unit> _("LandUnit");

/* END OF 'u_landscape.cpp' FILE */

