#include "Robot.h"



Robot::Robot(Robot_Info robot_info){
    this->name = robot_info.name;
    this->id = robot_info.id;
    this->manufacture= robot_info.manufacture;
    this->type = robot_info.type;
                  
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


