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

#include "OptionMenu.hpp"

#include <cstdio>
#include <cstdlib>

#include "Button.hpp"
#include "Label.hpp"
#include "MainMenu.hpp"
#include "Menu.hpp"
#include "Video.hpp"
#include "Window.hpp"

static Label *DifficultyLabel_ = nullptr;
static Label *FullscreenLabel_ = nullptr;
static Label *MusicVolumeLabel_ = nullptr;
static Label *SFXVolumeLabel_ = nullptr;
static Label *ShowGridLabel_ = nullptr;

static void DifficultyOption(Window *OptionWindow, BcApplication *Application);
static void SFXVolumeOption(Window *OptionWindow, BcApplication *Application);
static void MusicVolumeOption(Window *OptionWindow, BcApplication *Application);
static void ShowGridOption(Window *OptionWindow, BcApplication *Application);
static void FullscreenOption(Window *OptionWindow, BcApplication *Application);

// Signal Functions for the Option Menu
static void Back(Button *PressedButton, BcApplication *Application);
static void FullscreenOff(Button *PressedButton, BcApplication *Application);
static void FullscreenOn(Button *PressedButton, BcApplication *Application);
static void HigherDifficulty_(Button *PressedButton, BcApplication *Application);
static void HigherMusicVolume_(Button *PressedButton, BcApplication *Application);
static void HigherSFXVolume_(Button *PressedButton, BcApplication *Application);
static void LowerDifficulty_(Button *PressedButton, BcApplication *Application);
static void LowerMusicVolume_(Button *PressedButton, BcApplication *Application);
static void LowerSFXVolume_(Button *PressedButton, BcApplication *Application);
static void ShowGridOff(Button *PressedButton, BcApplication *Application);
static void ShowGridOn(Button *PressedButton, BcApplication *Application);

void CreateOptionMenu(BcApplication *Application)
{
  const int Width = 300;
  const int Height = 462;

  SDL_Texture *BackgroundTexture;
  Window *NewWindow;
  Button *NewButton;
  int WindowWidth, WindowHeight;
  Menu *NewMenu;

  NewMenu = new Menu;

  BackgroundTexture = Application->GetVideoDriver()->LoadTexture("TitleScreenBackground.png");

  NewMenu->Create();
  NewMenu->ChangeBackground(BackgroundTexture);

  Application->GetVideoDriver()->GetWindowSize(WindowWidth, WindowHeight);

  NewWindow = new Window;

  NewWindow->Create("OptionMenuWindow", Width, Height, 0, 0);
  NewWindow->Center(WindowWidth, WindowHeight);
  NewMenu->AddWidget(NewWindow);

  DifficultyOption(NewWindow, Application);
  SFXVolumeOption(NewWindow, Application);
  MusicVolumeOption(NewWindow, Application);
  ShowGridOption(NewWindow, Application);
  FullscreenOption(NewWindow, Application);

  // Back to main menu button
  NewButton = new Button;

  NewButton->Create("Back", 200, 32, 16, WindowHeight - 48);
  NewButton->SetFontScale(2.0);
  NewButton->ChangeSignal(SIGNAL(Back), Application);
  NewMenu->AddWidget(NewButton);

  Application->ChangeMenu(NewMenu);
}

// Private Functions of Option Menu
static void
DifficultyOption(Window *OptionWindow, BcApplication *Application)
{
  static const char *DifficultyValueName = "DifficultyValue";

  int WindowWidth, WindowHeight;
  Label *DifficultyLabel;
  Label *DifficultyValue;
  Button *NewButton;

  OptionWindow->GetSize(WindowWidth, WindowHeight);

  DifficultyLabel = new Label;

  DifficultyLabel->Create("DifficultyLabel", "Difficulty", WindowWidth - 32, 0, 16, 16);
  DifficultyLabel->ChangeScale(2.0);
  OptionWindow->AddWidget(DifficultyLabel);

  DifficultyValue = new Label;

  switch(Application->GetDifficulty()) {
  case 0:
    DifficultyValue->Create(DifficultyValueName, "Very Easy", WindowWidth - 96, 0, 48, 56);
    break;
  case 1:
    DifficultyValue->Create(DifficultyValueName, "Easy", WindowWidth - 96, 0, 48, 56);
    break;
  case 2:
    DifficultyValue->Create(DifficultyValueName, "Normal", WindowWidth - 96, 0, 48, 56);
    break;
  case 3:
    DifficultyValue->Create(DifficultyValueName, "Hard", WindowWidth - 96, 0, 48, 56);
    break;
  case 4:
    DifficultyValue->Create(DifficultyValueName, "Very Hard", WindowWidth - 96, 0, 48, 56);
    break;
  default:
    abort();
  }

  DifficultyValue->ChangeScale(2.0);
  DifficultyLabel_ = DifficultyValue;
  OptionWindow->AddWidget(DifficultyValue);

  NewButton = new Button;

  NewButton->Create("", 32, 32, 16, 56);
  NewButton->ChangeImage(Application->GetVideoDriver()->LoadTexture("LeftArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(LowerDifficulty_), Application);
  OptionWindow->AddWidget(NewButton);

  NewButton = new Button;

  NewButton->Create("", 32, 32, WindowWidth - 48, 56);
  NewButton->ChangeImage(Application->GetVideoDriver()->LoadTexture("RightArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(HigherDifficulty_), Application);
  OptionWindow->AddWidget(NewButton);
}

static void
SFXVolumeOption(Window *OptionWindow, BcApplication *Application)
{
  int WindowWidth, WindowHeight;
  Label *SFXVolumeLabel;
  Label *SFXVolumeValue;
  Button *NewButton;
  char Buffer[256];

  OptionWindow->GetSize(WindowWidth, WindowHeight);

  SFXVolumeLabel = new Label;

  SFXVolumeLabel->Create("SFXVolumeLabel", "SFX Volume", WindowWidth - 32, 0, 16, 104);
  SFXVolumeLabel->ChangeScale(2.0);
  OptionWindow->AddWidget(SFXVolumeLabel);

  SFXVolumeValue = new Label;

  sprintf(Buffer, "%d", Application->GetSFXVolume());

  SFXVolumeValue->Create("SFXVolumeValue", Buffer, WindowWidth - 96, 0, 48, 144);
  SFXVolumeValue->ChangeScale(2.0);
  SFXVolumeLabel_ = SFXVolumeValue;
  OptionWindow->AddWidget(SFXVolumeValue);

  NewButton = new Button;

  NewButton->Create("", 32, 32, 16, 144);
  NewButton->ChangeImage(Application->GetVideoDriver()->LoadTexture("LeftArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(LowerSFXVolume_), Application);
  OptionWindow->AddWidget(NewButton);

  NewButton = new Button;

  NewButton->Create("", 32, 32, WindowWidth - 48, 144);
  NewButton->ChangeImage(Application->GetVideoDriver()->LoadTexture("RightArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(HigherSFXVolume_), Application);
  OptionWindow->AddWidget(NewButton);
}

static void
MusicVolumeOption(Window *OptionWindow, BcApplication *Application)
{
  int WindowWidth, WindowHeight;
  Label *MusicVolumeLabel;
  Label *MusicVolumeValue;
  Button *NewButton;
  char Buffer[256];

  OptionWindow->GetSize(WindowWidth, WindowHeight);

  MusicVolumeLabel = new Label;

  MusicVolumeLabel->Create("MusicVolumeLabel", "Music Volume", WindowWidth - 32, 0, 16, 192);
  MusicVolumeLabel->ChangeScale(2.0);
  OptionWindow->AddWidget(MusicVolumeLabel);

  MusicVolumeValue = new Label;

  sprintf(Buffer, "%d", Application->GetMusicVolume());

  MusicVolumeValue->Create("MusicVolumeValue", Buffer, WindowWidth - 96, 0, 48, 240);
  MusicVolumeValue->ChangeScale(2.0);
  MusicVolumeLabel_ = MusicVolumeValue;
  OptionWindow->AddWidget(MusicVolumeValue);

  NewButton = new Button;

  NewButton->Create("", 32, 32, 16, 240);
  NewButton->ChangeImage(Application->GetVideoDriver()->LoadTexture("LeftArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(LowerMusicVolume_), Application);
  OptionWindow->AddWidget(NewButton);

  NewButton = new Button;

  NewButton->Create("", 32, 32, WindowWidth - 48, 240);
  NewButton->ChangeImage(Application->GetVideoDriver()->LoadTexture("RightArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(HigherMusicVolume_), Application);
  OptionWindow->AddWidget(NewButton);
}

static void
ShowGridOption(Window *OptionWindow, BcApplication *Application)
{
  static const char *ShowGridValueName = "ShowGridValue";

  int WindowWidth, WindowHeight;
  Label *ShowGridLabel;
  Label *ShowGridValue;
  Button *NewButton;

  OptionWindow->GetSize(WindowWidth, WindowHeight);

  ShowGridLabel = new Label;

  ShowGridLabel->Create("ShowGridLabel", "Grid", WindowWidth - 32, 0, 16, 288);
  ShowGridLabel->ChangeScale(2.0);
  OptionWindow->AddWidget(ShowGridLabel);

  ShowGridValue = new Label;

  if(Application->GetShowGridValue()) {
    ShowGridValue->Create(ShowGridValueName, "On", WindowWidth - 96, 0, 48, 328);
  } else {
    ShowGridValue->Create(ShowGridValueName, "Off", WindowWidth - 96, 0, 48, 328);
  }

  ShowGridValue->ChangeScale(2.0);
  ShowGridLabel_ = ShowGridValue;
  OptionWindow->AddWidget(ShowGridValue);

  NewButton = new Button;

  NewButton->Create("", 32, 32, 16, 328);
  NewButton->ChangeImage(Application->GetVideoDriver()->LoadTexture("LeftArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(ShowGridOff), Application);
  OptionWindow->AddWidget(NewButton);

  NewButton = new Button;

  NewButton->Create("", 32, 32, WindowWidth - 48, 328);
  NewButton->ChangeImage(Application->GetVideoDriver()->LoadTexture("RightArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(ShowGridOn), Application);
  OptionWindow->AddWidget(NewButton);
}

static void
FullscreenOption(Window *OptionWindow, BcApplication *Application)
{
  static const char *FullscreenValueName = "FullscreenValue";

  int WindowWidth, WindowHeight;
  Label *FullscreenLabel;
  Label *FullscreenValue;
  Button *NewButton;

  OptionWindow->GetSize(WindowWidth, WindowHeight);

  FullscreenLabel = new Label;

  FullscreenLabel->Create("FullscreenLabel", "Fullscreen", WindowWidth - 32, 0, 16, 376);
  FullscreenLabel->ChangeScale(2.0);
  OptionWindow->AddWidget(FullscreenLabel);

  FullscreenValue = new Label;

  if(Application->GetFullscreenValue()) {
    FullscreenValue->Create(FullscreenValueName, "On", WindowWidth - 96, 0, 48, 416);
  } else {
    FullscreenValue->Create(FullscreenValueName, "Off", WindowWidth - 96, 0, 48, 416);
  }

  FullscreenValue->ChangeScale(2.0);
  FullscreenLabel_ = FullscreenValue;
  OptionWindow->AddWidget(FullscreenValue);

  NewButton = new Button;

  NewButton->Create("", 32, 32, 16, 416);
  NewButton->ChangeImage(Application->GetVideoDriver()->LoadTexture("LeftArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(FullscreenOff), Application);
  OptionWindow->AddWidget(NewButton);

  NewButton = new Button;

  NewButton->Create("", 32, 32, WindowWidth - 48, 416);
  NewButton->ChangeImage(Application->GetVideoDriver()->LoadTexture("RightArrowButton.png"));
  NewButton->ChangeSignal(SIGNAL(FullscreenOn), Application);
  OptionWindow->AddWidget(NewButton);
}

// Signal Functions of Option Menu
static void
Back(Button *PressedButton, BcApplication *Application)
{
  if(PressedButton == nullptr) { abort(); }

  Application->GetAudioDriver()->PlayPressedSound();
  Application->SaveConfigFile();
  CreateMainMenu(Application);
}

static void
FullscreenOff(Button *PressedButton, BcApplication *Application)
{
  if(PressedButton == nullptr) { abort(); }

  if(Application->GetFullscreenValue()) {
    bool Ret;

    Ret = Application->ChangeFullscreenValue(false);
    if(!Ret) {
      FullscreenLabel_->ChangeString("Off");
    } else {
      FullscreenLabel_->ChangeString("On");
    }
  }

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
FullscreenOn(Button *PressedButton, BcApplication *Application)
{
  if(PressedButton == nullptr) { abort(); }

  if(!Application->GetFullscreenValue()) {
    bool Ret;

    Ret = Application->ChangeFullscreenValue(true);
    if(Ret) {
      FullscreenLabel_->ChangeString("On");
    } else {
      FullscreenLabel_->ChangeString("Off");
    }
  }

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
HigherDifficulty_(Button *PressedButton, BcApplication *Application)
{
  if(PressedButton == nullptr) { abort(); }

  Application->HigherDifficulty();

  switch(Application->GetDifficulty()) {
  case 0:
    DifficultyLabel_->ChangeString("Very Easy");
    break;
  case 1:
    DifficultyLabel_->ChangeString("Easy");
    break;
  case 2:
    DifficultyLabel_->ChangeString("Normal");
    break;
  case 3:
    DifficultyLabel_->ChangeString("Hard");
    break;
  case 4:
    DifficultyLabel_->ChangeString("Very Hard");
    break;
  default:
    abort();
  }

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
HigherMusicVolume_(Button *PressedButton, BcApplication *Application)
{
  char Buffer[256];

  if(PressedButton == nullptr) { abort(); }

  Application->HigherMusicVolume();

  sprintf(Buffer, "%d", Application->GetMusicVolume());
  MusicVolumeLabel_->ChangeString(Buffer);

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
HigherSFXVolume_(Button *PressedButton, BcApplication *Application)
{
  char Buffer[256];

  if(PressedButton == nullptr) { abort(); }

  Application->HigherSFXVolume();

  sprintf(Buffer, "%d", Application->GetSFXVolume());
  SFXVolumeLabel_->ChangeString(Buffer);

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
LowerDifficulty_(Button *PressedButton, BcApplication *Application)
{
  if(PressedButton == nullptr) { abort(); }

  Application->LowerDifficulty();

  switch(Application->GetDifficulty()) {
  case 0:
    DifficultyLabel_->ChangeString("Very Easy");
    break;
  case 1:
    DifficultyLabel_->ChangeString("Easy");
    break;
  case 2:
    DifficultyLabel_->ChangeString("Normal");
    break;
  case 3:
    DifficultyLabel_->ChangeString("Hard");
    break;
  case 4:
    DifficultyLabel_->ChangeString("Very Hard");
    break;
  default:
    abort();
  }

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
LowerMusicVolume_(Button *PressedButton, BcApplication *Application)
{
  char Buffer[256];

  if(PressedButton == nullptr) { abort(); }

  Application->LowerMusicVolume();

  sprintf(Buffer, "%d", Application->GetMusicVolume());
  MusicVolumeLabel_->ChangeString(Buffer);

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
LowerSFXVolume_(Button *PressedButton, BcApplication *Application)
{
  char Buffer[256];

  if(PressedButton == nullptr) { abort(); }

  Application->LowerSFXVolume();

  sprintf(Buffer, "%d", Application->GetSFXVolume());
  SFXVolumeLabel_->ChangeString(Buffer);

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
ShowGridOff(Button *PressedButton, BcApplication *Application)
{
  if(PressedButton == nullptr) { abort(); }

  if(Application->GetShowGridValue()) {
    Application->ChangeShowGridValue(false);

    ShowGridLabel_->ChangeString("Off");
  }

  Application->GetAudioDriver()->PlayPressedSound();
}

static void
ShowGridOn(Button *PressedButton, BcApplication *Application)
{
  if(PressedButton == nullptr) { abort(); }

  if(!Application->GetShowGridValue()) {
    Application->ChangeShowGridValue(true);

    ShowGridLabel_->ChangeString("On");
  }

  Application->GetAudioDriver()->PlayPressedSound();
}
