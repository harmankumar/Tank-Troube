#include "tank.h"

//CAUTION: AngleXAxis is positive in clockwise direction

Tank::Tank():tankTexture(),myTank(),lifeBg(),lifeFill(sf::Vector2f())
{
    //vector <int> keystate(255);
    ///
    //myTank.setColor(sf::Color(255, 255, 255, 128)); // half transparent);
    currentPowerUp=-1;
    time(&sheildActiveTime);
    sheildActive=false;
    ///
    //jeetu
    animDur=0.0;
    isDisplay=false;
    explodTex.loadFromFile("data/Explosion.png");
    explosion.setSpriteSheet(explodTex);
    for(int j=0;j<1;j++)
        {
            for(int i=0;i<15;i++)
            {
                explosion.addFrame(sf::IntRect(i*128,j*128,128,128));
            }
        }
    explode=new AnimatedSprite(sf::seconds(0.05),true,false);
    //
    isDestroyed=false;
    isLost=false;
    for(int i=0;i<255;i++)
    {
        previt[i]= false;
        this->keystate.push_back(0);
    }
    //jeetu
    fontMgr = new sf::Font;
    fontMgr->loadFromFile("data/Ubuntu-C.ttf");

    lifeBg.setSize(sf::Vector2f(45,10));
    lifeFill.setSize(sf::Vector2f(45,10));
    lifeBg.setOutlineColor(sf::Color::Black);
    lifeBg.setOutlineThickness(1);
    lifeBg.setFillColor(sf::Color::Transparent);

    lifeFill.setFillColor(sf::Color::Red);
    Name.setString("Tank i");
    Name.setColor(sf::Color::Blue);
    Name.setCharacterSize(15);
    Name.setFont(*fontMgr);
    //
    tankTexture.loadFromFile(string(res)+"tank.png");
    myTank.setTexture(tankTexture);
    myTank.setRotation(0.0);
    sf::FloatRect bounds = myTank.getLocalBounds();
    myTank.setOrigin(bounds.width/2.f,bounds.height/2.f);
    //nozzleTexture.loadFromFile(string(res)+"nozzle.jpeg");
    // myNozzle.setTexture(nozzleTexture);
    ///////////////////////// only when collision with tanks..........change
    sf::FloatRect boundsTank = myTank.getGlobalBounds();  ///it gives top left corner and width and height
    // save the 4 corner points in a vector.... do .left .right then widht and height
    //if any of these 4 coordinates lies on the outer wall then yes collision occurs also keep note with which wall collision is occuring
    //float w,h,initAngle;//halfDiag;
    w = boundsTank.width;     //towards Y axis
    h = boundsTank.height;      //towards X axis
    initAngle = atan(w/h);
    halfDiag = sqrt(w*w + h*h) / 2.0;
    /// numbering bottom right as 1 then clock wise
    xCordVertRect.resize(4);
    yCordVertRect.resize(4);
    angleRectRad.resize(4);
    ////////////////// check these in case facing of tanks is different
    xCordVertRect = {halfDiag*cos(initAngle) + centreX , halfDiag*cos(Pi - initAngle) + centreX , halfDiag*cos(Pi + initAngle) + centreX , halfDiag *cos(2*Pi - initAngle) + centreX};
    yCordVertRect = {halfDiag*sin(initAngle) + centreY , halfDiag*sin(Pi - initAngle) + centreY , halfDiag*sin(Pi + initAngle) + centreY , halfDiag *sin(2*Pi - initAngle) + centreY};
    angleRectRad = {initAngle, Pi - initAngle, Pi + initAngle , 2*Pi - initAngle};

      canonEndX = (h / 2.0 + 5.0) * cos(angleXaxis) + centreX;
    canonEndY = (h / 2.0 + 5.0) * cos(angleXaxis) + centreY;

    collisionTopWall = 0;
    collisionRightWall = 0;
    collisionLeftWall = 0;
    collisionBottomWall = 0;

    collisionMazeWallTop = 0;
    collisionMazeWallBottom = 0;
    collisionMazeWallRight = 0;
    collisionMazeWallLeft = 0;

        collisionTopLeftPoint = 0;
    collisionTopRightPoint = 0;
    collisionBottomLeftPoint = 0;
    collisionBottomRightPoint = 0;
     rightCentreUp = 0;
     rightCentreDown = 0;
   leftCentreUp = 0;
   leftCentreDown = 0;
   topCentreRight = 0;
   topCentreLeft = 0;
   bottomCentreRight = 0;
   bottomCentreLeft = 0;


    ///////////////////////////////////////////////////////equations of each side of tank
  xCoeffVertRect = {yCordVertRect[1]-yCordVertRect[0],yCordVertRect[2]-yCordVertRect[1],yCordVertRect[3]-yCordVertRect[2],yCordVertRect[0]-yCordVertRect[3]};
  yCoeffVertRect = {xCordVertRect[0]-xCordVertRect[1],xCordVertRect[1]-xCordVertRect[2],xCordVertRect[2]-xCordVertRect[3],xCordVertRect[3]-xCordVertRect[0]};
  float c0 = (-1)*xCordVertRect[0]*(yCordVertRect[1]-yCordVertRect[0]) + yCordVertRect[0]*(xCordVertRect[1]-xCordVertRect[0]);
  float c1 = (-1)*xCordVertRect[1]*(yCordVertRect[2]-yCordVertRect[1]) + yCordVertRect[1]*(xCordVertRect[2]-xCordVertRect[1]);
  float c2 = (-1)*xCordVertRect[2]*(yCordVertRect[3]-yCordVertRect[2]) + yCordVertRect[2]*(xCordVertRect[3]-xCordVertRect[2]);
  float c3 = (-1)*xCordVertRect[3]*(yCordVertRect[0]-yCordVertRect[3]) + yCordVertRect[3]*(xCordVertRect[0]-xCordVertRect[3]);
  constVertRect = {c0,c1,c2,c3};
  ////////////////////////////////////////////////////////

    speed=0.15;
    angleXaxis=0;
    radius = 0.1;
    Color.resize(3);
    Health = 5;
    Bullets = 10;
    Missiles = 0;
    Nitrous = 0;
    GMissiles = 0;
    Nuke = 0;
    Shield = 0;
}



Tank::~Tank()
{
  cout<<"Tank Desroyed"<<endl;
}

bool Tank::cannotMoveForward()    // true if cannot move forward
{
  float angleXaxisDeg = angleXaxis * 180 / Pi;
  if ((collisionRightWall == 1 || collisionMazeWallLeft == 1) && (angleXaxisDeg <= 90 || angleXaxisDeg >= 270))
  {
    ///cout<<"1\n";
    return 1;
  }
  if ((collisionLeftWall == 1 || collisionMazeWallRight == 1) && (angleXaxisDeg >= 90 && angleXaxisDeg <= 270))
    return 1;
  if ((collisionTopWall == 1 || collisionMazeWallBottom == 1) && (angleXaxisDeg >= 180 && angleXaxisDeg <= 360))
  {
    //cout<<"dfbjsdbjsbvjsbvjsvjsnjnvsjnvs\n";
    return 1;
  }
  if ((collisionBottomWall == 1 || collisionMazeWallTop == 1) && (angleXaxisDeg >= 0 && angleXaxisDeg <= 180))
    return 1;
  else
    return 0;
}

bool Tank::cannotMoveBackward()   // true if cannot move backward
{
  float angleXaxisDeg = angleXaxis * 180 / Pi;
  if ((collisionRightWall == 1 || collisionMazeWallLeft == 1) && (angleXaxisDeg >= 90 && angleXaxisDeg <= 270))
    return 1;
  if ((collisionLeftWall == 1 || collisionMazeWallRight == 1) && (angleXaxisDeg <= 90 || angleXaxisDeg >= 270))
    return 1;
  if ((collisionTopWall == 1 || collisionMazeWallBottom == 1) && (angleXaxisDeg >= 0 && angleXaxisDeg <= 180))
    return 1;
  if ((collisionBottomWall == 1 || collisionMazeWallTop == 1) && (angleXaxisDeg >= 180 && angleXaxisDeg <= 360))
    return 1;
  else
    return 0;
}


bool Tank::cannotRotateAntiClock()    // true if cannot rotate
{
  float angleXaxisDeg = angleXaxis * 180 / Pi;
  if ((collisionRightWall == 1 ) && (angleXaxisDeg <= 90))
    return 1;
  if ((collisionLeftWall == 1 ) && (angleXaxisDeg >= 180 && angleXaxisDeg <= 270))
    return 1;
  if ((collisionTopWall == 1 ) && (angleXaxisDeg >= 270 && angleXaxisDeg <= 360))
    return 1;
  if ((collisionBottomWall == 1 ) && (angleXaxisDeg >= 90 && angleXaxisDeg >= 180))
    return 1;
  else
    return 0;
}

bool Tank::cannotRotateClock()    // true if cannot rotate
{
  float angleXaxisDeg = angleXaxis * 180 / Pi;
  if ((collisionRightWall == 1 ) && (angleXaxisDeg >= 270))
    return 1;
  if ((collisionLeftWall == 1 ) && (angleXaxisDeg >= 90 && angleXaxisDeg <= 180))
    return 1;
  if ((collisionTopWall == 1 ) && (angleXaxisDeg >= 180 && angleXaxisDeg <= 270))
    return 1;
  if ((collisionBottomWall == 1 ) && (angleXaxisDeg >= 0 && angleXaxisDeg <= 90))
    return 1;
  else
    return 0;
}

bool Tank::cannotRotateAntiClock2()
{
  if (collisionMazeWallTop == 1 && topCentreLeft == 1)
    return 1;
  if (collisionMazeWallBottom == 1 && bottomCentreRight == 1)
    return 1;
  if (collisionMazeWallLeft == 1 && leftCentreUp == 1)
    return 1;
  if (collisionMazeWallRight == 1 && rightCentreDown == 1)
    return 1;
  else
    return 0;
}

bool Tank::cannotRotateClock2()
{
  if (collisionMazeWallTop == 1 && topCentreRight == 1)
    return 1;
  if (collisionMazeWallBottom == 1 && bottomCentreLeft == 1)
    return 1;
  if (collisionMazeWallLeft == 1 && leftCentreDown == 1)
    return 1;
  if (collisionMazeWallRight == 1 && rightCentreUp == 1)
    return 1;
  else
    return 0;
}

bool Tank::cannotRotate2()    // true if can rotate
{
  if (collisionTopLeftPoint == 1 || collisionTopRightPoint == 1 || collisionBottomLeftPoint == 1 || collisionBottomRightPoint == 1)
    return 1;
  else
    return 0;
}


bool Tank::cannotMoveForward2()
{
  float angleXaxisDeg = angleXaxis * 180 / Pi;
  if (collisionTopLeftPoint == 1 && angleXaxisDeg <= 90)
    return 1;
  if (collisionTopRightPoint == 1 && (angleXaxisDeg >= 90 && angleXaxisDeg <= 180))
    return 1;
  if (collisionBottomLeftPoint == 1 && (angleXaxisDeg >= 270))
    return 1;
  if (collisionBottomRightPoint == 1 && (angleXaxisDeg >= 180 && angleXaxisDeg <= 270))
    return 1;
  else
    return 0;
}


bool Tank::cannotMoveBackward2()
{
  float angleXaxisDeg = angleXaxis * 180 / Pi;
  if (collisionTopLeftPoint == 1 && (angleXaxisDeg >= 180 && angleXaxisDeg <= 270))
    return 1;
  if (collisionTopRightPoint == 1 && (angleXaxisDeg >= 270))
    return 1;
  if (collisionBottomLeftPoint == 1 && (angleXaxisDeg >= 90 && angleXaxisDeg <= 180))
    return 1;
  if (collisionBottomRightPoint == 1 && (angleXaxisDeg <= 90))
    return 1;
  else
    return 0;
}

void Tank::nexPosTank(vector<int> keyboardKeyList,double timeInterval)   // 0 for up, 1 for down, 2 for left, 3 for right
{
  //cout<<"x "<<xCordVertRect[0]<<" "<<xCordVertRect[1]<<" "<<xCordVertRect[2]<<" "<<xCordVertRect[3]<<endl;
  //cout<<"y "<<yCordVertRect[0]<<" "<<yCordVertRect[1]<<" "<<yCordVertRect[2]<<" "<<yCordVertRect[3]<<endl;
    int i=0;
    bool temPit[255];
    for(int iInit=0;iInit<255;iInit++)
      temPit[iInit]= false;

    for (vector<int> ::iterator it = keyboardKeyList.begin(); it != keyboardKeyList.end(); ++it , i++)
    {

      int keyboardKey = *it;
      if (keyboardKey == 1)     // 1 means that key i has been pressed
      {
        if(previt[i] == false)      //// change
        {
          if (i == 0)//up
          {
            if (!cannotMoveForward() && !cannotMoveForward2()){
              //cout<<"here\n";
            myTank.move(speed*deltaKeyPress*cos(angleXaxis),speed*deltaKeyPress*sin(angleXaxis));
            //updateCordRect(speed*deltaKeyPress*cos(angleXaxis),speed*deltaKeyPress*sin(angleXaxis));
            for (int iter = 0;iter < xCordVertRect.size();iter++)
      {
        xCordVertRect[iter] = xCordVertRect[iter] + speed*deltaKeyPress*cos(angleXaxis);
        yCordVertRect[iter] = yCordVertRect[iter] + speed*deltaKeyPress*sin(angleXaxis);
      }
      }
          }
          else if (i == 1)//down
          {
            if (!cannotMoveBackward() && !cannotMoveBackward2()){
            myTank.move(-speed*deltaKeyPress*cos(angleXaxis),-speed*deltaKeyPress*sin(angleXaxis));
            //updateCordRect(speed*deltaKeyPress*cos(angleXaxis),speed*deltaKeyPress*sin(angleXaxis));
            for (int iter = 0;iter < xCordVertRect.size();iter++)
      {
        xCordVertRect[iter] = xCordVertRect[iter] - speed*deltaKeyPress*cos(angleXaxis);
        yCordVertRect[iter] = yCordVertRect[iter] - speed*deltaKeyPress*sin(angleXaxis);
      }
      }
          }
          else if (i == 2)//left
          {
            if (!cannotRotateAntiClock() && !cannotRotateAntiClock2() && !cannotRotate2()){
            myTank.rotate(-2.0);
            /// update
            for (int iter = 0;iter < angleRectRad.size();iter++)
            {
              angleRectRad[iter] = angleRectRad[iter] - (2.0 * Pi / 180.0);
              xCordVertRect[iter] = halfDiag*cos(angleRectRad[iter]) + centreX;
              yCordVertRect[iter] = halfDiag*sin(angleRectRad[iter]) + centreY;
            }
          }
          }
          else if (i == 3)//right
          {
            if (!cannotRotateClock() && !cannotRotateClock2() && !cannotRotate2()){
            myTank.rotate(2.0);
            // update
            for (int iter = 0;iter < angleRectRad.size();iter++)
            {
              angleRectRad[iter] = angleRectRad[iter] + (2.0 * Pi / 180.0);
              xCordVertRect[iter] = halfDiag*cos(angleRectRad[iter]) + centreX;
              yCordVertRect[iter] = halfDiag*sin(angleRectRad[iter]) + centreY;
            }
          }
          }

        else if (i == 4)                      //////////change
          {
            // shoot only once
            // create a new bullet with its position as centreX and centreY of bullet

            bulletShot[numBullets].centreBulletX = canonEndX;     /// make it leave from the canon's end point
            bulletShot[numBullets].centreBulletY = canonEndY;
            //cout<<"centreX "<<bShot.centreBulletX<<endl;
            //cout<<"centreY "<<bShot.centreBulletY<<endl;
                  /// make it leave from the canon's end point
            bulletShot[numBullets].angleBullet = angleXaxis;
            (bulletShot[numBullets].myBullet).setPosition(canonEndX,canonEndY);
            (bulletShot[numBullets].myBullet).setRotation(angleXaxis*180.0/Pi);
            (bulletShot[numBullets].circle).setPosition(canonEndX,canonEndY);
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
                sf::FloatRect boundsTank = myTank.getGlobalBounds();
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

            /// now i need to PUSH it in the vector of bullets
            //in the render function we iterate through this vector
            numBullets++;

            
          }
          temPit[i]=true;
        }
        if(previt[i]==true)// long press /// change
        {
          /// Do it for small step
            if (i == 0)//up....for moving forward
            {
              if (!cannotMoveForward() && !cannotMoveForward2()){
                //cout<<"here\n";
                myTank.move(speed*smalldeltaKeyPress*cos(angleXaxis),speed*smalldeltaKeyPress*sin(angleXaxis));
                //updateCordRect(speed*smalldeltaKeyPress*cos(angleXaxis),speed*smalldeltaKeyPress*sin(angleXaxis));
                for (int iter = 0;iter < xCordVertRect.size();iter++)
        {
          xCordVertRect[iter] = xCordVertRect[iter] + speed*smalldeltaKeyPress*cos(angleXaxis);
          yCordVertRect[iter] = yCordVertRect[iter] + speed*smalldeltaKeyPress*sin(angleXaxis);
        }
        }
            }
            else if (i == 1)//down
            {
              if (!cannotMoveBackward() && !cannotMoveBackward2()){
                myTank.move(-speed*smalldeltaKeyPress*cos(angleXaxis),-speed*smalldeltaKeyPress*sin(angleXaxis));
              //updateCordRect(speed*smalldeltaKeyPress*cos(angleXaxis),speed*smalldeltaKeyPress*sin(angleXaxis));
              for (int iter = 0;iter < xCordVertRect.size();iter++)
        {
          xCordVertRect[iter] = xCordVertRect[iter] - speed*smalldeltaKeyPress*cos(angleXaxis);
          yCordVertRect[iter] = yCordVertRect[iter] - speed*smalldeltaKeyPress*sin(angleXaxis);
        }
        }
            }
            else if (i == 2)//left
            {
              if (!cannotRotateAntiClock() && !cannotRotateAntiClock2() && !cannotRotate2()){
                myTank.rotate(-0.5);
                //update
                for (int iter = 0;iter < angleRectRad.size();iter++)
              {
                angleRectRad[iter] = angleRectRad[iter] - (0.5 * Pi / 180.0);
                xCordVertRect[iter] = halfDiag*cos(angleRectRad[iter]) + centreX;
                yCordVertRect[iter] = halfDiag*sin(angleRectRad[iter]) + centreY;
              }
              }
            }
            else if (i == 3)//right
            {
              if (!cannotRotateClock() && !cannotRotateClock2() && !cannotRotate2()){
                myTank.rotate(0.5);
        //update
        for (int iter = 0;iter < angleRectRad.size();iter++)
              {
                angleRectRad[iter] = angleRectRad[iter] + (0.5 * Pi / 180.0);
                xCordVertRect[iter] = halfDiag*cos(angleRectRad[iter]) + centreX;
                yCordVertRect[iter] = halfDiag*sin(angleRectRad[iter]) + centreY;
              }
              }
            }
            else if (i == 4)
            {
              //no shooting on long press
            }

            temPit[i]=true;     ///change
        }
      }

      this->lifeBg.setPosition(myTank.getPosition().x,myTank.getPosition().y-50.0);
      this->lifeFill.setPosition(myTank.getPosition().x,myTank.getPosition().y-50.0);
      this->Name.setPosition(myTank.getPosition().x-50.0,myTank.getPosition().y-55.0);
    }
    /////////////change
    angleXaxis=Pi*myTank.getRotation()/180;
    sf::Vector2f temp=myTank.getPosition();
    centreX=temp.x;
    centreY=temp.y;
    canonEndX = (h / 2.0 + 5.0) * cos(angleXaxis) + centreX;
  canonEndY = (h / 2.0 + 5.0) * sin(angleXaxis) + centreY;
    updateEquationsRectEdges();
    for( int iFin=0;iFin<255;iFin++)
        previt[iFin]=temPit[iFin];
}

void Tank::draw(sf::RenderTarget &target)
{
    target.draw(this->myTank);
    target.draw(this->lifeBg);
    target.draw(this->lifeFill);
    target.draw(Name);

}

// Fucntion for sending and receiving tank parameters

void Tank::setTankRadius(float x)
{
  radius = x;
}

void Tank::setTankSpeed (float x)
{
  speed = x;
}

void Tank::setTankHealth(float setHealth)
{
  Health = setHealth;
}

void Tank::setTankPosition(vector<float> setPos,float angleTank)
{
//  cout<<"position setting"<<setPos.size()<<endl;
  centreX = setPos[0];
  centreY = setPos[1];
  sf::Vector2f temp;
  temp.x=setPos[0];
  temp.y=setPos[1];
  myTank.setPosition(temp);
  myTank.setRotation(angleTank*180/Pi);
  angleXaxis=angleTank;
  //////////////////////////////////////////////////////////////////////////////////////
  ///// for AI part we need to do this explicitily....given centreX and centreY and angleXaxis...set coordinates of all vertices
  for (int iter = 0;iter < 4;iter++)
  {
    if (iter == 0)
      angleRectRad[iter] = initAngle + angleXaxis;
    else if (iter == 1)
      angleRectRad[iter] = Pi - initAngle + angleXaxis;
    else if (iter == 2)
      angleRectRad[iter] = Pi + initAngle + angleXaxis;
    else
      angleRectRad[iter] =  2*Pi - initAngle + angleXaxis;
    xCordVertRect[iter] = halfDiag*cos(angleRectRad[iter]) + centreX;
    yCordVertRect[iter] = halfDiag*sin(angleRectRad[iter]) + centreY;
  }
    lifeBg.setPosition(temp.x,temp.y-50.0);
    lifeFill.setPosition(temp.x,temp.y-50.0);
    this->Name.setPosition(temp.x-50.0,temp.y-55.0 );
 // cout<<"Updating position for a tank"<<temp.x<<endl;
}

void Tank::setTankAngle(float angle)
{
   // cout<<"Updating angle for the tank"<<endl;
    this->angleXaxis=angle;
    myTank.setRotation(angle*180/Pi);
}

void Tank::setTankColour(vector<float> setColor)
{

  Color[0] = setColor[0];
  Color[1] = setColor[1];
  Color[2] = setColor[2];
}

void Tank::setLenCanon (float x)
{
  lenCanon = x;
}

float Tank::getTankHealth()
{
  return Health;
}

vector<float> Tank::getTankPosition()
{
  vector<float> temp;
  temp.push_back(centreX);
  temp.push_back(centreY);
  return temp;
}

float Tank::getTankSpeed()
{
  return speed;
}

vector<float> Tank::getTankColour()
{
  return Color;
}

void Tank::handleCollisionOuterWall()
{
  // set 4 flags
  //setting an offset of 0.5 units from the wall
  bool tempRight = 0;   //Right wall
  bool tempLeft = 0;
  bool tempTop = 0;
  bool tempBottom = 0;
  for(int i = 0; i < 4 ;i++)
  {
    if (yCordVertRect[i] <= 1.0)  // top wall
    {
      //cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
      tempTop = 1;
    }
    if (yCordVertRect[i] >= (window_height - 1.0))  //Bottom wall
      tempBottom = 1;
    if (xCordVertRect[i] <= 1.0)  // Left wall
      tempLeft = 1;
    if (xCordVertRect[i] >= (window_width - 1.0)) //Right wall
      tempRight = 1;
  }
  // once collision is handled set flags back to 0
  collisionRightWall = tempRight;
  collisionLeftWall = tempLeft;
  collisionBottomWall = tempBottom;
  collisionTopWall = tempTop;
}

bool Tank::destroyTank()
{
  sf::FloatRect boundsTank = myTank.getGlobalBounds();
    int temp=numBullets;
  for (int i = 0; i < bulletShot.size()&& i<temp;i++)
  {
    if (!(this->isDestroyed) && boundsTank.contains(bulletShot[i].centreBulletX,bulletShot[i].centreBulletY) && bulletShot[i].isActive)
    {
      //      cout<<"Alas the tank has been hit\n";
            Health=Health-1;
            bulletShot[i].isActive=false;
            if(Health<0)
            {
              isDestroyed=true;
            }
            // send data packet for the destroyed tank


      //~Tank();
      return 1;
    }
  }
  return 0;
}

void Tank::handleCollisionMazeWall()
{   /// change
    //////////////////////////change
  bool tempRightCentreUp = 0;   //Right wall
  bool tempRightCentreDown = 0;
  bool tempLeftCentreUp = 0;
  bool tempLeftCentreDown = 0;
  bool tempTopCentreRight = 0;
  bool tempTopCentreLeft = 0;
  bool tempBottomCentreRight = 0;
  bool tempBottomCentreLeft = 0;


        double Deg = ((180.0/Pi)*angleXaxis); /// angles from positive X-axis
        vector <vector <float> > quad;
        for (vector <vector <float> > :: iterator itMaze = quad1.begin(); itMaze != quad1.end(); ++itMaze)
      {
        quad.push_back(*itMaze);
      }
      bool tempRight = 0;
    bool tempLeft = 0;
    bool tempTop = 0;
    bool tempBottom = 0;
    int typeIter = 0;
        for(vector <vector <float> > :: iterator it = quad.begin(); it != quad.end(); ++it)
        {
          float distanceFromWall;
          //if it a vertical wall
            if ((*it)[0] == (*it)[2])
            {
              for (int iter = 0;iter<4;iter++)// for each coordinate of rectangle
              {
                distanceFromWall = absFloat(xCordVertRect[iter] - (*it)[0]);
                float tempCompTop,tempCompBottom;
              if ((*it)[1] > (*it)[3])
              {
              tempCompTop = (*it)[3];
              tempCompBottom = (*it)[1];
              }
              else
              {
              tempCompTop = (*it)[1];
              tempCompBottom = (*it)[3];
              }
              if((distanceFromWall <= 7.0) &&  (xCordVertRect[iter] <= (*it)[0]))   //// vertical wall and tank is on left of the wall
              {
                if (yCordVertRect[iter] <= (tempCompBottom) &&  yCordVertRect[iter] >= (tempCompTop))
                {
                //set flag as 1
                  //cout<<"hfushfshfj\n";
                //if (typeMazeWall[iter] == 3)
                  if (typeMazeWall[typeIter] == 3)
                  {
                    tempLeft = 1;
                    if (yCordVertRect[iter] > centreY)
                      tempLeftCentreUp = 1;
                    else
                      tempLeftCentreDown = 1;
                  }
                  }
              }
              else if((distanceFromWall <= 7.0) &&  (xCordVertRect[iter] >= (*it)[0]))    //// vertical wall and tank is on right of the wall
              {
                if (yCordVertRect[iter] <= (tempCompBottom) &&  yCordVertRect[iter] >= (tempCompTop))
                {
                //set flag as 1
                //  if (typeMazeWall[iter] == 4)
                  if (typeMazeWall[typeIter] == 4)
                  {
                    tempRight = 1;
                    if (yCordVertRect[iter] > centreY)
                      tempRightCentreUp = 1;
                    else
                      tempRightCentreDown = 1;
                  }
                  }
              }
            }
          }
          else if ((*it)[1] == (*it)[3])    /// horizontal wall
          {
            for (int iter = 0;iter<4;iter++)// for each coordinate of rectangle
              {
                distanceFromWall = absFloat(yCordVertRect[iter] - (*it)[1]);
                float tempCompLeft,tempCompRight;
              if ((*it)[0] > (*it)[2])
              {
              tempCompLeft = (*it)[2];
              tempCompRight = (*it)[0];
              }
              else
              {
              tempCompLeft = (*it)[0];
              tempCompRight = (*it)[2];
              }
              if((distanceFromWall <= 7.0) &&  (yCordVertRect[iter] <= (*it)[1]))   //// horizontal wall and tank is on the top of the wall
              {
                //cout<<"here8791\n";
                //cout<<xCordVertRect[iter]<<endl;
                //cout<<tempCompRight<<"    "<<tempCompLeft<<endl;
                if ((xCordVertRect[iter] <= tempCompRight) &&  (xCordVertRect[iter] >= tempCompLeft))
                {
                //set flag as 1
                  //cout<<"here2561\n";
                //  if (typeMazeWall[iter] == 1)
                  if (typeMazeWall[typeIter] == 1)
                  {
                    tempTop = 1;
                    if (xCordVertRect[iter] < centreX)
                      tempTopCentreRight = 1;
                    else
                      tempTopCentreLeft = 1;
                  }
                  }
              }
              else if((distanceFromWall <= 7.0) &&  (yCordVertRect[iter] >= (*it)[1]))    //// horizontal wall and tank is on bottom of the wall
              {
                if (xCordVertRect[iter] <= (tempCompRight) &&  xCordVertRect[iter] >= (tempCompLeft))
                {
                //set flag as 1
                //  if (typeMazeWall[iter] == 2)
                if (typeMazeWall[typeIter] == 2)
                {
                  tempBottom = 1;
                  if (xCordVertRect[iter] < centreX)
                      tempBottomCentreRight = 1;
                    else
                      tempBottomCentreLeft = 1;
                }
                  }
              }
            }
          }
        typeIter++;
        }
    collisionMazeWallRight = tempRight;
  collisionMazeWallLeft = tempLeft;
  collisionMazeWallBottom = tempBottom;
  collisionMazeWallTop = tempTop;
  ////////////////change
  rightCentreUp = tempRightCentreUp;    //Right wall
  rightCentreDown = tempRightCentreDown;
  leftCentreUp = tempLeftCentreUp;
  leftCentreDown = tempLeftCentreDown;
  topCentreRight = tempTopCentreRight;
  topCentreLeft = tempTopCentreLeft;
  bottomCentreRight = tempBottomCentreRight;
  bottomCentreLeft = tempBottomCentreLeft;

}

void Tank::handleHealth()
{
    switch(Health)
    {
        case 5 :
        {
            this->lifeFill.setFillColor(sf::Color::Green);
            this->lifeFill.setSize(sf::Vector2f(45,10));
            break;
        }

        case 4 :
        {
            this->lifeFill.setFillColor(sf::Color::Green);
            this->lifeFill.setSize(sf::Vector2f(36,10));
            break;
        }

        case 3 :
        {
            this->lifeFill.setFillColor(sf::Color::Yellow);
            this->lifeFill.setSize(sf::Vector2f(27,10));
            break;
        }

        case 2 :
        {
            this->lifeFill.setFillColor(sf::Color::Magenta);
            this->lifeFill.setSize(sf::Vector2f(18,10));
            break;
        }

        case 1 :
        {
            this->lifeFill.setFillColor(sf::Color::Red);
            this->lifeFill.setSize(sf::Vector2f(9,10));
            break;
        }
    }
}

void Tank::setName(string name)
{
    this->Name.setString(name);
}

void Tank::updateEquationsRectEdges()
{
  xCoeffVertRect = {yCordVertRect[1]-yCordVertRect[0],yCordVertRect[2]-yCordVertRect[1],yCordVertRect[3]-yCordVertRect[2],yCordVertRect[0]-yCordVertRect[3]};
  yCoeffVertRect = {xCordVertRect[0]-xCordVertRect[1],xCordVertRect[1]-xCordVertRect[2],xCordVertRect[2]-xCordVertRect[3],xCordVertRect[3]-xCordVertRect[0]};
  float c0 = (-1)*xCordVertRect[0]*(yCordVertRect[1]-yCordVertRect[0]) + yCordVertRect[0]*(xCordVertRect[1]-xCordVertRect[0]);
  float c1 = (-1)*xCordVertRect[1]*(yCordVertRect[2]-yCordVertRect[1]) + yCordVertRect[1]*(xCordVertRect[2]-xCordVertRect[1]);
  float c2 = (-1)*xCordVertRect[2]*(yCordVertRect[3]-yCordVertRect[2]) + yCordVertRect[2]*(xCordVertRect[3]-xCordVertRect[2]);
  float c3 = (-1)*xCordVertRect[3]*(yCordVertRect[0]-yCordVertRect[3]) + yCordVertRect[3]*(xCordVertRect[0]-xCordVertRect[3]);
  constVertRect = {c0,c1,c2,c3};
}

float Tank::distanceFromTankEdge(float xCoeff,float yCoeff,float constTerm,float xCordPoint,float yCordPoint)
{
  float numerator = absFloat(xCoeff*xCordPoint + yCoeff*yCordPoint + constTerm);
  float denominator = sqrt(xCoeff*xCoeff + yCoeff*yCoeff);
  return numerator/denominator;
}

void Tank::handleCollisionMazeWallVertices()
{
  double Deg = ((180.0/Pi)*angleXaxis); /// angles from positive X-axis
    bool tempTopLeft = 0;
    bool tempTopRight = 0;
    bool tempBottomLeft = 0;
    bool tempBottomRight = 0;
    for(int i = 0;i < topLeftPoints.size();i++)
    {
      float xCordPoint = topLeftPoints[i][0];
      float yCordPoint = topLeftPoints[i][1];
      for (int j = 0;j<4;j++)
      {
        float xCoeff = xCoeffVertRect[j];
        float yCoeff = yCoeffVertRect[j];
        float constTerm = constVertRect[j];
        float distanceFromEdge = distanceFromTankEdge(xCoeff,yCoeff,constTerm,xCordPoint,yCordPoint);
        if (distanceFromEdge < 5.0)
        {
          //set flag as 1
          /*for (int k = 0;k < 4;k++)
          {
            cout<<"xCordVertRect "<<xCordVertRect[k]<<endl;
            cout<<"yCordVertRect "<<yCordVertRect[k]<<endl;
          }*/
          /*cout<<"j "<<j<<endl;
          cout<<"xCoeff "<<xCoeff<<endl;
          cout<<"yCoeff "<<yCoeff<<endl;
          cout<<"constTerm "<<constTerm<<endl;
          cout<<"xCord "<<xCordPoint<<endl;
          cout<<"yCord "<<yCordPoint<<endl;
          cout<<"distanceFromEdge1 "<<distanceFromEdge<<endl;*/
          if (j != 3)
          {
            float xLeft,xRight;
            if (xCordVertRect[j] > xCordVertRect[j+1])
            {
              xLeft = xCordVertRect[j+1];
              xRight = xCordVertRect[j];
            }
            else
            {
              xLeft = xCordVertRect[j];
              xRight = xCordVertRect[j+1];
            }
            float yTop,yBottom;
            if (yCordVertRect[j] > yCordVertRect[j+1])
            {
              yTop = yCordVertRect[j+1];
              yBottom = yCordVertRect[j];
            }
            else
            {
              yTop = yCordVertRect[j];
              yBottom = yCordVertRect[j+1];
            }
            if (xLeft <= xCordPoint && xRight >= xCordPoint && yTop <= yCordPoint && yBottom >= yCordPoint)
              tempTopLeft = 1;
          }
          else /// j==3
          {
            float xLeft,xRight;
            if (xCordVertRect[0] > xCordVertRect[3])
            {
              xLeft = xCordVertRect[3];
              xRight = xCordVertRect[0];
            }
            else
            {
              xLeft = xCordVertRect[0];
              xRight = xCordVertRect[3];
            }
            float yTop,yBottom;
            if (yCordVertRect[0] > yCordVertRect[3])
            {
              yTop = yCordVertRect[3];
              yBottom = yCordVertRect[0];
            }
            else
            {
              yTop = yCordVertRect[0];
              yBottom = yCordVertRect[3];
            }
            if (xLeft <= xCordPoint && xRight >= xCordPoint && yTop <= yCordPoint && yBottom >= yCordPoint)
              tempTopLeft = 1;
          }

        }
      }
    }
    for(int i = 0;i < topRightPoints.size();i++)
    {
      float xCordPoint = topRightPoints[i][0];
      float yCordPoint = topRightPoints[i][1];
      for (int j = 0;j<4;j++)
      {
        float xCoeff = xCoeffVertRect[j];
        float yCoeff = yCoeffVertRect[j];
        float constTerm = constVertRect[j];
        float distanceFromEdge = distanceFromTankEdge(xCoeff,yCoeff,constTerm,xCordPoint,yCordPoint);
        if (distanceFromEdge < 5.0)
        {
          //set flag as 1
          /*for (int k = 0;k < 4;k++)
          {
            cout<<"xCordVertRect "<<xCordVertRect[k]<<endl;
            cout<<"yCordVertRect "<<yCordVertRect[k]<<endl;
          }*/
          /*cout<<"xCoeff "<<xCoeff<<endl;
          cout<<"yCoeff "<<yCoeff<<endl;
          cout<<"constTerm "<<constTerm<<endl;
          cout<<"xCord "<<xCordPoint<<endl;
          cout<<"yCord "<<yCordPoint<<endl;

          cout<<"distanceFromEdge2 "<<distanceFromEdge<<endl;*/
          if (j != 3)
          {
            float xLeft,xRight;
            if (xCordVertRect[j] > xCordVertRect[j+1])
            {
              xLeft = xCordVertRect[j+1];
              xRight = xCordVertRect[j];
            }
            else
            {
              xLeft = xCordVertRect[j];
              xRight = xCordVertRect[j+1];
            }
            float yTop,yBottom;
            if (yCordVertRect[j] > yCordVertRect[j+1])
            {
              yTop = yCordVertRect[j+1];
              yBottom = yCordVertRect[j];
            }
            else
            {
              yTop = yCordVertRect[j];
              yBottom = yCordVertRect[j+1];
            }
            if (xLeft <= xCordPoint && xRight >= xCordPoint && yTop <= yCordPoint && yBottom >= yCordPoint)
              tempTopRight = 1;
          }
          else /// j==3
          {
            float xLeft,xRight;
            if (xCordVertRect[0] > xCordVertRect[3])
            {
              xLeft = xCordVertRect[3];
              xRight = xCordVertRect[0];
            }
            else
            {
              xLeft = xCordVertRect[0];
              xRight = xCordVertRect[3];
            }
            float yTop,yBottom;
            if (yCordVertRect[0] > yCordVertRect[3])
            {
              yTop = yCordVertRect[3];
              yBottom = yCordVertRect[0];
            }
            else
            {
              yTop = yCordVertRect[0];
              yBottom = yCordVertRect[3];
            }
            if (xLeft <= xCordPoint && xRight >= xCordPoint && yTop <= yCordPoint && yBottom >= yCordPoint)
              tempTopRight = 1;
          }
        }
      }
    }
    for(int i = 0;i < bottomLeftPoints.size();i++)
    {
      float xCordPoint = bottomLeftPoints[i][0];
      float yCordPoint = bottomLeftPoints[i][1];
      for (int j = 0;j<4;j++)
      {
        float xCoeff = xCoeffVertRect[j];
        float yCoeff = yCoeffVertRect[j];
        float constTerm = constVertRect[j];
        float distanceFromEdge = distanceFromTankEdge(xCoeff,yCoeff,constTerm,xCordPoint,yCordPoint);
        if (distanceFromEdge < 5.0)
        {
          //set flag as 1
          /*for (int k = 0;k < 4;k++)
          {
            cout<<"xCordVertRect "<<xCordVertRect[k]<<endl;
            cout<<"yCordVertRect "<<yCordVertRect[k]<<endl;
          }*/
          /*cout<<"xCoeff "<<xCoeff<<endl;
          cout<<"yCoeff "<<yCoeff<<endl;
          cout<<"constTerm "<<constTerm<<endl;
          cout<<"xCord "<<xCordPoint<<endl;
          cout<<"yCord "<<yCordPoint<<endl;

          cout<<"distanceFromEdge3 "<<distanceFromEdge<<endl;
          */
          if (j != 3)
          {
            float xLeft,xRight;
            if (xCordVertRect[j] > xCordVertRect[j+1])
            {
              xLeft = xCordVertRect[j+1];
              xRight = xCordVertRect[j];
            }
            else
            {
              xLeft = xCordVertRect[j];
              xRight = xCordVertRect[j+1];
            }
            float yTop,yBottom;
            if (yCordVertRect[j] > yCordVertRect[j+1])
            {
              yTop = yCordVertRect[j+1];
              yBottom = yCordVertRect[j];
            }
            else
            {
              yTop = yCordVertRect[j];
              yBottom = yCordVertRect[j+1];
            }
            if (xLeft <= xCordPoint && xRight >= xCordPoint && yTop <= yCordPoint && yBottom >= yCordPoint)
              tempBottomLeft = 1;
          }
          else /// j==3
          {
            float xLeft,xRight;
            if (xCordVertRect[0] > xCordVertRect[3])
            {
              xLeft = xCordVertRect[3];
              xRight = xCordVertRect[0];
            }
            else
            {
              xLeft = xCordVertRect[0];
              xRight = xCordVertRect[3];
            }
            float yTop,yBottom;
            if (yCordVertRect[0] > yCordVertRect[3])
            {
              yTop = yCordVertRect[3];
              yBottom = yCordVertRect[0];
            }
            else
            {
              yTop = yCordVertRect[0];
              yBottom = yCordVertRect[3];
            }
            if (xLeft <= xCordPoint && xRight >= xCordPoint && yTop <= yCordPoint && yBottom >= yCordPoint)
              tempBottomLeft = 1;
          }
        }
      }
    }
    for(int i = 0;i < bottomRightPoints.size();i++)
    {
      float xCordPoint = bottomRightPoints[i][0];
      float yCordPoint = bottomRightPoints[i][1];
      for (int j = 0;j<4;j++)
      {
        float xCoeff = xCoeffVertRect[j];
        float yCoeff = yCoeffVertRect[j];
        float constTerm = constVertRect[j];
        float distanceFromEdge = distanceFromTankEdge(xCoeff,yCoeff,constTerm,xCordPoint,yCordPoint);
        if (distanceFromEdge < 5.0)
        {
          //set flag as 1
          /*for (int k = 0;k < 4;k++)
          {
            cout<<"xCordVertRect "<<xCordVertRect[k]<<endl;
            cout<<"yCordVertRect "<<yCordVertRect[k]<<endl;
          }*/
          /*cout<<"xCoeff "<<xCoeff<<endl;
          cout<<"yCoeff "<<yCoeff<<endl;
          cout<<"constTerm "<<constTerm<<endl;
          cout<<"xCord "<<xCordPoint<<endl;
          cout<<"yCord "<<yCordPoint<<endl;

          cout<<"distanceFromEdge4 "<<distanceFromEdge<<endl;
          */
          if (j != 3)
          {
            float xLeft,xRight;
            if (xCordVertRect[j] > xCordVertRect[j+1])
            {
              xLeft = xCordVertRect[j+1];
              xRight = xCordVertRect[j];
            }
            else
            {
              xLeft = xCordVertRect[j];
              xRight = xCordVertRect[j+1];
            }
            float yTop,yBottom;
            if (yCordVertRect[j] > yCordVertRect[j+1])
            {
              yTop = yCordVertRect[j+1];
              yBottom = yCordVertRect[j];
            }
            else
            {
              yTop = yCordVertRect[j];
              yBottom = yCordVertRect[j+1];
            }
            if (xLeft <= xCordPoint && xRight >= xCordPoint && yTop <= yCordPoint && yBottom >= yCordPoint)
              tempBottomRight = 1;
          }
          else /// j==3
          {
            float xLeft,xRight;
            if (xCordVertRect[0] > xCordVertRect[3])
            {
              xLeft = xCordVertRect[3];
              xRight = xCordVertRect[0];
            }
            else
            {
              xLeft = xCordVertRect[0];
              xRight = xCordVertRect[3];
            }
            float yTop,yBottom;
            if (yCordVertRect[0] > yCordVertRect[3])
            {
              yTop = yCordVertRect[3];
              yBottom = yCordVertRect[0];
            }
            else
            {
              yTop = yCordVertRect[0];
              yBottom = yCordVertRect[3];
            }
            if (xLeft <= xCordPoint && xRight >= xCordPoint && yTop <= yCordPoint && yBottom >= yCordPoint)
              tempBottomRight = 1;
          }
        }
      }
    }
    collisionTopLeftPoint = tempTopLeft;
    collisionTopRightPoint = tempTopRight;
    collisionBottomLeftPoint = tempBottomLeft;
    collisionBottomRightPoint = tempBottomRight;
}
