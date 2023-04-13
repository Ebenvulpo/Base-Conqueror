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

#ifndef _AUDIO_HPP_
#define _AUDIO_HPP_

class AudioDriver;

#define SONG_THE_COMING_WAR 1
#define SONG_TO_BE_DEFEATED 2
#define SONG_WE_ARE_VICTORIOUS 3
#define SONG_THE_ROAD_TO_VICTORY 4

#include <SDL2/SDL_mixer.h>

class AudioDriver {
  Mix_Music *TheComingWar;
  Mix_Music *ToBeDefeated;
  Mix_Music *WeAreVictorious;
  Mix_Music *TheRoadToVictory;

  Mix_Chunk *AttackingSound;
  Mix_Chunk *PressedSound;
  Mix_Chunk *SelectedSound;

  Mix_Music *PlayNextSong;

  int SFXVolume;
  int MusicVolume;

  bool LoadSongs(const char *ApplicationPath);
  bool LoadSounds(const char *ApplicationPath);
public:
  AudioDriver(void) { }
  ~AudioDriver(void);
  void ChangeMusicVolume(const int Volume);
  void ChangeSFXVolume(const int Volume);
  void FadeMusic(const int Ms);
  bool FadingOutMusic(void);
  void NextSong(const int SongNum);
  void PlayAttackingSound(void);
  void PlayMusic(const int SongNum);
  void PlayPressedSound(void);
  void PlaySelectedSound(void);
  bool Init(const char *ApplicationPath);
  bool IsMusicPlaying(void);
  void Tick(void);
};

#endif // _AUDIO_HPP_
