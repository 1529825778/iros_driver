#include "Capbility.h"
Capbility::Capbility(Capbility_Info* capbility_info){
    this->resolution = capbility_info->resoultion;
    this->angle = capbility_info->angle;
    this->capbility_name = capbility_info->capbility_name;
    this->radious = capbility_info->radious;
    this->capbility_id = capbility_info->capbility_id;
    this->frequency = capbility_info->frequency;
}

 string getName();
        string getId();
        string getResoluion();
        string getRadious();
        string getAngle();
        Robot getRobot();
        void setRobot(Robot robot);
        PayLoad_Device* getDevice();
        void setDevice(PayLoad_Device* device);
        Data getData();
        void setData(Data data);

string Capbility::getName(){
    return  this->capbility_name;
}
string Capbility::getId(){
    return this->capbility_id;
}

string Capbility::getResoluion(){
    return this->resolution;
}
string Capbility::getRadious(){
    return this->radious;
}
string Capbility::getAngle(){
    return this->angle;
}

string Capbility::getFrequency(){
    return this->frequency;
}
Robot* Capbility::getRobot(){
    return this->robot;
    
}
void Capbility::setRobot(Robot* robot){
    this->robot = robot;
}
Data* Capbility::getData(){
    return this->data;
}
void Capbility::setData(Data* data){
    this->data = data;
}
PayLoad_Device* Capbility::getDevice(){
    return this->payload_device;
}
void Capbility::setDevice(PayLoad_Device* device){
    this->payload_device = device;
}

