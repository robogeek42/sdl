all: invaders

.PHONY: invaders
invaders:
	$(MAKE) -C invaders

render_text:
	$(MAKE) -C render_text

sprite_test2:
	$(MAKE) -C sdl_sprite_test

clean:
	$(MAKE) -C invaders clean

distclean:
	$(MAKE) -C invaders distclean
