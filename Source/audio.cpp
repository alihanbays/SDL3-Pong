#include "../Headers/audio.h"

bool audioInit() {
    const char *files[4] = {"Assets/sfx/boing1.mp3", "Assets/sfx/boing2.mp3", "Assets/sfx/wallbang1.mp3", "Assets/sfx/wallbang2.mp3"};

    if (!MIX_Init()) {
        SDL_Log("Couldn't initialize SDL_mixer: %s", SDL_GetError());
        return false;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer) {
        SDL_Log("Couldn't create mixer: %s", SDL_GetError());
        return false;
    }

    for (int i = 0; i < 4; i++) {
        audio[i] = MIX_LoadAudio(mixer, files[i], true);

        if (!audio[i]) {
            SDL_Log("Couldn't load audio from %s: %s", files[i], SDL_GetError());
            return false;
        }
    }

    track = MIX_CreateTrack(mixer);
    if (!track) {
        SDL_Log("Couldn't create track: %s", SDL_GetError());
        return false;
    }
    
    return true;
}
    
void audioPlay(int trackNum) {
    MIX_SetTrackAudio(track, audio[trackNum]);
    MIX_PlayTrack(track, 0);
}

void audioCleanup() {
    MIX_Quit();
}