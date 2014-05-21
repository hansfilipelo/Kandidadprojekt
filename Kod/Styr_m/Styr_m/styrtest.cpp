#ifdef __APPLE__
#define TESTING 1
#endif

#include <stdio.h>
#include <iostream>
#include <ctime>

#include "MapSection.h"
#include "Map.h"
#include "Abstraction.h"
#include "Communication.h"
#include "slave.h"
#include "queueAtmel.h"
#include "node.h"
//#include <queue>

using namespace std;
/*
const int n=32; // horizontal size of the map
const int m=17; // vertical size size of the map
const int dir = 4;
static int dx[dir]={1, 0, -1, 0};
static int dy[dir]={0, 1, 0, -1};
*/

/*
 bool operator<(const node & a, const node & b)
 {
 return a.getPriority() > b.getPriority();
 }*/

int main(){
    
    
    //Initiate
    Map* mom = new Map();
    mom->initMap();
    Slave* testSlave = new Slave();
    Communication* testComm = new Communication(testSlave);
    Robot* testRobot = new Robot(16,2,mom, testComm);
    testComm->setRobot(testRobot);
    
    
    //filltest
    mom->printMap();
    
    cout << endl;
    
    mom->getPos(31,16)->cancer();
    
    cout << endl;
    
    mom->printMap();
    
    /*
     // queueAtmel-testing
     
     queueAtmel testQueue;
     
     node* sjua = new node(0,0,0,7);
     node* nolla = new node(0,0,0,0);
     node* etta = new node(0,0,0,1);
     node* tvaa = new node(0,0,0,2);
     node* trea = new node(0,0,0,3);
     
     
     cout << " Size: " << testQueue.Asize() << endl;
     testQueue.Apush(*nolla);
     cout << " Size: " << testQueue.Asize() << endl;
     testQueue.Apush(*etta);
     cout << " Size: " << testQueue.Asize() << endl;
     testQueue.Apush(*tvaa);
     cout << " Size: " << testQueue.Asize() << endl;
     testQueue.Apush(*sjua);
     cout << " Size: " << testQueue.Asize() << endl;
     testQueue.Apush(*trea);
     cout << " Size: " << testQueue.Asize() << endl;
     
     
     while (!testQueue.Aempty()) {
     cout << testQueue.Atop().getPriority() << endl;
     testQueue.Apop();
     cout << " Size: " << testQueue.Asize() << endl;
     }*/
    
    /*
     priority_queue<node> pq;
     
     cout << " Size pq: " << pq.size() << endl;
     pq.push(*nolla);
     cout << " Size: " << pq.size() << endl;
     pq.push(*etta);
     cout << " Size: " << pq.size() << endl;
     pq.push(*tvaa);
     cout << " Size: " << pq.size() << endl;
     pq.push(*trea);
     cout << " Size: " << pq.size() << endl;
     pq.push(*sjua);
     
     while (!pq.empty()) {
     cout << pq.top().getPriority() << endl;
     pq.pop();
     cout << " Size: " << pq.size() << endl;
     }*/
    
    /*
    // Map testing
    // -----------------------------
    cout << "--------" << " Printing map " << "--------" << endl;
    mom->convertToPathFinding();
    mom->printPathMap();
    cout << endl;
    
    srand(time(NULL));
    
    
    // randomly select start and finish locations
    
    int xA = 1;
    int yA = 1;
    int xB = 10;
    int yB = 10;
    
    
    switch(rand()%8)
    {
        case 0: xA=0;yA=0;xB=n-1;yB=m-1; break;
        case 1: xA=0;yA=m-1;xB=n-1;yB=0; break;
        case 2: xA=n/2-1;yA=m/2-1;xB=n/2+1;yB=m/2+1; break;
        case 3: xA=n/2-1;yA=m/2+1;xB=n/2+1;yB=m/2-1; break;
        case 4: xA=n/2-1;yA=0;xB=n/2+1;yB=m-1; break;
        case 5: xA=n/2+1;yA=m-1;xB=n/2-1;yB=0; break;
        case 6: xA=0;yA=m/2-1;xB=n-1;yB=m/2+1; break;
        case 7: xA=n-1;yA=m/2+1;xB=0;yB=m/2-1; break;
    }
    
    
    
    cout<<"Map Size (X,Y): "<<n<<","<<m<<endl;
    cout<<"Start: "<<xA<<","<<yA<<endl;
    cout<<"Finish: "<<xB<<","<<yB<<endl;
    
    // get the route
    clock_t start = clock();
    mom->aStar(xA, yA, xB, yB);
    
    //if(route=="") cout<<"An empty route generated!"<<endl;
    clock_t end = clock();
    double time_elapsed = double(end - start);
    cout<<"Time to calculate the route (ms): "<<time_elapsed<<endl;
    
    cout<<"["<< mom->pathArray<<"]"<<endl;
    
    int sizeOfArray = int(mom->pathArray[0]) - 48;
    string route;
    
    cout << sizeOfArray << endl;
    
    for (int i=1;i < sizeOfArray+1 ; i++) {
        
        char p = mom->pathArray[i];
        
        char c;
        if(p== 'l'){
            c = '0';
        }
        else if(p== 'f'){
            c = '1';
        }
        else if(p== 'r'){
            c = '2';
        }
        else{
            c = '3';
        }
        route = route+c;
    }
    cout<< route<<endl;
    
    // follow the route on the map and display it
    if(route.length()>0)
    {
        int j; char c;
        int x=xA;
        int y=yA;
        mom->pathMap[x][y]=2;
        for(int i=0;i<(int)route.length();i++)
        {
            c =route.at(i);
            j=atoi(&c);//converts string to int
            x=x+dx[j];
            y=y+dy[j];
            mom->pathMap[x][y]=3;
        }
        mom->pathMap[x][y]=4;
        
        // display the map with the route
        for(int y=0;y<m;y++)
        {
            for(int x=0;x<n;x++)
                if(mom->pathMap[x][y]==0)
                    cout<<".";
                else if(mom->pathMap[x][y]==1)
                    cout<<"O"; //obstacle
                else if(mom->pathMap[x][y]==2)
                    cout<<"S"; //start
                else if(mom->pathMap[x][y]==3)
                    cout<<"R"; //route
                else if(mom->pathMap[x][y]==4)
                    cout<<"F"; //finish
            cout<<endl;
        }
    }
    */
    //getchar();
    
    delete testRobot;
    delete mom;
	return 0;
}
