
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
    string multicast_ip;
    int multicast_port;
    string eth_name;
}Robot_Info;

class Robot{
    private:
        string name;
        string id;
        string manufacture;
        string type;
        Interface* interface;
        string multicast_ip;
        int multicast_port;
        string eth_name;
    public:
        Robot();
        ~Robot(){};
        Robot(Robot_Info*);
       
        string getName();
        string getId();
        string getManufacture();
        string getType();
        string getMulticast_ip();
        string getEth_name();
        int getMulticast_port();
        Interface* getInterface();
        void setInterface(Interface* interface);
        void setInfo(Robot_Info*);
                       

};
#endif