/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : topology.h
 * PURPOSE     : Animation project.
 *               Topology handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
 
#ifndef __topology_h_
#define __topology_h_

#include <string>
#include <vector>

#include "../../../../def.h"

/* Project namespace */
namespace ivgl
{
  class prim;

  /* Primitive shape representation type */
  enum struct prim_type 
  {
    TRIMESH,  /* Triangle mesh - array of triangles */
    TRISTRIP, /* Triangle strip - array of stripped triangles */
    LINES,    /* Line segments (by 2 points) */
    POINTS,   /* Array of points */
    PATH      /* Path of points */
  };

  /* Topology structures namespase */
  namespace topology
  {
    /* Base topology class */
    template<typename vertex_type>
      class base
      {
        friend class prim;
      protected:
        prim_type Type = prim_type::TRIMESH;
 
        /* Vertex array */
        std::vector<vertex_type> V;
        /* Index array */
        std::vector<INT> I;
 
      public:
        /* Class default constructor */
        base( VOID )
        {
        } /* End of 'base' function */
 
        /* Class constructor.
         * ARGUMENTS:
         *   - primitive type:
         *       prim_type NewType;
         *   - vertex array:
         *       const std::vector<vertex_type> &NewV;
         *   - index array:
         *       const std::vector<INT> &NewI;
         */
        base( prim_type NewType,
              const std::vector<vertex_type> &NewV = {},
              const std::vector<INT> &NewI = {} ) : Type(NewType), V(NewV), I(NewI)
        {
        } /* End of 'base' function */
      };
 
    /* Base topology class */
    template<typename vertex_type>
      class trimesh : public base<vertex_type>
      {
      public:
        /* Class default constructor */
        trimesh( VOID )
        {
        } /* End of 'trimesh' function */

        /* Class constructor.
         * ARGUMENTS:
         *   - vertex array:
         *       const std::vector<vertex> &NewV;
         *   - index array:
         *       const std::vector<INT> &NewI;
         */
        trimesh( const std::vector<vertex_type> &NewV,
                 const std::vector<INT> &NewI = {} ) : base<vertex_type>(prim_type::TRIMESH, NewV, NewI)
        {
        } /* End of 'trimesh' function */
        /* Evaluate trimesh vertex normals function.
         * ARGUMENTS: None.
         * RETURNS: None.
         */
        VOID EvalNormals( VOID )
        {
          for (INT i = 0; i < base<vertex_type>::I.size(); i += 3)   // may be i = 3
          {
            vec3 N = vec3((base<vertex_type>::V[base<vertex_type>::I[i + 1]].P -
              base<vertex_type>::V[base<vertex_type>::I[i]].P) %
                (base<vertex_type>::V[base<vertex_type>::I[i + 2]].P -
                  base<vertex_type>::V[base<vertex_type>::I[i]].P)).Normalizing();

            base<vertex_type>::V[base<vertex_type>::I[i + 0]].N = vec3(base<vertex_type>::V[base<vertex_type>::I[i + 0]].N + N).Normalizing();
            base<vertex_type>::V[base<vertex_type>::I[i + 1]].N = vec3(base<vertex_type>::V[base<vertex_type>::I[i + 1]].N + N).Normalizing();
            base<vertex_type>::V[base<vertex_type>::I[i + 2]].N = vec3(base<vertex_type>::V[base<vertex_type>::I[i + 2]].N + N).Normalizing();
            base<vertex_type>::V[base<vertex_type>::I[i + 0]].C =
              vec4(base<vertex_type>::V[base<vertex_type>::I[i + 0]].N[0],
                   base<vertex_type>::V[base<vertex_type>::I[i + 0]].N[1],
                   base<vertex_type>::V[base<vertex_type>::I[i + 0]].N[2],
                    1);
            base<vertex_type>::V[base<vertex_type>::I[i + 1]].C = 
              vec4(base<vertex_type>::V[base<vertex_type>::I[i + 1]].N[0],
                   base<vertex_type>::V[base<vertex_type>::I[i + 1]].N[1],
                   base<vertex_type>::V[base<vertex_type>::I[i + 1]].N[2],
                    1);
            base<vertex_type>::V[base<vertex_type>::I[i + 2]].C =
              vec4(base<vertex_type>::V[base<vertex_type>::I[i + 2]].N[0], 
                   base<vertex_type>::V[base<vertex_type>::I[i + 2]].N[1],
                   base<vertex_type>::V[base<vertex_type>::I[i + 2]].N[2], 
                    1);

          }

        }
      }; /* End of 'trimesh' class */

    template<typename vertex_type>
      class cube : public base<vertex_type>
      {
      public:
        /* Class constructor.
         * ARGUMENTS:
         *   - cube size:
         *       vec3 MinB, MaxB;
         */
        cube( vec3 MinB, vec3 MaxB ) : base<vertex_type>(prim_type::TRIMESH)
        {
          this->V.resize(24);
          this->I.resize(36);
          //vertex_type *MyVectors;
          //MyVectors = new vertex_type[24];
          vertex_type coords[24];
          vec3 v7(MinB[0], MinB[1], MinB[2]);
          vec3 v6(MinB[0], MaxB[1], MinB[2]);
          vec3 v5(MaxB[0], MaxB[1], MinB[2]);
          vec3 v4(MaxB[0], MinB[1], MinB[2]);
          vec3 v3(MaxB[0], MinB[1], MaxB[2]);
          vec3 v2(MinB[0], MinB[1], MaxB[2]);
          vec3 v1(MinB[0], MaxB[1], MaxB[2]);
          vec3 v0(MaxB[0], MaxB[1], MaxB[2]);

          vec3 nfront(0.0, 0.0, 1.0);
          vec3 nright(1.0, 0.0, 0.0);
          vec3 ntop(0.0, 1.0, 0.0);
          vec3 nleft(-1.0, 0.0, 0.0);
          vec3 nbottom(0.0, -1.0, 0.0);
          vec3 nback(0.0, 0.0, -1.0);

          //this7->V.pushback(MinB);
          coords[0].P = v0;
          coords[1].P = v1;
          coords[2].P = v2;
          coords[3].P = v3;
                          
          coords[4].P = v0;
          coords[5].P = v3;
          coords[6].P = v4;
          coords[7].P = v5;

          coords[8].P = v0;
          coords[9].P = v5;
          coords[10].P = v6;
          coords[11].P = v1;

          coords[12].P = v1;
          coords[13].P = v6;
          coords[14].P = v7;
          coords[15].P = v2;

          coords[16].P = v7;
          coords[17].P = v4;
          coords[18].P = v3;
          coords[19].P = v2;

          coords[20].P = v4;
          coords[21].P = v7;
          coords[22].P = v6;
          coords[23].P = v5;

          coords[0].N = nfront;
          coords[1].N = nfront;
          coords[2].N = nfront;
          coords[3].N = nfront;

          coords[4].N = nright;
          coords[5].N = nright;
          coords[6].N = nright;
          coords[7].N = nright;

          coords[8].N  = ntop;
          coords[9].N  = ntop;
          coords[10].N = ntop;
          coords[11].N = ntop;

          coords[12].N = nleft;
          coords[13].N = nleft;
          coords[14].N = nleft;
          coords[15].N = nleft;

          coords[16].N = nbottom;
          coords[17].N = nbottom;
          coords[18].N = nbottom;
          coords[19].N = nbottom;

          coords[20].N = nback;
          coords[21].N = nback;
          coords[22].N = nback;
          coords[23].N = nback;

          coords[0].T = vec2(1.0, 0.0);
          coords[1].T = vec2(0.0, 0.0);
          coords[2].T = vec2(0.0, 1.0);
          coords[3].T = vec2(1.0, 1.0);

          coords[4].T = vec2(0.0, 0.0);
          coords[5].T = vec2(0.0, 1.0);
          coords[6].T = vec2(1.0, 1.0);
          coords[7].T = vec2(1.0, 0.0);

          coords[8].T =  vec2(1.0, 1.0);
          coords[9].T =  vec2(1.0, 0.0);
          coords[10].T = vec2(0.0, 0.0);
          coords[11].T = vec2(0.0, 1.0);

          coords[12].T = vec2(1.0, 0.0);
          coords[13].T = vec2(0.0, 0.0);
          coords[14].T = vec2(0.0, 1.0);
          coords[15].T = vec2(1.0, 1.0);

          coords[16].T = vec2(0.0, 1.0);
          coords[17].T = vec2(1.0, 1.0);
          coords[18].T = vec2(1.0, 0.0);
          coords[19].T = vec2(0.0, 0.0);

          coords[20].T = vec2(0.0, 1.0);
          coords[21].T = vec2(1.0, 1.0);
          coords[22].T = vec2(1.0, 0.0);
          coords[23].T = vec2(0.0, 0.0);

          /*
          for (INT i = 0; i < 24; i++)
          {
            if ((i + 7) > 24)
              continue;
            vec3 res(0);
            res = coords[i + 1].N + coords[i + 7].N + coords[i + 3].N;
            coords[i].N = res.Normalizing(); 
          }
          */
          /*
          for (INT i = 0; i < 24; i++)
            coords[i].C = vec4(math::Rnd1F(), math::Rnd1F(), math::Rnd1F(), 1.0);
          */

          for (INT i = 0; i < 24; i++)
            this->V[i] = coords[i];
          UINT index[] = 
          {
            0, 1, 2,     2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
            4, 5, 6,     6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
            8, 9, 10,    10, 11, 8,    // v0-v5-v6, v6-v1-v0 (top)
            12, 13, 14,  14, 15, 12,     // v1-v6-v7, v7-v2-v1 (left)
            16, 17, 18,  18, 19, 16,     // v7-v4-v3, v3-v2-v7 (bottom)
            20, 21, 22,  22, 23, 20      // v4-v7-v6, v6-v5-v4 (back)
          };
          for (INT i = 0; i < 36; i++)
            this->I[i] = index[i];
          //delete [] MyVectors;
        } /* End of 'cube' function */
        /* Class constructor.
         * ARGUMENTS:
         *   - vertex array:
         *       const std::vector<vertex> &NewV;
         *   - index array:
         *       const std::vector<INT> &NewI;
         */
        cube( const std::vector<vertex_type> &NewV,
                 const std::vector<INT> &NewI = {} ) : base<vertex_type>(prim_type::TRIMESH, NewV, NewI)
        {
        } /* End of 'cube' function */

      }; /* End of 'cube' class */
    template<typename vertex_type>
      class points : public base<vertex_type>
      {
      public:
        points( const std::vector<vertex_type> &NewV,
                const std::vector<INT> &NewI = {} ) : base<vertex_type>(prim_type::POINTS, NewV, NewI)
        {
        }
      }; /* End of 'points' class */

    template<typename vertex_type>
      class grid : public base<vertex_type>
      {
      protected:
        /* Grid size */
        INT W, H;
 
        /* Class default constructor */
        grid( VOID )
        {
        } /* End of 'grid' function */
      public:
        /* Class constructor.
         * ARGUMENTS:
         *   - grid size:
         *       INT NewW, NewH;
         */
        grid( INT NewW, INT NewH ) : W(NewW), H(NewH), base<vertex_type>(prim_type::TRISTRIP)
        {
          /* Setting up vertices */
          this->V.resize((W * 2 + 1) * (H - 1));
          auto p = this->V.begin();
          for (INT i = 0; i < H; i++)
            for (INT j = 0; j < W; j++)
              p->T = vec2(j / (W - 1.0), i / (H - 1.0));
          /* Setting up indices */
          this->I.resize((W * 2 + 1) * (H - 1));
          for (INT k = 0, i = 0; i < H - 1; i++)
          {
            for (INT j = 0; j < W; j++)
            {
              this->I[k++] = (i + 1) * W + j;
              this->I[k++] = i * W + j;
            }
            if (i != H - 1)
             this->I[k++] = -1;

          }
        } /* End of 'grid' function */

        /* Obtain grid width function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (INT) grid width.
         */
        INT GetW( VOID ) const
        {
          return W;
        } /* End of 'GetW' function */
 
        /* Obtain grid height function.
         * ARGUMENTS: None.
         * RETURNS:
         *   (INT) grid height.
         */
        INT GetH( VOID ) const
        {
          return H;
        } /* End of 'GetH' function */
 
        /* Grid 2D array vertex access function.
         * ARGUMENTS:
         *   - row index:
         *       INT Row;
         * RETURNS:
         *   (vertex *) vertex row pointer.
         */
        vertex_type * operator[]( INT Row )
        {
          assert(Row >= 0 && Row < H);
          return &this->V[Row * W];
        } /* End of 'operator[]' function */ 
      };
  } /* end of 'topology' namespace */
 
  /* Vertex collection namespace */
  namespace vertex
  {
    /* Standard vertex representation type */
    struct std
    {
      vec3 P;  /* Vertex position */
      vec2 T;  /* Vertex texture coordinates */
      vec3 N;  /* Normal at vertex */
      vec4 C;  /* Vertex color */
 
      /* Vertex content description string */
      static const ::std::string Description;
 
      /* Class default constructor */
      std( VOID ) : P(vec3(0)), T(vec2(0)), N(vec3(0)), C(vec4(0))
      {
      } /* End of 'std' function */
 
      /* Class constructor.
       * ARGUMENTS:
       *   - new vertex position:
       *       const vec &NewP;
       *   - new vertex texture coordinates:
       *       const vec2 &NewT;
       *   - new vertex normal:
       *       const vec &NewN;
       *   - new vertex color:
       *       const vec4 &NewC;
       */
      std( const vec3 &NewP,
           const vec2 &NewT = vec2(0, 0),
           const vec3 &NewN = vec3(0, 1, 0),
           const vec4 &NewC = vec4(1) ) :
        P(NewP), T(NewT), N(NewN), C(NewC)
      {
      } /* End of 'std' function */
    };
  } /* end of 'vertex' namespace */
} /* end of 'ivgl' namespace */

#endif /* __topology_h_ */

/* END OF 'topology.h' FILE */
