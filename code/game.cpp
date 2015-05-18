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
    ///
    time(&timeLastPowerUp);
    activePowerUpId=0;
    //not visible
    gameWindow.setVisible(false);
    //
    numOfRounds=0;
    maxNumOfRounds=5;
    playerTanks.resize(numOfUsers);// Change this to number of players

    backGroundTex=new sf::Texture;
    backGroundTex->loadFromFile(string(res)+"grass.png");
    backGroundTex->setRepeated(true);

    //jeetueveinig
    vicBackground=new sf::Texture;
    vicBackground->loadFromFile(string(res)+"winner.jpg");
    lossBackground=new sf::Texture;
    lossBackground->loadFromFile(string(res)+"game-over.jpg");
    mazeWalls =  new sf::Texture;
    //mazeWalls->setSmooth(true);
    mazeWalls->loadFromFile(string(res)+"mazewallsmall.png");
    mazeWalls->setRepeated(true);

    //winnerId=0;
    //mazeBrick.setTexture(mazeWalls);
    //this->mazeBrick.setSize(sf::Vector2f(float(mazeWalls->getSize().x),float(mazeWalls->getSize().y)));
   // sf::FloatRect bounds = mazeBrick.getLocalBounds();
    //mazeBrick.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    //

    this->background.setSize(sf::Vector2f(float(backGroundTex->getSize().x),float(backGroundTex->getSize().y)));

    this->background.setTexture(backGroundTex);
     maze1 = {{0,200,150,200},{300,0,300,300},{500,0,500,100},{500,300,500,450},{400,450,600,450},{0,400,200,400},{100,500,100,600},{300,500,300,600},{500,500,600,500}};


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


}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    int frame=0;
    int sendCount=0;



    while (gameWindow.isOpen())
    {

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

        render();
        }
        else
        {

            cout<<"winner is decided"<<endl;
            if(winnerId==myUserId)
            {
                this->endDisplay.setTexture(*vicBackground);
                this->endDisplay.setScale(float(window_width)/float(1600.0),float(window_height)/float(1200.0) );
            }
            else
            {
                this->endDisplay.setTexture(*lossBackground);
                this->endDisplay.setScale(float(window_width)/float(640),float(window_height)/float(361));
            }
            //this->endDisplay->setorigin()
            //this->background.setPosition(window_width/2.0 , window_height/2.0);
            render2();
        }

    }
}

void Game::render2()
{
    gameWindow.clear();
    gameWindow.draw(this->endDisplay);
    gameWindow.display();

}

void Game::processEvents()  //add keys effects for own tank dont change directly only after proper interpolation
{
    //check for acttive users
    time_t tempTime;
    time(&tempTime);
    for(int iter=0;iter<numOfUsers;iter++)
    {
        if(iter!=myUserId)
        {
            if(difftime(tempTime,lastUserActivity[iter])>connectionTimeOut)
            {
                cout<<"User "<<iter<<" Has lost the connection "<<endl;
                userActive[iter]=false;
                playerTanks[iter].isLost=true;
            }
            else
            {
        //                cout<<"User "<<iter<<" Has joined back"<<endl;
                userActive[iter]=true;
                playerTanks[iter].isLost=false;
            }
        }
    }
    //
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
            case sf::Event::Resized:
                {
                     //window_width=event.size.width;
                     //window_height=event.size.height;
                    //this->gameWindow.setSize(sf::Vector2u(window_width,window_height));
                    //this->background.setSize(sf::Vector2f(window_width,window_height));
                    //this->background.setPosition(this->gameWindow.mapPixelToCoords(sf::Vector2i(0, 0)));
                    //this->background.setTexture(backGroundTex);
                    //this->background.setScale(1,1);
                    /*this->background.setScale(
                        float(event.size.width) / float(this->background.getTexture()->getSize().x),
                        float(event.size.height) / float(this->background.getTexture()->getSize().y));*/
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
    }

}

void Game::update(pair<double,vector<int> > updateSet)
{
    if(!playerTanks[myUserId].isDestroyed)
    {
        this->playerTanks[myUserId].nexPosTank(updateSet.second,updateSet.first);
        this->playerTanks[myUserId].handleCollisionOuterWall();
        this->playerTanks[myUserId].handleCollisionMazeWall();
        this->playerTanks[myUserId].handleCollisionMazeWallVertices();
        this->playerTanks[myUserId].destroyTank();
    }
        //jeetuanim

    for(int i=0;i<numOfUsers;i++)
    {
        this->playerTanks[i].explode->update(sf::seconds(timePerframe));
        if(playerTanks[i].isDisplay)
            playerTanks[i].animDur+=timePerframe;

        playerTanks[i].handleHealth();

    }

    //

    for (int iBullet = 0;iBullet < bulletShot.size() && iBullet< numBullets;iBullet++)  //////////////change
    {
        if(bulletShot[iBullet].isActive)
        {
            bulletShot[iBullet].nexPosBullet();
            bulletShot[iBullet].handleCollisionOuterWall();
            bulletShot[iBullet].handleCollisionMazeWall();
        }
    }

    //Nishant
    time_t tempTime;
    time(&tempTime);
    if(difftime(tempTime,powerUpVector[activePowerUpId].timeActivated)>5)
    {
        powerUpVector[activePowerUpId].isActive=false;
    }
    if(difftime(tempTime,timeLastPowerUp)>10 && numPowerUps<powerUpVector.size())
    {
        time(&timeLastPowerUp);
        cout<<"Activating powerUps"<<endl;
        activePowerUpId=numPowerUps;
        time(&powerUpVector[numPowerUps].timeActivated);
        powerUpVector[numPowerUps].isActive=true;

        if(numPowerUps<powerUpVector.size())
            numPowerUps++;
        // Send network message on powerUp Pickup
    }

    for(int i=0;i<numOfUsers;i++)
    {
        sf::FloatRect boundsTank = playerTanks[i].myTank.getGlobalBounds();
        if( powerUpVector[activePowerUpId].isActive && boundsTank.contains(powerUpVector[activePowerUpId].centrePowerUpX,powerUpVector[activePowerUpId].centrePowerUpY) )
        {
            powerUpVector[activePowerUpId].isActive=false;
            cout<<"User "<<i<< " picked up the powerUp:"<<powerUpVector[activePowerUpId].powerUpType<<endl;
            switch (powerUpVector[activePowerUpId].powerUpType)
            {
                case 0:// shield
                    {
                        playerTanks[i].sheildActive=true;
                        time(&playerTanks[i].sheildActiveTime);
                        break;
                    }
                case 1:// Health
                    {
                        playerTanks[i].Health=5;
                        break;
                    }
                case 2:// Armour
                    {
                        playerTanks[i].Bullets=20;
                        break;
                    }
                default:
                    {
                        cout<<"InValid powerUp Id"<<endl;
                        break;
                    }
            }
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
        if (i!=myUserId && userActive[i])
        {
            sendQueueItem toSend;
            toSend.destPort=userPort[i];        //rightnow for only one tank
            toSend.isAck=false;
            toSend.message=makeMessage(tempT);
            toSend.message=attachCrucial(toSend.message);
            toSend.destIp=userIP[i];
            sendQueue.push( toSend );
        }
    }
}

void Game::render()     //maze rendering part via shapes remain
{
    gameWindow.clear();

    for(int y=0;y<5;y++)
    {
        for(int x=0;x<5;x++)
        {
            background.setPosition(background.getSize().x*x,background.getSize().y*y);
                    gameWindow.draw(background);
        }
    }
    ///
    if(powerUpVector[activePowerUpId].isActive)
            gameWindow.draw(powerUpVector[activePowerUpId].myPowerUp);
    ///
    for(int i=0;i<numOfUsers;i++)
    {
        if(!playerTanks[i].isDestroyed)
        {
            if(!playerTanks[i].isLost)
            {
                playerTanks[i].myTank.setColor(sf::Color(255, 255, 255, 255)); // opaque
                gameWindow.draw(playerTanks[i].myTank);
            }
            else
            {
                playerTanks[i].myTank.setColor(sf::Color(255, 255, 255, 128)); // half transparent);
                gameWindow.draw(playerTanks[i].myTank);
            }
            gameWindow.draw(playerTanks[i].lifeBg);
            gameWindow.draw(playerTanks[i].lifeFill);
            gameWindow.draw(playerTanks[i].Name);
        }
        else
        {
            playerTanks[i].explode->setPosition(playerTanks[i].myTank.getPosition()-sf::Vector2f(playerTanks[i].w,playerTanks[i].h));
            playerTanks[i].explode->play(playerTanks[i].explosion);
            playerTanks[i].isDisplay=true;
            if(playerTanks[i].animDur<=1.0)        //since dying for once so no chance again resetting animDur
            {
                gameWindow.draw(*playerTanks[i].explode);
            }
        }
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
        //rectangle.setFillColor(sf::Color::Blue);
        sf::FloatRect bounds = rectangle.getLocalBounds();
		rectangle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        rectangle.setPosition((x1+x2)/2.0,(y1+y2)/2.0);
        //jeetueve
        /*float xBreadth= (x1+x2)/2.0-b/2.0;
        float yLength= (y1+y2)/2.0-l/2.0;
        for(int y=0;y<l/10;y++)
        {
            for(int x=0;x<b/10;b++)
            {
                mazeBrick.setPosition(xBreadth+10*x,yLength+10*y);
                    gameWindow.draw(mazeBrick);
            }
        */
        //
        rectangle.setTexture(mazeWalls);
        gameWindow.draw(rectangle);
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
        this->playerTanks[userId].setTankPosition(temp,angle);
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
                winnerId=i;
                return true;
                //exit(0);
            }
        }
    }
}
