#include <string>

using namespace std;

class Map;

class MapSection{
public:
    MapSection(int x,int y);
    MapSection(int x,int y, string createType);
	void setX(int coord);
	void setY(int coord);
	void setType(string newType);
	void setRobot(bool status);
	
	int getX();
	int getY();
	string getType();
	bool isRobot();
	
	MapSection* getTop();
	MapSection* getBottom();
	MapSection* getLeft();
	MapSection* getRight();
	
	void setTop(MapSection*);
	void setBottom(MapSection*);
	void setLeft(MapSection*);
	void setRight(MapSection*);
	
	bool isUnexplored();
	int findUnexplored();
	
private:
	int xCoord;
	int yCoord;
	int step;
	string type="undefined";
	bool containsRobot = false;
	Map* mom = nullptr;
	string next;
	

};