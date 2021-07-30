/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : material.h
 * PURPOSE     : Animation project.
 *               Material class declaration module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3.
 * LAST UPDATE : 24.07.2021.
 * NOTE        : Module namespace 'ivgl'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __material_h_
#define __material_h_

#include "../../../../def.h"
#include "../../resources/resources.h"
#include "../shaders/shader.h"
#include "../textures/textures.h"

//#include "../../resources/resource_collection.h"

/* Project namespace */
namespace ivgl
{
  /* Material class */
  class material : public shader_manager
  {
    friend class material_manager;
    template<typename entry_type, typename index_type>
      friend class resource_manager;

  public:
    std::string Name;
    std::pair <vec3, INT> 
      Ka,                         /* Ambient coefficient */
      Kd,                         /* Diffuse coefficient */
      Ks;                         /* Specular coefficient */
    std::pair <FLT, INT> Ph;      /* Phong power coefficient */
    std::pair <FLT, INT> Trans;   /* Transparency factor */
    INT CamLocUni = -1;
    INT CamRightUni = -1;
    INT CamUpUni = -1;
    INT CamDirUni = -1;

    INT TimeUniLoc = -1;
    INT MatrWVPLoc = -1;;
    INT MatrWPLoc = -1;
    INT MatrWInvLoc = -1;;

    texture *Tex[8];              /* Texture references from texture table (or -1) */
    INT IsTextures[8];
    shader *shd;
  public:
    /* Material class constructor */
    material( VOID )  
    {
      Ka.first = vec3(0);
      Kd.first = vec3(0);
      Ks.first = vec3(0);
      Ph.first = 0; 
    }

    /* Material class constructor */
    material( const std::string NewName ) : Name(NewName)
    {
    }
    /* Material class constructor */
    material( vec3 NewKa, vec3 NewKd, vec3 NewKs, FLT NewPh, FLT NewTrans, const std::string NewName, 
              const std::string ShaderNewName );
    /* Material storage initializate function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Apply( VOID );

    /* Material update uniform location function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateLoc( VOID );

   /* Free shader function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID Free( VOID )
    {
    }

  }; /* End of 'Material' class */
  /* Material manager class */
  class material_manager : public resource_manager<material, std::string>
  {
  public:
   /* Create material function.
    * ARGUMENTS:
    *   - Name of file:
    *       const std::string &FileName;
    * RETURNS: None.
    */
    material * MtlCreate( vec3 NewKa, vec3 NewKd, vec3 NewKs, FLT NewPh, FLT NewTrans,
                       const std::string MtlName, const std::string NewShdName, 
                        std::initializer_list<texture *> Textures );
   /* Create material function.
    * ARGUMENTS:
    *   - Material to be add:
    *       material mtl;
    * RETURNS: None.
    */
    material * MtlCreate( material mtl );

    /* Get default material function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    material * GetDef( VOID );
    /* Get material by name function.
     * ARGUMENTS:
     *   - Name of file:
     *       const std::string &FileName;
     * RETURNS: None.
     */
    material * GetMtl( const std::string MtlName ); 

    /*
    ~material_manager( VOID )
    {
      for (auto &pr : Stock)
        pr.second.Free();
    }
    */
  }; /* End of 'material_manager' class */
} /* end of 'ivgl' namespace */

#endif /* __material_h_ */

/* END OF 'material.h' FILE */
