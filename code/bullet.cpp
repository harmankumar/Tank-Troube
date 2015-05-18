
#include "bullet.h"
///////////////////////////////////////////////////////
int numBullets = 0;
vector <Bullet> bulletShot;

Bullet::Bullet():circle(),myBullet()
{

    bulletSound.loadFromFile(string(res)+"amazing.ogg");
    sound.setBuffer(bulletSound);


	///////////////////////////////////  change
    // bring coordinates at centre of bullet
    //sf::FloatRect bounds = mybullet.getLocalBounds();
	//mybullet.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	//bulletTexture.loadFromFile(string(res)+"bullet.jpeg");
    //myBullet.setTexture(bulletTexture);
    circle.setRadius(3.0);
    circle.setFillColor(sf::Color(100, 0,100));
    myBullet.setRotation(0.0);
    myBullet.setScale(0.1f, 0.1f);
    radiusBullet = 3.0;	/* set it as global */
    ///////////////////////////////////////////////////////
    speedBullet = 1.0;		/* set it as global */
    colorBulletR = 1.0;
    colorBulletG = 1.5;
    colorBulletB = 0.5;
    angleBullet = 0;
    isActive=false;
    ///////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////
void Bullet::nexPosBullet()										//// change
{
  	myBullet.move(speedBullet*cos(angleBullet),speedBullet*sin(angleBullet));
    circle.move(speedBullet*cos(angleBullet),speedBullet*sin(angleBullet));
  	float speedX=speedBullet*cos(angleBullet);
  	float speedY=speedBullet*sin(angleBullet);
     ///speed of bullet remains fixed...we can also add a feature of gradual increase/decrease of speed of bullet
 	speedBullet=sqrt(pow(speedX,2)+ pow(speedY,2));
    if (speedBullet!=0)
    {
        myBullet.setRotation(atan2(speedY,speedX)*180/Pi);//Check if it takes input in degrees
    	angleBullet = atan2(speedY,speedX);
    }

    if (angleBullet < 0)
    {
        myBullet.setRotation((angleBullet+2*Pi)*180/Pi);
        //angleBullet += 2*Pi;
    }
     angleBullet=Pi*myBullet.getRotation()/180;
    sf::Vector2f tempBullet = myBullet.getPosition();
    centreBulletX =tempBullet.x;

    centreBulletY =tempBullet.y;
   //cout<<centreBulletX<<endl;
   //cout<<centreBulletY<<endl;
}

///// (0,0) is the top left corner and towards bottom is window_width and towards right is window_height
void Bullet::handleCollisionOuterWall() // angles are from +ve x-axis in anti-clockwise fashion  //// change
{
	float tempAngleBulletRad = Pi*myBullet.getRotation()/180;
	float tempcentreBulletX,tempcentreBulletY;
  if(tempAngleBulletRad<0)
            tempAngleBulletRad+=2*Pi;
        if(tempAngleBulletRad >2*Pi)
            tempAngleBulletRad-=2*Pi;

        double Deg = ((180.0/Pi)*tempAngleBulletRad);
        ///////////////////////////////////////////////////////////////////
        ///////////////////  We have so many walls so we need to check for different walls....use some data structure to detect these
        sf::Vector2f tempBullet = myBullet.getPosition();
    tempcentreBulletX = tempBullet.x;
    tempcentreBulletY = tempBullet.y;
        if((centreBulletX > (window_height - radiusBullet)) && ((0 <= Deg && Deg <= 90 )|| (270 <= Deg && Deg <= 360))) // Right wall
        {
    if (0 <= Deg <= 90)
            tempAngleBulletRad=Pi- tempAngleBulletRad;
            else
            tempAngleBulletRad = 3*Pi - tempAngleBulletRad;
            if(tempAngleBulletRad <0)
                tempAngleBulletRad +=2*Pi;
            if(tempAngleBulletRad >2*Pi)
                tempAngleBulletRad -=2*Pi;
        }
        else if ((centreBulletX < radiusBullet)  && ((90 <= Deg && Deg <= 270))) // left wall
        {
        	cout<<centreBulletX<<endl;
   cout<<centreBulletY<<endl;
    if (90 <= Deg <= 180)
            tempAngleBulletRad =Pi- tempAngleBulletRad;
            else
            tempAngleBulletRad = 3*Pi - tempAngleBulletRad;
            if(tempAngleBulletRad <0)
                tempAngleBulletRad +=2*Pi;
            if(tempAngleBulletRad >2*Pi)
                tempAngleBulletRad -=2*Pi;
        }
        else if ((centreBulletY > (window_width - radiusBullet)) && ((0 <= Deg && Deg <= 180 ))) // bottom wall
        {
      tempAngleBulletRad = 2*Pi - tempAngleBulletRad;
            if(tempAngleBulletRad <0)
                tempAngleBulletRad +=2*Pi;
            if(tempAngleBulletRad >2*Pi)
                tempAngleBulletRad -=2*Pi;
        }
        else if ((centreBulletY < radiusBullet)  && ((180 <= Deg && Deg <= 360 )))  // top wall
        {
    tempAngleBulletRad =2*Pi - tempAngleBulletRad;
            if(tempAngleBulletRad <0)
                tempAngleBulletRad +=2*Pi;
            if(tempAngleBulletRad >2*Pi)
                tempAngleBulletRad -=2*Pi;
        }
        myBullet.setRotation(tempAngleBulletRad*180/Pi);
        angleBullet=Pi*myBullet.getRotation()/180;
}

void Bullet::handleCollisionMazeWall ()//(vector <vector <float> > quad)      //// check collisions only in a specific quadrant
{		/// change
  //// let width of each wall be x... we need minimum distance of r + x/2..
	//cout<<"here...handleCollisionMazeWall\n";
  //float tempAngleBulletRad = Pi*myBullet.getRotation()/180;
  float tempAngleBulletRad = angleBullet;
  float tempcentreBulletX,tempcentreBulletY;
  //float widthWall;      ///// ASSIGN
  //float x = radiusBullet + widthWall/2.0;
  float x = radiusBullet;
  sf::Vector2f tempBullet = myBullet.getPosition();
    tempcentreBulletX = tempBullet.x;
    tempcentreBulletY = tempBullet.y;
  if(tempAngleBulletRad<0)
            tempAngleBulletRad+=2*Pi;
        if(tempAngleBulletRad >2*Pi)
            tempAngleBulletRad -=2*Pi;

        double Deg = ((180.0/Pi)*tempAngleBulletRad); /// angles from positive X-axis
        /*
		for hadling collision with the maze walls...we first find the quadrant in which the bullet is present
		and initially the maze walls were divided based on the quadrants.
		initially the width of each line is ignored and can be incorporated later on.
		*/
		vector <vector <float> > quad;
		/*int i = findQuad();

  		if (i == 1) //bullet is in first quad
  			quad =  quad1;
		if (i == 2)
  			quad =  quad2;
		if (i == 3)
  			quad =  quad3;
		if (i == 4)
  			quad =  quad4;
       */
        for (vector <vector <float> > :: iterator itMaze = quad1.begin(); itMaze != quad1.end(); ++itMaze)
    {
    	/////////////////////////////////////////
    	quad.push_back(*itMaze);
    	/////////////////////////////////////////
    }
        for(vector <vector <float> > :: iterator it = quad.begin(); it != quad.end(); ++it)
        {

          float distanceFromWall;
          //if it a vertical wall
          if ((*it)[0] == (*it)[2])
          {
            //TODO
            distanceFromWall = absFloat(centreBulletX - (*it)[0]);
            float tempCompTop,tempCompBottom;
        	if ((*it)[1] > (*it)[3])
        	{
        		//tempCompTop = (*it)[1];
        		//tempCompBottom = (*it)[3];
        		tempCompTop = (*it)[3];
        		tempCompBottom = (*it)[1];
        	}
        	else
        	{
        		tempCompTop = (*it)[1];
        		tempCompBottom = (*it)[3];
        	}
        	//cout<<"distanceFromWall "<<distanceFromWall<<endl;

            if((distanceFromWall <= 0.5) && ((0 <= Deg && Deg <= 90 )|| (270 <= Deg && Deg <= 360)))   /// moving towards right
        	{
        		// we need to maintain an ordering between the coordinates of the walls..like which one is the top or bottom so on

        		//cout<<"here...handleCollisionMazeWall\n";
        	if (centreBulletY <= (tempCompBottom) && centreBulletY >= (tempCompTop))
        	{
        		//cout<<"vertical wall from right\n";
    		if (0 <= Deg <= 90)
            tempAngleBulletRad =Pi- tempAngleBulletRad;
            else
            tempAngleBulletRad = 3*Pi - tempAngleBulletRad;
            if(tempAngleBulletRad<0)
                tempAngleBulletRad +=2*Pi;
            if(tempAngleBulletRad >2*Pi)
                tempAngleBulletRad -=2*Pi;
        }
        }
        else if ((distanceFromWall <= 0.5)  && ((90 <= Deg && Deg <= 270)))      /// going towards left
        {

        	if (centreBulletY >= (tempCompTop) && centreBulletY <= (tempCompBottom))
        	{
        		cout<<"vertical wall from left\n";
    		if (90 <= Deg && Deg <= 180)
            tempAngleBulletRad =Pi- tempAngleBulletRad;
            else
            tempAngleBulletRad = 3*Pi - tempAngleBulletRad;
            if(tempAngleBulletRad <0)
                tempAngleBulletRad +=2*Pi;
            if(tempAngleBulletRad >2*Pi)
                tempAngleBulletRad -=2*Pi;
        }
        }
          }
          //if it a horizontal wall
          else if ((*it)[1] == (*it)[3])
          {
            //TODO

            distanceFromWall = absFloat(centreBulletY - (*it)[1]);
            //cout<<"distanceFromWall "<<distanceFromWall<<endl;
            float tempCompRight,tempCompLeft;
        	if ((*it)[0] > (*it)[2])
        	{
        		tempCompRight = (*it)[0];
        		tempCompLeft = (*it)[2];
        	}
        	else
        	{
        		tempCompRight = (*it)[2];
        		tempCompLeft = (*it)[0];
        	}


          ///// TODO
          // since we have line "segments" hence finding only distance does not suffice..we need to check that
          //bullet's coordinates lie within the coordinates of the maze wall

         if ((distanceFromWall <= 0.5) && (180 <= Deg && Deg <= 360))      /// going upwards
        {
        	if (centreBulletX <= (tempCompRight) && centreBulletX >= (tempCompLeft - radiusBullet))
        	{
        		//cout<<"horizontal wall from top\n";
      tempAngleBulletRad =2*Pi - tempAngleBulletRad;
            if(tempAngleBulletRad <0)
                tempAngleBulletRad +=2*Pi;
            if(tempAngleBulletRad >2*Pi)
                tempAngleBulletRad -=2*Pi;
        }
        }
        else if ((distanceFromWall <= 0.5)  && (0 <= Deg && Deg <= 180 ))  // going down
        {
        	if (centreBulletX <= (tempCompRight + radiusBullet) && centreBulletX >= (tempCompLeft - radiusBullet))
        	{
        		//cout<<"horizontal wall from bottom\n";
    tempAngleBulletRad =2*Pi - tempAngleBulletRad;
            if(tempAngleBulletRad <0)
                tempAngleBulletRad +=2*Pi;
            if(tempAngleBulletRad >2*Pi)
                tempAngleBulletRad -=2*Pi;
            }
        }
        }
          /////
    }
    myBullet.setRotation(tempAngleBulletRad*180/Pi);
    angleBullet=Pi*myBullet.getRotation()/180;
}

int Bullet::findQuad()		/// change
{
	sf::Vector2f tempBullet = myBullet.getPosition();
	//float tempcentreBulletX,tempcentreBulletY;
    //tempcentreBulletX = tempBullet.x;
    //tempcentreBulletY = tempBullet.y;
    if (centreBulletX >= (window_height)/2.0 || centreBulletY < (window_width)/2.0)
        return 1;
    else if (centreBulletX < (window_height)/2.0 || centreBulletY >= (window_width)/2.0)
        return 2;
    else if (centreBulletX < (window_height)/2.0 || centreBulletY >= (window_width)/2.0)
        return 3;
    else if (centreBulletX >= (window_height)/2.0 || centreBulletY >= (window_width)/2.0)
        return 4;
}

void Bullet::playSound()
{
    this->sound.setVolume(sound.getVolume()/2.0);
    this->sound.play();
}
