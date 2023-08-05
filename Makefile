all: invaders

.PHONY: invaders
invaders:
	$(MAKE) -C invaders
	
render_text:
	$(MAKE) -C render_text

clean:
	$(MAKE) -C invaders clean
