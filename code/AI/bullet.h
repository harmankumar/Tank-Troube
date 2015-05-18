#ifndef BULLET_H
#define BULLET_H

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





class Bullet
{
public:
    Bullet();
    sf::Texture bulletTexture;    //image to be loaded
    sf::Sprite myBullet;
    sf::CircleShape circle;
    //jeetu
    sf::SoundBuffer bulletSound;
    sf::Sound sound;
    float radiusBullet,centreBulletX,centreBulletY;
    int idBullet;
    int colorBulletR,colorBulletG,colorBulletB;   /// assign RGB later
    float speedBullet,angleBullet;
    void drawCircle();
    void nexPosBullet();
    void handleCollisionMazeWall ();//(vector < vector <float> > x);
    bool isCollidingWithWall();
    void handleCollisionOuterWall();
    void playSound();

    int findQuad();
    time_t lifeTime;
    bool isActive;
};
extern vector <Bullet> bulletShot;
extern int numBullets;

#endif

