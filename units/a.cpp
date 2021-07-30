/* FILE NAME  : shooter_control.h
 * PROGRAMMER : ID3
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Unit shooter control.
 */

#include "../src/anim/anim.h"

namespace ivgl
{
  /* Units namespace */
  namespace units
  {
    /* My unit class */
    class shooter_control : public unit
    {
    public:
      primitives Prs;                                                          // Hands primitive
      matr HandsRotate = matr::Translate(vec3(-1, -3, -1)) * matr::RotateY(180); // Rotate hands matrix
      matr HandsPos = matr::Identity();                                         // Position hands matrix
      BOOL IsAim = FALSE;
      BOOL IsShoot = FALSE;
      BOOL IsJump = FALSE;
    
      /* Shooter control constructor */
      shooter_control( anim *Ani )
      {
        //Ani->FlipFullScreen();
        //Ani->Camera.Resize(Ani->FrameW, Ani->FrameH);
        //Prs = new vagl::primitives;
        Prs.Load(&Prs, "bin/models/arms_new.g3dm");
        Ani->Camera.Move(vec3(0, Ani->Camera.Up[1] * 5, 0));
      } /* End of 'shooter_control' function */

      /* Response function.
       * ARGUMENTS:
       *   - animation instance:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Response( anim *Ani ) override
      {
        if (Ani->Keys['W'] || Ani->Keys['A'] || Ani->Keys['S'] || Ani->Keys['D'])
              Ani->Camera.Move(((Ani->Camera.Dir * ((Ani->Keys['W'] - Ani->Keys['S'])) +
                             Ani->Camera.Right * ((Ani->Keys['D'] - Ani->Keys['A']))) * 30 * Ani->GlobalDeltaTime).GetVecInPlaneXZ());
        //if (Ani->Keys[VK_LBUTTON])
        {
          Ani->Camera.Rotate(math::vec3<FLT>(0, 1, 0), (100 * -1 * Ani->MouseDX +
                                                30 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
          HandsRotate *= matr::Rotate(-math::vec3<FLT>(0, 1, 0), (100 * 1 * Ani->MouseDX +
                                                -30 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
        }

        //if (Ani->Keys[VK_LBUTTON] && Ani->Camera.Dir.Y < 0.95 && Ani->MouseDY <= 0)
        if (Ani->Camera.Dir[1] < 0.95)
        {
          Ani->Camera.Rotate(Ani->Camera.Right, (100 * -1 * Ani->MouseDY +
                                                  30 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
          HandsRotate *= matr::Rotate(-Ani->Camera.Right, (100 * 1 * Ani->MouseDY +
                                                  -30 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
        }

        //if (Ani->Keys[VK_LBUTTON] && Ani->Camera.Dir.Y > -0.95 && Ani->MouseDY >= 0)
        if (Ani->Camera.Dir[1] > -0.95)
        {
          Ani->Camera.Rotate(Ani->Camera.Right, (100 * -1 * Ani->MouseDY +
                                                  30 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
          HandsRotate *= matr::Rotate(-Ani->Camera.Right, (100 * 1 * Ani->MouseDY +
                                                  -30 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])) * Ani->GlobalDeltaTime / (IsAim + 0.5));
        }

        if (Ani->KeysClick[VK_SPACE])
          IsJump = TRUE;

        if (IsJump)
        {
          static DBL JumpTime;
          JumpTime += Ani->GlobalDeltaTime;
          Ani->Camera.Move(vec3(0, cos(PI * JumpTime) / 20, 0));
          if (Ani->Camera.Loc[1] < 5)
          {
            IsJump = 0;
            JumpTime = 0;
          }
        }

        if (Ani->KeysClick[VK_LBUTTON])
          IsShoot = TRUE;

        if (IsShoot)
        {
          static DBL ShotTime;
          ShotTime += Ani->GlobalDeltaTime;
          if (ShotTime < 0.05)
            HandsRotate = matr::RotateX(-2 * ShotTime) * HandsRotate;
          if (ShotTime > 0.05)
          {
            HandsRotate = matr::RotateX(2 * (ShotTime - 0.0518)) * HandsRotate;
          }
          if (ShotTime > 0.10)
          {
            IsShoot = 0;
            ShotTime = 0;
          }
        }

        vec3 Pos = vec3(Ani->Camera.Loc[0], Ani->Camera.Loc[1], Ani->Camera.Loc[2]);
        HandsPos = matr::Translate(Pos);
        if (Ani->Keys[VK_RBUTTON] || Ani->Keys[VK_RCONTROL])
        {
          if (!IsAim)
          {
            static FLT AimTime;
            AimTime += Ani->GlobalDeltaTime;
            Ani->Camera.SetProj(0.1, 0.1 + 0.5 * AimTime, 500);
            HandsRotate = matr::Translate(vec3(25 * Ani->GlobalDeltaTime, 1 * Ani->GlobalDeltaTime, -10 * Ani->GlobalDeltaTime)) * HandsRotate;
            if (AimTime > 0.1)
            {
              IsAim = 1;
              AimTime = 0;
            }
          }
        }
        else if ((!Ani->Keys[VK_RBUTTON]) || (!Ani->Keys[VK_RCONTROL]))
        {
          if (IsAim)
          {
            static FLT AimTime;
            AimTime += Ani->GlobalDeltaTime;
            Ani->Camera.SetProj(0.1, 0.15 - 0.5 * AimTime, 500);
            HandsRotate = matr::Translate(vec3(-25 * Ani->GlobalDeltaTime, -1 * Ani->GlobalDeltaTime, 10 * Ani->GlobalDeltaTime)) * HandsRotate;
            if (AimTime > 0.1)
            {
              IsAim = 0;
              AimTime = 0;
            }
          }
        }
      } /* End of 'Response' function */

      /* Render function.
       * ARGUMETNS:
       *   - animation instance:
       *       anim *Ani;
       * RETURNS: None.
       */
      VOID Render( anim *Ani ) override
      {
        Ani->PrimitivesDraw(&Prs, matr::Scale(vec3(0.1)) * HandsRotate * HandsPos);
      } /* End of 'Render' function */

    }; /* End of 'unit_control' class */

  }
}
static ivgl::anim::unit_register<ivgl::units::shooter_control> _("new");

/* END OF 'shooter_control.h' FILE */
