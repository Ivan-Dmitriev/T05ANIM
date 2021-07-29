/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : textures.cpp
 * PURPOSE     : Animation project.
 *               Primitive handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <fstream>

//#include "../resources.h"
#include "textures.h"

/* Project namespace */
namespace ivgl
{
  /* Add texture from image function.
   * ARGUMENTS:
   *   - image file name:
   *       const std::string TexName;
   *   - image width:
   *       INT W;
   *   - image hight:
   *       INT H;
   *   - components:
   *       INT C;
   *   - color bits:
   *       VOID *Bits;
   * RETURNS:
   *   (INT) new img stock number.
   */
  VOID texture::AddImg( const std::string TexName, INT w, INT h, INT C, VOID *Bits )
  {
    FLT mips;

    mips = w > h ? h : w;
    mips = log(mips) / log(2);
    if (mips < 1)
      mips = 1;

    /* Allocate texture space */
    glGenTextures(1, &TexId.first);
    glBindTexture(GL_TEXTURE_2D, TexId.first);

    /* Upload texture */
    ///gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
    glTexStorage2D(GL_TEXTURE_2D, mips, C == 3 ? GL_RGB8 : C == 4 ? GL_RGBA8 : GL_R8, w, h);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, Bits);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    Name = TexName;
    W = w;
    H = h;

    glBindTexture(GL_TEXTURE_2D, 0);
  } /* End of 'AddImg' function */

  /* Texture load from *.G32 or *.G24 file function.
   * ARGUMENTS:
   *   - Texture file name:
   *       const std::string FileName;
   * RETURNS:
   *   (INT) new img stock number.
   */
  texture * texture::LoadNewImage( const std::string FileName )
  {
    CHAR Buf[1000];
    DWORD d = 0;
    DWORD *img;
    INT w = 0, h = 0, ImageIdentifier = 0;

    sprintf(Buf, "bin/textures/%s", FileName.c_str());
    std::fstream f(Buf, std::fstream::in | std::fstream::binary);

    if (f.is_open())
    {
      f.read((CHAR *)&w, 2);
      f.read((CHAR *)&h, 2);
      if ((img = (DWORD *)malloc(4 * w * h)) == NULL)
      {
        f.close();
        return this;
      }
      f.seekg(0, std::fstream::end);
      INT flen = (INT)f.tellg();
      f.seekg(4, std::fstream::beg);
      // G24
      if (flen == 3 * w * h + 4)
        ImageIdentifier = 3;
      // G32
      else if (flen == 4 * w * h + 4)
        ImageIdentifier = 4;
      for (INT x = 0; x < w; x++)
        for (INT y = 0; y < h; y++)
        {
          f.read((CHAR *)&d, ImageIdentifier);
          d |= 0xFF000000;
          img[(h - y - 1) * w + x] = d;
        }
      AddImg(FileName, w, h, ImageIdentifier, img);

      //delete [] img;
      free(img);
      f.close();
      return this;
    }  
    return this;
  } /* End of 'texture::LoadG32' function */
  /* Free shader function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID texture::Free( VOID )
  {
    W = H = 0;
    Name = "None";
  } /* End of 'texture::Free' function */

} /* end of 'ivgl' namespace */

/* END OF 'textures.cpp' FILE */
