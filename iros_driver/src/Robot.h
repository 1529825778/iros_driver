
#ifndef Robot_H
#define Robot_H
#include<string>

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
    public:
        Robot(){};
        ~Robot(){};
        Robot(Robot_Info);
       
        string getName();
        string getId();
        string getManufacture();
        string getType();
                   

};
#endif