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

#include "Widget.hpp"

#include <iostream>

#include "Debug.hpp"

Widget::~Widget(void)
{
  for(auto It : WidgetArray) { delete It; }
}

bool
Widget::AddWidget(Widget *NewWidget)
{
  WidgetArray.push_back(NewWidget);

  return true;
}

void
Widget::Center(const int ScreenWidth, const int ScreenHeight)
{
  LocX = (ScreenWidth / 2) - (Width / 2);
  LocY = (ScreenHeight / 2) - (Height / 2);
}

void
Widget::CenterOnX(const int ScreenWidth)
{
  LocX = (ScreenWidth / 2) - (Width / 2);
}

void
Widget::ChangeLoc(const int X, const int Y)
{
  LocX = X;
  LocY = Y;
}

void
Widget::ChangeSignal(void (*Signal)(void *, void *),void *Arg)
{
  this->Arg = Arg;
  SignalFunc = Signal;
}

void
Widget::CheckPress(const int X, const int Y)
{
#if DEBUG_GUI
  std::cerr << "GUI: Widget CheckPress" << std::endl;
#endif /* DEBUG_GUI */

  if((X >= LocX && X <= LocX + Width) &&
     (Y >= LocY && Y <= LocY + Height)) {
    Pressed = true;
  }
}

void
Widget::CheckRelease(const int X, const int Y)
{
#if DEBUG_GUI
  std::cerr << "GUI: Widget CheckRelease" << std::endl;
#endif /* DEBUG_GUI */

  if(!Pressed) { return; }

  if((X >= LocX && X <= LocX + Width) &&
     (Y >= LocY && Y <= LocY + Height)) {
    if(SignalFunc != nullptr) { SignalFunc(this, Arg); }

    Pressed = false;
  } else {
    Pressed = false;
  }
}

void
Widget::Draw(VideoDriver *Video,
             const int X, const int Y)
{
  Video->DrawRect(16, 16, X, Y, 0xFF, 0xFF, 0xFF, 0xFF);
}

void
Widget::GetSize(int &Width, int &Height) const
{
  Width = this->Width;
  Height = this->Height;
}

void
Widget::Release(void)
{
  Pressed = false;
}
