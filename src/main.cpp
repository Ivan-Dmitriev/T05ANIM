/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

/* FILE NAME    : main.cpp
 * PURPOSE      : Animation project.
 *                Entry point.
 * PROGRAMMER   : CGSG-SummerCamp'2021.
 *                ID3.
 * LAST UPDATE  : 23.07.2021.
 * NOTE         : Module namespace 'ivgl'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include <iostream>

#include "mth/mth.h"
#include "win/win.h"
#include "win/win.h"
#include "anim/anim.h"
#include "def.h"

 /* The main program function.
  * ARGUMENTS:
  *   - handle of application instance:
  *       HINSTANCE hInstance;
  *   - dummy handle of previous application instance (not used):
  *       HINSTANCE hPrevInstance;
  *   - command line string:
  *       CHAR *CmdLine;
  *   - show window command parameter (see SW_***):
  *       INT CmdShow;
  * RETURNS:
  *   (INT) Error level for operation system (0 for success).
  */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  /*
  ivgl::matr m(1, 3, 13, 14,
         3, 47, 16, 17,
         30, 8, 9, 10,
         45, 34, 123, 12);
  */
  //ivgl::matr asd;
  //asd = m.Transpose();
  //ivgl::win myw;
  ivgl::anim &myw = ivgl::anim::Get();
  //ivgl::units::test_unit MyUnit;
  //ivgl::units::control_unit MyUnit2;
  //myw.MyScene << MyUnit;
  //myw << "ControlUnit" <<  "CubeUnit";
  myw << "ControlUnit" <<  "TestUnit" << "SkyUnit";
  //myw << "ControlUnit" <<  "Mobius";

  return myw.Run();
} /* End of 'WinMain' function */

/*
VOID f( int x )
{
  std:: cout << x << " "; 
}

int main( void )
{
  stock<INT> mys;
  mys << 2 << 213 << 30 << 47;
  mys.Walk(f);

}
*/

/* END OF 'main.cpp' FILE */