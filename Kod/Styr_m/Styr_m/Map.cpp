
#include "Map.h"
#include "MapSection.h"

using namespace std;

// ------------- Construct -----------------

Map::Map(){
	
	// Create 32 X by 17 Y
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            mapArea[i][it] = new MapSection(i,it,this); // it, i to i, it
        }
    }
}

// -----------------------------------------

Map::~Map(){
    
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            delete mapArea[i][it];
        }
    }
}

bool Map::getVisited(int xPos,int yPos){
    return mapArea[xPos][yPos]->isVisited;
}


void Map::setVisited(int xPos,int yPos,bool value){
    mapArea[xPos][yPos]->isVisited = value;
}


// ------------------------------------------

MapSection* Map::getPos(int x, int y){
    return mapArea[x][y];
}

// ------------------------------------------

void Map::setSection(int xPos, int yPos, MapSection* inSection){
    mapArea[xPos][yPos] = inSection;
}

// ------------------------------------------

void Map::convertSection(int xPos, int yPos, char inType){
	this->getPos(xPos,yPos)->setType(inType);
}

// ---------------- getColAsChar ------------
/*
char* Map::getColAsChar(int col)
{
	// Char sent to communications unit
	char* output = new char[25];
	// String telling type of the object we are interested in.
    
    // Abstraction for buss communications
    // Sending 19 positions of interest
    int crap = 19;
    output[0] = 23;
    // Sending Map data command
    output[1] = 'M';
    // Sending column number
    output[2] = col;
    
	for (int it = 0; it < 17; it++){
        // Type of the block we are looking at
        output[it+3] = this->getPos(col,it)->getType();
	}
    return output;
}
*/


void Map::getColAsChar(int col)
{
	
	
	// Sending 19 positions of interest
	colArray[0] = 23;
	// Sending Map data command
	colArray[1] = 'M';
	// Sending column number
	colArray[2] = col;

	for (int it = 0; it < 17; it++)
	{
		// Type of the block we are looking at
		colArray[it+3] = this->getPos(col,it)->getType();
	}
}


// Fills closed area if closed

void Map::fillClosedArea()
{
	bool firstC = false;
    bool eAfterC = false;
	// fill map with closed area from left
    for (int y = 0; y < 17; y++) {
		for (int x = 0; x < 32; x++) {
            
			if((this->getPos(x,y)->getType() != 'c' && !firstC)){
                convertSection(x,y,'c');
			}
            else if ( this->getPos(x,y)->getType() == 'c' && !eAfterC ){
                firstC = true;
            }
            else if ( this->getPos(x,y)->getType() == 'e' && firstC) {
                eAfterC = true;
            }
            else if ( eAfterC && this->getPos(x,y)->getType() == 'c' ) {
                eAfterC = false;
                firstC = false;
            }
            else if ( !eAfterC && this->getPos(x,y)->getType() == 'u' ){
                convertSection(x,y,'c');
            }
        }
        
        firstC = false;
        eAfterC = false;
    }
}
//-----------------------------------
// Checks if coordinates within map

bool Map::withinMap(int xPos, int yPos){
    if (xPos < 0 || xPos > 31 || yPos < 0 || yPos > 16) {
        return false;
    }
    else {
        return true;
    }
}


// -------------------- TESTING --------------------

#if TESTING == 1
void Map::printMap(){
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            cout << this->getPos(i,it)->getType() << " ";
        }
        cout << endl;
    }
}

void Map::printPathMap(){
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            cout << pathMap[i][it] << " ";
        }
        cout << endl;
    }
}

void Map::initMap(){
    char tempMap[32][17]={
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','c','c','c','c','c','c','c','c','c','c','c','c','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','c','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
        {'u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u','u'},
    };
    
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            mapArea[i][it] = new MapSection(i,it,this); // it, i to i, it
            
            if (tempMap[i][it] == 'c') {
                this->getPos(i,it)->setType('c');
            }
            if (tempMap[i][it] == 'e') {
                this->getPos(i,it)->setType('e');
            }
        }
    }
}

void Map::convertToPathFinding(){
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            if (this->getPos(i,it)->getType() == 'c') {
                pathMap[i][it] = 1;
            }
            else{
                pathMap[i][it] = 0;
            }
        }
    }
}


//operator override is needed for A-star

bool operator<(const node & a, const node & b)
{
    return a.getPriority() > b.getPriority();
}

// A-star algorithm.
// The route returned is a string of direction digits.
void Map::aStar(const int & xStart, const int & yStart,const int & xFinish, const int & yFinish)
{
    const int n=32; // horizontal size of the map
    const int m=17; // vertical size size of the map
    static int closed_nodes_map[n][m]; // map of closed (tried-out) nodes
    static int open_nodes_map[n][m]; // map of open (not-yet-tried) nodes
    static int dir_map[n][m]; // map of directions
    const int dir=4; // number of possible directions to go at any position
    static int dx[dir]={1, 0, -1, 0};
    static int dy[dir]={0, 1, 0, -1};
    
    static priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
    static int pqi; // pq index
    static node* n0;
    static node* m0;
    static int i, j, x, y, xdx, ydy;
    static int c;
    pqi=0;
    
    // reset the node maps
    for(y=0;y<m;y++)
    {
        for(x=0;x<n;x++)
        {
            closed_nodes_map[x][y]=0;
            open_nodes_map[x][y]=0;
        }
    }
    
    // create the start node and push into list of open nodes
    n0=new node(xStart, yStart, 0, 0);
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    open_nodes_map[x][y]=n0->getPriority(); // mark it on the open nodes map
    
    // A* search
    while(!pq[pqi].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        n0=new node( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
                    pq[pqi].top().getLevel(), pq[pqi].top().getPriority());
        
        x=n0->getxPos(); y=n0->getyPos();
        
        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map[x][y]=0;
        // mark it on the closed nodes map
        closed_nodes_map[x][y]=1;
        
        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if(x==xFinish && y==yFinish)
        {
            int counter = 0;
            
            while(!(x==xStart && y==yStart))
            {
                j=dir_map[x][y];
                c=(int)(j+dir/2)%dir;
                
                switch (c) {
                    case 0 : pathArray[counter] = 'h';
                        break;
                    case 1 : pathArray[counter] = 'b';
                        break;
                    case 2 : pathArray[counter] = 'v';
                        break;
                    case 3 : pathArray[counter] = 'f';
                        break;
                    default:
                        break;
                }
                x+=dx[j];
                y+=dy[j];
                counter++;
            }
            
            int size = counter + 1;
            char reversedArray[size];
            char charSize = 48 + counter;
            reversedArray[0]=charSize;
            for (int k=1; k < size; k++){
                reversedArray[size-k] = pathArray[k-1];
            }

            memcpy(pathArray,reversedArray,size);
            // garbage collection
            delete n0;
            // empty the leftover nodes
            while(!pq[pqi].empty()) pq[pqi].pop();
            return;
        }
        
        // generate moves (child nodes) in all possible directions
        for(i=0;i<dir;i++)
        {
            xdx=x+dx[i]; ydy=y+dy[i];
            
            if(!(xdx<0 || xdx>n-1 || ydy<0 || ydy>m-1 || pathMap[xdx][ydy]==1
                 || closed_nodes_map[xdx][ydy]==1))
            {
                // generate a child node
                m0=new node( xdx, ydy, n0->getLevel(),
                            n0->getPriority());
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);
                
                // if it is not in the open list then add into that
                if(open_nodes_map[xdx][ydy]==0)
                {
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    pq[pqi].push(*m0);
                    // mark its parent node direction
                    dir_map[xdx][ydy]=(i+dir/2)%dir;
                }
                else if(open_nodes_map[xdx][ydy]>m0->getPriority())
                {
                    // update the priority info
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    // update the parent direction info
                    dir_map[xdx][ydy]=(i+dir/2)%dir;
                    
                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(!(pq[pqi].top().getxPos()==xdx &&
                            pq[pqi].top().getyPos()==ydy))
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop(); // remove the wanted node
                    
                    // empty the larger size pq to the smaller one
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0); // add the better node instead
                }
                else delete m0; // garbage collection
            }
        }
        delete n0; // garbage collection
    }
    pathArray[0]='F';
    return;
}

#endif