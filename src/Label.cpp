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

#include "Label.hpp"

#include <cstring>

#include "Config.hpp"

void
Label::ChangeString(const char *String)
{
  strcpy(this->Buffer, String);
}

void
Label::Create(const std::string &Name,
              const char *Label,
              const int Width,
              const int Height,
              const int X, const int Y)
{
  this->Name = Name;
  strcpy(this->Buffer, Label);
  this->Width = Width;
  this->Height = Height;
  this->LocX = X;
  this->LocY = Y;
  this->SignalFunc = nullptr;
  this->Arg = nullptr;
  this->Pressed = false;
  this->Scale = 1.0;
}

void
Label::Draw(VideoDriver *Video, const int X, const int Y)
{
  size_t StrLength;
  int StrWidth, StrHeight;

  StrWidth = 0;
  StrHeight = 0;

  StrLength = strlen(Buffer);
  if(Width > 0)  { StrWidth = (StrLength * FONT_WIDTH * Scale); }
  if(Height > 0) { StrHeight = FONT_HEIGHT * Scale; }

  Video->DrawStr(Buffer,
                 (LocX + X) + ((Width / 2.0) - (StrWidth / 2.0)),
                 (LocY + Y) + ((Height / 2.0) - (StrHeight / 2.0)),
                 Scale);
}
