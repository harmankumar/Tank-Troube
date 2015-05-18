#ifndef AI_H
#define AI_H


#include "helperfn.h"
#include <cstdlib>
#include <ctime>
#include "game.h"
#include <set>


#define NUMRANDOMWALK 22

int myrandom (int i);
bool iswall ( int x1 , int y1 );
bool inboundstank( int x1 , int y1 );
bool inlineofsight( float x1 , float x2 , float y1 , float y2);

extern int finx;
extern int finy; // Destination ( If I need it )

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
		Priority_Queue();
        void insert(Node* item, float priority);
        Node* pop();
        void display();
};

bool inlineofsight( int x1 , int x2 , int y1, int y2);

float distance(int x1 , int x2 , int y1 , int y2);

vector<pair<int , int>> randomWalk (int x1 , int x2 , int y1 , int y2);	// A Star

vector<pair<int , int>> AStar (int x1 , int x2 , int y1 , int y2);	// A Star


#endif
