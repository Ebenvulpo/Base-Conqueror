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

#include "Video.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

#include "Application.hpp"
#include "Config.hpp"

extern BcApplication *Application;

static SDL_Surface *LoadSurfaceFromPath(const char *Application, const char *SurfaceName);
static SDL_Texture *LoadTextureFromPath(SDL_Renderer *Renderer,
                                        const char *ApplicationPath, const char *TextureName);

VideoDriver::~VideoDriver(void)
{
  for(int I = 0; I < NUM_TEXTURES; I++) { SDL_DestroyTexture(Textures[I]); }

  IMG_Quit();

  SDL_DestroyTexture(Cursor);
  SDL_DestroyTexture(Fonts);
  SDL_DestroyRenderer(MainRenderer);
  SDL_DestroyWindow(MainWindow);
}

bool
VideoDriver::ChangeFullscreenMode(const bool Fullscreen)
{
  if(Fullscreen) {
    int Error;
    Error = SDL_SetWindowFullscreen(MainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(Error < 0) { return false; }
  } else {
    SDL_SetWindowFullscreen(MainWindow, 0);

    return false;
  }

  return true;
}

void
VideoDriver::Clear(void)
{
  SDL_SetRenderDrawColor(MainRenderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(MainRenderer);
}

void
VideoDriver::DrawBox(const int Width,
                     const int Height,
                     const int X,
                     const int Y,
                     const Uint8 Red,
                     const Uint8 Green,
                     const Uint8 Blue, const Uint8 Alpha)
{
  SDL_Rect Rect;

  Rect.x = X;
  Rect.y = Y;
  Rect.w = Width;
  Rect.h = Height;

  SDL_SetRenderDrawColor(MainRenderer, Red, Green, Blue, Alpha);
  SDL_RenderDrawRect(MainRenderer, &Rect);
}

void
VideoDriver::DrawCursor(const int X, const int Y)
{
  SDL_Rect Dest;

  Dest.x = X;
  Dest.y = Y;
  Dest.w = 16;
  Dest.h = 16;

  SDL_RenderCopy(MainRenderer, Cursor, nullptr, &Dest);
}

void
VideoDriver::DrawLine(const int StartX,
                      const int StartY,
                      const int EndX,
                      const int EndY,
                      const Uint8 Red,
                      const Uint8 Green,
                      const Uint8 Blue, const Uint8 Alpha)
{
  SDL_SetRenderDrawColor(MainRenderer, Red, Green, Blue, Alpha);
  SDL_RenderDrawLine(MainRenderer, StartX, StartY, EndX, EndY);
}

void
VideoDriver::DrawRect(const int Width,
                      const int Height,
                      const int X,
                      const int Y,
                      const Uint8 Red,
                      const Uint8 Green,
                      const Uint8 Blue, const Uint8 Alpha)
{
  SDL_Rect Rect;

  Rect.x = X;
  Rect.y = Y;
  Rect.w = Width;
  Rect.h = Height;

  SDL_SetRenderDrawColor(MainRenderer, Red, Green, Blue, Alpha);
  SDL_RenderFillRect(MainRenderer, &Rect);
}

void
VideoDriver::DrawStr(const char *String, const int X, const int Y, const double Scale)
{
  for(int I = 0; String[I] != '\0'; I++) {
    DrawChar(String[I], X + (I * (int) (FONT_WIDTH * Scale)), Y, Scale);
  }
}

void
VideoDriver::DrawTexture(SDL_Texture *Texture,
                         const int Width,
                         const int Height,
                         const int SourceX,
                         const int SourceY,
                         const int SourceWidth,
                         const int SourceHeight,
                         const int X, const int Y)
{
  SDL_Rect Src;
  SDL_Rect Dest;

  Src.x = SourceX;
  Src.y = SourceY;
  Src.w = SourceWidth;
  Src.h = SourceHeight;

  Dest.x = X;
  Dest.y = Y;
  Dest.w = Width;
  Dest.h = Height;

  SDL_RenderCopy(MainRenderer, Texture, &Src, &Dest);
}

void
VideoDriver::DrawTile(const int X,
                      const int Y,
                      const int Offset_X,
                      const int Offset_Y, const TileType Type)
{
  SDL_Rect Src;
  SDL_Rect Dest;

  Src.x = 0;
  Src.y = 0;
  Src.w = TILE_SIZE;
  Src.h = TILE_SIZE;

  Dest.x = (-X + Y) * (TILE_SIZE / 2) + Offset_X;
  Dest.y = (X + Y) * (TILE_SIZE / 4) + Offset_Y;
  Dest.w = TILE_SIZE;
  Dest.h = TILE_SIZE;

  SDL_RenderCopy(MainRenderer, Textures[(int) Type], &Src, &Dest);
}

void
VideoDriver::Finish(void)
{
  SDL_RenderPresent(this->MainRenderer);
}

void
VideoDriver::GetWindowSize(int &Width, int &Height)
{
  Width = WindowWidth;
  Height = WindowHeight;
}

bool
VideoDriver::Init(const char *ApplicationPath)
{
  int Ret;

  WindowWidth = WINDOW_WIDTH;
  WindowHeight = WINDOW_HEIGHT;

  MainWindow =
    SDL_CreateWindow("Base Conqueror",
                     SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED,
                     WindowWidth, WindowHeight,
                     SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS);
  if(MainWindow == nullptr) {
    std::cerr << "Error: Could not create window." << std::endl;

    return false;
  }

  MainRenderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(MainRenderer == nullptr) {
    MainRenderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED);
  }

  if(MainRenderer == nullptr) {
    MainRenderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetWindowTitle(MainWindow, "Base Conqueror (Software Renderer)");
  }

  if(MainRenderer == nullptr) {
    SDL_DestroyWindow(MainWindow);

    std::cerr << "Error: Could not create renderer." << std::endl;

    return false;
  }

  Ret = IMG_Init(IMG_INIT_PNG);
  if((Ret & IMG_INIT_PNG) != IMG_INIT_PNG) {
    std::cerr << "Error: Could not Initialize SDL_Image." << std::endl;

    SDL_DestroyRenderer(MainRenderer);
    SDL_DestroyWindow(MainWindow);

    return false;
  }

  if(!LoadIcon(ApplicationPath)) {
    SDL_DestroyRenderer(MainRenderer);
    SDL_DestroyWindow(MainWindow);

    std::cerr << "Error: Could not load icon." << std::endl;

    return false;
  }

  if(!LoadCursor(ApplicationPath)) {
    SDL_DestroyRenderer(MainRenderer);
    SDL_DestroyWindow(MainWindow);

    std::cerr << "Error: Could not load cursor." << std::endl;

    return false;
  }

  if(!LoadFonts(ApplicationPath)) {
    SDL_DestroyTexture(Cursor);
    SDL_DestroyRenderer(MainRenderer);
    SDL_DestroyWindow(MainWindow);

    std::cerr << "Error: Could not load fontset." << std::endl;

    return false;
  }

  if(!LoadTextures(ApplicationPath)) {
    SDL_DestroyTexture(Cursor);
    SDL_DestroyTexture(Fonts);
    SDL_DestroyRenderer(MainRenderer);
    SDL_DestroyWindow(MainWindow);

    std::cerr << "Error: Could not load textures." << std::endl;

    return false;
  }

  SDL_RenderSetLogicalSize(MainRenderer, 1366, 768);

  SDL_DisableScreenSaver();
  SDL_ShowCursor(SDL_DISABLE);

  return true;
}

bool
VideoDriver::LoadTextures(const char *ApplicationPath)
{
  static const char *TileNames[] = {
    "Base.png",
    "Forest.png",
    "Grassland.png",
    "Hill.png",
    "Mountain.png",
    "BlackHighlight.png",
    "BrownHighlight.png",
    "CyanHighlight.png",
    "GrayHighlight.png",
    "RedHighlight.png",
    "PurpleHighlight.png",
    "YellowHighlight.png",
    "WhiteHighlight.png",
    "Water.png"
  };

  for(int I = 0; I < NUM_TEXTURES; I++) {
    Textures[I] = LoadTextureFromPath(MainRenderer, ApplicationPath, TileNames[I]);
    if(Textures[I] == nullptr) { abort(); }
  }

  return true;
}

// Private Functions of VideoDriver
void
VideoDriver::DrawChar(const char Ch, const int X, const int Y, const double Scale)
{
  SDL_Rect FontSrc;
  SDL_Rect FontDst;

  FontSrc.x = (Ch % 16) * FONT_WIDTH;
  FontSrc.y = (Ch / 16) * FONT_HEIGHT;
  FontSrc.w = FONT_WIDTH;
  FontSrc.h = FONT_HEIGHT;

  FontDst.x = X;
  FontDst.y = Y;
  FontDst.w = FONT_WIDTH * Scale;
  FontDst.h = FONT_HEIGHT * Scale;

  SDL_RenderCopy(MainRenderer, Fonts, &FontSrc, &FontDst);
}

bool
VideoDriver::LoadCursor(const char *ApplicationPath)
{
  this->Cursor = LoadTextureFromPath(MainRenderer, ApplicationPath, "Cursor.png");
  if(this->Cursor == nullptr) { return false; }

  return true;
}

bool
VideoDriver::LoadFonts(const char *ApplicationPath)
{
  Fonts = LoadTextureFromPath(MainRenderer, ApplicationPath, "Fontset.png");
  if(Fonts == nullptr) { return false; }

  return true;
}

bool
VideoDriver::LoadIcon(const char *ApplicationPath)
{
  SDL_Surface *Icon;

  Icon = LoadSurfaceFromPath(ApplicationPath, "Base.png");
  if(Icon == nullptr) { return false; }

  SDL_SetWindowIcon(MainWindow, Icon);
  SDL_FreeSurface(Icon);

  return true;
}

SDL_Texture *
VideoDriver::LoadTexture(const char *Filename)
{
  SDL_Texture *Texture;

  Texture = LoadTextureFromPath(MainRenderer, Application->GetApplicationPath(), Filename);
  if(Texture == nullptr) { abort(); }

  return Texture;
}

// Private Functions
static SDL_Surface *
LoadSurfaceFromPath(const char *ApplicationPath, const char *SurfaceName)
{
  SDL_Surface *Surface;
  size_t PathLength;
  char *SurfacePath;

  PathLength = strlen(ApplicationPath);
#if __WIN32__
  PathLength += strlen("Assets\\Graphics\\");
#else // Linux
  PathLength += strlen("Assets/Graphics/");
#endif // __WIN32__
  PathLength += strlen(SurfaceName);

  SurfacePath = new char[PathLength + sizeof(char)];
  if(SurfacePath == nullptr) { abort(); }

#if __WIN32__
  sprintf(SurfacePath, "%s%s%s", ApplicationPath, "Assets\\Graphics\\", SurfaceName);
#else // Linux
  sprintf(SurfacePath, "%s%s%s", ApplicationPath, "Assets/Graphics/", SurfaceName);
#endif // __WIN32__

  Surface = IMG_Load(SurfacePath);
  delete[] SurfacePath;
  if(Surface == nullptr) { goto Error; }

  SDL_SetColorKey(Surface, SDL_TRUE, SDL_MapRGB(Surface->format, 0xFF, 0x77, 0xCC));

  return Surface;
Error:
  fprintf(stderr, "Error: Could not find surface: %s\n", SurfaceName);

  return nullptr;
}

static SDL_Texture *
LoadTextureFromPath(SDL_Renderer *Renderer,
                    const char *ApplicationPath, const char *TextureName)
{
  SDL_Surface *Surface;
  SDL_Texture *Texture;

  Surface = LoadSurfaceFromPath(ApplicationPath, TextureName);
  Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
  SDL_FreeSurface(Surface);
  if(Texture == nullptr) { return nullptr; }

  return Texture;
}
