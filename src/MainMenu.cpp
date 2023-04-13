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

#include "MainMenu.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "Button.hpp"
#include "Config.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "OptionMenu.hpp"
#include "Version.hpp"
#include "Video.hpp"
#include "Window.hpp"

// Signal Functions for the Main Menu
static void Quit(Button *PressedButton, BcApplication *Application);
static void OptionsMenu(Button *PressedButton, BcApplication *Application);
static void StartNewGame(Button *PressedButton, BcApplication *Application);

void CreateMainMenu(BcApplication *Application)
{
  const int MenuWidth = 232;
  const int MenuHeight = 192;

  SDL_Texture *BackgroundTexture;
  int WindowWidth, WindowHeight;
  Window *NewWindow;
  Button *NewButton;
  Image *TitleImage;
  Label *NewLabel;
  Menu *NewMenu;
  char Buffer[256];

  Application->GetVideoDriver()->GetWindowSize(WindowWidth, WindowHeight);

  NewMenu = new Menu;

  BackgroundTexture = Application->GetVideoDriver()->LoadTexture("TitleScreenBackground.png");

  NewMenu->Create();
  NewMenu->ChangeBackground(BackgroundTexture);

  NewWindow = new Window;

  NewWindow->Create("MenuMainWindow", MenuWidth, MenuHeight, 0, 0);
  NewWindow->Center(WindowWidth, WindowHeight);
  NewMenu->AddWidget(NewWindow);

  TitleImage = new Image;

  TitleImage->Create(Application->GetVideoDriver()->LoadTexture("Title.png"), 308, 120, 0, 48);
  TitleImage->CenterOnX(WindowWidth);
  NewMenu->AddWidget(TitleImage);

  NewButton = new Button;

  NewButton->Create("Start Game", 200, 32, 0, 16);
  NewButton->CenterOnX(MenuWidth);
  NewButton->SetFontScale(2.0);
  NewButton->ChangeSignal(SIGNAL(StartNewGame), Application);
  NewWindow->AddWidget(NewButton);

  NewButton = new Button;

  NewButton->Create("Options", 200, 32, 0, 80);
  NewButton->CenterOnX(MenuWidth);
  NewButton->SetFontScale(2.0);
  NewButton->ChangeSignal(SIGNAL(OptionsMenu), Application);
  NewWindow->AddWidget(NewButton);

  NewButton = new Button;

  NewButton->Create("Quit", 200, 32, 0, 144);
  NewButton->CenterOnX(MenuWidth);
  NewButton->SetFontScale(2.0);
  NewButton->ChangeSignal(SIGNAL(Quit), Application);
  NewWindow->AddWidget(NewButton);

  NewLabel = new Label;

  NewLabel->Create("GameMakerLabel", "A Game by Ebenvulpo", 0, 0, 8, WindowHeight - 20);
  NewMenu->AddWidget(NewLabel);

  sprintf(Buffer, "Version: %d.%d", VERSION_MAJOR, VERSION_MINOR);

  NewLabel = new Label;

  NewLabel->Create("VersionLabel", Buffer, 0, 0, WindowWidth - (FONT_WIDTH * (int) (strlen(Buffer) + 1)), WindowHeight - 20);
  NewMenu->AddWidget(NewLabel);

  Application->ChangeMenu(NewMenu);
}

static void
Quit(Button *PressedButton, BcApplication *Application)
{
  if(PressedButton == nullptr) { abort(); }

  Application->Stop();
  Application->GetAudioDriver()->PlayPressedSound();

  // Give the audio driver enough time to play the pressed button sound.
  SDL_Delay(300);
}

static void
OptionsMenu(Button *PressedButton, BcApplication *Application)
{
  if(PressedButton == nullptr) { abort(); }

  CreateOptionMenu(Application);
  Application->GetAudioDriver()->PlayPressedSound();
}

static void
StartNewGame(Button *PressedButton, BcApplication *Application)
{
  if(PressedButton == nullptr) { abort(); }

  Application->NewGame();
  Application->GetAudioDriver()->PlayPressedSound();
}
