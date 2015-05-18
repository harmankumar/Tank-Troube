#ifndef POWERUP_H
#define POWERUP_H

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
#include <SFML/Audio.hpp>
#include "helperfn.h"





class PowerUp
{
public:
    PowerUp();
    sf::Texture PowerUpTexture;    //image to be loaded
    sf::Sprite myPowerUp;

    sf::SoundBuffer PowerUpSound;
    sf::Sound sound;
    int powerUpType;// 0 for shield, 1 for Health , 2 for armour
    float centrePowerUpX,centrePowerUpY;
    int idPowerUp;
    time_t timeActivated;
    void playSound();
    bool isActive;
};
extern int numPowerUps;
extern vector <PowerUp> powerUpVector;


#endif
