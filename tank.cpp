#include "tank.h"
PlayerTank::PlayerTank(int startX,int startY){
    ShootDelay=20;
    x=startX;
    y=startY;
    active=false;
    rect={x,y,TITLE_SIZE,TITLE_SIZE};
    dirX=0;
    dirY=1;
}
void PlayerTank::render(SDL_Renderer* renderer){
    if(active){
        if(spritesheet){
            SDL_RenderCopy(renderer,spritesheet,&scrRect,&rect);
        }
        else{
            SDL_SetRenderDrawColor(renderer,255,0,0,255);
            SDL_RenderFillRect(renderer,&rect);
        }
        for(auto &Bullets:bullets){
            Bullets.render(renderer);
        }
    }
}
void PlayerTank::setSpriteSheet(SDL_Texture* sheet,SDL_Rect source){
    spritesheet=sheet;
    scrRect=source;
}
void PlayerTank::move(int dx,int dy,const vector<Wall>&walls,const vector<EnemyTank>enemies){
    //Quyet dinh huong di cua xe tank dua vao dx dy
    if(dx>0) direction=LEFT;
    else if(dx<0) direction=RIGHT;
    else if(dy>0) direction=DOWN;
    else direction=UP;
    //di chuyen theo dx dy nhap tu ban phim
    int newX=x+dx;
    int newY=y+dy;
    this->dirX=dx;
    this->dirY=dy;
    SDL_Rect newRect={newX,newY,TITLE_SIZE,TITLE_SIZE};
    for(int i=0;i<walls.size();i++){
        if(walls[i].active&&walls[i].type!=BUSH&&SDL_HasIntersection(&newRect,&walls[i].rect)){ //Xac dinh va cham voi tuong
            return;
        }
    }
    if(newX>=TITLE_SIZE&&newX<=SCREEN_WIDTH-TITLE_SIZE*2&&newY>=TITLE_SIZE&&newY<=SCREEN_HEIGHT-TITLE_SIZE*2){ // chi di chuyen khi o trong man hinh
        x=newX;
        y=newY;
        rect.x=x;
        rect.y=y;
       }
}
void PlayerTank::shoot(){
    if(ShootDelay>0) return;
    Mix_PlayChannel(-1,shootSound,0);
    ShootDelay=20;
    bullets.push_back(Bullets(x+TITLE_SIZE/2-5,y+TITLE_SIZE/2-5,
                              this->direction)); //Khoi tao dan vao vector
}
void PlayerTank::updateBullets(const vector<Wall> &walls){
    for(auto &Bullets:bullets){
        Bullets.move();
        Bullets.inBush=false;
        for(auto &Wall:walls){
            if(Wall.active&&Wall.type==BUSH&&SDL_HasIntersection(&Bullets.rect,&Wall.rect)){
                Bullets.inBush=true;
                break;
            }
        }
    }
    bullets.erase(remove_if(bullets.begin(),bullets.end(),[](Bullets &b){return !b.active;}),bullets.end()); //Xoa dan neu khong con hoat dong
}

EnemyTank::EnemyTank(int startX,int startY){
    x=startX;
    y=startY;
    rect={x,y,TITLE_SIZE,TITLE_SIZE};
    dirX=0;
    dirY=1;
    shootDelay=5;
    moveDelay=0;
    randomMoveTimer=0;
    active=true;
}
void EnemyTank::move(const vector<Wall>&walls,const vector<EnemyTank>enemies,PlayerTank &player,PlayerTank &player2){
    if(--moveDelay>0) return; //Chi di chuyen khi het thoi gian cho
    moveDelay=10;
    if(randomMoveTimer>0){ //Chi doi huong ngau nhien khi het thoi gian cho
        randomMoveTimer--;
    }
    else{
        randomDirection(SCREEN_HEIGHT);
        randomMoveTimer=10;
    }
    if(dirX>0) direction=LEFT;
    else if(dirX<0) direction=RIGHT;
    else if(dirY>0) direction=DOWN;
    else direction=UP;
    int newX=x+this->dirX;
    int newY=y+this->dirY;
    SDL_Rect newRect={newX,newY,TITLE_SIZE,TITLE_SIZE};
    for(int i=0;i<walls.size();i++){
        if(walls[i].active&&walls[i].type!=BUSH&&SDL_HasIntersection(&newRect,&walls[i].rect)){
            return;
        }
    }

    if(SDL_HasIntersection(&this->rect,&player.rect)||SDL_HasIntersection(&this->rect,&player2.rect)){
       return;
       }
    if(newX>=TITLE_SIZE&&newX<=SCREEN_WIDTH-TITLE_SIZE*2&&
       newY>=TITLE_SIZE&&newY<=SCREEN_HEIGHT-TITLE_SIZE*2){
        x=newX;
        y=newY;
        rect.x=x;
        rect.y=y;
    }
}
//Ham doi huong ngau nhien
void EnemyTank::randomDirection(int screenheight){
    int ranDir= rand()%100;
    if(y<screenheight/2){ //Neu o nua tren man hinh thi di chuyen xuong nhieu hon
        if(ranDir<50){dirX=0;dirY=10;}
        else if(ranDir<70){dirX=-10;dirY=0;}
        else if(ranDir<90) {dirX=10;dirY=0;}
        else {dirX=0;dirY=-10;}
    }
    else{
        switch(rand()%4){ //neu ow nua duoi thi 4 huong nhu nhau
            case 0:dirX=10;dirY=0;break;
            case 1:dirX=-10;dirY=0;break;
            case 2:dirX=0;dirY=10;break;
            case 3:dirX=0;dirY=-10;break;
        }
    }
}

void EnemyTank::shoot(){
    if(--shootDelay>0) return;
    Mix_PlayChannel(-1,enemyShootSound,0);
    shootDelay=5;
    bullets.push_back(Bullets(x+TITLE_SIZE/2-5,y+TITLE_SIZE/2-5,
                              this->direction));
}

void EnemyTank::updateBullets(const vector<Wall>&walls,const vector<EnemyTank>enemies,const PlayerTank &player,const PlayerTank &player2){
    for(auto &Bullets:bullets){
        Bullets.move();
        Bullets.inBush=false;
        for(auto &Wall:walls){
            if(Wall.active&&Wall.type==BUSH&&SDL_HasIntersection(&Bullets.rect,&Wall.rect)){
                Bullets.inBush=true;
                break;
            }
        }
    }
    bullets.erase(remove_if(bullets.begin(),bullets.end(),[](Bullets &b){return !b.active;}),bullets.end());
}

void EnemyTank::render(SDL_Renderer* renderer){
    if(active){
        if(spritesheet){
            SDL_RenderCopy(renderer,spritesheet,&scrRect,&rect);
        }
        else{
            SDL_SetRenderDrawColor(renderer,255,0,0,255);
            SDL_RenderFillRect(renderer,&rect);
        }
        for(auto &Bullets:bullets){
        Bullets.render(renderer);
        }
    }
}
void EnemyTank::setSpriteSheet(SDL_Texture* sheet,SDL_Rect source){
    spritesheet=sheet;
    scrRect=source;
}

Boss::Boss(int _x, int _y):lazer(0,0){
    x=_x;
    y=_y;
    health = 5;
    currentFrame = 0;
    animationSpeed = 50;
    frameCount = 1;
    atkType = IDLE;
    shootDelay=5;
    lazerDelay=10;
    rect.x=x;
    rect.y=y;
    rect.w=160;
    rect.h=160;
}

void Boss::loadFrames(SDL_Renderer* renderer) {
    for (int i = 0; i < 8; i++) {
        string filename = "BossAssets/" + std::to_string(i) + ".png"; //Tai hinh anh boss file
        bossFrames[i] = IMG_LoadTexture(renderer, filename.c_str());
        if (!bossFrames[i]) {
            std::cerr << "Failed to load " << filename << " - " << IMG_GetError() << std::endl; //bao loi
        }
    }
}

void Boss::loadExplosionTexture(SDL_Renderer* renderer) {
    string filename="BossAssets/Explosion.png"; //Tai hoat anh no boss tu file
    explosionTexture = IMG_LoadTexture(renderer, filename.c_str( ));
    if (!explosionTexture) {
        std::cerr << "Failed to load boss explosion spritesheet! " << IMG_GetError() << std::endl; //Bao loi
    }
}


void Boss::render(SDL_Renderer* renderer) {
    if (explosionFrame >= 0 && explosionTexture) { //Khi trong thai phat no
        SDL_Rect explosionSrc = { explosionFrame * 96, 0, 96, 96 };
        SDL_Rect explosionDest = { x, y, 160, 160 };
        SDL_RenderCopy(renderer, explosionTexture, &explosionSrc, &explosionDest);
    }
    if(active){
        if (bossFrames[currentFrame]) {
            SDL_RenderCopy(renderer, bossFrames[currentFrame], nullptr, &rect);
        }
        for(auto &Bullet:bullets){
            Bullet.render(renderer);
        }
        if (lazer.active) {
            lazer.render(renderer);
        }
    }
}

void Boss::updateAnimation() {
    if (explosionFrame >= 0) { //Khi dc ve 0 thif bj phat no
        if (++explosionCounter >= explosionSpeed) {
            explosionFrame++;
            explosionCounter = 0;
            if (explosionFrame >= 11) { //Khi no xong
                explosionFrame = -1;
            }
        }
    }
    static int frameCounter = 0;
    frameCounter++;
    if (frameCounter >= animationSpeed) {
        currentFrame = (currentFrame + 1) % frameCount;
        frameCounter = 0;

        if (currentFrame == 0) {
            atkType = IDLE; //Dung yen
            frameCount = 1;
            lazer.deactivate(); //Tat lazer
        }
    }
    if(lazer.active){
        lazer.update();
    }
}

void Boss::shoot(){
    if(--shootDelay>0) return;
    Mix_PlayChannel(-1,enemyShootSound,0);
    shootDelay=5;
    atkType=BULLET;
    currentFrame = 2;
    frameCount = 2;
    animationSpeed = 8;
    bullets.push_back(Bullets(x+160/2-5,y+125,DOWN));
}

void Boss::shootLazer(){
    if(--lazerDelay>0) return;
    Mix_PlayChannel(-1,lazerSound,0);
    lazerDelay=10;
    atkType=LAZER;
    currentFrame=4;
    frameCount=7;
    animationSpeed=10;
    lazer.activate(x+50,y+170);
}

void Boss::updateBullets(const vector<Wall>&walls,const PlayerTank &player,const PlayerTank &player2){
    for(auto &Bullets:bullets){
        Bullets.move();
        Bullets.inBush=false;
        for(auto &Wall:walls){
            if(Wall.active&&Wall.type==BUSH&&SDL_HasIntersection(&Bullets.rect,&Wall.rect)){
                Bullets.inBush=true;
                break;
            }
        }
    }
    bullets.erase(remove_if(bullets.begin(),bullets.end(),[](Bullets &b){return !b.active;}),bullets.end());
}


Boss::~Boss(){
    for (int i = 0; i < 8; i++) {
        if (bossFrames[i]) {
            SDL_DestroyTexture(bossFrames[i]);
        }
    }
}

