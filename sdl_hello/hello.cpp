#include <SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// Imge to draw
SDL_Surface* gShip = NULL;
char gImagePath[] = "UFO_1_Enlarged.bmp";


void close();
bool loadMedia()
{
    bool success = true;

    SDL_Surface *gShip_unopt = SDL_LoadBMP( gImagePath );
    if( gShip_unopt == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", gImagePath, SDL_GetError() );
        success = false;
    }
    gShip = SDL_ConvertSurface(gShip_unopt, gScreenSurface->format, 0);
    SDL_FreeSurface(gShip_unopt);

    return success;
}

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
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );

            //Fill the surface grey
            SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x00, 0x00, 0x00 ) );
            
        }
    }

    return true;
}

int main( int argc, char* args[] )
{
    SDL_Event e; 
    bool quit = false; 
    SDL_Rect dest = {0,0,0,0};

   //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {

            //Apply the image
            SDL_BlitSurface( gShip, NULL, gScreenSurface, &dest );

            //Update the surface
            SDL_UpdateWindowSurface( gWindow );

            //Hack to get window to stay up
            while( quit == false )
            { 
                while( SDL_PollEvent( &e ) )
                { 
                    if( e.type == SDL_QUIT )
                    {
                        quit = true; 
                    } else if (e.type == SDL_KEYDOWN) 
                    {
                        //printf("Key 0x%X\n",e.key.keysym.sym);
                        switch (e.key.keysym.sym)
                        {
                            case SDLK_UP:
                            if (dest.y>0) dest.y -= 4;
                            break;
                            case SDLK_DOWN:
                            if (dest.y<SCREEN_HEIGHT - gShip->h) dest.y += 4;
                            break;
                            case SDLK_LEFT:
                            if (dest.x>0) dest.x -= 4;
                            break;
                            case SDLK_RIGHT:
                            if (dest.x<SCREEN_WIDTH - gShip->w) dest.x += 4;
                            break;
                            default:
                            break;
                        }
                        SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x00, 0x00, 0x00 ) );

                        //Apply the image
                        SDL_BlitSurface( gShip, NULL, gScreenSurface, &dest );

                        //Update the surface
                        SDL_UpdateWindowSurface( gWindow );


                    }
                } 
            }
        }
    }
    close();

    return 0;
}

void close() 
{
    printf("Free Ship\n"); fflush(stdout);
    //Deallocate surface
    SDL_FreeSurface( gShip );
    gShip = NULL;

    printf("Destroy window\n"); fflush(stdout);
    //Destroy window
    SDL_DestroyWindow( gWindow );

    printf("SQL_Quit\n"); fflush(stdout);
    //Quit SDL subsystems
    SDL_Quit();

    printf("Done\n"); fflush(stdout);
}
