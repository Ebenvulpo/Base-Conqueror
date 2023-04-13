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

#include "Game.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_scancode.h>
#include <string>

#include "Ai.hpp"
#include "Application.hpp"
#include "Button.hpp"
#include "Config.hpp"
#include "Debug.hpp"
#include "ExitWindow.hpp"
#include "GameExitWindow.hpp"
#include "GameOptionWindow.hpp"
#include "GameOverWindow.hpp"
#include "Label.hpp"
#include "QuitGameWindow.hpp"
#include "ScoreWindow.hpp"
#include "VictoryWindow.hpp"

extern const char *GameOverWindowName;
extern const char *VictoryWindowName;

const char *PlayerNames[] = {
  "Gen. Peacekeeper",
  "Gen. Knucklehead",
  "Gen. Ironhands",
  "Gen. Destruction"
};

extern BcApplication *Application;

Game::~Game(void)
{
  delete GameWorld;

  if(NewWindow != nullptr)  { delete NewWindow; }
  if(MainWindow != nullptr) { delete MainWindow; }
}

void
Game::ChangeMainWindow(Window *NewWindow)
{
  if(this->NewWindow != nullptr) { delete this->NewWindow; }

  this->NewWindow = NewWindow;
}

void
Game::CloseWindow(void)
{
  if(MainWindow != nullptr) { CloseMainWindow = true; }
}

void
Game::Draw(VideoDriver *Video)
{
  GameWorld->Draw(Video, Offset_X, Offset_Y, ShowGrid);

  DrawOwnerHighlights(Video);
  DrawSelectedBase(Video);
  DrawHighlight(Video);
  DrawLinks(Video);
  DrawAttacks(Video);
  DrawBaseStatus(Video);

  if(MainWindow) { MainWindow->Draw(Video, 0, 0); }
}

void
Game::FindNewHomeBase(Player *TargetPlayer)
{
  Base *LargestBase = nullptr;

  for(int I = 0; I < GameWorld->GetNumBases(); I++) {
    Base *TargetBase = GameWorld->GetBase(I);

    if(TargetBase->GetOwner() != nullptr) {
      if(TargetBase->GetOwner()->GetID() == TargetPlayer->GetID()) {
        if(LargestBase == nullptr) {
          LargestBase = TargetBase;
        } else if(LargestBase->GetSize() < TargetBase->GetSize()) {
          LargestBase = TargetBase;
        }
      }
    }
  }

  TargetPlayer->ChangeHomeBase(LargestBase);
}

Player *
Game::GetPlayer(const int I)
{
  if(I < 0 || I >= NUMBER_OF_PLAYERS) { return nullptr; }

  return &Players[I];
}

bool
Game::Init(const int DifficultyValue,
           VideoDriver *Video, std::minstd_rand &Rand)
{
  this->DifficultyValue = DifficultyValue;
  this->NewWindow = nullptr;
  this->MainWindow = nullptr;

  GameWorld = new World();

  GameWorld->Create(WORLD_SIZE_X, WORLD_SIZE_Y, NUMBER_OF_BASES, Rand);

  CreatePlayers();
  AssignPlayerBases(Rand);

  this->ShowGrid = false;
  this->CloseMainWindow = false;
  this->Offset_X = 0;
  this->Offset_Y = 0;
  this->Mouse_OldLocX = 0;
  this->Mouse_OldLocY = 0;
  this->EndGameTick = 0.0;

  CenterOnBase(Video, HumanPlayer->GetHomeBase());

  return true;
}

void
Game::Input(SDL_Event *Event,
            VideoDriver *Video, std::minstd_rand &Rand)
{
  static int Mouse_OldLocX = 0, Mouse_OldLocY = 0;
  static bool LeftMouseButtonPressed = false;
  static bool RightMouseButtonPressed = false;

  Uint32 State;
  int X, Y;

  State = SDL_GetMouseState(&X, &Y);

  if(MainWindow != nullptr) {
    if(State & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      if(!LeftMouseButtonPressed) {
        MainWindow->CheckPress(X, Y);
        LeftMouseButtonPressed = true;
      }
    } else {
      MainWindow->CheckRelease(X, Y);
      LeftMouseButtonPressed = false;
    }

    return;
  }

  if(State & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
    if(!RightMouseButtonPressed) {
      SelectedTileInput(X, Y, Rand);
      RightMouseButtonPressed = true;
    }
  } else {
    RightMouseButtonPressed = false;
  }

  const Uint8 *KeyboardState = SDL_GetKeyboardState(nullptr);
  if(KeyboardState[SDL_SCANCODE_G]) {
    if(!ShowGrid) {
      ShowGrid = true;
    } else {
      ShowGrid = false;
    }
  }

  if(KeyboardState[SDL_SCANCODE_A]) { FindNearbyEnemyBase(Video, HumanPlayer); }
  if(KeyboardState[SDL_SCANCODE_S]) { CenterOnBase(Video, HumanPlayer->GetHomeBase()); }
  if(KeyboardState[SDL_SCANCODE_F]) { DestroyLink(); }
  if(KeyboardState[SDL_SCANCODE_W]) { HumanPlayer->ChangeSelectedBase(nullptr); }
  if(KeyboardState[SDL_SCANCODE_ESCAPE]) { CreateExitWindow(this, Video); }
  if(KeyboardState[SDL_SCANCODE_F1]) { CreateScoreWindow(this, Video); }

  switch(Event->type) {
  case SDL_MOUSEMOTION:
    State = SDL_GetMouseState(&X, &Y);
    if(State & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      CheckMapBoarder(Video, X - Mouse_OldLocX, Y - Mouse_OldLocY);
      Mouse_OldLocX = X;
      Mouse_OldLocY = Y;
    } else {
      Mouse_OldLocX = X;
      Mouse_OldLocY = Y;
    }
    break;
  case SDL_QUIT:
    ExitApplicationWindow(this, Video);
    break;
  default:
    break;
  }
}

void
Game::Tick(const double Delta, VideoDriver *Video, std::minstd_rand &Rand)
{
  CheckMusic(Rand);
  CheckToCloseWindow();

  if(CloseMainWindow) {
    delete MainWindow;

    MainWindow = nullptr;
    CloseMainWindow = false;
  }

  if(NewWindow != nullptr) {
    if(MainWindow != nullptr) { delete MainWindow; }

    MainWindow = NewWindow;
    NewWindow = nullptr;
  }

  if(MainWindow != nullptr) { return; }

  BasesTick(Delta, Rand);
  PlayersTick(Delta, Rand);
  CheckIfPlayersAreAlive();
  CheckIfHumanPlayerIsAlive(Delta, Video);
  CheckIfAIPlayersAreAlive(Delta, Video);
}

/* Private Functions of Game */
void
Game::AssignPlayerBases(std::minstd_rand &Rand)
{
  Player *PlayersToAssignBases[NUMBER_OF_PLAYERS];

  for(int I = 0; I < NUMBER_OF_PLAYERS; I++) {
  AssignAgain:
    PlayersToAssignBases[I] = &Players[Rand() % NUMBER_OF_PLAYERS];

    for(int J = 0; J < I; J++) {
      if(PlayersToAssignBases[J]->GetID() == PlayersToAssignBases[I]->GetID()) {
        goto AssignAgain;
      }
    }
  }

  for(int I = 0; I < NUMBER_OF_PLAYERS; I++) {
    Base *TargetBase = GameWorld->GetBase((I * (NUMBER_OF_BASES / 4)) + (Rand() % (NUMBER_OF_BASES / 4)));

    if(TargetBase->GetOwner() != nullptr) { abort(); }

    TargetBase->ChangeOwner(PlayersToAssignBases[I]);
    if(TargetBase->GetSize() < 5) {
      TargetBase->ChangeSize(5); // Give the player a fighting chance.
    }

    TargetBase->ChangeSoldiers(10.0);

    if(PlayersToAssignBases[I]->IsHuman()) {
      PlayersToAssignBases[I]->ChangeHomeBase(TargetBase);
    }
  }
}

void
Game::AttackBase(Base *SrcBase, Base *TargetBase, std::minstd_rand &Rand)
{
  if(SrcBase->GetAttackingBase() != nullptr) {
    if(SrcBase->GetAttackingBase()->GetID() == TargetBase->GetID()) {
      const int Score = TargetBase->GetOwner()->GetScore();

      TargetBase->GetOwner()->ChangeScore(Score + 1);
      SrcBase->AttackBase(nullptr, Rand);
    } else {
      SrcBase->AttackBase(TargetBase, Rand);
    }
  } else {
    SrcBase->AttackBase(TargetBase, Rand);
  }
}

void
Game::BasesTick(const double Delta, std::minstd_rand &Rand)
{
  for(int I = 0; I < GameWorld->GetNumBases(); I++) {
    Base *TargetBase = GameWorld->GetBase(I);

    TargetBase->Tick(Delta, this->DifficultyValue, Rand);

    /* Check if the human player has a base selected that has been conquered by
       a computer player, and if so, then unselected it if the base has been
       conquered by a computer player. */
    if(HumanPlayer->GetSelectedBase() != nullptr) {
      if(HumanPlayer->GetSelectedBase()->GetOwner()->GetID() != HumanPlayer->GetID()) {
        HumanPlayer->ChangeSelectedBase(nullptr);
      }
    }
  }
}

void
Game::CenterOnBase(VideoDriver *Video, const Base *TargetBase)
{
  int WindowWidth, WindowHeight;
  int TileX, TileY;
  int LocX, LocY;

  if(TargetBase == nullptr) { return; }

  Offset_X = 0;
  Offset_Y = 0;

  TargetBase->GetLoc(LocX, LocY);
  Video->GetWindowSize(WindowWidth, WindowHeight);

  GetTilePixelLoc(LocX, LocY, TileX, TileY);
  Offset_X = -(TileX) + ((WindowWidth  / 2) - (TILE_SIZE / 2));
  Offset_Y = -(TileY) + ((WindowHeight / 2) - (TILE_SIZE / 2));
}

void
Game::CheckIfAIPlayersAreAlive(const double Delta, VideoDriver *Video)
{
  if(!Players[1].IsAlive() && !Players[2].IsAlive() && !Players[3].IsAlive()) {
    // If not, then a human victory over the AI overlords.
    if(EndGameTick >= 3.0) {
      VictoryWindow(this, Video);
    } else {
      EndGameTick += Delta;
    }
  }
}

void
Game::CheckIfHumanPlayerIsAlive(const double Delta, VideoDriver *Video)
{
  if(!HumanPlayer->IsAlive()) {
    // If not, then game over.
    if(EndGameTick >= 3.0) {
      GameOverWindow(this, Video);
    } else {
      EndGameTick += Delta;
    }
  }
}

void
Game::CheckIfPlayersAreAlive(void)
{
  for(int I = 0; I < NUMBER_OF_PLAYERS; I++) {
    Player *TargetPlayer = &Players[I];

    for(int J = 0; J < GameWorld->GetNumBases(); J++) {
      Base *TargetBase = GameWorld->GetBase(J);

      if(TargetBase->GetOwner() != nullptr) {
        if(TargetBase->GetOwner()->GetID() == TargetPlayer->GetID()) {
          goto IsAlive;
        }
      }
    }

    TargetPlayer->Dead();
  IsAlive:
    continue;
  }
}

void
Game::CheckMapBoarder(VideoDriver *Video, const int X, const int Y)
{
  int WindowWidth, WindowHeight;
  int WorldSizeX, WorldSizeY;
  int XHighLimit, XLowLimit;
  int YHighLimit, YLowLimit;

  Video->GetWindowSize(WindowWidth, WindowHeight);
  GameWorld->GetSize(WorldSizeX, WorldSizeY);

#if DEBUG_INPUT
  std::cerr << "Offset_X: " << Offset_X << " Offset_Y: " << Offset_Y << std::endl;
#endif /* DEBUG_INPUT */

  XHighLimit =  ((WorldSizeX * (TILE_SIZE / 2)) + (WindowWidth / 2));
  XLowLimit  = -((WorldSizeY * (TILE_SIZE / 2)) - (WindowWidth / 2));

  if((Offset_X + X) <= XHighLimit && (Offset_X + X) >= XLowLimit) {
    Offset_X += X;
  }

  YHighLimit = WindowHeight / 2;
  YLowLimit  = -((WorldSizeY * (TILE_SIZE / 2)) - (WindowHeight / 2));

  if((Offset_Y + Y) <= YHighLimit && (Offset_Y + Y) >= YLowLimit) {
    Offset_Y += Y;
  }
}

void
Game::CheckMusic(std::minstd_rand &Rand)
{
  if(MainWindow != nullptr) {
    if(MainWindow->GetName() != GameOverWindowName &&
       MainWindow->GetName() != VictoryWindowName &&
       !Application->GetAudioDriver()->IsMusicPlaying()) {
      if(Rand() % 2) {
        Application->GetAudioDriver()->PlayMusic(SONG_THE_COMING_WAR);
      } else {
        Application->GetAudioDriver()->PlayMusic(SONG_THE_ROAD_TO_VICTORY);
      }
    }
  } else {
    if(!Application->GetAudioDriver()->IsMusicPlaying()) {
      if(Rand() % 2) {
        Application->GetAudioDriver()->PlayMusic(SONG_THE_COMING_WAR);
      } else {
        Application->GetAudioDriver()->PlayMusic(SONG_THE_ROAD_TO_VICTORY);
      }
    }
  }
}

void
Game::CheckToCloseWindow(void)
{
  if(CloseMainWindow) {
    if(MainWindow != nullptr) { delete MainWindow; }

    MainWindow = nullptr;
    CloseMainWindow = false;
  }
}

void
Game::CreatePlayers(void)
{
  for(int I = 0; I < NUMBER_OF_PLAYERS; I++) {
    if(I == 0) {
      Players[I].Create(I + 1, true);
    } else {
      Players[I].Create(I + 1, false);
    }
  }

  HumanPlayer = &Players[0];
}

void
Game::ColonizeBase(Base *TargetBase, Player *TargetPlayer)
{
  Base *SelectedBase = TargetPlayer->GetSelectedBase();

  if(SelectedBase != nullptr) {
    const int Distance = SelectedBase->GetDistanceToBase(TargetBase);

    if(SelectedBase->GetSoldiers() > 10.0 && Distance <= 24) {
      TargetBase->Colonize(TargetPlayer, SelectedBase);
    }
  }
}

void
Game::DrawAttacks(VideoDriver *Video)
{
  for(int I = 0; I < GameWorld->GetNumBases(); I++) {
    Base *TargetBase = GameWorld->GetBase(I);

    if(TargetBase->GetAttackingBase() != nullptr) {
      Base *AttackingBase = TargetBase->GetAttackingBase();
      int StartX, StartY;
      int EndX, EndY;
      int StartPixelX, StartPixelY;
      int EndPixelX, EndPixelY;

      TargetBase->GetLoc(StartX, StartY);
      AttackingBase->GetLoc(EndX, EndY);

      GetTilePixelLoc(StartX, StartY, StartPixelX, StartPixelY);
      GetTilePixelLoc(EndX, EndY, EndPixelX, EndPixelY);

      StartPixelX += TILE_SIZE / 2;
      StartPixelY += (TILE_SIZE / 2) + (TILE_SIZE / 4);

      EndPixelX += TILE_SIZE / 2;
      EndPixelY += (TILE_SIZE / 2) + (TILE_SIZE / 4);

      Video->DrawLine(StartPixelX, StartPixelY,
                      EndPixelX, EndPixelY,
                      0xFF, 0x00, 0x00, 0xFF);
      Video->DrawRect(8, 8, EndPixelX - 4, EndPixelY - 4, 0xFF, 0x00, 0x00, 0xFF);
    }
  }
}

void
Game::DestroyLink(void)
{
  if(HumanPlayer->GetSelectedBase() != nullptr) {
    HumanPlayer->GetSelectedBase()->ChangeTargetBase(nullptr);
  }
}

void
Game::DrawBaseStatus(VideoDriver *Video)
{
  int TileX, TileY;
  int X, Y;

  if(MainWindow != nullptr) { return; }

  SDL_GetMouseState(&X, &Y);
  GetMousePointerTileLoc(X, Y, TileX, TileY);

  for(int I = 0; I < GameWorld->GetNumBases(); I++) {
    Base *TargetBase = GameWorld->GetBase(I);
    int LocX, LocY;

    TargetBase->GetLoc(LocX, LocY);

    /* If the player is the owner of this base, then highlight the soldiers
       and size of the base. */
    if(LocX == (int) TileX &&
       LocY == (int) TileY &&
       TargetBase->GetOwner() == HumanPlayer) {
      DrawBaseStatusOwner(Video, TargetBase);
    /* If the player is not the owner and not any other players of this base,
       then highlight that it is not captured. */
    } else if(LocX == (int) TileX &&
              LocY == (int) TileY &&
              TargetBase->GetOwner() == nullptr) {
      DrawBaseStatusUnowned(Video, TargetBase);
    /* If an other player owns this base, then
       highlight the base owner name. */
    } else if(LocX == (int) TileX &&
              LocY == (int) TileY &&
              TargetBase->GetOwner() != HumanPlayer) {
      DrawBaseStatusEnemy(Video, TargetBase);
    }
  }
}

void
Game::DrawBaseStatusEnemy(VideoDriver *Video, const Base *TargetBase)
{
  int WindowWidth, WindowHeight;
  size_t LongestString = 0;
  size_t StringLength[2];
  char Buffer[256];
  char Buffer2[256];
  int LocX, LocY;
  int X, Y;

  Video->GetWindowSize(WindowWidth, WindowHeight);

  TargetBase->GetLoc(LocX, LocY);
  GetTilePixelLoc(LocX, LocY, X, Y);

  switch(TargetBase->GetOwner()->GetID()) {
  case 2:
    sprintf(Buffer, PlayerNames[1]);
    break;
  case 3:
    sprintf(Buffer, PlayerNames[2]);
    break;
  case 4:
    sprintf(Buffer, PlayerNames[3]);
    break;
  default:
    abort();
  }

  StringLength[0] = strlen(Buffer);

  if(HumanPlayer->GetSelectedBase() != nullptr &&
     TargetBase != HumanPlayer->GetSelectedBase()) {
    double Distance;

    Distance = HumanPlayer->GetSelectedBase()->GetDistanceToBase(TargetBase);

    if(Distance <= 24) {
      int AttackingPercentage;

      if(Distance > 6) {
        AttackingPercentage = (1.0 - ((0.75 / 18.0) * (double) (Distance - 6))) * 100.0;
      } else {
        AttackingPercentage = 100;
      }

      sprintf(Buffer2, "Distance: %d (%d%%)", (int) Distance, AttackingPercentage);
    } else {
      sprintf(Buffer2, "Distance: %d (Too Far)", (int) Distance);
    }

    StringLength[1] = strlen(Buffer2);
  }

  if(HumanPlayer->GetSelectedBase() != nullptr &&
     TargetBase != HumanPlayer->GetSelectedBase()) {
    for(int I = 0; I < 2; I++) {
      if(StringLength[I] > LongestString) {
        LongestString = StringLength[I];
      }
    }

    if(((X + TILE_SIZE) + (FONT_WIDTH * (int) LongestString) + 8) > WindowWidth) {
      int LocX = (X - (FONT_WIDTH * (int) LongestString)) - 8;
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (36 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * LongestString) + 8, 36, LocX, LocY);

      Video->DrawStr(Buffer,  LocX + 4, LocY + 4,  1.0);
      Video->DrawStr(Buffer2, LocX + 4, LocY + 20, 1.0);
    } else {
      int LocX = (X + TILE_SIZE);
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (36 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * LongestString) + 8, 36, LocX, LocY);

      Video->DrawStr(Buffer,  LocX + 4, LocY + 4,  1.0);
      Video->DrawStr(Buffer2, LocX + 4, LocY + 20, 1.0);
    }
  } else {
    if(((X + TILE_SIZE) + (FONT_WIDTH * (int) strlen(Buffer) + 8)) > WindowWidth) {
      int LocX = (X - (FONT_WIDTH * strlen(Buffer))) - 8;
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (20 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * strlen(Buffer)) + 8, 20, LocX, LocY);

      Video->DrawStr(Buffer, LocX + 4, LocY + 4, 1.0);
    } else {
      int LocX = (X + TILE_SIZE);
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (20 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * strlen(Buffer)) + 8, 20, LocX, LocY);

      Video->DrawStr(Buffer, LocX + 4, LocY + 4, 1.0);
    }
  }
}

void
Game::DrawBaseStatusOwner(VideoDriver *Video, const Base *TargetBase)
{
  int WindowWidth, WindowHeight;
  size_t LongestString = 0;
  size_t StringLength[3];
  char Buffer[256];
  char Buffer2[256];
  char Buffer3[256];
  int LocX, LocY;
  int X, Y;

  Video->GetWindowSize(WindowWidth, WindowHeight);

  TargetBase->GetLoc(LocX, LocY);
  GetTilePixelLoc(LocX, LocY, X, Y);

  sprintf(Buffer, "Soldiers: %d", (int) TargetBase->GetSoldiers());
  StringLength[0] = strlen(Buffer);

  sprintf(Buffer2, "Size: %d", (int) TargetBase->GetSize());
  StringLength[1] = strlen(Buffer2);

  if(HumanPlayer->GetSelectedBase() != nullptr &&
     TargetBase != HumanPlayer->GetSelectedBase()) {
    double Distance;

    Distance = HumanPlayer->GetSelectedBase()->GetDistanceToBase(TargetBase);

    if(Distance <= 24) {
      int MovingPercentage;

      if(Distance > 6) {
        MovingPercentage = ceil((0.25 - ((0.20 / 18.0) * (double) (Distance - 6))) * 100.0);
      } else {
        MovingPercentage = 25;
      }

      sprintf(Buffer3, "Distance: %d (%d%%)", (int) Distance, MovingPercentage);
    } else {
      sprintf(Buffer3, "Distance: %d (Too Far)", (int) Distance);
    }

    StringLength[2] = strlen(Buffer3);
  }

  /* Make the box bigger if you have to show the distance from
     the selected base. */
  if(HumanPlayer->GetSelectedBase() != nullptr &&
     TargetBase != HumanPlayer->GetSelectedBase()) {
    for(int I = 0; I < 3; I++) {
      if(StringLength[I] > LongestString) {
        LongestString = StringLength[I];
      }
    }

    // Check if the base status is within window width.
    if(((X + TILE_SIZE) + (FONT_WIDTH * (int) LongestString) + 8) > WindowWidth) {
      int LocX = (X - (FONT_WIDTH * (int) (LongestString))) - 8;
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (52 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * LongestString) + 8, 52, LocX, LocY);

      Video->DrawStr(Buffer,  LocX + 4, LocY + 4,  1.0);
      Video->DrawStr(Buffer2, LocX + 4, LocY + 20, 1.0);
      Video->DrawStr(Buffer3, LocX + 4, LocY + 36, 1.0);
    } else {
      int LocX = (X + TILE_SIZE);
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (52 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * LongestString) + 8, 52, LocX, LocY);

      Video->DrawStr(Buffer,  LocX + 4, LocY + 4,  1.0);
      Video->DrawStr(Buffer2, LocX + 4, LocY + 20, 1.0);
      Video->DrawStr(Buffer3, LocX + 4, LocY + 36, 1.0);
    }
  } else {
    for(int I = 0; I < 2; I++) {
      if(StringLength[I] > LongestString) {
        LongestString = StringLength[I];
      }
    }

    if(((X + TILE_SIZE) + (FONT_WIDTH * (int) LongestString) + 8) > WindowWidth) {
      int LocX = (X - (FONT_WIDTH * (int) (LongestString))) - 8;
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (36 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * LongestString) + 8, 36, LocX, LocY);

      Video->DrawStr(Buffer,  LocX + 4, LocY + 4,  1.0);
      Video->DrawStr(Buffer2, LocX + 4, LocY + 20, 1.0);
    } else {
      int LocX = (X + TILE_SIZE);
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (36 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * LongestString) + 8, 36, LocX, LocY);

      Video->DrawStr(Buffer,  LocX + 4, LocY + 4,  1.0);
      Video->DrawStr(Buffer2, LocX + 4, LocY + 20, 1.0);
    }
  }
}

void
Game::DrawBaseStatusUnowned(VideoDriver *Video, const Base *TargetBase)
{
  int WindowWidth, WindowHeight;
  size_t LongestString = 0;
  size_t StringLength[3];
  char Buffer[256];
  char Buffer2[256];
  int LocX, LocY;
  int X, Y;

  Video->GetWindowSize(WindowWidth, WindowHeight);

  TargetBase->GetLoc(LocX, LocY);
  GetTilePixelLoc(LocX, LocY, X, Y);

  sprintf(Buffer, "Not Captured");
  StringLength[0] = strlen(Buffer);

  if(HumanPlayer->GetSelectedBase() != nullptr &&
     TargetBase != HumanPlayer->GetSelectedBase()) {
    double Distance;

    Distance = HumanPlayer->GetSelectedBase()->GetDistanceToBase(TargetBase);

    if(Distance <= 24) {
      sprintf(Buffer2, "Distance: %d", (int) Distance);
    } else {
      sprintf(Buffer2, "Distance: %d (Too Far)", (int) Distance);
    }

    StringLength[1] = strlen(Buffer2);
  }

  if(HumanPlayer->GetSelectedBase() != nullptr &&
     TargetBase != HumanPlayer->GetSelectedBase()) {
    for(int I = 0; I < 2; I++) {
      if(StringLength[I] > LongestString) {
        LongestString = StringLength[I];
      }
    }

    if(((X + TILE_SIZE) + (FONT_WIDTH * (int) LongestString) + 8) > WindowWidth) {
      int LocX = (X - (FONT_WIDTH * (int) (LongestString - 1))) - 8;
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (36 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * LongestString) + 8, 36, LocX, LocY);

      Video->DrawStr(Buffer,  LocX + 4, LocY + 4,  1.0);
      Video->DrawStr(Buffer2, LocX + 4, LocY + 20, 1.0);
    } else {
      int LocX = (X + TILE_SIZE);
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (36 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * (LongestString)) + 8, 36, LocX, LocY);

      Video->DrawStr(Buffer,  LocX + 4, LocY + 4,  1.0);
      Video->DrawStr(Buffer2, LocX + 4, LocY + 20, 1.0);
    }
  } else {
    if(((X + TILE_SIZE) + (FONT_WIDTH * (int) strlen(Buffer)) + 8) > WindowWidth) {
      int LocX = (X - (FONT_WIDTH * (int) (LongestString - 1))) - 8;
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (20 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * strlen(Buffer)) + 8, 20, LocX, LocY);

      Video->DrawStr(Buffer, LocX + 4, LocY + 4, 1.0);
    } else {
      int LocX = (X + TILE_SIZE);
      int LocY = (Y + (TILE_SIZE / 2) + (TILE_SIZE / 4)) - (20 / 2);

      DrawStatusRect(Video, (FONT_WIDTH * strlen(Buffer)) + 8, 20, LocX, LocY);

      Video->DrawStr(Buffer, LocX + 4, LocY + 4, 1.0);
    }
  }
}

void
Game::DrawHighlight(VideoDriver *Video)
{
  int X, Y;
  int TileX, TileY;

  if(MainWindow != nullptr) { return; }

  SDL_GetMouseState(&X, &Y);
  GetMousePointerTileLoc(X, Y, TileX, TileY);

  Video->DrawTile(TileX, TileY, Offset_X, Offset_Y, TileType::GrayHighlight);
}

void
Game::DrawLinks(VideoDriver *Video)
{
  for(int I = 0; I < GameWorld->GetNumBases(); I++) {
    Base *TargetBase = GameWorld->GetBase(I);

    if(TargetBase->GetTargetBase() != nullptr) {
      Player *Owner = TargetBase->GetOwner();
      Base *LinkBase = TargetBase->GetTargetBase();
      int StartX, StartY;
      int EndX, EndY;
      int StartPixelX, StartPixelY;
      int EndPixelX, EndPixelY;

      TargetBase->GetLoc(StartX, StartY);
      LinkBase->GetLoc(EndX, EndY);

      GetTilePixelLoc(StartX, StartY, StartPixelX, StartPixelY);
      GetTilePixelLoc(EndX, EndY, EndPixelX, EndPixelY);

      StartPixelX += TILE_SIZE / 2;
      StartPixelY += (TILE_SIZE / 2) + (TILE_SIZE / 4);

      EndPixelX += TILE_SIZE / 2;
      EndPixelY += (TILE_SIZE / 2) + (TILE_SIZE / 4);

      switch(Owner->GetID()) {
      case 1:
        Video->DrawLine(StartPixelX, StartPixelY,
                        EndPixelX, EndPixelY,
                        0xFF, 0xFF, 0x00, 0xFF);
        Video->DrawRect(8, 8,
                        EndPixelX - 4, EndPixelY - 4,
                        0xFF, 0xFF, 0x00, 0xFF);
        break;
      case 2:
        Video->DrawLine(StartPixelX, StartPixelY,
                        EndPixelX, EndPixelY,
                        0x00, 0xFF, 0xFF, 0xFF);
        Video->DrawRect(8, 8,
                        EndPixelX - 4, EndPixelY - 4,
                        0x00, 0xFF, 0xFF, 0xFF);
        break;
      case 3:
        Video->DrawLine(StartPixelX, StartPixelY,
                        EndPixelX, EndPixelY,
                        0x87, 0x7F, 0x00, 0xFF);
        Video->DrawRect(8, 8,
                        EndPixelX - 4, EndPixelY - 4,
                        0x87, 0x7F, 0x00, 0xFF);
        break;
      case 4:
        Video->DrawLine(StartPixelX, StartPixelY,
                        EndPixelX, EndPixelY,
                        0x7F, 0x00, 0x7F, 0xFF);
        Video->DrawRect(8, 8,
                        EndPixelX - 4, EndPixelY - 4,
                        0x7F, 0x00, 0x7F, 0xFF);
        break;
      default:
        abort();
      }
    }
  }
}

void
Game::DrawOwnerHighlights(VideoDriver *Video)
{
  for(int I = 0; I < GameWorld->GetNumBases(); I++) {
    Base *TargetBase = GameWorld->GetBase(I);
    int LocX, LocY;

    TargetBase->GetLoc(LocX, LocY);

    if(TargetBase->GetOwner() != nullptr) {
      switch(TargetBase->GetOwner()->GetID()) {
      case 1:
        Video->DrawTile(LocX, LocY, Offset_X, Offset_Y, TileType::YellowHighlight);
        break;
      case 2:
        Video->DrawTile(LocX, LocY, Offset_X, Offset_Y, TileType::CyanHighlight);
        break;
      case 3:
        Video->DrawTile(LocX, LocY, Offset_X, Offset_Y, TileType::BrownHighlight);
        break;
      case 4:
        Video->DrawTile(LocX, LocY, Offset_X, Offset_Y, TileType::PurpleHighlight);
        break;
      default:
        abort();
      }
    } else {
      // If the base is unowned, then highlight it as such.
      Video->DrawTile(LocX, LocY, Offset_X, Offset_Y, TileType::WhiteHighlight);
    }
  }
}

void
Game::DrawSelectedBase(VideoDriver *Video)
{
  int LocX, LocY;

  if(HumanPlayer->GetSelectedBase() != nullptr) {
    HumanPlayer->GetSelectedBase()->GetLoc(LocX, LocY);

    Video->DrawTile(LocX, LocY, Offset_X, Offset_Y, TileType::RedHighlight);
  }
}

void
Game::DrawStatusRect(VideoDriver *Video,
                     const int Width,
                     const int Height,
                     const int X, const int Y)
{
  Video->DrawRect(Width - 4, 2         , X + 2          , Y               , 0x99, 0x99, 0x99, 0xFF);
  Video->DrawRect(2        , Height - 4, X              , Y + 2           , 0x99, 0x99, 0x99, 0xFF);
  Video->DrawRect(Width - 4, Height - 4, X + 2          , Y + 2           , 0xCC, 0xCC, 0xCC, 0xFF);
  Video->DrawRect(2        , Height - 4, X + (Width - 2), Y + 2           , 0x66, 0x66, 0x66, 0xFF);
  Video->DrawRect(Width - 4, 2         , X + 2          , Y + (Height - 2), 0x66, 0x66, 0x66, 0xFF);
}

void
Game::FindNearbyEnemyBase(VideoDriver *Video, const Player *TargetPlayer)
{
  int WindowWidth, WindowHeight;
  Base *ClosestEnemyBase = nullptr;
  int ClosestDistance = -1;

  Video->GetWindowSize(WindowWidth, WindowHeight);

  for(int I = 0; I < GameWorld->GetNumBases(); I++) {
    Base *TargetBase = GameWorld->GetBase(I);

    if(TargetBase->GetOwner() != nullptr &&
       TargetBase->GetOwner() != TargetPlayer) {
      int TileX, TileY;
      int LocX, LocY;
      int Distance;
      int X, Y;

      GetMousePointerTileLoc(WindowWidth / 2, WindowHeight / 2, TileX, TileY);
      TargetBase->GetLoc(LocX, LocY);

      X = LocX - TileX;
      Y = LocY - TileY;

      X *= X;
      Y *= Y;
      Distance = ceil(sqrt(X + Y));

      if(ClosestDistance < 0) {
        ClosestEnemyBase = TargetBase;
        ClosestDistance = Distance;
      } else {
        if(Distance < ClosestDistance) {
          ClosestEnemyBase = TargetBase;
          ClosestDistance = Distance;
        }
      }
    }
  }

  CenterOnBase(Video, ClosestEnemyBase);
}

void
Game::GetMousePointerTileLoc(const int X,
                             const int Y,
                             int &TileLocX, int &TileLocY)
{
  double TileX, TileY, TempX;
  int SizeX, SizeY;

  TileX = (X - Offset_X) - (TILE_SIZE / 2);
  TileY = (Y - Offset_Y) - (TILE_SIZE / 2);

  TempX = TileX;
  TileX = (TileY - (TempX / 2)) / (TILE_SIZE / 2);
  TileY = (TileY + (TempX / 2)) / (TILE_SIZE / 2);

  GameWorld->GetSize(SizeX, SizeY);

  if(TileX < 0.0) TileX = 0.0;
  if(TileY < 0.0) TileY = 0.0;
  if(TileX >= (double) SizeX) { TileX = SizeX - 1; }
  if(TileY >= (double) SizeY) { TileY = SizeY - 1; }

  TileLocX = TileX;
  TileLocY = TileY;
}

void
Game::GetTilePixelLoc(const int X,
                      const int Y,
                      int &PixelX, int &PixelY)
{
  PixelX = ((-X + Y) * (TILE_SIZE / 2)) + Offset_X;
  PixelY = ((X + Y) * (TILE_SIZE / 4)) + Offset_Y;
}

void
Game::LinkBases(Base *SrcBase, Base *DstBase)
{
  const int Distance = SrcBase->GetDistanceToBase(DstBase);

  // Unlink the two base if they are already linked together.
  if(SrcBase->GetTargetBase() != nullptr) {
    if(SrcBase->GetTargetBase()->GetID() == DstBase->GetID()) {
      SrcBase->ChangeTargetBase(nullptr);
    }
  } else {
    // Check if the destination base has a linked base.
    if(DstBase->GetTargetBase() != nullptr) {
      // If destination base linked to the source base, then unlinked them both.
      if(DstBase->GetTargetBase()->GetID() == SrcBase->GetID()) {
        DstBase->ChangeTargetBase(nullptr);
      } else {
        if(Distance <= 24) { SrcBase->ChangeTargetBase(DstBase); }
      }
    } else {
      if(Distance <= 24) { SrcBase->ChangeTargetBase(DstBase); }
    }
  }
}

void
Game::PlayersTick(const double Delta, std::minstd_rand &Rand)
{
  for(int I = 0; I < NUMBER_OF_PLAYERS; I++) {
    Players[I].Tick(this, Delta, Rand);
  }
}

void
Game::SelectedTileInput(const int X,
                        const int Y, std::minstd_rand &Rand)
{
  int TileX, TileY;
  int LocX, LocY;

  if(MainWindow != nullptr) { return; }

  GetMousePointerTileLoc(X, Y, TileX, TileY);

  for(int I = 0; I < GameWorld->GetNumBases(); I++) {
    Base *TargetBase = GameWorld->GetBase(I);

    TargetBase->GetLoc(LocX, LocY);
    if(TileX == LocX && (int) TileY == LocY) {
#if DEBUG_INPUT
      std::cerr << "Input: SelectedTileInput Called" << std::endl;
#endif /* DEBUG_INPUT */

      if(TargetBase->GetOwner() == HumanPlayer) {
        /* Unselected player's target base. */
        if(HumanPlayer->GetSelectedBase() == TargetBase) {
          HumanPlayer->ChangeSelectedBase(nullptr);
          /* Link two bases together. */
        } else if(HumanPlayer->GetSelectedBase() != nullptr &&
                  HumanPlayer->GetSelectedBase() != TargetBase &&
                  TargetBase->GetOwner() == HumanPlayer) {
          LinkBases(HumanPlayer->GetSelectedBase(), TargetBase);
          Application->GetAudioDriver()->PlaySelectedSound();
          /* Change selected base tile. */
        } else {
          HumanPlayer->ChangeSelectedBase(TargetBase);
          Application->GetAudioDriver()->PlaySelectedSound();
        }
        /* Colonize base if it has no owner. */
      } else if(TargetBase->GetOwner() == nullptr) {
        ColonizeBase(TargetBase, HumanPlayer);
        /* Attack base if it has an owner. */
      } else if(TargetBase->GetOwner() != HumanPlayer &&
                TargetBase->GetOwner() != nullptr &&
                HumanPlayer->GetSelectedBase() != nullptr) {
        if(TargetBase->GetDistanceToBase(HumanPlayer->GetSelectedBase()) <= 24) {
          AttackBase(HumanPlayer->GetSelectedBase(), TargetBase, Rand);
        }
      }
    }
  }
}
