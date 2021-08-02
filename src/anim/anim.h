/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME   : anim.h
 * PURPOSE     : Animation project.
 *               Mathematics library.
 *               Space camera handle module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               ID3
 * LAST UPDATE : 23.07.2021
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
 
#ifndef __anim_h_
#define __anim_h_

#include <cstdio>
#include <map>

#include "../win/win.h"
#include "input.h"
#include "timer.h"
#include "render/render.h"

namespace ivgl 
{
  /* Forward declaraion of anim class */
  class anim;
  
  /* Unit class */
  class unit
  {
  public:
    /* Virtual destructor */
    virtual ~unit( VOID )
    {
    
    } /* End of 'destructor' function */

   /* Virtual render function.
    * ARGUMENTS:
    *   - Poiner on animation structure:
    *       anim *Ani;
    * RETURNS: None.  
    */
    virtual VOID Render( anim *Ani )
    {
    } /* End of 'Render' function */

   /* Virtual response function.
    * ARGUMENTS: 
    *   - Poiner on animation structure:
    *       anim *Ani;
    * RETURNS: None.  
    */
    virtual VOID Response( anim *Ani )
    {
    } /* End of 'Response' function */

  }; /* End of 'unit' class */

  /* General scene class */
  class scene : public unit
  {
  public:
    stock<unit *> Units;
  public:
    scene( VOID )
    {
    }
    ~scene( VOID ) override
    {
      Units.Walk([this]( unit * Uni )
                  {
                    delete Uni;
                  });
    } /* End of 'destructor' function */
    VOID Render( anim *Ani ) override
    {
      Units.Walk([Ani]( unit * Uni)
                  {
                    Uni->Render(Ani);
                  });
    } /* End of 'Render' function */
    VOID Response( anim *Ani ) override
    {
      Units.Walk([Ani]( unit * Uni)
                  {
                    Uni->Response(Ani);
                  });
    } /* End of 'Response' function */

   /* Add unit of scene to stock function.
    * ARGUMENTS: 
    *   - Unit to be add:
    *       unit *Uni;
    * RETURNS: (scene & ) link on result scene.
    */
    scene & operator<<( unit *Uni )
    {
      Units << Uni;
      //scene *s = dynamic_cast<scene *>();

      return *this;
      //Scene
    } /* End of 'operator<<' function */

  }; /* end of 'scene' class */

  /* Animation system class */
  class anim : public win, public timer, public input, public render
  {
  private:
    static anim Instance;

  public:
    std::map<std::string, unit * (*)( VOID )> UnitNames;

  /* Unit registration class */
  template<class UnitType>
    class unit_register
    {
    public:
      static unit * Create( VOID )
      {
        return new UnitType(&Instance);
      }
      unit_register( const std::string &Name )
      {
        Instance.UnitNames[Name] = Create;
      }
    }; /* End of 'unit_register' class */

  /* Add new unit by name function.
   * ARGUMENTS: None.
   * RETURNS: (anim &) self reference.
   */
  anim & operator<<( const std::string &Name )
  {
    if (UnitNames.find(Name) != UnitNames.end())
    {
      unit *uni = UnitNames[Name]();
 
      return *this << uni;
    }
  } /* End of 'operator<<' function */

  public:
    //scene MyScene;
    std::string Path;
    stock<unit *> Units;

   /* Get current directory function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID GetCurrentDir( VOID )
    {
      CHAR Buf[_MAX_PATH];
      GetCurrentDirectory(sizeof(Buf), Buf);
      anim::Path = Buf;
    } /* End of 'GetCurrentDir' function */

  private:
    //stock<unit *> MyScene;
    anim( VOID /*HINSTANCE hInst = GetModuleHandle(nullptr)*/ ) : win() , input(win::MouseWheel, win::hWnd), render(win::hWnd, W, H)
    {
      GetCurrentDir();
    }
  public:
  
    //unit *Scene;
    static anim & Get( VOID )
    {
      return Instance;
    }
    static anim * GetPtr( VOID )
    {
      return &Instance;
    }
    ~anim( VOID ) 
    {
      Units.Walk([]( unit *Uni ){ delete Uni; });;
    }
  public:
   /* Init function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID Init( VOID ) override
    {
    } /* End of 'Init' function */
   /* Close function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID Close( VOID ) override
    {
    } /* End of 'Close' function */
   /* Resize function.
    * ARGUMENTS: None.
    * RETURNS: None.
    */
    VOID Resize( VOID ) override
    {
      render::Resize();
      Render();
      CopyFrame();
      //MyScene.Response(this);
    } /* End of 'Resize' function */

    /* Paint function.
     * ARGUMENTS: 
     *   - Handle device context:
     *       HDC hDC;
     * RETURNS: None.
     */
    VOID Paint( HDC hDC )
    {
      //Rectangle(hDC, -1, -1, W, H);
      //SelectObject(hDC, GetStockObject(DC_BRUSH));
      //SetDCBrushColor(hDC, RGB(255, 0, 0));
      //Ellipse(hDC, -1, -1, MouseX - 10, MouseY - 10);
      //Ellipse(hDC, -1, -1, W - 10, H - 10);
    } /* End of 'Paint' function */

    /* Idle function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Idle( VOID ) override
    {
      //mouse::Response();
      //keyboard::Response();
      //timer::Response();
      //primitive Pr;
      //render::PrimLoad(&Pr, buf);
      Render();
      render::CopyFrame();
      //if (MyScene != nullptr)
      //MyScene.Render(this);

      //keyboard::Response();
    } /* End of 'Idle' function */

    /* Timer function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Timer( VOID ) override
    {
      Render();
      render::CopyFrame();
      //keyboard::Response();
      //timer::Response();

      /*
      CHAR Buf[100], Buf2[100];

      sprintf(Buf, "%.3f\n", FPS);
      SetWindowTextA(hWnd, Buf);
      if (KeysClick['F'])
        FlipFullScreen();

      if (Keys[VK_LMENU])
      {
        sprintf(Buf2, "%i\n", MouseZ);
        //OutputDebugStringA(Buf2);
      }
      */
    } /* End of 'Timer' function */

    /* Render function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Render( VOID )
    {
      input::Response();
      timer::Response();
      //MyScene->Response(this);
      //MyScene->Render(this);
      render::StartFrame();
      Units.Walk([this]( unit *Uni ){ Uni->Response(this); });
      Units.Walk([this]( unit *Uni ){ Uni->Render(this); });
      render::EndFrame();
    } /* End of 'Render' function */

    /* Add new unit function.
     * ARGUMENTS: 
     *   - Pointer to unit to be add:
     *       unit *Uni;
     * RETURNS: (anim &) link to result animation structure.
     */
    anim & operator<<( unit *Uni )
    {
      Units << Uni;
      return *this;
    } /* End of 'operator<<' function */

    /* Add new unit function.
     * ARGUMENTS: 
     *   - Pointer to unit to be add:
     *       unit *Uni;
     * RETURNS: (anim &) link to result animation structure.
     */
    anim & AddUnit( unit *Uni )
    {
      Units << Uni;
      return *this;
    } /* End of 'AddUnit' function */

  }; /* End of 'anim' class */
} /* end of 'ivgl' namespace */

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */
