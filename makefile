CC=gcc
CFLAGS=-c -Wall -g

all: project

project: ParticleLife.o
	$(CC) ParticleLife.o -o ParticleLife -I include -L lib -l SDL2-2.0.0

ParticleLife.o: ParticleLife.c
	$(CC) $(CFLAGS) ParticleLife.c -I include -L lib -l SDL2-2.0.0

clean:
	/bin/rm -f ParticleLife *.o *.gz

run: 
	./ParticleLife