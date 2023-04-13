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

#ifndef _WORLD_HPP_
#define _WORLD_HPP_

class World;

#include <random>

#include "Base.hpp"
#include "Tile.hpp"
#include "Video.hpp"

class World {
private:
  int SizeX;
  int SizeY;
  Tile *Tiles;

  int NumBases;
  Base *Bases;

  int BaseArraySize;
  Base **BaseArray;

  bool CheckBaseDistance(const int X, const int Y, const int NumBasesBuilt) const;
  bool CheckBaseGen(void);
  bool CheckBaseLoc(const int X, const int Y, const int NumBasesBuilt) const;
  bool DiamondSquare(const int X, const int Y, std::minstd_rand &Rand);
  bool DiamondStep(const int X, const int Y, std::minstd_rand &Rand);
  bool GenBases(std::minstd_rand &Rand);
  bool GenTerrain(std::minstd_rand &Rand);
  bool SquareStep(const int X,
                  const int Y, const int Side, std::minstd_rand &Rand);
  TileType TileProbablity(const unsigned int Value);
public:
  World(void) { }
  ~World(void);
  bool Create(const int SizeX,
              const int SizeY, const int NumBases, std::minstd_rand &Rand);
  void Draw(VideoDriver *Video,
            const int OffsetX, const int OffsetY, const bool DrawGrid);
  int GetBaseArraySize(void) const { return BaseArraySize; }
  Base *GetBase(const int BaseNum);
  Base *GetBaseByLoc(const int X, const int Y);
  Base *GetBaseFromArray(const int BaseNum);
  void GetNearByBases(const Base *SourceBase, const int MaxDistance);
  int GetNumBases(void) const { return NumBases; }
  void GetSize(int &X, int &Y);
  Tile *GetTile(const int X, const int Y);
  bool IsBaseBeingAttacked(const Base *SourceBase) const;
};

#endif // _WORLD_HPP_
