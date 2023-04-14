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

#include "Base.hpp"

#include <cmath>

#include "Application.hpp"

extern BcApplication *Application;

bool
Base::AttackBase(Base *TargetBase, std::minstd_rand &Rand)
{
  if(TargetBase == nullptr) {
    AttackingBase = nullptr;

    return true;
  }

  if(TargetBase->GetOwner() == this->Owner) { return false; }
  if(this->Soldiers < 10.0)                 { return false; }
  if(this->TargetBase != nullptr)           { return false; }

  this->AttackingBase = TargetBase;
  this->AttackRoll = (Rand() % 16) + 1;
  this->DefenseRoll = ((Rand() % 16) + 1) + TargetBase->GetDefenceValue();

  Application->GetAudioDriver()->PlayAttackingSound();

  return true;
}

bool
Base::ChangeTargetBase(Base *TargetBase)
{
  if(TargetBase == nullptr) {
    this->TargetBase = nullptr;

    return true;
  }

  if(TargetBase->GetTargetBase() != nullptr) {
    if(TargetBase->GetTargetBase()->GetID() == this->GetID()) { return false; }
  }

  if(TargetBase->GetOwner() != Owner) { return false; }

  this->TargetBase = TargetBase;

  return true;
}

bool
Base::Colonize(Player *NewOwner, Base *SourceBase)
{
  if(this->Owner != nullptr)           { return false; }
  if(SourceBase->GetSoldiers() < 10.0) { return false; }

  NewOwner->ChangeScore(NewOwner->GetScore() + 1);

  this->Owner = NewOwner;
  this->Soldiers = 5.0;

  SourceBase->ChangeSoldiers(SourceBase->GetSoldiers() - 5.0);

  return true;
}

void
Base::Create(const int LocX,
             const int LocY,
             const BaseID ID,
             const int Size, const int DefenceValue)
{
  this->LocX = LocX;
  this->LocY = LocY;
  this->Size = Size;
  this->ID = ID;
  this->DefenceValue = DefenceValue;
  this->Soldiers = 0.0;
  this->AttackingBase = nullptr;
  this->TargetBase = nullptr;
  this->Owner = nullptr;
}

int
Base::GetDistanceToBase(const Base *TargetBase) const
{
  int TargetBaseLocX, TargetBaseLocY;
  int X, Y;

  TargetBase->GetLoc(TargetBaseLocX, TargetBaseLocY);

  X = TargetBaseLocX - LocX;
  Y = TargetBaseLocY - LocY;

  X *= X;
  Y *= Y;

  return ceil(sqrt(Y + X));
}

void
Base::Tick(const double Delta, const int DifficultyValue, std::minstd_rand &Rand)
{
  double AiDifficulityModifier;

  switch(DifficultyValue) {
  case 0:
    AiDifficulityModifier = 0.5;
    break;
  case 1:
    AiDifficulityModifier = 0.75;
    break;
  case 2:
    AiDifficulityModifier = 1.0;
    break;
  case 3:
    AiDifficulityModifier = 1.15;
    break;
  case 4:
    AiDifficulityModifier = 1.3;
    break;
  default:
    abort();
  }

  if(Owner != nullptr) {
    double NewSoldiers;

    if(Owner->IsHuman()) {
      NewSoldiers = ((Delta * (double) Size) / 10.0);
    } else {
      NewSoldiers = ((Delta * (double) Size) / 10.0) * AiDifficulityModifier;
    }

    // Yes, there is actually a limit on how many soldiers a base can hold...
    if(this->Soldiers + NewSoldiers > 1000000000.0) {
      Soldiers = 1000000000.0;
    } else {
      Soldiers += NewSoldiers;
    }

    if(TargetBase != nullptr) {
      /* Check if the target base has changed owners. */
      if(TargetBase->GetOwner() != Owner) {
        TargetBase = nullptr;
      } else {
        int Distance;
        double Move;

        Distance = this->GetDistanceToBase(TargetBase);

        if(Distance > 6) {
          Move = (Soldiers * (0.25 - ((0.2 / 18.0) * (double) (Distance - 6)))) * Delta;
        } else {
          Move = (Soldiers * 0.25) * Delta;
        }

        if(TargetBase->GetSoldiers() + Move < 1000000000.0) {
          Soldiers -= Move;
          TargetBase->ChangeSoldiers(TargetBase->GetSoldiers() + Move);
        }
      }
    }

    if(AttackingBase != nullptr && this->Owner != nullptr) {
      const double Roll = (4.0 + (double) AttackRoll) / (4.0 + (double) DefenseRoll);

      double AttackingSoldiers;
      int Distance;

      Distance = this->GetDistanceToBase(AttackingBase);

      if(Distance > 6) {
        AttackingSoldiers = (this->Soldiers * (1.0 - ((0.75 / 18.0) * (double) (Distance - 6)))) * Delta;
      } else {
        AttackingSoldiers = this->Soldiers * Delta;
      }

      if(AttackingBase->GetOwner() == Owner) {
        AttackingBase = nullptr;

        return;
      }

      if(AttackingSoldiers >= this->Soldiers - 10.0) {
        AttackingSoldiers = this->Soldiers - 9.9;
      }

      this->Soldiers -= AttackingSoldiers;
      AttackingBase->ChangeSoldiers(AttackingBase->GetSoldiers() - (AttackingSoldiers * Roll));

      if(AttackingBase->GetSoldiers() <= 0.0) {
        Owner->ChangeScore(Owner->GetScore() + 3);

        AttackingBase->AttackBase(nullptr, Rand);
        AttackingBase->ChangeOwner(this->Owner);
        AttackingBase->ChangeTargetBase(nullptr);
        AttackingBase->ChangeSoldiers(this->Soldiers * 0.5);
        this->Soldiers *= 0.5;

        if(Application->GetGame()->GetHumanPlayer()->GetHomeBase() != nullptr) {
          if(AttackingBase->GetID() == Application->GetGame()->GetHumanPlayer()->GetHomeBase()->GetID()) {
            Application->GetGame()->FindNewHomeBase(Application->GetGame()->GetHumanPlayer());
          }
        }

        AttackingBase = nullptr;
      } else if(this->Soldiers <= 10.0) {
        AttackingBase->GetOwner()->ChangeScore(AttackingBase->GetOwner()->GetScore() + 1);

        AttackingBase = nullptr;
      }
    }
  }
}
