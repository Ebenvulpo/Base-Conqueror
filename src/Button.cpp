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

#include "Button.hpp"

#include <cstring>

#include "Config.hpp"

Button::~Button(void)
{
  if(Image != nullptr) { SDL_DestroyTexture(Image); }
}

void
Button::ChangeImage(SDL_Texture *Image)
{
  if(this->Image != nullptr) {
    SDL_DestroyTexture(this->Image);
  }

  this->Image = Image;
}

bool
Button::Create(const char *Label,
               const int Width,
               const int Height,
               const int X, const int Y)
{
  this->Width = Width;
  this->Height = Height;
  this->LocX = X;
  this->LocY = Y;
  this->Pressed = false;
  this->SignalFunc = nullptr;
  this->Image = nullptr;
  this->Scale = 1.0;

  strcpy(this->Label, Label);

  return true;
}

void
Button::Draw(VideoDriver *Video,
             const int X, const int Y)
{
  size_t LabelLength;
  int LabelLocX, LabelLocY;

  if(Image != nullptr) {
    if(Pressed) {
      Video->DrawTexture(Image,
                         (Width * Scale), (Height * Scale),
                         Width, 0,
                         Width, Height,
                         LocX + X, LocY + Y);
    } else {
      Video->DrawTexture(Image,
                         (Width * Scale), (Height * Scale),
                         0, 0,
                         Width, Height,
                         LocX + X, LocY + Y);
    }
  } else {
    if(Pressed) {
      Video->DrawRect(Width - 4, 2         , LocX + X + 2          , LocY + Y               , 0x33, 0x33, 0x33, 0xFF);
      Video->DrawRect(2        , Height - 4, LocX + X              , LocY + Y + 2           , 0x33, 0x33, 0x33, 0xFF);
      Video->DrawRect(Width - 4, Height - 4, LocX + X + 2          , LocY + Y + 2           , 0x66, 0x66, 0x66, 0xFF);
      Video->DrawRect(2        , Height - 4, LocX + X + (Width - 2), LocY + Y + 2           , 0xCC, 0xCC, 0xCC, 0xFF);
      Video->DrawRect(Width - 4, 2         , LocX + X + 2          , LocY + Y + (Height - 2), 0xCC, 0xCC, 0xCC, 0xFF);
    } else {
      Video->DrawRect(Width - 4, 2         , LocX + X + 2          , LocY + Y               , 0xCC, 0xCC, 0xCC, 0xFF);
      Video->DrawRect(2        , Height - 4, LocX + X              , LocY + Y + 2           , 0xCC, 0xCC, 0xCC, 0xFF);
      Video->DrawRect(Width - 4, Height - 4, LocX + X + 2          , LocY + Y + 2           , 0x99, 0x99, 0x99, 0xFF);
      Video->DrawRect(2        , Height - 4, LocX + X + (Width - 2), LocY + Y + 2           , 0x66, 0x66, 0x66, 0xFF);
      Video->DrawRect(Width - 4, 2         , LocX + X + 2          , LocY + Y + (Height - 2), 0x66, 0x66, 0x66, 0xFF);
    }
  }

  if(Label[0] != '\0') {
    LabelLength = strlen(Label);
    LabelLocX = (LocX + (Width / 2)) - ((LabelLength / 2.0) * (size_t) (FONT_WIDTH * Scale));
    LabelLocY = (LocY + (Height / 2)) - ((FONT_HEIGHT * Scale) / 2);

    if(Pressed) {
      Video->DrawStr(Label, LabelLocX + X + 1, LabelLocY + Y + 1, Scale);
    } else {
      Video->DrawStr(Label, LabelLocX + X, LabelLocY + Y, Scale);
    }
  }
}

void
Button::SetFontScale(const double Scale)
{
  this->Scale = Scale;
}
