/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : prims.h
 * PURPOSE     : Animation project.
 *               Primitive collection handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __prims_h_
#define __prims_h_

#include <vector>

#include "../prim/prim.h"
#include "../resources.h"

#include "../../../../mth/mth.h"

/* Project namespace */
namespace ivgl
{
  /* Primitive handle class */
  class primitives 
  {
    friend class primitives_manager;
    template<typename entry_type, typename index_type>
      friend class resource_manager;
  public:
    INT NumOfPrims;
    matr Transform;
    std::vector<prim> Prims;
  public:
    primitives( VOID ) : NumOfPrims(0)
    {
    }

    /* Delete array of primitives function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );

    /* Load array of primitives from .G3DM file function.
     * ARGUMENTS:
     *   - pointer to primitives structure:
     *       primitives *Prs;
     *   - file name:
     *       CHAR *FileName;
     * RETURNS:
     *   (BOOL) TRUE if successful, FALSE otherwise.
     */
    BOOL Load( primitives *Prs, const std::string FileName );

  }; /* End of 'primitives' class */

  /* Primitive manager */
  class primitives_manager : public resource_manager<primitives, INT> 
  {
    primitives * PrimsCreate( primitives *Prs, const std::string FileName )
    {
      //return resource_manager::Add(primitives().(Tpl));
    } /* End of 'PrimsCreate' function */
      
    /* Free render primitives function.
      * ARGUMENTS:
      *   - Primitives to be deleted:
      *       primitives *Pr;
      * RETURNS: None.
      */
    VOID PrimsFree( primitives *Prs )
    {
      resource_manager::Delete(Prs);
    } /* End of 'PrimsFree' function */

    /* Class destructor */
    ~primitives_manager( VOID )
    {
      for (auto &pr : Stock)
        pr.second.Free();
    } /* End of 'destructor' function */
  }; /* End of 'primitives_manager' class */

  /* Unstructured data file reader class */
  class rdr
  {
  private:
    // Memory pointer reference
    BYTE *&Ptr;
 
  public:
    /* Class constructor.
      * ARGUMENTS:
      *   - reference to memory pointer:
      *       BYTE *&PtrRef;
      */
    rdr( BYTE *&PtrRef ) : Ptr(PtrRef)
    {
    } /* End of 'rdr' function */
 
    /* Read data function.
      * ARGUMENT:
      *   - read data pointer:
      *       type *Data;
      *   - read data count:
      *       INT Count;
      * RETURNS: None.
      */
    template<typename type>
      VOID operator()( type *Data, const INT Count = 1 )
      {
        if (Count == 1)
          *Data = *(type *)Ptr, Ptr += sizeof(type);
        else
          memcpy(Data, Ptr, sizeof(type) * Count), Ptr += sizeof(type) * Count;
      } /* End of 'operator()' function */
  }; /* End of 'rdr' class */
} /* end of 'ivgl' namespace */

#endif /* __shader_h_ */

/* END OF 'shader.h' FILE */
