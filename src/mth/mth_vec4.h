/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : mth_vec3.h
 * PURPOSE     : Animation project.
 *               Mathematics library.
 *               Vec4 handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE :  23.07.2021
 * NOTE        : Module namespace 'mth'.
 * 
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */


#ifndef __mth_vec4_h_
#define __mth_vec4_h_

#include <iostream>

#include "mthdef.h"

namespace math
{
  template<typename Type>
    class vec4
    {
    private:
      Type X, Y, Z, W;
    public:
      vec4( Type NewX, Type NewY, Type NewZ, Type NewW  ) : X(NewX), Y(NewY), Z(NewZ), W(NewW)
      {
      } /* End of default constructor */

      /* Constructor of vec3 class function.
       * ARGUMENTS: None.
       * RETURNS: None.  
       */
      explicit vec4( Type NewV ) : X(NewV), Y(NewV), Z(NewV), W(NewV)
      {
      } /* End of default constructor */
      /* Constructor of vec3 class function.
       * ARGUMENTS: None.
       * RETURNS: None.  
       */
      explicit vec4( VOID )
      {
      } /* End of 'constructor' function */

    }; /* End of 'vec4' class */
} /* end of 'math' namespace */


#endif /* __mth_vec4_h_ */

/* END OF 'mth_vec4.h' FILE */