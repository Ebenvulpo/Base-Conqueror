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

#ifndef _LABEL_HPP_
#define _LABEL_HPP_

class Label;

#include <string>

#include "Widget.hpp"
#include "Video.hpp"

class Label : public Widget {
private:
  double Scale;
  char Buffer[512];

public:
  Label(void) { }
  void ChangeScale(const double Scale) { this->Scale = Scale; }
  void ChangeString(const char *String);
  void Create(const std::string &Name,
              const char *Label,
              const int Width,
              const int Height,
              const int X, const int Y);
  void Draw(VideoDriver *Video, const int X, const int Y);
};

#endif // _LABEL_HPP_
