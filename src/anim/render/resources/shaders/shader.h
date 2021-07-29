/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : shader.h
 * PURPOSE     : Animation project.
 *               Shader handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __shader_h_
#define __shader_h_

#include "../resources.h"

/* Project namespace */
namespace ivgl
{
  //class resource_manager<>;
  /* Shader class */
  class shader
  {
    friend class shader_manager;
    template<typename entry_type, typename index_type>
      friend class resource_manager;
    public:
    /* Save text to file function.
     * ARGUMENTS:
     *   - message file prefix, shader name and text:
     *       CHAR *Prefix*Text;
     * RETURNS: None.
     */
    VOID Log( const std::string &Stage, const std::string &Text );

    /* Load shader from file function.
     * ARGUMENTS:
     *   - Name of file:
     *       const std::string &FileName;
     * RETURNS: (shader & ) link on loaded shader.
     */
    shader & LoadFromFile( const std::string &FileName );

    /* Load text from file function.
     * ARGUMENTS:
     *   - Name of file:
     *       const std::string &FileName;
     * RETURNS: (shader) loaded text.
     */
    std::string LoadTextFromFile( const std::string &FileName );

    /* Load shader function.
     * ARGUMENTS: None
     * RETURNS: (shader & ) link on loaded shader.
     */
    shader & Load( VOID );

   /* Free shader function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID Free( VOID );
  public:
    /* Shader name*/
    std::string Name;
    /* Shader program Id */
    UINT ProgId;

    /* Class constructor */
    shader( VOID ) : ProgId(0)
    {
    }

    /* Class constructor */
    shader( const std::string &FileNamePrefix ) : ProgId(0)
    {
      Name = FileNamePrefix;
      //LoadFromFile(FileNamePrefix);
    }
    
    /* Class destructor */
    ~shader( VOID )
    {
    }

    /* Shader apply function.
     * ARGUMENTS: None.
     * RETURNS: (UINT) shader program id.
     */
    UINT Apply( VOID );
    /* Shader update function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Update( VOID );
    /* Shader uniform value set function.
     * ARGUMENTS:
     *   - uniform name:
     *       const CHAR *Name;
     *   - uniform value:
     *       const value_type &Value;
     * RETURNS: None.
     */
    template<typename value_type>
      VOID UniSet( const CHAR * Name, const value_type &Value )
      {
        INT loc;
 
        if ((loc = glGetUniformLocation(ProgId, Name)) != -1)
          if constexpr (std::is_convertible_v<FLT, value_type>)
            glUniform1f(loc, Value);
          else if constexpr (std::is_convertible_v<INT, value_type>)
            glUniform1i(loc, Value);
          else if constexpr (std::is_convertible_v<vec3, value_type>)
            glUniform3fv(loc, 1, Value);
          else if constexpr (std::is_convertible_v<matr, value_type>)
            glUniformMatrix4fv(loc, 1, FALSE, Value);
      } /* End of 'UniSet' function. */
    /* Shader uniform value set pair function.
     * ARGUMENTS:
     *   - uniform name:
     *       const CHAR *Name;
     *   - uniform value:
     *       const std::pair<value_type, INT> &Value;
     * RETURNS: None.
     */
    template<typename value_type>
      VOID UniSetPair( const CHAR * Name, const std::pair<value_type, INT> &Value )
      {
        if (Value.second > 0)
        {
          if constexpr (std::is_convertible_v<FLT, value_type>)
            glUniform1f(Value.second, Value.first);
          else if constexpr (std::is_convertible_v<INT, value_type>)
            glUniform1i(Value.second, Value.first);
          else if constexpr (std::is_convertible_v<vec3, value_type>)
            glUniform3fv(Value.second, 1, Value.first);
          else if constexpr (std::is_convertible_v<matr, value_type>)
            glUniformMatrix4fv(Value.second, 1, FALSE, Value.first);
        }
      } /* End of 'UniSetPair' function. */
    /* Shader uniform value set function.
     * ARGUMENTS:
     *   - uniform name:
     *       const CHAR *Name;
     *   - uniform value:
     *       const value_type &Value;
     * RETURNS: None.
     */
    template<typename value_type>
      VOID UniSetLoc( const CHAR * Name, const value_type &Value, INT Loc )
      {
        if (Loc != -1)
        {
          if constexpr (std::is_convertible_v<FLT, value_type>)
            glUniform1f(Loc, Value);
          else if constexpr (std::is_convertible_v<INT, value_type>)
            glUniform1i(Loc, Value);
          else if constexpr (std::is_convertible_v<vec3, value_type>)
            glUniform3fv(Loc, 1, Value);
          else if constexpr (std::is_convertible_v<matr, value_type>)
            glUniformMatrix4fv(Loc, 1, FALSE, Value);
        }
      } /* End of 'UniSetLoc' function. */
  }; /* End of 'shader' class */
  
  /* Shader manager class */
  class shader_manager : public resource_manager<shader, std::string>  
  {
    //friend class material_manager;
    friend class material;
  public:
   /* Create shader function.
    * ARGUMENTS:
    *   - Name of file:
    *       const std::string &FileName;
    * RETURNS: None.
    */
    shader * ShaderCreate( const std::string FileName )
    {
      shader *find = nullptr;
      shader shd(FileName);

      if ((find = Find(FileName)) != nullptr)
        return find;

      shd.LoadFromFile(FileName);
      return resource_manager::Add(shd);
      //return resource_manager::Add(shader(FileName));
    } /* End of 'Add' function. */

   /* Free shader function.
    * ARGUMENTS:
    *   - Shader to be deleted:
    *       shader *Shd;
    * RETURNS: None.
    */
    VOID ShaderFree( shader *Shd )
    {
    } /* End of 'ShaderFree' function. */
    /* Get shader by name function.
     * ARGUMENTS:
     *   - Name of file:
     *       const std::string ShdName;
     * RETURNS: None.
     */
    shader * GetShd( const std::string ShdName )
    {
      shader *find = nullptr;

      find = Find(ShdName);

      return find;
    } /* End of 'GetShd' function. */

    ~shader_manager( VOID )
    {
      for (auto &pr : Stock)
        pr.second.Free();
    }
    /* Update shaders function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateShaders( VOID )
    {
      for (auto &shd : Stock)
        shd.second.LoadFromFile(shd.second.Name);
    } /* End of 'UpdateShaders' function. */
  }; /* End of 'shader_manager' class */

} /* end of 'ivgl' namespace */

#endif /* __shader_h_ */

/* END OF 'shader.h' FILE */
