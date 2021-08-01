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
#define NumOfObjects 200

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
      primitives Tree;
      primitives Tree2;

      primitives Bush1;
      primitives Bush2;

      vec3 ArrayOfCoords[NumOfObjects];
      INT RandomRotate[NumOfObjects];

    public:
      /* Constructor of test unit */
      land_unit( anim *Ani )
      {
        math::noise<FLT> Noise;
        /*
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        system("chcp 1251");
        */
        //Land.Load(&Land, "bin/models/tree.g3dm");
        Bush1.Load(&Bush1, "bin/models/Bushes_with_berries.g3dm");
        Bush2.Load(&Bush2, "bin/models/Bushes.g3dm");

        Tree.Load(&Tree, "bin/models/fat_tree.g3dm");
        
        for (INT i = 0; i < Bush1.NumOfPrims; i++)
        {
          Bush1.Prims[i].Mtl->shd = Ani->shader_manager::ShaderCreate("TREE");
          Bush1.Prims[i].Mtl->UpdateLoc();
        }
        for (INT i = 0; i < Bush1.NumOfPrims; i++)
        {
          Bush2.Prims[i].Mtl->shd = Ani->shader_manager::ShaderCreate("TREE");
          Bush2.Prims[i].Mtl->UpdateLoc();
        }

        //for (INT i = 0; i < Land.NumOfPrims; i++)
        //{
          //Land.Prims[i].Mtl->shd = Ani->shader_manager::ShaderCreate("TARGET");
          //Land.Prims[i].Mtl->UpdateLoc();
        //}
        for (INT i = 0; i < NumOfObjects; i++)
        {
          FLT X = Noise.Noise1D(i * 4 + 7);
          FLT Z = Noise.Noise1D(i * 2 + 1);
          if (i % 4 == 0)
            ArrayOfCoords[i] = vec3(X, 0, Z);
          else if (i % 4 == 1)
            ArrayOfCoords[i] = vec3(-X, 0, Z);
          else if (i % 4 == 2)
            ArrayOfCoords[i] = vec3(-X, 0, -Z);
          else if (i % 4 == 3)
            ArrayOfCoords[i] = vec3(X, 0, -Z);
  
          ArrayOfCoords[i] = ArrayOfCoords[i] * 300;
          RandomRotate[i] = Noise.Noise1D(i * 5 + 1);
        }

        //INT loc;
        //INT id = Bush1.Prims[0].Mtl->shd->ProgId;
        //glUseProgram(id);
        //if ((loc = glGetUniformLocation(id, "Transform[0]")) != -1)
        //  glUniform3fv(loc, NumOfObjects, ArrayOfCoords[0]);

      } /* End of 'constructor' function */
      /* Destructor of test unit */
      ~land_unit( VOID )
      {
        Bush1.Free();
        Tree.Free();
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
        //static DBL ReloadTime = 0;

        //ReloadTime += Ani->GlobalDeltaTime;

        /* Update shafer */
        //if (ReloadTime > 3)
        //{
        //  printf("%f %f %f\n", Ani->Camera.Loc[0], Ani->Camera.Loc[1], Ani->Camera.Loc[2]);
        //  ReloadTime = 0;
        //}

        //CHAR Buf[100];

        //sprintf(Buf, "%f\n", Ani->FPS);

        //OutputDebugStringA(Buf);
      } /* End of 'Response' function */

     /* Unit render function.
      * ARGUMENTS: 
      *   - Pointer to animation system:
      *       anim *Ani;
      * RETURNS: None.
      */
      VOID Render( anim *Ani ) override
      {
        for (INT i = 0; i < NumOfObjects; i++)
        {
          vec3 v = ArrayOfCoords[i];
          vec3 v1 = v;
          v1[2] += 10;
          v1[0] += 10;

          Ani->PrimitivesDraw(&Bush1, matr::Scale(vec3(10.0f)) * matr::Translate(ArrayOfCoords[i]));
          Ani->PrimitivesDraw(&Bush2, matr::Scale(vec3(4.0f)) * matr::Translate(v1) * matr::RotateY(RandomRotate[i] * 30));

          v[0] += 10;
          Ani->PrimitivesDraw(&Tree, matr::Translate(v));
        }
        //Ani->PrimitivesDraw(&Bush1, matr::Scale(vec3(10.0f)));

        //Ani->PrimitivesDraw(&Tree, matr::Scale(vec3(10.0f)));
      } /* End of 'Render' function */
    }; /* End of 'land_unit' class */
  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

static ivgl::anim::unit_register<ivgl::units::land_unit> _("LandUnit");

/* END OF 'u_landscape.cpp' FILE */

