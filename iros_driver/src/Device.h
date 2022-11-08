#ifndef PayLoad_Device_H
#define PayLoad_Device_H
#include <string>
#include "Robot.h"
using namespace std;

struct Device_Info{
    string name;
    string id;
    string manufacture;
    string type;


};

class PayLoad_Device{
    public:
        PayLoad_Device(){};
        PayLoad_Device(Device_Info*);
        ~PayLoad_Device(){};
        string getName();
        string getId();
        string getManufacture();
        string getType();
        Robot* getRobot();
        void setRobot(Robot* robot);
        
        
     private:
        string name;
        string id;
        string manufacture;
        string type;  
        Robot* robot;  


};
#endif