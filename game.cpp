#include "game.h"
#include <iostream>

Game::Game() {
    std::cerr << "Initializing SDL..." << std::endl;
    running = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }

    std::cerr << "Creating window..." << std::endl;
    window = SDL_CreateWindow("Battle City",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }

    std::cerr << "Creating renderer..." << std::endl;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }
    loadAudio();
    playMusic(menuMusic);
    spritesheet=loadTexture("assets.png");
    std::cerr << "Initialization complete!" << std::endl;
}

SDL_Texture* Game::loadTexture(const string &path){
    SDL_Surface* tempSurface = IMG_Load(path.c_str());
    if(!tempSurface){
        cerr<<"Failed to load assets of the game, error: "<<IMG_GetError()<<endl;
        return nullptr;
    }
    SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}

void Game::handleEvent(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_QUIT){
            running=false;
        }
        else if(event.type==SDL_KEYDOWN){
            if(event.key.keysym.sym==SDLK_ESCAPE){
                Mix_PlayChannel(-1,menuSwitch,0);
                if(gstate==PAUSE){
                    gstate = prevState;
                }
                else{
                    prevState=gstate;
                    gstate=PAUSE;
                    prevMusic=currentMusic;
                    playMusic(menuMusic);
                }
            }
            if(gstate==SINGLEPLAYER||gstate==MULTIPLAYER){
                switch(event.key.keysym.sym){
                    case SDLK_UP:player.move(0,-10,walls,enemies);break;
                    case SDLK_DOWN:player.move(0,10,walls,enemies);break;
                    case SDLK_LEFT:player.move(-10,0,walls,enemies);break;
                    case SDLK_RIGHT:player.move(10,0,walls,enemies);break;
                    case SDLK_SPACE:player.shoot();break;
                    case SDLK_w:player2.move(0,-10,walls,enemies);break;
                    case SDLK_s:player2.move(0,10,walls,enemies);break;
                    case SDLK_a:player2.move(-10,0,walls,enemies);break;
                    case SDLK_d:player2.move(10,0,walls,enemies);break;
                    case SDLK_q:player2.shoot();break;
                }
            }
        }
    }

}

void Game::handleLoadEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    Mix_PlayChannel(-1,menuSwitch,0);
                    loadScreenSelection = (loadScreenSelection - 1 + 2) % 2;
                    break;
                case SDLK_DOWN:
                    Mix_PlayChannel(-1,menuSwitch,0);
                    loadScreenSelection = (loadScreenSelection + 1 + 2) % 2;
                    break;
                case SDLK_SPACE:
                    Mix_PlayChannel(-1,confirm,0);
                    if (loadScreenSelection == 0) {
                        loadGame();
                    } else {
                        currentLevel = 1;
                        loadLevel(currentLevel);
                        gstate = (menuSelection == 0) ? SINGLEPLAYER : MULTIPLAYER;
                    }
                    break;
            }
        }
    }
}

void Game::renderLoad(int loadSCreenSelection){
    string loadscr="load/";
    SDL_Texture* loadTexture=IMG_LoadTexture(renderer,(loadscr+to_string(loadSCreenSelection)+".jpg").c_str());
    if(!loadTexture){
        cerr<<"Failed to load loadscreen!"<<IMG_GetError()<<endl;
        return;
    }
    SDL_Rect menuRect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,loadTexture,nullptr,&menuRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(loadTexture);
}

void Game::handlePauseEvent(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_KEYDOWN){
            switch(event.key.keysym.sym){
            case SDLK_UP:Mix_PlayChannel(-1,menuSwitch,0);pauseSelection= (pauseSelection-1+3)%3;break;
            case SDLK_DOWN:Mix_PlayChannel(-1,menuSwitch,0);pauseSelection=(pauseSelection+1+3)%3;break;
            case SDLK_SPACE:
                Mix_PlayChannel(-1,confirm,0);
                if(pauseSelection==0){
                    gstate=prevState;
                    playMusic(prevMusic);
                }
                else if(pauseSelection==1) saveGame();
                else if(pauseSelection==2){
                        gstate=MENU;
                        playMusic(menuMusic);
                        break;
                }
            }
        }
    }
}

void Game::renderPause(int pauseSelection){
    string pausescr="pause/";
    SDL_Texture* pauseTexture=IMG_LoadTexture(renderer,(pausescr+to_string(pauseSelection)+".jpg").c_str());
    if(!pauseTexture){
        cerr<<"Failed to load pausescreen!"<<IMG_GetError()<<endl;
        return;
    }
    SDL_Rect menuRect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,pauseTexture,nullptr,&menuRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(pauseTexture);
}

void Game::handleLoss(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_UP: {Mix_PlayChannel(-1,menuSwitch,0);loseSelection=(loseSelection-1+2)%2;break;}
                case SDLK_DOWN: {Mix_PlayChannel(-1,menuSwitch,0);loseSelection=(loseSelection+1+2)%2;break;}
                case SDLK_SPACE: if(Mix_PlayChannel(-1,confirm,0);loseSelection==0) {gstate=MENU;playMusic(menuMusic);break;}
                                else {running=false;break;}
            }
        }
    }
}

void Game::renderLoss(int loseSelection){
    string losescr="Lose/";
    SDL_Texture* loseTexture=IMG_LoadTexture(renderer,(losescr+to_string(loseSelection)+".jpg").c_str());
    if(!loseTexture){
        cerr<<"Failed to load lose screen!"<<IMG_GetError()<<endl;
    }
    SDL_Rect menuRect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,loseTexture,nullptr,&menuRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(loseTexture);
}

void Game::handleWin(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_UP: {Mix_PlayChannel(-1,menuSwitch,0);winSelection=(winSelection-1+2)%2;break;}
                case SDLK_DOWN: {Mix_PlayChannel(-1,menuSwitch,0);winSelection=(winSelection+1+2)%2;break;}
                case SDLK_SPACE: {Mix_PlayChannel(-1,confirm,0);if(winSelection==0) {gstate=MENU;playMusic(menuMusic);break;}
                                else{ running=false;break;}}
            }
        }
    }
}

void Game::renderWin(int winSelection){
    string winscr="Win/";
    SDL_Texture* winTexture=IMG_LoadTexture(renderer,(winscr+to_string(winSelection)+".jpg").c_str());
    if(!winTexture){
        cerr<<"Failed to load victory screen!"<<IMG_GetError()<<endl;
    }
    SDL_Rect menuRect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,winTexture,nullptr,&menuRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(winTexture);
}

void Game::run() {
    std::cerr << "Starting game loop..." << std::endl;
    while (running) {
        if(gstate==MENU){
            handleMenuEvent();
            renderMenu(menuSelection);
        }
        else if(gstate==PAUSE){
            handlePauseEvent();
            renderPause(pauseSelection);
        }
        else if(gstate==LOADSCREEN){
            handleLoadEvent();
            renderLoad(loadScreenSelection);
        }
        else if(gstate==VICTORY){
            handleWin();
            renderWin(winSelection);
        }
        else if(gstate==LOSS){
            handleLoss();
            renderLoss(loseSelection);
        }
        else{
            handleEvent();
            render();
            update();
            SDL_Delay(16);
        }
    }
    std::cerr << "Game loop exited!" << std::endl;
}

void Game::render() {

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i < MAP_HEIGHT - 1; i++) {
        for (int j = 1; j < MAP_WIDTH - 1; j++) {
            SDL_Rect tile = { j * TITLE_SIZE, i * TITLE_SIZE, TITLE_SIZE, TITLE_SIZE };
            SDL_RenderFillRect(renderer, &tile);
        }
    }
    for(int i=0;i<walls.size();i++){
        if(walls[i].type==BRICK){
            SDL_Rect wallscr={1051,0,55,55};
            walls[i].setSpriteSheet(spritesheet,wallscr);
        }
        else if(walls[i].type==STEEL){
            SDL_Rect wallscr={1051,64,55,55};
            walls[i].setSpriteSheet(spritesheet,wallscr);
        }
        else if(walls[i].type==BUSH){
            SDL_Rect wallscr={1116,128,55,55};
            walls[i].setSpriteSheet(spritesheet,wallscr);
        }
    }
    for(auto &Wall:walls){
        if(Wall.type!=BUSH){
            Wall.render(renderer);
        }
    }

    if (player.active) {
        SDL_Rect player1scr;
        switch (player.direction) {
            case UP:    player1scr = {3 , 7, 55, 55}; break;
            case DOWN:  player1scr = {262, 3, 56, 55}; break;
            case RIGHT: player1scr = {126 , 66, 68, 56}; break;
            case LEFT:  player1scr = {391 , 3, 54, 55}; break;
    }
        player.setSpriteSheet(spritesheet, player1scr);
        player.render(renderer);
    }
    else if (player.explosionFrame >= 0) {
        SDL_Rect explosionSrc = { 1058+(player.explosionFrame%3) * 58, 516, 58, 58 };
        SDL_Rect explosionDest = {player.x, player.y, TITLE_SIZE, TITLE_SIZE};
        SDL_RenderCopy(renderer, spritesheet, &explosionSrc, &explosionDest);
        if (++player.explosionFrame > 2) {
            player.explosionFrame = -1;
        }
    }
    if (gstate == MULTIPLAYER) {
        if (player2.active) {
            SDL_Rect player2scr;
            switch (player2.direction) {
                case UP:    player2scr = {2, 529, 57, 57}; break;
                case DOWN:  player2scr = {262, 527, 56, 54}; break;
                case RIGHT: player2scr = {126, 589, 68, 59}; break;
                case LEFT:  player2scr = {390, 526, 56, 57}; break;
            }
            player2.setSpriteSheet(spritesheet, player2scr);
            player2.render(renderer);
        }
        else if (player2.explosionFrame >= 0) {
            SDL_Rect explosionSrc = { 1058+(player2.explosionFrame%3) * 58, 516, 58, 58 };
            SDL_Rect explosionDest = {player2.x, player2.y, TITLE_SIZE, TITLE_SIZE};
            SDL_RenderCopy(renderer, spritesheet, &explosionSrc, &explosionDest);
            if (++player2.explosionFrame > 2) {
                player2.explosionFrame = -1;
            }
        }
    }

    for (auto &enemy : enemies) {
        if (enemy.active) {
            SDL_Rect enemyscr;
            switch (enemy.direction) {
                case UP:    enemyscr = {521, 530, 57, 56}; break;
                case DOWN:  enemyscr = {778, 526, 57, 55}; break;
                case RIGHT: enemyscr = {651, 526, 59, 56}; break;
                case LEFT:  enemyscr = {906, 526, 55, 56}; break;
            }
            enemy.setSpriteSheet(spritesheet, enemyscr);
            enemy.render(renderer);
        }
        if (enemy.explosionFrame >= 0) {
            SDL_Rect explosionSrc = { 1058+(enemy.explosionFrame%3) * 58, 516, 58, 58 };
            SDL_Rect explosionDest = {enemy.x, enemy.y, TITLE_SIZE, TITLE_SIZE};
            SDL_RenderCopy(renderer, spritesheet, &explosionSrc, &explosionDest);
            if (++enemy.explosionFrame > 3) {
                enemy.explosionFrame = -1;
            }
        }
    }
    for(auto &Wall:walls){
        if(Wall.type==BUSH){
            Wall.render(renderer);
        }
    }
    if (currentLevel == 3) {
        if (boss.active || boss.explosionFrame >= 0) {
            boss.render(renderer);
        }
    }
    SDL_RenderPresent(renderer);
}

void Game::handleMenuEvent() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
            return;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    Mix_PlayChannel(-1,menuSwitch,0);
                    menuSelection = (menuSelection - 1 + 3) % 3;
                    break;
                case SDLK_DOWN:
                    Mix_PlayChannel(-1,menuSwitch,0);
                    menuSelection = (menuSelection + 1 + 3) % 3;
                    break;
                case SDLK_SPACE:
                    Mix_PlayChannel(-1,confirm,0);
                    if (menuSelection == 0 || menuSelection == 1) {
                        gstate = (menuSelection == 0) ? SINGLEPLAYER : MULTIPLAYER;
                        cout<<"gstate "<<gstate<<endl;
                        currentLevel=1;
                        loadLevel(currentLevel);
                        ifstream saveFile((gstate == MULTIPLAYER) ? "saves/multi.txt" : "saves/single.txt");
                        if (saveFile && saveFile.peek() != EOF) {
                            saveFile.close();
                            prevState = gstate;
                            gstate = LOADSCREEN;
                        }
                    } else {
                        running = false;
                    }
                    break;
            }
        }
    }
}

void Game::renderMenu(const int &menuSelection){
    string imagePath;
    switch(menuSelection){
        case 0: imagePath="MenuImgs/Menu0.jpg";break;
        case 1: imagePath="MenuImgs/Menu1.jpg";break;
        case 2: imagePath="MenuImgs/Menu2.jpg";break;
    }
    SDL_Texture* menuTexture=IMG_LoadTexture(renderer,imagePath.c_str());
    if(!menuTexture){
        cerr<<"Failed to load menu image! "<<IMG_GetError()<<endl;
        return;
    }
    SDL_Rect menuRect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,menuTexture,nullptr,&menuRect);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(menuTexture);
}
void Game::loadLevel(int level){
    enemies.clear();
    walls.clear();
    player.bullets.clear();
    player2.bullets.clear();
    boss.bullets.clear();
    boss.active=false;
    boss.lazer.active=false;
    string mapFile="maps/"+to_string(level)+".txt";
    enemynumber=1+level;
    generateWall(mapFile);
    spawnEnemyTank();
    player2.active = (prevState == MULTIPLAYER);
    if(currentLevel==3){
        player=PlayerTank((MAP_WIDTH-18)*TITLE_SIZE,(MAP_HEIGHT-2)*TITLE_SIZE);
        player.active=true;
        if(player2.active){
            player2 = PlayerTank((MAP_WIDTH-17)*TITLE_SIZE, (MAP_HEIGHT-2)*TITLE_SIZE);
            player2.active=true;
        }
    }
    else {
            player=PlayerTank(((MAP_WIDTH-1)/2)*TITLE_SIZE,(MAP_HEIGHT-2)*TITLE_SIZE);
    player.active=true;
    if(player2.active){
        player2 = PlayerTank(((MAP_WIDTH-3)/2)*TITLE_SIZE, (MAP_HEIGHT-2)*TITLE_SIZE);
        player2.active=true;
    }}
    if(currentLevel==1) playMusic(level1Music);
    else if(currentLevel==2) {
            playMusic(level2Music);
            boss.health=5;
    }
    else if (currentLevel == 3) {
            playMusic(bossMusic);
            boss.active = true;
            boss.loadFrames(renderer);
            boss.lazer.loadFrames(renderer);
            boss.loadExplosionTexture(renderer);
    }
}

void Game::generateWall(const string &mapFile){
    ifstream file(mapFile.c_str());
    if(!file){
        cerr << "Cannot open txt map" << endl;
        return;
    }

    string line;
    int row = 0;

    while(getline(file, line)){
        for(int col = 0; col < line.size(); col++){
            char tile = line[col];
            if(tile == '1'){
                walls.push_back(Wall(col * TITLE_SIZE, row * TITLE_SIZE, BRICK));
            }
            else if(tile == '2'){
                walls.push_back(Wall(col * TITLE_SIZE, row * TITLE_SIZE, STEEL));
            }
            else if(tile == '3'){
                walls.push_back(Wall(col * TITLE_SIZE, row * TITLE_SIZE, BUSH));
            }
        }
        row++;
    }

    file.close();
    cerr << "Map loaded successfully, total walls: " << walls.size() << endl;
}

void Game::spawnEnemyTank(){
    enemies.clear();
    for(int i=0;i<enemynumber;i++){
        int ex,ey;
        bool validPos=false;
        while(!validPos){
            ex=(rand()%(MAP_WIDTH/2-2)+1)*TITLE_SIZE;
            ey=(rand()%(MAP_HEIGHT/2-2)+1)*TITLE_SIZE;
            if(currentLevel==3){
                if(ex<280||ex>520){
                    validPos=true;
                }
            }
            else validPos=true;
            for(const auto &Wall:walls){
                if(Wall.active&&Wall.x==ex&&Wall.y==ey){
                    validPos=false;
                    break;
                }
            }
        }
        enemies.push_back(EnemyTank(ex,ey));
    }
}

void Game::saveGame(){
    cout << "Saving game - prevState: " << prevState << endl;
    string filename = (prevState == MULTIPLAYER) ? "saves/multi.txt" : "saves/single.txt";
    ofstream saveFile(filename, ios::trunc);
    if (!saveFile) {
        cerr << "Error! Cannot save game to " << filename << endl;
        return;
    }
    saveFile<<prevState<<endl;
    saveFile<<currentLevel<<endl;
    saveFile<<player.x<<" "<<player.y<<" "<<player.direction<<" "<<player.active<<endl;
    if(prevState==MULTIPLAYER){
        saveFile<<player2.x<<" "<<player2.y<<" "<<player2.direction<<" "<<player2.active<<endl;
    }
    saveFile<<walls.size()<<endl;
    for(auto &Wall:walls){
        saveFile<<Wall.x<<" "<<Wall.y<<" "<<Wall.type<<" "<<Wall.active<<endl;
    }
    saveFile<<enemies.size()<<endl;
    for(auto &enemy:enemies){
        saveFile<<enemy.x<<" "<<enemy.y<<" "<<enemy.direction<<" "<<enemy.active<<endl;
    }
    if (currentLevel == 3) {
        saveFile << boss.health << " " << boss.active << endl;
    }
    saveFile.close();
    cout<<"Game saved!"<<endl;
}

void Game::loadGame(){
    string filename = (prevState == MULTIPLAYER) ? "saves/multi.txt" : "saves/single.txt";
    ifstream loadFile(filename);
    if (!loadFile) {
        cerr << "Error! No save file found: " << filename << endl;
        return;
    }

    walls.clear();
    enemies.clear();
    int savedGstate;
    loadFile >> savedGstate;
    gstate = static_cast<GameState>(savedGstate);

    loadFile >> currentLevel;

    int x1, y1, dir1, active1;
    loadFile >> x1 >> y1 >> dir1 >> active1;
    player = PlayerTank(x1, y1);
    player.direction = static_cast<Direction>(dir1);
    player.active = active1;

    if (gstate == MULTIPLAYER) {
        int x2, y2, dir2, active2;
        loadFile >> x2 >> y2 >> dir2 >> active2;
        player2 = PlayerTank(x2, y2);
        player2.direction = static_cast<Direction>(dir2);
        player2.active = active2;
    } else {
        player2.active = false;
    }

    int wallCount;
    loadFile >> wallCount;
    for (int i = 0; i < wallCount; i++) {
        int x, y, type, active;
        loadFile >> x >> y >> type >> active;
        walls.emplace_back(x, y, (WallType)type);
        walls.back().active = active;
    }

    int enemyCount;
    loadFile >> enemyCount;
    for (int i = 0; i < enemyCount; i++) {
        int x, y, direction, active;
        loadFile >> x >> y >> direction >> active;
        enemies.emplace_back(x, y);
        enemies.back().direction = static_cast<Direction>(direction);
        enemies.back().active = active;
    }

    if (currentLevel == 3) {
        loadFile >> boss.health >> boss.active;
        if (boss.active) {
            boss.loadFrames(renderer);
            boss.lazer.loadFrames(renderer);
            boss.loadExplosionTexture(renderer);
        }
    }
    if(currentLevel==1) playMusic(level1Music);
    else if(currentLevel==2) playMusic(level2Music);
    else if (currentLevel == 3) {
    playMusic(bossMusic);}

    loadFile.close();
}

void Game::update(){
    if((currentLevel==3&&enemies.empty())&&boss.active==false){
        gstate=VICTORY;
        playMusic(winSound);
    }
    if(enemies.empty()&&currentLevel!=3){
        currentLevel++;
        loadLevel(currentLevel);
    }
    player.updateBullets(walls);
    player2.updateBullets(walls);
    player.ShootDelay--;
    player2.ShootDelay--;
    for(auto &enemy:enemies){
        enemy.move(walls,enemies,player,player2);
        enemy.updateBullets(walls,enemies,player,player2);
        if(rand()%100<2){
            enemy.shoot();
        }
    }
    if (currentLevel == 3) {
        boss.updateBullets(walls, player,player2);
        boss.updateAnimation();
        if(boss.active){
            if (rand() % 100 < 3) {
                boss.shoot();
            }
            if(rand()%300<3){
                boss.shootLazer();
            }
        }
    }
    for(auto &enemy:enemies){
        for(auto &Bullet:enemy.bullets){
            for(auto &Wall:walls){
                if(Wall.active&&Wall.type==BRICK&&SDL_HasIntersection(&Bullet.rect,&Wall.rect)){
                    Wall.active=false;
                    Bullet.active=false;
                    break;
                }
                else if(Wall.active&&Wall.type==STEEL&&SDL_HasIntersection(&Bullet.rect,&Wall.rect)){
                    Bullet.active=false;
                    break;
                }
                else if(Wall.active&&Wall.type==BUSH&&SDL_HasIntersection(&Bullet.rect,&Wall.rect)){
                    Bullet.inBush=true;
                    break;
                }
            }
        }
    }
    enemies.erase(remove_if(enemies.begin(), enemies.end(),
                        [](EnemyTank &e) { return e.explosionFrame == -1&&!e.active; }),
              enemies.end());
    for(auto &enemy:enemies){
        for(auto &Bullet1:player.bullets){
            for(auto &Bullet2:enemy.bullets){
                if(Bullet1.active&&Bullet2.active&&SDL_HasIntersection(&Bullet1.rect,&Bullet2.rect)){
                    Bullet1.active=false;
                    Bullet2.active=false;
                }
            }
        }
    }
    for(auto &enemy:enemies){
        for(auto &Bullet1:player2.bullets){
            for(auto &Bullet2:enemy.bullets){
                if(Bullet1.active&&Bullet2.active&&SDL_HasIntersection(&Bullet1.rect,&Bullet2.rect)){
                    Mix_PlayChannel(-1,explosionSound,0);
                    Bullet1.active=false;
                    Bullet2.active=false;
                }
            }
        }
    }
    if(player.active){
        for(auto &Bullets:player.bullets){
            for(auto &Wall:walls){
                if(Wall.active&&Wall.type==BRICK&&SDL_HasIntersection(&Bullets.rect,&Wall.rect)){
                    Wall.active=false;
                    Bullets.active=false;
                    break;
                }
                else if(Wall.active&&Wall.type==STEEL&&SDL_HasIntersection(&Bullets.rect,&Wall.rect)){
                    Bullets.active=false;
                    break;
                }
                else if(Wall.active&&Wall.type==BUSH&&SDL_HasIntersection(&Bullets.rect,&Wall.rect)){
                    Bullets.inBush=true;
                    break;
                }
            }
        }
        for(auto &Bullet:player.bullets){
            for(auto &enemy:enemies){
                if(enemy.active&&SDL_HasIntersection(&Bullet.rect,&enemy.rect)){
                    Mix_PlayChannel(-1,explosionSound,0);
                    enemy.explosionFrame=0;
                    enemy.active=false;
                    Bullet.active=false;
                }
            }
        }
        for(auto &enemy:enemies){
            for(auto &Bullet:enemy.bullets){
                if(SDL_HasIntersection(&Bullet.rect,&player.rect)){
                    Mix_PlayChannel(-1,deathSound,0);
                    Bullet.active=false;
                    player.explosionFrame=0;
                    player.active=false;
                }
            }
        }
        if(boss.lazer.active&&boss.lazer.canCollide){
            if(SDL_HasIntersection(&player.rect,&boss.lazer.rect)){
                player.explosionFrame=0;
                player.active=false;
            }
        }
        for(auto &Bullet:boss.bullets){
            if(SDL_HasIntersection(&Bullet.rect,&player.rect)){
                Bullet.active=false;
                player.explosionFrame=0;
                player.active=false;
            }
        }

        for (size_t i = 0; i < player.bullets.size(); i++) {
            if (boss.active&&SDL_HasIntersection(&player.bullets[i].rect, &boss.rect)) {
                boss.health--;
                player.bullets[i].active = false;
            }
        }
    }
    if(player2.active){
        for(auto &Bullets:player2.bullets){
            for(auto &Wall:walls){
                if(Wall.active&&Wall.type==BRICK&&SDL_HasIntersection(&Bullets.rect,&Wall.rect)){
                    Wall.active=false;
                    Bullets.active=false;
                    break;
                }
                else if(Wall.active&&Wall.type==STEEL&&SDL_HasIntersection(&Bullets.rect,&Wall.rect)){
                    Bullets.active=false;
                    break;
                }
                else if(Wall.active&&Wall.type==BUSH&&SDL_HasIntersection(&Bullets.rect,&Wall.rect)){
                    Bullets.inBush=true;
                    break;
                }
            }
        }
        for(auto &Bullet:player2.bullets){
            for(auto &enemy:enemies){
                if(enemy.active&&SDL_HasIntersection(&Bullet.rect,&enemy.rect)){
                    Mix_PlayChannel(-1,explosionSound,0);
                    enemy.explosionFrame=0;
                    enemy.active=false;
                    Bullet.active=false;
                }
            }
        }
        for(auto &enemy:enemies){
            for(auto &Bullet:enemy.bullets){
                if(SDL_HasIntersection(&Bullet.rect,&player2.rect)){
                    Mix_PlayChannel(-1,deathSound,0);
                    Bullet.active=false;
                    player2.explosionFrame=0;
                    player2.active=false;
                }
            }
        }
        if(boss.lazer.active&&boss.lazer.canCollide){
            if(SDL_HasIntersection(&player2.rect,&boss.lazer.rect)){
                player2.explosionFrame=0;
                player2.active=false;
            }
        }
        for (size_t i = 0; i < boss.bullets.size(); i++) {
            if (SDL_HasIntersection(&boss.bullets[i].rect, &player2.rect)) {
                player2.active = false;
                boss.bullets[i].active=false;
            }
        }
        for (size_t i = 0; i < player2.bullets.size(); i++) {
            if (boss.active&&SDL_HasIntersection(&player2.bullets[i].rect, &boss.rect)) {
                boss.health--;
                player2.bullets[i].active = false;
            }
        }
    }
    if (boss.health <= 0 && boss.explosionFrame == -1&&boss.active) {
        boss.explosionFrame = 0;
        boss.active = false;
    }
    if(!player2.active&&!player.active){
        gstate=LOSS;
        playMusic(loseSound);
        return;
    }
}
Game::~Game() {
    freeAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
