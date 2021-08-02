/*************************************************************
 * Copyright (C) 2021
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/
 
/* FILE NAME   : player.h
 * PURPOSE     : Animation project.
 *               Tracks player class declaration module.
 * PROGRAMMER  : CGSG-SummerCamp'2021.
 *               Vitaly A. Galinsky.
 * LAST UPDATE : 31.07.2021.
 * NOTE        : Module namespace 'ivgl'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */
 
#ifndef __player_h_
#define __player_h_
 
#include <filesystem>
#include <random>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
 
#include "../src/def.h"
#include <mmsystem.h>
 
namespace fs = std::filesystem;
 
/* Project namespace */
namespace ivgl
{
  /* Render system class declaration */
  class player
  {
  public:
    /* Loop flag type */
    enum repeat
    {
      REPEAT_NO,     // No repeat current composition
      REPEAT_ALL,    // Repeat all playlist
      REPEAT_CURRENT // Repeat current composition
    }; /* End of 'repeat' type */
 
    /* Playing status type */
    enum status
    {
      STATUS_NOTREADY, // Device is not ready
      STATUS_PLAYING,  // Device is playing
      STATUS_STOPPED,  // Device is stopped
      STATUS_PAUSED,   // Device is pause
      STATUS_SEEKING,  // Device is seeking
      STATUS_OTHER     // For other device status
    }; /* End of 'status' type */
 
  private:
    // Track entry store type
    struct entry
    {
      // Track name
      std::string Name;
 
      // Track file name
      std::string FileName;
 
      // Track type
      std::string Type;
 
      /* Class constructor */
      entry( VOID )
      {
      } /* End of 'entry' function */
 
      /* Class constructor.
       * ARGUMENTS:
       *   - track entry file name:
       *       const std::string &NewFileName;
       */
      entry( const std::string &NewFileName ) : FileName(NewFileName)
      {
        fs::path p(FileName);
        std::string ext = p.extension().string();
 
        if (ext == ".mp3" || ext == ".MP3")
          Type = "type mpegvideo";
        else if (ext == ".wav" || ext == ".wave" || ext == ".wav" || ext == ".WAVE")
          Type = "type waveaudio";
        else if (ext == ".mid" || ext == ".midi" || ext == ".MID" || ext == ".MIDI")
          Type = "type sequencer";
        else
          Type = "";
        Name = p.filename().string();
      } /* End of 'entry' function */
    }; /* End of 'entry' structure */
 
    // File names playlist
    std::vector<entry> PlayList;
 
    // Current playing file name
    entry Current;
 
    // Current playing playlist number (ngative if not used)
    INT PlayListNo = -1;
 
    // Loop flag
    repeat RepeatFlag = REPEAT_NO;
 
    // Audio alias name
    std::string Name;
 
  public:
    /* Class constructor */
    player( VOID )
    {
      SYSTEMTIME st;
 
      GetLocalTime(&st);
      Name = "audio_" +
        std::to_string(st.wHour) + "_" +
        std::to_string(st.wMinute) + "_" +
        std::to_string(st.wSecond) + "_" +
        std::to_string(st.wMilliseconds) + "_" +
        std::to_string(rand() % 3000);
    } /* End of 'player' function */
 
    /* Class destructor */
    ~player( VOID )
    {
    } /* End of '~player' function */
 
    /* Responce tracks player function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (player &) self reference.
     */
    player & Response( VOID )
    {
      status st = GetStatus();
 
      if (st != STATUS_PLAYING && st != STATUS_PAUSED)
        if (RepeatFlag != REPEAT_NO)
          Next();
      return *this;
    } /* End of 'Response' function */
 
    /* Responce tracks player function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (player &) self reference.
     */
    player & Shuffle( VOID )
    {
      std::random_device rd;
      std::mt19937 g(rd());
      std::shuffle(PlayList.begin(), PlayList.end(), g);
      return *this;
    } /* End of 'Response' function */
 
    /* Set loop flag function.
     * ARGUMENTS:
     *   - file name:
     *       repeat RepeatValue;
     * RETURNS:
     *   (player &) self reference.
     */
    player & SetRepeat( repeat RepeatValue )
    {
      RepeatFlag = RepeatValue;
      return *this;
    } /* End of 'SetRepeat' function */
 
    /* Add file name to playlist function.
     * ARGUMENTS:
     *   - file name:
     *       const std::string &FileName;
     * RETURNS:
     *   (player &) self reference.
     */
    player & operator<<( const std::string &FileName )
    {
      fs::path pt(FileName);
 
      if (pt.has_filename())
        PlayList.push_back(FileName);
      else
      {
        for (auto &f : fs::recursive_directory_iterator(FileName))
        {
          fs::path mf(f);
          std::string ext(mf.extension().string());
 
          if (ext == ".mp3" || ext == ".MP3" ||
              ext == ".wav" || ext == ".wave" || ext == ".wav" || ext == ".WAVE" ||
              ext == ".mid" || ext == ".midi" || ext == ".MID" || ext == ".MIDI")
            PlayList.push_back(mf.string());
        }
      }
      return *this;
    } /* End of 'operator<<' function */
 
    /* Play new or current track function.
     * ARGUMENTS:
     *   - new file name (empty for current):
     *       const std::string &FileName;
     * RETURNS:
     *   (player &) self reference.
     */
    player & Play( const std::string &FileName = "" )
    {
      if (FileName != "")
      {
        mciSendString((std::string("stop ") + Name).c_str(), nullptr, 0, nullptr);
        mciSendString((std::string("close ") + Name).c_str(), nullptr, 0, nullptr);
        Current = FileName;
        mciSendString((std::string("open \"") + Current.FileName + "\" " + Current.Type + " alias " + Name).c_str(), nullptr, 0, nullptr);
      }
      mciSendString((std::string("play ") + Name).c_str(), nullptr, 0, nullptr);
      return *this;
    } /* End of 'Play' function */
 
    /* Play track function.
     * ARGUMENTS:
     *   - track number:
     *       INT TrackNo;
     * RETURNS:
     *   (player &) self reference.
     */
    player & Play( INT TrackNo )
    {
      if (TrackNo >= 0 && TrackNo < PlayList.size())
      {
        PlayListNo = TrackNo;
        Play(PlayList[TrackNo].FileName);
      }
      return *this;
    } /* End of 'Play' function */
 
    /* Play next track function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (player &) self reference.
     */
    player & Next( VOID )
    {
      if (PlayListNo >= 0)
        if (PlayListNo < PlayList.size() - 1 ||
            PlayListNo == PlayList.size() - 1 && RepeatFlag != REPEAT_NO)
        {
          if (RepeatFlag != REPEAT_CURRENT)
            PlayListNo = (INT)((PlayListNo + 1) % PlayList.size());
          Play(PlayList[PlayListNo].FileName);
        }
      return *this;
    } /* End of 'Next' function */
 
    /* Play next track function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (player &) self reference.
     */
    player & Prev( VOID )
    {
      if (PlayListNo < PlayList.size())
        if (PlayListNo > 0 ||
            PlayListNo == 0 && RepeatFlag == REPEAT_ALL)
        {
          if (RepeatFlag != REPEAT_CURRENT)
            PlayListNo = (INT)((PlayListNo - 1 + PlayList.size()) % PlayList.size());
          Play(PlayList[PlayListNo].FileName);
        }
      return *this;
    } /* End of 'Prev' function */
 
    /* Pause playing function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (player &) self reference.
     */
    player & Pause( VOID )
    {
      mciSendString((std::string("pause ") + Name).c_str(), nullptr, 0, nullptr);
      return *this;
    } /* End of 'Pause' function */
 
    /* Resume playing after pause function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (player &) self reference.
     */
    player & Resume( VOID )
    {
      mciSendString((std::string("play ") + Name).c_str(), nullptr, 0, nullptr);
      return *this;
    } /* End of 'Resume' function */
 
    /* Stop playing function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (player &) self reference.
     */
    player & Stop( VOID )
    {
      mciSendString((std::string("stop ") + Name).c_str(), nullptr, 0, nullptr);
      return *this;
    } /* End of 'Stop' function */
 
    /* Get current track name function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (std::string) current strack name.
     */
    std::string GetName( VOID )
    {
      return Current.Name;
    } /* End of 'GetName' function */
 
    /* Get current track length in milliseconds function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (INT) current strack length.
     */
    INT GetLength( VOID )
    {
      CHAR Buf[300];
 
      mciSendString((std::string("set ") + Name + " time format milliseconds").c_str(), nullptr, 0, nullptr);
      mciSendString((std::string("status ") + Name + " length").c_str(), Buf, sizeof(Buf), nullptr);
      std::stringstream ss;
      ss << Buf;
      INT len;
      ss >> len;
      return len;
    } /* End of 'GetLength' function */
 
    /* Get current track playing position in milliseconds function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (INT) current strack length.
     */
    INT GetPos( VOID )
    {
      CHAR Buf[300];
 
      mciSendString((std::string("set ") + Name + " time format milliseconds").c_str(), nullptr, 0, nullptr);
      mciSendString((std::string("status ") + Name + " position").c_str(), Buf, sizeof(Buf), nullptr);
      std::stringstream ss;
      ss << Buf;
      INT pos;
      ss >> pos;
      return pos;
    } /* End of 'GetPos' function */
 
    /* Get current track playing status function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (status) current strack length.
     */
    status GetStatus( VOID )
    {
      CHAR Buf[300];
 
      mciSendString((std::string("set ") + Name + " time format milliseconds").c_str(), nullptr, 0, nullptr);
      mciSendString((std::string("status ") + Name + " mode").c_str(), Buf, sizeof(Buf), nullptr);
      std::string s(Buf);
      if (s == "not ready")
        return STATUS_NOTREADY;
      if (s == "playing")
        return STATUS_PLAYING;
      if (s == "paused")
        return STATUS_PAUSED;
      if (s == "stopped")
        return STATUS_STOPPED;
      if (s == "seeking")
        return STATUS_SEEKING;
      return STATUS_OTHER;
    } /* End of 'GetStatus' function */
 
    /* Set repeat playing mode function.
     * ARGUMENTS:
     *   - repeat mode:
     *       repeat RepeatMode;
     * RETURNS:
     *   (repeat) old repeat mode.
     */
    repeat SetLooping( repeat RepeatMode )
    {
      repeat old = RepeatFlag;
 
      RepeatFlag = RepeatMode;
      return old;
    } /* End of 'SetLooping' function */
  }; /* End of 'player' class */
} /* end of 'ivgl' namespace */
 
#endif /* __player_h_ */
 
/* END OF 'player.h' FILE */