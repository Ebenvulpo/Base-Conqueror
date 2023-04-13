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

#include "Player.hpp"

#include "Ai.hpp"
#include "Game.hpp"

Player::~Player(void)
{
  if(AI != nullptr) { delete ((AIDriver *) AI); }
}

void
Player::ChangeHomeBase(Base *HomeBase)
{
  this->HomeBase = HomeBase;
}

void
Player::ChangeSelectedBase(Base *SelectedBase)
{
  this->SelectedBase = SelectedBase;
}

bool
Player::Create(const PlayerID ID, const bool Human)
{
  this->Human = Human;
  this->Alive = true;
  this->ID = ID;
  this->Score = 0;
  this->SelectedBase = nullptr;
  this->HomeBase = nullptr;
  this->AI = nullptr;

  if(!Human) {
    this->AI = (void *) new AIDriver();

    if(!((AIDriver * ) AI)->Create(this)) {
      delete ((AIDriver *) this->AI);

      return false;
    }
  }

  return true;
}

void
Player::Tick(void *MainGame, const double Delta, std::minstd_rand &Rand)
{
  Game *MainGame_ = (Game *) MainGame;

  if(AI != nullptr) {
    ((AIDriver *) AI)->Tick(MainGame_, Delta, Rand);
  }
}
