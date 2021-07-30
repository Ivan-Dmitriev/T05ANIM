/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : shader.c
 * PURPOSE     : Animation project.
 *               Shader handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <fstream> 

#include "../../../anim.h"
#include "shader.h"

/* Project namespace */
namespace ivgl
{
 /* Store log to file function.
  * ARGUMENTS:
  *   - message file prefix, shader name and text:
  *       CHAR *Prefix*Text;
  * RETURNS: None.
  */
  VOID shader::Log( const std::string &Stage, const std::string &Text )
  {
    std::ofstream( /*ivgl::anim::GetPtr()->Path + */ "BIN/SHADERS/~OP{S}30.LOG", std::ios_base::app) <<
      Stage << ":\n" << Text << "\n";
  }
 /* Load shader from file function.
  * ARGUMENTS:
  *   - Name of file:
  *       const std::string &FileName;
  * RETURNS: (shader) loaded text.
  */
  std::string shader::LoadTextFromFile( const std::string & FileName )
  {
    std::ifstream f( /*ivgl::anim::GetPtr()->Path +*/ FileName);

    return std::string((std::istreambuf_iterator<char>(f)),
                        std::istreambuf_iterator<char>());

  } /* End of 'shader::LoadFromFile' function */
  shader & ivgl::shader::LoadFromFile( const std::string & FileName )
  {
    Free();

    struct
    {
      std::string NameS; /* Shader file prefix name (e.g. "VERT") */
      INT Type;   /* Shader OpenFL type (e.g. GL_VERTEX_SHADER) */
      UINT Id;     /* Obtained shader Id from OpenGL */
    } shdr[] =
    {
      {"VERT", GL_VERTEX_SHADER, 0},
      {"FRAG", GL_FRAGMENT_SHADER, 0},
      {"CTRL", GL_TESS_CONTROL_SHADER, 0},
      {"EVAL", GL_TESS_EVALUATION_SHADER, 0},
      {"GEOM", GL_GEOMETRY_SHADER, 0},
    };
    //INT NoofS = sizeof(shd) / sizeof(shd[0]), i, res, prg = 0;
    INT res;
    //CHAR *txt;
    BOOL is_ok = TRUE;
    static CHAR Buf[1000], Buf2[1000];

    for (auto &s : shdr)
    {
      if ((s.Id = glCreateShader(s.Type)) == 0)
      {
        Log(s.NameS, "Error create shader");
        is_ok = FALSE;
        break;
      }
      sprintf(Buf2, "bin/shaders/%s/%s.GLSL", FileName.c_str(), s.NameS.c_str());
      std::string txt = LoadTextFromFile(Buf2);
      if (txt.length() == 0)
        if (s.NameS != "VERT" && s.NameS != "FRAG")
        {
          glDeleteShader(s.Id);
          s.Id = 0;
          continue;
        }
        else
        {
          Log(s.NameS, "No file");
          is_ok = FALSE;
          break;
        }
      const CHAR * Src[] = {txt.c_str()};
      glShaderSource(s.Id, 1, Src, NULL);
      //free(Src);

      glCompileShader(s.Id);
      glGetShaderiv(s.Id, GL_COMPILE_STATUS, &res);
      if (res != 1)
      {
        glGetShaderInfoLog(s.Id, sizeof(Buf), &res, Buf);
        Log(s.NameS, Buf);
        is_ok = FALSE;
        break;
      }
    }
    if (is_ok)
      if ((ProgId = glCreateProgram()) == 0)
        is_ok = FALSE;
      else
      {
        for (auto s : shdr)
          if (s.Id != 0)
            glAttachShader(ProgId, s.Id);
        /* Link shader program */
        glLinkProgram(ProgId);
        glGetProgramiv(ProgId, GL_LINK_STATUS, &res);
        if (res != 1)
        {
          glGetProgramInfoLog(ProgId, sizeof(Buf), &res, Buf);
          Log("LINK", Buf);
          is_ok = FALSE;
        }
      }
      if (!is_ok)
      {
        for (auto s : shdr)
        {
          if (s.Id != 0)
            if (ProgId != 0)
            glDetachShader(ProgId, s.Id);
         glDeleteShader(s.Id);
        }
        if (ProgId != 0)
          glDeleteShader(ProgId);
        ProgId = 0;
      }
    return *this;
  } /* End of 'ShdLoad' function */
   /* Free shader function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID shader::Free( VOID )
    {
      INT n; 
      UINT shdrs[5];

      if (ProgId == 0)
        return;

      glGetAttachedShaders(ProgId, 5, &n, shdrs);
      for (INT i = 0; i < n; i++)
      {
        glDetachShader(ProgId, shdrs[i]);
        glDeleteShader(shdrs[i]);
      }
      glDeleteProgram(ProgId);
    } /* End of 'shader::Free' function */
    /* Shader apply function.
     * ARGUMENTS: None.
     * RETURNS: (UINT) shader program id.
     */
    UINT shader::Apply( VOID )
    {
      if (glIsProgram(ProgId))
      {
        glUseProgram(ProgId);
        return ProgId;
      }
      return 0;
    } /* End of 'shader::Apply' function */

    /* Shader update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID shader::Update( VOID )
    {
    } /* End of 'shader::Update' function */

} /* end of 'ivgl' namespace */

/* END OF 'shader.cpp' FILE */
