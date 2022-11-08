
#ifndef Robot_H
#define Robot_H
#include<string>
#include"Interface.h"

using namespace std;

typedef struct Robot_Info {
    string name;
    string id;
    string manufacture;
    string type;
}Robot_Info;

class Robot{
    private:
        string name;
        string id;
        string manufacture;
        string type;
        Interface* interface;
    public:
        Robot(){};
        ~Robot(){};
        Robot(Robot_Info*);
       
        string getName();
        string getId();
        string getManufacture();
        string getType();
        Interface* getInterface();
        void setInterface(Interface* interface);
                       

};
#endif