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

#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_ 1

class Player;

typedef unsigned int PlayerID;

#include <random>

#include "Base.hpp"

class Player {
private:
  bool Human;
  bool Alive;
  PlayerID ID;
  int Score;
  Base *SelectedBase;
  Base *HomeBase;

  void *AI;

public:
  ~Player(void);
  Player(void) { }
  void ChangeHomeBase(Base *HomeBase);
  void ChangeScore(const int NewScore) { Score = NewScore; }
  void ChangeSelectedBase(Base *SelectedBase);
  bool Create(const PlayerID ID, const bool Human);
  void Dead(void) { Alive =  false; }
  Base *GetHomeBase(void) { return HomeBase; }
  PlayerID GetID(void) const { return ID; }
  int GetScore(void) const { return Score; }
  Base *GetSelectedBase(void) const { return SelectedBase; }
  bool IsAlive(void) const { return Alive; }
  bool IsHuman(void) const { return Human; }
  void Tick(void *MainGame, const double Delta, std::minstd_rand &Rand);
};

#endif /* _PLAYER_HPP_ */
