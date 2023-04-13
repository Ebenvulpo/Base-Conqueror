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

#ifndef _BUTTON_HPP_
#define _BUTTON_HPP_

class Button;

#include <SDL2/SDL_render.h>

#include "Widget.hpp"
#include "Video.hpp"

class Button final : public Widget {
private:
  SDL_Texture *Image;
  double Scale;
  char Label[64];

public:
  ~Button(void);
  Button(void) { }
  void ChangeImage(SDL_Texture *Image);
  bool Create(const char *Label,
              const int Width,
              const int Height,
              const int X, const int Y);
  void Draw(VideoDriver *Video,
            const int X, const int Y);
  void SetFontScale(const double Scale);
};

#endif /* _BUTTON_HPP_ */
