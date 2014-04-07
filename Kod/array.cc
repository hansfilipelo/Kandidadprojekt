
#include <fstream>      // std::ofstream
#include <iostream>
#include <string.h>

using namespace std;
int main () {
    
    unsigned char* bla = (unsigned char*)"blabla";
    
    unsigned char data[10] ={'4','B','C','D','E'};
    int s = strlen((char*)data);
    cout << "Antal använda tecken: " << s << endl;
    
    
    cout << "Max storlek på array: " << sizeof(data) << endl;
    
    return 0;
}