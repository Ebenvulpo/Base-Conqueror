// Copyright (c) 2020 Ebenvulpo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

class BcApplication;

#include <random>
#include <SDL2/SDL_events.h>

#include "Audio.hpp"
#include "Game.hpp"
#include "Menu.hpp"
#include "Video.hpp"

class BcApplication {
private:
  VideoDriver *Video;
  AudioDriver *Audio;
  bool EndGame_;
  Game *MainGame;
  Menu *Menus;
  Menu *NewMenu;
  bool Running;
  std::minstd_rand Rand;
  char *ApplicationPath;

  bool Fullscreen;
  bool ShowGrid;
  int DifficultyValue;
  int SFXVolume;
  int MusicVolume;

  void Input(void);
  bool LoadConfigFile(void);
  bool ValidateConfig(void);
public:
  ~BcApplication(void);
  BcApplication(void) { }
  void ChangeDifficultyValue(const int NewValue) { DifficultyValue = NewValue; }
  bool ChangeFullscreenValue(const bool Value);
  void ChangeMenu(Menu *NewMenu);
  void ChangeShowGridValue(const bool Value) { ShowGrid = Value; }
  void EndGame(void) { if(MainGame != nullptr) { EndGame_ = true; } }
  AudioDriver *GetAudioDriver(void) { return Audio; }
  VideoDriver *GetVideoDriver(void) { return Video; }
  const char *GetApplicationPath(void) const;
  int GetDifficulty(void) const { return DifficultyValue; }
  bool GetFullscreenValue(void) const { return Fullscreen; }
  Game *GetGame(void) { return MainGame; }
  int GetMusicVolume(void)  const { return MusicVolume; }
  int GetSFXVolume(void) const { return SFXVolume; }
  bool GetShowGridValue(void) const { return ShowGrid; }
  void HigherDifficulty(void);
  void HigherMusicVolume(void);
  void HigherSFXVolume(void);
  bool Init(void);
  void Loop(void);
  void LowerDifficulty(void);
  void LowerMusicVolume(void);
  void LowerSFXVolume(void);
  void NewGame(void);
  bool SaveConfigFile(void);
  void Stop(void);
};

#endif /* _APPLICATION_HPP_ */
