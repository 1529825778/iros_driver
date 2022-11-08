#include "Device.h"
#include "Robot.h"
PayLoad_Device::PayLoad_Device(Device_Info* device_info){
    this->name = device_info->name;
    this->id = device_info->id;

    this->manufacture = device_info->manufacture;
    this->type = device_info->type;

}

string PayLoad_Device::getName(){
    return this->name;
}

string PayLoad_Device::getId(){
    return this->id;
}

string PayLoad_Device::getManufacture(){
    return this->manufacture; 
}
string PayLoad_Device::getType(){
    return this->type;
}

Robot* PayLoad_Device::getRobot(){
    return this->robot;
}
void PayLoad_Device::setRobot(Robot* robot){
    this->robot = robot;
}