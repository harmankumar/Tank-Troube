#ifndef NETWORK_H
#define NETWORK_H

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
#include <fstream>
#include <SFML/Audio.hpp>

#include "helperfn.h"
#include "bullet.h"
#include "game.h"
#include "tank.h"

/*
// userId stars from 0
// Message id starts from 0
// Each acknowledgement carries acknowledgement of prevous 20 Messages for now
// User Id starts from 0 onwards . The size of ackReceived array would be same as number of users , in this case one array would be left
// unused but this makes addressing is the vector pretty easy

Format of acknowledgement message  [30,1,0,0,1,1] this means ack for msg 30 is received and msg 29 ->Recvd,28->NotRecvd,..and so one
in this case ack of 5 previous message is received . for msg num 4 the message would be [4,1,0,0,1,X] the last thing is don' care , can be one or zero

IMPORTANT mostRecentMsgNum stores (serial number of most recent message sent + 1 for the myUser
For other users it stores information regarding most recent message received from other user
*/
int start(int joinFlag ,int numPlayers,string selfIp,int selfPort, string targetIp,int targetPort);
void * handleGame(void * arg);
void * sendMessage(void * arg);
void * receiveMessage(void * arg);
void * handleSend(void * arg);
void * handleReceive (void * arg);
//extern Game *tankTrouble;


void updateMsgReceived(int userId,int msgNumber );
void updateAckDatabase(int userId,int startPoint,vector<bool> previousMsg);// this takes in the bit array and the starting integer point
void decodeMsg(string msg);
void initialiseGlobalData();
void resetData();
void calculateRTT(int packetId);
void shootBullet(float centreX,float centreY,float angleXaxis,float velocity);


string generateAckPacket (int packetId,int userId);
string makeMessage ( vector <string> inputSet);
vector<string> getStringsOut(string input);  //takes in string in $k#k#$ format and gives back a string vector


bool joinNewGame(int fd,string targetIp,int targetPort);
void joinUsers(int fd);
void sendPlayerDataToAll();
void startInSync();
#endif
