#include "Device.h"
#include "Robot.h"
PayLoad_Device::PayLoad_Device(string name,string id,string manufacture,string payload_type){
    this->name = name;
    this->id = id;

    this->manufacture = manufacture;
    this->type = payload_type;

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
/*生成设备唯一标识*/
