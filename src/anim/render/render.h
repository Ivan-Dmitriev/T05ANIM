/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : render.h
 * PURPOSE     : Animation project.
 *               Render class implementation module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */


#ifndef __render_h_
#define __render_h_

#pragma comment(lib, "opengl32")

#define GLEW_STATIC
#include <glew.h>
#include <wglew.h>

#include "../../def.h"
#include "../../mth/mth.h"
#include "resources/resource_collection.h"

namespace ivgl
{
  /* Debug output function.
   * ARGUMENTS:
   *   - source APi or device:
   *      UINT Source;
   *   - error type:
   *      UINT Type;
   *   - error message id:
   *      UINT Id, 
   *   - message severity:
   *      UINT severity, 
   *   - message text length:
   *      INT Length, 
   *   - message text:
   *      CHAR *Message, 
   *   - user addon parameters pointer:
   *      VOID *UserParam;
   * RETURNS: None.
   */
  VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                               INT Length, const CHAR *Message, const VOID *UserParam );
  /* Render class */
  class render : public primitive_manager, public shader_manager, public material_manager, public texture_manager
  {
  private:
    HWND &hRndWnd;
 
    HDC hRndDC;
    HGLRC hRndGLRC;
  public:
    INT &FrameW, &FrameH;
 
  public:
    math::camera<FLT> Camera;
 
    /* --> anim() : win(), render(win::hWnd, ... ) */
    render( HWND &hWnd, INT &W, INT &H ) : hRndWnd(hWnd), FrameW(W), FrameH(H)
    {
      CurrentFill = fill::WIREFRAME;
      CurrentCS = cs::WORLD;

      INT i;
      UINT nums;
      PIXELFORMATDESCRIPTOR pfd = {0};
      HGLRC hRC;

      INT PixelAttribs[] =
      {
        WGL_DRAW_TO_WINDOW_ARB, TRUE,
        WGL_SUPPORT_OPENGL_ARB, TRUE,
        WGL_DOUBLE_BUFFER_ARB, TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 32,
        0
      };
      INT ContextAttribs[] =
      {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                      /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
        0
      };
      /* Store window and context handles */
      hRndDC = GetDC(hWnd);

      /* OpenGL init: pixel format setup */
      pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
      pfd.nVersion = 1;
      pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
      pfd.cColorBits = 32;
      pfd.cDepthBits = 32;
      i = ChoosePixelFormat(hRndDC, &pfd);
      DescribePixelFormat(hRndDC, i, sizeof(pfd), &pfd);
      SetPixelFormat(hRndDC, i, &pfd);

      /* OpenGL init: setup rendering context */
      hRndGLRC = wglCreateContext(hRndDC);
      wglMakeCurrent(hRndDC, hRndGLRC);

      /* Initialize GLEW library */
      if (glewInit() != GLEW_OK)
      {
        MessageBox(hWnd, L"Error OpenGL initialization", L"ERROR", MB_ICONERROR | MB_OK);
        exit(0);
      }
      if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
      {
        MessageBox(hRndWnd,L"Error: no shaders support", L"Error", MB_ICONERROR | MB_OK);
        exit(0);
      }
      /* Enable a new OpenGL profile support */
      wglChoosePixelFormatARB(hRndDC, PixelAttribs, NULL, 1, &i, &nums);
      hRC = wglCreateContextAttribsARB(hRndDC, NULL, ContextAttribs);

      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
      glDebugMessageCallback(glDebugOutput, NULL);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

      /*
      RndProjSize = 0.1;
      RndProjDist = RndProjSize;
      RndProjFarClip = 3000;
      RndFrameW = 30;
      RndFrameH = 30;
      RndCamSet(VecSet(70, 27, 70), VecSet(0, 0, 0), VecSet(0, 1, 0));
      */
      /* Set default render parameters */
      glClearColor(0.30, 0.47, 0.8, 1);
      //glEnable(GL_BLEND);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_PRIMITIVE_RESTART);
      glPrimitiveRestartIndex(-1);
      // Vertical Sync
      wglSwapIntervalEXT(0);
      //RndResInit();
      //RndTargetInit();
    }
    ~render( VOID )
    {
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext(hRndGLRC);
      ReleaseDC(hRndWnd, hRndDC);
    }
 
    VOID Resize( VOID );
    VOID StartFrame( VOID );
    VOID EndFrame( VOID );
    VOID CopyFrame( VOID );
 
    /* Draw render primitive function.
     * ARGUMENTS:
     *   - primitive:
     *      dg5PRIM *Pr;
     *   - matrix:
     *      MATR World;
     * RETURNS: None.
     */
    VOID PrimDraw( primitive *Pr, matr World );

    /* Draw render primitive function.
     * ARGUMENTS:
     *   - primitive:
     *      dg5PRIM *Pr;
     *   - matrix:
     *      MATR World;
     * RETURNS: None.
     */
    VOID PrimDraw( prim *Pr, matr World );

    /* Draw render primitives function.
     * ARGUMENTS:
     *   - primitive:
     *      primitives *Prs;
     *   - matrix:
     *      matr World;
     * RETURNS: None.
     */
    VOID PrimitivesDraw( primitives *Prs, matr World );

    enum struct cs
    {
      WORLD, SCREEN
    } CurrentCS;
 
    enum struct fill
    {
      WIREFRAME, SOLID
    } CurrentFill;
 
    VOID Set( cs NewCS );
    VOID Set( fill NewFill );
 
    VOID Push( cs NewCS );
    VOID Push( fill NewFill );
    VOID PopCS( VOID );
    VOID PopFill( VOID );
 
    //VOID Draw( const prim &Pr );
    //VOID Draw( const stock<prim> &Prs );  
  }; /* End of 'render' class */

} /* end of 'ivgl' namespace */


#endif /* __render_h_ */

/* END OF 'render.h' FILE */
