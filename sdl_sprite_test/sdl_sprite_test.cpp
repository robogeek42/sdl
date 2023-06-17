#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "MySprite.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Imge to create a sprite from
std::string gImagePath = "../resources/textures/UFO_1_Enlarged.png";


//Rendered texture
SDL_Texture* gTextTexture;
int gTextTextureWidth, gTextTextureHeight;

MySprite *s1, *s2;
#define NUM_PARTICLES 30
MySprite *particles[NUM_PARTICLES];

void addParticle(int px, int py, int vx, int vy);
void updateAndDrawParticles();

bool init() 
{

    //Initialize SDL
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
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0x40, 0x00, 0x40, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					return false;
				}
			}
        }
    }

    return true;
}



int main( int argc, char* args[] )
{
   //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        SDL_Event e; 
        bool quit = false; 
        s1 = new MySprite(gRenderer, gImagePath, 120);
        s1->setPos(100,100);
        s1->setVel(2,0);
        s1->setWrap(true);
        SDL_Color boxcol = {255, 50, 50, 255};
        s2 = new MySprite(gRenderer, 75, 75, &boxcol, 30);
        s2->setBorderColor(255,255,100);
        s2->setPos(200,200);
        s2->setVel(-3,2);
        s2->setWrap(true);

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
            SDL_SetRenderDrawColor( gRenderer, 0x40, 0x00, 0x40, 0xFF );

            //Clear screen
            SDL_RenderClear( gRenderer );

            s1->update();
            s1->draw();
            s2->update();
            s2->draw();

            int mouseX, mouseY; Uint32 mouseButtonState;
            mouseButtonState = SDL_GetMouseState(&mouseX, &mouseY);
            if (SDL_BUTTON(SDL_BUTTON_LEFT) & mouseButtonState) {
                addParticle(mouseX, mouseY, rand()%10-5, rand()%10-5);
            }
            updateAndDrawParticles();

            //Update screen
            SDL_RenderPresent( gRenderer );
        }
    }
    close();

    return 0;
}

void close() 
{
    // delete sprite
    s1->destroy();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void addParticle(int px, int py, int vx, int vy) {
    for(int i=0; i<NUM_PARTICLES; i++) {
        if (particles[i] == NULL) {
            particles[i] = new MySprite(gRenderer, 20, 20, rand()%255, rand()%255, rand()%255, 255, 30);
            particles[i]->setPos(px, py);
            particles[i]->setVel(vx, vy);
            particles[i]->setLifetime(2000);
            break;
        }
    }
}

void updateAndDrawParticles() {
    for(int i=0; i<NUM_PARTICLES; i++) {
        if (!particles[i]) continue;
        if (!particles[i]->update()) {
            delete particles[i]; particles[i] = NULL;
        } else {
            particles[i]->draw();
        }
    }
}