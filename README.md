# sdl
SDL coding

## Sprite Class
- /mnt/c/sdl/sdl_sprite/Sprite.h Sprite.cpp
- Supports 
	- animations
	- textured sprites
	- shape sprites (square) - circle not working currently
	- gravity
	- fade, lifetime
	- sprite sheets

## Space Invaders
Last project was Space Invaders in SDL2 with sound

Uses my Sprite class sdl_sprite
### To make/run
* Open WSL terminal 
```
cd /mnt/c/src/sdl 
make invaders
./invaders/invaders
```

Nearly complete: Has 
* Firing and invader kills and Spaceship
*  Correct looking barriers x 4
* Invader firing different bullets with collision detection
* Correct cannon death animation
* Sounds: Invader moving, Cannon death, Fire an kill, Spaceship

### TODO
- Game over state 
- Opening Screens
- High Score
- Barrier collision is still bad
- Invaders hitting bottom of screen is not tested


