/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : textures.h
 * PURPOSE     : Animation project.
 *               Textures handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __textures_h_
#define __textures_h_

#include "../resources.h"

/* Project namespace */
namespace ivgl
{
  /* Texture class */
  class texture
  {
    friend class texture_manager;
    template<typename entry_type, typename index_type>
      friend class resource_manager;
  public:
    std::string Name;       /* Texture name */
    INT W, H;               /* Texture size in pixels */
    std::pair<UINT, INT> TexId;             /* OpenGL texture Id */ 
  public:
    /* Class constructor */
    texture( INT NewW, INT NewH, const std::string NewName ) : W(NewW), H(NewH), Name(NewName)
    {
      TexId.first = 0;
    }
    /* Class constructor */
    texture( VOID ) : W(0), H(0)
    {
      TexId.first = 0;
    }
    /* Texture load from *.G32 or *.G24 file function.
     * ARGUMENTS:
     *   - Texture file name:
     *       CHAR *FileName;
     * RETURNS:
     *   (INT) new img stock number.
     */
    texture * LoadNewImage( const std::string FileName );
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
    VOID AddImg( const std::string Name, INT w, INT h, INT C, VOID *Bits );
    /* Free shader function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID );
  }; /* End of 'texture' class */

  /* Material manager class */
  class texture_manager : public resource_manager<texture, std::string>
  {
  public:
   /* Create texture function.
    * ARGUMENTS:
    *   - Name of file:
    *       const std::string &FileName;
    * RETURNS: None.
    */
    texture * TexCreate( const std::string FileName )
    {
      texture *find = nullptr;
      texture tex(0, 0, FileName);

      if ((find = Find(FileName)) != nullptr)
        return find;

      tex.LoadNewImage(FileName);
      return resource_manager::Add(tex);
    } /* End of 'TexCreate' function. */
   /* Create texture function.
    * ARGUMENTS:
    *   - Name of file:
    *       const std::string &FileName;
    * RETURNS: None.
    */
    texture * TexCreate( const std::string Name, INT w, INT h, INT C, VOID *Bits )
    {
      texture *find = nullptr;
      texture tex;

      if ((find = Find(Name)) != nullptr)
        return find;

      tex.AddImg(Name, w, h, C, Bits);
      return resource_manager::Add(tex);
    } /* End of 'TexCreate' function. */

    /* Get texture by name function.
     * ARGUMENTS:
     *   - Name of file:
     *       const std::string TexName;
     * RETURNS: None.
     */
    texture * GetTex( const std::string TexName )
    {
      texture *find = nullptr;
      
      find = Find(TexName);

      return find;
    } /* End of 'GetTex' function. */

    ~texture_manager( VOID )
    {
      for (auto &pr : Stock)
        pr.second.Free();
    }
  }; /* End of 'texture_manager' class */
} /* end of 'ivgl' namespace */

#endif /* __textures_h_ */

/* END OF 'textures.h' FILE */
