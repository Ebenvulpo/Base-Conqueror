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

#include <cstdlib>

#if __WIN32__
#define SDL_MAIN_HANDLED
#endif // __WIN32__

#include <SDL2/SDL.h>
#include <iostream>

#include "Application.hpp"

BcApplication *Application;

void Deinit(void);
bool Init(void);

int main(void)
{
  Application = new BcApplication;

  Init();

  Application->Init();
  Application->Loop();
  delete Application;

  Deinit();

  exit(EXIT_SUCCESS);
}

void Deinit(void)
{
  SDL_Quit();
}

bool Init(void)
{
  int ret;

  ret = SDL_Init (SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if(ret < 0) {
    std::cerr << "Error: SDL2 could not initialize." << std::endl;

    return false;
  }

  return true;
}

#ifdef __WIN32__
int WINAPI WinMain(void)
{
  main();

  return 0;
}
#endif // __WIN32__
