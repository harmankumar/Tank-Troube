#ifndef HELPERFN_H
#define HELPERFN_H

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
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "AnimatedSprite.hpp"

using namespace std;

//#define numOfUsers 3
#define numOfAcks 20
#define wrapAroundSize 20480
#define numOfCrucialMsg 2
#define maxRTT 1
#define connectionTimeOut 20
#define numOfConnectingAttempts 5

//Gui data///
struct initialData
{
    int joinFlagSet ;
    int numPlayersSet;
    string selfIpSet;
    int selfPortSet ;
    string targetIpSet;
    int targetPortSet;
};
extern initialData guiResult;
/////////

extern const char* res;
extern int window_width;
extern int window_height;
extern double Pi;
extern double timePerframe;
extern double deltaKeyPress;
extern double smalldeltaKeyPress;
//extern vector <int> keystate;
extern float widthWall;		// when we incorporate this..we need to consider wall as recatangularvector <vector <float> > mazeVertWalls;
extern vector <vector <float> > mazeHoriWalls;
extern vector <vector <float> > vertRight;
extern vector <vector <float> > vertLeft;
extern vector <vector <float> > horiUpper;
extern vector <vector <float> > horiLower;
extern vector <vector <float> > quad1;
extern vector <vector <float> > quad2;
extern vector <vector <float> > quad3;
extern vector <vector <float> > quad4;

extern vector <int> typeMazeWall;
extern vector <vector <float> > topLeftPoints;
extern vector <vector <float> > topRightPoints;
extern vector <vector <float> > bottomLeftPoints;
extern vector <vector <float> > bottomRightPoints;

extern vector<float> xInputcoords; // similar for angles and other data that may require interpolation
extern vector<float> yInputcoords; // user input and network input at same level to ease similar interpolation
extern vector<float> xSimulatedcoords;
extern vector<float> ySimulatedcoords;
//################ network ####################//
//recent
extern string myIp;
extern std::vector<string> userIP;

//
extern int myPort;	/* hard-coded port number for self */
extern vector<int> userPort;	/* hard-coded port number for remoter user */
extern int bufferSize;
extern int programState;
extern vector<bool> tempAck;
extern int numOfUsers;
extern vector<bool> usersJoined;
extern bool receivedAllUserData;

/*##################################################*/
extern int myUserId;
extern string tmpForWindow;
extern vector< int > mostRecentMsgNum;
extern vector<bool> msgSent;
extern vector< vector <bool> > msgReceived;
extern vector< vector <bool> > ackReceived;
extern vector<string> lastCrucialMsgs;
extern vector< int > sendTime;     // stores the time when a particuar message was sent
extern vector< int > receiveTime;  // receiveTime stores information about the time acknoledgement for a particular code was received
extern vector< int > RTT;
extern vector< time_t > lastUserActivity;
extern vector< bool > userActive;
extern pthread_mutex_t mtx;
extern time_t globalTimer;

string makeMessage ( vector <string> inputSet);
string attachCrucial (string mainMsg);

struct sendQueueItem
{
    string message;
    int destPort;
    string destIp;
    bool isAck;
};

extern queue<sendQueueItem > sendQueue;
extern queue<string> receiveQueue;

struct sendInfo
{
    int socketD;
    int destPort;
    //recent
    string destIp;
    //
    string sendMessage;
    bool isAck;
};

float absFloat(float x);

#endif
