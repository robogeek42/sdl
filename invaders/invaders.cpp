#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "../sdl_sprite/Sprite.h"

const int ZM = 4; // Zoom

//Screen dimension constants
const int SWIDTH  = 224*ZM;
const int SHEIGHT = 256*ZM;
const int STOP = 32*ZM; // 2 lines of score and text at top of screen
const int SSHIP = 32*ZM; // Height of Spaceship row
const int SBOT = SHEIGHT - 16*ZM; // Line of text/info at bottom
const int SCANNON = SBOT - (24*ZM); // top of cannon
const int SBARRIER = SHEIGHT - 64*ZM;

const int IGIW = 16*ZM; // standard width of an invader in grid
const int IGIH = 16*ZM; // standard height of an invader in grid
const int NILINE = 11; // number of invaders per line
const int NIROWS = 5; // number of invader rows
const int IGWIDTH = IGIW*NILINE;
const int IGHEIGHT = IGIH*NIROWS;
const int IGLEFT = (SWIDTH - IGWIDTH)/2; // Gap to left of invaders at start
const int NI = NILINE * NIROWS;

// Globally used font
std::string gInvadersFontSheetPath = "resources/textures/space_invaders_font.png";
SDL_Texture *gFontSheet;

//Starts up SDL and creates window
bool myinit();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Imge to create a sprite from
std::string gSpriteSheetPath = "resources/textures/invaders_sprite_sheet_clean.jpg";

// Game Variables
bool gQuit = false; 
bool gGameOver = false;
int gScore1 = 0;
int gScore2 = 0;
int gHiScore = 0;


// Invaders
Sprite *inv[NI];
float invVel = 4.0;
const int invSpeedMultiplier = 7;
int invAnimTime = 50+invSpeedMultiplier*NI;
int numInvaders = NI;

// Cannon
Sprite *cannon;
SDL_Rect cannonSSPos = {277, 28, 26, 16};
int cannonX = 0;
int cannonUpdateTime = 20;
int cannonSpeed = 2*ZM;
int cannonsRemaining = 2;

// Spaceship
Sprite *spaceship;
SDL_Rect spaceshipSSPos = {215, 24, 48, 22};
int spaceshipUpdateTime = 60;
bool bSpaceship = false;
int spaceshipDir = 1;
int spaceshipSpeed = 2*ZM;

// laser/bullet
Sprite *laser;
SDL_Rect laserSSPos = {8, 64, 2, 8};
bool bLaser = false;
Uint32 laserCoolTicks = 0;
bool laserCooling = false;
int laserUpdateTime = 10;
int laserSpeed = 0-1*ZM;

// invader explosion
Sprite *iexplosion;
SDL_Rect iexplosionSSPos = {437, 76, 26, 16};
Uint32 explosionTicks = 0;
Uint32 explosionTime = 200;

// invader missile
const int numMissiles = 3;
Sprite *missile[numMissiles];
SDL_Rect missileSSPos = {413, 77, 6, 12};
int missileUpdateTime = 10;
int numMissilesFired = 0;
Uint32 missleRateLo = 500; // 100 ms
Uint32 missleRateHi = 5000; // 100 ms
int missileSpeed = 1*ZM;

// Defence barriers
// 22x16 ... grow to 28x16
const int bw = 28;
const int bh = 16;
char bdata[] = {
    0,0,0,0,0,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,0,0,0,0,0, // 0
    0,0,0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,0,0, // 1
    0,0,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,0,0, // 2
    0,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,0, // 3
    1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1, // 8
    1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,0,0,0, 0,0,0,0,0,0, 0,0,0,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,1,1,1,1,1,1,1,
    1,1,1,1,1,1,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,1,1,1,1,1,1,
    1,1,1,1,1,1,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,1,1,1,1,1,1 // 16
};
const int numBarriers = 4;
SDL_Texture *barrier[numBarriers];
SDL_Rect barrierSSPos = {316, 13, 44, 32};
SDL_Surface *barrierSurface[numBarriers];
SDL_Rect barrierPos[numBarriers] = 
    {{ 23      *ZM,SBARRIER, bw*ZM,bh*ZM},
     {(23+50)  *ZM,SBARRIER, bw*ZM,bh*ZM},
     {(23+50*2)*ZM,SBARRIER, bw*ZM,bh*ZM},
     {(23+50*3)*ZM,SBARRIER, bw*ZM,bh*ZM}
    };
unsigned char *barrierPixels[numBarriers];

// Cannon explosion
Sprite *cexplosion[3];
SDL_Rect cexplosionSSPos = {367, 75, 36, 16};

SDL_Texture *sheet;

bool myinit() 
{

    //Initialise SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    else
    {
           //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SWIDTH, SHEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }
        else
        {
            printf("Window pixel format %s\n", SDL_GetPixelFormatName(SDL_GetWindowPixelFormat(gWindow)));
            //Create renderer for window
            SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE);
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                return false;
            }
            else
            {
                
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

                //Initialise PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialise! SDL_image Error: %s\n", IMG_GetError() );
                    return false;
                }

            }
        }
    }

    return true;
}

bool loadMedia()
{
    bool success = true;

    // Load the font sheet
    SDL_Surface *surface = IMG_Load(gInvadersFontSheetPath.c_str());
    if (surface) {
        printf("Loaded font sheet %dx%d format %s\n", surface->w, surface->h, SDL_GetPixelFormatName(surface->format->format));
        SDL_Surface *surface2 = SDL_ConvertSurfaceFormat(surface, SDL_GetWindowPixelFormat(gWindow), 0);
        if (surface2) {
            printf("Converted font sheet format to %s\n", SDL_GetPixelFormatName(surface2->format->format));
            gFontSheet = SDL_CreateTextureFromSurface(gRenderer, surface2);
            SDL_FreeSurface(surface2);
        }
        else {
            printf("Failed to convert font sheet format to window surface format \n");
            SDL_FreeSurface(surface);
        }
    }
    else
    {
        printf("Failed to load font sheet\n");
    }

    return success;
}

void renderText(const char *text, int X, int Y) {
    // font sheet is 2 x the basic size (chars are 16x16 instead of 8x8)
    int l = strlen(text); if (l>28) l=28;
    SDL_Rect src = {0, 0, 14, 16};
    SDL_Rect dst = {0, Y*ZM, 8*ZM, 8*ZM};
    for (int i=0; i<l; i++) {
        int index = (text[i]-32) % 32; // how many chars across the font page
        int row = (text[i]-32) / 32; // row of the font sheet
        src.x = index * 16;
        src.y = row * 32;
        dst.x = X*ZM + i*8*ZM;
        SDL_RenderCopy(gRenderer, gFontSheet, &src, &dst);
    }
}

void setupFormation() 
{
    numInvaders = NI;
    invVel = 4.0;
    invAnimTime = 50+invSpeedMultiplier*NI;

    SDL_Rect sspos1, sspos2;
    sspos1 = { 7, 25, 16, 16};
    sspos2 = {40, 25, 16, 16};
    for (int i=0;i<NILINE;i++)
    {
        inv[i] = new Sprite(&sspos1);
        inv[i]->addAnimSprite(&sspos2);
        inv[i]->setPos(IGLEFT+IGIW*i+4,STOP+SSHIP);
    }
    sspos1 = { 74, 25, 22, 16};
    sspos2 = {107, 25, 22, 16};
    for (int i=0;i<NILINE;i++)
    {
        inv[i+11] = new Sprite(&sspos1);
        inv[i+11]->addAnimSprite(&sspos2);
        inv[i+11]->setPos(IGLEFT+IGIW*i+2,STOP+SSHIP+IGIH);
        inv[i+22] = new Sprite(&sspos2);
        inv[i+22]->addAnimSprite(&sspos1);
        inv[i+22]->setPos(IGLEFT+IGIW*i+2,STOP+SSHIP+IGIH*2);
    }
    sspos1 = {147, 25, 24, 16};
    sspos2 = {179, 25, 24, 16};
    for (int i=0;i<NILINE;i++)
    {
        inv[i+33] = new Sprite(&sspos1);
        inv[i+33]->addAnimSprite(&sspos2);
        inv[i+33]->setPos(IGLEFT+IGIW*i+2,STOP+SSHIP+IGIH*3);
        inv[i+44] = new Sprite(&sspos2);
        inv[i+44]->addAnimSprite(&sspos1);
        inv[i+44]->setPos(IGLEFT+IGIW*i+2,STOP+SSHIP+IGIH*4);
    }
    for (int i=0;i<NI;i++)
    {
        inv[i]->setAnimTime(invAnimTime);
        inv[i]->setFrameTime(invAnimTime);
        inv[i]->setVel(invVel,0);
        inv[i]->setWrap(false);
        inv[i]->setSpriteZoom(ZM/2); // Sprites are already 2*size
    }
    
    cannon = new Sprite(&cannonSSPos);
    cannonX = 108*ZM;
    cannon->setPos(cannonX, 216*ZM);
    cannon->setAnimTime(0);
    cannon->setFrameTime(cannonUpdateTime);
    cannon->setVel(0,0);
    cannon->setWrap(false);
    cannon->setSpriteZoom(ZM/2);

    spaceship = new Sprite(&spaceshipSSPos);
    spaceship->setPos(0,36*ZM);
    spaceship->setAnimTime(0);
    spaceship->setFrameTime(spaceshipUpdateTime);
    spaceship->setVel(0,0);
    spaceship->setWrap(false);
    spaceship->setSpriteZoom(ZM/2);

    laser = new Sprite(&laserSSPos);
    laser->setAnimTime(0);
    laser->setFrameTime(laserUpdateTime);
    laser->setVel(0,0);
    laser->setWrap(false);
    laser->setSpriteZoom(ZM/2);

    iexplosion = new Sprite(&iexplosionSSPos);
    iexplosion->setAnimTime(0);
    iexplosion->setFrameTime(100);
    iexplosion->setVel(0,0);
    iexplosion->setWrap(false);
    iexplosion->setSpriteZoom(ZM/2);

    for (int i=0; i<numMissiles; i++) {
        missile[i] = new Sprite(&missileSSPos);
        missile[i]->setPos(0,0);
        missile[i]->setAnimTime(0);
        missile[i]->setFrameTime(missileUpdateTime);
        missile[i]->setVel(0,missileSpeed);
        missile[i]->setWrap(false);
        missile[i]->setSpriteZoom(ZM/2);
        missile[i]->dead = true;
    }
}

void setupBariers() 
{
    unsigned char *pixels;
    pixels = (unsigned char *)malloc(bw*bh*4);
    if (pixels == NULL) {
        printf("Alloc error\n");
        return;
    }
    unsigned char green[4] = { 255, 0, 255, 0 };
    unsigned char black[4] = { 255, 0, 0, 0 };
    for(int y = 0; y < bh; ++y) {
        for (int x = 0; x < bw; ++x) {
            if (bdata[y*bw+x]==0) {
                memcpy(&pixels[(y * bw + x)*4], black, 4);
            } else {
                memcpy(&pixels[(y * bw + x)*4], green, 4);
            }
        }
    }

	for (int b=0; b<numBarriers; b++) {
        barrierPixels[b] = (unsigned char *)malloc(bw*bh*4);
        memcpy(barrierPixels[b], pixels, bw*bh*4);
        if (barrierPixels[b] == NULL) {
            printf("Alloc error barrier %d\n",b);
            return;
        }
    }
    free(pixels);

	for (int b=0; b<numBarriers; b++) {
		barrier[b] = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, bw, bh);
		if (!barrier[b]) {
			printf("Failed to create texture for barrier\n");
		}
        unsigned char *texbytes = nullptr;
        int pitch = 0;
        if (SDL_LockTexture(barrier[b],  nullptr, reinterpret_cast<void**>(&texbytes), &pitch) ==0) {
            memcpy(texbytes, barrierPixels[b], bw*bh*4);
            SDL_UnlockTexture(barrier[b]);
        } else {
            printf("Error locking barrier texture %d\n",b);
            printf("Error: %s\n",SDL_GetError());
        }
        printf("Barrier %d at %d,%d, %dx%d\n", b, barrierPos[0].x, barrierPos[0].y, barrierPos[0].w, barrierPos[0].h);
    }
}

void drawBarriers() 
{
	for (int b = 0 ; b < numBarriers ; b++ ) {
		if (SDL_RenderCopy(gRenderer, barrier[b], NULL, &(barrierPos[b])) != 0) {
			printf("Error copying barrier texture to screen\n");
		}
        // SDL_SetRenderDrawColor(gRenderer, 0xFF,0x00,0x00,0xFF);
        // SDL_RenderDrawRect(gRenderer, &(barrierPos[b]));
	}
}

/* return -1 if not hit any
   or barrier number if it hits
*/
const int hsw=5; const int hs_leftoff=2;
const int hsh=6;
unsigned char hitshape[hsh][hsw] = {
    {0,0,0,0,0},
    {1,0,1,0,1},
    {0,1,1,1,0},
    {0,0,1,1,1},
    {0,1,1,1,0},
    {0,0,1,1,0},
};
bool checkHitBarrier() 
{
    int barrierHit = -1;
    bool hit = false;
    // first check bounding boxes
	for (int b = 0 ; b < numBarriers ; b++ ) {
        SDL_Rect *lp = laser->getPos();
        if (lp->x >= barrierPos[b].x &&
            lp->x <  barrierPos[b].x+barrierPos[b].w &&
            lp->y <  barrierPos[b].y+barrierPos[b].h &&
            lp->y >= barrierPos[b].y
            ) 
        {
            barrierHit = b;
            break;
        }    
    }

    if (barrierHit>=0) {
        // check pixel location at end of laser
        // laser pos in terms of zoomed screen
        int x = laser->getPos()->x;
        int y = laser->getPos()->y;
        // subtract barrier pos
        x -= barrierPos[barrierHit].x;
        y -= barrierPos[barrierHit].y;
        // divide to get non-zoomed pos relative to pixels in barrier texture
        x /= ZM; 
        y /= ZM;
        
        int newx=x;
        for (int i = x-1; i<=x+1; i++) {
            unsigned char *ptr = barrierPixels[barrierHit]+(y*bw + i)*4;
            // ptr [0]=A [1]=B [2]=G [3]=R
            if (ptr[1]==0 && ptr[2]==0xFF && ptr[3]==0){ // green = hit
                hit = true;
                //printf("Barrier %d x,y %d,%d (0x%02X 0x%02X 0x%02X 0x%02X)\n", barrierHit, x,y, ptr[0],ptr[1],ptr[2],ptr[3]);
                newx = i;
                break;    
            }
        }
        if (hit) {
            x = newx;
            for (int j=0; j<=hsh; j++) {
                for (int i=0; i<hsw; i++) {
                    int yoff = y - hsh + j + 1;
                    int xoff = x - hs_leftoff + i;

                    if(hitshape[j][i]==1 && xoff < bw && xoff >= 0 && yoff < bh && yoff >=0) {
                        int offset = (yoff*bw+xoff)*4;
                        barrierPixels[barrierHit][offset+1] = 0x00;
                        barrierPixels[barrierHit][offset+2] = 0x00;
                        barrierPixels[barrierHit][offset+3] = 0x00;
                    }
                }
            }        
            unsigned char *texbytes = nullptr;
            int pitch = 0;
            if (SDL_LockTexture(barrier[barrierHit],  nullptr, reinterpret_cast<void**>(&texbytes), &pitch) ==0) {
                memcpy(texbytes, barrierPixels[barrierHit], bw*bh*4);
                SDL_UnlockTexture(barrier[barrierHit]);
            }            
        }
    } 
    return hit;
}

Uint32 nextShip=0;
Uint32 getNextShipTime(int s)
{
    int secs;
    Uint32 ticks;
    if (s>0) {
        secs = s;
    } else {
        secs = s + rand()%25;
    } 
    ticks = SDL_GetTicks() + secs*1000;
    printf("Next ship in %ds\n",secs);
    return ticks;
}

void processEvents() 
{
    SDL_Event e;

    while( SDL_PollEvent( &e ) )
    { 
        if( e.type == SDL_QUIT )
        {
            gQuit = true; 
        } 
        else if (e.type == SDL_KEYDOWN) {
            //printf("Key 0x%X\n",e.key.keysym.sym);
            switch (e.key.keysym.sym) {
                case SDLK_q:
                {
                    gQuit = true;
                    break;
                }
                default:
                    break;
            }
        }
    } 

    // use scancodes to get keyboard state to avoid repeat delay
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        if (cannon->getX() >= cannonSpeed) {
            cannon->setVel(0-cannonSpeed, 0);
        } else {
            cannon->setVel( 0, 0);
        }
    } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        if (cannon->getX() < (SWIDTH-cannon->getW())) {
            cannon->setVel(cannonSpeed, 0);
        } else {
            cannon->setVel( 0, 0);
        }
    } else {
        cannon->setVel( 0, 0);
    }

    if (!laserCooling || SDL_TICKS_PASSED(SDL_GetTicks(),laserCoolTicks)) {
        if (currentKeyStates[SDL_SCANCODE_SPACE]) {
            if (!bLaser) {
                bLaser = true;
                laser->setPos(cannon->getX()+6*ZM, SCANNON);
                laser->setVel(0,laserSpeed);
            }
        }
        laserCooling = false;
    }

}

// Get a list of invaders with nothing below them
int invaderFireList[NILINE]; // maxium is number of columns
int numInvadersInFireList=0;
bool getInvaderFireList()
{
    numInvadersInFireList=0;
    bool ret = false;
    // go by column and find lowest invader in column, add to list
    for (int col = 0; col < NILINE; col++)
    {
        int possInv = -1;
        for (int row = 0; row < NIROWS; row++) 
        {
            int i = col + row * NILINE;
            if (inv[i] && !inv[i]->dead) {
                possInv = i;
            }
        }
        if (possInv>=0) {
            invaderFireList[col] = possInv;
            numInvadersInFireList++;
            ret = true;
        }
    }
    return ret;
}
// Fire a missile from an Invader
void fireNewMissile()
{
    if (numMissilesFired < numMissiles) {
        numMissilesFired++;
        // find an unfired missile
        int m=-1;
        for (int i=0; i<numMissiles; i++) 
        {
            if (missile[i]->dead) {
                m=i;
                break;
            }
        }
        if (m>=0) 
        {
            if (!getInvaderFireList()) {
                printf("Error - no invaders in fire list!\n");
            }
            int index = rand() % numInvadersInFireList;
            if (index>=NILINE) {
                printf("Error getting invader who fired missile\n");
                return;
            }
            int invader = invaderFireList[index];
            missile[m]->dead = false;
            int invX = inv[invader]->getX();
            int invY = inv[invader]->getY();
            int missileX = invX + 4*ZM;
            int missileY = invY + 8*ZM;
            missile[m]->setPos(missileX, missileY);
            //printf("Inv[%d] fires missile. Pos Inv = %d,%d Pos Missile = %d,%d\n", invader, invX, invY, missileX, missileY);
        }
    }
}


int main( int argc, char* args[] )
{
	bool initialised = false;
	bool medialoaded = false;

    Uint32 startTick = 0;
    Uint32 missleTimeoutTick = 0;

	srand(time(0));

   //Start up SDL and create window
    if( !myinit() )
    {
        printf( "Failed to initialise!\n" );
    }
    else
    {
    	initialised = true;
    }

	if (!loadMedia())
	{
		printf( "Failed to load media!\n" );
	}
    else
    {
    	medialoaded = true;

    }

	if (initialised && medialoaded)
    {
        startTick = SDL_GetTicks();
        missleTimeoutTick = SDL_GetTicks()+1000; // first missile will fire in 1sec

        Sprite::setRenderer(gRenderer);

        // Load a Sprite Sheet and create a sprite from it
        Sprite::loadSheet(gSpriteSheetPath);
        // {  7, 225, 16, 16} Invader1 pos1
        // { 40, 225, 16, 16} Invader1 pos2
        // { 74, 225, 22, 16} Invader2 pos1
        // {107, 225, 22, 16} Invader2 pos2
        // {147, 226, 24, 16} Invader3 pos1
        // {179, 226, 24, 16} Invader4 pos2
        sheet = Sprite::getSheet();

        while (!gQuit) {
            setupFormation();
            setupBariers();

            // set a time before the mystery ship appears
            // between 5 secs and 30 secs
            nextShip=getNextShipTime(5);
            bSpaceship = false;
            //****************************************************************
            // MAIN LOOP
            long loopCtr = 0;
            
            Uint32 ticks = startTick;
            long lastLoopCtr = 0;
            while( gQuit == false && gGameOver == false && numInvaders>0)
            { 
                loopCtr++;
                if (SDL_TICKS_PASSED(SDL_GetTicks(), ticks+1000))
                {
                    printf("1 Second %ld loops\n", loopCtr-lastLoopCtr);
                    ticks=SDL_GetTicks();
                    lastLoopCtr = loopCtr;
                }

                processEvents();

                // =======================================================================
                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                SDL_RenderClear( gRenderer );

                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );

                for (int i=0;i<ZM; i++) {
                    SDL_RenderDrawLine(gRenderer, 0, SBOT-ZM+i, SWIDTH, SBOT-ZM+i);
                }

                // Render The top-line
                renderText("SCORE<1>  HI-SCORE  SCORE<2>", 0, 0);

                char numstr[4];
                snprintf(numstr, 2, "%d", cannonsRemaining+1);
                renderText(numstr, 4, SBOT/ZM+1);
                for (int i=0;i<cannonsRemaining;i++) {
                    SDL_Rect dst = {24*ZM + 24*i*ZM, SBOT, cannon->getW(), cannon->getH()};
                    SDL_RenderCopy( gRenderer, sheet, &cannonSSPos, &dst );
                }

                char scorestr[10];
                snprintf(scorestr, 8, "%04d", gScore1);
                renderText(scorestr, 16, 16);
                snprintf(scorestr, 8, "%04d", gHiScore);
                renderText(scorestr, 96, 16);
                snprintf(scorestr, 8, "%04d", gScore2);
                renderText(scorestr, 176, 16);

                drawBarriers();

                // Draw cannon
                cannon->update();
                cannon->draw();

                // decide on spaceship appearing
                if (!bSpaceship) {
                    if (SDL_TICKS_PASSED(SDL_GetTicks(), nextShip)) {
                        bSpaceship = true;
                        spaceshipDir = ((rand() % 2)*2) - 1; // 1 or -1
                        spaceship->setVel(spaceshipDir * spaceshipSpeed, 0);
                        if (spaceshipDir == 1) {
                            spaceship->setPos(0, 36*ZM);
                        } else if (spaceshipDir == -1) {
                            spaceship->setPos(SWIDTH, 36*ZM);
                        } else {
                            printf("error\n");
                        }
                    }
                }
                // draw and update spaceship if it has appeared
                if (bSpaceship) {
                    spaceship->update();
                    spaceship->draw();
                    if (spaceship->getX() > SWIDTH || spaceship->getX() < (0-spaceship->getW())) {
                        bSpaceship = false;
                        nextShip=getNextShipTime(0);
                    }
                }

                // Decide on Missile fire
                {

                    if (SDL_TICKS_PASSED(SDL_GetTicks(), missleTimeoutTick)) {
                        if (numMissilesFired < numMissiles) {
                            fireNewMissile();
                            missleTimeoutTick += (rand() % (missleRateHi - missleRateLo)) + missleRateLo;
                        }

                    }
                }

                // Misile update if fired
                if (numMissilesFired>0) {
                    for (int m=0; m<numMissiles; m++) {
                        if (!missile[m]->dead) {
                            missile[m]->update();
                            if (missile[m]->getY()>SBOT) {
                                missile[m]->dead = true;
                                numMissilesFired--;
                            }
                            if (!missile[m]->dead) { 
                                missile[m]->draw();
                            }
                        }
                    }
                }

                // laser/bullet update if fired
                if (bLaser) {
                    laser->draw();
                    laser->update();
                    int lx = laser->getX();
                    int ly = laser->getY();
                    // check for invaders hit
                    for (int i=0; i<NI; i++) {
                        if (inv[i]->dead) continue;
                        SDL_Rect *ipos = inv[i]->getPos();
                        if (lx >= ipos->x && lx <= (ipos->x + ipos->w) && ly >= ipos->y && ly <= ipos->y + ipos->h ) {
                            // hit
                            //printf("Hit Inv %d at %d,%d - laser at %d,%d\n", i, ipos->x, ipos->y, lx, ly);
                            invVel = inv[i]->getVX();
                            inv[i]->dead = true;
                            numInvaders--;
                            invAnimTime -= invSpeedMultiplier;
                            if (invVel>0) {
                                invVel+=0.1;
                            } else {
                                invVel-=0.1;
                            }
                            
                            for (int v=0; v<NI; v++) {
                                if (inv[v] && !inv[v]->dead) {
                                    inv[v]->setAnimTime(invAnimTime);
                                    inv[v]->setFrameTime(invAnimTime);
                                    inv[v]->setVel(invVel,0);
                                }
                            }
                            bLaser = false;
                            gScore1 += 7; if (i<11) gScore1 += 8;

                            iexplosion->setPos(ipos->x, ipos->y);
                            explosionTicks = SDL_GetTicks() + explosionTime;
                            laserCoolTicks = SDL_GetTicks() + explosionTime;
                            laserCooling = true;
                            break;
                        }
                    }
                    // spaceship
                    if (bSpaceship) {
                        SDL_Rect *sspos = spaceship->getPos();
                        if (lx >= sspos->x && lx <= (sspos->x + sspos->w) && ly >= sspos->y && ly <= sspos->y + sspos->h ) {
                            // hit
                            //printf("Hit SS at %d,%d - laser at %d,%d\n", sspos->x, sspos->y, lx, ly);
                            bSpaceship = false;
                            nextShip=getNextShipTime(0);
                            gScore1 += 100;
                            iexplosion->setPos(lx-6*ZM, ly-8*ZM);
                            explosionTicks = SDL_GetTicks() + explosionTime;
                            laserCoolTicks = SDL_GetTicks() + explosionTime;
                            laserCooling = true;
                        }
                    }

                    if (checkHitBarrier()) {
                        //printf("Hit barrier\n");
                        bLaser=false;
                        laserCoolTicks = SDL_GetTicks() + 200;
                        laserCooling = true;
                    }

                    if (laser->getY()<32*ZM) {
                        bLaser = false;
                    }
                } // if laser fired

                // Move Invaders
                bool revdir = false;
                bool hitbottom = false;
                for (int i=0;i<NI;i++) {
                    if (inv[i] && !inv[i]->dead) {
                        if (invVel>0.0 && inv[i]->getX() >= (SWIDTH - inv[i]->getW())) {
                            revdir = true;
                            break;
                        }
                        if (invVel<0.0 && inv[i]->getX() <= (int)ceil(invVel)) {
                            revdir = true;
                            break;
                        }
                    }
                }
                // Hit wall and reversed direction
                if (revdir) {
                    for (int i=0;i<NI;i++) {
                        if (inv[i] && !inv[i]->dead) {
                            inv[i]->revVX();
                            inv[i]->incY(16);
                            if (inv[i]->getY() >= SCANNON) {
                                hitbottom = true;
                                printf("Invader %d at %d hit bottom (%d)\n",i, inv[i]->getY(), SCANNON);
                            }
                        }
                    }
                }

                // Update and draw invaders
                for (int i=0;i<NI;i++) {
                    if (inv[i] && !inv[i]->dead) {
                        inv[i]->update();
                        inv[i]->draw();
                    }
                }

                if (explosionTicks>0) {
                    iexplosion->draw();
                    if (SDL_TICKS_PASSED(SDL_GetTicks(),explosionTicks)) {
                        explosionTicks = 0;
                    }
                }

                if (numInvaders == 0) {
                    printf("Next wave ...\n");

                }
                if (hitbottom)
                {
                    gGameOver = true;
                }

            //    char fpsstr[20];
            //    sprintf(fpsstr, "%4.2f", cannon->fps);
            //    renderText(fpsstr, 180,SCANNON/ZM+8);

                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }

        renderText("GAME OVER", 70, 120);
    }

    // CLOSE

	if (medialoaded) {
        SDL_DestroyTexture(gFontSheet);
	    // delete sprites
	    for (int i=0;i<55;i++) {
	        if (inv[i]) inv[i]->destroy();
	    }
	}

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
    return 0;
}

