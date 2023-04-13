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

#ifndef _VIDEO_HPP_
#define _VIDEO_HPP_

#include <SDL2/SDL_render.h>

class VideoDriver;

#define NUM_TEXTURES 14

#include <SDL2/SDL_video.h>

#include "Tile.hpp"

class VideoDriver {
private:
  SDL_Window *MainWindow;
  SDL_Renderer *MainRenderer;
  SDL_Texture *Textures[NUM_TEXTURES];
  SDL_Texture *Cursor;
  SDL_Texture *Fonts;
  int WindowWidth, WindowHeight;

  void DrawChar(const char Ch, const int X, const int Y, const double Scale);
  bool LoadCursor(const char *ApplicationPath);
  bool LoadFonts(const char *ApplicationPath);
  bool LoadIcon(const char *ApplicationPath);
  bool LoadTextures(const char *ApplicationPath);
public:
  ~VideoDriver(void);
  VideoDriver(void) { }
  bool ChangeFullscreenMode(const bool Fullscreen);
  void Clear(void);
  void DrawBox(const int Width,
               const int Height,
               const int X,
               const int Y,
               const Uint8 Red,
               const Uint8 Green,
               const Uint8 Blue, const Uint8 Alpha);
  void DrawCursor(const int X, const int Y);
  void DrawLine(const int StartX,
                const int StartY,
                const int EndX,
                const int EndY,
                const Uint8 Red,
                const Uint8 Green,
                const Uint8 Blue, const Uint8 Alpha);
  void DrawRect(const int Width,
                const int Height,
                const int X,
                const int Y,
                const Uint8 Red,
                const Uint8 Green,
                const Uint8 Blue, const Uint8 Alpha);
  void DrawStr(const char *String, const int X, const int Y, const double Scale);
  void DrawTexture(SDL_Texture *Texture,
                   const int Width,
                   const int Height,
                   const int SourceX,
                   const int SourceY,
                   const int SourceWidth,
                   const int SourceHeight,
                   const int X, const int Y);
  void DrawTile(const int X,
                const int Y,
                const int Offset_X,
                const int Offset_Y, const TileType Type);
  void Finish(void);
  SDL_Renderer *GetRenderer(void) const { return MainRenderer; }
  void GetWindowSize(int &Width, int &Height);
  bool Init(const char *ApplicationPath);
  SDL_Texture *LoadTexture(const char *Filename);
};

#endif /* _VIDEO_HPP_ */
