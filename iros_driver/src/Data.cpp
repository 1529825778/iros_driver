#include "Data.h"
#include <map>
Data::Data(Data_Info Data_Info){
    this->data_length = Data_Info.data_length;
    this->data_type = Data_Info.data_type;
    this->data_describe = Data_Info.data_describe;
    this->data_handle_lib = data_handle_lib;
}
void Data:: setInterface(Interface interface){
    this->interface = interface;

}

Interface Data::getInterface(){
    return this->interface;
}

void Data::setParamter(map<string,string> p_map){
    this->paramter_map = p_map;
}

map<string,string> Data::getParamter(){
    return this->paramter_map;
}
