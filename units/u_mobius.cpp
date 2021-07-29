/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : u_mobius.cpp
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
#include "../src/anim/render/resources/topo/sphere.h"

/* Project namespace */
namespace ivgl
{
  /* Units namespace */
  namespace units
  {
    prim *Pr;
    prim *Pr2;
    prim Pr3;

    /* First unit test */
    class mobius_unit : public unit  
    {
    public:
      /* Constructor of test unit */
      mobius_unit( anim *Ani )
      {
        ivgl::anim *myw = ivgl::anim::GetPtr();

        topology::cube<vertex::std> T(vec3(0.5, 0.5, 2), vec3(-0.5, -0.5, -2));
        topology::sphere<vertex::std> T2(vec3(0, -10 + 1 + 0.5, 0), 0.5, 20, 20);

        //T.EvalNormals();

        topology::base<vertex::std> *B = dynamic_cast<topology::base<vertex::std> *>(&T);
        topology::base<vertex::std> *B2 = dynamic_cast<topology::base<vertex::std> *>(&T2);

        /* create primitive */
        //Pr->Create(*B2);

        //Pr[i] = ivgl::anim::GetPtr()->primitive_manager::PrimCreate(*B);
        //Pr[i]->Mtl = myw->material_manager::GetDef();
        //Pr[i]->Mtl->Tex[0] = myw->texture_manager::TexCreate("GIR.g32");
        Pr = ivgl::anim::GetPtr()->primitive_manager::PrimCreate(*B);
        Pr2 = ivgl::anim::GetPtr()->primitive_manager::PrimCreate(*B2);

        Pr3.PrimLoad(&Pr3, "bin/models/cow.object");

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
        //FLT u, v;
        CHAR Buf[100];
        static DBL ReloadTime = 0;

        ReloadTime += Ani->GlobalDeltaTime;

        /* Update shafer */
        if (ReloadTime > 2)
        {
          sprintf(Buf, "%.3f\n", Ani->FPS);
          OutputDebugStringA(Buf);
          ReloadTime = 0;
        }

        //FLT TranslateY = 0, TranslateZ = 0, TranslateX = 0;
        //matr mynew; 
        //for (u = 0; u < 2 * PI; u += 0.09)
        //{
        //  for (FLT v = -1; v <= 1; v += 2)
        //  {      
        //    TranslateX = (1 + cos(u / 2) ) * cos(u);
        //    TranslateY = (1 + cos(u / 2)) * sin(u);
        //    TranslateZ = sin(u);
        //    mynew = matr().Translate(vec3(TranslateX, TranslateY, TranslateZ)) * matr().Translate(vec3((1 + 0.25  * cos(Ani->Time + PI / 10 * i)) * cos(Ani->Time + PI / 10 * i),
        //    (1 + 0.25  * cos(Ani->Time + PI / 10 * i)) * sin(Ani->Time + PI / 10 * i), 0.25 * sin(Ani->Time + PI / 10 * i)));

        //    Ani->PrimDraw(Pr[i], mynew);
        //  }
        //  i++;
        //}
        static INT NumofPr = 15;
        static matr Rad = matr().Translate(vec3(0, 10, 0));
        for (INT i = 0; i < NumofPr; i++)
        {
          Ani->PrimDraw(Pr, matr().RotateX(180.0f / NumofPr * i + Ani->Time * 25) * Rad *
                         matr().RotateZ(360.0f / NumofPr * i + Ani->Time * 58));
          Ani->PrimDraw(Pr2, matr().Translate(vec3(0, sqrt(fabs(sin(math::D2R(Ani->Time * NumofPr * 25)))), 0))); 
        }
        Ani->PrimDraw(&Pr3, matr().Identity()); 

        //TranslateX = (1 + v / 2 * cos(math::D2R(u / 2) )) * cos(math::D2R(u));
        //TranslateY = (1 + v / 2 * cos(math::D2R(u / 2) )) * sin(math::D2R(u));
        //TranslateZ = v / 2 * sin(math::D2R(u));
        //Ani->PrimDraw(Pr[i], matr().Translate(vec3(TranslateX, 
        //  TranslateY, TranslateZ)) /* matr().RotateZ(sin(4 * Ani->Time) * 20) */ );
        //Ani->PrimDraw(Pr[0], matr().Identity() /* matr().RotateZ(sin(4 * Ani->Time) * 20) */ );
      } /* End of 'Render' function */
    }; /* End of 'mobius_unit' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

/* Unit declaration */
static ivgl::anim::unit_register<ivgl::units::mobius_unit> _("Mobius");
