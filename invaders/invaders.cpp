#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "../sdl_sprite/Sprite.h"

const int ZM = 2; // Zoom

//Screen dimension constants
const int SWIDTH  = 224*ZM;
const int SHEIGHT = 256*ZM;
const int STOP = 32*ZM; // 2 lines of score and text at top of screen
const int SSHIP = 32*ZM; // Height of Spaceship row
const int SBOT = 16*ZM; // Line of text/info at bottom
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
std::string gSpriteSheetPath = "resources/textures/sprite_sheet_space_invaders.jpg";

//Rendered texture
SDL_Texture* gTextTexture;
int gTextTextureWidth, gTextTextureHeight;

// Invaders
Sprite *inv[NI];

int invVel = 4;
int invAnimTime = 200;
int invUpdateTime = 200;


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
    gFont = TTF_OpenFont(gInvadersFontPath.c_str(), 16 );
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

int main( int argc, char* args[] )
{
	bool initialised = false;
	bool medialoaded = false;
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

        SDL_Rect sspos1, sspos2;
        sspos1 = { 7, 225, 16, 16};
        sspos2 = {40, 225, 16, 16};
        for (int i=0;i<NILINE;i++)
        {
            inv[i] = new Sprite(&sspos1);
            inv[i]->addAnimSprite(&sspos2);
            inv[i]->setPos(IGLEFT+IGIW*i+4,STOP+SSHIP);
        }
        sspos1 = { 74, 225, 22, 16};
        sspos2 = {107, 225, 22, 16};
        for (int i=0;i<NILINE;i++)
        {
            inv[i+11] = new Sprite(&sspos1);
            inv[i+11]->addAnimSprite(&sspos2);
            inv[i+11]->setPos(IGLEFT+IGIW*i+2,STOP+SSHIP+IGIH);
            inv[i+22] = new Sprite(&sspos1);
            inv[i+22]->addAnimSprite(&sspos2);
            inv[i+22]->setPos(IGLEFT+IGIW*i+2,STOP+SSHIP+IGIH*2);
        }
        sspos1 = {147, 225, 24, 16};
        sspos2 = {179, 225, 24, 16};
        for (int i=0;i<NILINE;i++)
        {
            inv[i+33] = new Sprite(&sspos1);
            inv[i+33]->addAnimSprite(&sspos2);
            inv[i+33]->setPos(IGLEFT+IGIW*i+2,STOP+SSHIP+IGIH*3);
            inv[i+44] = new Sprite(&sspos1);
            inv[i+44]->addAnimSprite(&sspos2);
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

        while( quit == false )
        { 
            while( SDL_PollEvent( &e ) )
            { 
                if( e.type == SDL_QUIT )
                {
                    quit = true; 
                } 
                else if (e.type == SDL_KEYDOWN) 
                {
                    //printf("Key 0x%X\n",e.key.keysym.sym);
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_UP:
                        break;
                        case SDLK_DOWN:
                        break;
                        case SDLK_LEFT:
                        break;
                        case SDLK_RIGHT:
                        break;
                        case SDLK_q:
                        quit = true;
                        break;
                        default:
                        break;
                    }
                }

            } 

            //Initialize renderer color
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

            //Clear screen
            SDL_RenderClear( gRenderer );

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
                        if (inv[i]->getY() <= SCANNON) {
                        	hitbottom = true;
                        }
                    }
                }
            }
            for (int i=0;i<NI;i++) {
                if (inv[i]) {
                    inv[i]->update();
                    inv[i]->draw();
                }
            }

            if (hitbottom)
            {
            	printf("End\n");
            }

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

