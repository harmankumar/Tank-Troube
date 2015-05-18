#include "helperfn.h"





//Gui data///

initialData guiResult;
/////////



int window_width=600;
int window_height=600;
double Pi = acos(-1);
const char* res="data/";
string selfIpAddr="";
string targetIpAddr="";
double deltaKeyPress = 10;
double smalldeltaKeyPress = 5;
double timePerframe = 1.0/60.0;
//recent
string myIp = "";
vector<string> userIP;
//
//vector <int> keystate(255);
float widthWall = 1.5;		// when we incorporate this..we need to consider wall as recatangularvector <vector <float> > mazeVertWalls;
vector <vector <float> > mazeHoriWalls;
vector <vector <float> > vertRight;
vector <vector <float> > vertLeft;
vector <vector <float> > horiUpper;
vector <vector <float> > horiLower;
vector <vector <float> > quad1;
vector <vector <float> > quad2;
vector <vector <float> > quad3;
vector <vector <float> > quad4;
vector <int> typeMazeWall;
vector <vector <float> > topLeftPoints;
vector <vector <float> > topRightPoints;
vector <vector <float> > bottomLeftPoints;
vector <vector <float> > bottomRightPoints;

////
int myPort;	/* hard-coded port number for self */
vector<int> userPort;	/* hard-coded port number for remoter user */
int bufferSize;
int programState;
vector<bool> tempAck;
int numOfUsers;
vector<bool> usersJoined;
bool receivedAllUserData;
////


int myUserId;
vector< int > mostRecentMsgNum;
vector<bool> msgSent;
vector< vector <bool> > msgReceived;
vector< vector <bool> > ackReceived;
vector<string> lastCrucialMsgs;
vector< int > sendTime;     // stores the time when a particuar message was sent
vector< int > receiveTime;  // receiveTime stores information about the time acknoledgement for a particular code was received
vector< int > RTT;
vector< time_t > lastUserActivity;
vector< bool > userActive;
pthread_mutex_t mtx;
time_t globalTimer;

queue< sendQueueItem > sendQueue;
queue< string > receiveQueue;
string tmpForWindow;



float absFloat(float x)
{
	if (x >= 0.0)
		return x;
	else
		return (-1.0)*x;

}

string makeMessage (vector <string> inputSet)
{
    string outputString="$";
    for(vector<string>::iterator iter=inputSet.begin();iter!=inputSet.end();  )
    {
    outputString+=*(iter);
    iter++;
    if(iter==inputSet.end())
       outputString+="$";
    else
        outputString+="#";
    }
    return outputString;
}

string attachCrucial (string mainMsg)
{
    string outputString = mainMsg.substr(0,mainMsg.size()-1);
    int i;
    outputString+="#";
    for(i=0;i<lastCrucialMsgs.size();i++)
    {
        outputString+=lastCrucialMsgs[i];
        outputString+="#";
    }
    for(;i<numOfCrucialMsg;i++)
    {
        outputString+="VOID#";
    }
    int temp=outputString.size();
    outputString[temp-1]='$';
    return outputString;
}



/*
void initialiseStart(int joinFlagGui ,int numPlayersGui,string selfIpGui,int selfPortGui, string targetIpGui,int targetPortGui)
{
    //Gui data///
    ::joinFlagSet=joinFlagGui ;
    ::numPlayersSet=numPlayersGui;
    ::selfIpSet=selfIpGui;
    ::selfPortSet=selfPortGui ;
    ::targetIpSet=targetIpGui;
    ::targetPortSet=targetPortGui;
}

*/
