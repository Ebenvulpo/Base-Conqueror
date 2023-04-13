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

#ifndef _WIDGET_HPP_
#define _WIDGET_HPP_

class Widget;

#define SIGNAL(s) ((void (*)(void *, void *)) s)

#include <string>
#include <vector>

#include "Video.hpp"

class  Widget {
protected:
  std::string Name;
  int Width, Height;
  int LocX, LocY;
  bool Pressed;
  void *Arg;
  void (*SignalFunc)(void *Widget, void *Arg);
  std::vector<Widget *> WidgetArray;

public:
  virtual ~Widget(void);
  virtual void CheckPress(const int X, const int Y);
  virtual void CheckRelease(const int X, const int Y);
  virtual void Draw(VideoDriver *Video,
                    const int X, const int Y);
  virtual void Release(void);
  Widget(void) { }
  bool AddWidget(Widget *NewWidget);
  void Center(const int ScreenWidth, const int ScreenHeight);
  void CenterOnX(const int ScreenWidth);
  void ChangeLoc(const int X, const int Y);
  void ChangeSignal(void (*Signal)(void *, void *), void *Arg);
  void GetLoc(int &X, int &Y) { X = this->LocX; Y = this->LocY; }
  const std::string &GetName(void) const { return Name; }
  void GetSize(int &Width, int &Height) const;
  void ToggleOn(void) { Pressed = true; }
  void ToggleOff(void) { Pressed = false; }
};

#endif /* _WIDGET_HPP_ */
