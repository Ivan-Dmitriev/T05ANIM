/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : u_cube.cpp
 * PURPOSE     : Animation project.
 *               Units implementation module.
 *               Cube unit module.
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
    class cube_unit : public unit  
    {
      prim *Pr;

    public:
      /* Constructor of test unit */
      cube_unit( anim *Ani )
      {
        ivgl::anim *myw = ivgl::anim::GetPtr();
        //myw->shader_manager::ShaderCreate("DEFAULT");
        //myw->shader_manager::ShaderCreate("DEFAULT");

        topology::cube<vertex::std> T(vec3(0, 0, 0), vec3(10, 20, 10));
        //T.EvalNormals();

        topology::base<vertex::std> *B = dynamic_cast<topology::base<vertex::std> *>(&T);
        /* create primitive */
        //Pr->Create(*B);
        Pr = ivgl::anim::GetPtr()->primitive_manager::PrimCreate(*B);
        Pr->Mtl = myw->material_manager::GetDef();
        Pr->Mtl->Tex[0] = myw->texture_manager::TexCreate("GIR.g32");
        //Pr.Create(&Prs, "bin/models/btr.g3dm");
      } /* End of 'constructor' function */
      
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

        Ani->PrimDraw(Pr, matr().Identity() /* matr().RotateZ(sin(4 * Ani->Time) * 20) */ );
      } /* End of 'Render' function */
    }; /* End of 'cube_unit' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

/* Unit declaration */
static ivgl::anim::unit_register<ivgl::units::cube_unit> _("CubeUnit");

/* END OF 'u_cube.cpp' FILE */

