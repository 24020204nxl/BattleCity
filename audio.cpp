#include "audio.h"
#include <iostream>

Mix_Music* menuMusic = nullptr;
Mix_Music* level1Music = nullptr;
Mix_Music* level2Music = nullptr;
Mix_Music* bossMusic = nullptr;
Mix_Music* winSound = nullptr;
Mix_Music* loseSound = nullptr;
Mix_Music* currentMusic=nullptr;
Mix_Music* prevMusic=nullptr;

Mix_Chunk* shootSound = nullptr;
Mix_Chunk* deathSound=nullptr;
Mix_Chunk* menuSwitch=nullptr;
Mix_Chunk* confirm=nullptr;
Mix_Chunk* enemyShootSound=nullptr;
Mix_Chunk* explosionSound = nullptr;
Mix_Chunk* lazerSound = nullptr;
Mix_Chunk* bossDeathSound = nullptr;

void loadAudio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return;
    }

    menuMusic = Mix_LoadMUS("sounds/theme.mp3");
    level1Music = Mix_LoadMUS("sounds/level1.mp3");
    level2Music = Mix_LoadMUS("sounds/level2.mp3");
    bossMusic = Mix_LoadMUS("sounds/bossFight.mp3");
    winSound = Mix_LoadMUS("sounds/win.mp3");
    loseSound = Mix_LoadMUS("sounds/lose.mp3");

    shootSound = Mix_LoadWAV("sounds/playerBullet.wav");
    menuSwitch=Mix_LoadWAV("sounds/menuChange.wav");
    confirm=Mix_LoadWAV("sounds/confirm.wav");
    enemyShootSound = Mix_LoadWAV("sounds/enemyBullet.wav");
    explosionSound = Mix_LoadWAV("sounds/explosion.wav");
    lazerSound = Mix_LoadWAV("sounds/lazer.wav");

    if (!menuMusic || !level1Music || !level2Music || !bossMusic ||
        !shootSound || !explosionSound || !lazerSound ||
        !winSound || !loseSound) {
        std::cerr << "Failed to load sound files! Mix_Error: " << Mix_GetError() << std::endl;
    }
}

void playMusic(Mix_Music* music) {
    Mix_HaltMusic();
    if (music) {
        currentMusic=music;
        Mix_PlayMusic(music, -1);
        Mix_VolumeMusic(30);
    }
}

void freeAudio() {
    Mix_FreeMusic(menuMusic);
    Mix_FreeMusic(level1Music);
    Mix_FreeMusic(level2Music);
    Mix_FreeMusic(bossMusic);
    Mix_FreeMusic(winSound);
    Mix_FreeMusic(loseSound);

    Mix_FreeChunk(shootSound);
    Mix_FreeChunk(explosionSound);
    Mix_FreeChunk(lazerSound);
    Mix_FreeChunk(bossDeathSound);

    Mix_CloseAudio();
}
