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

#ifndef _GAME_HPP_
#define _GAME_HPP_

class Game;

#include <random>
#include <SDL2/SDL_events.h>

#include "Base.hpp"
#include "Config.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "Video.hpp"
#include "Window.hpp"
#include "World.hpp"

class Game {
private:
  bool ShowGrid;
  int Offset_X;
  int Offset_Y;
  int Mouse_OldLocX;
  int Mouse_OldLocY;
  double EndGameTick;

  int DifficultyValue;

  bool CloseMainWindow;
  Window *NewWindow;
  Window *MainWindow;

  World *GameWorld;

  Player *HumanPlayer;
  Player Players[NUMBER_OF_PLAYERS];

  void AssignPlayerBases(std::minstd_rand &Rand);
  void AttackBase(Base *SrcBase, Base *TargetBase, std::minstd_rand &Rand);
  void BasesTick(const double Delta, std::minstd_rand &Rand);
  void CenterOnBase(VideoDriver *Video, const Base *TargetBase);
  void CheckMapBoarder(VideoDriver *Video, const int X, const int Y);
  void CheckIfAIPlayersAreAlive(const double Delta, VideoDriver *Video);
  void CheckIfHumanPlayerIsAlive(const double Delta, VideoDriver *Video);
  void CheckIfPlayersAreAlive(void);
  void CheckMusic(std::minstd_rand &Rand);
  void CheckToCloseWindow(void);
  void CreatePlayers(void);
  void DestroyLink(void);
  void DrawAttacks(VideoDriver *Video);
  void DrawBaseStatus(VideoDriver *Video);
  void DrawBaseStatusEnemy(VideoDriver *Video, const Base *TargetBaes);
  void DrawBaseStatusOwner(VideoDriver *Video, const Base *TargetBase);
  void DrawBaseStatusUnowned(VideoDriver *Video, const Base *TargetBase);
  void DrawHighlight(VideoDriver *Video);
  void DrawLinks(VideoDriver *Video);
  void DrawOwnerHighlights(VideoDriver *Video);
  void DrawSelectedBase(VideoDriver *Video);
  void DrawStatusRect(VideoDriver *Video,
                      const int Width,
                      const int Height,
                      const int X, const int Y);
  void FindNearbyEnemyBase(VideoDriver *Video, const Player *TargetPlayer);
  void GetMousePointerTileLoc(const int X,
                              const int Y,
                              int &TileLocX, int &TileLocY);
  void GetTilePixelLoc(const int X,
                       const int Y,
                       int &PixelX, int &PixelY);
  void LinkBases(Base *SrcBase, Base *DstBase);
  void PlayersTick(const double Delta, std::minstd_rand &Rand);
  void SelectedTileInput(const int X,
                         const int Y, std::minstd_rand &Rand);
public:
  ~Game(void);
  Game(void) { }
  void ChangeMainWindow(Window *NewWindow);
  void ChangeShowGrid(const bool Value) { ShowGrid = Value; }
  void CloseWindow(void);
  void ColonizeBase(Base *TargetBase, Player *TargetPlayer);
  void Draw(VideoDriver *Video);
  void FindNewHomeBase(Player *TargetPlayer);
  bool GetGridValue(void) const { return ShowGrid; }
  Player *GetHumanPlayer(void) { return HumanPlayer; }
  Player *GetPlayer(const int I);
  World *GetWorld(void) { return GameWorld; }
  bool Init(const int DifficultyValue,
            VideoDriver *Video, std::minstd_rand &Rand);
  void Input(SDL_Event *Event,
             VideoDriver *Video, std::minstd_rand &Rand);
  bool IsWindowOpen(void) const { return (MainWindow != nullptr) ? true : false; }
  void Tick(const double Delta, VideoDriver *Video, std::minstd_rand &Rand);
  TileType TileProbablity(const unsigned int RandValue);
};

#endif /* _GAME_HPP_ */
