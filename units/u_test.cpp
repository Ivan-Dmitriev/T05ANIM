/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : u_test.cpp
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

#include "../src/anim/anim.h"

/* Project namespace */
namespace ivgl
{
  /* Units namespace */
  namespace units
  {
    /* First unit test */
    class test_unit : public unit  
    {
    public:
      prim Pr;
      primitives Prs; 
      /* Constructor of test unit */
      test_unit( anim *Ani )
      {
        //Pr.PrimLoad(&Pr, "bin/models/cow.object");
        Prs.Load(&Prs, "bin/models/btr.g3dm");
        //Pr->PrimLoad<ivgl::vertex::std>(Pr, "bin/models/cow.object");
      } /* End of 'constructor' function */

      /* Destructor of test unit */
      ~test_unit( VOID )
      {
        //Pr->Free();
      } /* End of 'destructor' function */

     /* Unit response function.
      * ARGUMENTS: 
      *   - Pointer to animation system:
      *       anim *Ani;
      * RETURNS: None.
      */
      VOID Response( anim *Ani ) override
      {
        /*
        CHAR Buf[100], Buf2[100];
 
        sprintf(Buf, "%.3f\n", Ani->FPS);
        SetWindowTextA(Ani->hWnd, Buf);
        if (Ani->KeysClick['F'])
          Ani->FlipFullScreen();

        if (Ani->Keys[VK_LMENU])
        {
          sprintf(Buf2, "%i\n", Ani->MouseZ);
          //OutputDebugStringA(Buf2);
        }
        //OutputDebugStringA("WoW");
        */
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
        //SetWindowTextA(Ani->win::hWnd, Buf);
        //Ani->PrimDraw(&Pr, matr().Identity() /* matr().RotateZ(sin(4 * Ani->Time) * 20) */ );
        Ani->PrimitivesDraw(&Prs, matr().Scale(vec3(0.01)) /* matr().RotateZ(sin(4 * Ani->Time) * 20) */ );
      } /* End of 'Render' function */
    }; /* End of 'test_unit' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

static ivgl::anim::unit_register<ivgl::units::test_unit> _("TestUnit");

/* END OF 'u_test.cpp' FILE */

