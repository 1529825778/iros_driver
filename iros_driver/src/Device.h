#ifndef PayLoad_Device_H
#define PayLoad_Device_H
#include <string>
#include "common.h"
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
        PayLoad_Device(string,string,string,string);
        ~PayLoad_Device(){};
        string getName();
        string getId();
        string getManufacture();
        string getType();
        
     private:
        string name;
        string id;
        string manufacture;
        string type;    


};
#endif