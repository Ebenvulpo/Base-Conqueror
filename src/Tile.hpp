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

#ifndef _TILE_HPP_
#define _TILE_HPP_

class Tile;

enum class TileType {
  Base,
  Forest,
  Grassplane,
  Hill,
  Mountain,
  BlackHighlight,
  BrownHighlight,
  CyanHighlight,
  GrayHighlight,
  RedHighlight,
  PurpleHighlight,
  YellowHighlight,
  WhiteHighlight,
  Water,
};

class Tile {
private:
  TileType Type;
  int LocX, LocY;

public:
  ~Tile(void) { }
  Tile(void) { }
  void ChangeType(const TileType Type) { this->Type = Type; }
  void Create(const TileType Type,
              const int LocX, const int LocY) { this->Type = Type; this->LocX = LocX; this->LocY = LocY; }
  int GetDefenseValue(void) const;
  double GetDistanceToTile(const Tile *TargetTile) const;
  int GetHeightValue(void) const;
  TileType GetType(void) const { return this->Type; }
  void GetLoc(int &X, int &Y) const { X = LocX; Y = LocY; }
};

TileType GetTileTypeFromHeight(const int Height);

#endif /* _TILE_HPP_ */
