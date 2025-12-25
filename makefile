CC = gcc
CFLAGS = -Wall -Iinclude -O2 -g
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
SOURCES = src/main.c src/isoGraphic.c src/isoWindow.c src/control.c src/player.c src/world.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = game

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

rebuild: clean all
