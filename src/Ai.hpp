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

#ifndef _AI_HPP_
#define _AI_HPP_

class AIDriver;

#include <random>

#include "Game.hpp"
#include "Player.hpp"

class AIDriver {
private:
  double Ticks;
  Player *PlayerToControl;

  void CheckBases(Game *MainGame, std::minstd_rand &Rand);
  void CheckBasesToAttack(Base *PlayerBase, Game *MainGame, std::minstd_rand &Rand);
  void CheckBasesToColonize(Base *PlayerBase, Game *MainGame, std::minstd_rand &Rand);
  void LinkNearByBases(Base *PlayerBase, Game *MainGame);
  void ManageBase(Base *PlayerBase, Game *MainGame, std::minstd_rand &Rand);
  void MaybeUnlinkBase(Base *PlayerBase, Game *MainGame, std::minstd_rand &Rand);
public:
  AIDriver(void) { }
  ~AIDriver(void) { }
  bool Create(Player *PlayerToControl);
  void Tick(Game *MainGame, const double Delta, std::minstd_rand &Rand);
};

#endif // _AI_HPP_
