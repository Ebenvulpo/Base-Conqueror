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

#include "Application.hpp"

#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_filesystem.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>

#include "MainMenu.hpp"

static const char *ConfigFileName = "Config.bcf";

BcApplication::~BcApplication(void)
{
  if(Menus != nullptr)    { delete Menus; }
  if(MainGame != nullptr) { delete MainGame; }

  delete Video;
  delete Audio;

  SDL_free(ApplicationPath);
}

bool
BcApplication::ChangeFullscreenValue(const bool Value)
{
  bool Ret;

  Fullscreen = Value;

  Ret = Video->ChangeFullscreenMode(Fullscreen);
  if(!Ret) { Fullscreen = false; }

  return Ret;
}

void
BcApplication::ChangeMenu(Menu *NewMenu)
{
  this->NewMenu = NewMenu;
}

const char *
BcApplication::GetApplicationPath(void) const
{
  return ApplicationPath;
}

void
BcApplication::HigherDifficulty(void)
{
  if(DifficultyValue < 4) { DifficultyValue++; }
}

void
BcApplication::HigherMusicVolume(void)
{
  if(MusicVolume < 100) { MusicVolume += 5; }

  Audio->ChangeMusicVolume(MusicVolume);
}

void
BcApplication::HigherSFXVolume(void)
{
  if(SFXVolume < 100) { SFXVolume += 5; }

  Audio->ChangeSFXVolume(SFXVolume);
}

bool
BcApplication::Init(void)
{
  time_t Seed;

  Video = new VideoDriver;
  Audio = new AudioDriver;

  ApplicationPath = SDL_GetBasePath();

  if(!LoadConfigFile()) {
    DifficultyValue = 2;
    SFXVolume = 80;
    MusicVolume = 50;
    ShowGrid = false;
    Fullscreen = true;

    SaveConfigFile();
  }

  if(!Video->Init(ApplicationPath)) {
    delete Video;
    delete Audio;

    return false;
  }

  NewMenu = nullptr;
  Menus = nullptr;
  EndGame_ = false;
  MainGame = nullptr;
  Running = true;

  if(!Audio->Init(ApplicationPath)) {
    delete Video;
    delete Audio;

    return false;
  }

  Audio->ChangeSFXVolume(SFXVolume);
  Audio->ChangeMusicVolume(MusicVolume);

  Video->ChangeFullscreenMode(Fullscreen);

  Seed = time(nullptr);
  Rand.seed((unsigned int) Seed);

  CreateMainMenu(this);

  return true;
}

void
BcApplication::Loop(void)
{
  Uint32 PrevTick = 0;
  Uint32 Tick;
  int X, Y;
  int ret;
  SDL_Event Event;

  while(Running) {
    if(MainGame == nullptr) {
      if(!Audio->IsMusicPlaying()) {
        Audio->PlayMusic(SONG_THE_COMING_WAR);
      }
    }

    if(EndGame_) {
      delete MainGame;

      MainGame = nullptr;
      EndGame_ = false;

      CreateMainMenu(this);
    }

    if(NewMenu != nullptr) {
      delete Menus;

      Menus = NewMenu;
      NewMenu = nullptr;
    }

    Video->Clear();
    if(MainGame != nullptr) {
      if(Menus != nullptr) {
        delete Menus;

        Menus = nullptr;
      }

      MainGame->Draw(Video);
    } else {
      Menus->Draw(Video, 0, 0);
    }

    SDL_GetMouseState(&X, &Y);
    Video->DrawCursor(X, Y);

    Video->Finish();

    ret = SDL_PollEvent(&Event);
    if(ret == 1) {
      switch(Event.type) {
      case SDL_QUIT:
        if(MainGame != nullptr) {
          MainGame->Input(&Event, Video, Rand);
        } else {
          Running = false;
        }
        break;
      default:
        if(MainGame != nullptr) {
          MainGame->Input(&Event, Video, Rand);
        } else {
          Input();
        }
      }

      SDL_PumpEvents();
      SDL_FlushEvents(SDL_KEYDOWN, SDL_MULTIGESTURE);
    }

    Tick = SDL_GetTicks();
    if(PrevTick == 0) {
      if(MainGame != nullptr) {
        MainGame->Tick((double) Tick / 1000.0, Video, Rand);
      }
    } else {
      if(MainGame != nullptr) {
        MainGame->Tick((double) (Tick - PrevTick) / 1000.0, Video, Rand);
      }
    }

    PrevTick = Tick;

    Audio->Tick();
  }
}

void
BcApplication::LowerDifficulty(void)
{
  if(DifficultyValue > 0) { DifficultyValue--; }
}

void
BcApplication::LowerMusicVolume(void)
{
  if(MusicVolume > 0) { MusicVolume -= 5; }

  Audio->ChangeMusicVolume(MusicVolume);
}

void
BcApplication::LowerSFXVolume(void)
{
  if(SFXVolume > 0) { SFXVolume -= 5; }

  Audio->ChangeSFXVolume(SFXVolume);
}

void
BcApplication::NewGame(void)
{
  MainGame = new Game();

  MainGame->Init(DifficultyValue, Video, Rand);
  MainGame->ChangeShowGrid(ShowGrid);
}

bool
BcApplication::SaveConfigFile(void)
{
  static const char *MagicNumber = "bcf";

  FILE *ConfigFile;
  size_t BufferSize;
  char *Buffer;

  BufferSize = strlen(ApplicationPath);
  BufferSize += strlen(ConfigFileName);

  Buffer = new char[BufferSize + sizeof(char)];
  if(Buffer == nullptr) { abort(); }

  sprintf(Buffer, "%s%s", ApplicationPath, ConfigFileName);

  ConfigFile = fopen(Buffer, "wb");
  delete[] Buffer;
  if(ConfigFile == nullptr) { return false; }

  BufferSize = fwrite(&MagicNumber[0], 1, 1, ConfigFile);
  BufferSize += fwrite(&MagicNumber[1], 1, 1, ConfigFile);
  BufferSize += fwrite(&MagicNumber[2], 1, 1, ConfigFile);
  BufferSize += fwrite(&DifficultyValue, 1, 1, ConfigFile);
  BufferSize += fwrite(&SFXVolume, 1, 1, ConfigFile);
  BufferSize += fwrite(&MusicVolume, 1, 1, ConfigFile);
  BufferSize += fwrite(&ShowGrid, 1, 1, ConfigFile);
  BufferSize += fwrite(&Fullscreen, 1, 1, ConfigFile);

  if(BufferSize < 8 || BufferSize > 8) { goto Error; }

  fclose(ConfigFile);

  return true;
Error:
  fclose(ConfigFile);

  return false;
}

void
BcApplication::Stop(void)
{
  this->Running = false;
}

// Private Functions of Application
void
BcApplication::Input(void)
{
  static bool LeftMouseButtonPressed = false;

  Uint32 MouseState;
  int X, Y;

  MouseState = SDL_GetMouseState(&X, &Y);
  if(MouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
    if(!LeftMouseButtonPressed) {
      Menus->CheckPress(X, Y);
      LeftMouseButtonPressed = true;
    }
  } else {
    Menus->CheckRelease(X, Y);
    LeftMouseButtonPressed = false;
  }
}

bool
BcApplication::LoadConfigFile(void)
{
  unsigned char MagicNumber[3];
  FILE *ConfigFile;
  size_t BufferSize;
  char *Buffer;

  BufferSize = strlen(ApplicationPath);
  BufferSize += strlen(ConfigFileName);

  Buffer = new char[BufferSize + sizeof(char)];
  if(Buffer == nullptr) { abort(); }

  sprintf(Buffer, "%s%s", ApplicationPath, ConfigFileName);

  ConfigFile = fopen(Buffer, "rb");
  delete[] Buffer;
  if(ConfigFile == nullptr) { return false; }

  BufferSize = fread(&MagicNumber[0], 1, 1, ConfigFile);
  BufferSize += fread(&MagicNumber[1], 1, 1, ConfigFile);
  BufferSize += fread(&MagicNumber[2], 1, 1, ConfigFile);

  if(BufferSize < 3 || BufferSize > 3) { goto Error; }

  if(!(MagicNumber[0] == 'b' &&
       MagicNumber[1] == 'c' &&
       MagicNumber[2] == 'f')) {
    goto Error;
  }

  DifficultyValue = 0;
  SFXVolume = 0;
  MusicVolume = 0;
  ShowGrid = false;
  Fullscreen = false;

  BufferSize = fread(&DifficultyValue, 1, 1, ConfigFile);
  BufferSize += fread(&SFXVolume, 1, 1, ConfigFile);
  BufferSize += fread(&MusicVolume, 1, 1, ConfigFile);
  BufferSize += fread(&ShowGrid, 1, 1, ConfigFile);
  BufferSize += fread(&Fullscreen, 1, 1, ConfigFile);

  if(BufferSize < 5 || BufferSize > 5) { goto Error; }

  if(!ValidateConfig()) { goto Error; }

  fclose(ConfigFile);

  return true;
Error:
  fclose(ConfigFile);

  return false;
}

bool
BcApplication::ValidateConfig(void)
{
  if(DifficultyValue < 0 || DifficultyValue > 4) { return false; }
  if(SFXVolume < 0 || SFXVolume > 100)           { return false; }
  if(MusicVolume < 0 || MusicVolume > 100)       { return false; }
  if(ShowGrid != false && ShowGrid != true)      { return false; }
  if(Fullscreen != false && Fullscreen != true)  { return false; }

  return true;
}
