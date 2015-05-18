#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random>
#include <unistd.h>
#include <algorithm>
#include <stack>
#include <queue>
#include <SFML/Graphics.hpp>
#include <vector>

#include "helperfn.h"
#include "tank.h"
#include "bullet.h"
#include "AnimatedSprite.hpp"
#include "ai.h"

#define OFFSET 8
#define DISTEASY 200
#define DESTDEADEASY 60


class Game
{
public:
            Game(int mode);
    void    run(int mode);
    void    updateOtherTanks(int userId,float x,float y,float angle);
    void    destroyOtherTanks(int userId,int bulletId,float XCord,float YCord);
//    vector<Bullet> bulletsInGame;	
    sf::RenderWindow gameWindow;
        sf::Texture *backGroundTex;    //image to be loaded
    sf::RectangleShape background;

 sf::Texture *vicBackground;
    sf::Texture *lossBackground;
    sf::Texture *mazeWalls;
    sf::Sprite endDisplay;
    Tank user,computer;
    sf::RectangleShape mazeBrick;
    //
    
    time_t timeLastShot;
    int numOfRounds;
    int maxNumOfRounds;
    time_t AStarTime;
    vector <vector <float> > maze1;
    vector<pair<int , int>> path;
    int pathCtr;
private:
    
    void    processEvents();
    bool    decideWinner();
    void    update();
    
    void    updateClassic(); 
    void    updateNoMaze();
    void    updateDestroyTheTank();

    void    render();
    void    display();
    void    keyboardHandle(unsigned char key, int x, int y);
    void    unPressHandle(unsigned char key,int x ,int y);
    void    handlePlayerInput(sf::Keyboard::Key x, bool y);
  
};
#endif
