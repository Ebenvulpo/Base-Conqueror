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

#include "QuitGameWindow.hpp"

#include <cstdlib>

#include "Application.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Window.hpp"

extern BcApplication *Application;

static void ContinueGame(Button *PressedButton, Game *MainGame);
static void QuitGame(Button *PressedButton, void *Arg);

void
QuitGameWindow(Game *MainGame, VideoDriver *Video)
{
  const int ExitWindowWidth = 512;
  const int ExitWindowHeight = 108;

  int WindowWidth, WindowHeight;
  Window *NewWindow;
  Button *NewButton;
  Label *NewLabel;

  Video->GetWindowSize(WindowWidth, WindowHeight);

  NewWindow = new Window;

  NewWindow->Create("QuitGameWindow", ExitWindowWidth, ExitWindowHeight, 0, 0);
  NewWindow->Center(WindowWidth, WindowHeight);

  NewLabel = new Label;

  NewLabel->Create("QuitGameWindowLabel", "Are you sure you want to quit?", ExitWindowWidth - 32, 0, 16, 16);
  NewLabel->ChangeScale(2.0);
  NewWindow->AddWidget(NewLabel);

  NewButton = new Button;

  NewButton->Create("Yes", 200, 32, 16, ExitWindowHeight - 48);
  NewButton->SetFontScale(2.0);
  NewButton->ChangeSignal(SIGNAL(QuitGame), nullptr);
  NewWindow->AddWidget(NewButton);

  NewButton = new Button;
  NewButton->Create("No", 200, 32, ExitWindowWidth - 216, ExitWindowHeight - 48);
  NewButton->SetFontScale(2.0);
  NewButton->ChangeSignal(SIGNAL(ContinueGame), MainGame);
  NewWindow->AddWidget(NewButton);

  MainGame->ChangeMainWindow(NewWindow);
}

// Signal functions for quit game window
static void
ContinueGame(Button *PressedButton, Game *MainGame)
{
  if(PressedButton == nullptr) { abort(); }

  MainGame->CloseWindow();
  Application->GetAudioDriver()->PlayPressedSound();
}

static void
QuitGame(Button *PressedButton, void *Arg)
{
  if(PressedButton == nullptr) { abort(); }
  if(Arg != nullptr)           { abort(); }

  Application->EndGame();
  Application->GetAudioDriver()->PlayPressedSound();
}
