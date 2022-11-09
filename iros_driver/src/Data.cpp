#include "Data.h"
#include <map>
Data::Data(Data_Info* Data_Info){
    this->data_length = Data_Info->data_length;
    this->data_type = Data_Info->data_type;
    this->data_describe = Data_Info->data_describe;
    this->data_handle_lib = Data_Info->data_handle_lib;
    this->data_formate = Data_Info->data_formate;
}

void Data:: setInterface(Interface* interface){
    this->interface = interface;

}

Interface* Data::getInterface(){
    return this->interface;
}

void Data::setParamter(map<string,string> p_map){
    this->paramter_map = p_map;
}

map<string,string> Data::getParamter(){
    return this->paramter_map;
}

void Data::setInfo(Data_Info* data_info){
    this->data_length = data_info->data_length;
    this->data_type = data_info->data_type;
    this->data_describe = data_info->data_describe;
    this->data_formate = data_info->data_formate;
    this->data_handle_lib = data_info->data_handle_lib;
}

string Data::getDescribe(){
    return this->data_describe;
}
string Data::getFormate(){
    return this->data_formate;
}
int Data::getLength(){
    return this->data_length;
}
string Data::getLib(){
    return this->data_handle_lib;
}
string Data::getType(){
    return this->data_type;
}
