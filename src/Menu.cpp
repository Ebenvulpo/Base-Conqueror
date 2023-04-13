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

#include "Menu.hpp"

Menu::~Menu(void)
{
  if(Background != nullptr) { SDL_DestroyTexture(Background); }

  for(auto It : WidgetArray) { delete It; }

  WidgetArray.clear();
}

void
Menu::ChangeBackground(SDL_Texture *BackgroundImage)
{
  if(Background != nullptr) { SDL_DestroyTexture(Background); }

  Background = BackgroundImage;
}

void
Menu::CheckPress(const int X, const int Y)
{
  for(auto It : WidgetArray) { It->CheckPress(X, Y); }
}

void
Menu::CheckRelease(const int X, const int Y)
{
  for(auto It : WidgetArray) { It->CheckRelease(X, Y); }
}

bool
Menu::Create(void)
{
  Background = nullptr;
  Width = 0;
  Height = 0;
  LocX = 0;
  LocY = 0;
  Pressed = false;

  return true;
}

void
Menu::Draw(VideoDriver *Video,
           const int X, const int Y)
{
  if(Background != nullptr) {
    int WindowWidth, WindowHeight;

    Video->GetWindowSize(WindowWidth, WindowHeight);

    Video->DrawTexture(Background,
                       WindowWidth, WindowHeight,
                       0, 0,
                       WindowWidth, WindowHeight,
                       0, 0);
  }

  for(auto It : WidgetArray) { It->Draw(Video, X, Y); }
}

void
Menu::Release(void)
{
  for(auto It : WidgetArray) { It->Release(); }
}
