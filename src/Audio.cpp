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

#include "Audio.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>

#include "Debug.hpp"

static Mix_Music *LoadMusic(const char *ApplicationPath, const char *SongName);
static Mix_Chunk *LoadSound(const char *ApplicationPath, const char *SoundName);

AudioDriver::~AudioDriver(void)
{
  Mix_FreeMusic(TheComingWar);
  Mix_FreeMusic(ToBeDefeated);
  Mix_FreeMusic(WeAreVictorious);
  Mix_FreeMusic(TheRoadToVictory);

  Mix_FreeChunk(AttackingSound);
  Mix_FreeChunk(PressedSound);
  Mix_FreeChunk(SelectedSound);

  Mix_Quit();
}

void
AudioDriver::ChangeMusicVolume(const int Volume)
{
  if(Volume < 0) {
    MusicVolume = 0;

    Mix_VolumeMusic(64 * (MusicVolume / 100.0));

    return;
  }

  if(Volume > 100) {
    MusicVolume = 100;

    Mix_VolumeMusic(64 * (MusicVolume / 100.0));

    return;
  }

  MusicVolume = Volume;

  Mix_VolumeMusic(64 * (MusicVolume / 100.0));
}

void
AudioDriver::ChangeSFXVolume(const int Volume)
{
  if(Volume < 0) {
    SFXVolume = 0;

    Mix_Volume(-1, 64 * (SFXVolume / 100.0));

    return;
  }

  if(Volume > 100) {
    SFXVolume = 100;

    Mix_Volume(-1, 64 * (SFXVolume / 100.0));

    return;
  }

  SFXVolume = Volume;

  Mix_Volume(-1, 64 * (SFXVolume / 100.0));
}

void
AudioDriver::FadeMusic(const int Ms)
{
  Mix_FadeOutMusic(Ms);
}

bool
AudioDriver::FadingOutMusic(void)
{
  switch(Mix_FadingMusic()) {
  case MIX_NO_FADING:
    return false;
  case MIX_FADING_IN:
    return false;
  case MIX_FADING_OUT:
    return true;
  default:
    abort();
  }
}

void
AudioDriver::NextSong(const int SongNum)
{
  switch(SongNum) {
  case SONG_THE_COMING_WAR:
    PlayNextSong = TheComingWar;
    break;
  case SONG_TO_BE_DEFEATED:
    PlayNextSong = ToBeDefeated;
    break;
  case SONG_WE_ARE_VICTORIOUS:
    PlayNextSong = WeAreVictorious;
    break;
  case SONG_THE_ROAD_TO_VICTORY:
    PlayNextSong = TheRoadToVictory;
    break;
  default:
    abort();
  }
}

void
AudioDriver::PlayAttackingSound(void)
{
  if(Mix_Playing(0) == 0) {
    Mix_PlayChannel(0, AttackingSound, 0);
  }
}

void
AudioDriver::PlayMusic(const int SongNum)
{
  switch(SongNum) {
  case SONG_THE_COMING_WAR:
    Mix_PlayMusic(TheComingWar, 0);
    break;
  case SONG_TO_BE_DEFEATED:
    Mix_PlayMusic(ToBeDefeated, 0);
    break;
  case SONG_WE_ARE_VICTORIOUS:
    Mix_PlayMusic(WeAreVictorious, 0);
    break;
  case SONG_THE_ROAD_TO_VICTORY:
    Mix_PlayMusic(TheRoadToVictory, 0);
    break;
  default:
    abort();
  }
}

void
AudioDriver::PlayPressedSound(void)
{
  Mix_PlayChannel(-1, PressedSound, 0);
}

void
AudioDriver::PlaySelectedSound(void)
{
  Mix_PlayChannel(-1, SelectedSound, 0);
}

bool
AudioDriver::Init(const char *ApplicationPath)
{
  int ret;

  ret = Mix_Init(MIX_INIT_OGG);
  if((ret & MIX_INIT_OGG) != MIX_INIT_OGG) {
#ifdef DEBUG_AUDIO
    std::cerr << "AudioDriver: " << Mix_GetError() << std::endl;
#endif // DEBUG_AUDIO

    abort();
  }

  ret = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
  if(ret < 0) {
    Mix_Quit();

    return false;
  }

  Mix_AllocateChannels(8);
  Mix_ReserveChannels(1);

  if(!LoadSongs(ApplicationPath))  { abort(); }
  if(!LoadSounds(ApplicationPath)) { abort(); }

  SFXVolume = 64;
  MusicVolume = 32;
  PlayNextSong = nullptr;

  Mix_Volume(-1, SFXVolume);
  Mix_VolumeMusic(MusicVolume);

  return true;
}

bool
AudioDriver::IsMusicPlaying()
{
  int ret;

  ret = Mix_PlayingMusic();
  if(ret) { return true; }

  return false;
}

void
AudioDriver::Tick(void)
{
  if(!Mix_PlayingMusic() && PlayNextSong != nullptr) {
    Mix_PlayMusic(PlayNextSong, 0);

    PlayNextSong = nullptr;
  }
}

// Private Functions for AudioDriver
bool
AudioDriver::LoadSongs(const char *ApplicationPath)
{
  static const char *MusicList[] = {
    "TheComingWar.ogg",
    "ToBeDefeated.ogg",
    "WeAreVictorious.ogg",
    "TheRoadToVictory.ogg"
  };

  TheComingWar = LoadMusic(ApplicationPath, MusicList[0]);
  ToBeDefeated = LoadMusic(ApplicationPath, MusicList[1]);
  WeAreVictorious = LoadMusic(ApplicationPath, MusicList[2]);
  TheRoadToVictory = LoadMusic(ApplicationPath, MusicList[3]);

  return true;
}

bool
AudioDriver::LoadSounds(const char *ApplicationPath)
{
  static const char *SoundEffectNames[] = {
    "AttackingSound.ogg",
    "PressedSound.ogg",
    "SelectSound.ogg"
  };

  AttackingSound = LoadSound(ApplicationPath, SoundEffectNames[0]);
  PressedSound = LoadSound(ApplicationPath, SoundEffectNames[1]);
  SelectedSound = LoadSound(ApplicationPath, SoundEffectNames[2]);

  return true;
}

// Private Functions
static Mix_Music *
LoadMusic(const char *ApplicationPath, const char *SongName)
{
  size_t PathLength;
  Mix_Music *Song;
  char *SongPath;

  PathLength = strlen(ApplicationPath);
#if __WIN32__
  PathLength += strlen("Assets\\Music\\");
#else // Linux
  PathLength += strlen("Assets/Music/");
#endif // __WIN32__
  PathLength += strlen(SongName);

  SongPath = new char[PathLength + sizeof(char)];
  if(SongPath == nullptr) { abort(); }

#if __WIN32__
  sprintf(SongPath, "%s%s%s", ApplicationPath, "Assets\\Music\\", SongName);
#else // Linux
  sprintf(SongPath, "%s%s%s", ApplicationPath, "Assets/Music/", SongName);
#endif // __WIN32__

  Song = Mix_LoadMUS(SongPath);
  delete[] SongPath;
  if(Song == nullptr) {
    std::cerr << "Error: Could not load song: " << SongName << std::endl;
  }

  return Song;
}

static Mix_Chunk *
LoadSound(const char *ApplicationPath, const char *SoundName)
{
  size_t PathLength;
  Mix_Chunk *Sound;
  char *SoundPath;

  PathLength = strlen(ApplicationPath);
#if __WIN32__
  PathLength += strlen("Assets\\Sound\\");
#else // Linux
  PathLength += strlen("Assets/Sound/");
#endif // __WIN32__
  PathLength += strlen(SoundName);

  SoundPath = new char[PathLength + sizeof(char)];
  if(SoundPath == nullptr) { abort(); }

#if __WIN32__
  sprintf(SoundPath, "%s%s%s", ApplicationPath, "Assets\\Sound\\", SoundName);
#else // Linux
  sprintf(SoundPath, "%s%s%s", ApplicationPath, "Assets/Sound/", SoundName);
#endif // __WIN32__

  Sound = Mix_LoadWAV(SoundPath);
  delete[] SoundPath;
  if(Sound == nullptr) {
    std::cerr << "Error: Could not load sound: " << SoundName << std::endl;
  }

  return Sound;
}
