#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>

typedef struct {
    Mix_Chunk *fish_eaten;
    Mix_Music *bg_music;
    int muted;
} AudioManager;
/**
 * @brief initialise l'audio SDL
 * */
AudioManager *audio_init();

/**
 * @brief charge en mémoire les sons nécessaires au bon fonctionnement du jeu
 *
 * */
void audio_load(const char *path, AudioManager *audio);

/**@brief joue un son
 * @param sound le Mix_chunk à jouer
 */
void audio_play(Mix_Chunk *sound, AudioManager *audio);

/**
 * @brief libère la mémoire utilisée par l'ensemble de son*/
void audio_quit(AudioManager *audio);

int audio_load_music(const char *path, AudioManager *audio);
void audio_play_music(AudioManager *audio);
void audio_stop_music(void);
void audio_toggle_mute(AudioManager *audio);

#endif
