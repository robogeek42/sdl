#include "MySprite.h"

MySprite::MySprite() {
    sprite = NULL;
    loaded = false;
    width = 0;
    height = 0;
    speed = 0;
    run = false;
    pos = {0,0,0,0};
}

MySprite::~MySprite() {
    destroy();
}
MySprite* MySprite::destroy() {
    if (sprite) {
        SDL_DestroyTexture(sprite);
    }
    return this;
}

MySprite::MySprite(SDL_Renderer *r, std::string path) {
    this->renderer = r;
    sprite = loadTexture(path);
    if (this->sprite) this->loaded = true;
}

//Loads individual image as texture
SDL_Texture* MySprite::loadTexture( std::string path )
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
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );

        if (SDL_QueryTexture(newTexture,NULL, NULL, &width, &height)!=0) {
            printf("Unable to get Texture Size info\n");
        }
        printf("Sprite loaded %dx%d\n", width, height);
        pos.w = width;
        pos.h = height;
	}

	return newTexture;
}

bool MySprite::draw() {
    if (!loaded) {
        printf("No sprite?\n");
        return false;
    }
    //Render texture to screen
    SDL_RenderCopy( renderer, sprite, NULL, &pos );
    return true;
}

void MySprite::setPos(int x, int y) {
    pos.x = x;
    pos.y = y;
}
