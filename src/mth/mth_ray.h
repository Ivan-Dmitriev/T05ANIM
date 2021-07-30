/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME    : mth_ray.h
 * PURPOSE      : Animation project.
 *                Mathematics library.
 *                Space camera handle module.
 * PROGRAMMER   : CGSG-SummerCamp'2021.
 *                ID3
 * LAST UPDATE  : 23.07.2021
 * NOTE         : Module namespace 'math'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __mth_ray_h_
#define __mth_ray_h_

#include "mthdef.h"
         
/* Math library namespace */
namespace math
{
  /* 3D ray class declaration */
  template<class Type>
    class ray
    {
      //friend class camera;
    public:
      vec3<Type>
        /* ray Orgin */
        Org, 
        /* ray normalized Direction */
        Dir; 

    public:
      /* Class default constructor */
      ray( VOID )
      {
      } /* End of 'ray' function */

      /* Class copying constructor.
       * ARGUMENTS:
       *   - ray to be copied:
       *       const ray &R;
       */
      ray( const ray &R ) : Org(R.Org), Dir(R.Dir)
      {
      } /* End of 'ray' function */

      /* Class constructor.
       * ARGUMENTS:
       *   - ray new Orgin and Direction:
       *       const vec<Type> &O, &D;
       */
      ray( const vec3<Type> &O, const vec3<Type> &D ) : Org(O), Dir(D)
      {
        Dir.Normalize();
      } /* End of 'ray' function */

      /* Obtain ray point function.
       * ARGUMENTS:
       *   - ray point distance:
       *       Type T;
       * RETURNS:
       *   (vec<Type>) result point on ray.
       */
      vec3<Type> operator()( Type T ) const
      {
        return Org + Dir * T;
      } /* End of 'operator()' function */

      /* Obtain ray Orgin point function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec<Type>) result ray Orgin point.
       */
      vec3<Type> GetOrg( VOID ) const
      {
        return Org;
      } /* End of 'GetOrg' function */

      /* Obtain ray Direction vector function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (vec<Type>) result ray Direction vector.
       */
      vec3<Type> GetDir( VOID ) const
      {
        return Dir;
      } /* End of 'GetDir' function */
      /* Dot multiply two vectors function.
       * ARGUMENTS:
       *   - vectors to be dot multiplied:
       *       VEC V1, V2;
       * RETURNS:
       *   (FLT) result scalar.
       */
      Type VecDotVec( vec3<Type> V1, vec3<Type> V2 )
      {
        return V1[0] * V2[0] + V1[1] * V2[1] + V1[2] * V2[2];
      } /* End of 'VecDotVec' function */

      /* Check intersection between sphere and ray function.
       * ARGUMENTS:
       *   - sphere center:
       *       vec3<Type> C, V2;
       *   - sphere radius:
       *       Type R;
       * RETURNS:
       *   (BOOL) TRUE if ok, FALSE otherwise.
       */
      BOOL SphereInter( vec3<Type> C, Type R )
      {
        vec3<Type> a = C - Org;
        FLT OC2, OK, OK2, R2, h2;

        OC2 = VecDotVec(a, a);
        OK = VecDotVec(a, Dir);
        OK2 = VecDotVec(a, Dir) * VecDotVec(a, Dir);
        R2 = R * R;
        h2 = R2 - (OC2 - OK2);
        if (OC2 < R2)
          return TRUE;
        if (OK < 0 || h2 < 0)
          return FALSE;
        return TRUE;
      } /* End of 'SphereInter' function */

      /* Check intersection between box and ray function.
       * ARGUMENTS:
       *   - box borders:
       *       vec3<Type> min, max;
       *   - ray:
       *       const ray &r;
       * RETURNS:
       *   (BOOL) TRUE if ok, FALSE otherwise.
       */
      BOOL BoxInter( const ray &r, vec3<Type> min, vec3<Type> max )
      { 
        FLT tmin = (min[0] - r.Org[0]) / r.Dir[0]; 
        FLT tmax = (max[0] - r.Org[0]) / r.Dir[0]; 
 
        if (tmin > tmax) 
          math::Swap(&tmin, &tmax); 
 
        FLT tymin = (min[1] - r.Org[1]) / r.Dir[1]; 
        FLT tymax = (max[1] - r.Org[1]) / r.Dir[1]; 
 
        if (tymin > tymax) 
          math::Swap(&tymin, &tymax); 
 
        if ((tmin > tymax) || (tymin > tmax)) 
          return FALSE; 
 
        if (tymin > tmin) 
            tmin = tymin; 
 
        if (tymax < tmax) 
            tmax = tymax; 
 
        FLT tzmin = (min[2] - r.Org[2]) / r.Dir[2]; 
        FLT tzmax = (max[2] - r.Org[2]) / r.Dir[2]; 
 
        if (tzmin > tzmax) 
          math::Swap(&tzmin, &tzmax); 
 
        if ((tmin > tzmax) || (tzmin > tmax)) 
            return FALSE; 
 
        if (tzmin > tmin) 
            tmin = tzmin; 
 
        if (tzmax < tmax) 
            tmax = tzmax; 
 
        return TRUE; 
      } /* End of 'BoxInter' function */
    }; /* End of 'ray' class */

} /* end of 'math' namespace */

#endif /* __mth_ray_h_ */

/* END OF 'mth_ray.h' FILE */
