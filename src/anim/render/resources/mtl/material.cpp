/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : material.cpp
 * PURPOSE     : Animation project.
 *               Material class implementation module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3.
 * LAST UPDATE : 24.07.2021.
 * NOTE        : Module namespace 'ivgl'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "../textures/textures.h"
#include "material.h"
#include "../../../anim.h"

/* Project namespace */
namespace ivgl
{
  /* Material class constructor */
  material::material( vec3 NewKa, vec3 NewKd, vec3 NewKs, FLT NewPh, FLT NewTrans, const std::string NewName, 
            const std::string ShaderNewName )
  {
    Ka.first = NewKa;
    Kd.first = NewKd;
    Ks.first = NewKs;
    Ph.first = NewPh;
    Trans.first = NewTrans;
    Name = NewName;
 }
    /* Material update uniform location function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID material::UpdateLoc( VOID )
    {
      if (shd != nullptr)
      {
        Ka.second = glGetUniformLocation(shd->ProgId, "Ka");
        Kd.second = glGetUniformLocation(shd->ProgId, "Kd");
        Ks.second = glGetUniformLocation(shd->ProgId, "Ks");
        Ph.second = glGetUniformLocation(shd->ProgId, "Ph");
        Trans.second = glGetUniformLocation(shd->ProgId, "Trans");
        CamLocUni = glGetUniformLocation(shd->ProgId, "CamLoc");
        CamDirUni = glGetUniformLocation(shd->ProgId, "CamDir");
        CamRightUni  = glGetUniformLocation(shd->ProgId, "CamRight");
        CamUpUni = glGetUniformLocation(shd->ProgId, "CamUp");

        TimeUniLoc = glGetUniformLocation(shd->ProgId, "Time");
        MatrWInvLoc = glGetUniformLocation(shd->ProgId, "MatrVInv");
        MatrWPLoc = glGetUniformLocation(shd->ProgId, "MatrWP");
        MatrWVPLoc = glGetUniformLocation(shd->ProgId, "MatrWVP");

        /* Set textures */
        for (INT i = 0; i < 8; i++)
        {
          CHAR tname[] = "IsTexture0";

          tname[9] = '0' + i;
          IsTextures[i] = -1;
          if (Tex[i] != nullptr)
            IsTextures[i] = glGetUniformLocation(shd->ProgId, tname);
        }
      }
    }
 /* Material storage initializate function.
  * ARGUMENTS:
  *   - Material number:
  *       INT MtlNo;
  * RETURNS:
  *   (INT) shader program id.
  */
  VOID material::Apply( VOID )
  {
    anim *myanim = anim::GetPtr();

    //UINT prg = shd->ProgId;
    /* Set shading parameters */
    if (shd != nullptr && shd->Apply() != 0)
    {
      shd->UniSetPair("Ka", Ka);
      shd->UniSetPair("Kd", Kd);
      shd->UniSetPair("Ks", Ks);
      shd->UniSetPair("Ph", Ph);
      shd->UniSetPair("Trans", Trans);
      shd->UniSetLoc("CamLoc", myanim->Camera.Loc, CamLocUni);
      shd->UniSetLoc("CamDir", myanim->Camera.Dir, CamDirUni);
      shd->UniSetLoc("CamRight", myanim->Camera.Right, CamRightUni);
      shd->UniSetLoc("CamUp", myanim->Camera.Up, CamUpUni);

      //shd->UniSet("CamRight", myanim->Camera.Right);
      //shd->UniSet("CamDir", myanim->Camera.Dir);
      //shd->UniSet("CamAt", myanim->Camera.At);
      //shd->UniSetLoc("Time", myanim->Time, TimeUniLoc);

      /* Set textures */
      for (INT i = 0; i < 8; i++)
      {
        CHAR tname[] = "IsTexture0";

        tname[9] = '0' + i;
        if (Tex[i] != nullptr && Tex[i]->TexId.first != 0)
        {
          /* Activate sampler */
          glActiveTexture(GL_TEXTURE0 + i);
          /* Bind texture to sampler */
          glBindTexture(GL_TEXTURE_2D, Tex[i]->TexId.first);
        }
        if (Tex[i] != nullptr)
          if (glIsProgram(shd->ProgId))
            shd->UniSetLoc(tname, (Tex[i]->TexId.first != -1) && (Tex[i]->TexId.first != 0), IsTextures[i]);
      }
    }
  } /* End of 'material::Apply' function */
  material * material_manager::GetDef( VOID )
  {
    material Mtl;
    ivgl::anim *myanim = ivgl::anim::GetPtr();

    vec3 NewKa = vec3(0.19225,0.19225,0.19225);
    vec3 NewKd = vec3(0.50754,0.50754,0.50754);
    vec3 NewKs = vec3(0.508273,0.508273,0.508273);
    FLT NewPh = 51.2;
    FLT NewTrans = 1;
    //NewKa = vec3(0.25, 0.148, 0.06475);
    //NewKd = vec3(0.4, 0.2368, 0.1036);
    //NewKs = vec3(0.774597, 0.458561, 0.200621);

    //ShdNo = 0;
    std::string shd = "DEFAULT";
    //shd = ivgl::anim::GetPtr()->shader_manager::ShaderCreate("DEFAULT");
    std::string NewName = "silver";
    texture *txt1 = myanim->texture_manager::TexCreate("GIR.G32");
    //texture *txt2 = myanim->texture_manager::TexCreate("tree.G32");

    return myanim->material_manager::MtlCreate(NewKa, NewKd, NewKs, NewPh,
                                               NewTrans, NewName, shd, {txt1});
    //memset(Mtl.Tex, -1, sizeof(Mtl.Tex));
  } /* End of 'material::GetDef' function */

 /* Create material function.
  * ARGUMENTS:
  *   - Name of file:
  *       const std::string &FileName;
  * RETURNS: None.
  */
  material * material_manager::MtlCreate( vec3 NewKa, vec3 NewKd, vec3 NewKs, FLT NewPh, FLT NewTrans, 
                    const std::string MtlName, const std::string NewShdName, 
                      std::initializer_list<texture *> Textures = {} )
  {
    material *find = nullptr;
      
    if ((find = Find(MtlName)) != nullptr)
      return find;
    material mtl(NewKa, NewKd, NewKs, NewPh, NewTrans, MtlName, NewShdName);
    for (INT i = 0; i < 8; i++)
      mtl.Tex[i] = nullptr;
    INT i = 0;
    for (auto t : Textures)
      mtl.Tex[i++] = t;
    mtl.shd = ivgl::anim::GetPtr()->shader_manager::ShaderCreate(NewShdName);
    if (mtl.shd != nullptr)
      mtl.UpdateLoc();

    assert(mtl.shd != nullptr);
    return resource_manager::Add(mtl);
  } /* End of 'material_manager::MtlCreate' function */
  /* Create material function.
   * ARGUMENTS:
   *   - Material to be add:
   *       material mtl;
   * RETURNS: None.
   */
  material * material_manager::MtlCreate( material mtl )
  {
    material *find = nullptr;
      
    if ((find = Find(mtl.Name)) != nullptr)
      return find;
    mtl.UpdateLoc();

    return resource_manager::Add(mtl);
  } /* End of 'material_manager::AddMtl' function */

    /* Get material by name function.
     * ARGUMENTS:
     *   - Name of file:
     *       const std::string &FileName;
     * RETURNS: None.
     */
    material * material_manager::GetMtl( const std::string MtlName )
    {
      material *find = nullptr;
      
      find = Find(MtlName);

      return find;
    } /* End of 'material_manager::GetMtl' function */

} /* end of 'ivgl' namespace */

/* END OF 'material.cpp' FILE */


