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

#include "Tile.hpp"

#include <cmath>
#include <cstdlib>

int
Tile::GetDefenseValue(void) const
{
  switch(Type) {
  case TileType::Grassland:
    return 0;
  case TileType::Forest:
    return 4;
  case TileType::Hill:
    return 6;
  case TileType::Mountain:
    return 10;
  default:
    abort();
  }

  /* Should never get here. */
  return -1;
}

double
Tile::GetDistanceToTile(const Tile *TargetTile) const
{
  int TargetTileLocX, TargetTileLocY;
  int X, Y;

  TargetTile->GetLoc(TargetTileLocX, TargetTileLocY);

  X = LocX - TargetTileLocX;
  Y = LocY - TargetTileLocY;

  X *= X;
  Y *= Y;

  return ceil(sqrt(X + Y));
}

int
Tile::GetHeightValue(void) const
{
  switch(Type) {
  case TileType::Water:
    return 0;
  case TileType::Grassland:
  case TileType::Forest:
    return 1;
  case TileType::Hill:
    return 2;
  case TileType::Mountain:
    return 3;
  default:
    abort();
  }

  /* Should never get here. */
  return -1;
}

TileType GetTileTypeFromHeight(const int Height)
{
  switch(Height) {
  case 0:
    return TileType::Water;
  case 1:
    return TileType::Grassland;
  case 2:
    return TileType::Hill;
  default:
    return TileType::Mountain;
  }
}
