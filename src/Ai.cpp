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

#include "Ai.hpp"

#include <iostream>

#include "Debug.hpp"
#include "World.hpp"

bool
AIDriver::Create(Player *PlayerToControl)
{
  this->Ticks = 0.0;
  if(PlayerToControl == nullptr) { return false; }
  this->PlayerToControl = PlayerToControl;

  return true;
}

void
AIDriver::Tick(Game *MainGame, const double Delta, std::minstd_rand &Rand)
{
  Ticks += Delta;
  if(Ticks >= (double) AI_DECISION_TIME) {
#if DEBUG_AI
    std::cerr << "AI: Player: " << PlayerToControl->GetID() << " Tick." << std::endl;
#endif /* DEBUG_AI */

    CheckBases(MainGame, Rand);
    Ticks = 0.0;
  }
}

// Private Functions of AIDriver
void
AIDriver::CheckBases(Game *MainGame, std::minstd_rand &Rand)
{
  World *GameWorld;

  GameWorld = MainGame->GetWorld();
  for(int I = 0; I < GameWorld->GetNumBases(); I++) {
    Base *PlayerBase = GameWorld->GetBase(I);

    if(PlayerBase->GetOwner() != nullptr) {
      if(PlayerBase->GetOwner()->GetID() == PlayerToControl->GetID()) {
        ManageBase(PlayerBase, MainGame, Rand);
      }
    }
  }
}

void
AIDriver::CheckBasesToAttack(Base *PlayerBase, Game *MainGame, std::minstd_rand &Rand)
{
  World *GameWorld;

  GameWorld = MainGame->GetWorld();

  for(int I = 0; I < GameWorld->GetBaseArraySize(); I++) {
    Base *TargetBase = GameWorld->GetBaseFromArray(I);

    if(TargetBase->GetOwner() != nullptr) {
      unsigned int ChanceToAttack;

      if(TargetBase->GetTargetBase() != nullptr) {
        ChanceToAttack = AI_CHANCE_TO_ATTACK / 2;
      } else {
        ChanceToAttack = AI_CHANCE_TO_ATTACK;
      }

      if(TargetBase->GetOwner()->GetID() != PlayerToControl->GetID() &&
         (Rand() % ChanceToAttack) == 0) {
#if DEBUG_AI
        std::cerr << "AI: Attacking base: " << TargetBase->GetID()
                  << " from base: "         << PlayerBase->GetID() << std::endl;
#endif /* DEBUG_AI  */

        PlayerBase->AttackBase(TargetBase, Rand);
        LinkNearByBases(PlayerBase, MainGame);
      }
    }
  }
}

void
AIDriver::CheckBasesToColonize(Base *PlayerBase, Game *MainGame, std::minstd_rand &Rand)
{
  World *GameWorld;

  if(PlayerBase->GetTargetBase() != nullptr) { return; }

  GameWorld = MainGame->GetWorld();

  for(int I = 0; I < GameWorld->GetBaseArraySize(); I++) {
    Base *ClosestBase = GameWorld->GetBaseFromArray(0);

    // Find the closest non-occupied base.
    for(int J = 0; J < GameWorld->GetBaseArraySize(); J++) {
      Base *TargetBase = GameWorld->GetBaseFromArray(J);

      if(TargetBase->GetOwner() == nullptr &&
         PlayerBase->GetDistanceToBase(TargetBase) <
         PlayerBase->GetDistanceToBase(ClosestBase)) {
        ClosestBase = TargetBase;
      }
    }

    if(Rand() % AI_CHANCE_TO_COLONIZE) {
      if(ClosestBase->GetOwner() == nullptr) {
#if DEBUG_AI
        std::cerr << "AI: Colonize base: " << ClosestBase->GetID()
                  << " from "              << PlayerBase->GetID() << std::endl;
#endif /* DEBUG_AI */

        ClosestBase->Colonize(PlayerToControl, PlayerBase);

        return;
      }
    }
  }
}

void
AIDriver::LinkNearByBases(Base *PlayerBase, Game *MainGame)
{
  World *GameWorld;

  GameWorld = MainGame->GetWorld();

  for(int I = 0; I < GameWorld->GetBaseArraySize(); I++) {
    Base *TargetBase = GameWorld->GetBaseFromArray(I);

    if(!TargetBase->IsAttacking() &&
       !GameWorld->IsBaseBeingAttacked(TargetBase)) {
#if DEBUG_AI
      std::cerr << "AI: Linking base: " << TargetBase->GetID()
                << " to "               << PlayerBase->GetID() << std::endl;
#endif /* _DEBUG_AI */

      TargetBase->ChangeTargetBase(PlayerBase);
    }
  }
}

void
AIDriver::ManageBase(Base *PlayerBase, Game *MainGame, std::minstd_rand &Rand)
{
  World *GameWorld;

  // Too few soldiers to do anything really useful.
  if(PlayerBase->GetSoldiers() < 10.0) { return; }

  GameWorld = MainGame->GetWorld();

  GameWorld->GetNearByBases(PlayerBase, 24);
  if(GameWorld->GetBaseArraySize() > 0) {
    if(PlayerBase->IsAttacking() ||
       GameWorld->IsBaseBeingAttacked(PlayerBase)) {
      LinkNearByBases(PlayerBase, MainGame);
      PlayerBase->ChangeTargetBase(nullptr);
    } else {
      CheckBasesToColonize(PlayerBase, MainGame, Rand);
      CheckBasesToAttack(PlayerBase, MainGame, Rand);
      MaybeUnlinkBase(PlayerBase, MainGame, Rand);
    }
  }
}

void
AIDriver::MaybeUnlinkBase(Base *PlayerBase, Game *MainGame, std::minstd_rand &Rand)
{
  World *GameWorld;

  GameWorld = MainGame->GetWorld();

  if(GameWorld->IsBaseBeingAttacked(PlayerBase)) {
#if DEBUG_AI
    std::cerr << "AI: Unlinking base: " << PlayerBase->GetID()
              << " for defense."        << std::endl;
#endif /* DEBUG_AI */

    PlayerBase->ChangeTargetBase(nullptr);

    return;
  }

  if(PlayerBase->GetTargetBase() != nullptr) {
    if(!GameWorld->IsBaseBeingAttacked(PlayerBase->GetTargetBase()) &&
       (Rand() % AI_CHANCE_TO_UNLINK_BASE) == 0) {
#if DEBUG_AI
      std::cerr << "AI: Unlinking base: "         << PlayerBase->GetID()
                << " since it is doing nothing. " << std::endl;
#endif /* DEBUG_AI */
      // Check if an enemy base is nearby.
      for(int I = 0; I < GameWorld->GetBaseArraySize(); I++) {
        const Base *TargetBase = GameWorld->GetBaseFromArray(I);

        if(TargetBase->GetOwner() != nullptr) {
          if(TargetBase->GetOwner()->GetID() != PlayerBase->GetOwner()->GetID()) {
            // If an enemy is nearby, then unlink the base.
            PlayerBase->ChangeTargetBase(nullptr);
          }
        }
      }
    }
  }
}
