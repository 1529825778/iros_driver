#include "Robot.h"


Robot::Robot(){
    this->interface = new Interface;
}
Robot::Robot(Robot_Info* robot_info){
    this->name = robot_info->name;
    this->id = robot_info->id;
    this->manufacture= robot_info->manufacture;
    this->type = robot_info->type;
    this->multicast_ip = robot_info->multicast_ip;
    this->multicast_port = robot_info->multicast_port;
    this->eth_name = robot_info->eth_name;
                  
}

string Robot::getName(){
    return this->name;
}

string Robot::getId(){
    return this->id;
}

string Robot::getManufacture(){
    return this->manufacture;
}

string Robot::getType(){
    return this->type;
}

Interface* Robot::getInterface(){
    return this->interface;
}
void Robot::setInterface(Interface* interface){
    this->interface = interface;
}
string Robot::getMulticast_ip(){
    return this->multicast_ip;
}
int Robot::getMulticast_port(){
    return this->multicast_port;
}
string Robot::getEth_name(){
    return this->eth_name;
}

void Robot::setInfo(Robot_Info* robot_info){
    this->name = robot_info->name;
    this->id = robot_info->id;
    this->manufacture= robot_info->manufacture;
    this->type = robot_info->type;
    this->multicast_ip = robot_info->multicast_ip;
    this->multicast_port = robot_info->multicast_port;
    this->eth_name = robot_info->eth_name;

}


