#include "sound.h"
#include "config.h"
#include "SDL_mixer.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

AudioManager *audio_init(void) {
    // Initialiser le sous-système audio de SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL_Init audio: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialiser les décodeurs supplémentaires
    // MIX_INIT_MP3  : fichiers .mp3
    // MIX_INIT_OGG  : fichiers .ogg (Vorbis) — meilleur rapport qualité/taille
    // MIX_INIT_FLAC : fichiers .flac (sans perte)
    int flags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC;
    if ((Mix_Init(flags) & flags) != flags) {
        fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
        // pas forcément fatal si certains formats manquent
    }

    // Ouvrir le device audio
    if (Mix_OpenAudio(44100,              // 44100 Hz = qualité CD standard
                      MIX_DEFAULT_FORMAT, // AUDIO_S16SYS = 16 bits signé
                      2,                  // 2 = stéréo, 1 = mono
                      2048) < 0) {        // buffer de 2048 samples
        fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
        exit(1);
    }
    AudioManager *audio = calloc(1, sizeof(*audio));
    if (audio)
        return audio;
    return NULL;
}

void audio_load(const char *path, AudioManager *audio) {
    char path_buf[256];
    snprintf(path_buf, sizeof(path_buf), "%s%s", path, "bubble_pop.wav");
    audio->fish_eaten = Mix_LoadWAV(path_buf);
    if (!audio->fish_eaten) {
        fprintf(stderr, "audio_load: Mix_LoadWAV failed: %s\n", Mix_GetError());
    } else {
        Mix_VolumeChunk(audio->fish_eaten, MIX_MAX_VOLUME);
    }
}

void audio_play(Mix_Chunk *sound, AudioManager *audio) {
    if (!sound) {
        fprintf(stderr, "audio_play: sound is NULL!\n");
        return;
    }
    if (audio && audio->muted) {
        return;
    }
    Mix_PlayChannel(1, sound, 0);
}

int audio_load_music(const char *path, AudioManager *audio) {
    char path_buf[256];
    snprintf(path_buf, sizeof(path_buf), "%s%s.mp3", path, MUSIC_FILE);
    audio->bg_music = Mix_LoadMUS(path_buf);
    if (!audio->bg_music) {
        fprintf(stderr, "audio_load_music: Mix_LoadMUS failed: %s\n",
                Mix_GetError());
        return -1;
    }
    printf("[AUDIO] Music loaded: %s\n", path_buf);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    return 0;
}

void audio_play_music(AudioManager *audio) {
    if (!audio || !audio->bg_music) {
        fprintf(stderr, "audio_play_music: no music loaded\n");
        return;
    }
    if (audio->muted) {
        return;
    }
    Mix_PlayMusic(audio->bg_music, -1);
}

void audio_toggle_mute(AudioManager *audio) {
    if (!audio)
        return;
    audio->muted = !audio->muted;
    if (audio->muted) {
        Mix_HaltMusic();
    } else {
        if (audio->bg_music) {
            Mix_PlayMusic(audio->bg_music, -1);
        }
    }
}

void audio_stop_music(void) {
    Mix_HaltMusic();
}

void audio_quit(AudioManager *audio) {
    if (audio) {
        if (audio->bg_music) {
            Mix_FreeMusic(audio->bg_music);
        }
        if (audio->fish_eaten) {
            Mix_FreeChunk(audio->fish_eaten);
        }
        free(audio);
    }
}
