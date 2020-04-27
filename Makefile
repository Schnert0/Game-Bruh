src=$(wildcard src/*.cpp)\
$(wildcard src/cpu/*.cpp)\
$(wildcard src/input/*.cpp)\
$(wildcard src/memory/*.cpp)\
$(wildcard src/ppu/*.cpp)\
$(wildcard src/system/*.cpp)\
$(wildcard src/util/*.cpp)

obj=$(src:.cpp=.o)

LDFLAGS= -lstdc++ -lSDL2 -Wall -I.

GameBruh: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) GameBruh
