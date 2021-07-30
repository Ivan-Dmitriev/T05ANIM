/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : sphere.cpp
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

#ifndef __sphere_h_
#define __sphere_h_

#include "topology.h"

/* Project namespace */
namespace ivgl
{
  /* Units namespace */
  namespace topology
  {
    template<typename vertex_type>
      class sphere : public grid<vertex_type>
      {
      public:
        /* Constructor of test unit */
        sphere( vec3 Centre, FLT R, FLT W, FLT H ) : grid<vertex_type>(W + 1, H + 1)
        {
          //DBL theta = 0, phi = 0;
          INT k = 0;

          for (INT i = 0 ; i < H + 1; i++)
          {
            DBL theta = static_cast<DBL>(i) / (H) * PI; 
            for (INT j = 0; j < W + 1; j++)
            {
              DBL phi = static_cast<DBL>(j) / (W) * 2 * PI;

              vertex_type v;

              v.T = vec2(static_cast<FLT>(j) / W, static_cast<FLT>(i) / H);
              v.P[0] = R * sin(theta) * sin(phi) + Centre[0];
              v.P[1] = R * cos(theta) + Centre[1];
              v.P[2] = R * sin(theta) * cos(phi) + Centre[2];
              v.C = vec4(1);
              v.N = vec3(sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi));
              //this->V[k] = v;
              //k++;
              this->V[k] = v;
              k++;
            }
          }
        }
      };

    /*
    for (INT i = 0, theta = 0; i < N; i++, theta += PI / (N - 1))
      for (INT j = 0, phi = 0; j < M; j++, phi += 2 * PI / (M - 1))
      {

      }
    */

  } /* end of 'units' namespace */
} /* end of 'ivgl' namespace */

#endif /* __sphere_h_ */

/* END OF 'sphere.h' FILE */

