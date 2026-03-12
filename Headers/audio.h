#ifndef AUDIO_H
#define AUDIO_H

#include <SDL3_mixer/SDL_mixer.h>

static MIX_Mixer *mixer;
static MIX_Track *track;
static MIX_Audio *audio[4];
bool audioInit();
void audioPlay(int trackNum);
void audioCleanup();

#endif