/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : render_primitives.cpp
 * PURPOSE     : Animation project.
 *               Render class implementation module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "../resource_collection.h"
//#include "../resources.h"
#include "../../render.h"
#include "../../../anim.h"
//#include "../shaders/shader.h"

namespace ivgl
{
  /* Primitive creation function.
   * ARGUMENTS:
   *   - primitive pointer:
   *       vg4PRIM *Pr;
   *   - vertex attributes array:
   *       vertex *V;
   *   - number of vertices:
   *       INT NumOfV;
   *   - index array (for trimesh – by 3 ones, may be NULL)
   *       INT *I;
   *   - number of indices
   *       INT NumOfI;
   * RETURNS: None.
   */
  VOID primitive::PrimCreate( primitive *Pr, vertex::std *V, INT NumOfV, UINT *I, INT NumOfI )
  {
    memset(Pr, 0, sizeof(primitive));   /* <-- <string.h> */
 
    if (V != NULL && NumOfV != 0)
    {
      glGenBuffers(1, &Pr->VBuf);
      glGenVertexArrays(1, &Pr->VA);
 
      glBindVertexArray(Pr->VA);
      glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex::std) * NumOfV, V, GL_STATIC_DRAW);
 
      glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(vertex::std),
                            (VOID *)0); /* position */
      glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(vertex::std),
                            (VOID *)sizeof(vec3)); /* texture coordinates */
      glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(vertex::std),
                            (VOID *)(sizeof(vec3) + sizeof(vec2))); /* normal */
      glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(vertex::std),
                            (VOID *)(sizeof(vec3) * 2 + sizeof(vec2))); /* color */
 
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);
      glEnableVertexAttribArray(3);
      glBindVertexArray(0);
    }
 
    if (I != NULL && NumOfI != 0)
    {
      glGenBuffers(1, &Pr->IBuf);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);
      Pr->NumOfElements = NumOfI;
    }
    else
      Pr->NumOfElements = NumOfV;
    Pr->Trans = math::matr<FLT>::Identity();
  } /* End of 'PrimCreate' function */
  
  VOID EvalNormals( VOID )
  {
  }
  
  /* Load primitive from '*.OBJ' file function.
   * ARGUMENTS:
   *   - pointer to primitive to load:
   *       ns6PRIM *Pr;
   *   - '*.OBJ' file name:
   *       CHAR *FileName;
   * RETURNS:
   *   (BOOL) TRUE if success, FALSE otherwise.
   */
  BOOL primitive::PrimLoad( primitive *Pr, const CHAR *FileName )
  {
    FILE *F;
    INT i, nv = 0, nind = 0, size;
    vertex::std *V;
    UINT *Ind;
    static CHAR Buf[1000];
 
    memset(Pr, 0, sizeof(primitive));
    if ((F = fopen(FileName, "r")) == NULL)
      return FALSE;
 
    /* Count vertexes and indexes */
    while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    {
      if (Buf[0] == 'v' && Buf[1] == ' ')
        nv++;
      else if (Buf[0] == 'f' && Buf[1] == ' ')
      {
        INT n = 0;
 
        for (i = 1; Buf[i] != 0; i++)
          if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
            n++;
        nind += (n - 2) * 3;
      }
    }
 
    size = sizeof(vertex::std) * nv + sizeof(INT) * nind;
    //if ((V = malloc(size)) == NULL)
    //  return FALSE;
    V = new vertex::std[size];
    //V = new vertex[size];
    //Ind = new INT[ ]
    Ind = (UINT *)(V + nv);
    memset(V, 0, size);
 
    /* Load primitive */
    rewind(F);
    nv = 0;
    nind = 0;
    while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    {
      if (Buf[0] == 'v' && Buf[1] == ' ')
      {
        DBL x, y, z;
        /* !!!!!!!!!!!!!!!!! */
      
        //vec4 c(math::Rnd1F(), math::Rnd1F(), math::Rnd1F(), 1);
 
        (VOID)sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
        V[nv].C = vec4(0, 0, 0, 1);
        V[nv++].P = vec3(x, y, z);
      }
      else if (Buf[0] == 'f' && Buf[1] == ' ')
      {
        INT n = 0, n0 = 0, n1 = 0, nc = 0;
 
        for (i = 1; Buf[i] != 0; i++)
          if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
          {
            (VOID)sscanf(Buf + i, "%i", &nc);
            if (nc < 0)
              nc = nv + nc;
            else
              nc--;
 
            if (n == 0)
              n0 = nc;
            else if (n == 1)
              n1 = nc;
            else
            {
              Ind[nind++] = n0;
              Ind[nind++] = n1;
              Ind[nind++] = nc;
              n1 = nc;
            }
            n++;
          }
      }
    }
    fclose(F);
    /* making an auto normalize */
    AutoNorm(V, Ind, nind);

    PrimCreate(Pr, V, nv, Ind, nind);
    delete [] V;
    return TRUE;
  } /* End of 'PrimLoad' function */

  /* Draw render primitive function.
   * ARGUMENTS:
   *   - primitive:
   *      dg5PRIM *Pr;
   *   - matrix:
   *      MATR World;
   * RETURNS: None.
   */
  VOID render::PrimDraw( primitive *Pr, matr World )
  {
    matr w = Pr->Trans * World;
    w.Inverse();
    w.Transpose();
    matr winv = w;
    // MatrTranspose(MatrInverse(w)),
    matr wvp =  w * Camera.VP; /*camera:: MatrVP;  */

    //INT loc, prg;
    INT gl_prim_type = Pr->Type == prim_type::LINES ? GL_LINES :
                       Pr->Type == prim_type::TRIMESH ? GL_TRIANGLES :
                       Pr->Type == prim_type::TRISTRIP ? GL_TRIANGLE_STRIP :
                       Pr->Type == prim_type::PATH ? GL_PATCHES :
                       GL_POINTS;

    glLoadMatrixf(wvp);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* making an array of vertices active */
    glBindVertexArray(Pr->VA);
    if (gl_prim_type == GL_PATCHES)
    {
      glPatchParameteri(GL_PATCH_VERTICES, 16);
      glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
    }
    else if (Pr->IBuf == 0)
      glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
    else
    {
      /* making an array of indexes active */
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
      /* drawing */
      glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
      /* disable index array */
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    /* disable vertex array */
    glBindVertexArray(0);
  } /* End of 'PrimDraw' function */

  /* Free render primitive function.
   * ARGUMENTS:
   *   - Primitive to be deleted:
   *       primitive *Pr;
   * RETURNS: None.
   */
  VOID primitive::Free( primitive *Pr )
  {
    /* making an array of vertices active */
    glBindVertexArray(Pr->VA);
    /* unhooking the buffer */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Pr->VBuf);
    /* making an array of vertices inactive */
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Pr->VA);

    /* making an array of indexes inactive */
    glDeleteBuffers(1, &Pr->IBuf);

    memset(Pr, 0, sizeof(primitive));
  } /* End of 'Free' function */

  /* Auto normal function.
   * ARGUMENTS:
   *   - vertex array:
   *      dg5VERTEX *V;
   *   - index:
   *      INT *I;
   *   - num of index:
   *      INT NoofI;
   * RETURNS: None.
   */
  VOID primitive::AutoNorm( vertex::std *V, UINT *I, INT NoofI )
  {
    for (INT i = 3; i < NoofI; i += 3)
    {
    
      vec3 N = ((V[I[i + 1]].P - V[I[i]].P) %
                                       (V[I[i + 2]].P - V[I[i]].P)).Normalizing();

      V[I[i + 0]].N = (V[I[i + 0]].N + N).Normalizing();
      V[I[i + 1]].N = (V[I[i + 1]].N + N).Normalizing();
      V[I[i + 2]].N = (V[I[i + 2]].N + N).Normalizing();
      
      V[I[i + 0]].C = vec4(V[I[i + 0]].N[0], V[I[i + 0]].N[1], V[I[i + 0]].N[2], 1);
      V[I[i + 1]].C = vec4(V[I[i + 1]].N[0], V[I[i + 1]].N[1], V[I[i + 1]].N[2], 1);;
      V[I[i + 2]].C = vec4(V[I[i + 2]].N[0], V[I[i + 2]].N[1], V[I[i + 2]].N[2], 1);;

    }

  } /* End of 'AutoNorm' function */

  /* Draw render primitive function.
   * ARGUMENTS:
   *   - primitive:
   *      dg5PRIM *Pr;
   *   - matrix:
   *      MATR World;
   * RETURNS: None.
   */
  VOID render::PrimDraw( prim *Pr, matr World )
  {
    matr w = Pr->Transform * World;
    w.Inverse();
    w.Transpose();
    matr winv = w;
    // MatrTranspose(MatrInverse(w)),
    matr wvp =  w * Camera.VP; /*camera:: MatrVP;  */
    shader *shd = shader_manager::ShaderCreate("DEFAULT");
    ivgl::anim *myw = ivgl::anim::GetPtr();

    //INT loc, prg;
    INT gl_prim_type = Pr->Type == prim_type::LINES ? GL_LINES :
                       Pr->Type == prim_type::TRIMESH ? GL_TRIANGLES :
                       Pr->Type == prim_type::TRISTRIP ? GL_TRIANGLE_STRIP :
                       Pr->Type == prim_type::PATH ? GL_PATCHES :
                       GL_POINTS;

    if (shd != nullptr)
    {
      if (shd->Apply() != 0)
      {
        shd->UniSetLoc("MatrWVP", wvp, Pr->Mtl->MatrWVPLoc);
        shd->UniSetLoc("MatrWP", w, Pr->Mtl->MatrWPLoc);
        shd->UniSetLoc("MatrWInv", winv, Pr->Mtl->MatrWInvLoc);
        shd->UniSetLoc("Time", myw->Time, Pr->Mtl->TimeUniLoc);
      }
    }
    Pr->Mtl->Apply();

    /* making an array of vertices active */
    glBindVertexArray(Pr->VA);
    if (gl_prim_type == GL_PATCHES)
    {
      glPatchParameteri(GL_PATCH_VERTICES, 16);
      glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
    }
    else if (Pr->IBuf == 0)
      glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
    else
    {
      /* making an array of indexes active */
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
      /* drawing */
      glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
      /* disable index array */
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    /* disable vertex array */
    glBindVertexArray(0);
  } /* End of 'PrimDraw' function */
   /* Draw render primitives function.
   * ARGUMENTS:
   *   - primitive:
   *      primitives *Prs;
   *   - matrix:
   *      matr World;
   * RETURNS: None.
   */
  VOID render::PrimitivesDraw( primitives *Prs, matr World )
  {
    World = World * Prs->Transform;
    for (INT i = 0; i < Prs->NumOfPrims; i++)
      PrimDraw(&Prs->Prims[i], World);
  } /* End of 'PrimitivesDraw' function */
    /* Primitive creation function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &T;
     * RETURNS:
     *   (prim &) self reference.
     */
    template<class vertex>
      prim & prim::Create( const topology::base<vertex> &T )
      {
        Free();
        Type = T.Type;
        // Setup data order due to vertex description string
        const std::string dsc = vertex::Description;

        if (!T.V.empty() && T.V.size() != 0)
        {
          glGenBuffers(1, &VBuf);
          glGenVertexArrays(1, &VA);
 
          glBindVertexArray(VA);
          glBindBuffer(GL_ARRAY_BUFFER, VBuf);
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * T.V.size(), T.V.data(), GL_STATIC_DRAW);
 
          INT attr = 0, offset = 0;
          for (auto c = dsc.begin(); c != dsc.end(); c++)
            if (*c == 'f' || *c == 'i')
            {
              CHAR type = *c++;
              INT n = *c - '0';
 
              if (n < 0 || n > 9)
                break;
              // Enable used attributes
              glEnableVertexAttribArray(attr);
              glVertexAttribPointer(attr, n, type == 'f' ? GL_FLOAT : GL_INT, FALSE,
                sizeof(vertex), reinterpret_cast<VOID *>(offset));
              offset += n * 4;
              attr++;
            }
            else
              break;
          glBindVertexArray(0);
        }
        if (!T.I.empty() && T.I.size() != 0)
        {
          glGenBuffers(1, &IBuf);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBuf);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * T.I.size(), T.I.data(), GL_STATIC_DRAW);
          NumOfElements = static_cast<INT>(T.I.size());
        }
        else
          NumOfElements = static_cast<INT>(T.V.size());
        Transform = matr().Identity();
        Mtl = material_manager::GetDef();

        return *this;
      } /* End of 'Create' function */

   /* Load primitive from '*.OBJ' file function.
    * ARGUMENTS:
    *   - pointer to primitive to load:
    *       prim *Pr;
    *   - '*.OBJ' file name:
    *       CHAR *FileName;
    * RETURNS:
    *   (BOOL) TRUE if success, FALSE otherwise.
    */
    prim * prim::PrimLoad( prim *Pr, const CHAR *FileName )
    {
      INT
        noofv = 0,
        noofi = 0;
      FILE *F;
      CHAR Buf[1000];

      /* Open file */
      if ((F = fopen(FileName, "r")) == NULL)
        return FALSE;

      /* Count vertex and index quantities */
      while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
      {
        if (Buf[0] == 'v' && Buf[1] == ' ')
          noofv++;
        else if (Buf[0] == 'f' && Buf[1] == ' ')
          noofi++;
      }

      std::vector<INT> I(3 * noofi);
      std::vector<vertex::std> V(noofv);

      /* Read vertices and facets data */
      rewind(F);
      noofv = noofi = 0;
      while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
      {
        if (Buf[0] == 'v' && Buf[1] == ' ')
        {
          FLT x, y, z;

          sscanf(Buf + 2, "%f%f%f", &x, &y, &z);
          V[noofv++].P = vec3(x, y, z);
        }
        else if (Buf[0] == 'f' && Buf[1] == ' ')
        {
          INT n1, n2, n3;

          /* Read one of possible facet references */
          sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
          sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
          sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
          sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3);
          n1--;
          n2--;
          n3--;
          I[noofi++] = n1;
          I[noofi++] = n2;
          I[noofi++] = n3;
        }
      }

      topology::trimesh<vertex::std> T(V, I);
      /* making an auto normalize */
      T.EvalNormals();
      //T.EvalNormals();

      topology::base<vertex::std> *B = dynamic_cast<topology::base<vertex::std> *>(&T);
      /* create primitive */
      //Pr->Create(*B);
      fclose(F);

      return ivgl::anim::GetPtr()->primitive_manager::PrimCreate(*B);
    } /* End of 'prim::PrimLoad' function */

} /* end of 'ivgl' namespace */


/* END OF 'render_primitives.cpp' FILE */
