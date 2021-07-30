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
        Land.Load(&Land, "bin/models/landscape.g3dm");
      } /* End of 'constructor' function */
      /* Destructor of test unit */
      ~land_unit( VOID )
      {
        Land.Free();
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
        Ani->PrimitivesDraw(&Land, matr::Identity());
      } /* End of 'Render' function */
    }; /* End of 'land_unit' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

static ivgl::anim::unit_register<ivgl::units::land_unit> _("LandUnit");

/* END OF 'u_landscape.cpp' FILE */

