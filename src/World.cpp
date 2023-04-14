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

#include "World.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Config.hpp"

#define SQUARE_SIDE_WEST  0
#define SQUARE_SIDE_NORTH 1
#define SQUARE_SIDE_EAST  2
#define SQUARE_SIDE_SOUTH 3

World::~World(void)
{
  delete[] Tiles;
  delete[] Bases;
  delete[] BaseArray;
}

bool
World::Create(const int SizeX,
              const int SizeY, const int NumBases, std::minstd_rand &Rand)
{
  this->SizeX = SizeX;
  this->SizeY = SizeY;
  this->NumBases = NumBases;

  if(!GenTerrain(Rand)) { return false; }
  if(!GenBases(Rand)) {
    delete[] Tiles;

    return false;
  }

  this->BaseArraySize = 0;
  this->BaseArray = new Base *[NumBases];
  if(this->BaseArray == nullptr) {
    delete[] Tiles;
    delete[] Bases;

    return false;
  }

  while(!CheckBaseGen()) { GenBases(Rand); }

  return true;
}

void
World::Draw(VideoDriver *Video,
            const int OffsetX, const int OffsetY, const bool DrawGrid)
{
  for(int Y = 0; Y < SizeY; Y++) {
    for(int X = 0; X < SizeX; X++) {
      TileType Type = GetTile(X, Y)->GetType();

      Video->DrawTile(X, Y, OffsetX, OffsetY, Type);
      if(DrawGrid) {
        Video->DrawTile(X, Y, OffsetX, OffsetY, TileType::BlackHighlight);
      }
    }
  }

  for(int I = 0; I < NumBases; I++) {
    Base *TargetBase = &Bases[I];
    int X, Y;

    TargetBase->GetLoc(X, Y);
    Video->DrawTile(X, Y, OffsetX, OffsetY, TileType::Base);
  }
}

void
World::GetNearByBases(const Base *SourceBase, const int MaxDistance)
{
  BaseArraySize = 0;

  for(int I = 0; I < NumBases; I++) { BaseArray[I] = nullptr; }

  for(int I = 0; I < NumBases; I++) {
    Base *TargetBase = &Bases[I];

    if(SourceBase->GetID() != TargetBase->GetID()) {
      int Distance;

      Distance = SourceBase->GetDistanceToBase(TargetBase);
      if(Distance <= MaxDistance) {
        BaseArray[BaseArraySize++] = TargetBase;
      }
    }
  }
}

Base *
World::GetBase(const int BaseNum)
{
  if(BaseNum < 0 || BaseNum >= NumBases) { return nullptr; }

  return &Bases[BaseNum];
}

Base *
World::GetBaseByLoc(const int X, const int Y)
{
  for(int I = 0; I < NumBases; I++) {
    Base *TargetBase = &Bases[I];
    int LocX, LocY;

    TargetBase->GetLoc(LocX, LocY);
    if(LocX == X && LocY == Y) { return TargetBase; }
  }

  return nullptr;
}

Base *
World::GetBaseFromArray(const int BaseNum)
{
  if(BaseNum < 0 || BaseNum >= BaseArraySize) { return nullptr; }

  return BaseArray[BaseNum];
}

void
World::GetSize(int &X, int &Y)
{
  X = SizeX;
  Y = SizeY;
}

Tile *
World::GetTile(const int X, const int Y)
{
  if(X < 0 || X >= SizeX) { return nullptr; }
  if(Y < 0 || Y >= SizeY) { return nullptr; }

  return &Tiles[(Y * SizeX) + X];
}

bool
World::IsBaseBeingAttacked(const Base *SourceBase) const
{
  for(int I = 0; I < NumBases; I++) {
    const Base *AttackingBase = Bases[I].GetAttackingBase();

    if(AttackingBase != nullptr) {
      if(AttackingBase->GetID() == SourceBase->GetID()) {
        return true;
      }
    }
  }

  return false;
}

// Private Functions of World
bool
World::CheckBaseDistance(const int X, const int Y, const int NumBasesBuilt) const
{
  for(int I = 0; I < NumBasesBuilt; I++) {
    int BaseLocX, BaseLocY;
    double Distance;
    int LocX, LocY;

    Bases[I].GetLoc(BaseLocX, BaseLocY);
    LocX = BaseLocX - X;
    LocY = BaseLocY - Y;

    LocX *= LocX;
    LocY *= LocY;

    Distance = ceil(sqrt(LocX + LocY));
    // Check if the base is too close to another base.
    if(Distance <= 2.0) { return true; }
  }

  return false;
}

bool
World::CheckBaseGen(void)
{
  for(int I = 0; I < NumBases; I++) {
    const Base *TargetBase = &Bases[I];

    GetNearByBases(TargetBase, 24);
    if(BaseArraySize < 2) { return false; }
  }

  return true;
}

bool
World::CheckBaseLoc(const int X, const int Y, const int NumBasesBuilt) const
{
  for(int I = 0; I < NumBasesBuilt; I++) {
    int BaseLocX, BaseLocY;

    Bases[I].GetLoc(BaseLocX, BaseLocY);
    if(X == BaseLocX && Y == BaseLocY) { return true; }
  }

  return false;
}

bool
World::DiamondSquare(const int X, const int Y, std::minstd_rand &Rand)
{
  TileType SeedTile;

  // Initial Seeds
  if(GetTile(X, Y)->GetType() == TileType::Water) {
    SeedTile = TileProbablity(Rand() % 1024);
    GetTile(X, Y)->ChangeType(SeedTile);
  }

  if(GetTile(X, Y + 2)->GetType() == TileType::Water) {
    SeedTile = TileProbablity(Rand() % 1024);
    GetTile(X, Y)->ChangeType(SeedTile);
  }

  if(GetTile(X + 2, Y)->GetType() == TileType::Water) {
    SeedTile = TileProbablity(Rand() % 1024);
    GetTile(X, Y)->ChangeType(SeedTile);
  }

  if(GetTile(X + 2, Y + 2)->GetType() == TileType::Water) {
    SeedTile = TileProbablity(Rand() % 1024);
    GetTile(X, Y)->ChangeType(SeedTile);
  }

  if(!DiamondStep(X + 1, Y + 1, Rand))                   { return false; }
  if(!SquareStep(X, Y + 1, SQUARE_SIDE_EAST, Rand))      { return false; }
  if(!SquareStep(X + 1, Y, SQUARE_SIDE_NORTH, Rand))     { return false; }
  if(!SquareStep(X + 2, Y + 1, SQUARE_SIDE_WEST, Rand))  { return false; }
  if(!SquareStep(X + 1, Y + 2, SQUARE_SIDE_SOUTH, Rand)) { return false; }

  return true;
}

bool
World::DiamondStep(const int X, const int Y, std::minstd_rand &Rand)
{
  double Weight;
  TileType Type;
  int Height;

  Weight = 0.0;
  Weight += GetTile(X - 1, Y - 1)->GetHeightValue();
  Weight += GetTile(X - 1, Y + 1)->GetHeightValue();
  Weight += GetTile(X + 1, Y - 1)->GetHeightValue();
  Weight += GetTile(X + 1, Y + 1)->GetHeightValue();

  Height = (Weight / 4.0) + ((Rand() % 1000) / 500.0);
  Type = GetTileTypeFromHeight(Height);
  if(Type == TileType::Grassland) {
    if(Rand() % WORLDGEN_CHANCE_FOR_FOREST_TILE ==  0) { Type = TileType::Forest; }
  }

  GetTile(X, Y)->ChangeType(Type);

  return true;
}

bool
World::GenBases(std::minstd_rand &Rand)
{
  Tile *TargetTile;
  int NumBasesBuilt = 0;
  int BaseIDNum = 1;

  Bases = new Base[NumBases];
  if(Bases == nullptr) { return false; }

  for(int X = 0; X < 2; X++) {
    for(int Y = 0; Y < 2; Y++) {
      for(int J = 0; J < (NumBases / 4); ) {
        int LocX, LocY;

        LocX = (X * (SizeX / 2)) + (Rand() % (SizeX / 2));
        LocY = (Y * (SizeY / 2)) + (Rand() % (SizeY / 2));
        if(LocX > SizeX || LocY > SizeY) { abort(); }

        TargetTile = GetTile(LocX, LocY);
        if(TargetTile->GetType() == TileType::Water ||
           CheckBaseDistance(LocX, LocY, NumBasesBuilt) == true ||
           CheckBaseLoc(LocX, LocY, NumBasesBuilt) == true) {
          continue;
        }

        Bases[NumBasesBuilt++].Create(LocX, LocY, BaseIDNum, (Rand() % 9) + 1, TargetTile->GetDefenseValue());
        J++;
        BaseIDNum++;
      }
    }
  }

  return true;
}

bool
World::GenTerrain(std::minstd_rand &Rand)
{
  int X, Y;

  Tiles = new Tile[SizeX * SizeY];
  if(Tiles == nullptr) { return false; }

  // Initialize all the tiles as water.
  for(int Y = 0; Y < SizeY; Y++) {
    for(int X = 0; X < SizeX; X++) {
      GetTile(X, Y)->Create(TileType::Water, X, Y);
    }
  }

  for(int I = 0; I < (NUMBER_OF_MAPGEN_ITERATIONS / 4); I++) {
    X = Rand() % (((SizeX / 2) - 3) + 1);
    Y = Rand() % (((SizeY / 2) - 3) + 1);

    if(!DiamondSquare(X, Y, Rand)) { return false; }
  }

  for(int I = 0; I < (NUMBER_OF_MAPGEN_ITERATIONS / 4); I++) {
    X = Rand() % (((SizeX / 2) - 3) + 1);
    Y = Rand() % (((SizeY / 2) - 3) + 1);

    if(!DiamondSquare((SizeX / 2) + X, Y, Rand)) { return false; }
  }

  for(int I = 0; I < (NUMBER_OF_MAPGEN_ITERATIONS / 4); I++) {
    X = Rand() % (((SizeX / 2) - 3) + 1);
    Y = Rand() % (((SizeY / 2) - 3) + 1);

    if(!DiamondSquare(X, (SizeY / 2) + Y, Rand)) { return false; }
  }

  for(int I = 0; I < (NUMBER_OF_MAPGEN_ITERATIONS / 4); I++) {
    X = Rand() % (((SizeX / 2) - 3) + 1);
    Y = Rand() % (((SizeY / 2) - 3) + 1);

    if(!DiamondSquare((SizeX / 2) + X, (SizeY / 2) + Y, Rand)) { return false; }
  }

  return true;
}

bool
World::SquareStep(const int X,
                  const int Y, const int Side, std::minstd_rand &Rand)
{
  double Average;
  double Weight;
  TileType Type;
  int Height;

  Average = 0.0;
  Weight = 0.0;
  switch(Side) {
  case SQUARE_SIDE_EAST:
    if(X + 1 < SizeX) {
      Weight += GetTile(X + 1, Y)->GetHeightValue();
      Average += 1.0;
    }
    if(X - 1 >= 0) {
      Weight += GetTile(X - 1, Y)->GetHeightValue();
      Average += 1.0;
    }
    if(Y - 1 >= 0) {
      Weight += GetTile(X, Y - 1)->GetHeightValue();
      Average += 1.0;
    }
    if(Y + 1 < SizeY) {
      Weight += GetTile(X, Y + 1)->GetHeightValue();
      Average += 1.0;
    }
    break;
  case SQUARE_SIDE_NORTH:
    if(X - 1 <= 0) {
      Weight += GetTile(X - 1, Y)->GetHeightValue();
      Average += 1.0;
    }
    if(X + 1 < SizeX) {
      Weight += GetTile(X + 1, Y)->GetHeightValue();
      Average += 1.0;
    }
    if(Y - 1 >= 0) {
      Weight += GetTile(X, Y - 1)->GetHeightValue();
      Average += 1.0;
    }
    if(Y + 1 < SizeY) {
      Weight += GetTile(X, Y + 1)->GetHeightValue();
      Average += 1.0;
    }
    break;
  case SQUARE_SIDE_WEST:
    if(X - 1 >= 0) {
      Weight += GetTile(X - 1, Y)->GetHeightValue();
      Average += 1.0;
    }
    if(X + 1 < SizeX) {
      Weight += GetTile(X + 1, Y)->GetHeightValue();
      Average += 1.0;
    }
    if(Y - 1 <= 0) {
      Weight += GetTile(X, Y - 1)->GetHeightValue();
      Average += 1.0;
    }
    if(Y + 1 < SizeY) {
      Weight += GetTile(X, Y + 1)->GetHeightValue();
      Average += 1.0;
    }
    break;
  case SQUARE_SIDE_SOUTH:
    if(X - 1 >= 0) {
      Weight += GetTile(X - 1, Y)->GetHeightValue();
      Average += 1.0;
    }
    if(X + 1 < SizeX) {
      Weight += GetTile(X + 1, Y)->GetHeightValue();
      Average += 1.0;
    }
    if(Y - 1 >= 0) {
      Weight += GetTile(X, Y - 1)->GetHeightValue();
      Average += 1.0;
    }
    if(Y + 1 < SizeY) {
      Weight += GetTile(X, Y + 1)->GetHeightValue();
      Average += 1.0;
    }
    break;
  default:
    abort();
  }

  Height = (Weight / Average) + ((Rand() % 1000) / 500.0);
  Type = GetTileTypeFromHeight(Height);
  if(Type == TileType::Grassland) {
    if(Rand() % WORLDGEN_CHANCE_FOR_FOREST_TILE == 0) { Type = TileType::Forest; }
  }

  GetTile(X, Y)->ChangeType(Type);

  return true;
}

TileType
World::TileProbablity(const unsigned int Value)
{
  if(Value < 64)                   { return TileType::Water; }
  if(Value >= 64 && Value < 512)   { return TileType::Grassland; }
  if(Value >= 512 && Value < 768)  { return TileType::Hill; }
  if(Value >= 768 && Value < 1024) { return TileType::Mountain; }

  /* Should never get here. */
  abort();
}
