#ifndef TANK_H
#define TANK_H

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
#include<SFML/Graphics/Text.hpp>

#include "helperfn.h"
#include "bullet.h"


class Tank
{
public:

    sf::Texture tankTexture;    //image to be loaded
    sf::Sprite myTank;

    // jeetu
        //jeetu anim
    float animDur;              //it shows the current duration for which animation has been shown
    bool isDisplay;
    sf::Texture explodTex;
    AnimatedSprite *explode;
    Animation explosion;
    // jeetu
    sf::Text Name;
    sf::Font *fontMgr;
    sf::RectangleShape lifeBg;
    sf::RectangleShape lifeFill;
    //
   // sf::Texture nozzleTexture;    //image to be loaded
   // sf::Sprite myNozzle;
    bool isDestroyed;
    bool isLost;
    float centreX;
    float centreY;
    float radius;
    vector<float> Color;
    vector<int> keystate;          //JR
    float angleXaxis;
    float speed;
    float lenCanon;
    bool previt [255];      /// for handling keyboard functions

    bool collisionTopLeftPoint;
    bool collisionTopRightPoint;
    bool collisionBottomLeftPoint;
    bool collisionBottomRightPoint;
    bool rightCentreUp;     //Right wall
    bool rightCentreDown;
    bool leftCentreUp;
    bool leftCentreDown;
    bool topCentreRight;
    bool topCentreLeft;
    bool bottomCentreRight;
    bool bottomCentreLeft;

vector <float> xCoeffVertRect;
    vector <float> yCoeffVertRect;
    vector <float> constVertRect;
////
    float w;
    float h;
    float initAngle;
    float halfDiag;
    float canonEndX;
    float canonEndY;
    bool collisionTopWall;
    bool collisionRightWall;
    bool collisionLeftWall;
    bool collisionBottomWall;

    vector <float> xCordVertRect;
    vector <float> yCordVertRect;
    vector <float> angleRectRad;
    ///
    int currentPowerUp;
    time_t sheildActiveTime;
    bool sheildActive;
    ///
    int Health;
    int Bullets;
    int Missiles;
    int GMissiles;
    int Nuke;
    int Nitrous;
    int Shield;
    Tank();
    ~Tank();
    vector<float>   getTankPosition();
    vector<float>   getTankColour();
    //void            simulateTank();
    void            draw(sf::RenderTarget &target);
    float           getTankSpeed();
    float           getTankHealth();
    void            setTankRadius(float x);
    void            setTankSpeed (float x);
    void            setTankHealth(float setHealth);
    void            setTankPosition(vector<float> setPos,float angleTank);
    void            setTankAngle(float angle);
    void            setTankColour(vector<float> setColor);
    void            setLenCanon (float x);
    void        handleCollisionMazeWall();

    void            nexPosTank(vector<int> keyboardKeyList,double timeInterval);   // 0 for up, 1 for down, 2 for left, 3 for right //////////////////JR arg change

    void            handleCollisionOuterWall();
    //jeetu
    void            handleHealth();
    void            setName(string name);
    //
    //void      updateCordRect(float delX, float delY);
    bool        cannotMoveForward();
    bool        cannotMoveBackward();
    bool        cannotRotateAntiClock();
    bool        cannotRotateClock();
    bool        cannotRotateAntiClock2();
    bool        cannotRotateClock2();

    float distanceFromTankEdge(float xCoeff,float yCoeff,float constTerm,float xCordPoint,float yCordPoint);
    void handleCollisionMazeWallVertices();
    void updateEquationsRectEdges();
    bool cannotMoveForward2();
    bool cannotMoveBackward2();
    bool cannotRotate2();
    time_t timeLastShot;

    bool collisionMazeWallTop ;
    bool    collisionMazeWallBottom ;
    bool collisionMazeWallRight ;
    bool    collisionMazeWallLeft ;
    bool            destroyTank();
};

#endif
