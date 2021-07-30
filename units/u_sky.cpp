/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : u_sky.cpp
 * PURPOSE     : Animation project.
 *               Units implementation module.
 *               Sky unit module.
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
    class sky_unit : public unit  
    {
      prim *Pr;
    public:
      /* Constructor of test unit */
      sky_unit( anim *Ani )
      {
        material mtl;

        mtl.Name = "sky sphere material";
        mtl.Tex[0] = Ani->texture_manager::TexCreate("SkySphere.g32");
        mtl.shd = Ani->shader_manager::ShaderCreate("SKY");
        mtl.Ph = std::pair(1.0, -1);
  
        std::vector<vertex::std> NewV;
        std::vector<INT> NewI; 
        topology::points<vertex::std> T(NewV, NewI);

        Pr = Ani->primitive_manager::PrimCreate(T);
        Pr->Mtl = Ani->material_manager::MtlCreate(mtl);
        Pr->NumOfElements = 1;
      } /* End of 'constructor' function */
      /* Destructor of test unit */
      ~sky_unit( VOID )
      {
        Pr->Free();
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
        glDisable(GL_DEPTH_TEST);
        glDepthMask(FALSE);
        Ani->PrimDraw(Pr, matr::Identity());
        glDepthMask(TRUE);
        glEnable(GL_DEPTH_TEST);
      } /* End of 'Render' function */
    }; /* End of 'sky_unit' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

static ivgl::anim::unit_register<ivgl::units::sky_unit> _("SkyUnit");

/* END OF 'u_sky.cpp' FILE */

