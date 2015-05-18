#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

bool iswall ( int x1 , int y1 )
{
	// Please code this up.
}

bool inboundstank( int x1 , int y1 , float angle )
{
	// Please complete this.
}

bool inlineofsight( float x1 , float x2 , float y1 , float y2)
{
		// Checks whether there is straight line without walls connecting A and B
}

int finx , finy; // Destination ( If I need it )

struct Node
{
	int x;
	int y;
	Node* Parent;
};




struct node 	// For Priority queue

{
	int priority;
	Node* info;
	struct node *link;
};

class Priority_Queue
{
    public:
        node *front;
        Priority_Queue()
        {
            front = NULL;
        }

        void insert(Node* item, int priority)
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
        
        Node* pop()
        {
            node *tmp;
            if(front == NULL)
                cout<<"Queue Underflow\n";
            else
            {
                tmp = front;
                cout<<"Deleted item is: "<<tmp->info<<endl;
                front = front->link;
                Node* usefulinfo = tmp->info;
                //free(tmp);
                return usefulinfo;
            }
        }

        /*

         * Print Priority Queue

         */

        void display()

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

};





vector<pair<int , int>> AStar (int x1 , int x2 , int y1 , int y2)	// A Star
{

	// Must Read : It returns a vector of "INTEGER" co-ordinates to be visited in order to get to the final location
	Priority_Queue NodeQueue;
	Node* start = new Node;
	start -> x = x1;
	start -> y = y1;
	start -> Parent = NULL;

	Node* parentnow = start;
	vector<pair<int , int>> retvec;

	int priority = abs(x1 - x2) + abs(y1 - y2);
	NodeQueue.insert(start , priority);
	while(1)
	{
		Node* present = NodeQueue.pop();
		if((present ->x ==x1) && ( present -> y ==y1) )
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

			return retvec;
		}

		if(NodeQueue.front == NULL) 	// If PQ is Empty (Just in Case)
		{
			return retvec;
		}

		Node* up = new Node;
		up -> x = (present -> x) +1;
		up -> y = (present -> y) ;
		up -> Parent = present;
		priority = abs(up->x - x2) + abs(up->y - y2);
		if( !iswall(up->x , up ->y) /* && (inboundstank(up->x, up->y , pi/2)) */ )		// Make Changes Here!!!!!!!
		{
			NodeQueue.insert( up , priority);
		}

		Node* down = new Node;
		down -> x = (present -> x) - 1;
		down -> y = (present -> y) ;
		down -> Parent = present;
		priority = abs(down->x - x2) + abs(down->y - y2);
		if( !iswall(down->x , down ->y) /* && (inboundstank()) */ )		// Make Changes Here!!!!!!!
		{
			NodeQueue.insert( down , priority);
		}

		Node* right = new Node;
		right -> x = (present -> x) ;
		right -> y = (present -> y) +1 ;
		right -> Parent = present;
		priority = abs(right->x - x2) + abs(right->y - y2);
		if(!iswall(right->x , right ->y) /* && (inboundstank()) */ )		// Make Changes Here!!!!!!!
		{
			NodeQueue.insert( right , priority);
		}

		Node* left = new Node;
		left -> x = (present -> x) ;
		left -> y = (present -> y) -1 ;
		left -> Parent = present;
		priority = abs(left->x - x2) + abs(left->y - y2);
		if(!iswall(left->x , left ->y) /* && (inboundstank()) */ )		// Make Changes Here!!!!!!!
		{
			NodeQueue.insert( left , priority);
		}

	}
}