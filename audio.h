#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <SDL_mixer.h>

extern Mix_Music* menuMusic;
extern Mix_Music* level1Music;
extern Mix_Music* level2Music;
extern Mix_Music* bossMusic;
extern Mix_Music* winSound;
extern Mix_Music* loseSound;
extern Mix_Music* currentMusic;
extern Mix_Music* prevMusic;

extern Mix_Chunk* shootSound;
extern Mix_Chunk* menuSwitch;
extern Mix_Chunk* confirm;
extern Mix_Chunk* enemyShootSound;
extern Mix_Chunk* explosionSound;
extern Mix_Chunk* lazerSound;
extern Mix_Chunk* deathSound;

void loadAudio();

void playMusic(Mix_Music* music);

void freeAudio();

#endif // AUDIO_H_INCLUDED
