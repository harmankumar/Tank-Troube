
#include "powerUp.h"
///////////////////////////////////////////////////////
int numPowerUps = 0;
vector <PowerUp> powerUpVector;

PowerUp::PowerUp():myPowerUp()
{

    sf::Texture PowerUpTexture;    //image to be loaded
    PowerUpSound.loadFromFile(string(res)+"amazing.ogg");
    
    sound.setBuffer(PowerUpSound);
    myPowerUp.setRotation(0.0);
    myPowerUp.setScale(0.1f, 0.1f);
    isActive=false;
    powerUpType=rand()%3;
    if(powerUpType==0)
    {
        PowerUpTexture.loadFromFile(string(res)+"tank.png");
    }
    else
        if(powerUpType==1)
        {
            PowerUpTexture.loadFromFile(string(res)+"tank.png");
        }
        else
            if(powerUpType==2)
            {
                PowerUpTexture.loadFromFile(string(res)+"tank.png");
            }
    
    centrePowerUpX=rand()%window_width;
    centrePowerUpY=rand()%window_height;
    centrePowerUpX=100;
    centrePowerUpY=200;
    sf::Vector2f temp;
    temp.x=centrePowerUpX;
    temp.y=centrePowerUpY;
    myPowerUp.setPosition(temp);
    myPowerUp.setTexture(PowerUpTexture);
}

void PowerUp::playSound()
{
    this->sound.setVolume(sound.getVolume()/2.0);
    this->sound.play();
}
