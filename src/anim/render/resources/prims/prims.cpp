/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : prims.cpp
 * PURPOSE     : Animation project.
 *               Primitive collection handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <fstream>

#include "prims.h"
#include "../resource_collection.h"
#include "../topo/topology.h"
#include "../../../anim.h"

/* Project namespace */
namespace ivgl
{
  /* Delete array of primitives function.
   * ARGUMENTS:
   *   - pointer to primitives structure:
   *       primitives *Prs;
   * RETURNS: None.
   */
  VOID primitives::Free( VOID )
  {
    for (INT i = 0; i < NumOfPrims; i++)
       Prims[i].Free();
    //Prs->Prims.clear();
  }

  /* Load array of primitives from .G3DM file function.
   * ARGUMENTS:
   *   - pointer to primitives structure:
   *       primitives *Prs;
   *   - file name:
   *       CHAR *FileName;
   * RETURNS:
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL primitives::Load( primitives *Prs, const std::string FileName )
  {
    Free();

    BYTE *mem, *ptr;
    INT flen;
    //FILE *F;
    DWORD Sign;         /* == "G3DM" */
    INT NumOfPrims;
    INT NumOfMaterials;
    INT *MtlNums;
    INT NumOfTextures;
    struct tagG3DMMTL
    {
      CHAR Name[300];     /* Material name */

      /* Illumination coefficients */
      vec3 Ka, Kd, Ks;     /* Ambient, diffuse, specular coefficients */
      FLT Ph;              /* Phong power coefficient – shininess */
      FLT Trans;           /* Transparency factor */
      DWORD Tex[8];        /* Texture references 
                            * (8 time: texture number in G3DM file, -1 if no texture) */

      /* Shader information */
      CHAR ShaderString[300]; /* Additional shader information */
      DWORD Shader;       /* Shader number (uses after load into memory) */
    } *mtls;
    struct tagTEX1
    {
      CHAR Name[300]; /* Texture name */
      DWORD W, H;     /* Texture image size in pixels */
      DWORD C;        /* Texture image components (1-mono, 3-bgr or 4-bgra) */
     } *tex, *AllTex;
    struct tagTEXN
    {
      INT Num[8];
    } *Textures;

    //memset(Prs, 0, sizeof(primitives));
    /* Open file */
    
    std::fstream f(FileName, std::fstream::in | std::fstream::binary);
    if (!(f.is_open()))
      return FALSE;

    /////* Obtain file length */
    f.seekg(0, std::fstream::end);
    flen = (INT)f.tellg();
    f.seekg(0, std::fstream::beg);
    //f.close();
    
    try
    {
      mem = new BYTE[flen];
    }
    catch (std::bad_alloc& my_bad_alloc)
    {
      std::cerr << "bad_alloc caught: " << my_bad_alloc.what() << '\n';
      f.close();
      return FALSE;
    }
    //delete [] mem;

    f.read((CHAR *)mem, flen);
    f.close();
    ptr = mem;
    rdr rd(ptr);
    rd(&Sign);
    if (Sign != *(DWORD *)"G3DM")
    {
      delete [] mem;
      //delete [] ptr;
      return FALSE;
    }
    rd(&NumOfPrims);
    rd(&NumOfMaterials);
    rd(&NumOfTextures);
    Prims.resize(NumOfPrims);
    Prs->NumOfPrims = NumOfPrims;
    Prs->Transform = matr().Identity();
    //if (NumOfPrims > 0)
    MtlNums = new INT[NumOfPrims];
    AllTex = new tagTEX1[NumOfTextures];
    Textures = new tagTEXN[NumOfMaterials];

    ivgl::anim *myanim = ivgl::anim::GetPtr();

    /* Primitives */
    
    for (INT p = 0; p < NumOfPrims; p++)
    {
      DWORD NumOfVertexes;
      DWORD NumOfFacetIndexes;  /* num of facets * 3 */
      DWORD MtlNo;              /* Material number in table below (in material section) */
      vertex::std *NewV;
      INT *NewInd;

      rd(&NumOfVertexes);
      rd(&NumOfFacetIndexes);
      rd(&MtlNo);
      std::vector<vertex::std> V(NumOfVertexes);
      std::vector<INT> Ind(NumOfFacetIndexes);
      //rd(&NewV);
      //rd(&NewInd);
      NewV = (vertex::std *)ptr, ptr += sizeof(vertex::std) * NumOfVertexes;
      NewInd = (INT *)ptr, ptr += sizeof(INT) * NumOfFacetIndexes;

      for (INT i = 0; i < NumOfVertexes; i++)
        V[i] = NewV[i];
      for (INT i = 0; i < NumOfFacetIndexes; i++)
        Ind[i] = NewInd[i];

      topology::trimesh<vertex::std> T(V, Ind);

      topology::base<vertex::std> *B = dynamic_cast<topology::base<vertex::std> *>(&T);
      /* create primitive */
      Prs->Prims[p].Create(*B);

      MtlNums[p] = MtlNo;
    }
    //material 
    ///* Materials */
    //rd(&mtls);
    mtls = (struct tagG3DMMTL *)ptr, ptr += sizeof(struct tagG3DMMTL) * NumOfMaterials;

    for (INT m = 0; m < NumOfMaterials; m++)
    {
      INT i;
      material mtl;

      mtl.Name = mtls[m].Name;
      mtl.Ka.first = mtls[m].Ka;
      mtl.Kd.first = mtls[m].Kd;
      mtl.Ks.first = mtls[m].Ks;
      mtl.Ph.first = mtls[m].Ph;
      mtl.Trans.first = mtls[m].Trans;
      //mtl.shd = mtl.GetShd("DEFAULT");

      for (i = 0; i < 8; i++)
        Textures[m].Num[i] = (mtls[m].Tex[i] == -1 ? -1 : mtls[m].Tex[i]);

      myanim->material_manager::MtlCreate(mtl);
    }
    for (INT TexNo = 0; TexNo < NumOfTextures; TexNo++)
    {
      tex = new tagTEX1;
      rd(tex);
      AllTex[TexNo] = *tex;
      //ptr += sizeof(*tex);
      myanim->texture_manager::TexCreate(tex->Name, tex->W, tex->H, tex->C, ptr);
      ptr += tex->W * tex->H * tex->C;
      delete tex;
    }

    for (INT p = 0; p < NumOfPrims; p++)
    {
      Prs->Prims[p].Mtl = myanim->material_manager::Find(mtls[MtlNums[p]].Name);
      Prs->Prims[p].Mtl->shd = myanim->shader_manager::ShaderCreate("DEFAULT");
      Prs->Prims[p].Mtl->UpdateLoc();

      for (INT m = 0; m < NumOfMaterials; m++)
        for (INT t = 0; t < NumOfTextures; t++)
        {
          INT Num = mtls[MtlNums[p]].Tex[Textures[m].Num[t]];

          if (Num < 0 || Num > 7)
            continue;
          tagTEX1 Tex0 = AllTex[Num];
          Prs->Prims[p].Mtl->Tex[t] = myanim->texture_manager::TexCreate(Tex0.Name);
        }
    }
    delete [] mem;
    delete [] MtlNums;
    delete [] AllTex;
    delete [] Textures;

    return TRUE;
  } /* End of 'primitives::Load' function */
} /* end of 'ivgl' namespace */

/* END OF 'prims.cpp' FILE */
