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

bool loadFont(std::string path);

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

				loadFont(gInvadersFontPath);
			}
		}
	}

	return true;
}

bool loadFont(std::string path) {
	// Open the font
	gFont = TTF_OpenFont(path.c_str(), 16 );
	if( gFont == NULL )
	{
		printf( "Failed to load invaders font! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}
	else {
		printf("Font loaded\n");
	}
	return true;
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
		//printf("create texture from text surface\n");
		gTextTexture = SDL_CreateTextureFromSurface( gRenderer, gTextSurface );
		if( gTextTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//printf("OK\n");
			//Get image dimensions
			gTextureWidth = gTextSurface->w;
			gTextureHeight = gTextSurface->h;
			printf("Image size %dx%d\n", gTextureWidth, gTextureHeight);
		}

	}

	SDL_FreeSurface( gTextSurface );

	return success;
}

void renderChars(const char* chars, int XW, int Y, SDL_Color textColor) {
	printf("Render : %s\n", chars);
	int l = strlen(chars);
	for (int i = 0; i < l; i++ ) {
		char todraw[3]; 
		todraw[0] = chars[i];
		todraw[1] = 32;
		todraw[2] = 0;
		loadFromRenderedText(todraw, textColor);
		// centre it in the XW space
		SDL_Rect textRect = {i*16+(XW-gTextureWidth)/2,Y, gTextureWidth, gTextureHeight};
		SDL_RenderCopy( gRenderer, gTextTexture, NULL, &textRect );
	}
}

int main( int argc, char* args[] )
{
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

		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
		SDL_RenderClear( gRenderer );

		// Render Text
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

		renderChars(" !\"#$%&'()*+,-./0123456789:;<=>?", 16, 0, textColor);
		renderChars("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_", 16, 32, textColor);
		renderChars("`abcdefghijklmnopqrstuvwxyz{|}~", 16, 64, textColor);

		loadFromRenderedText("SCORE<1>  HI-SCORE  SCORE<2>", textColor);
		SDL_Rect textRect = {0,96, gTextureWidth, gTextureHeight};
		SDL_RenderCopy( gRenderer, gTextTexture, NULL, &textRect );

		loadFromRenderedText("SPACE INVADERS", textColor);
		textRect = {0,128, gTextureWidth, gTextureHeight};
		SDL_RenderCopy( gRenderer, gTextTexture, NULL, &textRect );

		loadFromRenderedText("GAME OVER", textColor);
		textRect = {0,160, gTextureWidth, gTextureHeight};
		SDL_RenderCopy( gRenderer, gTextTexture, NULL, &textRect );

		loadFromRenderedText("PLAYER ", textColor);
		textRect = {0,196, gTextureWidth, gTextureHeight};
		SDL_RenderCopy( gRenderer, gTextTexture, NULL, &textRect );

		SDL_RenderPresent( gRenderer );

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
		

		SDL_Surface *sshot = SDL_CreateRGBSurface(0, SWIDTH, SHEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000);
		if (sshot) {
			SDL_RenderReadPixels(gRenderer, NULL, 0, sshot->pixels, sshot->pitch);
			IMG_SavePNG(sshot, "screenshot.png");
			SDL_FreeSurface(sshot);
		} else {
			printf("Failed to create surface for sshot\n");
		}
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
