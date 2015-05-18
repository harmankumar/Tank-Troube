#include "network.h"
#include <fstream>
#include <SFML/Audio.hpp>


#define numOfAcks 20
#define wrapAroundSize 20480
#define numOfCrucialMsg 1
#define maxRTT 1
#define res ""

Game *tankTrouble;
/*Bullets are shot and the data is also transmitted but not processed*/


/*
Very important ... the game does not start unless all the users have joined in  .. the global numOfConnectingAttempts has no effect

*/

/*
// userId stars from 0
// Message id starts from 0
// Each acknowledgement carries acknowledgement of prevous 20 Messages for now
// User Id starts from 0 onwards . The size of ackReceived array would be same as number of users , in this case one array would be left
unused but this makes addressing is the vector pretty easy

Format of acknowledgement message  [30,1,0,0,1,1] this means ack for msg 30 is received and msg 29 ->Recvd,28->NotRecvd,..and so one
in this case ack of 5 previous message is received . for msg num 4 the message would be [4,1,0,0,1,X] the last thing is don' care , can be one or zero


IMPORTANT mostRecentMsgNum stores (serial number of most recent message sent + 1 for the myUser
For other users it stores information regarding most recent message received from other user

Curently information is sampled after key has been unpressed

CAUTION: AngleXAxis is positive in clockwise direction

TODO
When information about all players is sent to other players in order to start the game then message is sent only once ...
Ackowledgment for such message is sent and received  but this is slightly jugaadu
Fault in sending this message will have to be handled   .. Maybe ecery player asks for this information after certain stipulated time period in order
to confirm whether or not all the users have joined in


Now I am maintaining a global called programState
programState=TempServerMode,ExchangingUserData,GameStarted  ==0,1,2  ---> Change this to enumeration later on

Date 16 April
InitialiseGlobal was called twice hence probably we had that double free corrruption error.
mostRecentMsgNum was updated when the message was actually sent and this causes multiple messages to have same packet id
Now this is updated as soon as message to be sent to all users is created in the broadcast function

April 17
Now each user tries to connect at most 5 times to the requisite player and then no further attempts are made
Also a time-out is fixed for receiving back accepted request or failure request
...The case where in a user tries to join in middle of an ongoing game is not handled
ProgramState:
0--Game Setup has started
1--The user has successfully joined in the game
2--All user data has been sent or received
3--Now all players have started to play in sync
4--Game has ended unexpectedly
*/

int main()
{
    /*  //check for file presence and handle in qt  for any cancellation
    sf::Music music;
    if (!music.openFromFile("song.ogg"))
        return -1; // error
    music.play();*/
    sf::Music music;
    music.openFromFile("data/background.ogg");
    music.setLoop(1);
    music.play();


	std::ifstream fin ;
	fin.open(("QTGui/out.txt"));

    cout<<"join or leave"<<endl;
    string flag;
    fin>>flag;
    cout<<atoi(flag.c_str())<<endl;

    cout<<" no. player"<<endl;
    string num;
    fin>>num;
    cout<<atoi(num.c_str())<<endl;

    cout<<"selfip"<<endl;
    string Ip;
    fin>>Ip;
    cout<<Ip<<endl;


    cout<<"selfPort"<<endl;
    string port1;
    fin>>port1;
    cout<<atoi(port1.c_str())<<endl;

    cout<<"targetIp"<<endl;
    string Ip2;
    fin>>Ip2;
    cout<<Ip2<<endl;

    cout<<"targetPort"<<endl;
    string target;
    fin>>target;
    cout<<atoi(target.c_str())<<endl;
    string a;
    //cin >>a;
    //cin>>a;

    if(Ip2=="AI" )
        {
            switch(atoi(flag.c_str()))
            {
                case 1:
                {
                    cout<<"Classic"<<endl;
                    system("cd AI");
                    system("./AI");
                    break;
                }
                case 2:
                {

                    break;
                }
                case 3:
                {
                    break;
                }
            }
        }
   else if(atoi(flag.c_str())!=-1)
        start(atoi(flag.c_str()),atoi(num.c_str()),Ip,atoi(port1.c_str()),Ip2,atoi(target.c_str()) );
        else cout<<"make run to restartthe game !!"<<endl;
    /*cout<<"join or leave"<<endl;
    int flag;
    cin>>flag;
    cout<<flag<<endl;
    cout<<" no. playe"<<endl;
    int num;
    cin>>num;
    cout<<num<<endl;
    cout<<"selfPort"<<endl;
    int port1;
    cin>>port1;
    cout<<port1<<endl;
    cout<<"targetPort"<<endl;
    int target;
    cin>>target;
    cout<<target<<endl;

    start(flag,num,"",port1,"",target);*/

}

int start(int joinFlag ,int numPlayers,string selfIp,int selfPort, string targetIp,int targetPort)
{
    programState=0;
    bufferSize=2048;
    //  initialiseGlobalData();
    /*
    if(argc>1)
        myPort=atoi(argv[1]);
    else
    {
        cout<<"First Port address not provided"<<endl;
        return 0;
    }*/
    myPort=selfPort;
    myIp=selfIp;
    cout<<"MYIP.........."<<myIp<<endl;
	struct sockaddr_in myAddress;	        // our address
	int fd;				        // our socket
	// create a UDP socket
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("cannot create socket\n");
		return 0;
	}
	// bind the socket to a valid IP address and a specific port. This port and address will be used to communicate the this user
	memset((char *)&myAddress, 0, sizeof(myAddress));
	myAddress.sin_family       = AF_INET;
	myAddress.sin_addr.s_addr  = htonl(INADDR_ANY);
	myAddress.sin_port         = htons(myPort);

	if (bind(fd, (struct sockaddr *)&myAddress, sizeof(myAddress)) < 0)    // binding my address to the file descriptor
	{
		perror("bind failed");
		return 0;
	}
    pthread_t thread1,thread2,thread3,thread4;

    pthread_create(&thread1,0,handleSend,(void *)&fd);
    int option;
   // cout<<"You want to start a new game or join an existing game\n"; //1 for new game else join existing game
    //cin>>option;
    option=joinFlag;
    numOfUsers=numPlayers;
    if(option==1)
    {
        joinUsers(fd);
        programState=1;
        pthread_create(&thread2,0,receiveMessage,(void *)&fd);// These two threads need to be started here  as these are
        pthread_create(&thread3,0,handleReceive,NULL);          // needed in exchanging all user data
     //   cout<<"Sending data to all users"<<endl;
     //   cout<<"userPortsize"<<userPort.size()<<endl;
        while(sendQueue.size()>0)
        {
            //cout<<"Send queue size"<<sendQueue.size()<<endl;
        }
      //  cout<<"hi"<<endl;
        sendPlayerDataToAll();
        // In the function above ProgramState will be change to 2

        if(programState==2)
        {
        //    cout<<"After sending all data:"<<programState<<endl;

          //  cout<<"All the users have joined in now...\nNow  we have to make all of them start in sync and send data exchange data about players with those who have joined in"<<endl;

            for(int iter=0;iter<numOfUsers;iter++)
            {
            //    cout<<"UserId:"<<iter<<endl;
              //  cout<<"IP:"<<userIP[iter]<<endl;
              //  cout<<"Port:"<<userPort[iter]<<endl;
            }

            
            startInSync();


            char halt;
            

            programState=3;
            for(int iter=0;iter<numOfUsers;iter++)
            {
                time(&lastUserActivity[iter]);
            }
            tankTrouble->gameWindow.setVisible(true);
            tankTrouble->run();
            programState=4;// Basically due to some error this run() function has terminated so to handle we have this programstate
            // Though it might happen to serve very little purpose
        }
        else
        {
        
            for(int iter=0;iter<numOfUsers;iter++)
            {
                if(!tempAck[iter])
                {
                    cout<<iter<<endl;
                }
            }
        }

      //  cin>>halt;
    }
    else
    {
        bool joinSuccess=joinNewGame(fd,targetIp,targetPort);
        if(joinSuccess)// The game is successfully joined
        {
            programState=1;
            pthread_create(&thread2,0,receiveMessage,(void *)&fd);
            pthread_create(&thread3,0,handleReceive,NULL);
        

            bool tempHalt=false;
            while(!receivedAllUserData);
            while(programState!=2);  // This is so that nothing happens further until all users have joined in and their data is received
            while(sendQueue.size()>0)
            {
                sendQueueItem tmp=sendQueue.front();
            }
        
            char halt;
            startInSync();
            programState=3;
        
        //    cin>>halt;
            for(int iter=0;iter<numOfUsers;iter++)
            {
                time(&lastUserActivity[iter]);
            }
            tankTrouble->gameWindow.setVisible(true);
            tankTrouble->run();
            programState=4;// quite useless  to do this but anyways
        }
        else
        {
            cout<<"Failed to join the game... See what to do"<<endl;
        }
    }

    for(int iter=0;iter<numOfUsers;iter++)
    {
        cout<<iter<<"::"<<usersJoined[iter]<<endl;
    }

}

void joinUsers(int fd)
{
    //cout<<"Enter number of players that this game will have\n";
    //cin>>numOfUsers;
    initialiseGlobalData();
    myUserId=0;
    userPort[myUserId]=myPort;
    userIP[myUserId]=myIp;
    char * buf;
	buf = new char [bufferSize];
    int recvLength;
    struct sockaddr_in remAddress;
	socklen_t remAddressLength = sizeof(remAddress);
    int joinedUsers=1;//  Now the player starting the game has userId=0s
    //cout<<"Num of users"<<numOfUsers<<endl;
    while(joinedUsers<numOfUsers)
    {
        //cout<<"Waiting"<<endl;
        recvLength = recvfrom(fd, buf, bufferSize , 0,(struct sockaddr *)&remAddress, &remAddressLength);
        if (recvLength > 0)
        {
            buf[recvLength] = 0;
         //   cout<<"Received Message"<<buf<<"-->"<<recvLength<<endl;
            string tempS=string(buf);
            vector<string> stringVector = getStringsOut(tempS);
          //  cout<<"GotStringVector"<<endl;
            cout<<stringVector[0]<<endl;
            if(stringVector.size()==3 && stringVector[0]=="JoinRequest")  // These constraints are to ensure to some extent that we have correct format
            {

                userIP[joinedUsers]=stringVector[1];
                userPort[joinedUsers]=atoi(stringVector[2].c_str());
            //    cout<<"Incoming request from port:"<<userPort[joinedUsers];
                stringVector.resize(0);
                stringVector.push_back("RequestAccepted");

                sprintf(buf,"%d",joinedUsers);  // Giving the new user the userId
                stringVector.push_back(buf);

              //  cout<<"\nNewUserId:"<<joinedUsers<<endl;
                sprintf(buf,"%d",numOfUsers);  // Telling the new user about the number of players that will be in this game

                stringVector.push_back(buf);

                sendQueueItem toSend;
                toSend.destPort=userPort[joinedUsers];
                toSend.destIp=userIP[joinedUsers];
                toSend.isAck=true;
                toSend.message=makeMessage(stringVector);
                sendQueue.push(toSend);
                joinedUsers++;
            }
            else  // if some stray or wrong format message arrives
            {
                cout<<"Stray information packet\n";
            }
        }
        else
            printf("uh oh - something went wrong!\n");

    }
    delete buf;
    cout<<"Returning from function"<<endl;
}

bool joinNewGame(int fd,string targetIp,int targetPort)
{
    int portToJoin;
    cout<<"Enter port number of first player"<<endl;
    //cin>>portToJoin;
    portToJoin=targetPort;
    char * buf;
	buf = new char [bufferSize];
    int recvLength;		        // # bytes in acknowledgement message
    struct sockaddr_in remAddress;	        // remote address
	socklen_t remAddressLength = sizeof(remAddress);		// length of addresses
    bool joinSuccess=false;

    vector<string> stringVector;
    stringVector.push_back("JoinRequest");
    stringVector.push_back(myIp);
    string tempS;
    sprintf(buf,"%d",myPort);
    stringVector.push_back(buf);

    sendQueueItem toSend;
    toSend.destPort=portToJoin;
    toSend.destIp=targetIp;
    toSend.isAck=true;  // isAck should be set to true else sendMessage function tries to eter sent log for the message
    toSend.message=makeMessage(stringVector);
    int numOfTries=0;
    while(!joinSuccess && numOfTries<numOfConnectingAttempts)
    {
        numOfTries++;
        sendQueue.push(toSend);
        cout<<"Now waiting for approval\n";
        recvLength = recvfrom(fd, buf, bufferSize , 0,(struct sockaddr *)&remAddress, &remAddressLength);
        if (recvLength > 0)
        {
            buf[recvLength] = 0;
            cout<<"Received Message"<<buf<<"-->"<<recvLength<<endl;
            tempS=string(buf);
            stringVector.clear();
            stringVector=getStringsOut(tempS);
            if(stringVector.size()==3 && stringVector[0]=="RequestAccepted")
            {
                myUserId    =atoi(stringVector[1].c_str());
                numOfUsers  =atoi(stringVector[2].c_str());
                initialiseGlobalData();

                userPort[0]=portToJoin; // The main user who has started the game is stored at 0 positon for her/his user id is so
                userIP[0]=targetIp;

                userPort[myUserId]=myPort;// Setting up port id for the user her/himself
                userIP[myUserId]=myIp;
                cout<<"User successfully joined"<<endl;
                joinSuccess=true;// As of now no more repeated attempts but can have such a thing
            }
            else
            {
                cout<<"Joining failed .Try again"<<endl;
            }
        }
        else
            printf("uh oh - something went wrong!\n");

    }
    delete buf;
    return joinSuccess;
}

void sendPlayerDataToAll()
{
    cout<<"Now sending all players' data to all players"<<endl;
    vector<string> stringVector;
    char * buf;
    buf=new char [100];
    stringVector.push_back("UserData");
    stringVector.push_back(myIp);
    sprintf(buf,"%d",myPort);  // pushing mainUser port
    stringVector.push_back(buf);

    for(int iter=0;iter<numOfUsers;iter++)
    {
        cout<<"UserId:"<<iter<<endl;
        cout<<"Port:"<<userPort[iter]<<endl;

        sprintf(buf,"%d",iter);  // pushing userId
        stringVector.push_back(buf);

        stringVector.push_back(userIP[iter]);

        sprintf(buf,"%d",userPort[iter]); // pushing corresponding userPort Number
        stringVector.push_back(buf);
    }

    sendQueueItem toSend;
    toSend.isAck=true;  // isAck should be set to true else sendMessage function tries to enter sent log for the message
    toSend.message=makeMessage(stringVector);
    bool tempBool=false;
    tempAck[myUserId]=true;
    for(int i=0;/*i<numOfConnectingAttempts && */!tempBool;i++)//  This programState has to be updated by lobby msg decodee
    {

        for(int iter=0;iter<numOfUsers;iter++)
        {
            cout<<"Iter"<<iter<<endl;
            //This is for the purpose of giving in some gap between the messages being sent.. As of now this message is sent once
                        // Need to handle the case when this message gets lost and nothing happens
            if(iter!=myUserId && !tempAck[iter])
            {
                cout<<"Pushing data to user:"<<iter<<endl;
                toSend.destIp=userIP[iter];
                toSend.destPort=userPort[iter];
                sendQueue.push(toSend);
            }
        }
        usleep(100);
        tempBool=true;
        for(int tmp=0;tmp<numOfUsers;tmp++)
        {
            if(tmp!=myUserId)
            {
                if(tempBool && tempAck[tmp])
                {
                    tempBool=true;
                }
                else
                {
                    tempBool=false;
                }
            }
        }
    }
    if(tempBool)
        programState=2;

    cout<<"Returning from  function"<<endl;
}

void startInSync()
{
    bool allJoined = false;
    char * buf;
    buf=new char [100];
    for(int iter=0;/*iter<10 && */!allJoined;iter++)// Message exchangin will happen at max 10 times and if it gets conffirmed that all users have joined in then no more message sending
    {
        cout<<"All players not up yet!"<<endl;
        vector<string> tempVector;
        tempVector.push_back("StartGame");

        sprintf(buf,"%d",myUserId);
        tempVector.push_back(buf);
        //cout<<"B1"<<endl;
        for(int iter2=0;iter2<numOfUsers;iter2++)
        {
            sprintf(buf,"%d",iter2);
            tempVector.push_back(buf);// Pushing in userNumber
           // cout<<"B2"<<endl;
            if(!usersJoined[iter2] && iter2!=myUserId)
                sprintf(buf,"%d",0);
            else
                sprintf(buf,"%d",1);
          //  cout<<"B3"<<endl;
            tempVector.push_back(buf);// Pushing in status whether ready to join the game or not
        }
        //cout<<"B4"<<endl;
        string msg=makeMessage(tempVector);
        cout<<"Message:"<<msg<<endl;
        for(int uIter=0;uIter<numOfUsers;uIter++)
        {
            if(uIter!=myUserId)
            {
                sendQueueItem toSend;
                toSend.isAck=true;  // isAck should be set to true else sendMessage function tries to enter sent log for the message
                toSend.message=msg;
                toSend.destPort=userPort[uIter];
                toSend.destIp=userIP[uIter];
                sendQueue.push(toSend);
                usleep(1000);
                sendQueue.push(toSend);
                usleep(1000);
                sendQueue.push(toSend);
                usleep(1000);
                sendQueue.push(toSend);
                usleep(1000);
                sendQueue.push(toSend);
            }
        }
       // sleep(1);
        cout<<"Message sending round :"<<iter<<endl;
        // check if all users have joined in with help of information received in from all users
        bool tempB=true;
        for(int tmpIter=0;tmpIter<numOfUsers;tmpIter++)
        {
            if(usersJoined[tmpIter] && tempB)
            {
                tempB=true;
            }
            else
                tempB=false;
        }
        allJoined=tempB;
    }

    delete buf;
    cout<<"All Synced"<<endl;
}

void decodeLobby(string message)
{
    vector<string> tempStringVector=getStringsOut(message);
    if(tempStringVector[0]=="LobbyAck" && tempStringVector.size()>1 && programState==1)  // i.e the the user is the tempServer,.. Acknowledgement for info about all player is received by the user from whom this message is received
    {
        cout<<"Received Lobby Ack"<<endl;
        int tempAckUID= atoi(tempStringVector[1].c_str());
        if(tempAckUID<numOfUsers)
            tempAck[tempAckUID]=true;
        else
            cout<<"Invalid userId received:"<<tempAckUID<<endl;
    }
    else if(tempStringVector[0]=="UserData" && tempStringVector.size()>1 && programState==1)// information is being sent to this user
        {
            // update relevant data
            string tempMainIP=tempStringVector[1];
            int tempMainPort=atoi(tempStringVector[2].c_str());

            for(int iter=3;iter<tempStringVector.size();iter+=3)
            {
                int tempUID= atoi(tempStringVector[iter].c_str());
                if(tempUID<numOfUsers)
                {
                    userIP[tempUID]=tempStringVector[iter+1];
                    userPort[tempUID]=atoi(tempStringVector[iter+2].c_str());
                }
                else
                    cout<<"Invalid userId received"<<tempUID<<endl;
            }
            // send back an ackowldgement
            sendQueueItem toSend;
            toSend.destPort=tempMainPort;
            toSend.destIp=tempMainIP;
            toSend.isAck=true;  // isAck should be set to true else sendMessage function tries to eter sent log for the message
            vector<string> tempVector;
            tempVector.push_back("LobbyAck");
            char * buf;
            buf=new char [100];
            sprintf(buf,"%d",myUserId);
            tempVector.push_back(buf);

            toSend.message=makeMessage(tempVector);
            sendQueue.push(toSend);
            sendQueue.push(toSend);
            sendQueue.push(toSend);
            receivedAllUserData=true;
            programState=2;
            cout<<"Hey Program state =2 now "<<endl;
        }
        else if(tempStringVector[0]=="StartGame" && programState ==2)// this programState thing ensure that wrong info in not taken in
            {
                for(int iter=0;iter<numOfUsers;iter++)
                {
                    int tempUser=atoi(tempStringVector[2*iter+2].c_str());
                    int tempJoined=atoi(tempStringVector[2*iter+3].c_str());
                    if(!usersJoined[tempUser])
                        if(tempJoined==1)
                            usersJoined[tempUser]=true;
                        else
                            usersJoined[tempUser]=false;
                }
                usersJoined[myUserId]=true;
            }
            else
            {
                cout<<"ProgramState:"<<programState<<endl;
                cout<<"Wrong Message format:"<<message<<endl;
            }
}

void * handleGame(void * arg)
{
   // Game tankTrouble;

    tankTrouble->run();
}

void initialiseGlobalData()
{
    numBullets=0;
    bulletShot.resize(100);
    ///
    powerUpVector.resize(100);
    ///
    char a[19];
    sprintf(a,"%d",myUserId);
    tmpForWindow=string(a);

    cout<<"Heyy .."<<tmpForWindow<<endl;
	tankTrouble = new Game;
    tankTrouble->gameWindow.setVisible(false);
    receivedAllUserData=false;
    usersJoined.resize(numOfUsers);
    userPort.resize(numOfUsers);
    userIP.resize(numOfUsers);
    tempAck.resize(numOfUsers,false);

    lastUserActivity.resize(numOfUsers);
    userActive.resize(numOfUsers);

    //time(&timeLastSent);
    mostRecentMsgNum.resize(numOfUsers,0);  // This might cause packet numbers to be off by in sender and user
    //mostRecentMsgNum[myUserId]=0;  // this is important as message numbers start from 0
    msgSent.resize(wrapAroundSize,false);
    msgReceived.resize(numOfUsers);
    ackReceived.resize(numOfUsers);
    //lastCrucialMsgs.resize(numOfCrucialMsg);
    sendTime.resize(wrapAroundSize,0);
    receiveTime.resize(wrapAroundSize,0);
    RTT.resize(wrapAroundSize,0);
    for(int iter=0;iter<numOfUsers;iter++)
    {
        msgReceived[iter].resize(wrapAroundSize,false);
        ackReceived[iter].resize(wrapAroundSize,false);
    }
    //pTanks.resize(numOfUsers);
    pthread_mutex_init(&mtx,NULL);
    bufferSize=256;
}

void * receiveMessage(void * arg)
{
    int fd =*(int *)arg;
    char * buf;
	buf = new char [bufferSize];
    int recvLength;		        // # bytes in acknowledgement message
    struct sockaddr_in remAddress;	        // remote address
	socklen_t remAddressLength = sizeof(remAddress);		// length of addresses
    while(1)
    {
    /*        cout<<"Waiting on port:"<<myPort<<endl;
        cout<<"File descriptor:"<<fd<<endl;
        cout<<"buffersize:"<<bufferSize<<endl;*/
        recvLength = recvfrom(fd, buf, bufferSize , 0,(struct sockaddr *)&remAddress, &remAddressLength);
        if (recvLength > 0)
        {
            buf[recvLength] = 0;
//            cout<<"Received Message"<<buf<<"-->"<<recvLength<<endl;
            receiveQueue.push(string(buf));
           // cout<<"No. of message on Queue:"<<receiveQueue.size()<<endl;
        }
        else
            printf("uh oh - something went wrong!\n");
    }
}

void * handleSend(void * arg)
{
    while(true)
    {
        if(sendQueue.empty())
        {
            continue;
        }
        pthread_mutex_lock(&mtx);
        sendQueueItem toSend = sendQueue.front();
        sendQueue.pop();
        pthread_mutex_unlock(&mtx);

        struct sendInfo *info = new sendInfo;
        info->sendMessage=toSend.message;
       // cout<<"In handle send send message"<<info->sendMessage<<endl;
        info->socketD=*(int *)arg;
        info->destPort=toSend.destPort;
        info->isAck=toSend.isAck;
        info->destIp=toSend.destIp;

        vector<string> msgCheck = getStringsOut(info->sendMessage);
        sendMessage((void *)info);

        if(sendQueue.size()>1000)
        {
            cout<<"Exit because sendQueue has size:"<<sendQueue.size()<<endl;
            exit(0);
        }
    }

}

void * sendMessage(void *arg)
{
    int count=0;
    struct sockaddr_in remAddress;	        // remote address
    struct sendInfo  info= *(struct sendInfo *)arg;
    const char * buf ;// = info.sendMessage.c_str();
    int fd;
	socklen_t remAddressLength = sizeof(remAddress);		// length of addresses
	memset((char *) &remAddress, 0, sizeof(remAddress));
	remAddress.sin_family = AF_INET;
	remAddress.sin_port = htons(info.destPort);
	remAddress.sin_addr.s_addr=inet_addr(info.destIp.c_str());
    //remAddress.sin_addr.s_addr=inet_addr("192.168.30.134");
    fd=info.socketD;
    buf=new char [info.sendMessage.size()]; //using the strings size to fix the size
    buf = info.sendMessage.c_str();

    //cout<<"Sending to port "<<info.destPort<<"--"<<buf<<"AND IP:"<<info.destIp<<endl;

    if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remAddress, remAddressLength  )<0)
    {
        perror("sendto");
        exit(1);
    }
    else
    {
       // cout<<"SENT"<<endl;
        if(!info.isAck && programState==3) // if the message sent is not an acknowledgement then the sendTime is to be updated for that packet id
        {
          //  cout<<"Hi.."<<endl;
            time(&globalTimer);
            sendTime[mostRecentMsgNum[myUserId]] = globalTimer;
    //            mostRecentMsgNum[myUserId]++;
      //      cout<<"Send time  updated for "<<mostRecentMsgNum[myUserId]-1<<" Time:"<<sendTime[mostRecentMsgNum[myUserId]-1]<<endl;
        }
    }


    // now receive an acknowledgement from the server
}

void * handleReceive (void * arg)
{
    while(true)
    {
        if(receiveQueue.empty())
        {
            continue;
        }
        pthread_mutex_lock(&mtx);
        string rcvdString = receiveQueue.front();
        receiveQueue.pop();
        pthread_mutex_unlock(&mtx);

        if(programState<3)//Exchanging user data
            decodeLobby(rcvdString);
        else if (programState==3)// Game active
            decodeMsg(rcvdString);

        // cout<<"Size of receiveQ:"<<receiveQueue.size()<<endl;
        //Send AckPacket if it should be sent .. Decoder takes care of that
    }

}

string generateAckPacket (int packetId,int userId)
{
    if(packetId>=0 && userId>=0)
    {
        vector< string > ackPacketV;
        char tmp[40];
        string temp2="";
        sprintf(tmp,"%d",myUserId);
        ackPacketV.push_back(tmp);// pushing userId
        temp2="0";
        ackPacketV.push_back(temp2);// pushing Instruction code
        temp2="111";  // This is acknowledgement packet number
        ackPacketV.push_back(temp2);// pushing Instruction code

        sprintf(tmp,"%d",packetId);
        ackPacketV.push_back(tmp); // pushing packetId whose acknowledgement is being sent

        temp2="";
        for(int iter=1;iter<=numOfAcks && packetId-iter>=0;iter++)
        {
            if(msgReceived[userId][packetId-iter])
                temp2+="1";
            else
                temp2+="0";
        }
        while(temp2.size()<numOfAcks)
        {
            temp2+="0";
        }
       // cout<<"Size of bitStream"<<temp2.size()<<endl;
    //    cout<<"PacketId"<<packetId<<endl;
     //   cout<<"AckBitStream:"<<temp2<<endl;
        ackPacketV.push_back(temp2);
        return (makeMessage(ackPacketV));
    }
    else
    {
//        cout<<"Packet Id is invalid:"<<packetId<<endl;
  //      cout<<"Or User Id is invalid:"<<userId<<endl;
        return "";
    }
    //    ackPacketV.push_back(string(packetId));
}

vector<string> getStringsOut(string input)  //takes in string in $k#k#$ format and gives back a string vector
{
	string data=input;
	vector<string> ansSet ;
	string segment;
	for(int i=0;i<data.size();i++)
	{
		if(data[i]=='$')
		{
			ansSet.push_back(segment);
			segment="";
			continue;
		}
		segment=segment+data[i];

	}

	segment="";
	vector<string> finansSet;
	for(int i=0;i<ansSet[1].size();i++)
	{
		if(ansSet[1][i]=='#'  )
		{
			finansSet.push_back(segment);
			segment="";
			continue;
		}
		segment=segment+ansSet[1][i];
		if(i==ansSet[1].size()-1)
		{
			finansSet.push_back(segment);
		}

	}
	return finansSet;
}

void updateMsgReceived(int userId,int msgNumber )
{
    msgReceived[userId][msgNumber]=true;
}

void updateAckDatabase(int userId,int startPoint,vector<bool> previousMsg)// this takes in the bit array and the starting integer point
{
    int numOfPositives=0;
    if(userId>=0 && startPoint >=0)
    {
        ackReceived[userId][startPoint]=true;// setting the start point
        calculateRTT(startPoint);
        for(int iter=1;iter<= numOfAcks && (startPoint - iter >=0) ;iter++)
        {
            if(!ackReceived[userId][startPoint - iter])
            {
                ackReceived[userId][startPoint - iter]=previousMsg[iter];

                cout<<"Ack revd for packet:"<<startPoint-iter<<":"<<previousMsg[iter]<<endl;
                calculateRTT(startPoint-iter);
                // cout<<"RTT successfully calculated"<<endl;
            }
            if(previousMsg[iter])
            {
                    numOfPositives++;
            }
        }
       // if(numOfPositives<10 )
        //    cout<<"  Here..."<<numOfPositives<<":: we have network congestion between users "<<myUserId<<" and "<<userId<<endl;
    }
    else
    {
        cout<<"Invalid userId or startPoint "<<userId<<"::"<<startPoint<<endl;
    }


}

void decodeMsg(string msg)
{
    vector< string > msgVector = getStringsOut( msg );
    int vectorIter=0;
    if(msgVector[0]=="StartGame" || msgVector[0]=="UserData")
    {
        cout<<"Wrong message format"<<endl;
    }
    else
    {
        for(int iter=0;iter<=numOfCrucialMsg && vectorIter<msgVector.size();iter++)
        {
            //cout<<"Testing"<<msgVector[vectorIter+0]<<endl;
            if (msgVector[vectorIter+0]=="VOID")
                break;
            int tempUserId = atoi(msgVector[vectorIter+0].c_str());
            time(&lastUserActivity[tempUserId]);
            //cout<<"UserId:"<<tempUserId<<endl;
            int instCode = atoi(msgVector[vectorIter+1].c_str());
            //cout<<"InstructionCode:"<<instCode<<endl;
            switch (instCode)
            {
                case 0 : // This is an acknowledgement packet
                {
                    cout<<"Got an acknowledgment"<<endl;
                    int ackSerialNum = atoi(msgVector[vectorIter+2].c_str());  // this is the acknowledgement number that is sent --probably useless
                     // update global serial number
                    int msgSerialNum = atoi(msgVector[vectorIter+3].c_str());// this is the serial number of the message whose acknowledgement is received from the user

                    // Now get the bit array for previous messages
                    // Size of bit array is  fixed and equals 20
                    string bitString = msgVector[vectorIter+4];
                    vector <bool> boolForBitString;
                    boolForBitString.resize(numOfAcks,false);
                    for(int iter=0;iter<numOfAcks;iter++)
                    {
                        if( bitString[iter] = '1' )
                            boolForBitString[iter]=true;
                        else
                            boolForBitString[iter]=false;
                    }
                    updateAckDatabase(tempUserId,msgSerialNum,boolForBitString);

                     // calculate RTT and update that too  .. RTT is updated in updateAckDatabase
                     vectorIter+=5;
                    break;
                }
                case 1 : // This is for position packet
                {
                    // if serial number of this packet is less than that of most recent one then we don't bother decoding that information

                    int serialNum = atoi(msgVector[vectorIter+2].c_str());
                    cout<<"Serial Num:"<<serialNum<<endl;
                    if ((serialNum > mostRecentMsgNum[tempUserId]) &&  (!msgReceived[tempUserId][serialNum]))
                    {
                        float XCord = atof(msgVector[vectorIter+3].c_str());

                        float YCord = atof(msgVector[vectorIter+4].c_str());
                        float angle = atof(msgVector[vectorIter+5].c_str());
                        float velocity = atof(msgVector[vectorIter+6].c_str());

                        // cout<<"XCord"<<XCord<<endl;
                        // cout<<"Ycord"<<YCord<<endl;
                        // cout<<"angle"<<angle<<endl;
                        // cout<<"velocity"<<velocity<<endl;
                        updateMsgReceived(tempUserId,serialNum);
                        mostRecentMsgNum[tempUserId]=serialNum;
                        // cout<<"Going to update"<<endl;
                        tankTrouble->updateOtherTanks(tempUserId,XCord,YCord,angle);// Update information  in tank class
                        // cout<<"Information successfully updated"<<endl;

                        sendQueueItem toSend;
                        toSend.destPort=userPort[tempUserId];
                        toSend.destIp=userIP[tempUserId];
                        toSend.isAck=true;
                        toSend.message=generateAckPacket(serialNum,tempUserId);
                        sendQueue.push(toSend);
                        // cout<<"Ack pushed on queue"<<endl;
                    }
                    else
                    {
                        cout<<"In else"<<endl;
                    }

                    vectorIter+=7;
                    break;
                }
                case 2 : // This is bullet shot packet
                {

                    int serialNum   = atoi(msgVector[vectorIter+2].c_str());
                    float XCord     = atof(msgVector[vectorIter+3].c_str());
                    float YCord     = atof(msgVector[vectorIter+4].c_str());
                    float angle     = atof(msgVector[vectorIter+5].c_str());
                    float velocity  = atof(msgVector[vectorIter+6].c_str());


                        // cout<<"Serial Num:"<<serialNum<<endl;
                        // cout<<"XCord"<<XCord<<endl;
                        // cout<<"Ycord"<<YCord<<endl;
                        // cout<<"angle"<<angle<<endl;
                        // cout<<"velocity"<<velocity<<endl;
                    if(!msgReceived[tempUserId][serialNum])
                    {
                        updateMsgReceived(tempUserId,serialNum);
                        if(mostRecentMsgNum[tempUserId]< serialNum)
                            mostRecentMsgNum[tempUserId]=serialNum;
                    //    cout<<"Bullet shot by:"<<tempUserId<<"  PID:"<<serialNum<<endl;
                        shootBullet(XCord,YCord,angle,velocity);
                    }
                    // cout<<"Bullet shot"<<endl;
                    // Update information  in bullet class
                    vectorIter+=7;
                    break;
                }
                case 3 : // This is tank destruction packet
                {
                    int serialNum = atoi(msgVector[vectorIter+2].c_str());
                    float XCord = atof(msgVector[vectorIter+3].c_str());
                    float YCord = atof(msgVector[vectorIter+4].c_str());
                    int bulletId = atoi(msgVector[vectorIter+5].c_str());
                    if(!msgReceived[tempUserId][serialNum])
                    {
                        updateMsgReceived(tempUserId,serialNum);
                        if(mostRecentMsgNum[tempUserId]< serialNum)
                            mostRecentMsgNum[tempUserId]=serialNum;
                      //  cout<<"Tank destruction information received PID:"<<serialNum<<"  for user:"<<tempUserId<<" BulletId:"<<bulletId<<endl;
                        tankTrouble->destroyOtherTanks(tempUserId,bulletId,XCord,YCord);// Update information  in tank class
                        // Update information  in tank class
                    }
                    vectorIter+=6;
                    break;
                }
                case 4 : // This is for pinging the  user
                {
                    int serialNum = atoi(msgVector[vectorIter+2].c_str());
                    updateMsgReceived(tempUserId,serialNum);
                    if(mostRecentMsgNum[tempUserId]< serialNum)
                        mostRecentMsgNum[tempUserId]=serialNum;
                    cout<<"Pinging PID:"<<serialNum<<endl;
                    vectorIter+=2;
                    break;
                }
                case 5 : // this can be for sprecial powerups
                {
                    cout<<"This case is for powerups"<<endl;
                    break;
                }
                default:
                {
                    cout<<"Invalid instruction code"<<endl;
                    break;
                }
            }

            if(instCode==0)// i.e the istruction received is an acknowledgement, so it is not followed by previous crucial informations
                break;
        }
    }
}

void calculateRTT(int packetId) // should be called when ack of a message is received
                                        // Here we have 2 choices . To determine when a packet is received and then calculate RTT
                                        // or to calculate RTT for all acks received and retain the smallest RTT
{
    receiveTime[packetId]= time(&globalTimer);
    RTT[packetId]=receiveTime[packetId]-sendTime[packetId];
   // if(RTT[packetId]>maxRTT)
     //   cout<<"\n\nRTT EXCEEDING REQUISITE TIME LIMIT FOR PACKET "<<packetId<<"\n\n";
}

void shootBullet(float XCord,float YCord,float angleXaxis,float velocity)
{
    bulletShot[numBullets].centreBulletX = XCord;          /// make it leave from the canon's end point
    bulletShot[numBullets].centreBulletY = YCord;
                /// make it leave from the canon's end point
    bulletShot[numBullets].angleBullet = angleXaxis;
    (bulletShot[numBullets].myBullet).setPosition(XCord,YCord);
    (bulletShot[numBullets].myBullet).setRotation(angleXaxis*180.0/Pi);
    (bulletShot[numBullets].circle).setPosition(XCord,YCord);
    time(&bulletShot[numBullets].lifeTime);
    bulletShot[numBullets].isActive=true;
    numBullets++;

    /// now i need to PUSH it in the vector of bullets
}

