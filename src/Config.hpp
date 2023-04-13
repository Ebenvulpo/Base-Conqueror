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

#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

// AI Configurations
#define AI_DECISION_TIME         8  // How many seconds before the AI makes a decision
#define AI_CHANCE_TO_ATTACK      20 // (1/20) Chance
#define AI_CHANCE_TO_COLONIZE    4  // (1/4)  Chance
#define AI_CHANCE_TO_UNLINK_BASE 2  // (1/2)  Chance

// World Generation Configurations
#define WORLDGEN_CHANCE_FOR_FOREST_TILE 4 // (1/4) Chance
#define NUMBER_OF_BASES 64                // Must be 2**N
#define NUMBER_OF_MAPGEN_ITERATIONS 1024  // Must be 2**N
#define WORLD_SIZE_X 96                   // Must be 2**N
#define WORLD_SIZE_Y 96                   // Must be 2**N

// Game Configurations
#define NUMBER_OF_PLAYERS 4

// Graphics Configurations
#define TILE_SIZE 64   // Must be 2**N

#define FONT_WIDTH 8
#define FONT_HEIGHT 12

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

#endif // _CONFIG_HPP_
