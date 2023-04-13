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

#ifndef _BASE_HPP_
#define _BASE_HPP_

class Base;

typedef unsigned int BaseID;

#include <random>

#include "Player.hpp"

class Base {
private:
  int LocX, LocY;
  int Size;
  int DefenceValue;
  BaseID ID;
  double Soldiers;
  Base *AttackingBase;
  int AttackRoll;
  int DefenseRoll;
  Base *TargetBase;
  Player *Owner;

public:
  ~Base(void) { }
  Base(void) { }
  bool AttackBase(Base *TargetBase, std::minstd_rand &Rand);
  void ChangeOwner(Player *Owner) { this->Owner = Owner; }
  void ChangeSize(const int Value) { this->Size = Value; }
  void ChangeSoldiers(const double Soldiers) { this->Soldiers = Soldiers; }
  bool ChangeTargetBase(Base *TargetBase);
  bool Colonize(Player *Owner, Base *SourceBase);
  void Create(const int LocX,
              const int LocY,
              const BaseID ID,
              const int Size, const int DefenceValue);
  Base *GetAttackingBase(void) const { return AttackingBase; }
  int GetDefenceValue(void) const { return DefenceValue; }
  int GetDistanceToBase(const Base *TargetBase) const;
  BaseID GetID(void) const { return ID; }
  void GetLoc(int &X, int &Y) const { X = LocX; Y = LocY; }
  Player *GetOwner(void) const { return Owner; }
  int GetSize(void) const { return Size; }
  double GetSoldiers(void) const { return Soldiers; }
  Base *GetTargetBase(void) const { return TargetBase; }
  bool IsAttacking(void) const { return (AttackingBase != nullptr) ? true : false; }
  void Tick(const double Tick, const int DifficultyValue, std::minstd_rand &Rand);
};

#endif /* _BASE_HPP_ */
