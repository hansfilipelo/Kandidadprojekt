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
	
private:
	int xCoord;
	int yCoord;
	int size=40;
	string type="undefined";
	containsRobot = false;
	

};