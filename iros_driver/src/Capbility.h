
#ifndef CAPBILITY_H
#define CAPBILITY_H
#include<map>
#include"Data.h"
#include"Robot.h"
#include"Device.h"


using namespace std;

struct Capbility_Info{
    string capbility_id;
    string capbility_name;
    string resoultion;
    string radious;
    string angle;
    string frequency;
};
class Capbility{
    private:
        string capbility_name;
        string capbility_id;
        string resolution;
        string radious;
        string angle;
        string frequency;
        Robot* robot;
        PayLoad_Device* payload_device;
        Data* data;


        
    public:
        Capbility(Capbility_Info* capbility_info);
        ~Capbility();
        string getName();
        string getId();
        string getResoluion();
        string getRadious();
        string getAngle();
        string getFrequency();
        Robot* getRobot();
        void setRobot(Robot* robot);
        PayLoad_Device* getDevice();
        void setDevice(PayLoad_Device* device);
        Data* getData();
        void setData(Data* data);
        
                   

};
#endif