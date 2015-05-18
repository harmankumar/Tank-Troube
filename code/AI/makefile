CC=g++

CPPFLAGS=   -lGL -lglut -lGLU -std=c++11 -lpthread -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -g -w
OBJ = game.o tank.o bullet.o helperfn.o ai.o AnimatedSprite.o Animation.o
SRC = game.cpp tank.cpp bullet.cpp helperfn.cpp ai.cpp  AnimatedSprite.cpp Animation.cpp
HDR = game.h tank.h bullet.h helperfn.h ai.h


AI: $(OBJ)
		$(CC) $^ $(CPPFLAGS)  -o $@ 

game.o: game.cpp $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@
tank.o: tank.cpp $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@
bullet.o: bullet.cpp $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@

helperfn.o: helperfn.cpp $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@

ai.o: ai.cpp $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@

.PHONY : clean

clean:
	rm $(OBJ)
