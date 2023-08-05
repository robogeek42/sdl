#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

const int SWIDTH = 512;
const int SHEIGHT = 512;

// Globally used font
TTF_Font* gFont = NULL;
std::string gInvadersFontPath = "../resources/fonts/space_invaders.ttf";

//Starts up SDL and creates window
bool myinit();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Image to save
std::string gOutputImagePathPNG = "rendered_text.png";
std::string gOutputImagePathBMP = "rendered_text.bmp";

//Rendered texture
SDL_Texture* gTextTexture;
SDL_Surface* gTextSurface;
int gTextureWidth, gTextureHeight;

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
		printf("Initialised\n");
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		printf("Create window\n");
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SWIDTH, SHEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			return false;
		}
		else
		{
			//Create renderer for window
			printf("Create renderer\n");
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

				printf("SDL_image and SDL_ttf initialised\n");

			}
		}
	}

	return true;
}

bool loadFont() {
	// Open the font
	gFont = TTF_OpenFont(gInvadersFontPath.c_str(), 16 );
	if( gFont == NULL )
	{
		printf( "Failed to load invaders font! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else {
		printf("Font loaded\n");
	}

}

bool loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	bool success = true;
	//Render text surface
	gTextSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( gTextSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
		printf("create texture from text surface\n");
		gTextTexture = SDL_CreateTextureFromSurface( gRenderer, gTextSurface );
		if( gTextTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			printf("OK\n");
			//Get image dimensions
			gTextureWidth = gTextSurface->w;
			gTextureHeight = gTextSurface->h;
			printf("Image size %dx%d\n", gTextureWidth, gTextureHeight);
		}

	}

	SDL_FreeSurface( gTextSurface );

	return success;
}

int main( int argc, char* args[] )
{
	printf("Hello\n");

	//Start up SDL and create window
	if( !myinit() )
	{
		printf( "Failed to initialise!\n" );
	}
	else
	{
		SDL_Event e;
		bool quit = false;

		SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
		if (!loadFromRenderedText("SCORE<1>  HI-SCORE  SCORE<2>", textColor))
		{
			printf("Failed to render text\n");
		}
		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderClear( gRenderer );

		SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );

		// Render Text
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_Rect textRect = {0,0, gTextureWidth, gTextureHeight};
		SDL_RenderCopy( gRenderer, gTextTexture, NULL, &textRect );

		SDL_RenderPresent( gRenderer );

		/*
		while( quit == false ) { 
			while( SDL_PollEvent( &e ) ) { 
				if( e.type == SDL_QUIT ) { quit = true; } 
				else if (e.type == SDL_KEYDOWN) {
					//printf("Key 0x%X\n",e.key.keysym.sym);
					switch (e.key.keysym.sym) {
						case SDLK_q: {
							quit = true;
							break;
						}
						default:
							break;
					}
				}
			} 
		}
		*/

		SDL_Surface *sshot = SDL_CreateRGBSurface(0, SWIDTH, SHEIGHT, 24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_RGB888, sshot->pixels, sshot->pitch);
		SDL_SaveBMP(sshot, "screenshot.bmp");
		SDL_FreeSurface(sshot);

	}


	// CLOSE
	TTF_CloseFont( gFont );
	gFont = NULL;
	
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
