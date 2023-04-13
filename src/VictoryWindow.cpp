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

#include "VictoryWindow.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Application.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Window.hpp"

extern BcApplication *Application;

extern const char *PlayerNames[];

const char *VictoryWindowName = "VictoryWindow";

static void ExitGameOver(Button *Pressed, void *Arg);

void VictoryWindow(Game *MainGame, VideoDriver *Video)
{
  const int VictoryWindowWidth = 608;
  const int VictoryWindowHeight = 208;

  int WindowWidth, WindowHeight;
  Window *VictoryWin;
  size_t LongestName;
  Button *NewButton;
  char Buffer[512];

  if(MainGame->IsWindowOpen()) { return; }

  Video->GetWindowSize(WindowWidth, WindowHeight);

  VictoryWin = new Window;

  VictoryWin->Create(VictoryWindowName, VictoryWindowWidth, VictoryWindowHeight, 0, 0);
  VictoryWin->Center(WindowWidth, WindowHeight);

  LongestName = 0;
  for(int I = 0; I < NUMBER_OF_PLAYERS; I++) {
    size_t NameLength;

    NameLength = strlen(PlayerNames[I]);
    if(NameLength > LongestName) { LongestName = NameLength; }
  }

  for(int I = 0; I < NUMBER_OF_PLAYERS; I++) {
    const Player *TargetPlayer = MainGame->GetPlayer(I);
    Label *NewLabel;
    char Buffer2[256];

    sprintf(Buffer, "%s", PlayerNames[I]);
    if(strlen(Buffer) < LongestName) {
      for(size_t J = 0; J < ((LongestName + 1) - strlen(Buffer)); J++) {
        strcat(Buffer, " ");
      }
    }

    if(TargetPlayer->IsAlive()) {
      sprintf(Buffer2, ": %d", TargetPlayer->GetScore());
    } else {
      sprintf(Buffer2, ": %d (Dead)", TargetPlayer->GetScore());
    }

    strcat(Buffer, Buffer2);

    NewLabel = new Label;

    NewLabel->Create("PlayerScoreLabel", Buffer, 0, 0, 16, 16 + (I * 32));
    NewLabel->ChangeScale(2.0);
    VictoryWin->AddWidget(NewLabel);
  }

  NewButton = new Button;

  NewButton->Create("Victory!", 200, 32, 0, VictoryWindowHeight - 48);
  NewButton->SetFontScale(2.0);
  NewButton->CenterOnX(VictoryWindowWidth);
  NewButton->ChangeSignal(SIGNAL(ExitGameOver), nullptr);
  VictoryWin->AddWidget(NewButton);

  Application->GetAudioDriver()->NextSong(SONG_WE_ARE_VICTORIOUS);
  Application->GetAudioDriver()->FadeMusic(1000);

  MainGame->ChangeMainWindow(VictoryWin);
}

// Signal Functions for Victory Window
static void
ExitGameOver(Button *PressedButton, void *Arg)
{
  if(PressedButton == nullptr) { abort(); }
  if(Arg != nullptr)           { abort(); }

  Application->EndGame();
  Application->GetAudioDriver()->PlayPressedSound();
}
