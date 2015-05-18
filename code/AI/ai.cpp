#include "ai.h"
#include <cmath>

    Game *tankTrouble;



int main(int argc, char **argv)
{
    sf::Music music;
    music.openFromFile("background.ogg");
    music.setLoop(1);
    music.play();
    tankTrouble = new Game(atoi(argv[1]));
  //  cout<<"hi/:"<<atoi(argv[1])<<endl;
    tankTrouble->run(atoi(argv[1]));
}

int myrandom (int i) { return std::rand()%i;}

void get4pointsfromcentre( int x , int y , vector<float>& xvec , vector<float>& yvec)
{
	xvec.resize(4);
	yvec.resize(4);

	float theta = (tankTrouble->computer.angleXaxis) - Pi/4;

	float diag = sqrt(pow((tankTrouble->computer.w + OFFSET),2) + pow((tankTrouble->computer.h + OFFSET),2))/2;

	//cout<<tankTrouble->computer.w<<" "<<tankTrouble->computer.h<<endl;

	float point1x = x + diag*sin(theta);
	float point1y = y - diag*cos(theta);

	theta += Pi/2;

	float point2x = x + diag*sin(theta);
	float point2y = y - diag*cos(theta);

	theta += Pi/2;

	float point3x = x + diag*sin(theta);
	float point3y = y - diag*cos(theta);

	theta += Pi/2;

	float point4x = x + diag*sin(theta);
	float point4y = y - diag*cos(theta);

	xvec[0] = point1x;
	xvec[1] = point2x;
	xvec[2] = point3x;
	xvec[3] = point4x;

	yvec[0] = point1y;
	yvec[1] = point2y;
	yvec[2] = point3y;
	yvec[3] = point4y;


}

struct MyPoint
{
    float x;
    float y;

    void disp()
    {
    	cout<<x<<" "<<y<<endl;
    }
};



// Given three colinear MyPoints p, q, r, the function checks if
// MyPoint q lies on line segment 'pr'
bool onSegment(MyPoint p, MyPoint q, MyPoint r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
       return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(MyPoint p, MyPoint q, MyPoint r)
{
    // See 10th slides from following link for derivation of the formula
    // http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool doIntersect(MyPoint p1, MyPoint q1, MyPoint p2, MyPoint q2)
{
    // Find the four orientations needed for general and
    // special cases


    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
    float p2_x, float p2_y, float p3_x, float p3_y, float i_x, float i_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            i_y = p0_y + (t * s1_y);
        return 1;
    }

    return 0; // No collision
}

bool iswall ( int x1 , int y1 )
{
    // Please code this up.
    for (vector <vector <float> > :: iterator itMaze = quad1.begin(); itMaze != quad1.end(); ++itMaze)
    {
        //hori wall
        if ((*itMaze)[1] == (*itMaze)[3])
        {
            if (y1 == int((*itMaze)[1]))
                return 1;
        }
        else if ((*itMaze)[1] == (*itMaze)[3])
        {
            if (x1 == int((*itMaze)[0]))
                return 1;
        }
        //vert wall
    }
    return 0;
}

float distance(int x1 , int x2 , int y1 , int y2)
{
	return sqrt(pow((x1-x2),2) + pow((y1-y2),2));
}

/// To check if the centres of the two tanks are in line of sight.

bool inlineofsight( int x1 , int x2 , int y1, int y2)
{

  MyPoint tankbeg; tankbeg.x = x1; tankbeg.y = y1;
  MyPoint tankend; tankend.x = x2; tankend.y = y2;

	for (int i=0; i < tankTrouble->maze1.size(); ++i)
    {
        MyPoint wallbeg, wallend;
        wallbeg.x = tankTrouble->maze1[i][0];
        wallbeg.y = tankTrouble->maze1[i][1];

        wallend.x = tankTrouble->maze1[i][2];
        wallend.y = tankTrouble->maze1[i][3];

        // Now to check if the walls and the tank boundaries intersect or not.

        if(get_line_intersection(tankbeg.x ,tankbeg.y , tankend.x , tankend.y , wallbeg.x , wallbeg.y , wallend.x , wallend.y, 0 , 0) == 1)
        	return false;
    }

    return true;
}


bool inboundstank( int x1 , int y1 )
{
    // Please complete this.
    // Get corner point of the tank in a vector.
    // The co-ordinates of the tank right now at this alignment are in a vector say : vector<<pair<float,float>> TankCoord

	vector<float> xvec;
	vector<float> yvec;

	 get4pointsfromcentre( x1 , y1 , xvec , yvec);


	for(int i=0 ; i<4 ; i++)
	{

		int p1 = i;
		int p2 = (i+1)%4;
		MyPoint tankbeg, tankend;



		tankbeg.x = xvec[p1];
		tankbeg.y = yvec[p1];

		tankend.x = xvec[p2];
		tankend.y = yvec[p2];

		//cout<<tankbeg.x<<" "<<tankbeg.y<<endl;
		//cout<<tankend.x<<" "<<tankend.y<<endl;

		for (int i=0; i < tankTrouble->maze1.size(); ++i)
	    {
	        MyPoint wallbeg, wallend;
	        wallbeg.x = tankTrouble->maze1[i][0];
	        wallbeg.y = tankTrouble->maze1[i][1];

	        wallend.x = tankTrouble->maze1[i][2];
	        wallend.y = tankTrouble->maze1[i][3];

	        // Now to check if the walls and the tank boundaries intersect or not.

	        if(get_line_intersection(tankbeg.x ,tankbeg.y , tankend.x , tankend.y , wallbeg.x , wallbeg.y , wallend.x , wallend.y, 0 , 0) == 1)
	        {
	        	//cout<<"Intersection Found"<<endl;
	        	//tankbeg.disp();
	        	//tankend.disp();
	        	//wallbeg.disp();
	        	//wallend.disp();

	        	return false;
	    	}
	    }
	}

	return true;

}

int finx , finy; // Destination ( If I need it )

Priority_Queue::Priority_Queue()
{
    front = NULL;
}

void Priority_Queue::insert(Node* item, float priority)
{
    node *tmp, *q;
    tmp = new node;
    tmp->info = item;
    tmp->priority = priority;

    if (front == NULL || priority < front->priority)
    {
        tmp->link = front;
        front = tmp;
    }

    else
    {
        q = front;
        while (q->link != NULL && q->link->priority <= priority)
            q=q->link;
        tmp->link = q->link;
        q->link = tmp;
    }
}

Node* Priority_Queue::pop()
{
    node *tmp;

    if(front == NULL)
        cout<<"Queue Underflow\n";
    else
    {
        tmp = front;
       // cout<<"Deleted item is: "<<tmp->info<<endl;
        front = front->link;
        Node* usefulinfo = tmp->info;
        //free(tmp);
        return usefulinfo;
    }
}
void Priority_Queue::display()
{
    node *ptr;
    ptr = front;
    if (front == NULL)
        cout<<"Queue is empty\n";
    else
    {
        cout<<"Queue is :\n";
        cout<<"Priority       Item\n";
        while(ptr != NULL)
        {
            cout<<ptr->priority<<"                 "<<ptr->info<<endl;
            ptr = ptr->link;
        }
    }
}

// vector<pair<int , int>> randomWalk (int x1 , int x2 , int y1 , int y2)   // A Star
// {

//     // Must Read : It returns a vector of "INTEGER" co-ordinates to be visited in order to get to the final location

//     set<pair<int,int>> visitednodes;
//     vector<pair<int,int>> retvec;
//     //        pair<int,int> foo1 = make_pair(1,1);
//       //          retvec.push_back(foo1);
//         //return retvec;

//   //  cout<<"In coord : "<<x1<<" "<<y1<<"    "<<x2<<" "<<y2<<endl;

//     Priority_Queue NodeQueue;
//     Node* start = new Node;
//     start -> x = x1;
//     start -> y = y1;
//     start -> Parent = NULL;

//   //  Node* parentnow = start;
//   //  vector<pair<int , int>> retvec;

//     int count =0;

//     while(count < 22)
//     {
//         count++;



//         Node* present = NodeQueue.pop();
//         //cout<<present ->x <<" "<<present ->y<<endl;
//         if((present ->x ==x2) && ( present -> y ==y2) )
//         {
//             // Path has been found.
//             while( (present -> Parent) != NULL)
//             {
//                 pair<int,int> foo;
//                 foo = make_pair(present->x , present -> y);
//                 retvec.push_back(foo);
//                 present = present -> Parent;
//             }
//             reverse(retvec.begin(),retvec.end());
//             cout<<"Returning"<<endl;
//             return retvec;
//         }

//         pair<int,int> uppa,downpa,lefpa,rightpa;

//         Node* up = new Node;
//         up -> x = (present -> x) +1;
//         up -> y = (present -> y) ;
//         up -> Parent = present;

//         Node* down = new Node;
//         down -> x = (present -> x) - 1;
//         down -> y = (present -> y) ;
//         down -> Parent = present;

//         Node* right = new Node;
//         right -> x = (present -> x) ;
//         right -> y = (present -> y) +1 ;
//         right -> Parent = present;

//         Node* left = new Node;
//         left -> x = (present -> x) ;
//         left -> y = (present -> y) -1 ;
//         left -> Parent = present;
//         priority = abs(left->x - x2) + abs(left->y - y2);

//         set<int,int>::iterator it;

//         uppa = make_pair(up->x , up -> y);
//         downpa = make_pair(down->x , down -> y);
//         lefpa = make_pair(left->x , left -> y);
//         rightpa = make_pair(right->x , right -> y);

//          int priorityup , priorityright , prioritylef , prioritydown;
//         // float distup = distance(up->x,x2,up->y,y2) , distdown = distance(down->x,x2,down->y,y2) , distleft = distance(left->x,x2,left->y,y2) , distright = distance(right->x,x2,right->y,y2) ;

//      priorityup = abs(up->x - x2) + abs(up->y - y2);
//          priorityright = abs(right->x - x2) + abs(right->y - y2);
//        prioritylef = abs(up->x - x2) + abs(up->y - y2);
//        prioritydown = abs(down->x - x2) + abs(down->y - y2);


//         {
//            if( inboundstank(up->x , up->y) )      // Make Changes Here!!!!!!!
//            {
//               // cout<<"Pushing"<<endl;
//                if(visitednodes.find(uppa) == visitednodes.end())
//                {
//                      NodeQueue.insert( up , priorityup /*+ distup*/);
//                      visitednodes.insert(uppa);
//                }
//            }
//        }
//          if( inboundstank(right->x , right ->y) )      // Make Changes Here!!!!!!!
//          {
//            //cout<<"Pushing"<<endl;
//              if(visitednodes.find(rightpa) == visitednodes.end())
//              {
//                    NodeQueue.insert( right , priorityright /*+ distright*/);
//                    visitednodes.insert(rightpa);
//              }
//          }


//          if( inboundstank(down->x , down ->y) )      // Make Changes Here!!!!!!!
//          {
//            //cout<<"Pushing"<<endl;
//              if(visitednodes.find(downpa) == visitednodes.end())
//              {
//                    NodeQueue.insert( down , prioritydown /*+ distdown*/);
//                    visitednodes.insert(downpa);
//              }
//          }


//         if( inboundstank(left->x , left ->y) )      // Make Changes Here!!!!!!!
//         {
//           //cout<<"Pushing"<<endl;
//              if(visitednodes.find(lefpa) == visitednodes.end())
//              {
//                    NodeQueue.insert( left , prioritylef /*+ distleft*/);
//                    visitednodes.insert(lefpa);
//              }
//         }


//     }

// }



vector<pair<int , int>> AStar (int x1 , int x2 , int y1 , int y2)   // A Star
{

    // Must Read : It returns a vector of "INTEGER" co-ordinates to be visited in order to get to the final location



    set<pair<int,int>> visitednodes;
    vector<pair<int,int>> retvec;
    //        pair<int,int> foo1 = make_pair(1,1);
      //          retvec.push_back(foo1);
        //return retvec;

  //  cout<<"In coord : "<<x1<<" "<<y1<<"    "<<x2<<" "<<y2<<endl;

    Priority_Queue NodeQueue;
    Node* start = new Node;
    start -> x = x1;
    start -> y = y1;
    start -> Parent = NULL;

  //  Node* parentnow = start;
  //  vector<pair<int , int>> retvec;

    int priority = abs(x1 - x2) + abs(y1 - y2);
    NodeQueue.insert(start , priority);
    while(1)
    {

        if(NodeQueue.front == NULL)     // If PQ is Empty (Just in Case)
        {
            return retvec;
        }


        Node* present = NodeQueue.pop();
        //cout<<present ->x <<" "<<present ->y<<endl;
        if((present ->x ==x2) && ( present -> y ==y2) )
        {
            // Path has been found.
            while( (present -> Parent) != NULL)
            {
                pair<int,int> foo;
                foo = make_pair(present->x , present -> y);
                retvec.push_back(foo);
                present = present -> Parent;
            }
            reverse(retvec.begin(),retvec.end());
            cout<<"Returning"<<endl;
            return retvec;
        }

        pair<int,int> uppa,downpa,lefpa,rightpa;

        Node* up = new Node;
        up -> x = (present -> x) +1;
        up -> y = (present -> y) ;
        up -> Parent = present;

        Node* down = new Node;
        down -> x = (present -> x) - 1;
        down -> y = (present -> y) ;
        down -> Parent = present;

        Node* right = new Node;
        right -> x = (present -> x) ;
        right -> y = (present -> y) +1 ;
        right -> Parent = present;

        Node* left = new Node;
        left -> x = (present -> x) ;
        left -> y = (present -> y) -1 ;
        left -> Parent = present;
        priority = abs(left->x - x2) + abs(left->y - y2);

        set<int,int>::iterator it;

        uppa = make_pair(up->x , up -> y);
        downpa = make_pair(down->x , down -> y);
        lefpa = make_pair(left->x , left -> y);
        rightpa = make_pair(right->x , right -> y);

         int priorityup , priorityright , prioritylef , prioritydown;
        // float distup = distance(up->x,x2,up->y,y2) , distdown = distance(down->x,x2,down->y,y2) , distleft = distance(left->x,x2,left->y,y2) , distright = distance(right->x,x2,right->y,y2) ;

		 priorityup = abs(up->x - x2) + abs(up->y - y2);
         priorityright = abs(right->x - x2) + abs(right->y - y2);
  		 prioritylef = abs(up->x - x2) + abs(up->y - y2);
  		 prioritydown = abs(down->x - x2) + abs(down->y - y2);

  		 // if(priorityup == prioritydown)
  		 // {
  		 // 	prioritydown+=34;
  		 // }

  		 // if(prioritylef == priorityright)
  		 // {
  		 // 	priorityright+=23;
  		 // }

  		 // if(priorityup == priorityright)
  		 // {
  		 // 	priorityright+=45;
  		 // }

  		 // if(priorityup == prioritylef)
  		 // {
  		 // 	prioritylef+=21;
  		 // }

  		 // if(prioritylef == prioritydown)
  		 // {
  		 // 	prioritydown+=12;
  		 // }

  		 // if(priorityright == prioritydown)
  		 // {
  		 // 	prioritydown+=15;
  		 // }

         if( inboundstank(up->x , up->y) )      // Make Changes Here!!!!!!!
         {
            // cout<<"Pushing"<<endl;
             if(visitednodes.find(uppa) == visitednodes.end())
             {
                   NodeQueue.insert( up , priorityup /*+ distup*/);
                   visitednodes.insert(uppa);
             }
         }

         if( inboundstank(right->x , right ->y) )      // Make Changes Here!!!!!!!
         {
         	 //cout<<"Pushing"<<endl;
             if(visitednodes.find(rightpa) == visitednodes.end())
             {
                   NodeQueue.insert( right , priorityright /*+ distright*/);
                   visitednodes.insert(rightpa);
             }
         }


         if( inboundstank(down->x , down ->y) )      // Make Changes Here!!!!!!!
         {
         	 //cout<<"Pushing"<<endl;
             if(visitednodes.find(downpa) == visitednodes.end())
             {
                   NodeQueue.insert( down , prioritydown /*+ distdown*/);
                   visitednodes.insert(downpa);
             }
         }


        if( inboundstank(left->x , left ->y) )      // Make Changes Here!!!!!!!
        {
         	//cout<<"Pushing"<<endl;
             if(visitednodes.find(lefpa) == visitednodes.end())
             {
                   NodeQueue.insert( left , prioritylef /*+ distleft*/);
                   visitednodes.insert(lefpa);
             }
        }


    }

}
