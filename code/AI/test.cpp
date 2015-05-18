#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <unistd.h>
 #include <netdb.h>
 #include <sys/socket.h>
 #include <math.h>
using namespace std;
time_t  globalTimer;
int main()
{
            //	system("cd AI/");
                                system("./tanktrouble");
/*    int x=1,y=1;
    int x2=1,y2=2;
    float angle = y2-y;
    angle/=(x2-x);
    cout<<"Angle "<<angle<<endl;
    angle=atan(angle);
    cout<<"Angle "<<angle<<endl;


    char temp[1024];

    gethostname(temp,1024);
    cout<<"Hi:"<<temp<<endl;
    struct hostent *host = gethostbyname(temp);
    gethostbyname(temp);

    if(host == NULL)
    {
        cout<<"null"<<endl;
      return 0;
    }
    char b1[10],b2[10],b3[10],b4[10];
    cout<<"j:"<<host-><<endl;
    //Obtain the computer's IP

    b1 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1;
    b2 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2;
    b3 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3;
    b4 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b4;

*/




    time(&globalTimer);
    cout<<globalTimer<<endl;
    char temp[10];
    int temp2;
    //cin>>temp2;
    time_t t2;
    for(int i=0;i<900000000;i++);
    sprintf(temp,"%d",temp2);
    string temp3 =temp;
    vector<string> temp4;
    temp4.push_back(temp);
    cout<<"!!"<<endl;
        time(&t2);
        double t=difftime(t2,globalTimer);
    cout<<"1:"<<globalTimer<<endl;
    cout<<"2:"<<t<<endl;
    t=difftime(globalTimer,t2);
    cout<<"2:"<<t<<endl;
cout<<"3:"<<t2<<endl;
    return 0;
}
