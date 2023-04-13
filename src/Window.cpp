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

#include "Window.hpp"

Window::~Window(void)
{
  for(auto It : WidgetArray) { delete It; }

  WidgetArray.clear();
}

void
Window::CheckPress(const int X, const int Y)
{
  if((X >= LocX && X <= LocX + Width) &&
     (Y >= LocY && Y <= LocY + Height)) {
    for(auto It : WidgetArray) { It->CheckPress(X - LocX, Y - LocY); }
  }
}

void
Window::CheckRelease(const int X, const int Y)
{
  for(auto It : WidgetArray) { It->CheckRelease(X - LocX, Y - LocY); }
}

bool
Window::Create(const std::string &Name,
               const int Width,
               const int Height,
               const int LocX, const int LocY)
{
  this->Name = Name;
  this->Width = Width;
  this->Height = Height;
  this->LocX = LocX;
  this->LocY = LocY;
  this->Pressed = false;
  this->Arg = nullptr;
  this->SignalFunc = nullptr;

  return true;
}

void
Window::Draw(VideoDriver *Video,
             const int OffsetX, const int OffsetY)
{
    Video->DrawRect(Width - 4, 2         , LocX + OffsetX + 2          , LocY + OffsetY               , 0xCC, 0xCC, 0xCC, 0xFF);
    Video->DrawRect(2        , Height - 4, LocX + OffsetX              , LocY + OffsetY + 2           , 0xCC, 0xCC, 0xCC, 0xFF);
    Video->DrawRect(Width - 4, Height - 4, LocX + OffsetX + 2          , LocY + OffsetY + 2           , 0x99, 0x99, 0x99, 0xFF);
    Video->DrawRect(2        , Height - 4, LocX + OffsetX + (Width - 2), LocY + OffsetY + 2           , 0x66, 0x66, 0x66, 0xFF);
    Video->DrawRect(Width - 4, 2         , LocX + OffsetX + 2          , LocY + OffsetY + (Height - 2), 0x66, 0x66, 0x66, 0xFF);

  for(auto It : WidgetArray) {
    It->Draw(Video, LocX + OffsetX, LocY + OffsetY);
  }
}
