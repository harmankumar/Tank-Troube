#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <algorithm>
#include <stack>
#include <queue>
#include <SFML/Graphics.hpp>
#include <vector>

#include "helperfn.h"
#include "tank.h"
#include "bullet.h"
#include "network.h"
#include "AnimatedSprite.hpp"
#include "powerUp.h"

class Game
{
public:
            Game();
    void    run();
    void    updateOtherTanks(int userId,float x,float y,float angle);
    void    destroyOtherTanks(int userId,int bulletId,float XCord,float YCord);
//    vector<Bullet> bulletsInGame;
    sf::RenderWindow gameWindow;
    sf::Texture *backGroundTex;    //image to be loaded
    //jeetuevein
    sf::Texture *vicBackground;
    sf::Texture *lossBackground;
    sf::Texture *mazeWalls;
    sf::Sprite endDisplay;
    int winnerId;
    sf::RectangleShape mazeBrick;
    //
    sf::RectangleShape background;

    vector< Tank >	 playerTanks;
    int numOfRounds;
    int maxNumOfRounds;
    time_t timeLastPowerUp;
	int activePowerUpId;

private:
    vector <vector <float> > maze1;
    void    processEvents();
    bool    decideWinner();
    void    update(pair<double,vector<int> > set);
    void    render2();
    void    render();
    void    display();
    void    keyboardHandle(unsigned char key, int x, int y);
    void    unPressHandle(unsigned char key,int x ,int y);
    void    handlePlayerInput(sf::Keyboard::Key x, bool y);
    void    broadCastdata();


};
#endif
