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
using namespace std;

# include "game.h"


/////////////////////////////

Game::Game(int mode):gameWindow(sf::VideoMode(window_height, window_width), "Game Window->"+tmpForWindow)
{
    numOfRounds=0;
    maxNumOfRounds=5;
        numBullets=0;
    bulletShot.resize(100);

    backGroundTex=new sf::Texture;
    backGroundTex->loadFromFile("grass.jpeg");
    backGroundTex->setRepeated(true);
    this->background.setSize(sf::Vector2f(float(backGroundTex->getSize().x),float(backGroundTex->getSize().y)));

    this->background.setTexture(backGroundTex);
    switch(mode)
                {
                    case 1:
                    {
                        //updateClassic();
                        maze1 = {{0,0,0,1080},{0,0,1080,0},{1080,0,1080,1080},{0,1080,1080,1080},{1080,200,800,200},{400,0,400,200},{200,800,600,800},{800,1080,800,700},{0,400,200,400}};
                        break;
                    }
                    case 2:
                    {
                        maze1={{0,0,0,1080},{0,0,1080,0},{1080,0,1080,1080},{0,1080,1080,1080}};
                        //updateNoMaze();
                        break;
                    }
                    default:
                    {
                        //updateDestroyTheTank();
                        maze1 = {{0,0,0,1080},{0,0,1080,0},{1080,0,1080,1080},{0,1080,1080,1080},{1080,200,800,200},{400,0,400,200},{200,800,600,800},{800,1080,800,700},{0,400,200,400}};
                        break;
                    }
                }

    // For Without Maze : {{0,0,0,1080},{0,0,1080,0},{1080,0,1080,1080},{0,1080,1080,1080}};

    // For AI : maze1 = {{0,0,0,1080},{0,0,1080,0},{1080,0,1080,1080},{0,1080,1080,1080},{1080,200,800,200},{400,0,400,200},{200,800,600,800},{800,1080,800,700},{0,400,200,400}};

    //maze1 = {{0,0,0,1080},{0,0,1080,0},{1080,0,1080,1080},{0,1080,1080,1080},{360,0,360,360},{720,1080,720,720},{0,720,360,720},{1080,360,720,360}};


    //,{0,100,200,100},{300,0,300,300},{500,0,500,100},{500,300,500,450},{400,450,600,450},{0,400,200,400},{100,500,100,600},{300,500,300,600},{500,500,600,500}};


    for (vector <vector <float> > :: iterator itMaze = maze1.begin(); itMaze != maze1.end(); ++itMaze)
    {
        /////////////////////////////////////////
        if ((*itMaze)[0] == (*itMaze)[2])//vert wall..... 1st-> 3 & 4.....2nd-> 1 & 2
        {
            quad1.push_back({(*itMaze)[0]+5,(*itMaze)[1],(*itMaze)[2]+5,(*itMaze)[3]});
            typeMazeWall.push_back(4);
            quad1.push_back({(*itMaze)[0]-5,(*itMaze)[1],(*itMaze)[2]-5,(*itMaze)[3]});
            typeMazeWall.push_back(3);
            quad1.push_back({(*itMaze)[0]-5,(*itMaze)[1],(*itMaze)[2]+5,(*itMaze)[1]});
            quad1.push_back({(*itMaze)[0]-5,(*itMaze)[3],(*itMaze)[2]+5,(*itMaze)[3]});
            if ((*itMaze)[3] > (*itMaze)[1])
            {
                typeMazeWall.push_back(1);
                typeMazeWall.push_back(2);
            }
            else
            {
                typeMazeWall.push_back(2);
                typeMazeWall.push_back(1);
            }
            //////////////change
            if ((*itMaze)[3] > (*itMaze)[1])
            {
                topLeftPoints.push_back({(*itMaze)[0]-5,(*itMaze)[1]});
                topRightPoints.push_back({(*itMaze)[0]+5,(*itMaze)[1]});
                bottomLeftPoints.push_back({(*itMaze)[0]-5,(*itMaze)[3]});
                bottomRightPoints.push_back({(*itMaze)[0]+5,(*itMaze)[3]});
            }
            else
            {
                topLeftPoints.push_back({(*itMaze)[0]-5,(*itMaze)[3]});
                topRightPoints.push_back({(*itMaze)[0]+5,(*itMaze)[3]});
                bottomLeftPoints.push_back({(*itMaze)[0]-5,(*itMaze)[1]});
                bottomRightPoints.push_back({(*itMaze)[0]+5,(*itMaze)[1]});
            }
        }
        else//hori wall... 2 and 3
        {
            quad1.push_back({(*itMaze)[0],(*itMaze)[1]+5,(*itMaze)[0],(*itMaze)[1]-5});
            quad1.push_back({(*itMaze)[2],(*itMaze)[3]+5,(*itMaze)[2],(*itMaze)[3]-5});
            if ((*itMaze)[0] > (*itMaze)[2])
            {
                typeMazeWall.push_back(4);
                typeMazeWall.push_back(3);
            }
            else
            {
                typeMazeWall.push_back(3);
                typeMazeWall.push_back(4);
            }
            quad1.push_back({(*itMaze)[0],(*itMaze)[1]+5,(*itMaze)[2],(*itMaze)[3]+5});
            typeMazeWall.push_back(2);
            quad1.push_back({(*itMaze)[0],(*itMaze)[1]-5,(*itMaze)[2],(*itMaze)[3]-5});
            typeMazeWall.push_back(1);
            //////////////change

            if ((*itMaze)[0] > (*itMaze)[2])
            {
                topLeftPoints.push_back({(*itMaze)[2],(*itMaze)[1]-5});
                topRightPoints.push_back({(*itMaze)[0],(*itMaze)[1]-5});
                bottomLeftPoints.push_back({(*itMaze)[2],(*itMaze)[3]+5});
                bottomRightPoints.push_back({(*itMaze)[0],(*itMaze)[3]+5});
            }
            else
            {
                topLeftPoints.push_back({(*itMaze)[0],(*itMaze)[1]-5});
                topRightPoints.push_back({(*itMaze)[2],(*itMaze)[1]-5});
                bottomLeftPoints.push_back({(*itMaze)[0],(*itMaze)[3]+5});
                bottomRightPoints.push_back({(*itMaze)[2],(*itMaze)[3]+5});
            }
        }

    }



    std::vector<float> pos;
    pos.resize(2);
    pos[0]=500;
    pos[1]=100;
    user.setTankPosition(pos,0);
    pos[0]=50;
    pos[1]=100;
    computer.setTankPosition(pos,0);
}

void Game::run(int mode)
{
//    sf::Clock clock;
//    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (gameWindow.isOpen())
    {
        if(!decideWinner())
        {
  //          timeSinceLastUpdate+=clock.restart();
    //        while(timeSinceLastUpdate.asSeconds()>timePerframe)
      //      {
        //        timeSinceLastUpdate=sf::seconds(timePerframe);
                processEvents();
                switch(mode)
                {
                    case 1:
                    {
                        updateClassic();
                        break;
                    }
                    case 2:
                    {
                        updateNoMaze();
                        break;
                    }
                   default :
                    {
                        updateDestroyTheTank();
                        break;
                    }
                }
          //  }

        }
        render();
    }
}


void Game::processEvents()  //add keys effects for own tank dont change directly only after proper interpolation
{
    sf::Event event;
    while (gameWindow.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
                {
                    handlePlayerInput(event.key.code , true);
                    break;
                }
            case sf::Event::KeyReleased:
                {
                    handlePlayerInput(event.key.code, false);
                    break;
                }
            case sf::Event::Closed:
                {
                    gameWindow.close();
                    break;
                }
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key keyCode , bool pressFlag)
{
    if (pressFlag)
    {
        switch(keyCode)
        {
            case  sf::Keyboard::Left :
                    {
                        user.keystate[2] = 1;
                        break;
                    }
            case  sf::Keyboard::Right:
                    {
                        user.keystate[3] =  1;
                        break;
                    }
            case  sf::Keyboard::Up   :
                    {
                        user.keystate[0] = 1;
                        break;
                    }
            case  sf::Keyboard::Down :
                    {
                       user.keystate[1] = 1;
                        break;
                    }
            case sf::Keyboard::M   :
                    {
                        user.keystate[4] = 1;
                        break;
                    }
        }
    }
    else
        {
        switch(keyCode)
        {
            case  sf::Keyboard::Left :
                    {
                        user.keystate[2] = 0;
                        break;
                    }
            case  sf::Keyboard::Right:
                    {
                        user.keystate[3] =  0;
                         break;
                    }
            case  sf::Keyboard::Up   :
                    {
                        user.keystate[0] =  0;
                          break;
                    }
            case  sf::Keyboard::Down :
                    {
                        user.keystate[1] = 0;
                        break;
                    }
            case sf::Keyboard::M   :
                    {
                        user.keystate[4] = 0;
                        break;
                    }
        }
    }
}

bool callastar = false;

void   Game::updateClassic()
{
    if(!user.isDestroyed)
    {
        this->user.nexPosTank(user.keystate,0.1);
        this->user.handleCollisionOuterWall();
        this->user.handleCollisionMazeWall();
        this->user.destroyTank();
        this->user.handleCollisionMazeWallVertices();
        this->user.explode->update(sf::seconds(timePerframe));
        if(user.isDisplay)
            user.animDur+=timePerframe;

        user.handleHealth();

    }
    cout<<"UpdateLoop"<<endl;
    time_t temp;
    time(&temp);
    for(int iter=0;iter<bulletShot.size() && iter<numBullets;iter++)
    {
        if(bulletShot[iter].isActive )
        {
            bulletShot[iter].nexPosBullet();
            bulletShot[iter].handleCollisionOuterWall();
            bulletShot[iter].handleCollisionMazeWall();
        }
        if(difftime(temp,bulletShot[iter].lifeTime)>10)
            bulletShot[iter].isActive=false;
    }

    if((!computer.isDestroyed)&&(!user.isDestroyed))
    {
        this->computer.destroyTank();
        this->computer.handleCollisionOuterWall();
            // get next position using aStar
        int x1,x2,y1,y2;
        std::vector<float> pos = computer.getTankPosition();
        x1=(int)pos[0];
        y1=(int)pos[1];
        pos=user.getTankPosition();
        x2=(int)pos[0];
        y2=(int)pos[1];
        //cout<<"Before"<<endl;


        /* FOR DESTROY THE TANK MODE : */


/*
        if(inlineofsight(x1 , x2 , y1 , y2))
        {

            float Angle = atan( (float) ((float)(y2-y1))/(float)((x2-x1)));

            if((x2 < x1))
            {
                Angle += Pi;
            }

            // Set the orientation.



            // Shoot.
            time_t temp;
            time(&temp);
            if(difftime(temp,computer.timeLastShot)>0.4)
            {
                computer.setTankAngle(Angle);
                bulletShot[numBullets].centreBulletX = computer.centreX;           /// make it leave from the canon's end point
                bulletShot[numBullets].centreBulletY = computer.centreY;
                //cout<<"centreX "<<bShot.centreBulletX<<endl;
                //cout<<"centreY "<<bShot.centreBulletY<<endl;
                            /// make it leave from the canon's end point
                bulletShot[numBullets].angleBullet = computer.angleXaxis;
                (bulletShot[numBullets].myBullet).setPosition(computer.centreX,computer.centreY);
                (bulletShot[numBullets].myBullet).setRotation(computer.angleXaxis*180.0/Pi);
                (bulletShot[numBullets].circle).setPosition(computer.centreX,computer.centreY);
                time(&bulletShot[numBullets].lifeTime);

                bulletShot[numBullets].playSound();
                //
                bulletShot[numBullets].isActive=true;

                 bool freeFromShooter=false;
                  while(!freeFromShooter)
                  {
                    bulletShot[numBullets].nexPosBullet();
                    bulletShot[numBullets].handleCollisionOuterWall();
                    bulletShot[numBullets].handleCollisionMazeWall();
                    sf::FloatRect boundsTank = computer.myTank.getGlobalBounds();
                    if (boundsTank.contains(bulletShot[numBullets].centreBulletX,bulletShot[numBullets].centreBulletY) )
                    {
                        cout<<"Freeing bullet from tank"<<endl;
                        freeFromShooter=false;
                    }
                    else
                    {
                        cout<<"Bullet freed from tank"<<endl;
                        freeFromShooter=true;
                    }
                  }
                  time(&computer.timeLastShot);
                numBullets++;

            }
        }
*/

/* FOR CLASSIC MODE */

        bool goin = true;

        if(inlineofsight(x1 , x2 , y1 , y2)&&(sqrt(pow((x1-x2),2) + pow((y1-y2),2)) <= 400))
        {
                goin = false;

                float Angle = atan( (float) ((float)(y2-y1))/(float)((x2-x1)));

                if((x2 < x1))
                {
                    Angle += Pi;
                }

                computer.setTankAngle(Angle);

                // Set the orientation.

                callastar = true;

                // Shoot.
                time_t temp;
                time(&temp);
                if(difftime(temp,computer.timeLastShot)>1)
                {
                    computer.setTankAngle(Angle);
                    bulletShot[numBullets].centreBulletX = computer.centreX;           /// make it leave from the canon's end point
                    bulletShot[numBullets].centreBulletY = computer.centreY;
                    //cout<<"centreX "<<bShot.centreBulletX<<endl;
                    //cout<<"centreY "<<bShot.centreBulletY<<endl;
                                /// make it leave from the canon's end point
                    bulletShot[numBullets].angleBullet = computer.angleXaxis;
                    (bulletShot[numBullets].myBullet).setPosition(computer.centreX,computer.centreY);
                    (bulletShot[numBullets].myBullet).setRotation(computer.angleXaxis*180.0/Pi);
                    (bulletShot[numBullets].circle).setPosition(computer.centreX,computer.centreY);
                    time(&bulletShot[numBullets].lifeTime);
                    //jeetu
                    bulletShot[numBullets].playSound();
                    //
                    bulletShot[numBullets].isActive=true;

                     bool freeFromShooter=false;
                      while(!freeFromShooter)
                      {
                        bulletShot[numBullets].nexPosBullet();
                        bulletShot[numBullets].handleCollisionOuterWall();
                        bulletShot[numBullets].handleCollisionMazeWall();
                        sf::FloatRect boundsTank = computer.myTank.getGlobalBounds();
                        if (boundsTank.contains(bulletShot[numBullets].centreBulletX,bulletShot[numBullets].centreBulletY) )
                        {
                            cout<<"Freeing bullet from tank"<<endl;
                            freeFromShooter=false;
                        }
                        else
                        {
                            cout<<"Bullet freed from tank"<<endl;
                            freeFromShooter=true;
                        }
                      }

                    numBullets++;
                    time(&computer.timeLastShot);
                }

                // Noe to move the tank closer to the player till a fixed diatance
                if(inboundstank(x1+1,y1) && inboundstank(x1-1,y1) && inboundstank(x1,y1-1) && inboundstank(x1,y1+1))
                {
                    if(sqrt(pow((x1-x2),2) + pow((y1-y2),2)) >= 50)
                    {
                        int cossy = cos(Angle) >= 0 ? 1 : -1;
                        int sinny = sin(Angle) >= 0 ? 1 : -1;
                        pos[0] = x1 + cossy;
                        pos[1] = y1 + sinny;

                        this->computer.setTankPosition(pos,Angle);
                    }
                }
            }



        /*
        One option for Dead Easy level is having a local search with fixed path length.
        */
         else
         {
           time(&temp);
                if(difftime(temp,AStarTime)>1 || pathCtr>300 || callastar == true)
                {
                    callastar = false;
                    time(&AStarTime);
                    vector<pair<int , int>> tempPath = AStar (x1 , x2 , y1 , y2);
                    if(tempPath.size()>300)
                    {
                        tempPath.resize(300);
                    }
                    path=tempPath;
                    pathCtr=0;
                }
                if(path.size()==0 || pathCtr>300 || pathCtr>path.size())//
                {
                    cout<<" Nothing Returned ... Empty vector."<<endl;
                }
                else// Move the tank now
                {
                        cout<<"After calling"<<endl;
                            int x,y;
                        //cout<<path.size()<<"..."<<endl;

                         x=path[pathCtr].first;
                         y=path[pathCtr].second;

                        // Setting The tank Angle.

                             pos[0]=x;
                             pos[1]=y;
                        if( x - x1 == 1)
                        {
                            this->computer.setTankPosition(pos,0);
                            computer.setTankAngle(0);
                        }

                        if( x1 - x == 1)
                        {
                            this->computer.setTankPosition(pos,-Pi);
                            computer.setTankAngle(-Pi);
                        }

                        if( y - y1 == 1)
                        {
                            this->computer.setTankPosition(pos,Pi/2);
                            computer.setTankAngle(Pi/2);
                        }

                        if( y1 - y == 1)
                        {
                            this->computer.setTankPosition(pos,-Pi/2);
                            computer.setTankAngle(-Pi/2);
                        }
                        pathCtr++;
                }

            }

    /*  FOR THIRD MODE : SIMPLE IT'S NO MAZE MODE i.e. TWO TANKS AND BOUNDING BORDERS. */



     }


}

/////////////////// COMPLETE THIS ////////////////////////////////

void  Game::updateNoMaze()
{
    // The same AI has to be used
    if(!user.isDestroyed)
    {
        this->user.nexPosTank(user.keystate,0.1);
        this->user.handleCollisionOuterWall();
        this->user.handleCollisionMazeWall();
        this->user.destroyTank();
        this->user.handleCollisionMazeWallVertices();
        this->user.explode->update(sf::seconds(timePerframe));
        if(user.isDisplay)
            user.animDur+=timePerframe;

        user.handleHealth();

    }
    cout<<"UpdateLoop"<<endl;
    time_t temp;
    time(&temp);
    for(int iter=0;iter<bulletShot.size() && iter<numBullets;iter++)
    {
        if(bulletShot[iter].isActive )
        {
            bulletShot[iter].nexPosBullet();
            bulletShot[iter].handleCollisionOuterWall();
            bulletShot[iter].handleCollisionMazeWall();
        }
        if(difftime(temp,bulletShot[iter].lifeTime)>10)
            bulletShot[iter].isActive=false;
    }

    if((!computer.isDestroyed)&&(!user.isDestroyed))
    {
        this->computer.destroyTank();
        this->computer.handleCollisionOuterWall();
            // get next position using aStar
        int x1,x2,y1,y2;
        std::vector<float> pos = computer.getTankPosition();
        x1=(int)pos[0];
        y1=(int)pos[1];
        pos=user.getTankPosition();
        x2=(int)pos[0];
        y2=(int)pos[1];
        //cout<<"Before"<<endl;


        /* FOR DESTROY THE TANK MODE : */


/*
        if(inlineofsight(x1 , x2 , y1 , y2))
        {

            float Angle = atan( (float) ((float)(y2-y1))/(float)((x2-x1)));

            if((x2 < x1))
            {
                Angle += Pi;
            }

            // Set the orientation.



            // Shoot.
            time_t temp;
            time(&temp);
            if(difftime(temp,computer.timeLastShot)>0.4)
            {
                computer.setTankAngle(Angle);
                bulletShot[numBullets].centreBulletX = computer.centreX;           /// make it leave from the canon's end point
                bulletShot[numBullets].centreBulletY = computer.centreY;
                //cout<<"centreX "<<bShot.centreBulletX<<endl;
                //cout<<"centreY "<<bShot.centreBulletY<<endl;
                            /// make it leave from the canon's end point
                bulletShot[numBullets].angleBullet = computer.angleXaxis;
                (bulletShot[numBullets].myBullet).setPosition(computer.centreX,computer.centreY);
                (bulletShot[numBullets].myBullet).setRotation(computer.angleXaxis*180.0/Pi);
                (bulletShot[numBullets].circle).setPosition(computer.centreX,computer.centreY);
                time(&bulletShot[numBullets].lifeTime);

                bulletShot[numBullets].playSound();
                //
                bulletShot[numBullets].isActive=true;

                 bool freeFromShooter=false;
                  while(!freeFromShooter)
                  {
                    bulletShot[numBullets].nexPosBullet();
                    bulletShot[numBullets].handleCollisionOuterWall();
                    bulletShot[numBullets].handleCollisionMazeWall();
                    sf::FloatRect boundsTank = computer.myTank.getGlobalBounds();
                    if (boundsTank.contains(bulletShot[numBullets].centreBulletX,bulletShot[numBullets].centreBulletY) )
                    {
                        cout<<"Freeing bullet from tank"<<endl;
                        freeFromShooter=false;
                    }
                    else
                    {
                        cout<<"Bullet freed from tank"<<endl;
                        freeFromShooter=true;
                    }
                  }
                  time(&computer.timeLastShot);
                numBullets++;

            }
        }
*/

/* FOR CLASSIC MODE */

        bool goin = true;

        if(inlineofsight(x1 , x2 , y1 , y2))
        {

                goin = false;

                float Angle = atan( (float) ((float)(y2-y1))/(float)((x2-x1)));

                if((x2 < x1))
                {
                    Angle += Pi;
                }

                computer.setTankAngle(Angle);

                // Set the orientation.

                callastar = true;

                // Shoot.
                time_t temp;
                time(&temp);
                if(difftime(temp,computer.timeLastShot)>1)
                {
                    computer.setTankAngle(Angle);
                    bulletShot[numBullets].centreBulletX = computer.centreX;           /// make it leave from the canon's end point
                    bulletShot[numBullets].centreBulletY = computer.centreY;
                    //cout<<"centreX "<<bShot.centreBulletX<<endl;
                    //cout<<"centreY "<<bShot.centreBulletY<<endl;
                                /// make it leave from the canon's end point
                    bulletShot[numBullets].angleBullet = computer.angleXaxis;
                    (bulletShot[numBullets].myBullet).setPosition(computer.centreX,computer.centreY);
                    (bulletShot[numBullets].myBullet).setRotation(computer.angleXaxis*180.0/Pi);
                    (bulletShot[numBullets].circle).setPosition(computer.centreX,computer.centreY);
                    time(&bulletShot[numBullets].lifeTime);
                    //jeetu
                    bulletShot[numBullets].playSound();
                    //
                    bulletShot[numBullets].isActive=true;

                     bool freeFromShooter=false;
                      while(!freeFromShooter)
                      {
                        bulletShot[numBullets].nexPosBullet();
                        bulletShot[numBullets].handleCollisionOuterWall();
                        bulletShot[numBullets].handleCollisionMazeWall();
                        sf::FloatRect boundsTank = computer.myTank.getGlobalBounds();
                        if (boundsTank.contains(bulletShot[numBullets].centreBulletX,bulletShot[numBullets].centreBulletY) )
                        {
                            cout<<"Freeing bullet from tank"<<endl;
                            freeFromShooter=false;
                        }
                        else
                        {
                            cout<<"Bullet freed from tank"<<endl;
                            freeFromShooter=true;
                        }
                      }

                    numBullets++;
                    time(&computer.timeLastShot);
                }

                // Noe to move the tank closer to the player till a fixed diatance
                if(inboundstank(x1+1,y1) && inboundstank(x1-1,y1) && inboundstank(x1,y1-1) && inboundstank(x1,y1+1))
                {
                    if(sqrt(pow((x1-x2),2) + pow((y1-y2),2)) >= 20)
                    {
                        pos[0] = x1 + cos(Angle);
                        pos[1] = y1 + sin(Angle);

                        this->computer.setTankPosition(pos,Angle);
                    }
                }
            }



        /*
        One option for Dead Easy level is having a local search with fixed path length.
        */
         else
         {
           time(&temp);
                if(difftime(temp,AStarTime)>1 || pathCtr>100 || callastar == true)
                {
                    callastar = false;
                    time(&AStarTime);
                    vector<pair<int , int>> tempPath = AStar (x1 , x2 , y1 , y2);
                    if(tempPath.size()>100)
                    {
                        tempPath.resize(100);
                    }
                    path=tempPath;
                    pathCtr=0;
                }
                if(path.size()==0 || pathCtr>100 || pathCtr>path.size())//
                {
                    cout<<" Nothing Returned ... Empty vector."<<endl;
                }
                else// Move the tank now
                {
                        cout<<"After calling"<<endl;
                            int x,y;
                        //cout<<path.size()<<"..."<<endl;

                         x=path[pathCtr].first;
                         y=path[pathCtr].second;

                        // Setting The tank Angle.

                             pos[0]=x;
                             pos[1]=y;
                        if( x - x1 == 1)
                        {
                            this->computer.setTankPosition(pos,0);
                            computer.setTankAngle(0);
                        }

                        if( x1 - x == 1)
                        {
                            this->computer.setTankPosition(pos,-Pi);
                            computer.setTankAngle(-Pi);
                        }

                        if( y - y1 == 1)
                        {
                            this->computer.setTankPosition(pos,Pi/2);
                            computer.setTankAngle(Pi/2);
                        }

                        if( y1 - y == 1)
                        {
                            this->computer.setTankPosition(pos,-Pi/2);
                            computer.setTankAngle(-Pi/2);
                        }
                        pathCtr++;
                }

            }

    /*  FOR THIRD MODE : SIMPLE IT'S NO MAZE MODE i.e. TWO TANKS AND BOUNDING BORDERS. */



     }

}

//////////////////////////////////////////////////////////////////


void  Game::updateDestroyTheTank()
{
    if(!user.isDestroyed)
    {
        this->user.nexPosTank(user.keystate,0.1);
        this->user.handleCollisionOuterWall();
        this->user.handleCollisionMazeWall();
        this->user.destroyTank();
        this->user.handleCollisionMazeWallVertices();
        this->user.explode->update(sf::seconds(timePerframe));
        if(user.isDisplay)
            user.animDur+=timePerframe;

        user.handleHealth();

    }
    cout<<"UpdateLoop"<<endl;
    time_t temp;
    time(&temp);
    for(int iter=0;iter<bulletShot.size() && iter<numBullets;iter++)
    {
        if(bulletShot[iter].isActive )
        {
            bulletShot[iter].nexPosBullet();
            bulletShot[iter].handleCollisionOuterWall();
            bulletShot[iter].handleCollisionMazeWall();
        }
        if(difftime(temp,bulletShot[iter].lifeTime)>10)
            bulletShot[iter].isActive=false;
    }

    if((!computer.isDestroyed)&&(!user.isDestroyed))
    {
        this->computer.destroyTank();
        this->computer.handleCollisionOuterWall();
            // get next position using aStar
        int x1,x2,y1,y2;
        std::vector<float> pos = computer.getTankPosition();
        x1=(int)pos[0];
        y1=(int)pos[1];
        pos=user.getTankPosition();
        x2=(int)pos[0];
        y2=(int)pos[1];
        //cout<<"Before"<<endl;


        /* FOR DESTROY THE TANK MODE : */

        if(inlineofsight(x1 , x2 , y1 , y2))
        {

            float Angle = atan( (float) ((float)(y2-y1))/(float)((x2-x1)));

            if((x2 < x1))
            {
                Angle += Pi;
            }

            // Set the orientation.



            // Shoot.
            time_t temp;
            time(&temp);
            if(difftime(temp,computer.timeLastShot)>0.4)
            {
                computer.setTankAngle(Angle);
                bulletShot[numBullets].centreBulletX = computer.centreX;           /// make it leave from the canon's end point
                bulletShot[numBullets].centreBulletY = computer.centreY;
                //cout<<"centreX "<<bShot.centreBulletX<<endl;
                //cout<<"centreY "<<bShot.centreBulletY<<endl;
                            /// make it leave from the canon's end point
                bulletShot[numBullets].angleBullet = computer.angleXaxis;
                (bulletShot[numBullets].myBullet).setPosition(computer.centreX,computer.centreY);
                (bulletShot[numBullets].myBullet).setRotation(computer.angleXaxis*180.0/Pi);
                (bulletShot[numBullets].circle).setPosition(computer.centreX,computer.centreY);
                time(&bulletShot[numBullets].lifeTime);

                bulletShot[numBullets].playSound();
                //
                bulletShot[numBullets].isActive=true;

                 bool freeFromShooter=false;
                  while(!freeFromShooter)
                  {
                    bulletShot[numBullets].nexPosBullet();
                    bulletShot[numBullets].handleCollisionOuterWall();
                    bulletShot[numBullets].handleCollisionMazeWall();
                    sf::FloatRect boundsTank = computer.myTank.getGlobalBounds();
                    if (boundsTank.contains(bulletShot[numBullets].centreBulletX,bulletShot[numBullets].centreBulletY) )
                    {
                        cout<<"Freeing bullet from tank"<<endl;
                        freeFromShooter=false;
                    }
                    else
                    {
                        cout<<"Bullet freed from tank"<<endl;
                        freeFromShooter=true;
                    }
                  }
                  time(&computer.timeLastShot);
                numBullets++;

            }
        }


/* FOR CLASSIC MODE */

        // bool goin = true;

        // if(inlineofsight(x1 , x2 , y1 , y2))
        // {

        //         goin = false;

        //         float Angle = atan( (float) ((float)(y2-y1))/(float)((x2-x1)));

        //         if((x2 < x1))
        //         {
        //             Angle += Pi;
        //         }

        //         computer.setTankAngle(Angle);

        //         // Set the orientation.

        //         callastar = true;

        //         // Shoot.
        //         time_t temp;
        //         time(&temp);
        //         if(difftime(temp,computer.timeLastShot)>1)
        //         {
        //             computer.setTankAngle(Angle);
        //             bulletShot[numBullets].centreBulletX = computer.centreX;           /// make it leave from the canon's end point
        //             bulletShot[numBullets].centreBulletY = computer.centreY;
        //             //cout<<"centreX "<<bShot.centreBulletX<<endl;
        //             //cout<<"centreY "<<bShot.centreBulletY<<endl;
        //                         /// make it leave from the canon's end point
        //             bulletShot[numBullets].angleBullet = computer.angleXaxis;
        //             (bulletShot[numBullets].myBullet).setPosition(computer.centreX,computer.centreY);
        //             (bulletShot[numBullets].myBullet).setRotation(computer.angleXaxis*180.0/Pi);
        //             (bulletShot[numBullets].circle).setPosition(computer.centreX,computer.centreY);
        //             time(&bulletShot[numBullets].lifeTime);
        //             //jeetu
        //             bulletShot[numBullets].playSound();
        //             //
        //             bulletShot[numBullets].isActive=true;

        //              bool freeFromShooter=false;
        //               while(!freeFromShooter)
        //               {
        //                 bulletShot[numBullets].nexPosBullet();
        //                 bulletShot[numBullets].handleCollisionOuterWall();
        //                 bulletShot[numBullets].handleCollisionMazeWall();
        //                 sf::FloatRect boundsTank = computer.myTank.getGlobalBounds();
        //                 if (boundsTank.contains(bulletShot[numBullets].centreBulletX,bulletShot[numBullets].centreBulletY) )
        //                 {
        //                     cout<<"Freeing bullet from tank"<<endl;
        //                     freeFromShooter=false;
        //                 }
        //                 else
        //                 {
        //                     cout<<"Bullet freed from tank"<<endl;
        //                     freeFromShooter=true;
        //                 }
        //               }

        //             numBullets++;
        //             time(&computer.timeLastShot);
        //         }

        //         // Noe to move the tank closer to the player till a fixed diatance
        //         if(inboundstank(x1+1,y1) && inboundstank(x1-1,y1) && inboundstank(x1,y1-1) && inboundstank(x1,y1+1))
        //         {
        //             if(sqrt(pow((x1-x2),2) + pow((y1-y2),2)) >= 20)
        //             {
        //                 pos[0] = x1 + cos(Angle);
        //                 pos[1] = y1 + sin(Angle);

        //                 this->computer.setTankPosition(pos,Angle);
        //             }
        //         }
        //     }



        // /*
        // One option for Dead Easy level is having a local search with fixed path length.
        // */
        //  else
        //  {
        //    time(&temp);
        //         if(difftime(temp,AStarTime)>1 || pathCtr>100 || callastar == true)
        //         {
        //             callastar = false;
        //             time(&AStarTime);
        //             vector<pair<int , int>> tempPath = AStar (x1 , x2 , y1 , y2);
        //             if(tempPath.size()>100)
        //             {
        //                 tempPath.resize(100);
        //             }
        //             path=tempPath;
        //             pathCtr=0;
        //         }
        //         if(path.size()==0 || pathCtr>100 || pathCtr>path.size())//
        //         {
        //             cout<<" Nothing Returned ... Empty vector."<<endl;
        //         }
        //         else// Move the tank now
        //         {
        //                 cout<<"After calling"<<endl;
        //                     int x,y;
        //                 //cout<<path.size()<<"..."<<endl;

        //                  x=path[pathCtr].first;
        //                  y=path[pathCtr].second;

        //                 // Setting The tank Angle.

        //                      pos[0]=x;
        //                      pos[1]=y;
        //                 if( x - x1 == 1)
        //                 {
        //                     this->computer.setTankPosition(pos,0);
        //                     computer.setTankAngle(0);
        //                 }

        //                 if( x1 - x == 1)
        //                 {
        //                     this->computer.setTankPosition(pos,-Pi);
        //                     computer.setTankAngle(-Pi);
        //                 }

        //                 if( y - y1 == 1)
        //                 {
        //                     this->computer.setTankPosition(pos,Pi/2);
        //                     computer.setTankAngle(Pi/2);
        //                 }

        //                 if( y1 - y == 1)
        //                 {
        //                     this->computer.setTankPosition(pos,-Pi/2);
        //                     computer.setTankAngle(-Pi/2);
        //                 }
        //                 pathCtr++;
        //         }

        //     }

    /*  FOR THIRD MODE : SIMPLE IT'S NO MAZE MODE i.e. TWO TANKS AND BOUNDING BORDERS. */



     }


}

void Game::update()
{
    if(!user.isDestroyed)
    {
        this->user.nexPosTank(user.keystate,0.1);
        this->user.handleCollisionOuterWall();
        this->user.handleCollisionMazeWall();
        this->user.destroyTank();
        this->user.handleCollisionMazeWallVertices();
        this->user.explode->update(sf::seconds(timePerframe));
        if(user.isDisplay)
            user.animDur+=timePerframe;

        user.handleHealth();

    }
    cout<<"UpdateLoop"<<endl;
    time_t temp;
    time(&temp);
    for(int iter=0;iter<bulletShot.size() && iter<numBullets;iter++)
    {
        if(bulletShot[iter].isActive )
        {
            bulletShot[iter].nexPosBullet();
            bulletShot[iter].handleCollisionOuterWall();
            bulletShot[iter].handleCollisionMazeWall();
        }
        if(difftime(temp,bulletShot[iter].lifeTime)>10)
            bulletShot[iter].isActive=false;
    }

    if((!computer.isDestroyed)&&(!user.isDestroyed))
    {
        this->computer.destroyTank();
        this->computer.handleCollisionOuterWall();
            // get next position using aStar
        int x1,x2,y1,y2;
        std::vector<float> pos = computer.getTankPosition();
        x1=(int)pos[0];
        y1=(int)pos[1];
        pos=user.getTankPosition();
        x2=(int)pos[0];
        y2=(int)pos[1];
        //cout<<"Before"<<endl;


        /* FOR DESTROY THE TANK MODE : */


/*
        if(inlineofsight(x1 , x2 , y1 , y2))
        {

            float Angle = atan( (float) ((float)(y2-y1))/(float)((x2-x1)));

            if((x2 < x1))
            {
                Angle += Pi;
            }

            // Set the orientation.



            // Shoot.
            time_t temp;
            time(&temp);
            if(difftime(temp,computer.timeLastShot)>0.4)
            {
                computer.setTankAngle(Angle);
                bulletShot[numBullets].centreBulletX = computer.centreX;           /// make it leave from the canon's end point
                bulletShot[numBullets].centreBulletY = computer.centreY;
                //cout<<"centreX "<<bShot.centreBulletX<<endl;
                //cout<<"centreY "<<bShot.centreBulletY<<endl;
                            /// make it leave from the canon's end point
                bulletShot[numBullets].angleBullet = computer.angleXaxis;
                (bulletShot[numBullets].myBullet).setPosition(computer.centreX,computer.centreY);
                (bulletShot[numBullets].myBullet).setRotation(computer.angleXaxis*180.0/Pi);
                (bulletShot[numBullets].circle).setPosition(computer.centreX,computer.centreY);
                time(&bulletShot[numBullets].lifeTime);

                bulletShot[numBullets].playSound();
                //
                bulletShot[numBullets].isActive=true;

                 bool freeFromShooter=false;
                  while(!freeFromShooter)
                  {
                    bulletShot[numBullets].nexPosBullet();
                    bulletShot[numBullets].handleCollisionOuterWall();
                    bulletShot[numBullets].handleCollisionMazeWall();
                    sf::FloatRect boundsTank = computer.myTank.getGlobalBounds();
                    if (boundsTank.contains(bulletShot[numBullets].centreBulletX,bulletShot[numBullets].centreBulletY) )
                    {
                        cout<<"Freeing bullet from tank"<<endl;
                        freeFromShooter=false;
                    }
                    else
                    {
                        cout<<"Bullet freed from tank"<<endl;
                        freeFromShooter=true;
                    }
                  }
                  time(&computer.timeLastShot);
                numBullets++;

            }
        }
*/

/* FOR CLASSIC MODE */

        bool goin = true;

        if(inlineofsight(x1 , x2 , y1 , y2)&&(sqrt(pow((x1-x2),2) + pow((y1-y2),2)) <= 400))
        {
                goin = false;

                float Angle = atan( (float) ((float)(y2-y1))/(float)((x2-x1)));

                if((x2 < x1))
                {
                    Angle += Pi;
                }

                computer.setTankAngle(Angle);

                // Set the orientation.

                callastar = true;

                // Shoot.
                time_t temp;
                time(&temp);
                if(difftime(temp,computer.timeLastShot)>1)
                {
                    computer.setTankAngle(Angle);
                    bulletShot[numBullets].centreBulletX = computer.centreX;           /// make it leave from the canon's end point
                    bulletShot[numBullets].centreBulletY = computer.centreY;
                    //cout<<"centreX "<<bShot.centreBulletX<<endl;
                    //cout<<"centreY "<<bShot.centreBulletY<<endl;
                                /// make it leave from the canon's end point
                    bulletShot[numBullets].angleBullet = computer.angleXaxis;
                    (bulletShot[numBullets].myBullet).setPosition(computer.centreX,computer.centreY);
                    (bulletShot[numBullets].myBullet).setRotation(computer.angleXaxis*180.0/Pi);
                    (bulletShot[numBullets].circle).setPosition(computer.centreX,computer.centreY);
                    time(&bulletShot[numBullets].lifeTime);
                    //jeetu
                    bulletShot[numBullets].playSound();
                    //
                    bulletShot[numBullets].isActive=true;

                     bool freeFromShooter=false;
                      while(!freeFromShooter)
                      {
                        bulletShot[numBullets].nexPosBullet();
                        bulletShot[numBullets].handleCollisionOuterWall();
                        bulletShot[numBullets].handleCollisionMazeWall();
                        sf::FloatRect boundsTank = computer.myTank.getGlobalBounds();
                        if (boundsTank.contains(bulletShot[numBullets].centreBulletX,bulletShot[numBullets].centreBulletY) )
                        {
                            cout<<"Freeing bullet from tank"<<endl;
                            freeFromShooter=false;
                        }
                        else
                        {
                            cout<<"Bullet freed from tank"<<endl;
                            freeFromShooter=true;
                        }
                      }

                    numBullets++;
                    time(&computer.timeLastShot);
                }

                // Noe to move the tank closer to the player till a fixed diatance
                if(inboundstank(x1+1,y1) && inboundstank(x1-1,y1) && inboundstank(x1,y1-1) && inboundstank(x1,y1+1))
                {
                    if(sqrt(pow((x1-x2),2) + pow((y1-y2),2)) >= 50)
                    {
                        int cossy = cos(Angle) >= 0 ? 1 : -1;
                        int sinny = sin(Angle) >= 0 ? 1 : -1;
                        pos[0] = x1 + cossy;
                        pos[1] = y1 + sinny;

                        this->computer.setTankPosition(pos,Angle);
                    }
                }
            }



        /*
        One option for Dead Easy level is having a local search with fixed path length.
        */
         else
         {
           time(&temp);
                if(difftime(temp,AStarTime)>1 || pathCtr>300 || callastar == true)
                {
                    callastar = false;
                    time(&AStarTime);
                    vector<pair<int , int>> tempPath = AStar (x1 , x2 , y1 , y2);
                    if(tempPath.size()>300)
                    {
                        tempPath.resize(300);
                    }
                    path=tempPath;
                    pathCtr=0;
                }
                if(path.size()==0 || pathCtr>300 || pathCtr>path.size())//
                {
                    cout<<" Nothing Returned ... Empty vector."<<endl;
                }
                else// Move the tank now
                {
                        cout<<"After calling"<<endl;
                            int x,y;
                        //cout<<path.size()<<"..."<<endl;

                         x=path[pathCtr].first;
                         y=path[pathCtr].second;

                        // Setting The tank Angle.

                             pos[0]=x;
                             pos[1]=y;
                        if( x - x1 == 1)
                        {
                            this->computer.setTankPosition(pos,0);
                            computer.setTankAngle(0);
                        }

                        if( x1 - x == 1)
                        {
                            this->computer.setTankPosition(pos,-Pi);
                            computer.setTankAngle(-Pi);
                        }

                        if( y - y1 == 1)
                        {
                            this->computer.setTankPosition(pos,Pi/2);
                            computer.setTankAngle(Pi/2);
                        }

                        if( y1 - y == 1)
                        {
                            this->computer.setTankPosition(pos,-Pi/2);
                            computer.setTankAngle(-Pi/2);
                        }
                        pathCtr++;
                }

            }

    /*  FOR THIRD MODE : SIMPLE IT'S NO MAZE MODE i.e. TWO TANKS AND BOUNDING BORDERS. */



     }


}

void Game::render()     //maze rendering part via shapes remain
{
   // cout<<"In render"<<endl;
    gameWindow.clear();
    for(int y=0;y<5;y++)
    {
        for(int x=0;x<5;x++)
        {
            background.setPosition(background.getSize().x*x,background.getSize().y*y);
                    gameWindow.draw(background);
        }
    }
    if(!user.isDestroyed)
    {
        gameWindow.draw(user.myTank);
        gameWindow.draw(user.lifeBg);
        gameWindow.draw(user.lifeFill);
        gameWindow.draw(user.Name);
    }
    if(!computer.isDestroyed)
    {
        gameWindow.draw(computer.myTank);
        gameWindow.draw(computer.lifeBg);
        gameWindow.draw(computer.lifeFill);
        gameWindow.draw(computer.Name);
    }
    time_t temp;
    time(&temp);
    for (int iBullet = 0;iBullet < bulletShot.size() && iBullet<numBullets;iBullet++)  //////////////change
    {
        if(difftime(temp,bulletShot[iBullet].lifeTime)<10 && bulletShot[iBullet].isActive)
            gameWindow.draw(bulletShot[iBullet].circle);
        else
            bulletShot[iBullet].isActive=false;
    }
    for (int  itmaze = 0; itmaze < quad1.size(); itmaze += 4)
    {
        sf::RectangleShape rectangle;
        float x1 = quad1[itmaze][2];
        float y1 = quad1[itmaze][3];
        float x2 = quad1[itmaze + 1][0];
        float y2 = quad1[itmaze + 1][1];
        float x3 = quad1[itmaze][0];
        float y3 = quad1[itmaze][1];
        float x4 = quad1[itmaze+2][2];
        float y4 = quad1[itmaze+2][3];
        float x5 = quad1[itmaze+2][0];
        float y5 = quad1[itmaze+2][1];
        float l = sqrt((x3-x1)*(x3-x1) + (y3-y1)*(y3-y1));
        float b = sqrt((x5-x4)*(x5-x4) + (y5-y4)*(y5-y4));
        rectangle.setSize(sf::Vector2f(b,l));
        rectangle.setFillColor(sf::Color::Blue);
        sf::FloatRect bounds = rectangle.getLocalBounds();
		rectangle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        rectangle.setPosition((x1+x2)/2.0,(y1+y2)/2.0);
        gameWindow.draw(rectangle);
    }
    gameWindow.display();
}

bool Game::decideWinner()
{
    if(user.isDestroyed)
        cout<<"computer wins"<<endl;

    if(computer.isDestroyed)
        cout<<"User wins"<<endl;


}
