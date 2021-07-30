/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : prim.h
 * PURPOSE     : Animation project.
 *               Primitive handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
 
#ifndef __prim_h_
#define __prim_h_

#include <iostream>
#include <map>

#include "../topo/topology.h"
#include "../resources.h"
#include "../mtl/material.h"
#include "../textures/textures.h"
#include "../topo/sphere.h"

/* Project namespace */
namespace ivgl
{
  /* Topology namespace */
  namespace topology
  {
    template<class vertex> class base;
  }

  /* Primitive class */
  class primitive
  {
  public:
    UINT
      NumOfElements,    /* Number of indices/vecrtices */
      VA,               /* Vertex array Id */
      VBuf,             /* Vertex buffer Id */
      IBuf;             /* Index buffer Id */

    matr Trans;   /* Additional transformation matrix */
    INT MtlNo;    /* Material number in material array */
    prim_type Type;

    vec3 MinBB, MaxBB; /* max and min bound box */

  /* Load primitive from '*.OBJ' file function.
   * ARGUMENTS:
   *   - pointer to primitive to load:
   *       ns6PRIM *Pr;
   *   - '*.OBJ' file name:
   *       CHAR *FileName;
   * RETURNS:
   *   (BOOL) TRUE if success, FALSE otherwise.
   */
  BOOL PrimLoad( primitive *Pr, const CHAR *FileName );

  /* Primitive delete function.
   * ARGUMENTS:
   *   - primitive pointer:
   *       primitive *Pr;
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
  VOID PrimCreate( primitive *Pr, vertex::std *V, INT NumOfV, UINT *I, INT NumOfI );

  /* Free render primitive function.
   * ARGUMENTS:
   *   - Primitive to be deleted:
   *       primitive *Pr;
   * RETURNS: None.
   */
  VOID Free( primitive *Pr );
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
  VOID AutoNorm( vertex::std *V, UINT *I, INT NoofI );

  }; /* End of 'primitive' class */

  /* Primitive handle class */
  class prim : public material_manager
  {
    friend class primitive_manager;
    template<typename entry_type, typename index_type>
      friend class resource_manager;

    friend class render;
  private:
    prim_type Type;    /* Primitive type */
    UINT VA;           /* Primitive vertex array */
    UINT VBuf;         /* Vertex buffer */
    UINT IBuf;         /* Index buffer */
    matr Transform;    /* Primitive transformation matrix */

  public:
    INT NumOfElements; /* Number of elements for OpenGL */
    vec3
      Min,             /* Minimal primitive position */
      Max;             /* Maximal primitive position */
    material *Mtl;     /* Material pointer */
 
    /* Class constructor */
    prim( VOID ) : VA(0), VBuf(0), IBuf(0), NumOfElements(0), Type(prim_type::TRIMESH)
    {
      Min = vec3(0);
      Max = vec3(0);
    } /* End of 'prim' function */
 
    /* Class destructor */
    ~prim( VOID )
    {
    } /* End of '~prim' function */
 
    /* Primitive creation function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &T;
     * RETURNS:
     *   (prim &) self reference.
     */
    template<class vertex>
      prim & Create( const topology::base<vertex> &T );

      /* Free render primitive function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      VOID Free( VOID )
      {
        if (VA != 0)
        {
          /* making an array of vertices active */
          glBindVertexArray(VA);
          /* unhooking the buffer */
          glBindBuffer(GL_ARRAY_BUFFER, 0);
          if (VBuf != 0)
            glDeleteBuffers(1, &VBuf);
          /* making an array of vertices inactive */
          glBindVertexArray(0);
          glDeleteVertexArrays(1, &VA);
          VA = 0;
          VBuf = 0;
        } 
        if (IBuf != 0)
          /* making an array of indexes inactive */
          glDeleteBuffers(1, &IBuf);

        //memset(Pr, 0, sizeof(prim));
      } /* End of 'Free' function */
     /* Load primitive from '*.OBJ' file function.
      * ARGUMENTS:
      *   - pointer to primitive to load:
      *       prim *Pr;
      *   - '*.OBJ' file name:
      *       CHAR *FileName;
      * RETURNS:
      *   (prim *) pointer to created primitive.
      */
    prim * PrimLoad( prim *Pr, const CHAR *FileName );

     /* Find borders of primitive function.
      * ARGUMENTS: NONE.
      * RETURNS: NONE.
      */
    VOID FindBB( VOID );

  };
  /* Primitive manager */
  class primitive_manager : public resource_manager<prim, INT> 
  {
    /* Resource stock */
    //stock<prim> Stock;
  public:  
    /* Create primitive function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &Tpl;
     * RETURNS:
     *   (prim *) created primitive interface.
     */
    template<class vertex_type>
      prim * PrimCreate( const topology::base<vertex_type> &Tpl )
      {
        return resource_manager::Add(prim().Create(Tpl));
      } /* End of 'PrimCreate' function */
    /* Create primitive from file function.
     * ARGUMENTS:
     *   - file name:
     *       const CHAR *FileName;
     * RETURNS:
     *   (prim *) created primitive interface.
     */
    template<class vertex_type>
      prim * PrimCreate( const CHAR *FileName )
      {
        return resource_manager::Add(prim().PrimLoad(FileName));
      } /* End of 'PrimCreate' function */
    /* Create primitive box function.
     * ARGUMENTS:
     *   - topology base reference:
     *       const topology::base &Tpl;
     * RETURNS:
     *   (prim *) created primitive interface.
     */
    prim * PrimCreateBox( vec3 Min, vec3 Max )
    {
      topology::cube<vertex::std> T(Max, Min);
      topology::base<vertex::std> *B = dynamic_cast<topology::base<vertex::std> *>(&T);

      return PrimCreate(*B);
    } /* End of 'PrimCreateBox' function */
    /* Create primitive box function.
     * ARGUMENTS:
     *   - sphere position:
     *       vec3 Pos;
     *   - sphere radius:
     *       FLT R;
     *   - sphere sections:
     *       FLT W, H; 
     * RETURNS:
     *   (prim *) created primitive interface.
     */

    prim * PrimCreateSphere( vec3 Pos, FLT R, FLT W, FLT H )
    {
      topology::sphere<vertex::std> T(Pos, R, W, H);
      topology::base<vertex::std> *B = dynamic_cast<topology::base<vertex::std> *>(&T);

      return PrimCreate(*B);
    } /* End of 'PrimCreateBox' function */

      /* Free render primitive function.
       * ARGUMENTS:
       *   - Primitive to be deleted:
       *       primitive *Pr;
       * RETURNS: None.
       */
      VOID PrimFree( prim *Pr )
      {
        resource_manager::Delete(Pr);
      } /* End of 'PrimFree' function */

      /* Class destructor */
      ~primitive_manager( VOID )
      {
        for (auto &pr : Stock)
          pr.second.Free();
      } /* End of 'destructor' function */
  }; /* End of 'primitive_manager' class */

} /* end of 'ivgl' namespace */

#endif /* __prim_h_ */

/* END OF 'prim.h' FILE */
