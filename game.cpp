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
    spritesheet=loadTexture("assets.png");
    loadLevel(currentLevel);
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
                if(gstate==PAUSE) gstate=(gstate==SINGLEPLAYER)?SINGLEPLAYER:MULTIPLAYER;
                else{
                    prevState=gstate;
                    gstate=PAUSE;
                }
            }
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
                case SDLK_x:player2.shoot();break;
            }
        }
    }

}
void Game::handlePauseEvent(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.key.keysym.sym){
        case SDLK_UP:pauseSelection= (pauseSelection-1+3)%3;break;
        case SDLK_DOWN:pauseSelection=(pauseSelection+1+3)%3;break;
        case SDLK_SPACE:
            if(pauseSelection==0){
                gstate=prevState;
                SDL_FlushEvent(SDL_KEYDOWN);
                SDL_FlushEvent(SDL_KEYUP);
            }
            else if(pauseSelection==1) gstate=MENU;
            else if(pauseSelection==2) gstate=MENU;
            break;
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
    if(gstate!=MENU){
    player2.active=false;
    SDL_Rect player1scr,player2scr;
        switch(player.direction){
            case UP:{player1scr={3,7,55,55};break;}
            case DOWN:{player1scr={262,3,56,55};break;}
            case RIGHT:{player1scr={126,66,68,56};break;}
            case LEFT:{player1scr={391,3,54,55};break;}
            }
            player.setSpriteSheet(spritesheet,player1scr);
            player.render(renderer);
            if(gstate==MULTIPLAYER){
                player2.active=true;
                switch(player2.direction){
                case UP:{player2scr={2,529,57,57};break;}
                case DOWN:{player2scr={262,527,56,54};break;}
                case RIGHT:{player2scr={126,589,68,59};break;}
                case LEFT:{player2scr={390,526,56,57};break;}
                    };
                    player2.setSpriteSheet(spritesheet,player2scr);
                    player2.render(renderer);
            }
    }
    for(auto &enemy:enemies){
            SDL_Rect enemyscr;
    switch(enemy.direction){
        case UP:{enemyscr={521,530,57,56};break;}
        case DOWN:{enemyscr={778,526,57,55};break;}
        case RIGHT:{enemyscr={651,526,59,56};break;}
        case LEFT:{enemyscr={906,526,55,56};break;}
        };
        enemy.setSpriteSheet(spritesheet,enemyscr);
    }
    for(auto &enemy:enemies){
        enemy.render(renderer);
    }
    for(auto &Wall:walls){
        if(Wall.type==BUSH){
            Wall.render(renderer);
        }
    }
    SDL_RenderPresent(renderer);
}
void Game::handleMenuEvent(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type==SDL_QUIT){
            running=false;
            return;
        }
        if(event.type==SDL_KEYDOWN){
            switch(event.key.keysym.sym){
            case SDLK_UP:
                menuSelection=((menuSelection-1+3)%3);
                break;
            case SDLK_DOWN:
                menuSelection=((menuSelection+1+3)%3);
                break;
            case SDLK_SPACE:
                if(menuSelection==0) gstate=SINGLEPLAYER;
                else if(menuSelection==1) gstate=MULTIPLAYER;
                else if(menuSelection==2) running=false;
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
    string mapFile="maps/"+to_string(level)+".txt";
    enemynumber=5+level;
    generateWall(mapFile);
    spawnEnemyTank();
    player=PlayerTank(((MAP_WIDTH-1)/2)*TITLE_SIZE,(MAP_HEIGHT-2)*TITLE_SIZE);
    player2=PlayerTank(((MAP_WIDTH-3)/2)*TITLE_SIZE,(MAP_HEIGHT-2)*TITLE_SIZE);
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
            validPos=true;
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

void Game::update(){
    if(enemies.empty()){
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
    for(auto &Bullet:player.bullets){
        for(auto &enemy:enemies){
            if(enemy.active&&SDL_HasIntersection(&Bullet.rect,&enemy.rect)){
                enemy.active=false;
                Bullet.active=false;
            }
        }
    }
    for(auto &Bullet:player2.bullets){
        for(auto &enemy:enemies){
            if(enemy.active&&SDL_HasIntersection(&Bullet.rect,&enemy.rect)){
                enemy.active=false;
                Bullet.active=false;
            }
        }
    }
    enemies.erase(remove_if(enemies.begin(),enemies.end(),
                            [](EnemyTank &e){return !e.active;}),enemies.end());
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
                    Bullet1.active=false;
                    Bullet2.active=false;
                }
            }
        }
    }
    if(player.active){
    for(auto &enemy:enemies){
        for(auto &Bullet:enemy.bullets){
            if(SDL_HasIntersection(&Bullet.rect,&player.rect)){
                Bullet.active=false;
                player.active=false;
            }
        }
    }
    }
    if(player2.active){
    for(auto &enemy:enemies){
        for(auto &Bullet:enemy.bullets){
            if(SDL_HasIntersection(&Bullet.rect,&player2.rect)){
                Bullet.active=false;
                player2.active=false;
            }
        }
    }
    }
    if(!player2.active&&!player.active){
        running=false;
        return;
    }
}
Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
