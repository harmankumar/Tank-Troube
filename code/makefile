CC=g++

CPPFLAGS=   -lGL -lglut -lGLU -std=c++11 -lpthread -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -w -g
OBJ = game.o tank.o bullet.o helperfn.o network.o AnimatedSprite.o Animation.o powerUp.o
SRC = game.cpp tank.cpp bullet.cpp helperfn.cpp network.cpp AnimatedSprite.cpp Animation.cpp powerUp.cpp 
HDR = game.h tank.h bullet.h helperfn.h network.h powerUp.h


all : tanktrouble lobby AI

lobby: 
	(cd QTGui/ ; qmake ; make all  )

AI:	
	(cd AI/ ; make  )

tanktrouble: $(OBJ)
		$(CC) $^ $(CPPFLAGS)  -o $@ 
run :
	(cd QTGui/ ; ./lobby ; cd .. ;./tanktrouble ) 
	
runAI :
	$(x)
	(cd AI/ ; ./AI x )

game.o: game.cpp $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@
tank.o: tank.cpp $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@

bullet.o: bullet.cpp $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@

helperfn.o: helperfn.cpp $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@

network.o: network.cpp  $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@
	
powerUp.o: powerUp.cpp  $(HDR)
	$(CC) -c $< $(CPPFLAGS) -lm -o $@


.PHONY : clean

clean:
	rm $(OBJ) 
	( cd QTGui/ ; make clean )
	( cd AI/ ; make clean )
