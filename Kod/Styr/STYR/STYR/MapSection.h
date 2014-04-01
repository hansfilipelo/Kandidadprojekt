#include <string>

class MapSection{
public:
	void setX(coord);
	void setY(coord);
	void setType(newType);
	void setRobot(status);
	
	int getX();
	int getY();
	string getType();
	int getSize();
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
	void findUnexplored();
	
private:
	int xCoord;
	int yCoord;
	int size=40;
	int step;
	string type="undefined";
	containsRobot = false;
	MapSection* top = nullptr;
	MapSection* bottom = nullptr;
	MapSection* left = nullptr;
	MapSection* right = nullptr;
	string next;
	

};