#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SWIDTH = 1280;
const int SHEIGHT = 720;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Imge to draw
SDL_Texture* gShip = NULL;
std::string gImagePath = "UFO_1_Enlarged.png";
int gShipWidth, gShipHeight;

//Globally used font
TTF_Font* gFont = NULL;

//Rendered texture
SDL_Texture* gTextTexture;
int gTextTextureWidth, gTextTextureHeight;

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
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0x40, 0x00, 0x40, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					return false;
				}

                //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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

	//Load PNG texture
	gShip = loadTexture( gImagePath );
	if( gShip == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

    //Open the font
    // gFont = TTF_OpenFont( "lazy.ttf",56 );
    // gFont = TTF_OpenFont( "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 56 );
    gFont = TTF_OpenFont( "/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf", 56 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    return success;
}


SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );

        if (SDL_QueryTexture(newTexture,NULL, NULL, &gShipWidth, &gShipHeight)!=0) {
            printf("Unable to get Texture Size info\n");
        }
	}

	return newTexture;
}

bool loadFromRenderedText( std::string textureText, SDL_Color textColor )
{

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        gTextTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( gTextTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            gTextTextureWidth = textSurface->w;
            gTextTextureHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
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
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            SDL_Event e; 
            bool quit = false; 
            SDL_Rect shipDest = {0,0,gShipWidth/2,gShipHeight/2};
            SDL_Rect fillRect = {200,200,100,100};

            SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
            loadFromRenderedText("Space Invaders", textColor);
            SDL_Rect textRect = {0,600,gTextTextureWidth, gTextTextureHeight};
        

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
                            if (fillRect.y>0) fillRect.y -= 4;
                            break;
                            case SDLK_DOWN:
                            if (fillRect.y<SHEIGHT - gShipHeight/2) fillRect.y += 4;
                            break;
                            case SDLK_LEFT:
                            if (fillRect.x>0) fillRect.x -= 4;
                            break;
                            case SDLK_RIGHT:
                            if (fillRect.x<SWIDTH - gShipWidth/2) fillRect.x += 4;
                            break;
                            default:
                            break;
                        }
                    }

                } 
                shipDest.x += 1;
                if (shipDest.x>SWIDTH) shipDest.x=0;

				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0x40, 0x00, 0x40, 0xFF );

                //Clear screen
                SDL_RenderClear( gRenderer );

                //Render red filled quad
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );        
                SDL_RenderFillRect( gRenderer, &fillRect );

                //Render texture to screen
				SDL_SetRenderDrawColor( gRenderer, 0x40, 0x00, 0x40, 0xFF );
                SDL_RenderCopy( gRenderer, gShip, NULL, &shipDest );

                // Render Text 
                SDL_RenderCopy( gRenderer, gTextTexture, NULL, &textRect );

                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );  
                for (int i=0;i<4;i++)
                {
                    SDL_RenderDrawLine( gRenderer, 0,180+i,SWIDTH,180+i);
                }

                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    close();

    return 0;
}

void close() 
{
    //Deallocate surface
    SDL_DestroyTexture( gShip );
    gShip = NULL;
    TTF_CloseFont( gFont );
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}