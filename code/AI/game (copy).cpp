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

queue<pair<double,vector<int> > > gameQueue ; //to produce a lag of 5 frames
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Game::Game():gameWindow(sf::VideoMode(window_height, window_width), "Game Window->"+tmpForWindow)
{
    numOfRounds=0;
    maxNumOfRounds=5;
    //.setActive(false);
    playerTanks.resize(numOfUsers);// Change this to number of players
    //    bulletsInGame.resize(0);

    /*for (int i = 0;i < 255;i++)
		keystate[i] = 0;*/

 maze1 = {{0,100,200,100},{300,0,300,300},{500,0,500,100},{500,300,500,450},{400,450,600,450},{0,400,200,400},{100,500,100,600},{300,500,300,600},{500,500,600,500}};


    for (vector <vector <float> > :: iterator itMaze = maze1.begin(); itMaze != maze1.end(); ++itMaze)
    {
        /////////////////////////////////////////
        quad1.push_back(*itMaze);
        /////////////////////////////////////////

        /// divide lines as per quadrant

      /*  if ((*itMaze)[1] == (*itMaze)[3])   // horizontal wall
        {
          mazeHoriWalls.push_back(*itMaze);
          if ((*itMaze)[1] < (window_width)/2.0)    /// upperhalf
          {
            if ((*itMaze)[2] >= (window_height)/2.0 || (*itMaze)[0] >= (window_height)/2.0)
            {
              quad1.push_back(*itMaze);
            }
            if ((*itMaze)[2] < (window_height)/2.0 || (*itMaze)[0] < (window_height)/2.0)
            {
              quad2.push_back(*itMaze);
            }
            horiUpper.push_back(*itMaze);

          }
          else
          {
            if ((*itMaze)[2] >= (window_height)/2.0 || (*itMaze)[0] >= (window_height)/2.0)
            {
              quad4.push_back(*itMaze);
            }
            if ((*itMaze)[2] < (window_height)/2.0 || (*itMaze)[0] < (window_height)/2.0)
            {
              quad3.push_back(*itMaze);
            }
            horiLower.push_back(*itMaze);

          }
        }
        else if ((*itMaze)[0] == (*itMaze)[2])    // vertical wall
        {
          mazeHoriWalls.push_back(*itMaze);
          if ((*itMaze)[0] >= (window_height)/2.0)    /// Righthalf
          {
            if ((*itMaze)[1] < (window_width)/2.0 || (*itMaze)[3] < (window_width)/2.0)
            {
              quad1.push_back(*itMaze);
            }
            if ((*itMaze)[1] >= (window_width)/2.0 || (*itMaze)[3] >= (window_width)/2.0)
            {
              quad4.push_back(*itMaze);
            }
            vertRight.push_back(*itMaze);
          }
          else
          {
            if ((*itMaze)[1] < (window_width)/2.0 || (*itMaze)[3] < (window_width)/2.0)
            {
              quad2.push_back(*itMaze);
            }
            if ((*itMaze)[1] >= (window_width)/2.0 || (*itMaze)[3] >= (window_width)/2.0)
            {
              quad3.push_back(*itMaze);
            }
            vertLeft.push_back(*itMaze);
          }
        }*/
  }

    //  vector <float> posTank = {12.0,0.0};   //right now only one tank not using sf::vectorf for now but later
    //  playerTanks[0].setTankPosition(posTank);
    //  posTank={0.0,12.0};
    //  playerTanks[1].setTankPosition(posTank);
    //  t1.setLenCanon(0.1);
    //  t1.setTankSpeed(0.05);
   //   vector<float> colorTank;
   //   colorTank.resize(3);
   //   colorTank[0] = 1.0;
   //   colorTank[1] = 0.5;
   //   colorTank[2] = 1.5;
   //   t1.setTankColour(colorTank);
   //   t1.setTankRadius(0.05);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    int frame=0;
    int sendCount=0;
    while (gameWindow.isOpen())
    {
        //processEvents();
        //update(deltaKeyPress);
        //render();
        
        if(!decideWinner())
        {
            processEvents();
            timeSinceLastUpdate += clock.restart();
            while (timeSinceLastUpdate.asSeconds() > timePerframe)
            {
                timeSinceLastUpdate -= sf::seconds(timePerframe);
                processEvents();
                if(sendCount==4 && programState==3)
                {
                    if(!playerTanks[myUserId].isDestroyed)
                        broadCastdata();
                    
                    sendCount=0;
                }
                gameQueue.push(make_pair(timePerframe,playerTanks[myUserId].keystate));                 //maximum five size is there can handle with simple
                //cout<<frame<<endl;
                if(frame>10)
                {
                  update(gameQueue.front());
                  gameQueue.pop();
                  frame--;              //rendering starts at five frames late
                }
                frame++;
                sendCount++;
            }
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
                        playerTanks[myUserId].keystate[2] = 1;
                        break;
                    }
            case  sf::Keyboard::Right:
                    {
                        playerTanks[myUserId].keystate[3] =  1;
                        break;
                    }
            case  sf::Keyboard::Up   :
                    {
                        playerTanks[myUserId].keystate[0] = 1;
                        break;
                    }
            case  sf::Keyboard::Down :
                    {
                       playerTanks[myUserId].keystate[1] = 1;
                        break;
                    }
            case sf::Keyboard::M   :
                    {
                        playerTanks[myUserId].keystate[4] = 1;
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
                        playerTanks[myUserId].keystate[2] = 0;
                        break;
                    }
            case  sf::Keyboard::Right:
                    {
                        playerTanks[myUserId].keystate[3] =  0;
                         break;
                    }
            case  sf::Keyboard::Up   :
                    {
                        playerTanks[myUserId].keystate[0] =  0;
                          break;
                    }
            case  sf::Keyboard::Down :
                    {
                        playerTanks[myUserId].keystate[1] = 0;
                        break;
                    }
            case sf::Keyboard::M   :
                    {
                        playerTanks[myUserId].keystate[4] = 0;
                        break;
                    }
        }
    /*
        vector <string> tempT;
        char temp2[100];

        int i2 ;
        sprintf(temp2,"%d",myUserId); //userId
        tempT.push_back(temp2);

        sprintf(temp2,"%d",1); // Instruction code
        tempT.push_back(temp2);

        sprintf(temp2,"%d",mostRecentMsgNum[myUserId]); // Serial Code
        tempT.push_back(temp2);

        sf::Vector2f tempPos=this->playerTanks[myUserId].myTank.getPosition();
        float i = tempPos.x;
        sprintf(temp2,"%f",i); // X
        tempT.push_back(temp2);
        i = tempPos.y;
        sprintf(temp2,"%f",i); // Y
        tempT.push_back(temp2);
        i = this->playerTanks[myUserId].myTank.getRotation();;
        sprintf(temp2,"%f",i);  // Angle
        tempT.push_back(temp2);
        i = 0.011;
        sprintf(temp2,"%f",i); // Velocity
        tempT.push_back(temp2);

        sendQueueItem toSend;
        toSend.destPort=remPort;
        toSend.isAck=false;
        toSend.message=makeMessage(tempT);

        sendQueue.push( toSend );*/

    }

}

void Game::update(pair<double,vector<int> > updateSet)
{
    if(!playerTanks[myUserId].isDestroyed)
    {
        this->playerTanks[myUserId].nexPosTank(updateSet.second,updateSet.first);
        this->playerTanks[myUserId].handleCollisionOuterWall();
        this->playerTanks[myUserId].destroyTank();
    }
    
    for (int iBullet = 0;iBullet < bulletShot.size() && iBullet< numBullets;iBullet++)  //////////////change
    {
        if(bulletShot[iBullet].isActive)
        {
            bulletShot[iBullet].nexPosBullet();
            bulletShot[iBullet].handleCollisionOuterWall();
            bulletShot[iBullet].handleCollisionMazeWall();
        }
    }
}


void Game::broadCastdata()
{

    vector <string> tempT;
    char temp2[100];

    int i2 ;
    sprintf(temp2,"%d",myUserId); //userId
    tempT.push_back(temp2);

    sprintf(temp2,"%d",1);        // Instruction code
    tempT.push_back(temp2);

    sprintf(temp2,"%d",mostRecentMsgNum[myUserId]); // Serial Code
    mostRecentMsgNum[myUserId]++;
    tempT.push_back(temp2);

    sf::Vector2f tempPos=this->playerTanks[myUserId].myTank.getPosition();
    float i = tempPos.x;
    sprintf(temp2,"%f",i);        // X
    tempT.push_back(temp2);
    i = tempPos.y;
    sprintf(temp2,"%f",i);        // Y
    tempT.push_back(temp2);
    i = this->playerTanks[myUserId].myTank.getRotation();;
    sprintf(temp2,"%f",i);        // Angle
    tempT.push_back(temp2);
    i = 0.011;
    //sprintf(temp2,"%f",i);        // Velocity
    tempT.push_back(temp2);
    for(int i=0;i<numOfUsers;i++)
    {
        if (i!=myUserId)
        {
            sendQueueItem toSend;
            toSend.destPort=userPort[i];        //rightnow for only one tank
            toSend.isAck=false;
            toSend.message=makeMessage(tempT);
            sendQueue.push( toSend );

        }
    }
}

void Game::render()     //maze rendering part via shapes remain
{
    gameWindow.clear();
    for(int i=0;i<numOfUsers;i++)
    {
        if(!playerTanks[i].isDestroyed)
            gameWindow.draw(playerTanks[i].myTank);
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

    for (int  itmaze = 0; itmaze < maze1.size(); ++itmaze)
    {
       sf::Vertex line[]=
        { //adjust  the opengl coordinates to correct the maze
            sf::Vertex(sf::Vector2f(maze1[itmaze][0],maze1[itmaze][1])),
            sf::Vertex(sf::Vector2f(maze1[itmaze][2],maze1[itmaze][3]))
        };
        gameWindow.draw(line,2,sf::Lines);      //use inbuilt line shape or rectangle for thick lines using sf::vectorf
    }
    gameWindow.display();
}

void Game::keyboardHandle(unsigned char key, int x, int y)
{
    // Do something to the vector : vec[int(key)] = 1
    if (key == 'm' || key == 'M')
    {
    playerTanks[myUserId].keystate[4] = 1;	// for shooting...create a new bullet now from the end point of cannon
    }
    /// modify for powerups
}

void Game::unPressHandle(unsigned char key,int x ,int y)
{
    if ((key == 'm') || (key == 'M'))
    {
      playerTanks[myUserId].keystate[4] = 0;
    /// now shoot
    }
}

void Game::updateOtherTanks(int userId,float x,float y,float angle)
{
    //cout<<"Hello"<<endl;
    if(userId<numOfUsers)
    {
      //  cout<<"UserId"<<userId<<endl;
        vector<float> temp;
        temp.push_back(x);
        temp.push_back(y);
      //  cout<<"Really"<<endl;
        this->playerTanks[userId].setTankPosition(temp);
      //  cout<<"Really2"<<endl;
        this->playerTanks[userId].setTankAngle(angle);
      //  cout<<"Updating..."<<userId<<endl;
    }
    else
    {
        cout<<"Invalid userId:"<<userId<<endl;
    }
    //cout<<"Going back"<<endl;

}

void Game::destroyOtherTanks(int userId,int bulletId,float XCord,float YCord)
{
    if(userId<numOfUsers && bulletId<numBullets && bulletId<bulletShot.size())
        {
            playerTanks[userId].Health=playerTanks[userId].Health-1;
            if(playerTanks[userId].Health<0)
            {
                playerTanks[userId].isDestroyed=true;
            }
            bulletShot[bulletId].isActive=false;
        }
    else
    {
        cout<<"Invalid userId"<<endl;
        cout<<"Invalid bullet Id"<<endl;
    }
}

bool Game::decideWinner()
{
    int numOfActiveTanks=0;
    for(int iter=0;iter<numOfUsers;iter++)
    {
        if(!playerTanks[iter].isDestroyed)
            numOfActiveTanks++;
    }
    if(numOfActiveTanks==1)
    {
        for(int i=0;i<numOfUsers;i++)
        {
            if(!playerTanks[i].isDestroyed)
            {
                cout<<"\n\n\n\n\n\n\n\n\nThis player is the winner:"<<i<<endl;
                return true;                
                //exit(0);
            }
        }
    }
}
