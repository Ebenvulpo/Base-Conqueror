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

#include "Image.hpp"

Image::~Image(void)
{
  if(ImageTexture != nullptr) { SDL_DestroyTexture(ImageTexture); }
}

bool
Image::Create(SDL_Texture *TextureImage,
              const int Width,
              const int Height,
              const int X, const int Y)
{
  this->ImageTexture = TextureImage;
  this->Width = Width;
  this->Height = Height;
  this->LocX = X;
  this->LocY = Y;
  this->Pressed = false;
  this->Arg = nullptr;
  this->SignalFunc = nullptr;

  return true;
}

void
Image::Draw(VideoDriver *Video,
            const int OffsetX, const int OffsetY)
{
  if(ImageTexture != nullptr) {
    Video->DrawTexture(ImageTexture,
                       Width, Height,
                       0, 0, Width, Height,
                       LocX + OffsetX, LocY + OffsetY);
  }
}
