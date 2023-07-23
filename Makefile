all: invaders

.PHONY: invaders
invaders:
	$(MAKE) -C invaders
	
clean:
	$(MAKE) -C invaders clean