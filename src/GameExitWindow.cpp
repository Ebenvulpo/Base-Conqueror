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

#include "GameExitWindow.hpp"

#include <cstdio>
#include <cstdlib>

#include "Application.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "GameOptionWindow.hpp"
#include "QuitGameWindow.hpp"
#include "Window.hpp"

extern BcApplication *Application;

const char *ExitWindowName = "ExitWindow";

static void ContinueGame(Button *PressedButton, Game *MainGame);
static void OptionWindow(Button *PressedButton, Game *MainGame);
static void ExitGame(Button *PressedButton, Game *MainGame);

void
CreateExitWindow(Game *MainGame, VideoDriver *Video)
{
  const int ExitWindowWidth = 280;
  const int ExitWindowHeight = 192;

  int WindowWidth, WindowHeight;
  Window *GameExitWindow;
  Button *ContinueButton;
  Button *OptionButton;
  Button *ExitButton;

  if(MainGame->IsWindowOpen()) { return; }

  GameExitWindow = new Window;

  Video->GetWindowSize(WindowWidth, WindowHeight);

  GameExitWindow->Create(ExitWindowName, ExitWindowWidth, ExitWindowHeight, 0, 0);
  GameExitWindow->Center(WindowWidth, WindowHeight);

  ContinueButton = new Button;

  ContinueButton->Create("Continue Game", 248, 32, 0, 16);
  ContinueButton->SetFontScale(2.0);
  ContinueButton->CenterOnX(ExitWindowWidth);
  ContinueButton->ChangeSignal(SIGNAL(ContinueGame), MainGame);
  GameExitWindow->AddWidget(ContinueButton);

  OptionButton = new Button;

  OptionButton->Create("Options", 248, 32, 0, 80);
  OptionButton->CenterOnX(ExitWindowWidth);
  OptionButton->SetFontScale(2.0);
  OptionButton->ChangeSignal(SIGNAL(OptionWindow), MainGame);
  GameExitWindow->AddWidget(OptionButton);

  ExitButton = new Button;

  ExitButton->Create("Exit", 248, 32, 0, 144);
  ExitButton->SetFontScale(2.0);
  ExitButton->CenterOnX(ExitWindowWidth);
  ExitButton->ChangeSignal(SIGNAL(ExitGame), MainGame);
  GameExitWindow->AddWidget(ExitButton);

  MainGame->ChangeMainWindow(GameExitWindow);
}

// Signal Functions for Game Exit Window
static void
ContinueGame(Button *PressedButton, Game *MainGame)
{
  if(PressedButton == nullptr) { abort(); }

  MainGame->CloseWindow();
  Application->GetAudioDriver()->PlayPressedSound();
}

static void
OptionWindow(Button *PressedButton, Game *MainGame)
{
  if(PressedButton == nullptr) { abort(); }

  GameOptionWindow(MainGame, Application->GetVideoDriver());
  Application->GetAudioDriver()->PlayPressedSound();
}

static void
ExitGame(Button *PressedButton, Game *MainGame)
{
  if(PressedButton == nullptr) { abort(); }

  QuitGameWindow(MainGame, Application->GetVideoDriver());
  Application->GetAudioDriver()->PlayPressedSound();
}
