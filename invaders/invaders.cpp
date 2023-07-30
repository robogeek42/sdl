#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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

const int IGIW = 16*ZM; // standard width of an invader in grid
const int IGIH = 16*ZM; // standard height of an invader in grid
const int NILINE = 11; // number of invaders per line
const int NIROWS = 5; // number of invader rows
const int IGWIDTH = IGIW*NILINE;
const int IGHEIGHT = IGIH*NIROWS;
const int IGLEFT = (SWIDTH - IGWIDTH)/2; // Gap to left of invaders at start
const int NI = NILINE * NIROWS;

// Globally used font
TTF_Font* gFont = NULL;
std::string gInvadersFontPath = "resources/fonts/space_invaders.ttf";


//Starts up SDL and creates window
bool myinit();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Imge to create a sprite from
std::string gSpriteSheetPath = "resources/textures/invaders_sprite_sheet_clean.jpg";

//Rendered texture
SDL_Texture* gNumbersTexture[10];
int gNumbersTextureWidth, gNumbersTextureHeight;

// Invaders
Sprite *inv[NI];
int invVel = 4;
int invAnimTime = 200;
int invUpdateTime = 200;

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
int laserUpdateTime = 2;
int laserSpeed = 0-1*ZM;

// invader explosion
Sprite *iexplosion;
SDL_Rect iexplosionSSPos = {437, 76, 26, 16};
int countIExplosion = 0;

// invader missile
Sprite *missile[3];
SDL_Rect missileSSPos = {413, 77, 6, 12};


// Defence
Sprite *defence[3];
SDL_Rect defenceSSPos = {316, 13, 44, 32};

// Cannon explosion
Sprite *cexplosion[3];
SDL_Rect cexplosionSSPos = {367, 75, 36, 16};

SDL_Texture *sheet;

int gScore1 = 0;
int gScore2 = 0;

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
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
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

                // Initialise SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialise! SDL_ttf Error: %s\n", TTF_GetError() );
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

	// Load background

    // Open the font
    gFont = TTF_OpenFont(gInvadersFontPath.c_str(), 8*ZM );
    if( gFont == NULL )
    {
        printf( "Failed to load invaders font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else {
    	printf("Font loaded\n");
    }

    return success;
}

SDL_Texture* loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	SDL_Texture *textTexture;
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
    	textTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( textTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
//            gTextTextureWidth = textSurface->w;
//            gTextTextureHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return texture
    return textTexture;
}

void setupNumberTexure() {
    SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
    const char *nums = "0123456789.";
    for (int i=0; i<11; i++) {
    	gNumbersTexture[i] = loadFromRenderedText(&nums[i], textColor);
    }
}

void drawNumber(const char* numstr, int x, int y, int cw, int ch) {
	SDL_Rect srcRect = {0, 0, 6*ZM, 8*ZM};
	SDL_Rect dstRect = {x, y, cw, ch};

	int n = 0;
	int shift=0;
	while (numstr[n] != 0) {
		dstRect.x += cw;
		if (numstr[n] == 46) {
			srcRect.w = 2*ZM;
			dstRect.w = cw-4*ZM;
			dstRect.x += shift;
			shift =0;
			SDL_RenderCopy( gRenderer, gNumbersTexture[10], &srcRect, &dstRect );
		} else {
			if (numstr[n] == 49) {
				srcRect.w = 4*ZM;
				dstRect.w = cw-2*ZM;
				dstRect.x += 2*ZM;
				shift = -2*ZM;
			} else {
				srcRect.w = 6*ZM;
				dstRect.w = cw;
				dstRect.x += shift;
				shift =0;
			}
			SDL_RenderCopy( gRenderer, gNumbersTexture[numstr[n]-48], &srcRect, &dstRect );
		}
		n++;
	}
}

int main( int argc, char* args[] )
{
	bool initialised = false;
	bool medialoaded = false;

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
        SDL_Event e; 
        bool quit = false; 

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
            inv[i]->setFrameTime(invUpdateTime);
            inv[i]->setVel(invVel,0);
            inv[i]->setWrap(false);
            inv[i]->setSpriteZoom(ZM/2); // Sprites are already 2*size
        }

        SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
        SDL_Texture *textTexture = loadFromRenderedText("SCORE<1>  HI-SCORE  SCORE<2>", textColor);
        SDL_Rect textRect = {0,0,SWIDTH, 10*ZM};
        setupNumberTexure();

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


        //****************************************************************
        // MAIN LOOP

        while( quit == false )
        { 
            while( SDL_PollEvent( &e ) )
            { 
                if( e.type == SDL_QUIT )
                {
                    quit = true; 
                } 
                else if (e.type == SDL_KEYDOWN) {
                    //printf("Key 0x%X\n",e.key.keysym.sym);
                    switch (e.key.keysym.sym) {
                        case SDLK_q:
                        {
                        	quit = true;
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

            if (currentKeyStates[SDL_SCANCODE_SPACE]) {
            	if (!bLaser) {
            		bLaser = true;
            		laser->setPos(cannon->getX()+6*ZM, SCANNON);
            		laser->setVel(0,laserSpeed);
            	}
            }

            // =======================================================================
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderClear( gRenderer );

            SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );

            for (int i=0;i<ZM; i++) {
            	SDL_RenderDrawLine(gRenderer, 0, SBOT-ZM+i, SWIDTH, SBOT-ZM+i);
            }

            // Render Text
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderCopy( gRenderer, textTexture, NULL, &textRect );

            char numstr[4];
            snprintf(numstr, 2, "%d", cannonsRemaining+1);
            drawNumber(numstr, 1*ZM, SBOT+ZM, 6*ZM, 6*ZM);
            for (int i=0;i<cannonsRemaining;i++) {
            	SDL_Rect dst = {16*ZM + 16*i*ZM, SBOT, cannon->getW(), cannon->getH()};
                SDL_RenderCopy( gRenderer, sheet, &cannonSSPos, &dst );
            }

            char scorestr[10];
            snprintf(scorestr, 8, "%04d", gScore1);
            drawNumber(scorestr, 8*ZM,16*ZM, 8*ZM, 8*ZM);
            snprintf(scorestr, 8, "%04d", gScore2);
            drawNumber(scorestr, 112*ZM,16*ZM, 8*ZM, 8*ZM);

            // Draw cannon
            cannon->update();
            cannon->draw();

            // decide on spaceship appearing
            if (!bSpaceship) {
            	if ((rand() % 1000) == 0) {
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
            // spaceship
            if (bSpaceship) {
				spaceship->update();
				spaceship->draw();
	            if (spaceship->getX() > SWIDTH || spaceship->getX() < (0-spaceship->getW())) bSpaceship = false;
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
						printf("Hit Inv %d at %d,%d - laser at %d,%d\n", i, ipos->x, ipos->y, lx, ly);
						inv[i]->dead = true;
						bLaser = false;
						gScore1 += 7; if (i<11) gScore1 += 8;

						iexplosion->setPos(ipos->x, ipos->y);
						countIExplosion = 10;
						break;
					}
				}
				// spaceship
				if (bSpaceship) {
					SDL_Rect *sspos = spaceship->getPos();
					if (lx >= sspos->x && lx <= (sspos->x + sspos->w) && ly >= sspos->y && ly <= sspos->y + sspos->h ) {
						// hit
						printf("Hit SS at %d,%d - laser at %d,%d\n", sspos->x, sspos->y, lx, ly);
						bSpaceship = false;
						gScore1 += 100;

						iexplosion->setPos(lx-6*ZM, ly-8*ZM);
						countIExplosion = 10;
					}
				}

				if (laser->getY()<32*ZM) {
					bLaser = false;
				}
            }


            // Move Invaders
            bool revdir = false;
            bool hitbottom = false;
            for (int i=0;i<NI;i++) {
                if (inv[i] && inv[i]->getVX()>0 && inv[i]->getX() >= (SWIDTH - inv[i]->getW())) {
                    revdir = true;
                    break;
                }
                if (inv[i] && inv[i]->getVX()<0 && inv[i]->getX() <= invVel) {
                    revdir = true;
                    break;
                }
            }
            if (revdir) {
                for (int i=0;i<NI;i++) {
                    if (inv[i]) {
                        inv[i]->revVX();
                        inv[i]->incY(16);
                        if (inv[i]->getY() >= SCANNON) {
                        	hitbottom = true;
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
            if (countIExplosion) {
            	iexplosion->draw();
            	countIExplosion--;
            }
            if (hitbottom)
            {
            	printf("End\n");
            }

//            char fpsstr[20];
//            sprintf(fpsstr, "%4.2f", cannon->fps);
//            drawNumber(fpsstr, 8*ZM,SCANNON+16*ZM, 8*ZM, 8*ZM);

            //Update screen
            SDL_RenderPresent( gRenderer );
        }
    }

    // CLOSE

	if (medialoaded) {
	    TTF_CloseFont( gFont );
	    gFont = NULL;

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

