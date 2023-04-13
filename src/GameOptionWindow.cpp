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

#include "GameOptionWindow.hpp"

#include <cstdio>
#include <cstdlib>

#include "Application.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Window.hpp"

extern BcApplication *Application;

static Label *MusicVolumeLabel_;
static Label *SFXVolumeLabel_;
static Label *ShowGridStatus_;

const char *GameOptionWindowName = "GameOptionWindow";

static void SoundSFXVolumeOptions(Window *OptionWindow,
                                  VideoDriver *Video,
                                  const int OptionWindowWidth);
static void MusicVolumeOptions(Window *OptionWindow,
                               VideoDriver *Video,
                               const int OptionWindowWidth);
static void ShowGridOptions(Window *OptionWindow,
                            Game *MainGame,
                            VideoDriver *Video,
                            const int OptionWindowWidth);

static void ExitOptionWindow(Button *PressedButton, Game *MainGame);
static void HigherMusicVolume(Button *PressedButton, void *Arg);
static void HigherSFXVolume(Button *PressedButton, void *Arg);
static void LowerMusicVolume(Button *PressedButton, void *Arg);
static void LowerSFXVolume(Button *PressedButton, void *Arg);
static void TurnGridOff(Button *PressedButton, Game *MainGame);
static void TurnGridOn(Button *PressedButton, Game *MainGame);

void GameOptionWindow(Game *MainGame, VideoDriver *Video)
{
  const int OptionWindowWidth = 300;
  const int OptionWindowHeight = 340;

  int WindowWidth, WindowHeight;
  Window *OptionWindow;
  Button *NewButton;

  Video->GetWindowSize(WindowWidth, WindowHeight);

  OptionWindow = new Window;

  OptionWindow->Create(GameOptionWindowName, OptionWindowWidth, OptionWindowHeight, 0, 0);
  OptionWindow->Center(WindowWidth, WindowHeight);

  SoundSFXVolumeOptions(OptionWindow, Video, OptionWindowWidth);
  MusicVolumeOptions(OptionWindow, Video, OptionWindowWidth);
  ShowGridOptions(OptionWindow, MainGame, Video, OptionWindowWidth);

  NewButton = new Button;

  NewButton->Create("Okay", 200, 32, 0, OptionWindowHeight - 48);
  NewButton->CenterOnX(OptionWindowWidth);
  NewButton->SetFontScale(2.0);
  NewButton->ChangeSignal(SIGNAL(ExitOptionWindow), MainGame);
  OptionWindow->AddWidget(NewButton);

  MainGame->ChangeMainWindow(OptionWindow);
}

// Private Function of game option window
static void
SoundSFXVolumeOptions(Window *OptionWindow,
                      VideoDriver *Video,
                      const int OptionWindowWidth)
{
  Button *NewButton;
  Label *SFXLabel;
  Label *SFXVolumeLabel;
  char Buffer[256];

  SFXLabel = new Label;

  SFXLabel->Create("SFXVLabel", "SFX Volume", OptionWindowWidth - 32, 0, 16, 16);
  SFXLabel->ChangeScale(2.0);
  OptionWindow->AddWidget(SFXLabel);

  SFXVolumeLabel = new Label;

  sprintf(Buffer, "%d", Application->GetSFXVolume());

  SFXVolumeLabel->Create("SFXVolumeLabel", Buffer, OptionWindowWidth - 96, 0, 48, 56);
  SFXVolumeLabel->ChangeScale(2.0);
  SFXVolumeLabel_ = SFXVolumeLabel;
  OptionWindow->AddWidget(SFXVolumeLabel);

  NewButton = new Button;

  NewButton->Create("", 32, 32, 16, 56);
  NewButton->ChangeImage(Video->LoadTexture("LeftArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(LowerSFXVolume), nullptr);
  OptionWindow->AddWidget(NewButton);

  NewButton = new Button;

  NewButton->Create("", 32, 32, OptionWindowWidth - 48, 56);
  NewButton->ChangeImage(Video->LoadTexture("RightArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(HigherSFXVolume), nullptr);
  OptionWindow->AddWidget(NewButton);
}

static void
MusicVolumeOptions(Window *OptionWindow,
                   VideoDriver *Video,
                   const int OptionWindowWidth)
{
  Label *MusicVolumeLabel;
  Label *MusicLabel;
  Button *NewButton;
  char Buffer[256];

  MusicLabel = new Label;

  MusicLabel->Create("MusicLabel", "Music Volume", OptionWindowWidth - 32, 0, 16, 104);
  MusicLabel->ChangeScale(2.0);
  OptionWindow->AddWidget(MusicLabel);

  MusicVolumeLabel = new Label;

  sprintf(Buffer, "%d", Application->GetMusicVolume());

  MusicVolumeLabel->Create("MusicVolumeLabel", Buffer, OptionWindowWidth - 96, 0, 48, 144);
  MusicVolumeLabel->ChangeScale(2.0);
  MusicVolumeLabel_ = MusicVolumeLabel;
  OptionWindow->AddWidget(MusicVolumeLabel);

  NewButton = new Button;

  NewButton->Create("", 32, 32, 16, 144);
  NewButton->ChangeImage(Video->LoadTexture("LeftArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(LowerMusicVolume), nullptr);
  OptionWindow->AddWidget(NewButton);

  NewButton = new Button;

  NewButton->Create("", 32, 32, OptionWindowWidth - 48, 144);
  NewButton->ChangeImage(Video->LoadTexture("RightArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(HigherMusicVolume), nullptr);
  OptionWindow->AddWidget(NewButton);
}

static void
ShowGridOptions(Window *OptionWindow,
                Game *MainGame,
                VideoDriver *Video,
                const int OptionWindowWidth)
{
  Button *NewButton;
  Label *ShowGridLabel;
  Label *ShowGridStatus;
  char Buffer[256];

  ShowGridLabel = new Label;

  ShowGridLabel->Create("ShowGridLabel", "Grid", OptionWindowWidth - 32, 0, 16, 190);
  ShowGridLabel->ChangeScale(2.0);
  OptionWindow->AddWidget(ShowGridLabel);

  ShowGridStatus = new Label;

  if(MainGame->GetGridValue()) {
    sprintf(Buffer, "On");
  } else {
    sprintf(Buffer, "Off");
  }

  ShowGridStatus->Create("ShowGridStatus", Buffer, OptionWindowWidth - 96, 0, 48, 232);
  ShowGridStatus->ChangeScale(2.0);
  ShowGridStatus_ = ShowGridStatus;
  OptionWindow->AddWidget(ShowGridStatus);

  NewButton = new Button;

  NewButton->Create("", 32, 32, 16, 232);
  NewButton->ChangeImage(Video->LoadTexture("LeftArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(TurnGridOff), MainGame);
  OptionWindow->AddWidget(NewButton);

  NewButton = new Button;

  NewButton->Create("", 32, 32, OptionWindowWidth - 48, 232);
  NewButton->ChangeImage(Video->LoadTexture("RightArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(TurnGridOn), MainGame);
  OptionWindow->AddWidget(NewButton);
}

// Signal functions for game option window
static void
ExitOptionWindow(Button *PressedButton, Game *MainGame)
{
  if(PressedButton == nullptr) { abort(); }

  MainGame->CloseWindow();
  Application->GetAudioDriver()->PlayPressedSound();
}

static void
HigherMusicVolume(Button *PressedButton, void *Arg)
{
  char Buffer[256];

  if(PressedButton == nullptr) { abort(); }
  if(Arg != nullptr)           { abort(); }

  Application->HigherMusicVolume();

  sprintf(Buffer, "%d", Application->GetMusicVolume());
  MusicVolumeLabel_->ChangeString(Buffer);

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
HigherSFXVolume(Button *PressedButton, void *Arg)
{
  char Buffer[256];

  if(PressedButton == nullptr) { abort(); }
  if(Arg != nullptr)           { abort(); }

  Application->HigherSFXVolume();

  sprintf(Buffer, "%d", Application->GetSFXVolume());
  SFXVolumeLabel_->ChangeString(Buffer);

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
LowerMusicVolume(Button *PressedButton, void *Arg)
{
  char Buffer[256];

  if(PressedButton == nullptr) { abort(); }
  if(Arg != nullptr)           { abort(); }

  Application->LowerMusicVolume();

  sprintf(Buffer, "%d", Application->GetMusicVolume());
  MusicVolumeLabel_->ChangeString(Buffer);

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
LowerSFXVolume(Button *PressedButton, void *Arg)
{
  char Buffer[256];

  if(PressedButton == nullptr) { abort(); }
  if(Arg != nullptr)           { abort(); }

  Application->LowerSFXVolume();

  sprintf(Buffer, "%d", Application->GetSFXVolume());
  SFXVolumeLabel_->ChangeString(Buffer);

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
TurnGridOff(Button *PressedButton, Game *MainGame)
{
  if(PressedButton == nullptr) { abort(); }

  if(MainGame->GetGridValue()) {
    MainGame->ChangeShowGrid(false);

    ShowGridStatus_->ChangeString("Off");
  }

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
TurnGridOn(Button *PressedButton, Game *MainGame)
{
  if(PressedButton == nullptr) { abort(); }

  if(!MainGame->GetGridValue()) {
    MainGame->ChangeShowGrid(true);

    ShowGridStatus_->ChangeString("On");
  }

  Application->GetAudioDriver()->PlayPressedSound();
}
