all: cli con

cli:
	cd client
	$(MAKE) html

con:
	make -C console/
